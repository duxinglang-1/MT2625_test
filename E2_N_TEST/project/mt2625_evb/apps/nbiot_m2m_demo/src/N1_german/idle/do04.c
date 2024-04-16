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
* Source file: do04                                                  *
* Dimensions:  44 * 24                                               *
* NumColors:   16bpp: 65536                                          *
*                                                                    *
**********************************************************************
*/

#include <stdlib.h>

#include "GUI.h"

#ifndef GUI_CONST_STORAGE
  #define GUI_CONST_STORAGE const
#endif

extern GUI_CONST_STORAGE GUI_BITMAP bmdo04;

static GUI_CONST_STORAGE unsigned short _acdo04[] = {
  0xED20, 0xED20, 0xED01, 0xED20, 0xED20, 0xED20, 0xED20, 0xED20, 0xED20, 0xED00, 0x51E0, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
        0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
  0xED20, 0xED20, 0xED20, 0xED20, 0xED20, 0xED20, 0xED20, 0xED20, 0xED20, 0xED20, 0xED20, 0xED20, 0x1880, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
        0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
  0xED20, 0xED20, 0xED20, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x1060, 0xCC40, 0xED00, 0xED20, 0xED20, 0x3120, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
        0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
  0xED20, 0xED20, 0xED20, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x20A0, 0xED20, 0xED20, 0xED00, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
        0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
  0xED20, 0xED20, 0xED20, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x20C0, 0xED20, 0xED20, 0xDCC0, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
        0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
  0xED20, 0xED20, 0xED20, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xC440, 0xED20, 0xED20, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x49A0, 0xED00, 0xED20, 0xED20, 0xED20, 0xE500, 0x4980, 
        0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
  0xED20, 0xED20, 0xED00, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0820, 0xED20, 0xED20, 0x82C0, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xED00, 0xED20, 0xED20, 0xED20, 0xED20, 0xED20, 0xED20, 0xED20, 
        0xED00, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
  0xED20, 0xED20, 0xED20, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xED20, 0xED20, 0xED00, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xED00, 0xED20, 0xED20, 0xA360, 0x0020, 0x0000, 0x0000, 0xAB80, 0xED00, 
        0xED20, 0xED00, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
  0xED20, 0xED20, 0xED20, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xED00, 0xED20, 0xED20, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xDCA0, 0xED20, 0xED20, 0x28E0, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x30E0, 
        0xED20, 0xED20, 0xD4A0, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
  0xED20, 0xED20, 0xED20, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xBC20, 0xED01, 0xED20, 0x0000, 0x0000, 0x0000, 0x0000, 0x0840, 0xED20, 0xED20, 0x8B00, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
        0x9320, 0xED20, 0xED20, 0x0840, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
  0xED20, 0xED20, 0xED20, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xABA0, 0xED20, 0xED20, 0x0000, 0x0000, 0x0000, 0x0000, 0xABA0, 0xED20, 0xED20, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
        0x0000, 0xED20, 0xED00, 0xAB80, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
  0xED20, 0xED20, 0xED20, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xB3E0, 0xED00, 0xED20, 0x0000, 0x0000, 0x0000, 0x0000, 0xED20, 0xED20, 0xED20, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
        0x0000, 0xED00, 0xED20, 0xED20, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
  0xED20, 0xED20, 0xED20, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xDCC0, 0xED20, 0xED20, 0x0000, 0x0000, 0x0000, 0x0000, 0xED00, 0xED20, 0xABC0, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
        0x0000, 0xABC0, 0xED20, 0xED20, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
  0xED20, 0xED20, 0xED20, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xED20, 0xED20, 0xED00, 0x0000, 0x0000, 0x0000, 0x0000, 0xED20, 0xED20, 0x9300, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
        0x0000, 0x9320, 0xED20, 0xED20, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
  0xED20, 0xED20, 0xED20, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xED20, 0xED20, 0xABA0, 0x0000, 0x0000, 0x0000, 0x0000, 0xED01, 0xED20, 0xA360, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
        0x0000, 0x9B60, 0xED20, 0xED20, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
  0xED20, 0xED20, 0xED20, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x7280, 0xED20, 0xED20, 0x20C0, 0x0000, 0x0000, 0x0000, 0x0000, 0xED20, 0xED20, 0xDCC0, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
        0x0000, 0xDCE0, 0xED20, 0xED20, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
  0xED20, 0xED20, 0xED20, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xED20, 0xED20, 0xED20, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xD480, 0xED00, 0xED20, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
        0x0000, 0xED20, 0xED20, 0xCC80, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
  0xED20, 0xED20, 0xED00, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xDCA0, 0xED20, 0xED01, 0x49A0, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x3120, 0xED00, 0xED20, 0x4160, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
        0x4980, 0xED20, 0xED20, 0x3120, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
  0xED20, 0xED20, 0xED20, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0820, 0xE500, 0xED00, 0xED20, 0xCC40, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xED20, 0xED20, 0xED20, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
        0xED20, 0xED20, 0xED20, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x9B60, 0xB3E0, 0x0000, 0x0000,
  0xED20, 0xED20, 0xED20, 0x3100, 0x28E0, 0x3100, 0x4180, 0x8AE0, 0xED00, 0xED20, 0xED20, 0xED20, 0xCC40, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x1880, 0xED00, 0xED20, 0xED20, 0x20A0, 0x0000, 0x0000, 0x0000, 0x28E0, 0xED20, 
        0xED20, 0xED20, 0x1060, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xB3E0, 0xED20, 0xED20, 0xED00, 0x0000,
  0xED20, 0xED20, 0xED01, 0xED00, 0xED00, 0xED20, 0xED20, 0xED20, 0xED20, 0xED20, 0xED20, 0x4980, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x3120, 0xED20, 0xED20, 0xED20, 0xED20, 0xED20, 0xED20, 0xED20, 0xED20, 
        0xED20, 0x28E0, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xED20, 0xED20, 0xED20, 0xED20, 0x0000,
  0xED20, 0xED20, 0xED20, 0xED20, 0xED20, 0xED20, 0xED20, 0xD4A0, 0x8AE0, 0x1060, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xC440, 0xED00, 0xED20, 0xED20, 0xED20, 0xED20, 0xBC20, 
        0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x59E0, 0xED20, 0xED20, 0xB3C0, 0x0000,
  0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x1880, 0x0000, 0x0000, 0x0000, 
        0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0820, 0x0000, 0x0000,
  0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
        0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000
};

GUI_CONST_STORAGE GUI_BITMAP bmdo04 = {
  44, // xSize
  24, // ySize
  88, // BytesPerLine
  16, // BitsPerPixel
  (unsigned char *)_acdo04,  // Pointer to picture data
  NULL,  // Pointer to palette
  GUI_DRAW_BMPM565
};

/*************************** End of file ****************************/