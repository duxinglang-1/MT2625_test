/*
 * Copyright (c) 1990 The Regents of the University of California.
 * All rights reserved.
 *
 * This code is derived from software contributed to Berkeley by
 * Chris Torek.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

/*
 * Copyright (c) 2012-2014 ARM Ltd
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. The name of the company may not be used to endorse or promote
 *    products derived from this software without specific prior written
 *    permission.
 *
 * THIS SOFTWARE IS PROVIDED BY ARM LTD ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL ARM LTD BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 * TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

 /* Copyright Statement:
*
* (C) 2005-2018  MediaTek Inc. All rights reserved.
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
 *************************************************************************/

/***************************************************************************
* Include Files
**************************************************************************/

#include <newlib.h>

#include <_ansi.h>
#include <reent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <stdint.h>
#include <wchar.h>
#include <sys/lock.h>
#include <stdarg.h>


/***************************************************************************
* MACROS
**************************************************************************/

/* Nonzero if either X or Y is not aligned on a "long" boundary.  */
#define UNALIGNED(X, Y) \
    (((long)X & (sizeof(long)-1)) | ((long)Y & (sizeof(long) - 1)))

/* How many bytes are copied each iteration of the 4X unrolled loop.  */
#define BIGBLOCKSIZE        (sizeof(long) << 2)

/* How many bytes are copied each iteration of the word copy loop.  */
#define LITTLEBLOCKSIZE     (sizeof(long))

/* Threshhold for punting to the byte copier.  */
#define TOO_SMALL(LEN)      ((LEN) < BIGBLOCKSIZE)


#ifndef NO_FLOATING_POINT
# define FLOATING_POINT
#endif

#define _NO_POS_ARGS
#undef _WANT_IO_C99_FORMATS

/* Currently a test is made to see if long double processing is warranted.
   This could be changed in the future should the _ldtoa_r code be
   preferred over _dtoa_r.  */
#define _NO_LONGDBL

#define _NO_LONGLONG

#define _PRINTF_FLOAT_TYPE double

#if defined (FLOATING_POINT)
# include <locale.h>
#endif
#ifdef FLOATING_POINT
# include <math.h>

/* For %La, an exponent of 15 bits occupies the exponent character,
   a sign, and up to 5 digits.  */
# define MAXEXPLEN    7
# define DEFPREC    6

extern char *_dtoa_r (struct _reent *, double, int,
            int, int *, int *, char **);

# define _DTOA_R _dtoa_r
# define FREXP frexp

#endif /* FLOATING_POINT.  */

/* BUF must be big enough for the maximum %#llo (assuming long long is
   at most 64 bits, this would be 23 characters), the maximum
   multibyte character %C, and the maximum default precision of %La
   (assuming long double is at most 128 bits with 113 bits of
   mantissa, this would be 29 characters).  %e, %f, and %g use
   reentrant storage shared with mprec.  All other formats that use
   buf get by with fewer characters.  Making BUF slightly bigger
   reduces the need for malloc in %.*a and %S, when large precision or
   long strings are processed.
   The bigger size of 100 bytes is used on systems which allow number
   strings using the locale's grouping character.  Since that's a multibyte
   value, we should use a conservative value.  */
#define BUF   40

#define quad_t long
#define u_quad_t unsigned long

typedef quad_t * quad_ptr_t;
typedef void *void_ptr_t;
typedef char *   char_ptr_t;
typedef long *   long_ptr_t;
typedef int  *   int_ptr_t;
typedef short *  short_ptr_t;

/* Macros for converting digits to letters and vice versa.  */
#define to_digit(c) ((c) - '0')
#define is_digit(c) ((unsigned)to_digit (c) <= 9)
#define to_char(n)  ((n) + '0')

/* Flags used during conversion.  */
#define ALT   0x001   /* Alternate form.  */
#define LADJUST   0x002   /* Left adjustment.  */
#define ZEROPAD   0x004   /* Zero (as opposed to blank) pad.  */
#define PLUSSGN   0x008   /* Plus sign flag.  */
#define SPACESGN  0x010   /* Space flag.  */
#define HEXPREFIX 0x020   /* Add 0x or 0X prefix.  */
#define SHORTINT  0x040   /* Short integer.  */
#define LONGINT   0x080   /* Long integer.  */
#define LONGDBL   0x100   /* Long double.  */
/* ifdef _NO_LONGLONG, make QUADINT equivalent to LONGINT, so
   that %lld behaves the same as %ld, not as %d, as expected if:
   sizeof (long long) = sizeof long > sizeof int.  */
#define QUADINT   LONGINT
#define FPT   0x400   /* Floating point number.  */
/* Define as 0, to make SARG and UARG occupy fewer instructions.  */
# define CHARINT  0

/* Macros to support positional arguments.  */
#define GET_ARG(n, ap, type) (va_arg ((ap), type))

/* To extend shorts properly, we need both signed and unsigned
   argument extraction methods.  Also they should be used in nano-vfprintf_i.c
   and nano-vfprintf_float.c only, since ap is a pointer to va_list.  */
#define SARG(flags) \
  (flags&LONGINT ? GET_ARG (N, (*ap), long) : \
      flags&SHORTINT ? (long)(short)GET_ARG (N, (*ap), int) : \
      flags&CHARINT ? (long)(signed char)GET_ARG (N, (*ap), int) : \
      (long)GET_ARG (N, (*ap), int))
#define UARG(flags) \
  (flags&LONGINT ? GET_ARG (N, (*ap), u_long) : \
      flags&SHORTINT ? (u_long)(u_short)GET_ARG (N, (*ap), int) : \
      flags&CHARINT ? (u_long)(unsigned char)GET_ARG (N, (*ap), int) : \
      (u_long)GET_ARG (N, (*ap), u_int))

/* BEWARE, these `goto error' on error. And they are used
   in more than one functions.

   Following macros are each referred about twice in printf for integer,
   so it is not worth to rewrite them into functions. This situation may
   change in the future.  */
#define PRINT(ptr, len) {   \
  if (pfunc (data, fp, (ptr), (len)) == EOF) \
    goto error;   \
}
#define PAD(howmany, ch) {             \
       int temp_i = 0;                 \
       while (temp_i < (howmany))      \
       {                               \
               if (pfunc (data, fp, &(ch), 1) == EOF) \
                       goto error;     \
               temp_i++;               \
       }             \
}
#define PRINTANDPAD(p, ep, len, ch) {  \
       int temp_n = (ep) - (p);        \
       if (temp_n > (len))             \
               temp_n = (len);         \
       if (temp_n > 0)                 \
               PRINT((p), temp_n);     \
       PAD((len) - (temp_n > 0 ? temp_n : 0), (ch)); \
}


/* All data needed to decode format string are kept in below struct.  */
struct _prt_data_t
{
  int flags;    /* Flags.  */
  int prec;   /* Precision.  */
  int dprec;    /* Decimal precision.  */
  int width;    /* Width.  */
  int size;   /* Size of converted field or string.  */
  int ret;    /* Return value accumulator.  */
  char code;    /* Current conversion specifier.  */
  char blank;   /* Blank character.  */
  char zero;    /* Zero character.  */
  char buf[BUF];  /* Output buffer for non-floating point number.  */
  char l_buf[3];  /* Sign&hex_prefix, "+/-" and "0x/X".  */
#ifdef FLOATING_POINT
  _PRINTF_FLOAT_TYPE _double_;  /* Double value.  */
  char expstr[MAXEXPLEN]; /* Buffer for exponent string.  */
  int lead;   /* The sig figs before decimal or group sep.  */
#endif
};


/***************************************************************************
 * Processes
 **************************************************************************/

void * __attribute__((naked)) __wrap_memcpy(void *v_dst, const void *v_src, size_t c)
{
    asm(
        "mov ip, r0\n\t"
        "orr r3, r1, r0\n\t"
        "ands    r3, r3, #3\n\t"
        "bne .Lmisaligned_copy\n"

      ".Lbig_block:\n\t"
        "subs   r2, #64\n\t"
        "blo .Lmid_block\n\t"
        /* Kernel loop for big block copy */
        ".align 2\n"
      ".Lbig_block_loop:\n\t"

        "ldr r3, [r1], #4\n\t"
        "str r3, [r0], #4\n\t"
        "ldr r3, [r1], #4\n\t"
        "str r3, [r0], #4\n\t"
        "ldr r3, [r1], #4\n\t"
        "str r3, [r0], #4\n\t"
        "ldr r3, [r1], #4\n\t"
        "str r3, [r0], #4\n\t"
        "ldr r3, [r1], #4\n\t"
        "str r3, [r0], #4\n\t"
        "ldr r3, [r1], #4\n\t"
        "str r3, [r0], #4\n\t"
        "ldr r3, [r1], #4\n\t"
        "str r3, [r0], #4\n\t"
        "ldr r3, [r1], #4\n\t"
        "str r3, [r0], #4\n\t"
        "ldr r3, [r1], #4\n\t"
        "str r3, [r0], #4\n\t"
        "ldr r3, [r1], #4\n\t"
        "str r3, [r0], #4\n\t"
        "ldr r3, [r1], #4\n\t"
        "str r3, [r0], #4\n\t"
        "ldr r3, [r1], #4\n\t"
        "str r3, [r0], #4\n\t"
        "ldr r3, [r1], #4\n\t"
        "str r3, [r0], #4\n\t"
        "ldr r3, [r1], #4\n\t"
        "str r3, [r0], #4\n\t"
        "ldr r3, [r1], #4\n\t"
        "str r3, [r0], #4\n\t"
        "ldr r3, [r1], #4\n\t"
        "str r3, [r0], #4\n\t"

        "subs   r2, #64\n\t"
        "bhs .Lbig_block_loop\n"

      ".Lmid_block:\n\t"
        "adds   r2, #48\n\t"
        "blo .Lcopy_word_by_word \n\t"

        /* Kernel loop for mid-block copy */
        ".align 2\n"
      ".Lmid_block_loop:\n\t"

        "ldr r3, [r1], #4\n\t"
        "str r3, [r0], #4\n\t"
        "ldr r3, [r1], #4\n\t"
        "str r3, [r0], #4\n\t"
        "ldr r3, [r1], #4\n\t"
        "str r3, [r0], #4\n\t"
        "ldr r3, [r1], #4\n\t"
        "str r3, [r0], #4\n\t"

        "subs   r2, #16\n\t"
        "bhs .Lmid_block_loop\n"

      ".Lcopy_word_by_word:\n\t"
        "adds   r2, #12\n\t"
        "blo .Lcopy_less_than_4\n\t"

        /* Kernel loop for small block copy */
        ".align 2\n"
      ".Lcopy_word_by_word_loop:\n\t"
        "ldr r3, [r1], #4\n\t"
        "str r3, [r0], #4\n\t"
        "subs   r2, #4\n\t"
        "bhs    .Lcopy_word_by_word_loop\n"
        ".Lcopy_less_than_4:\n\t"
        "adds   r2, #4\n\t"
        "beq .Ldone\n\t"
        "lsls   r2, r2, #31\n\t"
        "itt ne\n\t"
        "ldrbne r3, [r1], #1\n\t"
        "strbne r3, [r0], #1\n\t"
        "bcc    .Ldone\n\t"
        "ldrh   r3, [r1]\n\t"
        "strh   r3, [r0]\n"
        ".Ldone:\n\t"
        "mov    r0, ip\n\t"
        "bx lr\n\t"

        ".align 2\n"
      ".Lmisaligned_copy:\n\t"

        /* Copy word by word using LDR when alignment can be done in hardware,
        i.e., SCTLR.A is set, supporting unaligned access in LDR and STR.  */
        "cmp    r2, #8\n\t"
        "blo    .Lbyte_copy\n\t"

        /* if src is aligned, just go to the big block loop.  */
        "lsls   r3, r1, #30\n\t"
        "beq    .Lbig_block\n\t"

        /* Align dst only, not trying to align src.  That is the because
        handling of aligned src and misaligned dst need more overhead than
        otherwise.  By doing this the worst case is when initial src is aligned,
        additional up to 4 byte additional copy will executed, which is
        acceptable.  */
        "ands   r3, r0, #3\n\t"
        "beq    .Lbig_block\n\t"
        "rsb    r3, #4\n\t"
        "subs   r2, r3\n\t"
        "lsls    r3, r3, #31\n\t"
        "itt ne\n\t"
        "ldrbne  r3, [r1], #1\n\t"
        "strbne  r3, [r0], #1\n\t"
        "bcc .Lbig_block\n\t"

        "ldrh    r3, [r1], #2\n\t"
        "strh    r3, [r0], #2\n\t"
        "b   .Lbig_block\n"

      ".Lbyte_copy:\n\t"
        "subs    r2, #4\n\t"
        "blo .Lcopy_less_than_4\n"

      ".Lbyte_copy_loop:\n\t"
        "subs    r2, #1\n\t"
        "ldrb    r3, [r1], #1\n\t"
        "strb    r3, [r0], #1\n\t"
        "bhs .Lbyte_copy_loop\n\t"

        "ldrb    r3, [r1]\n\t"
        "strb    r3, [r0]\n\t"
        "ldrb    r3, [r1, #1]\n\t"
        "strb    r3, [r0, #1]\n\t"
        "ldrb    r3, [r1, #2]\n\t"
        "strb    r3, [r0, #2]\n\t"

        "mov r0, ip\n\t"
        "bx  lr\n\t"
    );

}

void *__wrap_memset (void *m, int c, size_t n)
{
    char *s = (char *) m;

    unsigned int i;
    unsigned long buffer;
    unsigned long *aligned_addr;
    unsigned int d = c & 0xff;   /* To avoid sign extension, copy C to an unsigned variable.  */
    while ((long)s & (LITTLEBLOCKSIZE - 1))
    {
        if (n--)
            *s++ = (char) c;
        else
            return m;
    }
    if (!(n < LITTLEBLOCKSIZE))
    {   /* If we get this far, we know that n is large and s is word-aligned. */
        aligned_addr = (unsigned long *) s;
        /* Store D into each char sized location in BUFFER so that we can set large blocks quickly.  */
        buffer = (d << 8) | d;
        buffer |= (buffer << 16);

        for (i = 32; i < LITTLEBLOCKSIZE * 8; i <<= 1)
            buffer = (buffer << i) | buffer;      /* Unroll the loop.  */

        while (n >= LITTLEBLOCKSIZE*4)
        {
            *aligned_addr++ = buffer;
            *aligned_addr++ = buffer;
            *aligned_addr++ = buffer;
            *aligned_addr++ = buffer;
            n -= 4*LITTLEBLOCKSIZE;
        }

        while (n >= LITTLEBLOCKSIZE)
        {
            *aligned_addr++ = buffer;
            n -= LITTLEBLOCKSIZE;
        }      /* Pick up the remainder with a bytewise loop.  */

        s = (char*)aligned_addr;

    }

    while (n--)
    *s++ = (char) c;

    return m;
}

void *__wrap_memmove (void *dst_void, const void *src_void, size_t length)
{
    char *dst = dst_void;
    const char *src = src_void;
    long *aligned_dst;
    const long *aligned_src;
    if (src < dst && dst < src + length)
    {
        /* Destructive overlap...have to copy backwards */
        src += length;
        dst += length;
        while (length--)
        {
            *--dst = *--src;
        }
    }
    else
    {
        /* Use optimizing algorithm for a non-destructive copy to closely match memcpy. If the size is small or either SRC or DST is unaligned,
        then punt into the byte copy loop.  This should be rare.  */
        if (!TOO_SMALL(length) && !UNALIGNED (src, dst))
        {
            aligned_dst = (long*)dst;
            aligned_src = (long*)src;
        /* Copy 4X long words at a time if possible.  */
        while (length >= BIGBLOCKSIZE)
        {
            *aligned_dst++ = *aligned_src++;
            *aligned_dst++ = *aligned_src++;
            *aligned_dst++ = *aligned_src++;
            *aligned_dst++ = *aligned_src++;
            length -= BIGBLOCKSIZE;
        }
        /* Copy one long word at a time if possible.  */
        while (length >= LITTLEBLOCKSIZE)
        {
            *aligned_dst++ = *aligned_src++;
            length -= LITTLEBLOCKSIZE;
        }
        /* Pick up any residual with a byte copier.  */
        dst = (char*)aligned_dst;
        src = (char*)aligned_src;
        }
        while (length--)
        {
            *dst++ = *src++;
        }
    }
    return dst_void;
}

int __wrap_memcmp (const void *m1, const void *m2, size_t n)
{
    unsigned char *s1 = (unsigned char *) m1;
    unsigned char *s2 = (unsigned char *) m2;
    unsigned long *a1;
    unsigned long *a2;

    /* If the size is too small, or either pointer is unaligned,
    then we punt to the byte compare loop.  Hopefully this will
    not turn up in inner loops.  */
    if (!TOO_SMALL(n) && !UNALIGNED(s1,s2))
    {
        /* Otherwise, load and compare the blocks of memory one
         word at a time.  */
        a1 = (unsigned long*) s1;
        a2 = (unsigned long*) s2;
        while (n >= LITTLEBLOCKSIZE)
        {
            if (*a1 != *a2)
                break;
            a1++;
            a2++;
            n -= LITTLEBLOCKSIZE;
        }

        /* check m mod LBLOCKSIZE remaining characters */

        s1 = (unsigned char*)a1;
        s2 = (unsigned char*)a2;
    }

    while (n--)
    {
        if (*s1 != *s2)
            return *s1 - *s2;
        s1++;
        s2++;
    }

    return 0;
}

size_t __attribute__((naked)) __wrap_strlen(const char *str)
{
    asm(
        ".syntax unified\n\t"
        ".align 6 \n\t"

        "pld    [r0, #0]\n\t"
        "strd   r4, r5, [sp, #-8]!\n\t"
        "bic    r1, r0, #7\n\t"
        "mvn    r12, #0\n\t"
        "ands   r4, r0, #7\n\t"                     /* (8 - bytes) to alignment.  */
        "pld    [r1, #32]\n\t"
        "bne.w  .Lmisaligned8\n\t"
        "mov    r4, #0\n\t"
        "mov    r0, #-8\n"
        ".Lloop_aligned:\n\t"
        /* Bytes 0-7.  */
        "ldrd   r2, r3, [r1]\n\t"
        "pld    [r1, #64]\n\t"
        "add    r0, r0, #8\n"

        ".Lstart_realigned:\n\t"
        "uadd8   r2, r2, r12\n\t"                   /* Saturating GE<0:3> set.  */
        "sel     r2, r4, r12\n\t"                   /* Select based on GE<0:3>.  */
        "uadd8   r3, r3, r12\n\t"
        "sel     r3, r2, r12\n\t"                   /* Only used if d1a == 0.  */
        "cbnz    r3, .Lnull_found\n\t"
        /* Bytes 8-15.  */
        "ldrd    r2, r3, [r1, #8]\n\t"
        "uadd8   r2, r2, r12\n\t"                   /* Saturating GE<0:3> set.  */
        "add     r0, r0, #8\n\t"
        "sel     r2, r4, r12\n\t"                   /* Select based on GE<0:3>.  */
        "uadd8   r3, r3, r12\n\t"
        "sel     r3, r2, r12\n\t"                   /* Only used if d1a == 0.  */
        "cbnz    r3, .Lnull_found\n\t"
        /* Bytes 16-23.  */
        "ldrd    r2, r3, [r1, #16]\n\t"
        "uadd8   r2, r2, r12\n\t"                   /* Saturating GE<0:3> set.  */
        "add     r0, r0, #8\n\t"
        "sel     r2, r4, r12\n\t"                   /* Select based on GE<0:3>.  */
        "uadd8   r3, r3, r12\n\t"
        "sel     r3, r2, r12\n\t"                   /* Only used if d1a == 0.  */
        "cbnz    r3, .Lnull_found\n\t"
        /* Bytes 24-31.  */
        "ldrd    r2, r3, [r1, #24]\n\t"
        "add     r1, r1, #32\n\t"
        "uadd8   r2, r2, r12\n\t"                   /* Saturating GE<0:3> set.  */
        "add     r0, r0, #8\n\t"
        "sel     r2, r4, r12\n\t"                   /* Select based on GE<0:3>.  */
        "uadd8   r3, r3, r12\n\t"
        "sel     r3, r2, r12\n\t"                   /* Only used if d1a == 0.  */
        "cmp     r3, #0\n\t"
        "beq     .Lloop_aligned\n"

        ".Lnull_found:\n\t"
        "cmp     r2, #0\n\t"
        "itt     eq\n\t"
        "addeq   r0, r0, #4\n\t"
        "moveq   r2, r3\n\t"
        "rev     r2, r2\n\t"
        "clz     r2, r2\n\t"
        "ldrd    r4, r5, [sp], #8\n\t"
        "add     r0, r0, r2, lsr #3\n\t"           /* Bits -> Bytes.  */
        "bx      lr\n"

        ".Lmisaligned8:\n\t"
        "ldrd    r2, r3, [r1]\n\t"
        "and     r5, r4, #3\n\t"
        "rsb     r0, r4, #0\n\t"
        "lsl     r5, r5, #3\n\t"                    /* Bytes -> bits.  */
        "tst     r4, #4\n\t"
        "pld     [r1, #64]\n\t"
        "lsl     r5, r12, r5\n\t"
        "orn     r2, r2, r5\n\t"
        "itt     ne\n\t"
        "ornne   r3, r3, r5\n\t"
        "movne   r2, r12\n\t"
        "mov     r4, #0\n\t"
        "b       .Lstart_realigned\n\t");
}

int __attribute__((naked)) __wrap_strcmp (const char *s1, const char *s2)
{
    asm(
        "orr   ip, r0, r1\n\t"
        "tst   ip, #7\n\t"
        "bne   2f \n\t"
        "subs  sp, sp, #16\n\t"
        "strd  r4, r5, [sp, #8]\n\t"
        "strd  r6, r7, [sp]\n\t"
        "mvn   r6, #0\n\t"
        "mov   r7, #0\n\t"

        ".align 2\n\t"
        "1:\n\t"
        "ldrd  r2, r3, [r0], #8\n\t"
        "ldrd  r4, r5, [r1], #8\n\t"
        "cmp   r2, r4\n\t"
        "uadd8 ip, r2, r6\n\t"
        "sel   ip, r7, r6\n\t"
        "it    eq\n\t"
        "cmpeq ip, #0\n\t"
        "bne   19f \n\t"
        "cmp   r3, r5\n\t"
        "uadd8 ip, r3, r6\n\t"
        "sel   ip, r7, r6\n\t"
        "it    eq\n\t"
        "cmpeq ip, #0\n\t"
        "bne   18f\n\t"
        "b     1b \n"
        "2:\n\t"
        "ands  ip, r0, #3\n\t"
        "beq   6f\n\t"
        "bic   r0, r0, #3\n\t"
        "ldr   r2, [r0], #4\n\t"
        "movs  ip, ip, lsl #31\n\t"
        "beq   3f \n\t"
        "bcs   4f \n\t"
        "ldrb  ip, [r1], #1\n\t"
        "uxtb  r3, r2, ror #8\n\t"
        "subs  ip, r3, ip\n\t"
        "bne   5f \n\t"
        "cbz   r3, 5f \n"
        "3:\n\t"
        "ldrb  ip, [r1], #1\n\t"
        "uxtb  r3, r2, ror #16\n\t"
        "subs  ip, r3, ip\n\t"
        "bne   5f\n\t"
        "cbz   r3, 5f \n"
        "4:\n\t"
        "ldrb  ip, [r1], #1\n\t"
        "uxtb  r3, r2, ror #24\n\t"
        "subs  ip, r3, ip\n\t"
        "bne   5f \n\t"
        "cbnz  r3, 6f \n"
        "5:\n\t"
        "mov   r0, ip\n\t"
        "bx    lr\n"
        "6:\n\t"
        "subs  sp, sp, #16\n\t"
        "strd  r4, r5, [sp, #8]\n\t"
        "strd  r6, r7, [sp]\n\t"
        "mvn   r6, #0\n\t"
        "mov   r7, #0\n\t"
        "ands  ip, r1, #3\n\t"
        "bne   9f \n\t"
        "tst   r0, #4\n\t"
        "beq   7f\n\t"
        "ldr   r2, [r0], #4\n\t"
        "ldr   r4, [r1], #4\n\t"
        "cmp   r2, r4\n\t"
        "uadd8 ip, r2, r6\n\t"
        "sel   ip, r7, r6\n\t"
        "it    eq\n\t"
        "cmpeq ip, #0\n\t"
        "bne   19f\n"
        "7:\n\t"
        "tst   r1, #4\n\t"
        "beq   1b \n\t"
        "ldr   r5, [r1], #4\n\t"

        ".align 2\n"
        "8:\n\t"
        "ldrd  r2, r3, [r0], #8\n\t"
        "cmp   r2, r5\n\t"
        "uadd8 ip, r2, r6\n\t"
        "sel   ip, r7, r6\n\t"
        "it    eq\n\t"
        "cmpeq ip, #0\n\t"
        "bne   17f \n\t"
        "ldrd  r4, r5, [r1], #8\n\t"
        "cmp   r3, r4\n\t"
        "uadd8 ip, r3, r6\n\t"
        "sel   ip, r7, r6\n\t"
        "it    eq\n\t"
        "cmpeq ip, #0\n\t"
        "bne   16f \n\t"
        "b     8b \n"

        "9:\n\t"
        "bic   r1, r1, #3\n\t"
        "cmp   ip, #2\n\t"
        "beq   11f \n\t"
        "bge   13f \n\t"
        "ldr   r5, [r1], #4\n\t"

        ".align 2\n"
        "10:\n\t"
        "ldr   r3, [r0], #4\n\t"
        "mov   r5, r5, lsr #8\n\t"
        "uadd8 ip, r3, r6\n\t"
        "sel   ip, r7, r6\n\t"
        "cmp   r7, ip, lsl #8\n\t"
        "and   r2, r3, r6, lsr #8\n\t"
        "it    eq\n\t"
        "cmpeq r2, r5\n\t"
        "bne   17f \n\t"
        "ldr   r5, [r1], #4\n\t"
        "cmp   ip, #0\n\t"
        "eor   r3, r2, r3\n\t"
        "mov   r2, r5, lsl #24\n\t"
        "it    eq\n\t"
        "cmpeq r3, r2\n\t"
        "bne   15f \n\t"
        "b     10b \n"
        "11:\n\t"
        "ldr   r5, [r1], #4\n\t"

        ".align 2\n"
        "12:\n\t"
        "ldr   r3, [r0], #4\n\t"
        "mov   r5, r5, lsr #16\n\t"
        "uadd8 ip, r3, r6\n\t"
        "sel   ip, r7, r6\n\t"
        "cmp   r7, ip, lsl #16\n\t"
        "and   r2, r3, r6, lsr #16\n\t"
        "it    eq\n\t"
        "cmpeq r2, r5\n\t"
        "bne   17f \n\t"
        "ldr   r5, [r1], #4\n\t"
        "cmp   ip, #0\n\t"
        "eor   r3, r2, r3\n\t"
        "mov   r2, r5, lsl #16\n\t"
        "it    eq\n\t"
        "cmpeq r3, r2\n\t"
        "bne   15f \n\t"
        "b.n   12b \n"
        "13:\n\t"
        "ldr.w r5, [r1], #4\n\t"

        ".align 2\n"
        "14:\n\t"
        "ldr   r3, [r0], #4\n\t"
        "mov   r5, r5, lsr #24\n\t"
        "uadd8 ip, r3, r6\n\t"
        "sel   ip, r7, r6\n\t"
        "cmp   r7, ip, lsl #24\n\t"
        "and   r2, r3, r6, lsr #24\n\t"
        "it    eq\n\t"
        "cmpeq r2, r5\n\t"
        "bne   17f \n\t"
        "ldr   r5, [r1], #4\n\t"
        "cmp   ip, #0\n\t"
        "eor   r3, r2, r3\n\t"
        "mov   r2, r5, lsl #8\n\t"
        "it    eq\n\t"
        "cmpeq r3, r2\n\t"
        "bne   15f \n\t"
        "b     14b \n"

        "15:\n\t"
        "rev   r1, r3\n\t"
        "rev   r2, r2\n\t"
        "b     20f \n"
        "16:\n\t"
        "rev   r1, r3\n\t"
        "rev   r2, r4\n\t"
        "b     20f \n"
        "17:\n\t"
        "rev   r1, r2\n\t"
        "rev   r2, r5\n\t"
        "b     20f \n"
        "18:\n\t"
        "rev   r1, r3\n\t"
        "rev   r2, r5\n\t"
        "b     20f \n"
        "19:\n\t"
        "rev   r1, r2\n\t"
        "rev   r2, r4\n"
        "20:\n\t"
        "rev   r0, ip\n\t"
        "ldrd  r6, r7, [sp]\n\t"
        "ldrd  r4, r5, [sp, #8]\n\t"
        "adds  sp, sp, #16\n\t"
        "cbz   r0, 21f \n\t"
        "clz   r0, r0 \n\t"
        "rsb   r0, r0, #24\n\t"
        "lsr   r1, r1, r0\n\t"
        "lsr   r2, r2, r0\n"
        "21:\n\t"
        "movs  r0, #1\n\t"
        "cmp   r1, r2\n\t"
        "it    ls\n\t"
        "sbcls r0, r0\n\t"
        "bx    lr\n\t"
    );

}


char* __attribute__((naked)) __wrap_strcpy (char* dst, const char* src)
{
    asm (
        ".syntax unified\n\t"

        "eor  r2, r0, r1\n\t"
        "mov  ip, r0\n\t"
        "tst  r2, #3\n\t"
        "bne  4f\n\t"
        "tst  r1, #3\n\t"
        "bne  3f\n"
      "5:\n\t"
        "str   r4, [sp, #-4]!\n\t"
        "tst  r1, #4\n\t"
        "ldr  r3, [r1], #4\n\t"
        "beq    2f\n\t"
        "sub  r2, r3, #0x01010101\n\t"
        "bics   r2, r2, r3\n\t"
        "tst  r2, #0x80808080\n\t"
        "itt    eq\n\t"
        "streq    r3, [ip], #4\n\t"
        "ldreq  r3, [r1], #4\n"
        "bne  1f\n\t"
        /* Inner loop.  We now know that r1 is 64-bit aligned, so we
        can safely fetch up to two words.  This allows us to avoid
        load stalls.  */
        ".p2align 2\n"
      "2:\n\t"
        "ldr  r4, [r1], #4\n\t"
        "sub    r2, r3, #0x01010101\n\t"
        "bics   r2, r2, r3\n\t"
        "tst  r2, #0x80808080\n\t"
        "sub    r2, r4, #0x01010101\n\t"
        "bne    1f\n\t"
        "str  r3, [ip], #4\n\t"
        "bics   r2, r2, r4\n\t"
        "tst  r2, #0x80808080\n\t"
        "itt    eq\n\t"
        "ldreq    r3, [r1], #4\n\t"
        "streq  r4, [ip], #4\n\t"
        "beq    2b\n\t"
        "mov  r3, r4\n"
      "1:\n\t"
        "strb  r3, [ip], #1\n\t"
        "tst    r3, #0xff\n\t"
        "ror   r3, r3, #8\n\t"
        "bne    1b\n\t"
        "ldr  r4, [sp], #4\n\t"
        "bx   lr\n"
        /* Strings have the same offset from word alignment, but it's
        not zero.  */
      "3:\n\t"
        "tst    r1, #1\n\t"
        "beq  1f\n\t"
        "ldrb r2, [r1], #1\n\t"
        "strb   r2, [ip], #1\n\t"
        "cmp    r2, #0\n\t"
        "it   eq\n"
        "bxeq   lr\n"
      "1:\n\t"
        "tst  r1, #2\n\t"
        "beq  5b\n\t"
        "ldrh r2, [r1], #2\n\t"

        "tst  r2, #0xff\n\t"
        "itet  ne\n\t"
        "strhne   r2, [ip], #2\n\t"
        "strbeq r2, [ip]\n\t"
        "tstne  r2, #0xff00\n\t"

        "bne   5b\n\t"
        "bx   lr\n"
        /* src and dst do not have a common word-alignement.  Fall back to
        byte copying.  */
      "4:\n\t"
        "ldrb   r2, [r1], #1\n\t"
        "strb   r2, [ip], #1\n\t"
        "cmp    r2, #0\n\t"
        "bne  4b\n\t"
        "bx   lr\n\t");
}

/* Decode and print non-floating point data.  */
int
__wrap__printf_common (struct _reent *data,
    struct _prt_data_t *pdata,
    int *realsz,
    FILE *fp,
    int (*pfunc)(struct _reent *, FILE *,
           const char *, size_t len))
{
  int n;
  /*
   * All reasonable formats wind up here.  At this point, `cp'
   * points to a string which (if not flags&LADJUST) should be
   * padded out to `width' places.  If flags&ZEROPAD, it should
   * first be prefixed by any sign or other prefix; otherwise,
   * it should be blank padded before the prefix is emitted.
   * After any left-hand padding and prefixing, emit zeroes
   * required by a decimal [diouxX] precision, then print the
   * string proper, then emit zeroes required by any leftover
   * floating precision; finally, if LADJUST, pad with blanks.
   * If flags&FPT, ch must be in [aAeEfg].
   *
   * Compute actual size, so we know how much to pad.
   * size excludes decimal prec; realsz includes it.
   */
  *realsz = pdata->dprec > pdata->size ? pdata->dprec : pdata->size;
  if (pdata->l_buf[0])
    (*realsz)++;

  if (pdata->flags & HEXPREFIX)
    *realsz += 2;

  /* Right-adjusting blank padding.  */
  if ((pdata->flags & (LADJUST|ZEROPAD)) == 0)
    PAD (pdata->width - *realsz, pdata->blank);

  /* Prefix.  */
  n = 0;
  if (pdata->l_buf[0])
    n++;

  if (pdata->flags & HEXPREFIX)
    {
      pdata->l_buf[n++] = '0';
      pdata->l_buf[n++] = pdata->l_buf[2];
    }

  PRINT (pdata->l_buf, n);
  n = pdata->width - *realsz;
  if ((pdata->flags & (LADJUST|ZEROPAD)) != ZEROPAD || n < 0)
    n = 0;

  if (pdata->dprec > pdata->size)
    n += pdata->dprec - pdata->size;

  PAD (n, pdata->zero);
  return 0;
error:
  return -1;
}

int
__wrap__printf_i (struct _reent *data, struct _prt_data_t *pdata, FILE *fp,
     int (*pfunc)(struct _reent *, FILE *, const char *, size_t len),
     va_list *ap)
{
  /* Field size expanded by dprec.  */
  int realsz;
  u_quad_t _uquad;
  int base;
  int n;
  char *cp = pdata->buf + BUF;
  char *xdigs = "0123456789ABCDEF";

  /* Decoding the conversion specifier.  */
  switch (pdata->code)
    {
    case 'c':
      *--cp = GET_ARG (N, *ap, int);
      pdata->size = 1;
      goto non_number_nosign;
    case 'd':
    case 'i':
      _uquad = SARG (pdata->flags);
      if ((long) _uquad < 0)
  {
    _uquad = -_uquad;
    pdata->l_buf[0] = '-';
  }
      base = 10;
      goto number;
    case 'u':
    case 'o':
      _uquad = UARG (pdata->flags);
      base = (pdata->code == 'o') ? 8 : 10;
      goto nosign;
    case 'X':
      pdata->l_buf[2] = 'X';
      goto hex;
    case 'p':
      /*
       * ``The argument shall be a pointer to void.  The
       * value of the pointer is converted to a sequence
       * of printable characters, in an implementation-
       * defined manner.''
       *  -- ANSI X3J11
       */
      pdata->flags |= HEXPREFIX;
      if (sizeof (void*) > sizeof (int))
  pdata->flags |= LONGINT;
      /* NOSTRICT.  */
    case 'x':
      pdata->l_buf[2] = 'x';
      xdigs = "0123456789abcdef";
hex:
      _uquad = UARG (pdata->flags);
      base = 16;
      if (pdata->flags & ALT)
  pdata->flags |= HEXPREFIX;

      /* Leading 0x/X only if non-zero.  */
      if (_uquad == 0)
  pdata->flags &= ~HEXPREFIX;

      /* Unsigned conversions.  */
nosign:
      pdata->l_buf[0] = '\0';
      /*
       * ``... diouXx conversions ... if a precision is
       * specified, the 0 flag will be ignored.''
       *  -- ANSI X3J11
       */
number:
      if ((pdata->dprec = pdata->prec) >= 0)
  pdata->flags &= ~ZEROPAD;

      /*
       * ``The result of converting a zero value with an
       * explicit precision of zero is no characters.''
       *  -- ANSI X3J11
       */
      if (_uquad != 0 || pdata->prec != 0)
  {
    do
      {
        *--cp = xdigs[_uquad % base];
        _uquad /= base;
      }
    while (_uquad);
  }
      /* For 'o' conversion, '#' increases the precision to force the first
   digit of the result to be zero.  */
      if (base == 8 && (pdata->flags & ALT) && pdata->prec <= pdata->size)
  *--cp = '0';

      pdata->size = pdata->buf + BUF - cp;
      break;
    case 'n':
      if (pdata->flags & LONGINT)
  *GET_ARG (N, *ap, long_ptr_t) = pdata->ret;
      else if (pdata->flags & SHORTINT)
  *GET_ARG (N, *ap, short_ptr_t) = pdata->ret;
      else
  *GET_ARG (N, *ap, int_ptr_t) = pdata->ret;
    case '\0':
      pdata->size = 0;
      break;
    case 's':
      cp = GET_ARG (N, *ap, char_ptr_t);
      if(cp == NULL)
  cp = "(null)";

      /* Precision gives the maximum number of chars to be written from a
   string, and take prec == -1 into consideration.
   Use normal Newlib approach here to support case where cp is not
   nul-terminated.  */
      char *p = memchr (cp, 0, pdata->prec);

      if (p != NULL)
  pdata->prec = p - cp;

      pdata->size = pdata->prec;
      goto non_number_nosign;
    default:
      /* "%?" prints ?, unless ? is NUL.  */
      /* Pretend it was %c with argument ch.  */
      *--cp = pdata->code;
      pdata->size = 1;
non_number_nosign:
      pdata->l_buf[0] = '\0';
      break;
    }

    /* Output.  */
    n = __wrap__printf_common (data, pdata, &realsz, fp, pfunc);
    if (n == -1)
      goto error;

    PRINT (cp, pdata->size);
    /* Left-adjusting padding (always blank).  */
    if (pdata->flags & LADJUST)
      PAD (pdata->width - realsz, pdata->blank);

    return (pdata->width > realsz ? pdata->width : realsz);
error:
    return -1;
}


/* END OF FILE */
