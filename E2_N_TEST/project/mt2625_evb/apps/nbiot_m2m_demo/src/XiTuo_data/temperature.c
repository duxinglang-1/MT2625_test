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
* Source file: temperature                                           *
* Dimensions:  20 * 20                                               *
* NumColors:   16bpp: 65536                                          *
*                                                                    *
**********************************************************************
*/

#include <stdlib.h>

#include "GUI.h"

#ifndef GUI_CONST_STORAGE
  #define GUI_CONST_STORAGE const
#endif

extern GUI_CONST_STORAGE GUI_BITMAP bmtemperature;

static GUI_CONST_STORAGE unsigned short _actemperature[] = {
  0x2104, 0x3166, 0x5ACB, 0x39E7, 0x2104, 0x2104, 0x2104, 0x2104, 0x2104, 0x2104, 0x2104, 0x2104, 0x2104, 0x2104, 0x2104, 0x2104, 0x2104, 0x2104, 0x2104, 0x2104,
  0x4A69, 0xEF7D, 0xC618, 0xEF7D, 0x94B2, 0x2104, 0x2104, 0x2104, 0x2104, 0x2104, 0x2104, 0x2104, 0x2104, 0x2104, 0x2104, 0x2104, 0x2104, 0x2104, 0x2104, 0x2104,
  0xDF1B, 0x7BCF, 0x2104, 0x3166, 0xDF1B, 0x4A69, 0x2104, 0x2104, 0x2104, 0x2104, 0x2104, 0x2104, 0x2104, 0x2104, 0x2104, 0x2104, 0x2104, 0x2104, 0x2104, 0x2104,
  0xFFFF, 0x2104, 0x2104, 0x2104, 0xA534, 0x8C51, 0x2104, 0x2104, 0x5ACB, 0xA534, 0xDF1B, 0xFFFF, 0xFFFF, 0xEF7D, 0xB596, 0x5ACB, 0x2104, 0x2104, 0x2104, 0x2104,
  0xD69A, 0x8C51, 0x2104, 0x39E7, 0xEF7D, 0x4A69, 0x2104, 0x94B2, 0xFFFF, 0xFFFF, 0xD69A, 0x94B2, 0x94B2, 0xC618, 0xFFFF, 0xFFFF, 0xA534, 0x2104, 0x2104, 0x2104,
  0x4A69, 0xDF1B, 0xD69A, 0xFFFF, 0x7BCF, 0x2104, 0x94B2, 0xFFFF, 0xD69A, 0x39E7, 0x2104, 0x2104, 0x2104, 0x2104, 0x3166, 0xB596, 0xFFFF, 0x94B2, 0x2104, 0x2104,
  0x2104, 0x2104, 0x4A69, 0x3166, 0x2104, 0x5ACB, 0xFFFF, 0xD69A, 0x3166, 0x2104, 0x2104, 0x2104, 0x2104, 0x2104, 0x2104, 0x2104, 0xC618, 0xFFFF, 0x4A69, 0x2104,
  0x2104, 0x2104, 0x2104, 0x2104, 0x2104, 0xB596, 0xFFFF, 0x5ACB, 0x2104, 0x2104, 0x2104, 0x2104, 0x2104, 0x2104, 0x2104, 0x2104, 0x6B4D, 0xFFFF, 0x8C51, 0x2104,
  0x2104, 0x2104, 0x2104, 0x2104, 0x2104, 0xFFFF, 0xDF1B, 0x2104, 0x2104, 0x2104, 0x2104, 0x2104, 0x2104, 0x2104, 0x2104, 0x2104, 0x2104, 0x2104, 0x2104, 0x2104,
  0x2104, 0x2104, 0x2104, 0x2104, 0x5ACB, 0xFFFF, 0xA534, 0x2104, 0x2104, 0x2104, 0x2104, 0x2104, 0x2104, 0x2104, 0x2104, 0x2104, 0x2104, 0x2104, 0x2104, 0x2104,
  0x2104, 0x2104, 0x2104, 0x2104, 0x5ACB, 0xFFFF, 0x8C51, 0x2104, 0x2104, 0x2104, 0x2104, 0x2104, 0x2104, 0x2104, 0x2104, 0x2104, 0x2104, 0x2104, 0x2104, 0x2104,
  0x2104, 0x2104, 0x2104, 0x2104, 0x5ACB, 0xFFFF, 0x8C51, 0x2104, 0x2104, 0x2104, 0x2104, 0x2104, 0x2104, 0x2104, 0x2104, 0x2104, 0x2104, 0x2104, 0x2104, 0x2104,
  0x2104, 0x2104, 0x2104, 0x2104, 0x5ACB, 0xFFFF, 0x8C51, 0x2104, 0x2104, 0x2104, 0x2104, 0x2104, 0x2104, 0x2104, 0x2104, 0x2104, 0x2104, 0x2104, 0x2104, 0x2104,
  0x2104, 0x2104, 0x2104, 0x2104, 0x3166, 0xFFFF, 0xC618, 0x2104, 0x2104, 0x2104, 0x2104, 0x2104, 0x2104, 0x2104, 0x2104, 0x2104, 0x2104, 0x8C51, 0x7BCF, 0x2104,
  0x2104, 0x2104, 0x2104, 0x2104, 0x2104, 0xDF1B, 0xFFFF, 0x3166, 0x2104, 0x2104, 0x2104, 0x2104, 0x2104, 0x2104, 0x2104, 0x2104, 0x39E7, 0xFFFF, 0xA534, 0x2104,
  0x2104, 0x2104, 0x2104, 0x2104, 0x2104, 0x7BCF, 0xFFFF, 0x94B2, 0x2104, 0x2104, 0x2104, 0x2104, 0x2104, 0x2104, 0x2104, 0x2104, 0xB596, 0xFFFF, 0x5ACB, 0x2104,
  0x2104, 0x2104, 0x2104, 0x2104, 0x2104, 0x2104, 0xD69A, 0xFFFF, 0x8C51, 0x2104, 0x2104, 0x2104, 0x2104, 0x2104, 0x2104, 0x8C51, 0xFFFF, 0xB596, 0x2104, 0x2104,
  0x2104, 0x2104, 0x2104, 0x2104, 0x2104, 0x2104, 0x3166, 0xD69A, 0xFFFF, 0xDF1B, 0x8C51, 0x5ACB, 0x5ACB, 0x8C51, 0xDF1B, 0xFFFF, 0xD69A, 0x3166, 0x2104, 0x2104,
  0x2104, 0x2104, 0x2104, 0x2104, 0x2104, 0x2104, 0x2104, 0x3166, 0x8C51, 0xEF7D, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xDF1B, 0x8C51, 0x2104, 0x2104, 0x2104, 0x2104,
  0x2104, 0x2104, 0x2104, 0x2104, 0x2104, 0x2104, 0x2104, 0x2104, 0x2104, 0x2104, 0x4A69, 0x5ACB, 0x5ACB, 0x4A69, 0x2104, 0x2104, 0x2104, 0x2104, 0x2104, 0x2104
};

GUI_CONST_STORAGE GUI_BITMAP bmtemperature = {
  20, // xSize
  20, // ySize
  40, // BytesPerLine
  16, // BitsPerPixel
  (unsigned char *)_actemperature,  // Pointer to picture data
  NULL,  // Pointer to palette
  GUI_DRAW_BMPM565
};

/*************************** End of file ****************************/