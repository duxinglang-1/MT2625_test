#ifndef _GDI_FONT_H_
#define _GDI_FONT_H_

#include <stdio.h> 
#include <stdarg.h>
#include <stdint.h>

#include "ril.h"

#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"
#include "task_def.h"
#include "gdi.h"

typedef	struct{
	uint32_t code;
	int index;
}WORD_MAP;

#endif