
#include <math.h>
#include "gdi.h"
#include "GUI.h"
//#include "image_info.h"

#include "syslog.h"


#define gdi_debug(fmt,arg...) LOG_I(common,"[gdi]"fmt,##arg)

ATTR_RWDATA_IN_NONCACHED_RAM_4BYTE_ALIGN uint8_t *gdi_frame_buffer;

extern GUI_CONST_STORAGE image_info_struct image_information[];
extern uint32_t gdi_get_max_image_num(void);

uint8_t * gdi_draw_get_frame_addr(void)
{
	return gdi_frame_buffer;
}

GUI_BITMAP *gui_get_bitmap_info(uint32_t index)
{
    int i;
    uint32_t total;
    
    total = gdi_get_max_image_num();
	if(index > total){
		return 0;
	}
    
    for(i = 0 ; i < total;i++ )
    {
        if(image_information[i].image_id == index){
            return image_information[i].bitmap;
        }
    
    }
	return 0;
}
gdi_result_enum gdi_draw_point(uint32_t x,uint32_t y,color c) 
{
    uint32_t i,j;
    gdi_result_enum ret = GDI_DRAW_RESULT_OK;

    if(x > GDI_LCD_WIDTH){
		ret = GDI_DRAW_RESULT_ERROR_SIZE;
	}else if(GDI_LCD_HIGHT < y){
        ret = GDI_DRAW_RESULT_ERROR_SIZE;
    }else if(gdi_frame_buffer == NULL){
		ret = GDI_DRAW_RESULT_ERROR_BUFFER;
	}

    if( ret != GDI_DRAW_RESULT_OK){
        return ret;
    }

    *(gdi_frame_buffer + (y*GDI_LCD_WIDTH + x)*2 )   = (c>>8)&0xFF;
    *(gdi_frame_buffer + (y*GDI_LCD_WIDTH + x)*2 +1) = c&0xFF;
	
    return ret;
}


gdi_result_enum gdi_draw_line(uint32_t sx,uint32_t sy,uint32_t ex,uint32_t ey,color c)
{
    float k,value;
    int b;
    uint32_t i ,j;

    if(sy == ey){
        for(i = sx ; i < ex ; i++){
            gdi_draw_point(i,sy,c);
        }
    }else if(sx == ex){
        for(i = sy ; i < ey ; i++){
            gdi_draw_point(sx,i,c);
        }
    }else{
        k = (ey - sy)/(ex - sx);
        b = sy - k*sx;
		if(sy <= ey){
			for(i = sy ; i < ey ; i++){
			    value = (i-b)/k;
			    value += 0.5;
			    gdi_draw_point((uint32_t)value,i,c);
			}
		}else{
			for(i = sy ; i > ey ; i--){
			    value = (i-b)/k;
			    value += 0.5;
			    gdi_draw_point((uint32_t)value,i,c);
			}
		}	
    }

    return GDI_DRAW_RESULT_OK;
}
gdi_result_enum CH_gdi_draw_arc_right(uint32_t x,uint32_t y,uint32_t r,uint32_t start,uint32_t end, color c)
{
    int i;
	int32_t starty,endy;
	float value;

	if(r == 0){
		return GDI_DRAW_RESULT_ERROR_PARAM;
	}
		
	starty = start;
	if(starty < 0){
		starty = 0;
	}

	endy = end;
	if(endy >GDI_LCD_HIGHT){
		endy = GDI_LCD_HIGHT;
	}
	
	for(i = starty ; i < endy; i++){
		value = sqrt(r*r-(i-y)*(i-y)) + x;
		value += 0.5;
		if(value < GDI_LCD_WIDTH){
			gdi_draw_point((uint32_t)value,i,c);
		}
		
	}

	return GDI_DRAW_RESULT_OK;
}

gdi_result_enum CH_gdi_draw_arc_high(uint32_t x,uint32_t y,uint32_t r,uint32_t start,uint32_t end, color c)
{
    int i;
	int32_t starty,endy;
	float value;

	if(r == 0){
		return GDI_DRAW_RESULT_ERROR_PARAM;
	}
		
	starty = start;
	if(starty < 0){
		starty = 0;
	}

	endy = end;
	if(endy >GDI_LCD_HIGHT){
		endy = GDI_LCD_HIGHT;
	}
	
	for(i = starty ; i < endy; i++){
		
	    value = x - sqrt(r*r-(i-y)*(i-y));
		value += 0.5;
		if(value > 0){
			gdi_draw_point(i,(uint32_t)value,c);
		}
	}

	return GDI_DRAW_RESULT_OK;
}

gdi_result_enum CH_gdi_draw_arc_again(uint32_t x,uint32_t y,uint32_t r,uint32_t start,uint32_t end, color c)
{
    int i;
	int32_t starty,endy;
	float value;

	if(r == 0){
		return GDI_DRAW_RESULT_ERROR_PARAM;
	}
		
	starty = start;
	if(starty < 0){
		starty = 0;
	}

	endy = end;
	if(endy >GDI_LCD_HIGHT){
		endy = GDI_LCD_HIGHT;
	}
	
	for(i = starty ; i < endy; i++){
		value = sqrt(r*r-(i-y)*(i-y)) + x;
		value += 0.5;
		if(value < 240){
			gdi_draw_point(i,(uint32_t)value,c);	

		}
			}

	return GDI_DRAW_RESULT_OK;
}
gdi_result_enum CH_gdi_draw_arc(uint32_t x,uint32_t y,uint32_t r,uint32_t start,uint32_t end, color c)
{
    int i;
	int32_t starty,endy;
	float value, v_x = 0, v_y = 0;

	if(r == 0){
		return GDI_DRAW_RESULT_ERROR_PARAM;
	}
		
	starty = start;
	if(starty < 0){
		starty = 0;
	}

	endy = end;
	if(endy > 360){
		endy = 360;
	}
	
	for(i = starty ; i < endy; i++)
	{
		v_x = x + r*sin(i/180.0f * M_PI);
		v_y = y - r*cos(i/180.0f * M_PI);
		gdi_draw_point(v_x,v_y,c);
	}

	return GDI_DRAW_RESULT_OK;
}

gdi_result_enum gdi_draw_circle(uint32_t x,uint32_t y,uint32_t r,color c)
{
    int i;
	int32_t starty,endy,m, n;
	float value;
	float delta;

	if(r == 0)
	{
		return GDI_DRAW_RESULT_ERROR_PARAM;
	}
		
    for (delta = 5.0 / 4 - r, m = 0, n = r; m <= n; m ++)  /* jerry add */
    {
        gdi_draw_point(x + m, y - n, c);  /* 1 */ 
        gdi_draw_point(x + n, y - m, c);  /* 2 */
        gdi_draw_point(x + n, y + m, c);  /* 3 */ 
        gdi_draw_point(x + m, y + n, c);  /* 4 */ 
        gdi_draw_point(x - m, y + n, c);  /* 5 */ 
        gdi_draw_point(x - n, y + m, c);  /* 6 */
        gdi_draw_point(x - n, y - m, c);  /* 7 */
        gdi_draw_point(x - m, y - n, c);  /* 8 */ 
		
        if (delta >= 0)
        {
            delta += 2.0 * (m - n) + 5;
            n --;
        }
        else
        {
            delta += m * 2.0 + 3;
        }
    }


	return GDI_DRAW_RESULT_OK;
}

 
 void gdi_draw_circle1(uint32_t x,uint32_t y,uint32_t r,uint32_t Angle, color c)
 {
	 float delta;
	 int32_t m, n;

	 for (delta = 5.0 / 4 - r, m = 0, n = r; m <= n; m ++)
	 {
		 if(m <=((75*Angle)/45))
		 gdi_draw_point(x + m, y - n, c); /* 1 */ 
		 if (delta >= 0)
		 {
			 delta += 2.0 * (m - n) + 5;
			 n --;
		 }
		 else
		 {
			 delta += m * 2.0 + 3;
		 }
	 }
		 
 }
 
 void gdi_draw_circle2(uint32_t x,uint32_t y,uint32_t r,uint32_t Angle, color c)
 {
	 float delta;
	 int32_t m, n;

	 for (delta = 5.0 / 4 - r, m = 0, n = r; m <= n; m ++)
	 {
		 gdi_draw_point(x + m, y - n, c); /* 1 */ 
		 if(m >=(75-((75*(Angle-45))/45)))
		 gdi_draw_point(x + n, y - m, c); /* 2 */
		 if (delta >= 0)
		 {
			 delta += 2.0 * (m - n) + 5;
			 n --;
		 }
		 else
		 {
			 delta += m * 2.0 + 3;
		 }
	 }
		 
 }
 
 void gdi_draw_circle3(uint32_t x,uint32_t y,uint32_t r,uint32_t Angle, color c)
 {
	 float delta;
	 int32_t m, n;

	 for (delta = 5.0 / 4 - r, m = 0, n = r; m <= n; m ++)
	 {
		 gdi_draw_point(x + m, y - n, c); /* 1 */ 
		 gdi_draw_point(x + n, y - m, c); /* 2 */
		 if(m <=((75*(Angle-90))/45))
		 gdi_draw_point(x + n, y + m, c); /* 3 */	 
		 if (delta >= 0)
		 {
			 delta += 2.0 * (m - n) + 5;
			 n --;
		 }
		 else
		 {
			 delta += m * 2.0 + 3;
		 }
	 }
		 
 }
 
 void gdi_draw_circle4(uint32_t x,uint32_t y,uint32_t r,uint32_t Angle, color c)
 {
	 float delta;
	 int32_t m, n;

	 for (delta = 5.0 / 4 - r, m = 0, n = r; m <= n; m ++)
	 {
		 gdi_draw_point(x + m, y - n, c); /* 1 */			 
		 gdi_draw_point(x + n, y - m, c); /* 2 */
		 gdi_draw_point(x + n, y + m, c); /* 3 */
		 if(m >=(75-((75*(Angle-135))/45)))
		 gdi_draw_point(x + m, y + n, c); /* 4 */ 
		 
		 if (delta >= 0)
		 {
			 delta += 2.0 * (m - n) + 5;
			 n --;
		 }
		 else
		 {
			 delta += m * 2.0 + 3;
		 }
	 }
		 
 }
 
 void gdi_draw_circle5(uint32_t x,uint32_t y,uint32_t r,uint32_t Angle, color c)
 {
	 float delta;
	 int32_t m, n;

	 for (delta = 5.0 / 4 - r, m = 0, n = r; m <= n; m ++)
	 {
		 gdi_draw_point(x + m, y - n, c); /* 1 */ 
		 gdi_draw_point(x + n, y - m, c); /* 2 */
		 gdi_draw_point(x + n, y + m, c); /* 3 */		 
		 gdi_draw_point(x + m, y + n, c); /* 4 */ 
		 if(m <=((75*(Angle-180))/45))
		 gdi_draw_point(x - m, y + n, c); /* 5 */ 
		 
		 if (delta >= 0)
		 {
			 delta += 2.0 * (m - n) + 5;
			 n --;
		 }
		 else
		 {
			 delta += m * 2.0 + 3;
		 }
	 }
		 
 }
 
 void gdi_draw_circle6(uint32_t x,uint32_t y,uint32_t r,uint32_t Angle, color c)
 {
	 float delta;
	 int32_t m, n;

	 for (delta = 5.0 / 4 - r, m = 0, n = r; m <= n; m ++)
	 {
		 gdi_draw_point(x + m, y - n, c); /* 1 */ 
		 gdi_draw_point(x + n, y - m, c); /* 2 */
		 gdi_draw_point(x + n, y + m, c); /* 3 */		 
		 gdi_draw_point(x + m, y + n, c); /* 4 */ 
		 gdi_draw_point(x - m, y + n, c); /* 5 */ 
		 if(m >=(75-((75*(Angle-225))/45)))
		 gdi_draw_point(x - n, y + m, c); /* 6 */
		 
		 if (delta >= 0)
		 {
			 delta += 2.0 * (m - n) + 5;
			 n --;
		 }
		 else
		 {
			 delta += m * 2.0 + 3;
		 }
	 }
		 
 }
 
 void gdi_draw_circle7(uint32_t x,uint32_t y,uint32_t r,uint32_t Angle, color c)
 {
	 
	 float delta;
	 int32_t m, n;

	 for (delta = 5.0 / 4 - r, m = 0, n = r; m <= n; m ++)
	 {
		 gdi_draw_point(x + m, y - n, c); /* 1 */ 
		 gdi_draw_point(x + n, y - m, c); /* 2 */
		 gdi_draw_point(x + n, y + m, c); /* 3 */		 
		 gdi_draw_point(x + m, y + n, c); /* 4 */ 
		 gdi_draw_point(x - m, y + n, c); /* 5 */ 
		 gdi_draw_point(x - n, y + m, c); /* 6 */
		 if(m <=((75*(Angle-270))/45))
		 gdi_draw_point(x - n, y - m, c); /* 7 */

		 if (delta >= 0)
		 {
			 delta += 2.0 * (m - n) + 5;
			 n --;
		 }
		 else
		 {
			 delta += m * 2.0 + 3;
		 }
	 }
		 
 }
 
 void gdi_draw_circle8(uint32_t x,uint32_t y,uint32_t r,uint32_t Angle, color c)
 {
	 
	 float delta;
	 int32_t m, n;

	 for (delta = 5.0 / 4 - r, m = 0, n = r; m <= n; m ++)
	 {
		 gdi_draw_point(x + m, y - n, c); /* 1 */ 
		 gdi_draw_point(x + n, y - m, c); /* 2 */
		 gdi_draw_point(x + n, y + m, c); /* 3 */		 
		 gdi_draw_point(x + m, y + n, c); /* 4 */ 
		 gdi_draw_point(x - m, y + n, c); /* 5 */ 
		 gdi_draw_point(x - n, y + m, c); /* 6 */
		 gdi_draw_point(x - n, y - m, c); /* 7 */
		 if(m >=(75-((75*(Angle-315))/45)))
		 gdi_draw_point(x - m, y - n, c); /* 8 */ 
		 
		 if (delta >= 0)
		 {
			 delta += 2.0 * (m - n) + 5;
			 n --;
		 }
		 else
		 {
			 delta += m * 2.0 + 3;
		 }
	 }
		 
 }
 
 gdi_result_enum gdi_draw_circle_dynamic(uint32_t x,uint32_t y,uint32_t r,uint32_t Angle, color c)
 {
	 if(r == 0)
	 return GDI_DRAW_RESULT_ERROR_PARAM;
	 
	 if(Angle<=45)
	 gdi_draw_circle1(x,y,r,Angle,c);
	 else if(Angle<=90)
	 gdi_draw_circle2(x,y,r,Angle,c);
	 else if(Angle<=135)
	 gdi_draw_circle3(x,y,r,Angle,c);
	 else if(Angle<=180)
	 gdi_draw_circle4(x,y,r,Angle,c);
	 else if(Angle<=225)
	 gdi_draw_circle5(x,y,r,Angle,c);
	 else if(Angle<=270)
	 gdi_draw_circle6(x,y,r,Angle,c);
	 else if(Angle<=315)
	 gdi_draw_circle7(x,y,r,Angle,c);
	 else if(Angle<=360)
	 gdi_draw_circle8(x,y,r,Angle,c);
	 
	 return GDI_DRAW_RESULT_OK;
	 
 }

void gdi_draw_solid_circle(uint32_t x, uint32_t y, uint32_t r, color circle_color)
{
    uint32_t i;

	for(i = 0; i <= r; i++)
	{
		gdi_draw_circle(x,y,i,circle_color);
	}
}

gdi_result_enum gdi_draw_rectangle(uint32_t sx,uint32_t sy,uint32_t ex,uint32_t ey,color c)
{
	if(ex > GDI_LCD_WIDTH ){
		ex = GDI_LCD_WIDTH;
	}else if(ey > GDI_LCD_HIGHT){
		ey = GDI_LCD_HIGHT;
	}
		
	gdi_draw_line(sx,sy,ex,sy,c);
	gdi_draw_line(sx,sy,sx,ey,c);
	gdi_draw_line(ex,sy,ex,ey,c);
	gdi_draw_line(sx,ey,ex,ey,c);

	return GDI_DRAW_RESULT_OK;
}

gdi_result_enum gdi_draw_fill_rectangle(uint32_t sx,uint32_t sy,uint32_t ex,uint32_t ey,color c)
{
	uint32_t i ;
	
	if(ex > GDI_LCD_WIDTH ){
		ex = GDI_LCD_WIDTH;
	}else if(ey > GDI_LCD_HIGHT){
		ey = GDI_LCD_HIGHT;
	}
	
	for(i = sy ;i < ey; i++){
		gdi_draw_line(sx,i,ex,i,c);
	}
	
	return GDI_DRAW_RESULT_OK;
}

gdi_result_enum gdi_draw_fill_data(uint32_t sx,uint32_t sy,gdi_fill_data_struct *data)
{
	uint8_t *p;
	uint32_t i ,j,k,w,h;
	
	if(data == NULL){
		return GDI_DRAW_RESULT_ERROR_BUFFER;
	}

	p = gdi_draw_get_frame_addr();
	
	w = data->width + sx;
	if(w > GDI_LCD_WIDTH){
		w = GDI_LCD_WIDTH;
	}

	h = data->hight + sy;
	if(h > GDI_LCD_HIGHT){
		h = GDI_LCD_HIGHT;
	}
	
	for(k= 0,j = sy;j < h; j++){
		for(i = sx;i < w; i++){
			*(p + (j*GDI_LCD_WIDTH + i)*2)    = *(data->data+k+1);
			*(p + (j*GDI_LCD_WIDTH + i)*2 +1) = *(data->data+k);
			
			k+=2;
		}
	}

	return GDI_DRAW_RESULT_OK;
}

gdi_result_enum gdi_draw_image_demansoin(uint32_t image_id, uint32_t *width,uint32_t *height)
{
	GUI_BITMAP *bitmap;
	gdi_result_enum ret = GDI_DRAW_RESULT_OK;

	bitmap = gui_get_bitmap_info(image_id);
        if(bitmap == NULL){
		gdi_debug("ERROR:GDI_DRAW_RESULT_ERROR_BUFFER\r\n");
		return GDI_DRAW_RESULT_ERROR_BUFFER;
	}

	*width   = bitmap->xsize;
	*height  = bitmap->ysize;

	return ret;
}
	
gdi_result_enum gdi_draw_image(uint32_t sx,uint32_t sy,uint32_t image_id)
{
	uint8_t *res=NULL,*src=NULL;
	uint32_t img_size,width,height;
	gdi_fill_data_struct fill;
	GUI_BITMAP *bitmap;
	gdi_result_enum ret = GDI_DRAW_RESULT_OK;
	
	bitmap = gui_get_bitmap_info(image_id);
        if(bitmap == NULL){
		gdi_debug("ERROR:GDI_DRAW_RESULT_ERROR_BUFFER\r\n");
		return GDI_DRAW_RESULT_ERROR_BUFFER;
	}

	fill.hight = bitmap->ysize;
	fill.width = bitmap->xsize;
	fill.data  = bitmap->data;
	//gdi_debug("hight %d width %d data %d\r\n",fill.hight,fill.width,fill.data);

	ret = gdi_draw_fill_data(sx,sy,&fill);
	if(ret != GDI_DRAW_RESULT_OK){
		return ret;
	}

	return ret;
}

gdi_result_enum gdi_clean(void)
{
    char *p;
    
    p = gdi_draw_get_frame_addr();
    if(p == 0){
        return GDI_DRAW_RESULT_ERROR_BUFFER;
    }
    
	memset(p ,0,GDI_LCD_WIDTH*GDI_LCD_HIGHT*2 );

    return GDI_DRAW_RESULT_OK;
}
gdi_result_enum gdi_init(uint8_t *fb)
{
	if(fb == NULL){
		return GDI_DRAW_RESULT_ERROR_BUFFER;
	}
	
	gdi_frame_buffer = fb;

	memset(gdi_frame_buffer ,0,GDI_LCD_WIDTH*GDI_LCD_HIGHT*2 );

	return GDI_DRAW_RESULT_OK;
}
