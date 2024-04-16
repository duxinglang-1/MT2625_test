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
* Source file: c9                                                    *
* Dimensions:  25 * 43                                               *
* NumColors:   16bpp: 65536                                          *
*                                                                    *
**********************************************************************
*/

#include <stdlib.h>

#include "GUI.h"

#ifndef GUI_CONST_STORAGE
  #define GUI_CONST_STORAGE const
#endif

extern GUI_CONST_STORAGE GUI_BITMAP bmc9;

static GUI_CONST_STORAGE unsigned short _acc9[] = {
  0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8D, 0x3D4D, 0x4D8F, 0x558F, 0x4D8F, 0x4D8F, 0x4D8F, 0x4D8F, 0x4D8F, 0x4D8F, 0x55EF, 0x4DEF, 0x3D4E, 0x358D, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E,
  0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8D, 0x354C, 0x6590, 0xB6F8, 0xBF39, 0xB6F9, 0xB6F9, 0xB6F9, 0xB6F9, 0xB6F9, 0xB6F9, 0xBF39, 0xB6F9, 0x6D91, 0x3D2C, 0x358D, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E,
  0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8D, 0x3D2D, 0x5D50, 0xC6F9, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xCEFA, 0x6D51, 0x3D4D, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E,
  0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x354D, 0x554F, 0xBEB8, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xCEFA, 0x6D90, 0x354D, 0x3D8E, 0x3D8E, 0x3D8E,
  0x3D8E, 0x3D8E, 0x3D8E, 0x3D4D, 0x24EB, 0x75F2, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x8E34, 0x2CAA, 0x3D4D, 0x3D8E, 0x3D8E,
  0x3D8E, 0x3D8E, 0x3D4D, 0x5D4F, 0x654F, 0x9EB6, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xAEF8, 0x6590, 0x5D50, 0x3D4D, 0x358D,
  0x3D8E, 0x3D8D, 0x552F, 0xB677, 0xE77C, 0xEFBD, 0xF7FF, 0xEFBE, 0xEFBE, 0xF7BE, 0xF7BE, 0xF7BE, 0xF7BE, 0xF7BE, 0xF7BE, 0xF7BE, 0xF7BE, 0xF7BE, 0xE7BD, 0xF7FE, 0xEFFE, 0xE7BC, 0xBEB8, 0x5D4F, 0x352C,
  0x354C, 0x452D, 0xA675, 0xF7BF, 0xFFFF, 0xFFFF, 0xDF7C, 0x6DF1, 0x6D90, 0x7D92, 0x8592, 0x8592, 0x8592, 0x8592, 0x8592, 0x8592, 0x8592, 0x7592, 0x6D91, 0xC77A, 0xFFFF, 0xFFFF, 0xFFFF, 0xAE77, 0x5D2F,
  0x352C, 0x44EE, 0xD73A, 0xFFFF, 0xFFFF, 0xFFFF, 0xEFBD, 0x6DF2, 0x2CEB, 0x2CEB, 0x2D2C, 0x2D2C, 0x2D2C, 0x2D2C, 0x2D2C, 0x2D2C, 0x2CEC, 0x2CEB, 0x6591, 0xDF7C, 0xFFFF, 0xFFFF, 0xFFFF, 0xF7BE, 0xCEF9,
  0x352C, 0x44EE, 0xD73B, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xCF3B, 0x458E, 0x254C, 0x358E, 0x358E, 0x358E, 0x358E, 0x358E, 0x358E, 0x2D4D, 0x3D8E, 0xB6F9, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
  0x352C, 0x44EE, 0xCEFA, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xD77C, 0x4DEF, 0x2D8C, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x2D4D, 0x3D8E, 0xC73A, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
  0x352C, 0x44EE, 0xCEFA, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xCF7B, 0x4DEF, 0x2D4C, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x354D, 0x458E, 0xBF39, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
  0x352C, 0x44EE, 0xCEFA, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xCF7B, 0x4DEF, 0x2D4C, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x354D, 0x458E, 0xBF39, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
  0x352C, 0x44EE, 0xCEFA, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xCF7B, 0x4DEF, 0x2D4C, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x354D, 0x458E, 0xBF39, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
  0x352C, 0x44EE, 0xCEFA, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xCF7B, 0x4DEF, 0x2D4C, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x354D, 0x458E, 0xBF39, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
  0x352C, 0x44EE, 0xCEFA, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xD77B, 0x4DEF, 0x2D4C, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x2D4D, 0x458E, 0xC73A, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
  0x352C, 0x44EE, 0xCEFA, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xD73B, 0x45EF, 0x254C, 0x358D, 0x358D, 0x358D, 0x358D, 0x358D, 0x3D8D, 0x2D4C, 0x3D8E, 0xBF39, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
  0x352C, 0x44EE, 0xCEFA, 0xFFFF, 0xFFFF, 0xFFFF, 0xF7BE, 0x8E35, 0x2D2C, 0x352C, 0x454E, 0x3D4E, 0x3D4E, 0x3D4E, 0x3D4E, 0x458E, 0x3D4D, 0x352C, 0x6DF2, 0xDF7C, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
  0x352C, 0x44EE, 0xD73B, 0xFFFF, 0xFFFF, 0xFFFF, 0xD7BC, 0x5590, 0x552E, 0x85F3, 0x8DF4, 0x8DF3, 0x8DF3, 0x8DF3, 0x8DF3, 0x8DF4, 0x8DF3, 0x6550, 0x4D8F, 0xBF3A, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
  0x352C, 0x44EE, 0xCEFA, 0xFFFF, 0xFFFF, 0xFFFF, 0xF7FE, 0xCF7A, 0xD73B, 0xEF7D, 0xEFBE, 0xEFBD, 0xEFBD, 0xEFBD, 0xEFBD, 0xF7BD, 0xEFBD, 0xDF7C, 0xCF3B, 0xEFBE, 0xFFFF, 0xF7FE, 0xEFFE, 0xFFBF, 0xFFFF,
  0x3D4D, 0x4CED, 0x7D92, 0x8634, 0xBF3A, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xC77A, 0x7632, 0x95F5, 0xC678,
  0x3D8E, 0x454E, 0x3D2C, 0x3D2D, 0xAEB7, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xD77B, 0x6590, 0x34EC, 0x552F,
  0x3D8E, 0x3D8E, 0x358D, 0x4D8F, 0x8E34, 0xCEFA, 0xFFBF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xDF7C, 0x6590, 0x2CEC, 0x452E,
  0x3D8E, 0x3D8E, 0x3D8E, 0x458E, 0x3D4D, 0x5D50, 0xA676, 0xD73B, 0xF7BF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFE, 0xDF7C, 0xEFBE, 0xFFFF, 0xC739, 0x6591, 0x8593, 0xAE37,
  0x3D8E, 0x3D8E, 0x3D8E, 0x3D8D, 0x358D, 0x354C, 0x3D2D, 0x5D4F, 0xAEB7, 0xDF7B, 0xDF7C, 0xD77C, 0xD77C, 0xD77C, 0xD77C, 0xDF7C, 0xDFBC, 0xAEB7, 0x5CEE, 0xB6B8, 0xFFFF, 0xF7BE, 0xDF7D, 0xF7BE, 0xFFBF,
  0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8D, 0x354C, 0x352C, 0x4D8E, 0x5DF0, 0x5DF1, 0x5DF1, 0x5DF1, 0x5DF1, 0x5DF1, 0x5DF1, 0x5DF1, 0x458E, 0x3CEC, 0xBEB8, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
  0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8D, 0x358C, 0x2D4C, 0x2D4C, 0x2D4C, 0x2D4C, 0x2D4C, 0x2D4C, 0x2D4C, 0x252B, 0x2D4C, 0x9675, 0xF7BE, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
  0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8D, 0x3D8D, 0x3D8D, 0x3D8D, 0x3D8D, 0x3D8D, 0x3D8D, 0x2D4C, 0x3D8E, 0xC77A, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
  0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x354D, 0x458E, 0xBF39, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
  0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x354D, 0x458E, 0xBF39, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
  0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x354D, 0x458E, 0xBF39, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
  0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x354D, 0x458E, 0xBF39, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
  0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x354D, 0x458E, 0xBF39, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
  0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x354D, 0x458E, 0xC73A, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
  0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x2D4C, 0x3D8E, 0xC739, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
  0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8D, 0x352C, 0x352C, 0x352C, 0x352C, 0x352C, 0x352C, 0x352C, 0x352C, 0x2D2C, 0x2CEB, 0x7E32, 0xE7BD, 0xFFFF, 0xFFFF, 0xFFFF, 0xF7FF, 0xDF3B,
  0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x358E, 0x354D, 0x554F, 0x6D50, 0x6D51, 0x6D51, 0x6D51, 0x6D51, 0x6D51, 0x6D51, 0x6D51, 0x5550, 0x5D8F, 0xC73A, 0xFFFF, 0xFFFF, 0xFFFF, 0xBEF9, 0x6D51,
  0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x358D, 0x2D2B, 0x5D4F, 0xC6F9, 0xDF7C, 0xDF7C, 0xDF7B, 0xDF7B, 0xDF7B, 0xDF7B, 0xDF7B, 0xDF7C, 0xD77B, 0xCF3A, 0xEFBD, 0xF7FF, 0xF7FE, 0xCEFA, 0x6D50, 0x352C,
  0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x354C, 0x554F, 0xC6F9, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xB6F9, 0x7DF2, 0x7551, 0x3D4D, 0x358D,
  0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x354C, 0x5D8F, 0xCEFA, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x8E34, 0x24AA, 0x3D4D, 0x3D8E, 0x3D8E,
  0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x2D2C, 0x6D91, 0xDF3B, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xDF3B, 0x6D91, 0x2D4C, 0x3D8D, 0x3D8E, 0x3D8E,
  0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x358E, 0x3D4D, 0x7591, 0xCEFA, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xD77B, 0x7D92, 0x454D, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E,
  0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E, 0x358D, 0x2D2C, 0x7592, 0xF7BE, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFBF, 0x9E35, 0x3D2D, 0x358D, 0x3D8E, 0x3D8E, 0x3D8E, 0x3D8E
};

GUI_CONST_STORAGE GUI_BITMAP bmc9 = {
  25, // xSize
  43, // ySize
  50, // BytesPerLine
  16, // BitsPerPixel
  (unsigned char *)_acc9,  // Pointer to picture data
  NULL,  // Pointer to palette
  GUI_DRAW_BMPM565
};

/*************************** End of file ****************************/