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
 * Configurable items across the whole system (must be application
 * independent). This is mainly the development assertion macros.
 **************************************************************************/

#ifndef EXSYSTEM_H
#define EXSYSTEM_H

/** \defgroup CfgGkiAssert GKI Warning and Assert Configuration
 * \ingroup Cfg
 * \ingroup PrdGki
 */

/* Assert always fatal in target */
#ifdef MTK_NBIOT_TARGET_BUILD
#define AssertHandler AssertHandlerNoReturn
#endif

 /****************************************************************************
 * Gnrl R2 debug kit
 ****************************************************************************/
#if defined(R2DEBUGKIT_ON)
#include <r2_dbgToolKit.h>
#endif

/****************************************************************************
 * Configuration
 ****************************************************************************/

/** Assertion checking and reporting configuration.
 * Checks are placed throughout the code to ensure that where possible, values
 * and conditions meet predefined criteria such that erroneous states can be
 * detected and reported as soon as possible.  These checks utilise the
 * \ref ApiGkiAssert "GKI Assertion API", and when a check fails or some error
 * condition is met, an action as configured by these switches is taken.
 *
 * In general assertions are reported by dumping the MODULE_NAME and line
 * number to the screen together with any other parameters that can be
 * reported through the assertion API.  However, the options described in this
 * section allow for simple changes in the assertion behaviour, without the
 * need to change the assertion handling code.
 *
 * In a DEVELOPMENT_VERSION build the default is for both warnings and
 * assertions to be checked. The default for warnings is to
 * generate a KI_WARN_*_IND signal and continue. The default for assertions
 * is to display assert information on the target screen and
 * halt the system.
 *
 * In a non development build the current default is not to check assertions
 * or warnings. This will change in the future to have assertions still checked
 * and warnings not checked. Warn macros should only be used where there is
 * recovery code. In a release build assert checking should still be enabled
 * so that the system is stopped (as there is no recovery code after this point)
 * but there are various options to reduce the impact of having assert checking
 * enabled in the build see #SHORT_DEV_ASSERTS and #KI_ASSERT_REMOVE_FILE_AND_LINE
 * @{
 */

/** \def KI_CHECK_ASSERTIONS
 * Explicitly enable assert checking.
 * When this macro is defined, assertions will be checked and reported as
 * needed.
 *
 * If this is not defined the assertion macros will expand to nothing,
 * resulting is slightly smaller code and marginally faster execution.
 *
 * \note This macro is automatically defined if #DEVELOPMENT_VERSION is
 *       defined i.e. #DEVELOPMENT_VERSION implies that assertions are
 *       checked.
 *
 * \see FatalCheck, FatalParam, FatalFail, FatalAssert, KI_DONT_CHECK_ASSERTIONS
 */

/** \def KI_CHECK_WARNINGS
 * Explicitly enable warning checking.
 * When this macro is defined, warnings will be checked and reported as
 * needed. Assertion checking must also be enabled to allow checking
 * of warnings.
 *
 * If this is not defined, the
 * warning macros will expand to nothing, resulting is slightly smaller
 * code and marginally faster execution.
 *
 * \note This macro is automatically defined if #DEVELOPMENT_VERSION is
 *       defined i.e. #DEVELOPMENT_VERSION implies that warnings are
 *       checked.
 *
 * \see WarnCheck, WarnParam, WarnFail, WarnAssert
 */

/** \def KI_DONT_CHECK_ASSERTIONS
 * Disable assert checking.
 * This switch explicitly disables assertion checking, even if
 * #KI_CHECK_ASSERTIONS or #DEVELOPMENT_VERSION is defined.
 *
 * \warning Assertion checking should be enabled during development such
 *          that faults in the system can be identified and reported.
 *          Developing without assertion checking can lead to problems that
 *          may initially be unseen, and can cause system instability.
 *
 * \see KI_CHECK_ASSERTIONS
 */

/** \def KI_DONT_CHECK_WARNINGS
 * Disable warning checking.
 * This switch explicitly disables warning checking, even if
 * #KI_CHECK_WARNINGS or #DEVELOPMENT_VERSION is defined.
 *
 * \warning Warning checking should be enabled during development such
 *          that faults in the system can be identified and reported.
 *          Developing without warning checking can lead to problems that
 *          may initially be unseen, and can cause system instability.
 *
 * \see KI_CHECK_WARNINGS
 */

/** \def DEVELOPMENT_VERSION
 * Enable development build.
 * This conditional enables the development version of a build. This will
 * implicitly enable assert (#KI_CHECK_ASSERTIONS) and warning checking
 * (#KI_CHECK_WARNINGS). It may also enable other development code throughout
 * the system. */

/** \def KI_ASSERT_REMOVE_FILE_AND_LINE
 * Remove file and line information from warnings and asserts.
 * The KI_ASSERT_REMOVE_FILE_AND_LINE conditional removes the file and
 * line information from warnings and asserts. This can be used in a
 * release build to further reduce the ROM impact of having assert
 * checking present. But it should only be used if there is some way
 * of retrieving the caller address at run time otherwise the assert
 * information will be useless. */

/** \def SYS_DETECT_MISSING_MODULE_NAME
 * Prevent this file defining a default MODULE_NAME if none exists.
 * This is useful for tracking down source files that fail to define
 * MODULE_NAME, but rely on it's expansion.
 *
 * \note We cannot #error if SYS_DETECT_MISSING_MODULE_NAME is defined and
 *       MODULE_NAME is not as this is valid in the case of things that
 *       process header files only, for example signal database creation.
 */

/** \def SHORT_DEV_ASSERTS
 * Remove condition strings from warnings and asserts.
 * The SHORT_DEV_ASSERTS conditional removes the condition strings from
 * #FatalCheck, #FatalAssert, #WarnCheck and #WarnAssert and the string from
 * #FatalFail and #WarnFail. This will reduce how much ROM is used in the
 * system but it does reduce the amount of debug information especially
 * in the case of #FatalFail and #WarnFail (only get file and line number).
 * This is another reason why it is always best to use the
 * #FatalCheck/#WarnCheck and #FatalParam/#WarnParam macros instead of
 * #FatalAssert/#WarnAssert and #FatalFail/#WarnFail macros.
 */

#ifdef MTK_NBIOT_TARGET_BUILD
#define SHORT_DEV_ASSERTS
#endif
/**************************************************************************
 * Defines that documented in this header file but not used here.
 **************************************************************************/

/** \def CONTINUE_ON_ASSERTFAIL
 * Do not stop the system on an assert failure.
 * This conditional compile causes the system not to stop when an assert occurs,
 * and hence should only really be used if you are sure the asserts you are
 * wanting to capture allow the system to continue.  If it is not safe to
 * continue there is no guarantee that the assert information will still be
 * visible or logged if the target fails completely.
 *
 * \see KI_USE_ASSERT_SIGNALS
 */

/** \def KI_USE_ASSERT_SIGNALS
 * Reports assert failures by GKI signals that can be logged within a test tool.
 * When this is enabled the system continues after an assert, as hence should
 * only be used if you are sure the asserts you are wanting to log allow the
 * system to continue.  If it is not safe to continue there is no guarantee that
 * the assert signal will get off the target before the target fails completely.
 *
 * \note Since this macro requires the system to continue, use of
 *       #KI_USE_ASSERT_SIGNALS it automatically implies that
 *       #CONTINUE_ON_ASSERTFAIL is defined.
 */

/** \def DONT_DISPLAY_ASSERTFAIL_MSG
 * Prevent display of assertion messages on the system's screen.
 * The default implementation of asShowFailure() will display the assertion
 * message on the system's screen.  This can be prevented by defining this
 * macro, noting that in such a case a different way to observe assertion
 * failures should be enabled or implemented.
 *
 * This macro can be useful in situations where continuing on assertion
 * failures is desired, but a slow display driver can cause the assertion
 * rendering to the display to make the system unstable.  For example, it
 * may not be possible to continue from an assertion in an interrupt if the
 * display driver is exceptionally slow.
 *
 * \note This option is only of use if some other mechanism of logging or
 *       recording assertion failures is also defined.  If you wish to simply
 *       suppress all assertion failure warnings, #KI_DONT_CHECK_ASSERTIONS is
 *       the correct way to achieve this.
 */

/****************************************************************************
 * Deprecated types
 ****************************************************************************/

/* Deprecated enumeration to identify the type of assert */
typedef enum DevAssertFailTypeTag
{
    DAF_TYPE_ASSERT_LONG,
    DAF_TYPE_ASSERT_SHORT,
    DAF_TYPE_FAIL_LONG,
    DAF_TYPE_FAIL_SHORT
}
DevAssertFailType;

/****************************************************************************
 * Defines
 ****************************************************************************/

/** \def MODULE_NAME
 * Short name for a source file used for assert reporting.
 * All source files should define this macro prior to including system.h such
 * that asserts can be reported in a meaningful way.  If this macro is not
 * defined prior to inclusion of system.h, a default definition of \a __FILE__
 * is supplied which is probably less concise than just the filename, and hence
 * wastes ROM storage.
 */

#if defined (ENABLE_FULL_MODULE_NAME_IN_ASSERTS)
    #undef MODULE_NAME
    #define MODULE_NAME __FILE__
#elif !defined(MODULE_NAME)
    #if !defined (SYS_DETECT_MISSING_MODULE_NAME)
        #if defined(__ARMCC_VERSION) && __ARMCC_VERSION >= 210000
           /* Avoid listing full path which may be truncated on the display as well as
            *  using more ROM.  Instead use the ARM specific define to obtain just the
            *  filename.
            */
            #define MODULE_NAME __MODULE__
        #else
           /* Define default long module name */
            #define MODULE_NAME __FILE__
        #endif
    #endif /* SYS_DETECT_MISSING_MODULE_NAME */
#endif /* ENABLE_FULL_MODULE_NAME_IN_ASSERTS */


/* -------------------------------------------------------------------------- */

/* Decide whether or not to enable assertions and warnings
 * The below code attempts to reduce all define interactions to simple states
 * for KI_CHECK_ASSERTIONS (defined or not defined) and KI_CHECK_WARNINGS
 * (defined or not defined).
 *
 * Logic for the defines should follow the following...
 *
 * Top priority is given to...
 *  - KI_CHECK_ASSERTIONS
 *  - KI_CHECK_WARNINGS
 *  - KI_DONT_CHECK_ASSERTIONS
 *  - KI_DONT_CHECK_WARNINGS
 *
 * Conflicting above defines cause compile error
 * (e.g. KI_CHECK_ASSERTIONS + KI_DONT_CHECK_ASSERTIONS)
 *
 * To be able to use KI_CHECK_WARNINGS, KI_CHECK_ASSERTIONS must be defined.
 *
 * If none of above defined then the debug/development options
 * are handled as such..
 *
 *  |   NDEBUG   |   DEVELOPMENT_VERSION   |  Asserts Checked?  |  Warnings Checked?  |
 *  |  !defined  |        !defined         |        Yes         |          No         |
 *  |  !defined  |         defined         |        Yes         |         Yes         |
 *  |   defined  |        !defined         |         No         |          No         |
 *  |   defined  |         defined         |        Yes         |         Yes         |
 */

/* Check for incompatible values */
#if (defined(KI_CHECK_ASSERTIONS) && defined(KI_DONT_CHECK_ASSERTIONS)) || \
    (defined(KI_CHECK_WARNINGS) && defined(KI_DONT_CHECK_WARNINGS))
    #error "Conflicting assertions and warnings settings"
#endif

#if defined(KI_DONT_CHECK_ASSERTIONS)
    #undef KI_CHECK_ASSERTIONS
#elif !defined(KI_CHECK_ASSERTIONS) && (defined(DEVELOPMENT_VERSION) || !defined(NDEBUG))
    #define KI_CHECK_ASSERTIONS
#endif

#if defined(KI_DONT_CHECK_WARNINGS) || !defined(KI_CHECK_ASSERTIONS)
    #undef KI_CHECK_WARNINGS
#elif !defined(KI_CHECK_WARNINGS) && defined(DEVELOPMENT_VERSION)
    #define KI_CHECK_WARNINGS
#endif

/* Note for Callstack unwind process
   For functions which used infinit loop to stop
   the system please use:
   
      while(OPT_TRUE)
      
   Instead of "while(TRUE)"
*/
#if defined(R2DEBUGKIT_ON)
#define OPT_TRUE _global_volatile_true_
#else
#define OPT_TRUE TRUE
#endif

/** @} */ /* End of CfgGkiAssert */

/****************************************************************************
 * Function prototypes
 ****************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif

#if defined(__GNUC__)
    #define AF_NORETURN_ __attribute__((__noreturn__))
#elif defined(_MSC_VER) || defined(__BORLANDC__)
    #define AF_NORETURN_ __declspec(noreturn)
#else
    #define AF_NORETURN_
#endif

#ifdef GEN_NVDM_DATABASE_FOR_PC
#undef AF_NORETURN_
#define AF_NORETURN_
#endif

#if defined(ENABLE_GGE_ROM_PATCH)
/* Assert Handler function prototypes*/
extern void romAssertHandler(unsigned flags,...);
extern AF_NORETURN_ void romAssertHandlerNoReturn(unsigned flags,...);
#endif /* ~ENABLE_GGE_ROM_PATCH */

/* Assert Handler function prototypes*/
extern void AssertHandler(unsigned flags,...);
extern AF_NORETURN_ void AssertHandlerNoReturn(unsigned flags,...);

#if defined(__cplusplus)
} /* extern "C" */
#endif

/****************************************************************************
 * Deprecated function prototypes
 ****************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif

#if defined(KI_CHECK_ASSERTIONS)
/** Base report function for assertions without parameters.
 * This function is the default entry point for assertion macros where
 * no parameters are passed.  #FatalFail() and #FatalAssert() both call this
 * function if assert checking is enabled.
 *
 * \param assertFailType  The type of the failed assertion, used to control
 *                         how the assert is reported.
 * \param cond            The string representation of the condition that
 *                         failed, or NULL if the assertion condition is not
 *                         supplied.
 * \param file            The name of the file in which the assertion was set.
 *                         This is normally taken from the MODULE_NAME define.
 * \param line            The line number in \a file at which the assertion was
 *                         set.
 */
void assertfail(DevAssertFailType        assertFailType,
                const char              *cond,
                const char              *file,
                Int16                    line);

/** Base report function for assertions with parameters.
 * This function is the default entry point for assertion macros where
 * three parameters are passed. #FatalCheck() calls this function if its
 * condition fails and assert checking is enabled.
 *
 * \param assert          The string representation of the condition that
 *                         failed, or NULL if the assertion condition is not
 *                         supplied.
 * \param v1              The first value to help describe the failed condition.
 * \param v2              The second value to help describe the failed condition.
 * \param v3              The third value to help describe the failed condition.
 * \param file            The name of the file in which the assertion was set.
 *                         This is normally taken from the MODULE_NAME define.
 * \param line            The line number in \a file at which the assertion was
 *                         set.

 */
void checkDevParam(const char              *assert,
                   Int32                    v1,
                   Int32                    v2,
                   Int32                    v3,
                   const char              *file,
                   Int16                    line);

#endif /* #if defined(KI_CHECK_ASSERTIONS) */

#if defined (KI_CHECK_WARNINGS)
/** Base report function for warnings without parameters.
 * This function is the default entry point for warning macros where
 * no parameters are passed.  #WarnFail() and #WarnAsser() both call this
 * function if warn checking is enabled.
 *
 * \param warnFailType The type of the failed warning, used to control
 *                     how the warning is reported.
 * \param warnCond The string representation of the condition that
 *                     failed, or NULL if the warning condition is not
 *                     supplied.
 * \param file     The name of the file in which the warning was set.
 *                     This is normally taken from the MODULE_NAME define.
 * \param line     The line number in \a file at which the warning was
 *                     set.
 */
void SysWarnFail(DevAssertFailType        warnFailType,
                 const char             * warnCond,
                 const char             * file,
                 Int32                    line);

/** Base report function for warnings with parameters.
 * This function is the default entry point for warning macros where
 * three parameters are passed.  #WarnCheck() calls this function if its
 * condition fails and warn checking is enabled. #WarnParam() calls this
 * function.
 *
 * \param warnCheck Set to TRUE for calls to WarnCheck and FALSE for
 *                     calls to WarnParam.
 * \param warnCond The string representation of the condition that
 *                     failed, or NULL if the warning condition is not
 *                     supplied.
 * \param v1       The first value to help describe the failed condition.
 * \param v2       The second value to help describe the failed condition.
 * \param v3       The third value to help describe the failed condition.
 * \param file     The name of the file in which the warning was set.
 *                     This is normally taken from the MODULE_NAME define.
 * \param line     The line number in \a file at which the warning was
 *                     set.

 */
void SysWarnParam(Boolean                  warnCheck,
                  const char             * warnCond,
                  Int32                    v1,
                  Int32                    v2,
                  Int32                    v3,
                  const char             * file,
                  Int32                    line);

#endif /* #if defined (KI_CHECK_WARNINGS) */

#if defined(__cplusplus)
} /* extern "C" */
#endif

/****************************************************************************
 * Macros and defines used in the implementation. Not part of the API.
 ****************************************************************************/

/* flags indicating which parameters are given to AssertHandler */
#define AF_FLAGS_FILE_     1          /* filename and line number    */
#define AF_FLAGS_FUNC_     2          /* function name               */
#define AF_FLAGS_COND_     4          /* assertion condition checked */
#define AF_FLAGS_MSG_      8          /* user message string         */
#define AF_FLAGS_VARS_     16         /* three user variables        */

/* flags indicating what macro name AssertHandler should report */
#define AF_FLAGS_FATAL_    (0&32)     /* Fatal* macro                */
#define AF_FLAGS_WARN_     (32&32)    /* Warn* macro                 */

/* Macro to pass flags, filename, line number and function name into AssertHandler.
 * Note: this evaluates to one, three or four arguments depending on conditions.
 */
#if defined(KI_ASSERT_REMOVE_FILE_AND_LINE)
    #define ASSERT_LOCUS_FLAGS    0
    #define ASSERT_LOCUS_(fLAGS)  (fLAGS)
#elif defined(__arm) || defined(__CEVAX16__) || defined(__AEON__)
    #define ASSERT_LOCUS_FLAGS    AF_FLAGS_FILE_
    #define ASSERT_LOCUS_(fLAGS)  (fLAGS)|AF_FLAGS_FILE_,MODULE_NAME,__LINE__
#elif defined(_MSC_VER)
    #define ASSERT_LOCUS_FLAGS    AF_FLAGS_FILE_ | AF_FLAGS_FUNC_
    #define ASSERT_LOCUS_(fLAGS)  (fLAGS)|AF_FLAGS_FILE_|AF_FLAGS_FUNC_,MODULE_NAME,__LINE__,__FUNCTION__"()"
#elif defined(__GNUC__) || defined(__INTEL_COMPILER)
    #define ASSERT_LOCUS_FLAGS    AF_FLAGS_FILE_|AF_FLAGS_FUNC_
    #define ASSERT_LOCUS_(fLAGS)  (fLAGS)|AF_FLAGS_FILE_|AF_FLAGS_FUNC_,MODULE_NAME,__LINE__,__PRETTY_FUNCTION__
#else
    #define ASSERT_LOCUS_FLAGS    AF_FLAGS_FILE_
    #define ASSERT_LOCUS_(fLAGS)  (fLAGS)|AF_FLAGS_FILE_,MODULE_NAME,__LINE__
#endif

/* Helper macro used to make flags test in ASSERT_ easier to read */
#if defined(SHORT_DEV_ASSERTS)
    #define AF_FLAGS_MSGVARS_(fLAGS,wHAT) ((fLAGS)&AF_FLAGS_VARS_)==(wHAT)
#else
    #define AF_FLAGS_MSGVARS_(fLAGS,wHAT) ((fLAGS)&(AF_FLAGS_MSG_|AF_FLAGS_VARS_))==(wHAT)
#endif

/** ASSERT_ macro that collects all arguments and calls the AssertHandler function.
 * Note that we deliberately cast v1,v2 and v3 to int (and not Int32)
 * so we avoid problems in C++ namespaces where Int32 may not be visible.
 */
#define ASSERT_(fUNCTION,fLAGS,cOND,mSG,v1,v2,v3)                                                \
    ( ((fLAGS)&AF_FLAGS_COND_) && (cOND) ? ((void)0)                                             \
        : (                                                                                      \
            AF_FLAGS_MSGVARS_(fLAGS,0)              ? fUNCTION(ASSERT_LOCUS_(fLAGS))           : \
            AF_FLAGS_MSGVARS_(fLAGS,AF_FLAGS_MSG_)  ? fUNCTION(ASSERT_LOCUS_(fLAGS),(mSG))     : \
            AF_FLAGS_MSGVARS_(fLAGS,AF_FLAGS_VARS_) ? fUNCTION(ASSERT_LOCUS_(fLAGS),(int)(v1),(int)(v2),(int)(v3)) : \
                fUNCTION(ASSERT_LOCUS_(fLAGS),(mSG),(int)(v1),(int)(v2),(int)(v3))               \
          )                                                                                      \
    )


/* Turn assertions on and off based on compilation mode */
#if defined(KI_CHECK_ASSERTIONS)
  /* If there is a possibility that the assert may return, map to the returning function */
  #if defined(CONTINUE_ON_ASSERTFAIL)
      #define ASSERT_FATAL_(fLAGS,cOND,mSG,v1,v2,v3)  ASSERT_(AssertHandler,(fLAGS)|AF_FLAGS_FATAL_,(cOND),(mSG),(v1),(v2),(v3))
  #else
      #define ASSERT_FATAL_(fLAGS,cOND,mSG,v1,v2,v3)  ASSERT_(AssertHandlerNoReturn,(fLAGS)|AF_FLAGS_FATAL_,(cOND),(mSG),(v1),(v2),(v3))
  #endif
#else
   #define ASSERT_FATAL_(fLAGS,cOND,mSG,v1,v2,v3)  ((void)0)
#endif
#if defined(KI_CHECK_WARNINGS)
  #define ASSERT_WARN_(fLAGS,cOND,mSG,v1,v2,v3)   ASSERT_(AssertHandler,(fLAGS)|AF_FLAGS_WARN_ ,(cOND),(mSG),(v1),(v2),(v3))
#else
   #define ASSERT_WARN_(fLAGS,cOND,mSG,v1,v2,v3)   ((void)0)
#endif


/** M_ExtractAssertVaList macro that extracts variable argument list.
 * Provided to allow consistancy of the extraction of the assert values
 * using the flag values to extract the different parts of the assert.
 *
 * Note: This macro should always appear straight after the beginning
 * variable declarations at start of function as the va_list value ap
 * is declared in the macro.
 */
#define M_ExtractAssertVaList(aRGS, fLAGS, fILE, lINE, fUNCTION, mSG, v1, v2, v3)     \
    /* Check to see if fILEname and lINEe number details are includes */                          \
    if(fLAGS & AF_FLAGS_FILE_)                                                                    \
    {                                                                                             \
        /* Extract fILE/lINEe details from argument list */                                       \
        fILE = va_arg(aRGS, const char *);                                                        \
        lINE = (Int16)va_arg(aRGS, int);                                                          \
    }                                                                                             \
    /* Check to see if funcion name details are included */                                       \
    if(fLAGS & AF_FLAGS_FUNC_)                                                                    \
    {                                                                                             \
      /* Extract fUNCTION details from argument list */                                           \
      fUNCTION = va_arg(aRGS,const char *);                                                       \
    }                                                                                             \
    /* Check to see if a mSG is included in argument list */                                      \
    if(fLAGS & AF_FLAGS_MSG_)                                                                     \
    {                                                                                             \
      /* Extract the included mSG */                                                              \
      mSG = va_arg(aRGS,const char *);                                                            \
    }                                                                                             \
    /* Check to see if assert variables are included in arguments */                              \
    if(fLAGS & AF_FLAGS_VARS_)                                                                    \
    {                                                                                             \
      /* Extract the three included assert variables */                                           \
      v1=(Int32)va_arg(aRGS, int);                                                                \
      v2=(Int32)va_arg(aRGS, int);                                                                \
      v3=(Int32)va_arg(aRGS, int);                                                                \
    }

/** M_DefineAssertHandlerFunctions macro helper
 * The M_DefineAssertHandlerFunctions macro will define the
 * two required assert handler functions. The two functions are required
 * to allow the use of either returning or non-returning handling of
 * the asserts. A warning can return where as an assert will usually not.
 * This macro should be included just after the implimentation of the
 * 'AssertHandler_' function to be used. These functions will then call
 * that implimention in the required fashion.
 *
 * Note: It should be noted that in some situations an assert can return.
 *       For example if the 'CONTINUE_ON_ASSERTFAIL' define is used or if
 *       the 'Continue on PC task asserts' option is enabled with a PC build.
 *       In these situations the AF_NORETURN_ will not declare the function
 *       as returning and the while condition given to the macro will decide
 *       if/when the function will return.
 */
#if defined(ENABLE_GGE_ROM_PATCH)
#define M_DefineRomAssertHandlerFunctions()                                         \
void romAssertHandler(unsigned flags,...)                                           \
{                                                                                \
    va_list args;                                                                \
    va_start(args,flags);                                                        \
    (*p_AssertHandler_jmptbl)(flags,args);                                                  \
    va_end(args);                                                                \
}                                                                                \
AF_NORETURN_ void romAssertHandlerNoReturn(unsigned flags,...)                      \
{                                                                                \
    va_list args;                                                                \
    va_start(args,flags);                                                        \
    (*p_AssertHandler_jmptbl)((flags&~AF_FLAGS_WARN_)|AF_FLAGS_FATAL_,args);                \
    va_end(args);                                                                \
    while(1);                                                                    \
}
#endif /* ENABLE_GGE_ROM_PATCH */

#define M_DefineAssertHandlerFunctions()                                         \
void AssertHandler(unsigned flags,...)                                           \
{                                                                                \
    va_list args;                                                                \
    va_start(args,flags);                                                        \
    AssertHandler_(flags,args);                                                  \
    va_end(args);                                                                \
}                                                                                \
AF_NORETURN_ void AssertHandlerNoReturn(unsigned flags,...)                      \
{                                                                                \
    va_list args;                                                                \
    va_start(args,flags);                                                        \
    AssertHandler_((flags&~AF_FLAGS_WARN_)|AF_FLAGS_FATAL_,args);                \
    va_end(args);                                                                \
    while(1);                                                                    \
}

/****************************************************************************
 * Assertion API
 ****************************************************************************/

/** \defgroup ApiGkiAssert GKI Warning and Assert API
 * \ingroup Api
 * \ingroup PrdGki
 * The macro interface to GKI warning and assert handling.
 * The GKI warning and assert API provides macros for checking conditions and
 * warning or asserting when things have gone wrong. The main macros that
 * should normally be used are #FatalCheck/#WarnCheck and #FatalParam/#WarnParam.
 * The #FatalAssert/#WarnAssert and #FatalFail/#WarnFail macros are less
 * useful as they do not provide as much information as the FatalCheck/WarnCheck
 * and FatalParam/WarnParam alternatives.
 *
 * There are also versions of these macros that end in 'File' e.g.
 * #FatalCheckFile. These macros should be used with care as they allow the file
 * and line number of the assert to be explicitly set. These should only be
 * used in functions that are called from many different places and where the
 * calling file and line number are available. Care should be taken if these
 * are used several times within a function as it can be difficult to tell
 * which one has asserted since they all have the same file and line number
 * (similar to using the normal assert macros several times in a macro). When used
 * several times within the same function use either #FatalCheckFile or
 * #FatalParamFile and use one of the parameters to indicate which assert it
 * is.
 *
 * Assert checking is enabled explicitly with the #KI_CHECK_ASSERTIONS
 * conditional compile or implicitly with the #DEVELOPMENT_VERSION conditional
 * compile. If assertions are not enabled all the macros resolve to nothing.
 *
 * Warning checking is enabled explicitly with the #KI_CHECK_WARNINGS
 * conditional compile or implicitly with the #DEVELOPMENT_VERSION conditional
 * compile.
 *
 *
 * @{
 */

/** Condition warning.
 * This macro checks a condition and causes a warning if the
 * condition is not met. The #WarnCheck macro should be used
 * instead where possible as it provides more information. */
#define WarnAssert(cOND)             ASSERT_WARN_(AF_FLAGS_COND_|AF_FLAGS_MSG_,(cOND),#cOND,0,0,0)

/** Condition assert, extended edition.
 * This macro checks a condition and causes a warning if the
 * condition is not met.
 * This macro allows the message and three parameters to be passed.
 */
#define WarnAssertEx(cOND,mSG,v1,v2,v3) ASSERT_WARN_(AF_FLAGS_COND_|AF_FLAGS_MSG_|AF_FLAGS_VARS_,(cOND),(mSG),(v1),(v2),(v3))

/** String warning.
 * This macro always causes a warning with string information
 * when warnings are enabled. Execution continues after the macro.
 * The #WarnParam macro should be used instead where possible as
 * it provides more information.
 */
#define WarnFail(mSG)                ASSERT_WARN_(AF_FLAGS_MSG_,0,(mSG),0,0,0)

/** Condition warning with parameters.
 * This macro checks a condition and causes a warning if the
 * condition is not met. Execution continues after the macro.
 * Three parameters are also provided to give more information
 * about the warning condition.
 */
#define WarnCheck(cOND,v1,v2,v3)     ASSERT_WARN_(AF_FLAGS_COND_|AF_FLAGS_MSG_|AF_FLAGS_VARS_,(cOND),#cOND,(v1),(v2),(v3))

/** Warning with parameters.
 * This macro always causes a warning when warnings are enabled.
 * Execution continues after the macro. Three parameters are also
 * provided to give more information about the warning condition.
 */
#define WarnParam(v1,v2,v3)          ASSERT_WARN_(AF_FLAGS_VARS_,0,"",(v1),(v2),(v3))

/** Warning with string and parameters.
 * This macro always causes a warning when warnings are enabled.
 * Execution continues after the macro. Three parameters are also
 * provided to give more information about the warning condition.
 * There is also a string parameter although this is removed when
 * SHORT_DEV_ASSERTS is defined.
 */
#define WarnStrParam(mSG,v1,v2,v3)   ASSERT_WARN_(AF_FLAGS_MSG_|AF_FLAGS_VARS_,0,(mSG),(v1),(v2),(v3))

/** Condition assert.
 * This macro checks a condition and causes an assertion if the
 * condition is not met. In that case, execution does not continue.
 * The #FatalCheck macro should be used instead where possible as
 * it provides more information.
 */
#define FatalAssert(cOND)            ASSERT_FATAL_(AF_FLAGS_COND_|AF_FLAGS_MSG_,(cOND),#cOND,0,0,0)

/** Condition assert, extended edition.
 * This macro checks a condition and causes an assertion if the
 * condition is not met. In that case, execution does not continue.
 * This macro allows the message and three parameters to be passed.
 */
#define FatalAssertEx(cOND,mSG,v1,v2,v3) ASSERT_FATAL_(AF_FLAGS_COND_|AF_FLAGS_MSG_|AF_FLAGS_VARS_,(cOND),(mSG),(v1),(v2),(v3))

/** String assert.
 * This macro always asserts with the given string information.
 * Execution does not continue after this macro. The #FatalParam
 * macro should be used instead where possible as it provides
 * more information.
 */
#define FatalFail(mSG)               ASSERT_FATAL_(AF_FLAGS_MSG_,0,(mSG),0,0,0)

/** Condition assert with parameters.
 * This macro checks a condition and causes an assertion if the
 * condition is not met. In that case, execution does not continue.
 * Three parameters are also provided to give more information
 * about the assert condition.
 */
#ifdef MTK_NBIOT_TARGET_BUILD
#define FatalCheck(cOND,v1,v2,v3)    ASSERT_FATAL_(AF_FLAGS_COND_|AF_FLAGS_VARS_,(cOND),#cOND,(v1),(v2),(v3))
#else
#define FatalCheck(cOND,v1,v2,v3)    ASSERT_FATAL_(AF_FLAGS_COND_|AF_FLAGS_MSG_|AF_FLAGS_VARS_,(cOND),#cOND,(v1),(v2),(v3))
#endif

/** Assert with parameters.
 * This macro always asserts.
 * Execution does not continue after this macro. Three parameters
 * are also provided to give more information about the assert
 * condition.
 */
#define FatalParam(v1,v2,v3)         ASSERT_FATAL_(AF_FLAGS_VARS_,0,"",(v1),(v2),(v3))

/** Assert with string and parameters.
 * This macro always asserts.
 * Execution does not continue after this macro. Three parameters
 * are also provided to give more information about the assert
 * condition. There is also a string parameter although this is
 * removed when SHORT_DEV_ASSERTS is defined.
 */
#define FatalStrParam(mSG,v1,v2,v3)  ASSERT_FATAL_(AF_FLAGS_MSG_|AF_FLAGS_VARS_,0,(mSG),(v1),(v2),(v3))

/****************************************************************************
 * Macros to check if a number is within a range
 ****************************************************************************/

/** Assert a value is within the given range.
 *  Note: (Half open range used ( [lOW, hIGH) ). So check lOW <= vALUE < hIGH */
#define WarnCheckRange(lOW,vALUE,hIGH)                             \
        WarnAssertEx(((lOW)<=(vALUE) && (vALUE)<(hIGH)),#lOW "<=" #vALUE "<" #hIGH,(lOW),(vALUE),(hIGH))

#define FatalCheckRange(lOW,vALUE,hIGH)                            \
        FatalAssertEx(((lOW)<=(vALUE) && (vALUE)<(hIGH)),#lOW "<=" #vALUE "<" #hIGH,(lOW),(vALUE),(hIGH))

/** Assert an index is within the bounds for the array */
#define WarnCheckArray(aRRAY,iNDEX)                                \
        WarnAssertEx((iNDEX)>=0 && (iNDEX)<(sizeof(aRRAY)/sizeof(aRRAY[0])),#aRRAY"["#iNDEX"]",0,(iNDEX),(sizeof(aRRAY)/sizeof(aRRAY[0])))

#define FatalCheckArray(aRRAY,iNDEX)                               \
        FatalAssertEx((iNDEX)>=0 && (iNDEX)<(sizeof(aRRAY)/sizeof(aRRAY[0])),#aRRAY"["#iNDEX"]",0,(iNDEX),(sizeof(aRRAY)/sizeof(aRRAY[0])))

/** Assert an index is within the bounds for the std::vector, std::deque or similar */
#define WarnCheckVector(vECTOR,iNDEX)                              \
        WarnAssertEx((iNDEX)>=0 && (iNDEX)<(vECTOR).size(), #vECTOR"["#iNDEX"]",0,(iNDEX),(vECTOR).size())

#define FatalCheckVector(vECTOR,iNDEX)                             \
        FatalAssertEx((iNDEX)>=0 && (iNDEX)<(vECTOR).size(), #vECTOR"["#iNDEX"]",0,(iNDEX),(vECTOR).size())

/****************************************************************************
 * Compile time assertions
 ****************************************************************************/

/** Compile time assertion
 * This can be used in places where a condition must be verified
 * at compile time. The macro evaluates to zero so that it can be
 * used in all places where a (constant) expression is expected.
 */
#define COMPILE_TIME_ASSERT(cONDITION) (sizeof(char[1-2*!(cONDITION)])-1)

/** Macro for compile time assertion of constant expressions.
 * This macro checks a condition and causes a compilation
 * error if the condition is not met. This macro can be used anywhere
 * a statement is expected.
 *
 * Examples:
 *
 * DevAssertStatic (sizeof (Int8) < sizeof (Int16));
 * - condition evaluates to TRUE
 *
 * DevAssertStatic (sizeof (Int8) > sizeof (Int16));
 * - condition evaluates to FALSE, and will generate a
 * compilation error similar to the following:
 *
 * "example.c:50: error: size of array is negative
 * DevAssertStatic (sizeof (Int8) > sizeof (Int16));
 *
 */
#define DevAssertStatic(cOND) ((void)COMPILE_TIME_ASSERT(cOND))

/****************************************************************************
 * Deprecated API
 ****************************************************************************/

/* Deprecated Dev* macros */
#define DevAssert(cOND)              ASSERT_FATAL_(AF_FLAGS_COND_|AF_FLAGS_MSG_,(cOND),#cOND,0,0,0)
#define DevFail(mSG)                 ASSERT_FATAL_(AF_FLAGS_MSG_,0,(mSG),0,0,0)
#define DevCheck(cOND,v1,v2,v3)      ASSERT_FATAL_(AF_FLAGS_COND_|AF_FLAGS_MSG_|AF_FLAGS_VARS_,(cOND),#cOND,(v1),(v2),(v3))
#define DevParam(v1,v2,v3)           ASSERT_FATAL_(AF_FLAGS_VARS_,0,"",(v1),(v2),(v3))
#define DevStrParam(mSG,v1,v2,v3)    ASSERT_FATAL_(AF_FLAGS_MSG_|AF_FLAGS_VARS_,0,(mSG),(v1),(v2),(v3))

/* Fatal...File variants of macros used in some kernel code to report
 * errors on behalf of their callers.
 */
#if defined(EXSYSTEM_DEFINE_FILE_ASSERT_MACROS)
    /* If the file/line values are availiable, use, if not then strip them off */
    #if ((ASSERT_LOCUS_FLAGS & AF_FLAGS_FILE_) == AF_FLAGS_FILE_)
        #define FatalFailFile(mSG, fILE, lINE)         AssertHandler(AF_FLAGS_MSG_|AF_FLAGS_FILE_,(fILE),(lINE),(mSG))
        #define FatalParamFile(v1, v2, v3, fILE, lINE) AssertHandler(AF_FLAGS_VARS_|AF_FLAGS_FILE_,(fILE),(lINE),(v1),(v2),(v3))
        /* Deprecated Dev* macros */
        #define DevAssertFile(cOND, fILE, lINE)        ((cOND) ? ((void)0) : (AssertHandler(AF_FLAGS_COND_|AF_FLAGS_MSG_|AF_FLAGS_FILE_,(fILE),(lINE),(#cOND))))
        #define DevParamFile(v1, v2, v3, fILE, lINE)   FatalParamFile(v1, v2, v3, fILE, lINE)
    #else
        #define FatalFailFile(mSG, fILE, lINE)         AssertHandler(AF_FLAGS_MSG_,(mSG))
        #define FatalParamFile(v1, v2, v3, fILE, lINE) AssertHandler(AF_FLAGS_VARS_,(v1),(v2),(v3))
        /* Deprecated Dev* macros */
        #define DevAssertFile(cOND, fILE, lINE)        ((cOND) ? ((void)0) : (AssertHandler(AF_FLAGS_COND_|AF_FLAGS_MSG_,(#cOND))))
        #define DevParamFile(v1, v2, v3, fILE, lINE)   FatalParamFile(v1, v2, v3, fILE, lINE)
    #endif
#endif

/* @} */ /* End of ApiGkiAssert group */


/****************************************************************************
 * Macros to stop warnings about unused variables
 ***************************************************************************/

/** Macro to remove compiler warnings for unused variables */
#if (defined(__GNUC__) && __GNUC__>2) || defined(__INTEL_COMPILER) || defined(_MSC_VER) || defined(__PGI)
    #define UNUSED(...) ((void)(__VA_ARGS__))
#else
    #define UNUSED(v)   ((void)(v))
#endif

/** Macro to remove compiler warnings for unused function parameters */
#if (defined(__GNUC__) && __GNUC__>2) || defined(__INTEL_COMPILER) || defined(_MSC_VER)
    #define PARAMETER_NOT_USED(...) ((void)(__VA_ARGS__))
#elif defined(__IAR_SYSTEMS_ICC)
    #define PARAMETER_NOT_USED(p) ((p) = (p))
#else
    #define PARAMETER_NOT_USED(p) ((void)(p))
#endif


/***************************************************************************
 * Compiler version checks
 ***************************************************************************/

#if defined(__ARMCC_VERSION)

    /* Check for SDT.
     *  Check for at least SDT2.51, since earlier version have a code generation bug
     *  that can cause erroneous asserts.  The comparison here is nested to prevent
     *  errors when comparing a fractional value.
     */
    #if __ARMCC_VERSION < 100000
        #if __ARMCC_VERSION < 4.91
            #error ARM SDT2.50 has code generation bugs.  Please upgrade to at least SDT2.51
        #endif
    #endif

    /* Check for RVCT2.0
     *  RVCT2.0 was short lived and quickly superceded by 2.1.  ARM recommend all
     *  2.0 and 2.0.1 users to upgrade 2.1.  TTPCom recommend the same and do not
     *  use or test 2.0 internally.
     */
    #if __ARMCC_VERSION >= 200000 && __ARMCC_VERSION < 210000
        #error ARM RVCT2.0 is not supported.  Please upgrade to at least RVCT2.1
    #endif

    /* Check for RVCT2.1
     *  Various patch releases have been released, and prior to patch 498 there are
     *  code generation bugs as well as Internal errors that are undesirable for
     *  producing production releases.
     */
    #if __ARMCC_VERSION >= 210000 && __ARMCC_VERSION < 210498
        #error ARM RVCT2.1 versions prior to patch 498 contain code generation bugs.
    #endif

#endif /* __ARMCC_VERSION */


/****************************************************************************
 * R2 debug kit Stub
 ****************************************************************************/
#if !defined(R2DEBUGKIT_ON)
#define M_STACK_TRACK_OPEN(_id_,_offst_)
#define M_STACK_CHECKPOINT(_id_,_offst_)
#define M_STACK_TRACK_CLOSE(_id_)

#define M_MEMORY_TRACK_OPEN(_id_,_CheckFctAddr_)
#define M_MEMORY_CHECKPOINT(_id_,_isVerbose_)
#define M_MEMORY_TRACK_CLOSE(_id_)

#define   M_G1_GPRCOPY_2_ARRAY_S
#define   M_G1_PUSH_EPCR_ON_STACK_S
#define   M_G1_POP_EPCR_ON_STACK_S

#define M_G1_GPRCOPY_2_ARRAY_C
#define M_G1_GETREGISTER(_pAddr_)  { if (((void*)0) != _pAddr_)  *_pAddr_=((void*)0); }
#define M_G1_OUTPUT_REGISTER_2_HSL(_pAddr_)
#define M_G1_OUTPUT_REGISTER_2_RAMLOG(_pAddr_)

#define M_G1_GETCALLSTACK(_pAddr_,_psize_)\
    { if (((void*)0) != _psize_)  *_psize_=0;     \
      if (((void*)0) != _pAddr_)  *_pAddr_=((void*)0);  \
    }

#define M_G1_OUTPUT_CALLSTACK_2_HSL(_pAddr_,_psize_)

typedef enum {

        CS_STOP_UNKNOW=0,    // Unknow stop reason
        CS_STOP_MAXDEPH,     // Unwind INS_SEARCH_DEPTH instructions
        CS_STOP_MAXENTRY,    // Unwind CALL_STACK_DEPTH functions calls
        CS_STOP_MAXSKIPJUMP, // Too much immediate jump occurs (>MAX_IMMJUMP_DONE)
        CS_STOP_INVALIDPC,   // Stop due to an invalid PC (0x00000000, 0xFFFFFFFF or 0xA5A55A5A) detection

} CallStack_Stop_Reason_t;
#endif

#endif /* EXSYSTEM_H */

/* END OF FILE */
