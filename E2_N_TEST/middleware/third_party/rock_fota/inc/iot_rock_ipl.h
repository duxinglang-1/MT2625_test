#ifndef _IOT_ROCK_IPL_H_
#define _IOT_ROCK_IPL_H_

void rock_trace(void* ctx, const char* fmt, ...);
void rock_progress(void* ctx, int percent);
int rock_read_block(void* ctx, unsigned char* dest, unsigned int start, unsigned int size);
int rock_read_delta(void* ctx, unsigned char* dest, unsigned int offset, unsigned int size);
int rock_write_block(void* ctx, unsigned char* src, unsigned int start, unsigned int size);
int rock_get_blocksize(void* ctx);

int rock_read_file(void* ctx,  void* name, unsigned char* dest, unsigned int offset, unsigned int size);
int rock_write_file(void* ctx,  void* name, unsigned char* src, unsigned int offset, unsigned int size);

int rock_delete_file(void* ctx, void* name);

int rock_fatal(void* ctx, int error_code);

int rock_mismatch(void* ctx, unsigned char* buf, unsigned int start, unsigned int size, 
                            unsigned int source_hash,unsigned int target_hash);
#endif

