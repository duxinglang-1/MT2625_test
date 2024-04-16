/* Copyright Statement:
*
* (C) 2005-2017  MediaTek Inc. All rights reserved.
*
* This software/firmware and related documentation ("MediaTek Software") are
* protected under relevant copyright laws. The information contained herein
* is confidential and proprietary to MediaTek Inc. ("MediaTek") and/or its licensors.
* Without the prior written permission of MediaTek and/or its licensors,
* any reproduction, modification, use or disclosure of MediaTek Software,
* and information contained herein, in whole or in part, shall be strictly prohibited.
* You may only use, reproduce, modify, or distribute (as applicable) MediaTek Software
* if you have agreed to and been bound by the applicable license agreement with
* MediaTek ("License Agreement") and been granted explicit permission to do so within
* the License Agreement ("Permitted User").  If you are not a Permitted User,
* please cease any access or use of MediaTek Software immediately.
* BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
* THAT MEDIATEK SOFTWARE RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES
* ARE PROVIDED TO RECEIVER ON AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL
* WARRANTIES, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
* NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
* SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
* SUPPLIED WITH MEDIATEK SOFTWARE, AND RECEIVER AGREES TO LOOK ONLY TO SUCH
* THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. RECEIVER EXPRESSLY ACKNOWLEDGES
* THAT IT IS RECEIVER'S SOLE RESPONSIBILITY TO OBTAIN FROM ANY THIRD PARTY ALL PROPER LICENSES
* CONTAINED IN MEDIATEK SOFTWARE. MEDIATEK SHALL ALSO NOT BE RESPONSIBLE FOR ANY MEDIATEK
* SOFTWARE RELEASES MADE TO RECEIVER'S SPECIFICATION OR TO CONFORM TO A PARTICULAR
* STANDARD OR OPEN FORUM. RECEIVER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND
* CUMULATIVE LIABILITY WITH RESPECT TO MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
* AT MEDIATEK'S OPTION, TO REVISE OR REPLACE MEDIATEK SOFTWARE AT ISSUE,
* OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY RECEIVER TO
* MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
*/
/*
 **************************************************************************
 * File Description
 * ----------------
 *                  Display Handler Task Type Defintions
 **************************************************************************/

#ifndef ADSH_TYP_H
#define ADSH_TYP_H

/***************************************************************************
 * Nested Include Files
 **************************************************************************/

#if !defined(APPLAYER_H)
#   include "applayer.h"
#endif


/***************************************************************************
 * Manifest Constants
 **************************************************************************/

/* Display Screen size settings                                           */
#define DISPLAY_TOP         (0)
#define DISPLAY_BOTTOM      (3)
#define DISPLAY_LEFT        (0)
#define DISPLAY_RIGHT       (11)
#define DISPLAY_HEIGHT      (DISPLAY_BOTTOM - DISPLAY_TOP + 1)
#define DISPLAY_WIDTH       (DISPLAY_RIGHT - DISPLAY_LEFT + 1)


/* per-character attributes (held in DishTextAttribute) */
#define CHAR_ATTR_FLASH     1
#define CHAR_ATTR_REVERSE   2

#define CHAR_ATTR_NONE_SET  0


/***************************************************************************
 * Enumerated Types
 **************************************************************************/

/* types of "stand-alone" Icons i.e. those that are handled by DISH.  */
/* Additional icon types are supplied for use in function calls to    */
/* AFDI's flDisplayIcon() function.                                   */
typedef enum DishIconTypeTag
{
  ICON_MESSAGE_INDICATOR,
  ICON_CALL_INDICATOR,
  ICON_POWER_INDICATOR,
  ICON_ROAM_INDICATOR,
  ICON_MENU_INDICATOR,
  ICON_NO_SERVICE_INDICATOR,
  ICON_CELL_BROADCAST_INDICATOR,
  ICON_MULTIPARTY_INDICATOR,
  ICON_CIPHERING_INDICATOR,
  ICON_VIBRATOR_INDICATOR,
  ICON_QUIET_MODE_INDICATOR,
  ICON_KB_LOCKED_INDICATOR,
  ICON_VMAIL_INDICATOR,
  NUM_OF_ICON_TYPES,

  ICON_NULL
}
DishIconType;

/* the attributes that are possible for an icon (mutually exclusive) */
typedef enum DishIconAttribTag
{
  ICON_ATTR_OFF,
  ICON_ATTR_ON,
  ICON_ATTR_FLASH,
  ICON_ATTR_REVERSE,

  NUM_OF_ICON_ATTRIBS,

  ICON_ATTR_NULL
}
DishIconAttrib;


/* the LCD level meters */
typedef enum LevelMeterTypeTag
{
  LEVEL_METER_BATTERY,
  LEVEL_METER_RSSI,

  NUM_OF_LEVEL_METERS,
  LEVEL_METER_NULL
}
LevelMeterType;



/***************************************************************************
 * Typed Constants
 **************************************************************************/



/***************************************************************************
 * Type Definitions
 **************************************************************************/


typedef Int8  DisplaySize;



typedef struct DishIconDataTag
{
  DishIconType    iconType;
  DishIconAttrib  iconAttrib;
}
DishIconData;


/*  attributes set for an individual character cell on the Display Screen */
/*  CHAR_ATTR_... bits are used to test/set/clear the attributes.         */
typedef Int8  DishTextAttrib;


/*  the contents of all the character cells on one line of the display */
typedef struct DishLineDataTag
{
  Char            text[DISPLAY_WIDTH];
  DishTextAttrib  textAttribute[DISPLAY_WIDTH];
}
DishLineData;


/*  the contents of the whole of the display screen */
typedef struct DishTextDataTag
{
  DishLineData    lineData[DISPLAY_HEIGHT];
  Boolean         cursorOn;                   /* TRUE if cursor visible */
  DisplaySize     cursorRow;                  /* row number (from 0) */
  DisplaySize     cursorColumn;               /* column number (from 0) */
}
DishTextData;


/*  new value of a level meter */
typedef struct DishLevelMeterDataTag
{
  LevelMeterType  levelMeterType;
  Percentage      meterValue;
  DishIconAttrib  meterAttribute;
}
DishLevelMeterData;



/***************************************************************************
 *  Macros
 **************************************************************************/


/***************************************************************************
 *  Function Prototypes
 **************************************************************************/


#endif

/* END OF FILE */
