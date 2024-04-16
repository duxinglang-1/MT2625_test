#ifndef _IOT_ROCK_H_
#define _IOT_ROCK_H_

/* public */

#define E_ROCK_SUCCESS (0)
#define E_ROCK_INVALID_DELTA (-1)
#define E_ROCK_DELTA_MISMATCH (-2)
#define E_ROCK_DELTA_CHUNK_MISMATCH (-3)
#define E_ROCK_READ_DELTA_ERROR (-4)
#define E_ROCK_READ_BLOCK_ERROR (-11)
#define E_ROCK_WRITE_BLOCK_ERROR (-12)
#define E_ROCK_RAM_NOT_ENOUGH (-20)
#define E_ROCK_INVALID_CTX    (-30)



typedef struct {
    void* user_context;
    unsigned int rom_base;    // old rom start
    unsigned char* ram_base;    // ram working buffer start
    unsigned int ram_len;         // ram working buffer len

    unsigned int backup_base;    // ram backup storage start
    unsigned int backup_len;         // ram backup storage len



    unsigned int update_nvram;    // nvram update flag

    int read_rom_directly;
    int first_run;
} IOT_UPDATA_CONTEXT;

int iot_patch(IOT_UPDATA_CONTEXT* update_ctx);
unsigned int iot_hash(unsigned char *buf,unsigned int len, unsigned int* value);

#endif



