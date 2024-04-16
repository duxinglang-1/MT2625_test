
#include <stdlib.h>

#include "rs_mem.h"

//ƽ̨ͷ�ļ�
#include "FreeRTOS.h"



void* rs_malloc_porting(rs_u32 allocSize)
{
	void *returnPointer;
	

	returnPointer = pvPortMalloc(allocSize);
	return(returnPointer);
}

void  rs_free_porting(void* memBlock)
{
	vPortFree(memBlock);
}


