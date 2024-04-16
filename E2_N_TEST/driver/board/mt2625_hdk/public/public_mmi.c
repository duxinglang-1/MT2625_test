
#include "gdi.h"
#include "ril.h"
#include "hal_platform.h"
#include "hal_gpio.h"
#include "image_info.h"
#include "h10_mmi.h"

#define MMI_WIDTH	240
#define MMI_HEIGHT	240


#if defined(MTK_QRCODE_SUPPORT)
#define QRCODE_START_X	64
#define QRCODE_START_Y	44

#define MAX_MODULESIZE		33
extern unsigned char	 m_byModuleData[MAX_MODULESIZE][MAX_MODULESIZE];
#define DISPLSY_TIME	6
extern bool EncodeData(char *lpsSource);

void qrcode_init(char *url)
{
	EncodeData(url);
}

void qrcode_show(void)
{
	int len_max = 0;
	int i = 0, j = 0, k = 0;
	int x, y;

	len_max = sizeof(m_byModuleData);
	gdi_draw_fill_rectangle(0,0,MMI_WIDTH,MMI_HEIGHT,0x0000);
	for(i = 0; i < len_max; i++)
	{
		x = i/33;
		y = i%33;
		if(m_byModuleData[x][y] != 0)
		{

			for(j = 0; j < DISPLSY_TIME; j++)
			{
				for(k = 0; k < DISPLSY_TIME; k++)
				{
					gdi_draw_point(QRCODE_START_X+(x*DISPLSY_TIME+j),QRCODE_START_Y+(y*DISPLSY_TIME+k),0xFFFF);
				}
			}
		}
	}
}

void sbit_show_imei(void)
{
	char qrcode_str[108] = {0};
	char temp_str[32] = {0};
	static bool qrcode_init_flag = false;

	strcat(qrcode_str,"手表信息\r\n");
	strcat(qrcode_str,"IMEI:");
	strcat(qrcode_str,temp_info.imei_num);
	strcat(qrcode_str,"\r\n");
	strcat(qrcode_str,"DATE:");
	strcat(qrcode_str,temp_info.build_time);
	strcat(qrcode_str,"\r\n");
	strcat(qrcode_str,"失败:");
	memset(temp_str, 0 , sizeof(temp_str));
	sprintf(temp_str, "%02d         ", NVRAM_info.network_fail_record);
	strcat(qrcode_str,temp_str);
	strcat(qrcode_str,"缓存:");
	memset(temp_str, 0 , sizeof(temp_str));
	sprintf(temp_str, "%02d", get_m2m_Queue_count());
	strcat(qrcode_str,temp_str);
	qrcode_init(qrcode_str);
	qrcode_show();
	bsp_lcd_update_screen(0, 0, MMI_WIDTH - 1, MMI_HEIGHT - 1);
}
#endif


