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
* Source file: stepdw                                                *
* Dimensions:  30 * 30                                               *
* NumColors:   16bpp: 65536                                          *
*                                                                    *
**********************************************************************
*/

#include <stdlib.h>

#include "GUI.h"

#ifndef GUI_CONST_STORAGE
  #define GUI_CONST_STORAGE const
#endif

extern GUI_CONST_STORAGE GUI_BITMAP bmstepdw;

static GUI_CONST_STORAGE unsigned short _acstepdw[] = {
  0x3D4D, 0x45EF, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x45EF, 0x3D4D,
  0x3D4D, 0x45EF, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0xAEB7, 0xFFFF, 0xFFFF, 0x9EB6, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x45EF, 0x3D4D,
  0x3D4D, 0x45EF, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0xAEB7, 0xFFFF, 0xFFFF, 0x9EB6, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x45EF, 0x3D4D,
  0x3D4D, 0x45EF, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0xBF39, 0xFFFF, 0xFFFF, 0x8633, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0xAEB7, 0xFFFF, 0xFFFF, 0x9EB6, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x45EF, 0x3D4D,
  0x3D4D, 0x45EF, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0xBF39, 0xFFFF, 0xFFFF, 0x8633, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0xAEB7, 0xFFFF, 0xFFFF, 0x9EB6, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x45EF, 0x3D4D,
  0x3D4D, 0x45EF, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0xBF39, 0xFFFF, 0xFFFF, 0x8633, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0xAEB7, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xEFBD, 0x3D8E, 0x45EF, 0x3D4D,
  0x3D4D, 0x45EF, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0xBF39, 0xFFFF, 0xFFFF, 0x8633, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0xAEB7, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xEFBD, 0x3D8E, 0x45EF, 0x3D4D,
  0x3D4D, 0x45EF, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0xBF39, 0xFFFF, 0xFFFF, 0x8633, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0xAEB7, 0xFFFF, 0xFFFF, 0xCF3A, 0xAEB7, 0xAEB7, 0xAEB7, 0xAEB7, 0xAEB7, 0xAEB7, 0xAEB7, 0xAEB7, 0xAEB7, 0x3D8E, 0x45EF, 0x3D4D,
  0x3D4D, 0x45EF, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0xBF39, 0xFFFF, 0xFFFF, 0x8633, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0xAEB7, 0xFFFF, 0xFFFF, 0x9EB6, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x45EF, 0x3D4D,
  0x3D4D, 0x45EF, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0xBF39, 0xFFFF, 0xFFFF, 0x8633, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0xAEB7, 0xFFFF, 0xFFFF, 0x9EB6, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x45EF, 0x3D4D,
  0x3D4D, 0x45EF, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0xBF39, 0xFFFF, 0xFFFF, 0x8633, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0xAEB7, 0xFFFF, 0xFFFF, 0x9EB6, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x45EF, 0x3D4D,
  0x3D4D, 0x86B4, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x3D4D,
  0x3D4D, 0x86B4, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x3D4D,
  0x3D4D, 0x6631, 0xAEB7, 0xAEB7, 0xAEB7, 0xAEB7, 0xAEB7, 0xAEB7, 0xAEB7, 0xAEB7, 0xAEB7, 0xAEB7, 0xAEB7, 0xAEB7, 0xAEB7, 0xAEB7, 0xAEB7, 0xAEB7, 0xAEB7, 0xAEB7, 0xAEB7, 0xAEB7, 0xAEB7, 0xAEB7, 0xAEB7, 0xAEB7, 0xAEB7, 0xAEB7, 0xAEF7, 0x3D4D,
  0x3D4D, 0x45EF, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0xBF39, 0xFFFF, 0xFFFF, 0x8633, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x45EF, 0x3D4D,
  0x3D4D, 0x45EF, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0xAEB7, 0xF7BE, 0x7632, 0x3D8E, 0x3D8E, 0x3D8E, 0xBF39, 0xFFFF, 0xFFFF, 0x8633, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x45EF, 0x3D4D,
  0x3D4D, 0x45EF, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0xAEB7, 0xFFFF, 0xFFFF, 0xEFBD, 0x65F1, 0x3D8E, 0x3D8E, 0xBF39, 0xFFFF, 0xFFFF, 0x8633, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0xBF39, 0xEFBD, 0x65F1, 0x3D8E, 0x45EF, 0x3D4D,
  0x3D4D, 0x45EF, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0xAEB7, 0xFFFF, 0xFFFF, 0xFFFF, 0xEFBD, 0x65F1, 0x3D8E, 0x3D8E, 0xBF39, 0xFFFF, 0xFFFF, 0x8633, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0xBF39, 0xFFFF, 0xFFFF, 0xDF7C, 0x558F, 0x45EF, 0x3D4D,
  0x3D4D, 0x45EF, 0x3D8E, 0x3D8E, 0x3D8E, 0xAEB7, 0xFFFF, 0xFFFF, 0xFFFF, 0xEFBD, 0x65F1, 0x3D8E, 0x3D8E, 0x3D8E, 0xBF39, 0xFFFF, 0xFFFF, 0x8633, 0x3D8E, 0x3D8E, 0x3D8E, 0x558F, 0xD77B, 0xFFFF, 0xFFFF, 0xFFFF, 0xDF7C, 0x558F, 0x45EF, 0x3D4D,
  0x3D4D, 0x45EF, 0x3D8E, 0x3D8E, 0xB6F8, 0xFFFF, 0xFFFF, 0xFFFF, 0xEFBD, 0x65F1, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0xBF39, 0xFFFF, 0xFFFF, 0x8633, 0x3D8E, 0x3D8E, 0x8633, 0xEFBD, 0xFFFF, 0xFFFF, 0xFFFF, 0xD77B, 0x558F, 0x3D8E, 0x45EF, 0x3D4D,
  0x3D4D, 0x45EF, 0x558F, 0xD77B, 0xFFFF, 0xFFFF, 0xFFFF, 0xDF7C, 0x558F, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0xBF39, 0xFFFF, 0xFFFF, 0x8633, 0x3D8E, 0xBF39, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xCF3A, 0x3D8E, 0x3D8E, 0x3D8E, 0x45EF, 0x3D4D,
  0x3D4D, 0x45EF, 0xD77B, 0xFFFF, 0xFFFF, 0xFFFF, 0xD77B, 0x558F, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0xBF39, 0xFFFF, 0xFFFF, 0xBF39, 0xF7BE, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xAEB7, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x45EF, 0x3D4D,
  0x3D4D, 0x45EF, 0x65F1, 0xF7BE, 0xFFFF, 0xCF3A, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x8E74, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xDF7C, 0x7632, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x45EF, 0x3D4D,
  0x3D4D, 0x45EF, 0x3D8E, 0x7632, 0xAEB7, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x8E74, 0xBF39, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xF7BE, 0x9EB6, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x45EF, 0x3D4D,
  0x3D4D, 0x45EF, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x558F, 0xAEB7, 0xBF39, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xF7BE, 0xAEB7, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x45EF, 0x3D4D,
  0x3D4D, 0x45EF, 0x3D8E, 0x65F1, 0xAEB7, 0xAEB7, 0xCF3A, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xCF3A, 0x8E74, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x45EF, 0x3D4D,
  0x3D4D, 0x6631, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xCF3A, 0xAEB7, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x45EF, 0x3D4D,
  0x3D4D, 0x45EF, 0xD77B, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xB6F8, 0xAEB7, 0x8633, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x45EF, 0x3D4D,
  0x3D4D, 0x45EF, 0x8633, 0xAEB7, 0xAEB7, 0x65F1, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x45EF, 0x3D4D,
  0x3D4D, 0x45EF, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x45EF, 0x3D4D
};

GUI_CONST_STORAGE GUI_BITMAP bmstepdw = {
  30, // xSize
  30, // ySize
  60, // BytesPerLine
  16, // BitsPerPixel
  (unsigned char *)_acstepdw,  // Pointer to picture data
  NULL,  // Pointer to palette
  GUI_DRAW_BMPM565
};

/*************************** End of file ****************************/