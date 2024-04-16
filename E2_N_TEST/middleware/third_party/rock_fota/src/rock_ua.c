// 2017-07-31 Gmobi Rock FOTA ua porting file

#include "bl_common.h"
#include "memory_map.h"
#include "hal_flash.h"
#define ROCK_FOTA_SUPPORT

#ifdef ROCK_FOTA_SUPPORT

#include "iot_rock.h"
#include "iot_rock_ipl.h"
#include <string.h>
#include <stdarg.h>

/* ROCK USER define & struct  start */
//default block = 4K
#define ROCK_DEFAULT_BLOCK_SIZE 0x10000

#define ROCK_RAM_LEN  (VRAM_LENGTH - 0x40000)
#define ROCK_RAM_BUFFER (RAM_BASE + 0x40000)

#define ROCK_CM4_ROM_BASE RTOS_BASE

#define ROCK_DELTA_BASE FOTA_RESERVED_BASE
#define ROCK_DELTA_LEN FOTA_RESERVED_LENGTH

// backup base =  the last block of delta partition
// backup len = one block size
#define ROCK_BACKUP_LEN ROCK_DEFAULT_BLOCK_SIZE
#define ROCK_BACKUP_BASE (FOTA_RESERVED_BASE - ROCK_BACKUP_LEN)

extern void bl_print_internal(bl_log_level_t level, char *fmt, va_list ap);

int rock_mismatch(void* ctx, unsigned char* buf, unsigned int start, unsigned int size, 
                            unsigned int source_hash,unsigned int target_hash) {

      return 0;
}
                            
int rock_fatal(void* ctx, int error_code) {
    return 0;
}

void rock_trace(void* ctx, const char* fmt, ...) {
    char buf[1024] = {0};
    strcat(buf, "[rock_fota]");
    strcat(buf, fmt);
    va_list     ap;
    va_start (ap, buf);

    bl_print_internal(LOG_DEBUG, buf, ap);

    va_end (ap);
}
void rock_progress(void* ctx, int percent) {
    rock_trace(ctx, "rock update progress %d\r\n", percent);
}
int rock_write_block(void* ctx, unsigned char* src, unsigned int start, unsigned int size){
    int writen = 0;
    int ret, offset;
    start -= ROM_BASE;
    offset = start;

    int block_size = 0x1000;
    int block_size_enum = get_block_size_from_address(offset);

    if (block_size_enum == HAL_FLASH_BLOCK_32K) {
        block_size = 0x8000;
    } else if (block_size_enum == HAL_FLASH_BLOCK_64K) {
        block_size = 0x10000;
    }
    // erase and write
    while(writen < size) {
       
        ret = hal_flash_erase(offset, block_size_enum);
        if(ret != 0) {
             // erase error;
            rock_trace(ctx, "hal_flash_erase error ret=%d, offset=%d\r\n", ret, offset);
            return ret;
        }

        ret = hal_flash_write(offset, src+writen, block_size);
        if(ret != 0) {
             // write error;
            rock_trace(ctx, "hal_flash_write error ret=%d, offset=%d, block_size=%d\r\n", ret, offset, block_size);
            return ret;
        }
        
        writen += block_size;
        offset += block_size;
    }

    return size;
}

int rock_read_block(void* ctx, unsigned char* dest, unsigned int start, unsigned int size){
    int ret ;
    start -= ROM_BASE;
    ret = hal_flash_read(start, dest, size);
    if(ret == 0) {
        // read success
        return size;
    }
    // read error
    rock_trace(ctx, "rock_read_block error ret=%d, offset=%x\r\n", ret, start);
    return ret;
}
int rock_read_delta(void* ctx, unsigned char* dest, unsigned int offset, unsigned int size){
    int ret = 0;
    offset -= ROM_BASE;
    ret = hal_flash_read(ROCK_DELTA_BASE+offset, dest, size);
    if(ret == 0) {
        // read success
        return size;
    }
    // read error
    rock_trace(ctx, "rock_read_delta error ret=%d, offset=%d\r\n", ret, offset);
    return ret;
}

int rock_get_blocksize(void* ctx) {
    return ROCK_DEFAULT_BLOCK_SIZE;
}

int rock_read_file(void* ctx,  void* name, unsigned char* dest, unsigned int offset, unsigned int size){return 0;}
int rock_write_file(void* ctx,  void* name, unsigned char* src, unsigned int offset, unsigned int size){return 0;}
int rock_delete_file(void* ctx, void* name){return 0;}
/* ROCK IPL end */

static int rock_update_main(unsigned int rom_base,  unsigned int backup_base, unsigned int backup_len, int read_rom_directly, int first_run) {
    int status;

    IOT_UPDATA_CONTEXT ctx;

    memset(&ctx, 0, sizeof(ctx));
    ctx.rom_base = rom_base;
    ctx.ram_base = ROCK_RAM_BUFFER;
    ctx.ram_len = ROCK_RAM_LEN;
    ctx.backup_base = backup_base;
    ctx.backup_len = backup_len;
    ctx.update_nvram = 0;
    ctx.read_rom_directly = read_rom_directly;
    ctx.first_run = first_run;

    rock_trace(&ctx, "ram_base:%x, ram_len:%x\r\n", ROCK_RAM_BUFFER, ROCK_RAM_LEN);
    rock_trace(&ctx, "backup_base:%x, backup_len:%x\r\n", backup_base, backup_len);
    status = iot_patch(&ctx);

    return status;
}

static void rock_fail_handler() {
    
}

/* main entrpoint */
int rock_main(int first_run)  {
    int ret = 0;
    ret = rock_update_main(ROCK_CM4_ROM_BASE, ROCK_BACKUP_BASE, ROCK_BACKUP_LEN, 1, first_run);
    if(ret) {
        rock_fail_handler();
    }
    return ret;
}
#endif
