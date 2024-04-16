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
 *
 * System wide constants/typedefs etc.
 **************************************************************************/

#ifndef RVSYSTEM_H
#define RVSYSTEM_H

/***************************************************************************
 * Nested Include Files
 **************************************************************************/

#include <system.h>
#include <kitqid.h>
#include <nbiot_modem_common_config.h>

/***************************************************************************
 * Manifest Constants
 **************************************************************************/

/* To remove compile warnings on GCC */
//#define ATCI_CONST_CHAR_STR (const Char*)
#define ATCI_CONST_CHAR_STR (Char *)

/* Temporary for testing */
//#define ATCI_ENABLE_DYN_AT_BUFF (1)
#define ENABLE_MATREADY_ON_ALL_CHANNELS (1)


/********************************************/
/* Make sure config. correct for unit tests */
/********************************************/
#if defined (ENABLE_ATCI_UNIT_TEST)

#if defined (DEVELOPMENT_VERSION)
/* TODO: Remove when ATCI fully tested */
#define ATCI_DEBUG (1)
#endif

#if 0 /* Leave Phonebook ON for now in UTs */
 #if defined (FEA_PHONEBOOK)
  #undef FEA_PHONEBOOK
 #endif
#endif
 #if defined (FEA_A_SLASH)
  #undef FEA_A_SLASH
 #endif
 #if defined (FEA_MT_PDN_ACT)
  #undef FEA_MT_PDN_ACT
 #endif
 #if defined (FEA_QOS_TFT)
  #undef FEA_QOS_TFT
 #endif
#if 0 /* Leave PPP ON for now in UTs */
 #if defined (FEA_PPP)
  #undef FEA_PPP
 #endif
#endif /* 0 */
#if 0 /* Leave SIM Lock ON for now in UTs */
 #if defined (FEA_SIMLOCK)
  #undef FEA_SIMLOCK
 #endif
#endif
#endif
/**************************************************************************/
/* Maximum allowed CI entities and PSD connections                         */
/**************************************************************************/
#define ATCI_MAX_NUM_ENABLED_AT_CHANNELS        (11) /* Default 11 AT channels
                                                      */
#if defined (ENABLE_ATCI_UNIT_TEST)
#define ATCI_MAX_NUM_PSD_CONNECTIONS            (6)  /* Default 6 PSD connections */
#else
#define ATCI_MAX_NUM_PSD_CONNECTIONS            (3)  /* Default 3 PSD connections */
#endif

#if defined (ATCI_ENABLE_DYN_AT_BUFF)
/* Maximum number of AT command buffers - shared across all AT channels */
#define ATCI_MAX_NUM_AT_CMD_BUFFERS             (4)
#endif

/* CR MAX values - may have to be tuned */
#define ATCI_MAX_NUM_CONV_BUFF                  (1)
#define ATCI_MAX_NUM_OUTPUT_BUFF                (1)

#if defined (FEA_PHONEBOOK)
#define ATCI_MAX_NUM_ALPHA_BUFF                 (1)
#define ATCI_MAX_NUM_ALPHA_SEARCH_BUFF          (1)
#endif /* FEA_PHONEBOOK */

/*  Some General constants that don't exists elsewhere
*   U for unsigned variables and S for signed ones*/
#define VG_MAX_UINT8        (0xFF)
#define VG_MAX_UINT16       (0xFFFF)
#define VG_MAX_UINT32       (0xFFFFFFFF)
#define VG_MAX_SINT8        (0x7F)
#define VG_MAX_SINT16       (0x7FFF)
#define VG_MAX_SINT32       (0x7FFFFFFF)

/* AT command constants    */

/* Max number of ATCI Channel IDs - this matches MUX channel IDs - but is
 * different to the maximum permitted enabled AT channels which is set to
 * ATCI_MAX_NUM_ENABLED_AT_CHANNELS
 */
#define CI_MAX_ENTITIES      (MAX_NUM_MDL_CHANNELS)

#define VGMUX_CHANNEL_COMMAND_1   (0)    /**< AT command channel number 1. */
#define VGMUX_CHANNEL_COMMAND_2   (1)    /**< AT command channel number 2. */
#define VGMUX_CHANNEL_COMMAND_3   (2)    /**< AT command channel number 3. */
#define VGMUX_CHANNEL_COMMAND_4   (3)    /**< AT command channel number 4. */
#define VGMUX_CHANNEL_COMMAND_5   (4)    /**< AT command channel number 5. */
#define VGMUX_CHANNEL_COMMAND_6   (5)    /**< AT command channel number 6. */
#define VGMUX_CHANNEL_COMMAND_7   (6)    /**< AT command channel number 7. */
#define VGMUX_CHANNEL_COMMAND_8   (7)    /**< AT command channel number 8. */
#define VGMUX_CHANNEL_COMMAND_9   (8)    /**< AT command channel number 9. */
#define VGMUX_CHANNEL_COMMAND_10  (9)    /**< AT command channel number 10. */
#define VGMUX_CHANNEL_COMMAND_11  (10)   /**< AT command channel number 11. */
#define VGMUX_CHANNEL_COMMAND_12  (11)   /**< AT command channel number 12. */
#define VGMUX_CHANNEL_COMMAND_13  (12)   /**< AT command channel number 13. */
#define VGMUX_CHANNEL_COMMAND_14  (13)   /**< AT command channel number 14. */

#define VGMUX_CHANNEL_INVALID     (0xF0)    /**< An invalid multiplexer channel. Unsolicited messages
                                          * sent on this channel are yet to be converted to Channel
                                          * Management format. */
#define AT_CACHE_SIZE        (200)

#define SHORT_COMMAND_LINE_SIZE    (200)

#if defined (ENABLE_LONG_AT_CMD_RSP)
/* Large enough to take NVRAM write (AT*MNVMW of length 2048 bytes */
#define COMMAND_LINE_SIZE    (4200)

/* Smaller buffer sizes to save memory */
#define AT_LARGE_BUFF_SIZE         (1500)
#define AT_MEDIUM_LARGE_BUFF_SIZE  (600)
#define AT_MEDIUM_BUFF_SIZE        (150)
#define AT_SMALL_BUFF_SIZE         (75)

#else
#if defined (DISABLE_RAVEN_LONG_AT_COMMAND_LINE)
#define COMMAND_LINE_SIZE    (180)
#else
#define COMMAND_LINE_SIZE    (1500)
#endif
#endif

/* string and character constants */

#define STRING_LENGTH_40       (40)
#define NULL_TERMINATOR_LENGTH (1)
#define SINGLE_CHAR_LENGTH     (1)
#define TERMINATOR_STRING      ((const Char *)"NULL")

#define SEMICOLON_CHAR       (';')
#define COLON_CHAR           (':')
#define COMMA_CHAR           (',')
#define DOT_CHAR             ('.')
#define QUOTES_CHAR          ('\"')
#define QUERY_CHAR           ('?')
#define EQUALS_CHAR          ('=')
#define STAR_CHAR            ('*')
#define HASH_CHAR            ('#')
#define SPACE_CHAR           (' ')
#define AMPERSAND_CHAR       ('&')
#define HAT_CHAR             ('^')
#define BACK_SLASH_CHAR      (92)
#define PLUS_CHAR            ('+')
#define NULL_CHAR            ('\0')
#define ESC_CHAR             (27)
#define CTRL_Z_CHAR          (26)
#define INTERNATIONAL_PREFIX ('+')
#define S_REGISTER_CHAR      ('S')
#define FWD_SLASH_CHAR       ('/')
#define ZERO_CHAR            ('0')
#define NINE_CHAR            ('9')

#define VG_MAXIMUM_ON_AIR_BITRATE       (14400)

#define VG_DTEIF_DEFAULT_BAUDRATE       VG_DTEIF_115200
#define VG_DTEIF_DEFAULT_NUM_DATA_BITS  8
#define VG_DTEIF_DEFAULT_NUM_STOP_BITS  1
#define VG_DTEIF_DEFAULT_PARITY         GSM_PARITY_NONE

#define VG_DTEIF_DATA_REQ_LEVEL         4

/* -----------------18/09/97 09:56-------------------
FIFO sizes and HWM and LWM settings
 - Note Fast FIFOs are always 256 in size,
   therefore no sizes need defining
 --------------------------------------------------*/

/* -----------------11/08/97 16:52-------------------
Factory defaults follow. jcd
 --------------------------------------------------*/
#define VG_DEFAULT_ESC_CHAR             '+'
#define VG_DEFAULT_TERMINATOR           (13)
#define VG_DEFAULT_ESC_GUARD_TIME       1000

#define VG_V42_MAX_STRING_SIZE            (64)
/* VG_V42BIS_DICT_INDEX_SIZE is the number of bits needed for an index
   into the dictionary.  It must hold that
   log2(VG_V42BIS_MAX_DICT_SIZE) <= VG_V42BIS_DICT_INDEX_SIZE.
   Provided the dictionary size is a power of 2, the following use of
   `1 <<' ensures this. */
#define VG_V42BIS_DICT_INDEX_SIZE       10
#define VG_V42BIS_MAX_DICT_SIZE         (1 << (VG_V42BIS_DICT_INDEX_SIZE))

#define VG_MAX_SMS_TDPU_SIZE            (232)


/* -----------------22/09/97 11:04-------------------
Status Line definitions
 --------------------------------------------------*/
#define VG_DTR_DSR_MASK     0x0001
#define VG_RTS_CTS_MASK     0x0002
#define VG_DCD_MASK         0x0004
#define VG_RI_MASK          0x0008
#define VG_BREAK_MASK       0x0010
#define VG_FC_MASK          0x0020
#define VG_IS_STATUS_MASK   0x8000
#define VG_DATA_MASK        0x00ff
/* -----------------21/10/97 13:15-------------------
Includes BREAK in the status mask
 --------------------------------------------------*/
#define VG_STATUS_BITS_MASK 0x001F

/* -----------------19/10/97 16:35-------------------
ASCII control codes. Useful throughout code.
 --------------------------------------------------*/

#define VG_ASCII_NUL       0x00
#define VG_ASCII_SOH       0x01
#define VG_ASCII_STX       0x02
#define VG_ASCII_ETX       0x03
#define VG_ASCII_EOT       0x04
#define VG_ASCII_ENQ       0x05
#define VG_ASCII_ACK       0x06
#define VG_ASCII_BEL       0x07
#define VG_ASCII_BS        0x08
#define VG_ASCII_HT        0x09
#define VG_ASCII_LF        0x0A
#define VG_ASCII_VT        0x0B
#define VG_ASCII_FF        0x0C
#define VG_ASCII_CR        0x0D
#define VG_ASCII_SO        0x0E
#define VG_ASCII_SI        0x0F
#define VG_ASCII_DLE       0x10
#define VG_ASCII_DC1       0x11
#define VG_ASCII_DC2       0x12
#define VG_ASCII_DC3       0x13
#define VG_ASCII_DC4       0x14
#define VG_ASCII_NAK       0x15
#define VG_ASCII_SYN       0x16
#define VG_ASCII_ETB       0x17
#define VG_ASCII_CAN       0x18
#define VG_ASCII_EM        0x19
#define VG_ASCII_SUB       0x1A
#define VG_ASCII_ESC       0x1B
#define VG_ASCII_FS        0x1C
#define VG_ASCII_GS        0x1D
#define VG_ASCII_RS        0x1E
#define VG_ASCII_US        0x1F
#define VG_ASCII_SC        0x3B

#define VG_XON             VG_ASCII_DC1
#define VG_XOFF            VG_ASCII_DC3

/* ------------------------------------------------
Hang up parameter definitions
 --------------------------------------------------*/
#define ATH_PARAM_DISCONNECT_NORMAL 0
#define ATH_PARAM_DISCONNECT_MO     1

#define MIN_CID_VALUE_CID0          (0)
#define MIN_CID_VALUE_CID1          (1)

/* allow for CID index starting at 1 */
#if !defined (MAX_NUMBER_OF_CIDS)
/* We actually only support 15 contexts, as context 0 is not used */
#    define MAX_NUMBER_OF_CIDS    16
#endif /* !MAX_NUMBER_OF_CIDS */

#define CID_NUMBER_UNKNOWN (MAX_NUMBER_OF_CIDS + 1)

/** The maximum size of dial string */
#define MAX_DIAL_STRING_SIZE 255
/** The maximum size of connect string */
#define MAX_CONNECT_STRING_SIZE 255

/***************************************************************************
 * Type Definitions
 **************************************************************************/

typedef Int16 VgStatus;
typedef enum VgFlowControlStateTag
{
  VG_FC_ON_REQUESTED = 0,
  VG_FC_ON,
  VG_FC_OFF_REQUESTED,
  VG_FC_OFF
} VgFlowControlState;

#if defined (FEA_PPP)
typedef enum VgLoopbackStateTag
{
  VG_LOOPBACK_DISABLED         = 0,
  VG_LOOPBACK_ENABLED          = 1,
  VG_LOOPBACK_ENABLE_PENDING   = 2,
  VG_LOOPBACK_DISABLE_PENDING  = 3
}
VgLoopbackState;
#endif /* FEA_PPP */

/** Multiplexer Channel Type.
 * The type is used to identify multiplexer channels.
 */
typedef Int8     VgmuxChannelNumber;

/***************************************************************************
 *  Macros
 **************************************************************************/

/* -----------------22/09/97 11:11-------------------
Status Line Handling Macros
 --------------------------------------------------*/
#define VG_EXTRACT_STATUS_BITS(dATA)             ((dATA) & VG_STATUS_BITS_MASK)
#define VG_STATUS_CHANGED(oLDsTATUS, nEWsTATUS)  (VG_EXTRACT_STATUS_BITS(oLDsTATUS)!= \
                                                  VG_EXTRACT_STATUS_BITS(nEWsTATUS))

#define VG_DTR_DSR_IS_SET(dATA)        (((dATA) & VG_DTR_DSR_MASK) != 0)
#define VG_RTS_CTS_IS_SET(dATA)        (((dATA) & VG_RTS_CTS_MASK) != 0)
#define VG_DCD_IS_SET(dATA)            (((dATA) & VG_DCD_MASK) != 0)
#define VG_RI_IS_SET(dATA)             (((dATA) & VG_RI_MASK) != 0)
#define VG_BREAK_IS_SET(dATA)          (((dATA) & VG_BREAK_MASK) != 0)
#define VG_FC_IS_SET(dATA)             (((dATA) & VG_FC_MASK) != 0)
#define VG_IS_STATUS_IS_SET(dATA)      (((dATA) & VG_IS_STATUS_MASK) != 0)
#define VG_IS_DATA_VALID(dATA)         (((dATA) & VG_IS_STATUS_MASK) == 0)

#define VG_SET_STATUS_BITS(dATA, bITS)   (dATA) |= (bITS)
#define VG_SET_DTR_DSR(dATA)            VG_SET_STATUS_BITS(dATA,VG_DTR_DSR_MASK)
#define VG_SET_RTS_CTS(dATA)            VG_SET_STATUS_BITS(dATA,VG_RTS_CTS_MASK)
#define VG_SET_DCD(dATA)                VG_SET_STATUS_BITS(dATA,VG_DCD_MASK)
#define VG_SET_RI(dATA)                 VG_SET_STATUS_BITS(dATA,VG_RI_MASK)
#define VG_SET_BREAK(dATA)              VG_SET_STATUS_BITS(dATA,VG_BREAK_MASK)
#define VG_SET_FC(dATA)                 VG_SET_STATUS_BITS(dATA,VG_FC_MASK)
#define VG_SET_IS_STATUS(dATA)          VG_SET_STATUS_BITS(dATA,VG_IS_STATUS_MASK)

#define VG_CLEAR_STATUS_BITS(dATA, bITS) (dATA) &= ~(bITS)
#define VG_CLEAR_DTR_DSR(dATA)          VG_CLEAR_STATUS_BITS(dATA,VG_DTR_DSR_MASK)
#define VG_CLEAR_RTS_CTS(dATA)          VG_CLEAR_STATUS_BITS(dATA,VG_RTS_CTS_MASK)
#define VG_CLEAR_DCD(dATA)              VG_CLEAR_STATUS_BITS(dATA,VG_DCD_MASK)
#define VG_CLEAR_RI(dATA)               VG_CLEAR_STATUS_BITS(dATA,VG_RI_MASK)
#define VG_CLEAR_BREAK(dATA)            VG_CLEAR_STATUS_BITS(dATA,VG_BREAK_MASK)
#define VG_CLEAR_FC(dATA)               VG_CLEAR_STATUS_BITS(dATA,VG_FC_MASK)
#define VG_CLEAR_IS_STATUS(dATA)        VG_CLEAR_STATUS_BITS(dATA,VG_IS_STATUS_MASK)
#define VG_CLEAR_ALL_STATUS_BITS(dATA)  VG_CLEAR_STATUS_BITS(dATA,VG_STATUS_BITS_MASK)

#define VG_EXTRACT_DATA(dATA)           ((dATA) & VG_DATA_MASK)
#define VG_SET_DATA_VALID(dATA)         VG_CLEAR_IS_STATUS(dATA)
#define CIVAL_CIMUX_AT_QUEUE_FLOWCONTROL_DEACTIVE_SIZE (20)
#define CIVAL_CIMUX_AT_QUEUE_FLOWCONTROL_ACTIVE_SIZE (30)


/***************************************************************************
 * Variables
 ***************************************************************************/

/* the task to which voyager data signals should be directed */

extern TaskId vgDataTaskId;

/***************************************************************************
 *  Function Prototypes
 **************************************************************************/

#endif

/* END OF FILE */

