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
 **************************************************************************/
/** \file
 * Types common across the whole system.
 **************************************************************************/
#ifndef SYSTEM_H
#define SYSTEM_H

/****************************************************************************
 * Nested Include Files
 ****************************************************************************/

#include    <limits.h>
#include "memory_attribute.h"

#if !defined (TTCONFIG_H)
#include <ttconfig.h>
#endif

#if defined(__STDC__) && defined(__STDC_VERSION__)
	#ifndef __STDC_CONSTANT_MACROS
		#define __STDC_CONSTANT_MACROS
	#endif
	#ifndef __STDC_LIMIT_MACROS
		#define __STDC_LIMIT_MACROS
	#endif
	
	#include <stdint.h>
	#define SYSTEMH_HAVE_STDINT

	#include <stdbool.h>
	#define SYSTEMH_HAVE_STDBOOL

#elif defined(__GNUC__) && (__GNUC__>3 || (__GNUC__==3 && __GNUC_MINOR__>=2))
    #ifndef __STDC_CONSTANT_MACROS
        #define __STDC_CONSTANT_MACROS
    #endif
    #ifndef __STDC_LIMIT_MACROS
        #define __STDC_LIMIT_MACROS
    #endif
    #include <stdint.h>
    #include <stdbool.h>
    #define SYSTEMH_HAVE_STDINT
#elif defined(__PGI)
    #ifndef __STDC_CONSTANT_MACROS
        #define __STDC_CONSTANT_MACROS
    #endif
    #ifndef __STDC_LIMIT_MACROS
        #define __STDC_LIMIT_MACROS
    #endif
    #include <stdint.h>
    #define SYSTEMH_HAVE_STDINT
#elif defined(_MSC_VER) && _MSC_VER>=1600
    #ifndef __STDC_CONSTANT_MACROS
        #define __STDC_CONSTANT_MACROS
    #endif
    #ifndef __STDC_LIMIT_MACROS
        #define __STDC_LIMIT_MACROS
    #endif
    #include <stdint.h>
    #define SYSTEMH_HAVE_STDINT
    /* do not define SYSTEMH_HAVE_STDBOOL as VC10 doesn't define _Bool */
#if !defined(__cplusplus) 
    #include <stdbool.h>
#endif
#endif

/****************************************************************************
 * Defines
 ****************************************************************************/
#if defined (ULS_FUNCTION_PROFILER)
ATTR_MD_TEXT_IN_TCM void __cyg_profile_func_enter(void *func, void *caller);
ATTR_MD_TEXT_IN_TCM void __cyg_profile_func_exit(void *func, void *caller);
#endif
/*
** The NULL POINTER
*/
#if defined(__cplusplus) || defined (__TMS470__) || (__ARMCC_VERSION >= 200000) ||  defined(_MSC_VER)
/** Null pointer type. */
#define      PNULL                   0
#else
/** Null pointer type. */
#define      PNULL                   ((void *) 0)
#endif


/** Value to force minimum enumeration size.
 * This value can be used in enumerations to force them to be at least
 * 16Bit wide (but the compiler may only support bigger enums).
 */
#define     FORCE_ENUM_TO_INT16      0xffff

/** Number of bits in an Int8 */
#define BITS_PER_INT8                8

/** Number of Int8's in an Int16 */
#define INT8S_PER_INT16              2

/** Number of bits in an Int16 */
#define BITS_PER_INT16               (BITS_PER_INT8 * INT8S_PER_INT16)

/** Number of Int8's in an Int32 */
#define INT8S_PER_INT32              4

/** Number of Int8's in an Int32 */
#define INT8S_PER_INT64              8

/** Number of bits in an Int32 */
#define BITS_PER_INT32               (BITS_PER_INT8 * INT8S_PER_INT32)

/** Number of bits in an Int32 */
#define BITS_PER_INT64               (BITS_PER_INT8 * INT8S_PER_INT64)

/****************************************************************************
 * Macros
 ****************************************************************************/

#if defined (FUNCTION_CALL_TRACING)

/** Function trace entry hook.
 * Calls \a traceEntry with the file, line and the function name.
 */
# define ENTRY(fcn)     traceEntry (__FILE__, __LINE__, #fcn)

/** Function trace exit hook.
 * Calls \a traceExit with the file, line and the function name.
 */
# define EXIT(fcn)      traceExit (__FILE__, __LINE__, #fcn)

#else
# define ENTRY(m)
# define EXIT(m)
#endif

/** Maximum value function.
 * \retval   The maximum of the two supplied values.
 * \warning  If either \a a or \a b are not constants, they risk being
 *           evaluated multiple times.  For example the following is unsafe:
 *           \code
 *             v = MAX_VALUE(x++, y)
 *           \endcode
 */
#define MAX_VALUE(a,b)        (((a) > (b)) ? (a) : (b))

/** Minimum value function.
 * \retval   The minimum of the two supplied values.
 * \warning  If either \a a or \a b are not constants, they risk being
 *           evaluated multiple times.  For example the following is unsafe:
 *           \code
 *             v = MIN_VALUE(x++, y)
 *           \endcode
 */
#define MIN_VALUE(a,b)        (((a) < (b)) ? (a) : (b))

/** Absolute value function.
 * \retval   The absolute of the supplied value.
 * \warning  If \a a is not constant, it risks being evaluated multiple times.
 *           For example the following is unsafe:
 *           \code
 *             v = ABS_VALUE(x++)
 *           \endcode
 */
#define ABS_VALUE(a)          (((a) < 0) ? (-(a)) : (a))

/** Pretty shift function.
 * Left shift 1 by some number of places.
 */
#define BIT_SHIFT_LEFT1(x)    (1<<(x))

/** Get the number of Int8's needed to store some number of bits.
 * This will return the minimum number of Int8's needed to store some
 * number of individual bits.
 */
#define BITS_TO_INT8S(bits)   (((bits) + BITS_PER_INT8 - 1)/BITS_PER_INT8)

#define BITS_TO_INT32S(bits)  (((bits) + BITS_PER_INT32 - 1)/BITS_PER_INT32)

/* For sprintf the code must be use the secure version which specifies the
 * max number of bytes to write to the buffer. By default the snprintf
 * version is used but for MSVC the function is _snprintf.
 */
#if defined (_MSC_VER)
#define snprintf   _snprintf
#endif
/****************************************************************************
 * C Types
 ****************************************************************************/

/*** Char *********************************************************/
#if !defined (SYS_DONT_TYPE_CHAR)
/** 8 bit unsigned integer for containing ascii values. */
typedef unsigned char Char;
#endif

/*** Int8 *********************************************************/

#if defined(SYSTEMH_HAVE_STDINT)
typedef uint8_t Int8;                   /**< 8-bit unsigned integer value. */
typedef int8_t  SignedInt8;             /**< 8-bit signed integer value. */
#else /* not SYSTEMH_HAVE_STDINT */
typedef unsigned char Int8;             /**< 8-bit unsigned integer value. */
typedef   signed char SignedInt8;       /**< 8-bit signed integer value. */
#endif /* SYSTEMH_HAVE_STDINT */

/*** Int16 ********************************************************/

/*
** These two types MUST always be 16 bit values. The following
** pre-processor commands will trap any compiler which does not
** use 16 bits for an unsigned short. For C99 or GCC we use the
** stdint.h types uint16_t and int16_t.
*/
#if defined(SYSTEMH_HAVE_STDINT)
typedef uint16_t Int16;                 /**< 16-bit unsigned integer value. */
typedef int16_t  SignedInt16;           /**< 16-bit signed integer value. */
#else /* not SYSTEMH_HAVE_STDINT */
#if (USHRT_MAX == 0xFFFFU)
typedef unsigned short Int16;           /**< 16-bit unsigned integer value. */
typedef   signed short SignedInt16;     /**< 16-bit signed integer value. */
#else
# error "short does not contain 16 bits"
#endif
#endif /* SYSTEMH_HAVE_STDINT */

/*** Int32 ********************************************************/

/*
** These two types MUST always be 32 bit values. The following
** pre-processor commands will trap any compiler which does not
** use 32 bits for an unsigned long. For C99 or GCC we use the
** stdint.h types uint32_t and int32_t.
*/
#if defined(SYSTEMH_HAVE_STDINT)
typedef uint32_t Int32;                 /**< 32-bit unsigned integer value. */
typedef int32_t  SignedInt32;           /**< 32-bit signed integer value. */
#else /* not SYSTEMH_HAVE_STDINT */
# if (ULONG_MAX == 0xFFFFFFFFL)
typedef unsigned long Int32;            /**< 32-bit unsigned integer value. */
typedef   signed long SignedInt32;      /**< 32-bit signed integer value. */
# else
/*
** This is for the CANTATA preprocessor which doesn't think
** 0xFFFFFFFFL equals 4294967295UL (yet it is quite happy with USHRT_MAX)
** weird!
*/
#  if (ULONG_MAX == 4294967295UL)
typedef unsigned long Int32;            /**< 32-bit unsigned integer value. */
typedef   signed long SignedInt32;      /**< 32-bit signed integer value. */
#  else
#   if (UINT_MAX == 0xFFFFFFFFL)
typedef unsigned int Int32;             /**< 32-bit unsigned integer value. */
typedef   signed int SignedInt32;       /**< 32-bit signed integer value. */
#   else
#    error "Cannot define an Int32"
#   endif
#  endif
# endif
#endif /* SYSTEMH_HAVE_STDINT */


/* these macro should be used to declare constants as 32bits without relying on native type
 * the use of suffix L is ANSI C compliant but is not understood by some asm compilers,
 * they could also be redefined based on compiler */
#if !defined(SYS_SINT32)
# if !defined (__HIGHC__)
# if !defined (ASM_BLOCK_TYPEDEFS)
#   define SYS_SINT32(VAR) VAR##L
# else
#   define SYS_SINT32(VAR) VAR
# endif
# else
#   define SYS_SINT32(VAR) VAR
# endif
#endif

#if !defined(SYS_UINT32)
# if !defined (__HIGHC__)
# if !defined (ASM_BLOCK_TYPEDEFS)
#   define SYS_UINT32(VAR) VAR##UL
# else
#   define SYS_UINT32(VAR) VAR
# endif
# else
#   define SYS_UINT32(VAR) VAR
# endif
#endif


/*** Int64 ********************************************************/

/*
** These two types MUST always be 64 bit values. The following
** pre-processor commands will trap any compiler which does not
** use 64 bits for an unsigned long. For C99 or GCC we use the
** stdint.h types uint64_t and int64_t.
*/

#if defined(SYSTEMH_HAVE_STDINT)
    /* most modern compilers have <stdint.h> */

    /* try to guess whether stdint.h defines a 64-bit type */
    #if defined(INT64_MIN) && defined(INT64_MAX)
        /* these are optional types in <stdint.h> */
        typedef uint64_t Int64;             /**< stdint.h 64-bit unsigned integer */
        typedef int64_t  SignedInt64;       /**< stdint.h 64-bit integer */
    #endif

#elif defined(_MSC_VER) || defined(__BORLANDC__)
    typedef unsigned __int64 Int64;     /**< Visual C++ 64-bit unsigned integer */
    typedef __int64  SignedInt64;       /**< Visual C++ 64-bit integer */
    #if !defined(INT64_C)
        #define INT64_C(c)  c##LL     /**< macro to make a 64-bit literal (from ISO/IEC 9899:1999 section 7.18) */
        #define UINT64_C(c) c##ULL    /**< macro to make a unsigned 64-bit literal (from ISO/IEC 9899:1999 section 7.18) */
    #endif /* INT64_C */

#elif defined(__CEVAX16__)
    #define Int64           Int64_not_supported_by_CEVAX
    #define SignedInt64     Int64_not_supported_by_CEVAX

#elif defined(__HIGHC__) || defined(__GNUC__)
    typedef signed long long SignedInt64; /**< 64-bit signed integer */
    typedef unsigned long long Int64;     /**< 64-bit unsigned integer */

    #if !defined(INT64_C)
        #define INT64_C(c)  c##LL     /**< macro to make a 64-bit literal (from ISO/IEC 9899:1999 section 7.18) */
        #define UINT64_C(c) c##ULL    /**< macro to make a unsigned 64-bit literal (from ISO/IEC 9899:1999 section 7.18) */
    #endif /* INT64_C */

#elif defined(__ARMCC_VERSION) && __ARMCC_VERSION>=200000
    typedef signed long long SignedInt64; /**< 64-bit signed integer */
    typedef unsigned long long Int64;     /**< 64-bit unsigned integer */

    #if !defined(INT64_C)
        #define INT64_C(c)  c##LL     /**< macro to make a 64-bit literal (from ISO/IEC 9899:1999 section 7.18) */
        #define UINT64_C(c) c##ULL    /**< macro to make a unsigned 64-bit literal (from ISO/IEC 9899:1999 section 7.18) */
    #endif /* INT64_C */

#endif


/*** intptr_t *****************************************************/

#if defined(SYSTEMH_HAVE_STDINT)
    /* most modern compilers have <stdint.h>
     * no need to do anything, stdint.h defines all the types we need.
     */

#elif defined(_MSC_VER)
    /* Microsoft Visual C C++ */
    #if defined(_WIN64)
        typedef __int64 intptr_t;
        typedef unsigned __int64 uintptr_t;
    #else
        typedef int intptr_t;
        typedef unsigned int uintptr_t;
    #endif

#elif defined(__BORLANDC__)
    /* Borland C++Builder */
    typedef int intptr_t;
    typedef unsigned int uintptr_t;

#elif defined( __ARMCC_VERSION)
    /* ARM RealView C */
    typedef int intptr_t;
    typedef unsigned int uintptr_t;

#elif defined(__CEVAX16__)
    /* CEVA DSP, gcc-based */
    typedef int intptr_t;
    typedef unsigned int uintptr_t;

#elif defined(__SIZEOF_POINTER__) && defined(__SIZEOF_INT__) && __SIZEOF_POINTER__==__SIZEOF_INT__
    /* generic gcc ILP32 target */
    typedef int intptr_t;
    typedef unsigned int uintptr_t;

#elif defined(__SIZEOF_POINTER__) && defined(__SIZEOF_LONG__) && __SIZEOF_POINTER__==__SIZEOF_LONG__
    /* generic gcc LP32 or LP64 target */
    typedef long intptr_t;
    typedef unsigned long uintptr_t;

#else
    #error "system.h needs to have intptr_t defined for this target"

#endif

/*** Boolean ******************************************************/
#if !defined (SYS_USE_ENUM_BOOLEAN)

#if defined(SYSTEMH_HAVE_STDBOOL)
/* For C99 we use IEEE Std 1003.1-2001 type _Bool */
typedef _Bool Boolean;              /**< Basic type for storing Boolean values */

#  if !defined (FALSE)
/** Logical false value. */
#   define FALSE false
#  endif

#  if !defined (TRUE)
/** Logical true value. */
#   define TRUE  true
#  endif

# else /* not C99 */

#  if defined(__cplusplus)
/* For C++ we use ISO/IEC 14882-1998 type bool */
typedef bool Boolean;               /**< Basic type for storing Boolean values */

#   if !defined (FALSE)
/** Logical false value. */
#    define FALSE false
#   endif

#   if !defined (TRUE)
/** Logical true value. */
#    define TRUE  true
#   endif

#  else /* not C++ */

/** Basic type for storing Boolean values.
 */
typedef Int8 Boolean;

#   if !defined (FALSE)
/** Logical false value. */
#    define FALSE (0)
#   endif

#   if !defined (TRUE)
/** Logical true value. */
#    define TRUE  (1)
#   endif

#  endif /* C++ */
# endif /* C99 */

#else /* SYS_USE_ENUM_BOOLEAN */

/* Windows defines its own FALSE and TRUE as constants */
# if !defined (__WINDOWS_H) && !defined (_INC_WINDOWS)

/** Boolean enumeration.
 * This has the advantage that debuggers can display the value as a
 * meaningful enumeration but may have an efficiency impact if a
 * small enum is not naturally an 8-bit value.
 */
typedef enum BooleanTag
{
    FALSE = (1==0),     /**< Logical false value */
    TRUE  = (1==1)      /**< Logical true value */
}
Boolean;

# else
typedef int Boolean;    /**< Basic type for storing Boolean values. */
# endif /* not seen windows.h yet */
#endif /* SYS_USE_INT8_BOOLEAN */

/****************************************************************************
 * For builds without C99 types
 ****************************************************************************/

#if defined(_MSC_VER) && _MSC_VER<1600 && !defined(__cplusplus)
#ifndef uint8_t
typedef Int8          uint8_t;
#endif
#ifndef int8_t
typedef SignedInt8    int8_t;
#endif
#ifndef uint16_t
typedef Int16         uint16_t;
#endif
#ifndef int16_t
typedef SignedInt16   int16_t;
#endif
#ifndef uint32_t
typedef Int32         uint32_t;
#endif
#ifndef int32_t
typedef SignedInt32   int32_t;
#endif
#ifndef uint64_t
typedef Int64         uint64_t;
#endif
#ifndef int64_t
typedef SignedInt64   int64_t;
#endif
#ifndef bool
typedef Boolean       bool;
#endif
#ifndef true
#define true TRUE
#endif
#ifndef false
#define false FALSE
#endif
#endif

/****************************************************************************
 * C++ Types
 ****************************************************************************/

/* These are C++ types used in upper layers and in some cases
 * code that interfaces to C++ */
typedef SignedInt8  TP_SInt8;
typedef SignedInt16 TP_SInt16;
typedef SignedInt32 TP_SInt32;
typedef Int8        TP_UInt8;
typedef Int16       TP_UInt16;
typedef Int32       TP_UInt32;
typedef Boolean     TP_Boolean;
typedef char *      TP_String; /* There is no base-type of 'String' as this
                                * may conflict with definitions of String
                                * on other platforms (e.g. Windows) */
typedef char        TP_Char;   /**< \note This is SIGNED as opposed to #Char which is UNSIGNED */
typedef TP_UInt16   TP_WChar;  /**< A 16 bit unicode character (wide) */

typedef TP_WChar    TP_TChar;
#define TP_PNULL    ((void *) 0)

/****************************************************************************
 * Misc types
 ****************************************************************************/

/** This enum should be aligned with the union elements in SysBasicTypesUnion.
 * This is used by Genie and other GKIS applications to interpret the contents
 * of SysBasicTypesUnion.
 */
typedef enum SysBasicTypeEnumTag
{
    SYS_BASIC_TYPE_INT8MEMBER,
    SYS_BASIC_TYPE_SIGNEDINT8MEMBER,
    SYS_BASIC_TYPE_INT16MEMBER,
    SYS_BASIC_TYPE_SIGNEDINT16MEMBER,
    SYS_BASIC_TYPE_INT32MEMBER,
    SYS_BASIC_TYPE_SIGNEDINT32MEMBER,
    SYS_BASIC_TYPE_BOOLEANMEMBER,
    SYS_BASIC_TYPE_POINTERMEMBER,
    SYS_BASIC_TYPE_TP_SINT8MEMBER,
    SYS_BASIC_TYPE_TP_SINT16MEMBER,
    SYS_BASIC_TYPE_TP_SINT32MEMBER,
    SYS_BASIC_TYPE_TP_UINT8MEMBER,
    SYS_BASIC_TYPE_TP_UINT16MEMBER,
    SYS_BASIC_TYPE_TP_UINT32MEMBER,
    SYS_BASIC_TYPE_TP_BOOLEANMEMBER,
    SYS_BASIC_TYPE_TP_STRINGMEMBER,
    SYS_BASIC_TYPE_TP_CHARMEMBER,
    SYS_BASIC_TYPE_TP_WCHARMEMBER,
    SYS_BASIC_TYPE_TP_TCHARMEMBER
}SysBasicTypeEnum;

/** Union of all the basic system types.
 * This can be used to ensure correct alignment of data arrays that could have
 * these types cast onto it. This is normally achieved by unionising this
 * union with the data array, this makes sure that the data array is suitably
 * aligned to allow the use of any of these types. If a new basic type is added
 * to the system (i.e. in system.h) then it should be added to this union.
 */
typedef union SysBasicTypesUnionTag
{
    Int8         int8Member;
    SignedInt8   signedInt8Member;
    Int16        int16Member;
    SignedInt16  signedInt16Member;
    Int32        int32Member;
    SignedInt32  signedInt32Member;
	Int64		 int64Member;
	SignedInt64  signedInt64Member;
    Boolean      booleanMember;
    void        *pointerMember;

    TP_SInt8     tp_SInt8Member;
    TP_SInt16    tp_SInt16Member;
    TP_SInt32    tp_SInt32Member;
    TP_UInt8     tp_UInt8Member;
    TP_UInt16    tp_UInt16Member;
    TP_UInt32    tp_UInt32Member;
    TP_Boolean   tp_BooleanMember;
    TP_String    tp_StringMember;
    TP_Char      tp_CharMember;
    TP_WChar     tp_WCharMember;
    TP_TChar     tp_TCharMember;
} SysBasicTypesUnion;

/** Structure containing a union of all the basic system types along with an
 * enum which indicates how to interpret the contents of that union.
 */
typedef struct SysBasicTypesStructTag
{
/** \assoc UNION \ref value */
    SysBasicTypeEnum   valueType;
    SysBasicTypesUnion value;
} SysBasicTypesStruct;

/****************************************************************************
 * Nested Include Files
 ****************************************************************************/

#if defined(UPGRADE_STRIPPER_HAS_BEEN_RUN)
/* Intentionally blank */
#else
/* This block will be removed by the stripper.
 *  If it is found that stripperinit.h is missing from a build and causing
 *  compile failure, it is likely that the source has been fetched using
 *  the CAT/FLS structure, but not yet stripped.  stripperinit.h is
 *  deliberately not included in CAT or FLS and must not be released to
 *  clients; stripping the code removes the need for this file.
 */
#include <stripperinit.h>
#endif

/* EXSYSTEM.H is deliberately put here so the above basic types can be used
** within the header file. */
#if !defined (EXSYSTEM_H)
#include <exsystem.h>
#endif

#if defined (UPGRADE_DSP_R2)

#define _       0
#define __      0
#define ___     0

/*
  Define enum for PAMAC
*/
#undef MK_PAMAC_OPCODE_AMAC
#define MK_PAMAC_OPCODE_AMAC(mac0_op, mac0_unsigned, mac0_fmode, mac0_read, mac0_ab_size, mac0_reg,  \
                             mac1_op, mac1_unsigned, mac1_fmode, mac1_read, mac1_ab_size, mac1_reg ) \
    mac0_op ## mac0_unsigned ## mac0_fmode ## mac0_read ## mac0_ab_size ## mac0_reg ## __ ## mac1_op ## mac1_unsigned ## mac1_fmode ## mac1_read ## mac1_ab_size ## mac1_reg

#undef MK_PAMAC_SOURCE
#define MK_PAMAC_SOURCE(mac0_qa, mac0_qamem, mac0_qb, mac0_qbmem, mac1_qa, mac1_qamem, mac1_qb, mac1_qbmem) \
    mac0_qa ## _ ## mac0_qamem ## mac0_qb ## _ ## mac0_qbmem ## __ ## mac1_qa ## _ ## mac1_qamem ## mac1_qb ## _ ## mac1_qbmem

#undef MK_PAMAC_DEST
#define MK_PAMAC_DEST(mac0_q, mac0_qmem, mac1_q, mac1_qmem, round_plus1, shift_plus1, saturate_plus1 ) \
    mac0_q ## _ ## mac0_qmem ## _ ## mac1_q ## _ ## mac1_qmem ## round_plus1 ## shift_plus1 ## saturate_plus1


typedef enum
{
#define _PAMAC_DEFINE_OPCODE
#include "pamac.h"
#undef _PAMAC_DEFINE_OPCODE
} pamac_table_entry_opcode_t;

typedef enum
{
#define _PAMAC_DEFINE_SOURCE
#include "pamac.h"
#undef _PAMAC_DEFINE_SOURCE
} pamac_table_entry_source_t;

typedef enum
{
#define _PAMAC_DEFINE_DEST
#include "pamac.h"
#undef _PAMAC_DEFINE_DEST
} pamac_table_entry_dest_t;

#define L_PAMAC_SYNC    \
{                       \
    asm("l.nop");       \
    asm("l.nop");       \
    asm("l.nop");       \
    asm("l.nop");       \
}


#endif /* UPGRADE_DSP_R2 */

#ifdef _MSC_VER
#define L_BTB(id,h,b,t)
#define __asm__(x)
#endif

#if !defined(R2_FORCE_LONG_CALL)
 #define __FAR__ __attribute__((far))
#else
 #define __FAR__
#endif /* !R2_FORCE_LONG_CALL */

#if defined(ENABLE_PLTSIM_R2_FPGA)

#ifdef  __cplusplus
extern "C" {
#endif

int UTL_printf(const char *pFmt, ...);
#ifndef printf
#define printf      UTL_printf
#endif

#ifdef  __cplusplus
}
#endif


#endif /* ENABLE_PLTSIM_R2_FPGA */

/* macros for section placement.
 * They are use to abstract compiler specific syntax.
 * Each module can define section placement for initialisation code/data
 * and critical code/data (see frcfg.h for an example)
 */
#if (defined(__GNUC__) || (__ARMCC_VERSION)) && !defined (__MINGW32__) && !defined(__CEVAX16__)
#define SECTION(S) __attribute__ ((section(S)))
#else
#define SECTION(S)
#endif

#endif
/* END OF FILE */
