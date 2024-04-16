
#include "gdi_font.h"

typedef struct
{
    short start;
    short end;
} key_index_t;

typedef struct
{
    unsigned char start_value;
    unsigned char end_value;
    unsigned short index;
} MSB_Mapping_Struct;

#include "chinese_24.c"
	
