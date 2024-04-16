#include <stdio.h> 
#include <stdarg.h>
#include <stdint.h>

#include "ril.h"

#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"
#include "task_def.h"
#include "gdi.h"
#include "syslog.h"


#include "chinese_24.h"
#include "gdi_font.h"

log_create_module(sbit_font,PRINT_LEVEL_INFO);

#define SBIT_ERR(fmt,arg...)   LOG_E(sbit_font, fmt,##arg)
#define SBIT_WARN(fmt,arg...)  LOG_W(sbit_font, fmt,##arg)
#define SBIT_DBG(fmt,arg...)   LOG_I(sbit_font, fmt,##arg)

#define LINE_WIDTH	240
#define LINE_HEIGHT	36
extern gdi_result_enum gdi_draw_point(uint32_t x,uint32_t y,color c) ;


extern WORD_MAP ascii_map[95];
extern const unsigned char ascii_24[][72];


void draw_font_point(int x, int y, unsigned char bit,color c)
{
	if(bit != 0)
	{
		gdi_draw_point(x,y,c);
	}
}

void draw_font_point_n(unsigned char bit, int offset_x, int offset_y, int n, int pix,color c)
{
	int x = 0;
	int y = 0;

	x = offset_x + n%pix;
	y = offset_y + n/pix;
	
	if(bit != 0)
	{
		draw_font_point(x,y,bit, c);
	}
}

void draw_font_byte(unsigned char hex_byte, int offset_x, int offset_y, int arry_n, int pix,color c)
{
	int bit = 0x00;
	int n = 0;

	bit = hex_byte&0x80;
	n = 8*arry_n+1;
	draw_font_point_n(bit, offset_x, offset_y, n, pix, c);

	bit = hex_byte&0x40;
	n = 8*arry_n+2;
	draw_font_point_n(bit, offset_x, offset_y, n, pix, c);

	bit = hex_byte&0x20;
	n = 8*arry_n+3;
	draw_font_point_n(bit, offset_x, offset_y, n, pix, c);

	bit = hex_byte&0x10;
	n = 8*arry_n+4;
	draw_font_point_n(bit, offset_x, offset_y, n, pix, c);

	bit = hex_byte&0x08;
	n = 8*arry_n+5;
	draw_font_point_n(bit, offset_x, offset_y, n, pix, c);

	bit = hex_byte&0x04;
	n = 8*arry_n+6;
	draw_font_point_n(bit, offset_x, offset_y, n, pix, c);

	bit = hex_byte&0x02;
	n = 8*arry_n+7;
	draw_font_point_n(bit, offset_x, offset_y, n, pix, c);

	bit = hex_byte&0x01;
	n = 8*arry_n+8;
	draw_font_point_n(bit, offset_x, offset_y, n, pix, c);
}

void draw_font_arry_16(unsigned char hex_arry[][32], int offset_x, int offset_y, int arry_len, int pix,color c)
{
	int i = 0;
	unsigned char hex_byte = 0x00;

	for(i = 0; i < arry_len; i++)
	{
		draw_font_byte(hex_arry[i/32][i%32], offset_x, offset_y, i, pix, c);
	}
}

void draw_font_arry_24(unsigned char hex_arry[][72], int offset_x, int offset_y, int arry_len, int pix,color c)
{
	int i = 0;
	unsigned char hex_byte = 0x00;

	for(i = 0; i < arry_len; i++)
	{
		draw_font_byte(hex_arry[i/72][i%72], offset_x, offset_y, i, pix, c);
	}
}

void draw_font_arry(unsigned char *hex_arry, int offset_x, int offset_y, int arry_len, int pix,color c)
{
	if(pix == 16)
	{
		draw_font_arry_16(hex_arry,offset_x,offset_y,arry_len,pix,c);
	}
	else if(pix == 24)
	{
		draw_font_arry_24(hex_arry,offset_x,offset_y,arry_len,pix,c);
	}
}

int get_map_index(WORD_MAP arry[], int arry_len, uint32_t code)
{
	int i;
	int ret = -1;

	for(i = 0; i < arry_len; i++)
	{
		if(code == arry[i].code)
		{
			ret = i;
			break;
		}
	}
	return ret;
}

void draw_ascii(uint16_t ch, int offset_x, int offset_y, color c,int font_size)
{
	int index = -1;
	index = get_map_index(ascii_map, sizeof(ascii_map)/sizeof(WORD_MAP), ch);
	if(index != -1)
	{
		if(font_size == 24)
		{
			draw_font_arry(&ascii_24[index], offset_x, offset_y, sizeof(ascii_24[index]), 24, c);
		}
		else if(font_size == 16)
		{
			
		}
	}
	else
	{
		SBIT_DBG("draw_ascii  error:%x",ch);
	}
}


void draw_chinese(char *ch, int offset_x, int offset_y, color c)
{
	bool ret = false;
	uint16_t ucscode = 0x00;
	
	int index = -1;
	ret = mmi_chset_text_to_ucs2(ch, &ucscode);
	if(ret == true)
	{
		index = get_map_index(chinese_map, sizeof(chinese_map), ucscode);
	}
	if(index != -1)
	{
		draw_font_arry(&chinese_24[index], offset_x, offset_y, sizeof(chinese_24[index]), 24, c);
	}
}
void draw_chinese_16(char *ch, int offset_x, int offset_y, color c)
{
#if 0
	bool ret = false;
	uint16_t ucscode = 0x00;
	
	int index = -1;
	ret = mmi_chset_text_to_ucs2(ch, &ucscode);
	if(ret == true)
	{
		index = get_map_index(chinese_map_16, sizeof(chinese_map_16), ucscode);
	}
	if(index != -1)
	{
		draw_font_arry(&chinese_16[index], offset_x, offset_y, sizeof(chinese_16[index]), 16, c);
	}
#endif
}

void draw_chinese_by_ucstr(char *uc_str, int offset_x, int offset_y, color c)
{
	uint16_t ucscode = 0x00;
	int index = -1;
	
	ucscode = ucstr_to_ucdigits(uc_str);
	if(ucscode != 0)
	{
		index = get_map_index(chinese_map, sizeof(chinese_map), ucscode);
	}
	if(index != -1)
	{
		draw_font_arry(&chinese_24[index], offset_x, offset_y, sizeof(chinese_24[index]), 24, c);
	}
}

void draw_chinese_by_ucdigit(uint16_t ucscode, int offset_x, int offset_y, color c)
{
	int index = -1;

	if(ucscode > 0)
	{
		index = get_map_index(chinese_map, sizeof(chinese_map), ucscode);
	}
	if(index != -1)
	{
		draw_font_arry(&chinese_24[index], offset_x, offset_y, sizeof(chinese_24[index]), 24, c);
	}
	//SBIT_DBG("=======================================draw_chinese_by_ucstr:0x%x, %d",ucscode, index);
}

void draw_string(char *str, int offset_x, int offset_y, color c)
{
	int i = 0;
	int len = 0;
	char ch[4] = {0};
	int ascii_num = 0;
	int letter_num = 0;
	int chinese_num = 0;

	if(str == NULL)
	{
		return;
	}
	len = strlen(str);

	for(i = 0; i < len; i++)
	{
		memset(ch, 0, sizeof(ch));
		ch[0] = str[i];
		if((ch[0])>= ' ' && (ch[0] <= 0xDF))
		{
			SBIT_DBG("draw_string  1:%x",ch[0]);
			draw_ascii(ch[0], offset_x+(ascii_num+chinese_num)*16, offset_y, c,24);
			ascii_num++;
		}
		else
		{
			SBIT_DBG("draw_string  2:%x",ch[0]);
			if(str[i+1] != 0)
			{
				ch[1] = str[i+1];
				draw_chinese(ch, offset_x+(ascii_num+chinese_num)*24, offset_y, c);
				chinese_num++;
				i++;
			}
		}
	}
}

void draw_string_16(char *str, int offset_x, int offset_y, color c)
{
	int i = 0;
	int len = 0;
	char ch[4] = {0};
	int digit_num = 0;
	int letter_num = 0;
	int chinese_num = 0;

	if(str == NULL)
	{
		return;
	}
	len = strlen(str);

	for(i = 0; i < len; i++)
	{
		memset(ch, 0, sizeof(ch));
		ch[0] = str[i];
		if((ch[0])>= ' ' && (ch[0] <= '~'))
		{
			draw_ascii(ch[0], offset_x+(digit_num+letter_num+chinese_num)*16, offset_y, c,16);
			digit_num++;
		}
		else
		{
			if(str[i+1] != 0)
			{
				ch[1] = str[i+1];
				draw_chinese(ch, offset_x+(digit_num+letter_num+chinese_num)*16, offset_y, c);
				chinese_num++;
				i++;
			}
		}
	}
}


int ucstrlen(uint16_t *ucstr)
{
	int i = 0;
	int len = 0;

	for(i = 0;ucstr[i] != 0; i++)
	{
		len++;
	}
	return len;
}

void draw_string_byuc(uint16_t *ucstr, int offset_x, int offset_y, color c)
{
	int i = 0;
	int len = 0;
	uint16_t ucdigit;
	int digit_num = 0;
	int letter_num = 0;
	int chinese_num = 0;

	int line_offset_x = 0;
	int line_offset_y = 0;
	int line_num = 0;
	int line_start_x = (LINE_WIDTH%24)/2;
	int one_line_word_num = LINE_WIDTH/24;
	int first_line_num = 0;

	if(ucstr == NULL)
	{
		return;
	}
	len = ucstrlen(ucstr);
	//SBIT_DBG("=======================================draw_string_byuc:%d",len);
	for(i = 0; i < len; i++)
	{
		ucdigit = ucstr[i];
		if(line_num == 0)
		{
			line_offset_x = offset_x+(digit_num+letter_num+chinese_num)*24 + line_start_x;
		}
		else
		{
			line_offset_x = (digit_num+letter_num+chinese_num - (line_num-1)*one_line_word_num - first_line_num)*24 + line_start_x;
		}
		//SBIT_DBG("=======================================draw_string_aaaaaa:0x%x, [%d, %d](%d, %d)",ucdigit, line_offset_x, line_offset_y, line_start_x, offset_x);
		if((line_offset_x > (LINE_WIDTH - 24)))
		{
			if(line_num == 0)
			{
				first_line_num = digit_num+letter_num+chinese_num;
			}
			line_num++;
			line_offset_x = line_start_x;
			line_offset_y = LINE_HEIGHT*line_num;
		}
		if(line_offset_y > (240 - 24))
		{
			return;
		}
		//SBIT_DBG("=======================================draw_string_byuc:0x%x, [%d, %d](%d, %d)",ucdigit, line_offset_x, line_offset_y, line_start_x, offset_x);
		if(ucdigit>= ' ' && ucdigit <= '~')
		{
			draw_ascii(ucdigit, offset_x+(digit_num+letter_num+chinese_num)*16, offset_y, c,16);
			digit_num++;
		}
		else
		{
			draw_chinese_by_ucdigit(ucdigit, line_offset_x, offset_y+line_offset_y, c);
			chinese_num++;
		}
	}
}
