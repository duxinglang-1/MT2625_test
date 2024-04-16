#ifndef _GDI_H_
#define _GDI_H_

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include "memory_attribute.h"
//#include "image_info.h"
#if defined(YQ_WATCH_SPPPORT)
#define GDI_LCD_WIDTH   160
#define GDI_LCD_HIGHT   80
#else
#define GDI_LCD_WIDTH   240
#define GDI_LCD_HIGHT   240
#endif

typedef enum
{
	GDI_DRAW_RESULT_OK,
	GDI_DRAW_RESULT_ERROR_SIZE,
	GDI_DRAW_RESULT_ERROR_BUFFER,
	GDI_DRAW_RESULT_ERROR_PARAM,
	GDI_DRAW_RESULT_ERROR_IMAGE_FORMAT,
	GDI_DRAW_RESULT_ERROR_IMAGE_FRAME,
	GDI_DRAW_RESULT_ERROR_IMAGE_INDEX,
	GDI_DRAW_RESULT_ERROR_IMAGE_RES,
	GDI_DRAW_RESULT_ERROR_MAX
}gdi_result_enum;

typedef unsigned short color ;

typedef struct
{
	uint32_t width;
	uint32_t hight;
	uint8_t  *data;
}gdi_fill_data_struct;

uint8_t * gdi_draw_get_frame_addr(void);
gdi_result_enum gdi_init(uint8_t *fb);
gdi_result_enum gdi_clean(void);
gdi_result_enum gdi_draw_point(uint32_t x,uint32_t y,color c) ;
gdi_result_enum gdi_draw_line(uint32_t sx,uint32_t sy,uint32_t ex,uint32_t ey,color c);
gdi_result_enum gdi_draw_circle(uint32_t x,uint32_t y,uint32_t r,color c);
gdi_result_enum gdi_draw_fill_rectangle(uint32_t sx,uint32_t sy,uint32_t ex,uint32_t ey,color c);
gdi_result_enum gdi_draw_fill_data(uint32_t sx,uint32_t sy,gdi_fill_data_struct *data);
gdi_result_enum gdi_draw_image(uint32_t sx,uint32_t sy,uint32_t image_id);
gdi_result_enum gdi_draw_image_demansoin(uint32_t image_id, uint32_t *width,uint32_t *height);

#endif
