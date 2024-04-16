#ifndef _GUI_
#define _GUI_

#define GUI_CONST_STORAGE const

#define GUI_DRAW_BMP565  1
#define GUI_DRAW_BMPM565 2

typedef struct{
	unsigned short xsize;
	unsigned short ysize;
	unsigned short BytesPerLine;
	unsigned short BitsPerPixel;
	unsigned char *data;
	unsigned char *palette;
	unsigned short format;
}GUI_BITMAP;


typedef struct {
	unsigned short image_id;
	GUI_BITMAP     *bitmap;
}image_info_struct;

#endif
