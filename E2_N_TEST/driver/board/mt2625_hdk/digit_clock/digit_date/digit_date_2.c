/*********************************************************************
*                SEGGER Microcontroller GmbH & Co. KG                *
*        Solutions for real time microcontroller applications        *
*                           www.segger.com                           *
**********************************************************************
*                                                                    *
* C-file generated by                                                *
*                                                                    *
*        Bitmap Converter for emWin (Demo version) V5.24.            *
*        Compiled Jan 27 2014, 08:56:32                              *
*        (c) 1998 - 2013 Segger Microcontroller GmbH && Co. KG       *
*                                                                    *
*        May not be used in a product                                *
*                                                                    *
**********************************************************************
*                                                                    *
* Source file: digit_date_2                                          *
* Dimensions:  9 * 9                                                 *
* NumColors:   16bpp: 65536                                          *
*                                                                    *
**********************************************************************
*/

#include <stdlib.h>

#include "GUI.h"

#ifndef GUI_CONST_STORAGE
  #define GUI_CONST_STORAGE const
#endif

extern GUI_CONST_STORAGE GUI_BITMAP bmdigit_date_2;

static GUI_CONST_STORAGE unsigned short _acdigit_date_2[] = {
  0xBDF7, 0xBDF7, 0xBDF7, 0xBDF7, 0xBDF7, 0xBDF7, 0x738E, 0x0000, 0x0000,
  0xBDF7, 0xBDF7, 0xBDF7, 0xBDF7, 0xBDF7, 0xEF7D, 0xFFFF, 0x738E, 0x0000,
  0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x528A, 0xFFFF, 0xBDF7, 0x0000,
  0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x738E, 0xFFFF, 0xAD75, 0x0000,
  0x528A, 0xEF7D, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x528A, 0x0000,
  0xDEFB, 0xFFFF, 0x8C71, 0x8410, 0x8410, 0x8410, 0x3186, 0x0000, 0x0000,
  0xFFFF, 0xBDF7, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
  0xFFFF, 0xDEFB, 0x8410, 0x8410, 0x8410, 0x8410, 0x8410, 0x4208, 0x0000,
  0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x8410, 0x0000
};

GUI_CONST_STORAGE GUI_BITMAP bmdigit_date_2 = {
  9, // xSize
  9, // ySize
  18, // BytesPerLine
  16, // BitsPerPixel
  (unsigned char *)_acdigit_date_2,  // Pointer to picture data
  NULL,  // Pointer to palette
  GUI_DRAW_BMPM565
};

/*************************** End of file ****************************/