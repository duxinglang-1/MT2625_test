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

/*---------------------------------------------------------------------------
 | Filename:
 | ---------
 |   frulscfg.h
 |
 | Project:
 | --------
 |   UMOLY
 |
 | Description:
 | ------------
 |   Unified Logging System (ULS) Config header
 |
 | Author:
 | -------
 |   mtk08928
 |
 |---------------------------------------------------------------------------
 | $Log$
 |
 ---------------------------------------------------------------------------*/

#ifndef _ULS_CONFIG_H_
#define _ULS_CONFIG_H_

/*---------------------------------------------------------------------------
 | The following compile time switch (USE_ULS macro) needs to be defined
 |	for all codes related to ULS.
 ---------------------------------------------------------------------------*/
/*	Use as project predefine to select ULS as used trace output
    #define USE_ULS
*/

/*===========================================================================
 | IMPORTANT NOTE
 ============================================================================
 | The following compile time switch (USE_ULS_SIMULATOR macro) switches between ULS Simulator and ULS HW.
 |	If USE_ULS_SIMULATOR is defined, the ULS Verification project will use ULS Simulator
 |		if not defined, the project will use ULS HW.
 ---------------------------------------------------------------------------*/
/*	Use as project predefine to select ULS simulator in PC env
    #define USE_ULS_SIMULATOR
*/
# include <uls_api.h>

/**
    Definitions needed to compalibility for previous HSL versions
*/

/*******************************************************************************
* Define      : FR_HSL_BASE_ADDR
* Group       : Framework, HSL, Config
* Description : The base address for the Core HSL Controller.
*******************************************************************************/
#define FR_HSL_BASE_ADDR    ULS_ABP_BASE_ADDR

/** Shift the address offsets left by this number of bits for every
 *  register in the HSL Controller.
 *  Shift the address offsets left by this number of bits for every
 *  register in the HSL Controller.  This may be required on certain platforms
 *  which can not provide 2-byte bus accesses to the HSL.
 */
# define FR_HSL_ADDR_OFFSET_SHIFT   (0)


# define    LG_DEBUG_1                  (0x0000)
/* Default HSL group id inserted by silox when none provided. */
# define DEFAULT_GROUP                 (0x0001)

/** @} */ /* End of addtogroup CfgCBEcorePLFdriversFrHsl */

/*******************************************************************************
** Macros
*******************************************************************************/
#if defined (ULS_FUNCTION_PROFILER)
# define M_FrHslPrintf0(hDR,mDFlOGiD,fORMATsPECIFER)
# define M_FrHslPrintf1(hDR,mDFlOGiD,fORMATsPECIFER, aRG1)  
# define M_FrHslPrintf2(hDR,mDFlOGiD,fORMATsPECIFER, aRG1, aRG2)
# define M_FrHslPrintf3(hDR,mDFlOGiD,fORMATsPECIFER, aRG1, aRG2, aRG3)
# define M_FrHslPrintf4(hDR,mDFlOGiD,fORMATsPECIFER, aRG1, aRG2, aRG3, aRG4)
# define M_FrHslPrintf5(hDR,mDFlOGiD,fORMATsPECIFER, aRG1, aRG2, aRG3, aRG4, aRG5)
# define M_FrHslPrintf6(hDR,mDFlOGiD,fORMATsPECIFER, aRG1, aRG2, aRG3, aRG4, aRG5, aRG6)
# define M_FrHslPrintf7(hDR,mDFlOGiD,fORMATsPECIFER, aRG1, aRG2, aRG3, aRG4, aRG5, aRG6, aRG7)
# define M_FrHslPrintf8(hDR,mDFlOGiD,fORMATsPECIFER, aRG1, aRG2, aRG3, aRG4, aRG5, aRG6, aRG7, aRG8)
# define M_FrHslPrintf9(hDR,mDFlOGiD,fORMATsPECIFER, aRG1, aRG2, aRG3, aRG4, aRG5, aRG6, aRG7, aRG8,                      \
                                                     aRG9 )
# define M_FrHslPrintf10(hDR,mDFlOGiD,fORMATsPECIFER, aRG1, aRG2, aRG3, aRG4, aRG5, aRG6, aRG7, aRG8,                     \
                                                      aRG9, aRG10 )
# define M_FrHslPrintf11(hDR,mDFlOGiD,fORMATsPECIFER, aRG1, aRG2, aRG3, aRG4, aRG5, aRG6, aRG7, aRG8,                     \
                                                      aRG9, aRG10, aRG11 )
# define M_FrHslPrintf12(hDR,mDFlOGiD,fORMATsPECIFER, aRG1, aRG2, aRG3, aRG4, aRG5, aRG6, aRG7, aRG8,                     \
                                                      aRG9, aRG10, aRG11, aRG12 )
# define M_FrHslPrintf13(hDR,mDFlOGiD,fORMATsPECIFER, aRG1, aRG2, aRG3, aRG4, aRG5, aRG6, aRG7, aRG8,                     \
                                                      aRG9, aRG10, aRG11, aRG12, aRG13 )
# define M_FrHslPrintf14(hDR,mDFlOGiD,fORMATsPECIFER, aRG1, aRG2, aRG3, aRG4, aRG5, aRG6, aRG7, aRG8,                     \
                                                      aRG9, aRG10, aRG11, aRG12, aRG13, aRG14 )
# define M_FrHslPrintf15(hDR,mDFlOGiD,fORMATsPECIFER, aRG1, aRG2, aRG3, aRG4, aRG5, aRG6, aRG7, aRG8,                     \
                                                      aRG9, aRG10, aRG11, aRG12, aRG13, aRG14, aRG15 )
# define M_FrHslPrintf16(hDR,mDFlOGiD,fORMATsPECIFER, aRG1, aRG2, aRG3, aRG4, aRG5, aRG6, aRG7, aRG8,                     \
                                                      aRG9, aRG10, aRG11, aRG12, aRG13, aRG14, aRG15, aRG16 )
# define M_FrHslTypedef(hDR, gROUPiD, fORMATsPECIFER, tYPE, pARRAY)
# define M_FrHslArray32(hDR, mDFlOGiD, fORMATsPECIFER, lENGTH, pDATA)
# define M_FrHslShArray16(hDR, mDFlOGiD, fORMATsPECIFER, lENGTH, pDATA)   

#else

# define M_FrHslPrintf0(hDR,mDFlOGiD,fORMATsPECIFER)                      \
           if ( M_FrPrimeLogFilter(mDFlOGiD) )                       \
           {                                                         \
             M_FrPrimeLogMsg1((Int32)hDR<<16);                       \
           }\
           if(M_FrPrimeLogFilterHslCheck(mDFlOGiD) )\
           {\
               if ( M_FrHslIsLogPointEnabled(hDR, M_FrPrimeLogGetHslMdfId(mDFlOGiD) ) )                 \
               {                                                              \
                 M_FrHsl0ByteMsg(hDR,M_FrPrimeLogGetHslMdfId(mDFlOGiD));                                        \
               }\
           }

/** Write a 1 argument message to HSL (just a message header, no data).
  * Write a 1 argument message to HSL (just a message header, no data).
  * Equivalent to former LG_MES_1 macro.
  * \param Int16 hDR = Autogenerated Log point ID.
  * \param Int16 mDFlOGiD = Log point ID kept for backward compatibility with MDF tool.
  * \param String fORMATsPECIFER = Format specifier string which resolves to nothing at compile time.
  * \param Int16 aRG1 = argument value to log
  */

# define M_FrHslPrintf1(hDR,mDFlOGiD,fORMATsPECIFER, aRG1)                \
           if ( M_FrPrimeLogFilter(mDFlOGiD) )                       \
           {                                                         \
             M_FrPrimeLogMsg1(((Int32)hDR<<16)|(aRG1));                       \
           }\
           if(M_FrPrimeLogFilterHslCheck(mDFlOGiD))\
           {\
               if ( M_FrHslIsLogPointEnabled(hDR, M_FrPrimeLogGetHslMdfId(mDFlOGiD) ))                 \
               {                                                              \
                 M_FrHsl1ByteMsg(hDR, M_FrPrimeLogGetHslMdfId(mDFlOGiD), aRG1);                                  \
               }\
           }

/** Write a 2 argument message to HSL (just a message header, no data).
  * Write a 2 argument message to HSL (just a message header, no data).
  * Equivalent to former LG_MES_2 macro.
  * \param Int16 hDR = Autogenerated Log point ID.
  * \param Int16 mDFlOGiD = Log point ID kept for backward compatibility with MDF tool.
  * \param String fORMATsPECIFER = Format specifier string which resolves to nothing at compile time.
  * \param Int16 aRG1 = first argument value to log
  * \param Int16 aRG2 = second argument value to log
  */

# define M_FrHslPrintf2(hDR,mDFlOGiD,fORMATsPECIFER, aRG1, aRG2)          \
           M_FrPrimeLogCheckInterface(mDFlOGiD);\
           if ( M_FrHslIsLogPointEnabled(hDR, mDFlOGiD) )                 \
           {                                                              \
             M_FrHsl2ByteMsg(hDR, mDFlOGiD, aRG1, aRG2);                            \
           }

/** Write a 3 argument message to HSL (just a message header, no data).
  * Write a 3 argument message to HSL (just a message header, no data).
  * Equivalent to former LG_MES_3 macro.
  * \param Int16 hDR = Autogenerated Log point ID.
  * \param Int16 mDFlOGiD = Log point ID kept for backward compatibility with MDF tool.
  * \param String fORMATsPECIFER = Format specifier string which resolves to nothing at compile time.
  * \param Int16 aRG1 = first argument value to log
  * \param Int16 aRG2 = second argument value to log
  * \param Int16 aRG3 = third argument value to log
  */

/** Write a 1 argument message to HSL (just a message header, no data).
  * Write a 1 argument message to HSL (just a message header, no data).
  * Equivalent to former LG_MES_1 macro.
  * \param Int16 hDR = Autogenerated Log point ID.
  * \param Int16 mDFlOGiD = Log point ID kept for backward compatibility with MDF tool.
  * \param String fORMATsPECIFER = Format specifier string which resolves to nothing at compile time.
  * \param Int16 aRG1 = argument value to log
  */

# define M_FrHslPrintf3(hDR,mDFlOGiD,fORMATsPECIFER, aRG1, aRG2, aRG3)    \
           if ( M_FrPrimeLogFilter(mDFlOGiD) )                       \
           {                                                         \
             M_FrPrimeLogMsg2(((Int32)hDR<<16)|(aRG1), ((Int32)(aRG2)<<16)|(aRG3)); \
           }\
           if(M_FrPrimeLogFilterHslCheck(mDFlOGiD))\
           {\
               if ( M_FrHslIsLogPointEnabled(hDR, M_FrPrimeLogGetHslMdfId(mDFlOGiD) ))                 \
               {                                                              \
                 M_FrHsl3ByteMsg(hDR, M_FrPrimeLogGetHslMdfId(mDFlOGiD), aRG1, aRG2, aRG3);                      \
               }\
           }

/** Write a 4 argument message to HSL (just a message header, no data).
  * Write a 4 argument message to HSL (just a message header, no data).
  * Equivalent to former LG_MES_4 macro.
  * \param Int16 hDR = Autogenerated Log point ID.
  * \param Int16 mDFlOGiD = Log point ID kept for backward compatibility with MDF tool.
  * \param String fORMATsPECIFER = Format specifier string which resolves to nothing at compile time.
  * \param Int16 aRG1 = first argument value to log
  * \param Int16 aRG2 = second argument value to log
  * \param Int16 aRG3 = third argument value to log
  * \param Int16 aRG4 = fourth argument value to log
  */
# define M_FrHslPrintf4(hDR,mDFlOGiD,fORMATsPECIFER, aRG1, aRG2, aRG3, aRG4)    \
           M_FrPrimeLogCheckInterface(mDFlOGiD);\
           if ( M_FrHslIsLogPointEnabled(hDR, mDFlOGiD) )                       \
           {                                                                    \
             M_FrHsl4ByteMsg(hDR, mDFlOGiD, aRG1, aRG2, aRG3, aRG4);                      \
           }

/** Write a 5 argument message to HSL (just a message header, no data).
  * Write a 5 argument message to HSL (just a message header, no data).
  * \param Int16 hDR = Autogenerated Log point ID.
  * Equivalent to former LG_MES_5 macro.
  * \param Int16 mDFlOGiD = Log point ID kept for backward compatibility with MDF tool.
  * \param String fORMATsPECIFER = Format specifier string which resolves to nothing at compile time.
  * \param Int16 aRG1 = first argument value to log
  * \param Int16 aRG2 = second argument value to log
  * \param Int16 aRG3 = third argument value to log
  * \param Int16 aRG4 = fourth argument value to log
  * \param Int16 aRG5 = fifth argument value to log
  */
# define M_FrHslPrintf5(hDR,mDFlOGiD,fORMATsPECIFER, aRG1, aRG2, aRG3, aRG4, aRG5)    \
           if ( M_FrPrimeLogFilter(mDFlOGiD) )                       \
           {                                                         \
             M_FrPrimeLogMsg3(((Int32)hDR<<16)|(aRG1), ((Int32)(aRG2)<<16)|(aRG3), ((Int32)(aRG4)<<16)|(aRG5));  \
           }\
           if(M_FrPrimeLogFilterHslCheck(mDFlOGiD))\
           {\
               if ( M_FrHslIsLogPointEnabled(hDR, M_FrPrimeLogGetHslMdfId(mDFlOGiD)) )                 \
               {                                                              \
                 M_FrHsl5ByteMsg(hDR, M_FrPrimeLogGetHslMdfId(mDFlOGiD), \
                        aRG1, aRG2, aRG3, aRG4, aRG5);     \
               }\
           }




/** Write a 6 argument message to HSL (just a message header, no data).
  * Write a 6 argument message to HSL (just a message header, no data).
  * \param Int16 hDR = Autogenerated Log point ID.
  * Equivalent to former LG_MES_6 macro.
  * \param Int16 mDFlOGiD = Log point ID kept for backward compatibility with MDF tool.
  * \param String fORMATsPECIFER = Format specifier string which resolves to nothing at compile time.
  * \param Int16 aRG1 = first argument value to log
  * \param Int16 aRG2 = second argument value to log
  * \param Int16 aRG3 = third argument value to log
  * \param Int16 aRG4 = fourth argument value to log
  * \param Int16 aRG5 = fifth argument value to log
  * \param Int16 aRG6 = sixth argument value to log
  */
# define M_FrHslPrintf6(hDR,mDFlOGiD,fORMATsPECIFER, aRG1, aRG2, aRG3, aRG4, aRG5, aRG6)    \
           M_FrPrimeLogCheckInterface(mDFlOGiD);\
           if ( M_FrHslIsLogPointEnabled(hDR, mDFlOGiD) )                                   \
           {                                                                                \
             M_FrHsl6ByteMsg(hDR, mDFlOGiD, aRG1, aRG2, aRG3, aRG4, aRG5, aRG6);                      \
           }

/** Write a 7 argument message to HSL (just a message header, no data).
  * Write a 7 argument message to HSL (just a message header, no data).
  * \param Int16 hDR = Autogenerated Log point ID.
  * Equivalent to former LG_MES_6 macro.
  * \param Int16 mDFlOGiD = Log point ID kept for backward compatibility with MDF tool.
  * \param String fORMATsPECIFER = Format specifier string which resolves to nothing at compile time.
  * \param Int16 aRG1 = first argument value to log
  * \param Int16 aRG2 = second argument value to log
  * \param Int16 aRG3 = third argument value to log
  * \param Int16 aRG4 = fourth argument value to log
  * \param Int16 aRG5 = fifth argument value to log
  * \param Int16 aRG6 = sixth argument value to log
  * \param Int16 aRG7 = seventh argument value to log
  */

# define M_FrHslPrintf7(hDR,mDFlOGiD,fORMATsPECIFER, aRG1, aRG2, aRG3, aRG4, aRG5, aRG6, aRG7)    \
           if ( M_FrPrimeLogFilter(mDFlOGiD) )                       \
           {                                                         \
             M_FrPrimeLogMsg45(((Int32)hDR<<16)|(aRG1), ((Int32)(aRG2)<<16)|(aRG3), \
                ((Int32)(aRG4)<<16)|(aRG5), ((Int32)(aRG6)<<16)|(aRG7), 0, 4);                      \
           }\
           if(M_FrPrimeLogFilterHslCheck(mDFlOGiD))\
           {\
               if ( M_FrHslIsLogPointEnabled(hDR, M_FrPrimeLogGetHslMdfId(mDFlOGiD)) )                 \
               {                                                              \
                  M_FrHsl7ByteMsg(hDR, M_FrPrimeLogGetHslMdfId(mDFlOGiD), \
                                aRG1, aRG2, aRG3, aRG4, aRG5, aRG6, aRG7);                      \
               }\
           }

/** Write a 8 argument message to HSL (just a message header, no data).
  * Write a 8 argument message to HSL (just a message header, no data).
  * \param Int16 hDR = Autogenerated Log point ID.
  * Equivalent to former LG_MES_6 macro.
  * \param Int16 mDFlOGiD = Log point ID kept for backward compatibility with MDF tool.
  * \param String fORMATsPECIFER = Format specifier string which resolves to nothing at compile time.
  * \param Int16 aRG1 = first argument value to log
  * \param Int16 aRG2 = second argument value to log
  * \param Int16 aRG3 = third argument value to log
  * \param Int16 aRG4 = fourth argument value to log
  * \param Int16 aRG5 = fifth argument value to log
  * \param Int16 aRG6 = sixth argument value to log
  * \param Int16 aRG7 = seventh argument value to log
  * \param Int16 aRG8 = eighth argument value to log
  */
# define M_FrHslPrintf8(hDR,mDFlOGiD,fORMATsPECIFER, aRG1, aRG2, aRG3, aRG4, aRG5, aRG6, aRG7, aRG8)    \
           M_FrPrimeLogCheckInterface(mDFlOGiD);\
           if ( M_FrHslIsLogPointEnabled(hDR, mDFlOGiD) )                                               \
           {                                                                                            \
             M_FrHsl8ByteMsg(hDR, mDFlOGiD, aRG1, aRG2, aRG3, aRG4, aRG5, aRG6, aRG7, aRG8);            \
           }

/** Write a 9 argument message to HSL (just a message header, no data).
  * Write a 9 argument message to HSL (just a message header, no data).
  * \param Int16 hDR = Autogenerated Log point ID.
  * Equivalent to former LG_MES_6 macro.
  * \param Int16 mDFlOGiD = Log point ID kept for backward compatibility with MDF tool.
  * \param String fORMATsPECIFER = Format specifier string which resolves to nothing at compile time.
  * \param Int16 aRG1 = first argument value to log
  * \param Int16 aRG2 = second argument value to log
  * \param Int16 aRG3 = third argument value to log
  * \param Int16 aRG4 = fourth argument value to log
  * \param Int16 aRG5 = fifth argument value to log
  * \param Int16 aRG6 = sixth argument value to log
  * \param Int16 aRG7 = seventh argument value to log
  * \param Int16 aRG8 = eighth argument value to log
  * \param Int16 aRG9 = nineth argument value to log
  */
# define M_FrHslPrintf9(hDR,mDFlOGiD,fORMATsPECIFER, aRG1, aRG2, aRG3, aRG4, aRG5, aRG6, aRG7, aRG8,                      \
                                                     aRG9 )                                                               \
           if ( M_FrPrimeLogFilter(mDFlOGiD) )                       \
           {                                                         \
              M_FrPrimeLogMsg45(((Int32)hDR<<16)|(aRG1),((Int32)(aRG2)<<16)|(aRG3), \
                ((Int32)(aRG4)<<16)|(aRG5), ((Int32)(aRG6)<<16)|(aRG7), ((Int32)(aRG8)<<16)|(aRG9), 5); \
           }\
           if(M_FrPrimeLogFilterHslCheck(mDFlOGiD))\
           {\
               if ( M_FrHslIsLogPointEnabled(hDR, M_FrPrimeLogGetHslMdfId(mDFlOGiD)) )                 \
               {                                                              \
                  M_FrHsl9ByteMsg(hDR, M_FrPrimeLogGetHslMdfId(mDFlOGiD), \
                                                    aRG1, aRG2, aRG3, aRG4, aRG5, aRG6, aRG7, aRG8,                                         \
                                                     aRG9 );                                                              \
               }\
           }

/** Write a 10 argument message to HSL (just a message header, no data).
  * Write a 10 argument message to HSL (just a message header, no data).
  * \param Int16 hDR = Autogenerated Log point ID.
  * Equivalent to former LG_MES_6 macro.
  * \param Int16 mDFlOGiD = Log point ID kept for backward compatibility with MDF tool.
  * \param String fORMATsPECIFER = Format specifier string which resolves to nothing at compile time.
  * \param Int16 aRG1 = first argument value to log
  * \param Int16 aRG2 = second argument value to log
  * \param Int16 aRG3 = third argument value to log
  * \param Int16 aRG4 = fourth argument value to log
  * \param Int16 aRG5 = fifth argument value to log
  * \param Int16 aRG6 = sixth argument value to log
  * \param Int16 aRG7 = seventh argument value to log
  * \param Int16 aRG8 = eighth argument value to log
  * \param Int16 aRG9 = nineth argument value to log
  * \param Int16 aRG10 = tenth argument value to log
  */
# define M_FrHslPrintf10(hDR,mDFlOGiD,fORMATsPECIFER, aRG1, aRG2, aRG3, aRG4, aRG5, aRG6, aRG7, aRG8,                     \
                                                      aRG9, aRG10 )                                                       \
           M_FrPrimeLogCheckInterface(mDFlOGiD);\
           if ( M_FrHslIsLogPointEnabled(hDR, mDFlOGiD) )                                                                 \
           {                                                                                                              \
                                M_FrHsl10ByteMsg(hDR, mDFlOGiD, aRG1, aRG2, aRG3, aRG4, aRG5, aRG6, aRG7, aRG8,                     \
                                                     aRG9, aRG10 );                                                       \
           }

/** Write a 11 argument message to HSL (just a message header, no data).
  * Write a 11 argument message to HSL (just a message header, no data).
  * \param Int16 hDR = Autogenerated Log point ID.
  * Equivalent to former LG_MES_6 macro.
  * \param Int16 mDFlOGiD = Log point ID kept for backward compatibility with MDF tool.
  * \param String fORMATsPECIFER = Format specifier string which resolves to nothing at compile time.
  * \param Int16 aRG1 = first argument value to log
  * \param Int16 aRG2 = second argument value to log
  * \param Int16 aRG3 = third argument value to log
  * \param Int16 aRG4 = fourth argument value to log
  * \param Int16 aRG5 = fifth argument value to log
  * \param Int16 aRG6 = sixth argument value to log
  * \param Int16 aRG7 = seventh argument value to log
  * \param Int16 aRG8 = eighth argument value to log
  * \param Int16 aRG9 = nineth argument value to log
  * \param Int16 aRG10 = tenth argument value to log
  * \param Int16 aRG11 = eleventh argument value to log
  */
# define M_FrHslPrintf11(hDR,mDFlOGiD,fORMATsPECIFER, aRG1, aRG2, aRG3, aRG4, aRG5, aRG6, aRG7, aRG8,                     \
                                                      aRG9, aRG10, aRG11 )                                                \
           if ( M_FrHslIsLogPointEnabled(hDR, mDFlOGiD) )                                                                 \
           {                                                                                                              \
                                M_FrHsl11ByteMsg(hDR, mDFlOGiD, aRG1, aRG2, aRG3, aRG4, aRG5, aRG6, aRG7, aRG8,                     \
                                                     aRG9, aRG10, aRG11 );                                                \
           }

/** Write a 12 argument message to HSL (just a message header, no data).
  * Write a 12 argument message to HSL (just a message header, no data).
  * \param Int16 hDR = Autogenerated Log point ID.
  * Equivalent to former LG_MES_6 macro.
  * \param Int16 mDFlOGiD = Log point ID kept for backward compatibility with MDF tool.
  * \param String fORMATsPECIFER = Format specifier string which resolves to nothing at compile time.
  * \param Int16 aRG1 = first argument value to log
  * \param Int16 aRG2 = second argument value to log
  * \param Int16 aRG3 = third argument value to log
  * \param Int16 aRG4 = fourth argument value to log
  * \param Int16 aRG5 = fifth argument value to log
  * \param Int16 aRG6 = sixth argument value to log
  * \param Int16 aRG7 = seventh argument value to log
  * \param Int16 aRG8 = eighth argument value to log
  * \param Int16 aRG9 = nineth argument value to log
  * \param Int16 aRG10 = tenth argument value to log
  * \param Int16 aRG11 = eleventh argument value to log
  * \param Int16 aRG12 = twelveth argument value to log
  */
# define M_FrHslPrintf12(hDR,mDFlOGiD,fORMATsPECIFER, aRG1, aRG2, aRG3, aRG4, aRG5, aRG6, aRG7, aRG8,                     \
                                                      aRG9, aRG10, aRG11, aRG12 )                                         \
           M_FrPrimeLogCheckInterface(mDFlOGiD);\
           if ( M_FrHslIsLogPointEnabled(hDR, mDFlOGiD) )                                                                 \
           {                                                                                                              \
                                M_FrHsl12ByteMsg(hDR, mDFlOGiD, aRG1, aRG2, aRG3, aRG4, aRG5, aRG6, aRG7, aRG8,                     \
                                                     aRG9, aRG10, aRG11, aRG12 );                                         \
           }

/** Write a 13 argument message to HSL (just a message header, no data).
  * Write a 13 argument message to HSL (just a message header, no data).
  * \param Int16 hDR = Autogenerated Log point ID.
  * Equivalent to former LG_MES_6 macro.
  * \param Int16 mDFlOGiD = Log point ID kept for backward compatibility with MDF tool.
  * \param String fORMATsPECIFER = Format specifier string which resolves to nothing at compile time.
  * \param Int16 aRG1 = first argument value to log
  * \param Int16 aRG2 = second argument value to log
  * \param Int16 aRG3 = third argument value to log
  * \param Int16 aRG4 = fourth argument value to log
  * \param Int16 aRG5 = fifth argument value to log
  * \param Int16 aRG6 = sixth argument value to log
  * \param Int16 aRG7 = seventh argument value to log
  * \param Int16 aRG8 = eighth argument value to log
  * \param Int16 aRG9 = nineth argument value to log
  * \param Int16 aRG10 = tenth argument value to log
  * \param Int16 aRG11 = eleventh argument value to log
  * \param Int16 aRG12 = twelveth argument value to log
  * \param Int16 aRG13 = thirteenth argument value to log
  */
# define M_FrHslPrintf13(hDR,mDFlOGiD,fORMATsPECIFER, aRG1, aRG2, aRG3, aRG4, aRG5, aRG6, aRG7, aRG8,                     \
                                                      aRG9, aRG10, aRG11, aRG12, aRG13 )                                  \
           if ( M_FrHslIsLogPointEnabled(hDR, mDFlOGiD) )                                                                 \
           {                                                                                                              \
                                M_FrHsl13ByteMsg(hDR, mDFlOGiD, aRG1, aRG2, aRG3, aRG4, aRG5, aRG6, aRG7, aRG8,                     \
                                                     aRG9, aRG10, aRG11, aRG12, aRG13 );                                  \
           }

/** Write a 14 argument message to HSL (just a message header, no data).
  * Write a 14 argument message to HSL (just a message header, no data).
  * \param Int16 hDR = Autogenerated Log point ID.
  * Equivalent to former LG_MES_6 macro.
  * \param Int16 mDFlOGiD = Log point ID kept for backward compatibility with MDF tool.
  * \param String fORMATsPECIFER = Format specifier string which resolves to nothing at compile time.
  * \param Int16 aRG1 = first argument value to log
  * \param Int16 aRG2 = second argument value to log
  * \param Int16 aRG3 = third argument value to log
  * \param Int16 aRG4 = fourth argument value to log
  * \param Int16 aRG5 = fifth argument value to log
  * \param Int16 aRG6 = sixth argument value to log
  * \param Int16 aRG7 = seventh argument value to log
  * \param Int16 aRG8 = eighth argument value to log
  * \param Int16 aRG9 = nineth argument value to log
  * \param Int16 aRG10 = tenth argument value to log
  * \param Int16 aRG11 = eleventh argument value to log
  * \param Int16 aRG12 = twelveth argument value to log
  * \param Int16 aRG13 = thirteenth argument value to log
  * \param Int16 aRG14 = fourteenth argument value to log
  */
# define M_FrHslPrintf14(hDR,mDFlOGiD,fORMATsPECIFER, aRG1, aRG2, aRG3, aRG4, aRG5, aRG6, aRG7, aRG8,                     \
                                                      aRG9, aRG10, aRG11, aRG12, aRG13, aRG14 )                           \
           M_FrPrimeLogCheckInterface(mDFlOGiD);\
           if ( M_FrHslIsLogPointEnabled(hDR, mDFlOGiD) )                                                                 \
           {                                                                                                              \
                                M_FrHsl14ByteMsg(hDR, mDFlOGiD, aRG1, aRG2, aRG3, aRG4, aRG5, aRG6, aRG7, aRG8,                     \
                                                     aRG9, aRG10, aRG11, aRG12, aRG13, aRG14 );                           \
           }

/** Write a 15 argument message to HSL (just a message header, no data).
  * Write a 15 argument message to HSL (just a message header, no data).
  * \param Int16 hDR = Autogenerated Log point ID.
  * Equivalent to former LG_MES_6 macro.
  * \param Int16 mDFlOGiD = Log point ID kept for backward compatibility with MDF tool.
  * \param String fORMATsPECIFER = Format specifier string which resolves to nothing at compile time.
  * \param Int16 aRG1 = first argument value to log
  * \param Int16 aRG2 = second argument value to log
  * \param Int16 aRG3 = third argument value to log
  * \param Int16 aRG4 = fourth argument value to log
  * \param Int16 aRG5 = fifth argument value to log
  * \param Int16 aRG6 = sixth argument value to log
  * \param Int16 aRG7 = seventh argument value to log
  * \param Int16 aRG8 = eighth argument value to log
  * \param Int16 aRG9 = nineth argument value to log
  * \param Int16 aRG10 = tenth argument value to log
  * \param Int16 aRG11 = eleventh argument value to log
  * \param Int16 aRG12 = twelveth argument value to log
  * \param Int16 aRG13 = thirteenth argument value to log
  * \param Int16 aRG14 = fourteenth argument value to log
  * \param Int16 aRG15 = fifteenth argument value to log
  */
# define M_FrHslPrintf15(hDR,mDFlOGiD,fORMATsPECIFER, aRG1, aRG2, aRG3, aRG4, aRG5, aRG6, aRG7, aRG8,                     \
                                                      aRG9, aRG10, aRG11, aRG12, aRG13, aRG14, aRG15 )                    \
           if ( M_FrHslIsLogPointEnabled(hDR, mDFlOGiD) )                                                                 \
           {                                                                                                              \
                                M_FrHsl15ByteMsg(hDR, mDFlOGiD, aRG1, aRG2, aRG3, aRG4, aRG5, aRG6, aRG7, aRG8,                     \
                                                     aRG9, aRG10, aRG11, aRG12, aRG13, aRG14, aRG15 );                    \
           }

/** Write a 16 argument message to HSL (just a message header, no data).
  * Write a 16 argument message to HSL (just a message header, no data).
  * \param Int16 hDR = Autogenerated Log point ID.
  * Equivalent to former LG_MES_6 macro.
  * \param Int16 mDFlOGiD = Log point ID kept for backward compatibility with MDF tool.
  * \param String fORMATsPECIFER = Format specifier string which resolves to nothing at compile time.
  * \param Int16 aRG1 = first argument value to log
  * \param Int16 aRG2 = second argument value to log
  * \param Int16 aRG3 = third argument value to log
  * \param Int16 aRG4 = fourth argument value to log
  * \param Int16 aRG5 = fifth argument value to log
  * \param Int16 aRG6 = sixth argument value to log
  * \param Int16 aRG7 = seventh argument value to log
  * \param Int16 aRG8 = eighth argument value to log
  * \param Int16 aRG9 = nineth argument value to log
  * \param Int16 aRG10 = tenth argument value to log
  * \param Int16 aRG11 = eleventh argument value to log
  * \param Int16 aRG12 = twelveth argument value to log
  * \param Int16 aRG13 = thirteenth argument value to log
  * \param Int16 aRG14 = fourteenth argument value to log
  * \param Int16 aRG15 = fifteenth argument value to log
  * \param Int16 aRG16 = sixteenth argument value to log
  */
# define M_FrHslPrintf16(hDR,mDFlOGiD,fORMATsPECIFER, aRG1, aRG2, aRG3, aRG4, aRG5, aRG6, aRG7, aRG8,                     \
                                                      aRG9, aRG10, aRG11, aRG12, aRG13, aRG14, aRG15, aRG16 )             \
           M_FrPrimeLogCheckInterface(mDFlOGiD);\
           if ( M_FrHslIsLogPointEnabled(hDR, mDFlOGiD) )                                                                 \
           {                                                                                                              \
                                M_FrHsl16ByteMsg(hDR, mDFlOGiD, aRG1, aRG2, aRG3, aRG4, aRG5, aRG6, aRG7, aRG8,                     \
                                                     aRG9, aRG10, aRG11, aRG12, aRG13, aRG14, aRG15, aRG16 );             \
           }

#endif
/** Writes to the HSL SWMsg Last Word register.
*  Writes the last word of a multi-word message to this register.
*/
# define M_FrHslWriteSwMsgLastWord(vALUE, vALUE2)   ULS_WRITE(REG_ULS_END, (((Int32)((vALUE2)<<16))|(vALUE)))

/** Writes to the HSL SWMsg Body register.
*  Writes the middle words (but not the first or last) of a multi-word message to
*  this register.
*/
#  define M_FrHslWriteSwMsgBody(vALUE, vALUE2)      ULS_WRITE(REG_ULS_BODY, (((Int32)(vALUE2<<16))|(vALUE)) )

/** Writes to the HSL SWMsg Header register.
*  Writes the first word of a multi-word message to this register.
*/
# define M_FrHslWriteSwMsgHeader(vALUE, len)       ULS_WRITE(REG_ULS_START, (((Int32)(vALUE<<16))|(len)) )

/** Writes to the HSL SWMsg Single register.
*  Writes single-word logpoints to this register.  Note that the LG_SINGLE_WORD
*  bit (0x2000) must be set in the header value written.  This is essential
*  to allow the message nesting scheme to operate correctly.
*/
# define M_FrHslWriteSwMsgSingle(vALUE)     ULS_WRITE(REG_ULS_SINGLE, vALUE)

#endif //_ULS_CONFIG_H_
