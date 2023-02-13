/* stdio.h: ANSI 'C' (X3J11 Oct 88) library header, section 4.9 */
/* Copyright (C) Codemist Ltd., 1988-1993                       */
/* Copyright 1991-1998 ARM Limited. All rights reserved.        */

/*
 * RCS $Revision$
 * Checkin $Date$
 * Revising $Author: sdouglas $
 */

/*
 * stdio.h declares two types, several macros, and many functions for
 * performing input and output. For a discussion on Streams and Files
 * refer to sections 4.9.2 and 4.9.3 in the above ANSI draft, or to a
 * modern textbook on C.
 */

#ifndef __stdio_h
#define __stdio_h
#define __ARMCLIB_VERSION 5060037

/*
 * Depending on compiler version __int64 or __INT64_TYPE__ should be defined.
 */
#ifndef __int64
#ifdef __INT64_TYPE__
#define __int64 __INT64_TYPE__
#endif
/* On some architectures neither of these may be defined - if so, fall
   through and error out if used. */
#endif

#define _ARMABI __declspec(__nothrow)

#ifndef __STDIO_DECLS
#define __STDIO_DECLS

#undef __CLIBNS
#ifdef __cplusplus
namespace std
{
#define __CLIBNS ::std::
    extern "C"
    {
#else /* ndef __cplusplus */
#define __CLIBNS
#endif /* ndef __cplusplus */

#if defined(__cplusplus) || !defined(__STRICT_ANSI__) || !defined(__size_t)
        /* always defined in C++ and non-strict C for consistency of debug info */
#if __sizeof_ptr == 8
        typedef unsigned long size_t; /* see <stddef.h> */
#else
        typedef unsigned int size_t; /* see <stddef.h> */
#endif
#if !defined(__cplusplus) && defined(__STRICT_ANSI__)
#define __size_t 1
#endif
#endif

#undef NULL
#define NULL 0 /* see <stddef.h> */

/* ANSI forbids va_list to be defined here */
/* keep in step with <stdarg.h> and <wchar.h> */
#if !defined(__va_list) && (defined(__cplusplus) || !defined(__STRICT_ANSI__) || !defined(__va_list_defined))
        /* always defined in C++ and non-strict C for consistency of debug info */
#ifdef __clang__
        typedef __builtin_va_list __va_list;
#else
        typedef struct __va_list __va_list;
#endif
#if !defined(__cplusplus) && defined(__STRICT_ANSI__)
#define __va_list_defined 1
#endif
#endif

        /*
         * If the compiler supports signalling nans as per N965 then it
         * will define __SUPPORT_SNAN__, in which case a user may define
         * _WANT_SNAN in order to obtain compliant versions of the printf
         * and scanf families of functions
         */
#if defined(__SUPPORT_SNAN__) && defined(_WANT_SNAN)
#pragma import(__use_snan)
#endif

        typedef struct __fpos_t_struct
        {
            unsigned __int64 __pos;
            /*
             * this structure is equivalent to an mbstate_t, but we're not
             * allowed to actually define the type name `mbstate_t' within
             * stdio.h
             */
            struct
            {
                unsigned int __state1, __state2;
            } __mbstate;
        } fpos_t;
        /*
         * fpos_t is an object capable of recording all information needed to
         * specify uniquely every position within a file.
         */

#define _SYS_OPEN 16
        /* _SYS_OPEN defines a limit on the number of open files that is imposed
         * by this C library
         */

        typedef struct __FILE FILE;
        /*
         * FILE is an object capable of recording all information needed to control
         * a stream, such as its file position indicator, a pointer to its
         * associated buffer, an error indicator that records whether a read/write
         * error has occurred and an end-of-file indicator that records whether the
         * end-of-file has been reached.
         * Its structure is not made known to library clients.
         */

#if defined(__STRICT_ANSI__) && !__FILE_INCOMPLETE
        struct __FILE
        {
            union
            {
                long __FILE_alignment;
#ifdef __TARGET_ARCH_AARCH64
                char __FILE_size[136];
#else  /* __TARGET_ARCH_AARCH64 */
                char __FILE_size[84];
#endif /* __TARGET_ARCH_AARCH64 */
            } __FILE_opaque;
        };
        /*
         * FILE must be an object type (C99 - 7.19.1) and an object type fully
         * describes an object [including its static size] (C99 - 6.2.5).
         * This definition is a placeholder which matches the struct __FILE in
         * size and alignment as used internally by libc.
         */
#endif

        extern FILE __stdin, __stdout, __stderr;
        extern FILE *__aeabi_stdin, *__aeabi_stdout, *__aeabi_stderr;

#if _AEABI_PORTABILITY_LEVEL != 0 || (!defined _AEABI_PORTABILITY_LEVEL && __DEFAULT_AEABI_PORTABILITY_LEVEL != 0)
#define stdin (__CLIBNS __aeabi_stdin)
        /* pointer to a FILE object associated with standard input stream */
#define stdout (__CLIBNS __aeabi_stdout)
        /* pointer to a FILE object associated with standard output stream */
#define stderr (__CLIBNS __aeabi_stderr)
        /* pointer to a FILE object associated with standard error stream */
        extern const int __aeabi_IOFBF;
#define _IOFBF (__CLIBNS __aeabi_IOFBF)
        extern const int __aeabi_IONBF;
#define _IONBF (__CLIBNS __aeabi_IONBF)
        extern const int __aeabi_IOLBF;
#define _IOLBF (__CLIBNS __aeabi_IOLBF)
        extern const int __aeabi_BUFSIZ;
#define BUFSIZ (__CLIBNS __aeabi_BUFSIZ)
        extern const int __aeabi_FOPEN_MAX;
#define FOPEN_MAX (__CLIBNS __aeabi_FOPEN_MAX)
        extern const int __aeabi_TMP_MAX;
#define TMP_MAX (__CLIBNS __aeabi_TMP_MAX)
        extern const int __aeabi_FILENAME_MAX;
#define FILENAME_MAX (__CLIBNS __aeabi_FILENAME_MAX)
        extern const int __aeabi_L_tmpnam;
#define L_tmpnam (__CLIBNS __aeabi_L_tmpnam)
#else
#define stdin (&__CLIBNS __stdin)
/* pointer to a FILE object associated with standard input stream */
#define stdout (&__CLIBNS __stdout)
/* pointer to a FILE object associated with standard output stream */
#define stderr (&__CLIBNS __stderr)
/* pointer to a FILE object associated with standard error stream */

#define _IOFBF 0x100 /* fully buffered IO */
#define _IOLBF 0x200 /* line buffered IO */
#define _IONBF 0x400 /* unbuffered IO */

/* Various default file IO buffer sizes */
#define BUFSIZ (512) /* system buffer size (as used by setbuf) */

#define FOPEN_MAX _SYS_OPEN
/*
 * an integral constant expression that is the minimum number of files that
 * this implementation guarantees can be open simultaneously.
 */

#define FILENAME_MAX 256
/*
 * an integral constant expression that is the size of an array of char
 * large enough to hold the longest filename string
 */
#define L_tmpnam FILENAME_MAX
/*
 * an integral constant expression that is the size of an array of char
 * large enough to hold a temporary file name string generated by the
 * tmpnam function.
 */
#define TMP_MAX 256
/*
 * an integral constant expression that is the minimum number of unique
 * file names that shall be generated by the tmpnam function.
 */

#endif

#define EOF (-1)
        /*
         * negative integral constant, indicates end-of-file, that is, no more input
         * from a stream.
         */

#define SEEK_SET 0 /* start of stream (see fseek) */
#define SEEK_CUR 1 /* current position in stream (see fseek) */
#define SEEK_END 2 /* end of stream (see fseek) */

        /*
         * _IOBIN is the flag passed to _sys_write to denote a binary
         * file.
         */
#define _IOBIN 0x04 /* binary stream */

#define __STDIN_BUFSIZ (64)  /* default stdin buffer size */
#define __STDOUT_BUFSIZ (64) /* default stdout buffer size */
#define __STDERR_BUFSIZ (16) /* default stderr buffer size */

        extern _ARMABI int remove(const char * /*filename*/) __attribute__((__nonnull__(1)));
        /*
         * causes the file whose name is the string pointed to by filename to be
         * removed. Subsequent attempts to open the file will fail, unless it is
         * created anew. If the file is open, the behaviour of the remove function
         * is implementation-defined.
         * Returns: zero if the operation succeeds, nonzero if it fails.
         */
        extern _ARMABI int rename(const char * /*old*/, const char * /*new*/) __attribute__((__nonnull__(1, 2)));
        /*
         * causes the file whose name is the string pointed to by old to be
         * henceforth known by the name given by the string pointed to by new. The
         * file named old is effectively removed. If a file named by the string
         * pointed to by new exists prior to the call of the rename function, the
         * behaviour is implementation-defined.
         * Returns: zero if the operation succeeds, nonzero if it fails, in which
         *          case if the file existed previously it is still known by its
         *          original name.
         */
        extern _ARMABI FILE *tmpfile(void);
        /*
         * creates a temporary binary file that will be automatically removed when
         * it is closed or at program termination. The file is opened for update.
         * Returns: a pointer to the stream of the file that it created. If the file
         *          cannot be created, a null pointer is returned.
         */
        extern _ARMABI char *tmpnam(char * /*s*/);
        /*
         * generates a string that is not the same as the name of an existing file.
         * The tmpnam function generates a different string each time it is called,
         * up to TMP_MAX times. If it is called more than TMP_MAX times, the
         * behaviour is implementation-defined.
         * Returns: If the argument is a null pointer, the tmpnam function leaves
         *          its result in an internal static object and returns a pointer to
         *          that object. Subsequent calls to the tmpnam function may modify
         *          the same object. if the argument is not a null pointer, it is
         *          assumed to point to an array of at least L_tmpnam characters;
         *          the tmpnam function writes its result in that array and returns
         *          the argument as its value.
         */

        extern _ARMABI int fclose(FILE * /*stream*/) __attribute__((__nonnull__(1)));
        /*
         * causes the stream pointed to by stream to be flushed and the associated
         * file to be closed. Any unwritten buffered data for the stream are
         * delivered to the host environment to be written to the file; any unread
         * buffered data are discarded. The stream is disassociated from the file.
         * If the associated buffer was automatically allocated, it is deallocated.
         * Returns: zero if the stream was succesfully closed, or nonzero if any
         *          errors were detected or if the stream was already closed.
         */
        extern _ARMABI int fflush(FILE * /*stream*/);
        /*
         * If the stream points to an output or update stream in which the most
         * recent operation was output, the fflush function causes any unwritten
         * data for that stream to be delivered to the host environment to be
         * written to the file. If the stream points to an input or update stream,
         * the fflush function undoes the effect of any preceding ungetc operation
         * on the stream.
         * Returns: nonzero if a write error occurs.
         */
        extern _ARMABI FILE *fopen(const char *__restrict /*filename*/,
                                   const char *__restrict /*mode*/) __attribute__((__nonnull__(1, 2)));
        /*
         * opens the file whose name is the string pointed to by filename, and
         * associates a stream with it.
         * The argument mode points to a string beginning with one of the following
         * sequences:
         * "r"         open text file for reading
         * "w"         create text file for writing, or truncate to zero length
         * "a"         append; open text file or create for writing at eof
         * "rb"        open binary file for reading
         * "wb"        create binary file for writing, or truncate to zero length
         * "ab"        append; open binary file or create for writing at eof
         * "r+"        open text file for update (reading and writing)
         * "w+"        create text file for update, or truncate to zero length
         * "a+"        append; open text file or create for update, writing at eof
         * "r+b"/"rb+" open binary file for update (reading and writing)
         * "w+b"/"wb+" create binary file for update, or truncate to zero length
         * "a+b"/"ab+" append; open binary file or create for update, writing at eof
         *
         * Opening a file with read mode ('r' as the first character in the mode
         * argument) fails if the file does not exist or cannot be read.
         * Opening a file with append mode ('a' as the first character in the mode
         * argument) causes all subsequent writes to be forced to the current end of
         * file, regardless of intervening calls to the fseek function. In some
         * implementations, opening a binary file with append mode ('b' as the
         * second or third character in the mode argument) may initially position
         * the file position indicator beyond the last data written, because of the
         * NUL padding.
         * When a file is opened with update mode ('+' as the second or third
         * character in the mode argument), both input and output may be performed
         * on the associated stream. However, output may not be directly followed
         * by input without an intervening call to the fflush fuction or to a file
         * positioning function (fseek, fsetpos, or rewind), and input be not be
         * directly followed by output without an intervening call to the fflush
         * fuction or to a file positioning function, unless the input operation
         * encounters end-of-file. Opening a file with update mode may open or
         * create a binary stream in some implementations. When opened, a stream
         * is fully buffered if and only if it does not refer to an interactive
         * device. The error and end-of-file indicators for the stream are
         * cleared.
         * Returns: a pointer to the object controlling the stream. If the open
         *          operation fails, fopen returns a null pointer.
         */
        extern _ARMABI FILE *freopen(const char *__restrict /*filename*/,
                                     const char *__restrict /*mode*/,
                                     FILE *__restrict /*stream*/) __attribute__((__nonnull__(2, 3)));
        /*
         * opens the file whose name is the string pointed to by filename and
         * associates the stream pointed to by stream with it. The mode argument is
         * used just as in the fopen function.
         * The freopen function first attempts to close any file that is associated
         * with the specified stream. Failure to close the file successfully is
         * ignored. The error and end-of-file indicators for the stream are cleared.
         * Returns: a null pointer if the operation fails. Otherwise, freopen
         *          returns the value of the stream.
         */
        extern _ARMABI void setbuf(FILE *__restrict /*stream*/,
                                   char *__restrict /*buf*/) __attribute__((__nonnull__(1)));
        /*
         * Except that it returns no value, the setbuf function is equivalent to the
         * setvbuf function invoked with the values _IOFBF for mode and BUFSIZ for
         * size, or (if buf is a null pointer), with the value _IONBF for mode.
         * Returns: no value.
         */
        extern _ARMABI int setvbuf(FILE *__restrict /*stream*/,
                                   char *__restrict /*buf*/,
                                   int /*mode*/, size_t /*size*/) __attribute__((__nonnull__(1)));
        /*
         * may be used after the stream pointed to by stream has been associated
         * with an open file but before it is read or written. The argument mode
         * determines how stream will be buffered, as follows: _IOFBF causes
         * input/output to be fully buffered; _IOLBF causes output to be line
         * buffered (the buffer will be flushed when a new-line character is
         * written, when the buffer is full, or when input is requested); _IONBF
         * causes input/output to be completely unbuffered. If buf is not the null
         * pointer, the array it points to may be used instead of an automatically
         * allocated buffer (the buffer must have a lifetime at least as great as
         * the open stream, so the stream should be closed before a buffer that has
         * automatic storage duration is deallocated upon block exit). The argument
         * size specifies the size of the array. The contents of the array at any
         * time are indeterminate.
         * Returns: zero on success, or nonzero if an invalid value is given for
         *          mode or size, or if the request cannot be honoured.
         */
#pragma __printf_args
        extern _ARMABI int fprintf(FILE *__restrict /*stream*/,
                                   const char *__restrict /*format*/, ...) __attribute__((__nonnull__(1, 2)));
        /*
         * writes output to the stream pointed to by stream, under control of the
         * string pointed to by format that specifies how subsequent arguments are
         * converted for output. If there are insufficient arguments for the format,
         * the behaviour is undefined. If the format is exhausted while arguments
         * remain, the excess arguments are evaluated but otherwise ignored. The
         * fprintf function returns when the end of the format string is reached.
         * The format shall be a multibyte character sequence, beginning and ending
         * in its initial shift state. The format is composed of zero or more
         * directives: ordinary multibyte characters (not %), which are copied
         * unchanged to the output stream; and conversion specifiers, each of which
         * results in fetching zero or more subsequent arguments. Each conversion
         * specification is introduced by the character %. For a description of the
         * available conversion specifiers refer to section 4.9.6.1 in the ANSI
         * draft mentioned at the start of this file or to any modern textbook on C.
         * The minimum value for the maximum number of characters producable by any
         * single conversion is at least 509.
         * Returns: the number of characters transmitted, or a negative value if an
         *          output error occurred.
         */
#pragma __printf_args
        extern _ARMABI int _fprintf(FILE *__restrict /*stream*/,
                                    const char *__restrict /*format*/, ...) __attribute__((__nonnull__(1, 2)));
        /*
         * is equivalent to fprintf, but does not support floating-point formats.
         * You can use instead of fprintf to improve code size.
         * Returns: as fprintf.
         */
#pragma __printf_args
        extern _ARMABI int printf(const char *__restrict /*format*/, ...) __attribute__((__nonnull__(1)));
        /*
         * is equivalent to fprintf with the argument stdout interposed before the
         * arguments to printf.
         * Returns: the number of characters transmitted, or a negative value if an
         *          output error occurred.
         */
#pragma __printf_args
        extern _ARMABI int _printf(const char *__restrict /*format*/, ...) __attribute__((__nonnull__(1)));
        /*
         * is equivalent to printf, but does not support floating-point formats.
         * You can use instead of printf to improve code size.
         * Returns: as printf.
         */
#pragma __printf_args
        extern _ARMABI int sprintf(char *__restrict /*s*/, const char *__restrict /*format*/, ...) __attribute__((__nonnull__(1, 2)));
        /*
         * is equivalent to fprintf, except that the argument s specifies an array
         * into which the generated output is to be written, rather than to a
         * stream. A null character is written at the end of the characters written;
         * it is not counted as part of the returned sum.
         * Returns: the number of characters written to the array, not counting the
         *          terminating null character.
         */
#pragma __printf_args
        extern _ARMABI int _sprintf(char *__restrict /*s*/, const char *__restrict /*format*/, ...) __attribute__((__nonnull__(1, 2)));
        /*
         * is equivalent to sprintf, but does not support floating-point formats.
         * You can use instead of sprintf to improve code size.
         * Returns: as sprintf.
         */

#pragma __printf_args
        extern _ARMABI int __ARM_snprintf(char *__restrict /*s*/, size_t /*n*/,
                                          const char *__restrict /*format*/, ...) __attribute__((__nonnull__(3)));

#if !defined(__STRICT_ANSI__) || (defined(__STDC_VERSION__) && 199901L <= __STDC_VERSION__) || (defined(__cplusplus) && 201103L <= __cplusplus)
#pragma __printf_args
        extern _ARMABI int snprintf(char *__restrict /*s*/, size_t /*n*/,
                                    const char *__restrict /*format*/, ...) __attribute__((__nonnull__(3)));
        /*
         * is equivalent to fprintf, except that the argument s specifies an array
         * into which the generated output is to be written, rather than to a
         * stream. The argument n specifies the size of the output array, so as to
         * avoid overflowing the buffer.
         * A null character is written at the end of the characters written, even
         * if the formatting was not completed; it is not counted as part of the
         * returned sum. At most n characters of the output buffer are used,
         * _including_ the null character.
         * Returns: the number of characters that would have been written to the
         *          array, not counting the terminating null character, if the
         *          array had been big enough. So if the return is >=0 and <n, then
         *          the entire string was successfully formatted; if the return is
         *          >=n, the string was truncated (but there is still a null char
         *          at the end of what was written); if the return is <0, there was
         *          an error.
         */
#endif
#pragma __printf_args
        extern _ARMABI int _snprintf(char *__restrict /*s*/, size_t /*n*/,
                                     const char *__restrict /*format*/, ...) __attribute__((__nonnull__(3)));
        /*
         * is equivalent to snprintf, but does not support floating-point formats.
         * You can use instead of snprintf to improve code size.
         * Returns: as snprintf.
         */
#pragma __scanf_args
        extern _ARMABI int fscanf(FILE *__restrict /*stream*/,
                                  const char *__restrict /*format*/, ...) __attribute__((__nonnull__(1, 2)));
        /*
         * reads input from the stream pointed to by stream, under control of the
         * string pointed to by format that specifies the admissible input sequences
         * and how thay are to be converted for assignment, using subsequent
         * arguments as pointers to the objects to receive the converted input. If
         * there are insufficient arguments for the format, the behaviour is
         * undefined. If the format is exhausted while arguments remain, the excess
         * arguments are evaluated but otherwise ignored.
         * The format is composed of zero or more directives: one or more
         * white-space characters; an ordinary character (not %); or a conversion
         * specification. Each conversion specification is introduced by the
         * character %. For a description of the available conversion specifiers
         * refer to section 4.9.6.2 in the ANSI draft mentioned at the start of this
         * file, or to any modern textbook on C.
         * If end-of-file is encountered during input, conversion is terminated. If
         * end-of-file occurs before any characters matching the current directive
         * have been read (other than leading white space, where permitted),
         * execution of the current directive terminates with an input failure;
         * otherwise, unless execution of the current directive is terminated with a
         * matching failure, execution of the following directive (if any) is
         * terminated with an input failure.
         * If conversions terminates on a conflicting input character, the offending
         * input character is left unread in the input strem. Trailing white space
         * (including new-line characters) is left unread unless matched by a
         * directive. The success of literal matches and suppressed asignments is
         * not directly determinable other than via the %n directive.
         * Returns: the value of the macro EOF if an input failure occurs before any
         *          conversion. Otherwise, the fscanf function returns the number of
         *          input items assigned, which can be fewer than provided for, or
         *          even zero, in the event of an early conflict between an input
         *          character and the format.
         */
#pragma __scanf_args
        extern _ARMABI int _fscanf(FILE *__restrict /*stream*/,
                                   const char *__restrict /*format*/, ...) __attribute__((__nonnull__(1, 2)));
        /*
         * is equivalent to fscanf, but does not support floating-point formats.
         * You can use instead of fscanf to improve code size.
         * Returns: as fscanf.
         */
#pragma __scanf_args
        extern _ARMABI int scanf(const char *__restrict /*format*/, ...) __attribute__((__nonnull__(1)));
        /*
         * is equivalent to fscanf with the argument stdin interposed before the
         * arguments to scanf.
         * Returns: the value of the macro EOF if an input failure occurs before any
         *          conversion. Otherwise, the scanf function returns the number of
         *          input items assigned, which can be fewer than provided for, or
         *          even zero, in the event of an early matching failure.
         */
#pragma __scanf_args
        extern _ARMABI int _scanf(const char *__restrict /*format*/, ...) __attribute__((__nonnull__(1)));
        /*
         * is equivalent to scanf, but does not support floating-point formats.
         * You can use instead of scanf to improve code size.
         * Returns: as scanf.
         */
#pragma __scanf_args
        extern _ARMABI int sscanf(const char *__restrict /*s*/,
                                  const char *__restrict /*format*/, ...) __attribute__((__nonnull__(1, 2)));
        /*
         * is equivalent to fscanf except that the argument s specifies a string
         * from which the input is to be obtained, rather than from a stream.
         * Reaching the end of the string is equivalent to encountering end-of-file
         * for the fscanf function.
         * Returns: the value of the macro EOF if an input failure occurs before any
         *          conversion. Otherwise, the scanf function returns the number of
         *          input items assigned, which can be fewer than provided for, or
         *          even zero, in the event of an early matching failure.
         */
#pragma __scanf_args
        extern _ARMABI int _sscanf(const char *__restrict /*s*/,
                                   const char *__restrict /*format*/, ...) __attribute__((__nonnull__(1, 2)));
        /*
         * is equivalent to sscanf, but does not support floating-point formats.
         * You can use instead of sscanf to improve code size.
         * Returns: as sscanf.
         */
#if !defined(__STRICT_ANSI__) || (defined(__STDC_VERSION__) && 199901L <= __STDC_VERSION__) || (defined(__cplusplus) && 201103L <= __cplusplus)
        /* C99 additions */
        extern _ARMABI int vfscanf(FILE *__restrict /*stream*/, const char *__restrict /*format*/, __va_list) __attribute__((__nonnull__(1, 2)));
        extern _ARMABI int vscanf(const char *__restrict /*format*/, __va_list) __attribute__((__nonnull__(1)));
        extern _ARMABI int vsscanf(const char *__restrict /*s*/, const char *__restrict /*format*/, __va_list) __attribute__((__nonnull__(1, 2)));
#endif
        extern _ARMABI int _vfscanf(FILE *__restrict /*stream*/, const char *__restrict /*format*/, __va_list) __attribute__((__nonnull__(1, 2)));
        extern _ARMABI int _vscanf(const char *__restrict /*format*/, __va_list) __attribute__((__nonnull__(1)));
        extern _ARMABI int _vsscanf(const char *__restrict /*s*/, const char *__restrict /*format*/, __va_list) __attribute__((__nonnull__(1, 2)));
        extern _ARMABI int __ARM_vsscanf(const char *__restrict /*s*/, const char *__restrict /*format*/, __va_list) __attribute__((__nonnull__(1, 2)));

        extern _ARMABI int vprintf(const char *__restrict /*format*/, __va_list /*arg*/) __attribute__((__nonnull__(1)));
        /*
         * is equivalent to printf, with the variable argument list replaced by arg,
         * which has been initialised by the va_start macro (and possibly subsequent
         * va_arg calls). The vprintf function does not invoke the va_end function.
         * Returns: the number of characters transmitted, or a negative value if an
         *          output error occurred.
         */
        extern _ARMABI int _vprintf(const char *__restrict /*format*/, __va_list /*arg*/) __attribute__((__nonnull__(1)));
        /*
         * is equivalent to vprintf, but does not support floating-point formats.
         * You can use instead of vprintf to improve code size.
         * Returns: as vprintf.
         */
        extern _ARMABI int vfprintf(FILE *__restrict /*stream*/,
                                    const char *__restrict /*format*/, __va_list /*arg*/) __attribute__((__nonnull__(1, 2)));
        /*
         * is equivalent to fprintf, with the variable argument list replaced by
         * arg, which has been initialised by the va_start macro (and possibly
         * subsequent va_arg calls). The vfprintf function does not invoke the
         * va_end function.
         * Returns: the number of characters transmitted, or a negative value if an
         *          output error occurred.
         */
        extern _ARMABI int vsprintf(char *__restrict /*s*/,
                                    const char *__restrict /*format*/, __va_list /*arg*/) __attribute__((__nonnull__(1, 2)));
        /*
         * is equivalent to sprintf, with the variable argument list replaced by
         * arg, which has been initialised by the va_start macro (and possibly
         * subsequent va_arg calls). The vsprintf function does not invoke the
         * va_end function.
         * Returns: the number of characters written in the array, not counting the
         *          terminating null character.
         */
        extern _ARMABI int __ARM_vsnprintf(char *__restrict /*s*/, size_t /*n*/,
                                           const char *__restrict /*format*/, __va_list /*arg*/) __attribute__((__nonnull__(3)));
#if !defined(__STRICT_ANSI__) || (defined(__STDC_VERSION__) && 199901L <= __STDC_VERSION__) || (defined(__cplusplus) && 201103L <= __cplusplus)
        extern _ARMABI int vsnprintf(char *__restrict /*s*/, size_t /*n*/,
                                     const char *__restrict /*format*/, __va_list /*arg*/) __attribute__((__nonnull__(3)));
        /*
         * is equivalent to snprintf, with the variable argument list replaced by
         * arg, which has been initialised by the va_start macro (and possibly
         * subsequent va_arg calls). The vsprintf function does not invoke the
         * va_end function.
         * Returns: the number of characters that would have been written in the
         *          array, not counting the terminating null character. As
         *          snprintf.
         */
#endif
        extern _ARMABI int _vsprintf(char *__restrict /*s*/,
                                     const char *__restrict /*format*/, __va_list /*arg*/) __attribute__((__nonnull__(1, 2)));
        /*
         * is equivalent to vsprintf, but does not support floating-point formats.
         * You can use instead of vsprintf to improve code size.
         * Returns: as vsprintf.
         */
        extern _ARMABI int _vfprintf(FILE *__restrict /*stream*/,
                                     const char *__restrict /*format*/, __va_list /*arg*/) __attribute__((__nonnull__(1, 2)));
        /*
         * is equivalent to vfprintf, but does not support floating-point formats.
         * You can use instead of vfprintf to improve code size.
         * Returns: as vfprintf.
         */
        extern _ARMABI int _vsnprintf(char *__restrict /*s*/, size_t /*n*/,
                                      const char *__restrict /*format*/, __va_list /*arg*/) __attribute__((__nonnull__(3)));
        /*
         * is equivalent to vsnprintf, but does not support floating-point formats.
         * You can use instead of vsnprintf to improve code size.
         * Returns: as vsnprintf.
         */
#if !defined(__STRICT_ANSI__)
#pragma __printf_args
        extern _ARMABI int asprintf(char ** /*strp*/, const char *__restrict /*format*/, ...) __attribute__((__nonnull__(2)));
        extern _ARMABI int vasprintf(char ** /*strp*/, const char *__restrict /*format*/, __va_list /*arg*/) __attribute__((__nonnull__(2)));
#endif
#pragma __printf_args
        extern _ARMABI int __ARM_asprintf(char ** /*strp*/, const char *__restrict /*format*/, ...) __attribute__((__nonnull__(2)));
        extern _ARMABI int __ARM_vasprintf(char ** /*strp*/, const char *__restrict /*format*/, __va_list /*arg*/) __attribute__((__nonnull__(2)));
        /*
         * dynamically allocates a buffer of the right size for the
         * formatted string, and returns it in (*strp). Formal return value
         * is the same as any other printf variant, except that it returns
         * -1 if the buffer could not be allocated.
         *
         * (The functions with __ARM_ prefixed names are identical to the
         * ones without, but are available in all compilation modes without
         * violating user namespace.)
         */

        extern _ARMABI int fgetc(FILE * /*stream*/) __attribute__((__nonnull__(1)));
        /*
         * obtains the next character (if present) as an unsigned char converted to
         * an int, from the input stream pointed to by stream, and advances the
         * associated file position indicator (if defined).
         * Returns: the next character from the input stream pointed to by stream.
         *          If the stream is at end-of-file, the end-of-file indicator is
         *          set and fgetc returns EOF. If a read error occurs, the error
         *          indicator is set and fgetc returns EOF.
         */
        extern _ARMABI char *fgets(char *__restrict /*s*/, int /*n*/,
                                   FILE *__restrict /*stream*/) __attribute__((__nonnull__(1, 3)));
        /*
         * reads at most one less than the number of characters specified by n from
         * the stream pointed to by stream into the array pointed to by s. No
         * additional characters are read after a new-line character (which is
         * retained) or after end-of-file. A null character is written immediately
         * after the last character read into the array.
         * Returns: s if successful. If end-of-file is encountered and no characters
         *          have been read into the array, the contents of the array remain
         *          unchanged and a null pointer is returned. If a read error occurs
         *          during the operation, the array contents are indeterminate and a
         *          null pointer is returned.
         */
        extern _ARMABI int fputc(int /*c*/, FILE * /*stream*/) __attribute__((__nonnull__(2)));
        /*
         * writes the character specified by c (converted to an unsigned char) to
         * the output stream pointed to by stream, at the position indicated by the
         * asociated file position indicator (if defined), and advances the
         * indicator appropriately. If the file position indicator is not defined,
         * the character is appended to the output stream.
         * Returns: the character written. If a write error occurs, the error
         *          indicator is set and fputc returns EOF.
         */
        extern _ARMABI int fputs(const char *__restrict /*s*/, FILE *__restrict /*stream*/) __attribute__((__nonnull__(1, 2)));
        /*
         * writes the string pointed to by s to the stream pointed to by stream.
         * The terminating null character is not written.
         * Returns: EOF if a write error occurs; otherwise it returns a nonnegative
         *          value.
         */
        extern _ARMABI int getc(FILE * /*stream*/) __attribute__((__nonnull__(1)));
        /*
         * is equivalent to fgetc except that it may be implemented as an unsafe
         * macro (stream may be evaluated more than once, so the argument should
         * never be an expression with side-effects).
         * Returns: the next character from the input stream pointed to by stream.
         *          If the stream is at end-of-file, the end-of-file indicator is
         *          set and getc returns EOF. If a read error occurs, the error
         *          indicator is set and getc returns EOF.
         */
#ifdef __cplusplus
        inline int getchar()
        {
            return getc(stdin);
        }
#else
#define getchar() getc(stdin)
extern _ARMABI int(getchar)(void);
#endif
        /*
         * is equivalent to getc with the argument stdin.
         * Returns: the next character from the input stream pointed to by stdin.
         *          If the stream is at end-of-file, the end-of-file indicator is
         *          set and getchar returns EOF. If a read error occurs, the error
         *          indicator is set and getchar returns EOF.
         */
        extern _ARMABI char *gets(char * /*s*/) __attribute__((__nonnull__(1)));
        /*
         * reads characters from the input stream pointed to by stdin into the array
         * pointed to by s, until end-of-file is encountered or a new-line character
         * is read. Any new-line character is discarded, and a null character is
         * written immediately after the last character read into the array.
         * Returns: s if successful. If end-of-file is encountered and no characters
         *          have been read into the array, the contents of the array remain
         *          unchanged and a null pointer is returned. If a read error occurs
         *          during the operation, the array contents are indeterminate and a
         *          null pointer is returned.
         */
        extern _ARMABI int putc(int /*c*/, FILE * /*stream*/) __attribute__((__nonnull__(2)));
        /*
         * is equivalent to fputc except that it may be implemented as aan unsafe
         * macro (stream may be evaluated more than once, so the argument should
         * never be an expression with side-effects).
         * Returns: the character written. If a write error occurs, the error
         *          indicator is set and putc returns EOF.
         */
#ifdef __cplusplus
        inline int putchar(int __c)
        {
            return putc(__c, stdout);
        }
#else
#define putchar(c) putc(c, stdout)
extern _ARMABI int(putchar)(int /*c*/);
#endif
        /*
         * is equivalent to putc with the second argument stdout.
         * Returns: the character written. If a write error occurs, the error
         *          indicator is set and putc returns EOF.
         */
        extern _ARMABI int puts(const char * /*s*/) __attribute__((__nonnull__(1)));
        /*
         * writes the string pointed to by s to the stream pointed to by stdout, and
         * appends a new-line character to the output. The terminating null
         * character is not written.
         * Returns: EOF if a write error occurs; otherwise it returns a nonnegative
         *          value.
         */
        extern _ARMABI int ungetc(int /*c*/, FILE * /*stream*/) __attribute__((__nonnull__(2)));
        /*
         * pushes the character specified by c (converted to an unsigned char) back
         * onto the input stream pointed to by stream. The character will be
         * returned by the next read on that stream. An intervening call to the
         * fflush function or to a file positioning function (fseek, fsetpos,
         * rewind) discards any pushed-back characters. The extern _ARMABIal storage
         * corresponding to the stream is unchanged.
         * One character pushback is guaranteed. If the unget function is called too
         * many times on the same stream without an intervening read or file
         * positioning operation on that stream, the operation may fail.
         * If the value of c equals that of the macro EOF, the operation fails and
         * the input stream is unchanged.
         * A successful call to the ungetc function clears the end-of-file
         * indicator. The value of the file position indicator after reading or
         * discarding all pushed-back characters shall be the same as it was before
         * the characters were pushed back. For a text stream, the value of the file
         * position indicator after a successful call to the ungetc function is
         * unspecified until all pushed-back characters are read or discarded. For a
         * binary stream, the file position indicator is decremented by each
         * successful call to the ungetc function; if its value was zero before a
         * call, it is indeterminate after the call.
         * Returns: the character pushed back after conversion, or EOF if the
         *          operation fails.
         */

        extern _ARMABI size_t fread(void *__restrict /*ptr*/,
                                    size_t /*size*/, size_t /*nmemb*/, FILE *__restrict /*stream*/) __attribute__((__nonnull__(1, 4)));
        /*
         * reads into the array pointed to by ptr, up to nmemb members whose size is
         * specified by size, from the stream pointed to by stream. The file
         * position indicator (if defined) is advanced by the number of characters
         * successfully read. If an error occurs, the resulting value of the file
         * position indicator is indeterminate. If a partial member is read, its
         * value is indeterminate. The ferror or feof function shall be used to
         * distinguish between a read error and end-of-file.
         * Returns: the number of members successfully read, which may be less than
         *          nmemb if a read error or end-of-file is encountered. If size or
         *          nmemb is zero, fread returns zero and the contents of the array
         *          and the state of the stream remain unchanged.
         */

        extern _ARMABI size_t __fread_bytes_avail(void *__restrict /*ptr*/,
                                                  size_t /*count*/, FILE *__restrict /*stream*/) __attribute__((__nonnull__(1, 3)));
        /*
         * reads into the array pointed to by ptr, up to count characters from the
         * stream pointed to by stream. The file position indicator (if defined)
         * is advanced by the number of characters successfully read. If an error
         * occurs, the resulting value of the file position indicator is
         * indeterminate. The ferror or feof function shall be used to
         * distinguish between a read error and end-of-file.  The call will block
         * only if no characters are available.
         * Returns: the number of characters successfully read, which may be less than
         *          count. If count is zero, __fread_bytes_avail returns zero and
         *          the contents of the array and the state of the stream remain
         *          unchanged.
         */

        extern _ARMABI size_t fwrite(const void *__restrict /*ptr*/,
                                     size_t /*size*/, size_t /*nmemb*/, FILE *__restrict /*stream*/) __attribute__((__nonnull__(1, 4)));
        /*
         * writes, from the array pointed to by ptr up to nmemb members whose size
         * is specified by size, to the stream pointed to by stream. The file
         * position indicator (if defined) is advanced by the number of characters
         * successfully written. If an error occurs, the resulting value of the file
         * position indicator is indeterminate.
         * Returns: the number of members successfully written, which will be less
         *          than nmemb only if a write error is encountered.
         */

        extern _ARMABI int fgetpos(FILE *__restrict /*stream*/, fpos_t *__restrict /*pos*/) __attribute__((__nonnull__(1, 2)));
        /*
         * stores the current value of the file position indicator for the stream
         * pointed to by stream in the object pointed to by pos. The value stored
         * contains unspecified information usable by the fsetpos function for
         * repositioning the stream to its position at the time  of the call to the
         * fgetpos function.
         * Returns: zero, if successful. Otherwise nonzero is returned and the
         *          integer expression errno is set to an implementation-defined
         *          nonzero value.
         */
        extern _ARMABI int fseek(FILE * /*stream*/, long int /*offset*/, int /*whence*/) __attribute__((__nonnull__(1)));
        /*
         * sets the file position indicator for the stream pointed to by stream.
         * For a binary stream, the new position is at the signed number of
         * characters specified by offset away from the point specified by whence.
         * The specified point is the beginning of the file for SEEK_SET, the
         * current position in the file for SEEK_CUR, or end-of-file for SEEK_END.
         * A binary stream need not meaningfully support fseek calls with a whence
         * value of SEEK_END.
         * For a text stream, either offset shall be zero, or offset shall be a
         * value returned by an earlier call to the ftell function on the same
         * stream and whence shall be SEEK_SET.
         * The fseek function clears the end-of-file indicator and undoes any
         * effects of the ungetc function on the same stream. After an fseek call,
         * the next operation on an update stream may be either input or output.
         * Returns: nonzero only for a request that cannot be satisfied.
         */
        extern _ARMABI int fsetpos(FILE *__restrict /*stream*/, const fpos_t *__restrict /*pos*/) __attribute__((__nonnull__(1, 2)));
        /*
         * sets  the file position indicator for the stream pointed to by stream
         * according to the value of the object pointed to by pos, which shall be a
         * value returned by an earlier call to the fgetpos function on the same
         * stream.
         * The fsetpos function clears the end-of-file indicator and undoes any
         * effects of the ungetc function on the same stream. After an fsetpos call,
         * the next operation on an update stream may be either input or output.
         * Returns: zero, if successful. Otherwise nonzero is returned and the
         *          integer expression errno is set to an implementation-defined
         *          nonzero value.
         */
        extern _ARMABI long int ftell(FILE * /*stream*/) __attribute__((__nonnull__(1)));
        /*
         * obtains the current value of the file position indicator for the stream
         * pointed to by stream. For a binary stream, the value is the number of
         * characters from the beginning of the file. For a text stream, the file
         * position indicator contains unspecified information, usable by the fseek
         * function for returning the file position indicator to its position at the
         * time of the ftell call; the difference between two such return values is
         * not necessarily a meaningful measure of the number of characters written
         * or read.
         * Returns: if successful, the current value of the file position indicator.
         *          On failure, the ftell function returns -1L and sets the integer
         *          expression errno to an implementation-defined nonzero value.
         */
        extern _ARMABI void rewind(FILE * /*stream*/) __attribute__((__nonnull__(1)));
        /*
         * sets the file position indicator for the stream pointed to by stream to
         * the beginning of the file. It is equivalent to
         *          (void)fseek(stream, 0L, SEEK_SET)
         * except that the error indicator for the stream is also cleared.
         * Returns: no value.
         */

        extern _ARMABI void clearerr(FILE * /*stream*/) __attribute__((__nonnull__(1)));
        /*
         * clears the end-of-file and error indicators for the stream pointed to by
         * stream. These indicators are cleared only when the file is opened or by
         * an explicit call to the clearerr function or to the rewind function.
         * Returns: no value.
         */

        extern _ARMABI int feof(FILE * /*stream*/) __attribute__((__nonnull__(1)));
        /*
         * tests the end-of-file indicator for the stream pointed to by stream.
         * Returns: nonzero iff the end-of-file indicator is set for stream.
         */
        extern _ARMABI int ferror(FILE * /*stream*/) __attribute__((__nonnull__(1)));
        /*
         * tests the error indicator for the stream pointed to by stream.
         * Returns: nonzero iff the error indicator is set for stream.
         */
        extern _ARMABI void perror(const char * /*s*/);
        /*
         * maps the error number  in the integer expression errno to an error
         * message. It writes a sequence of characters to the standard error stream
         * thus: first (if s is not a null pointer and the character pointed to by
         * s is not the null character), the string pointed to by s followed by a
         * colon and a space; then an appropriate error message string followed by
         * a new-line character. The contents of the error message strings are the
         * same as those returned by the strerror function with argument errno,
         * which are implementation-defined.
         * Returns: no value.
         */

        extern _ARMABI int _fisatty(FILE * /*stream*/) __attribute__((__nonnull__(1)));
        /* Returns 1 if the stream is tty (stdin), 0 otherwise. Not ANSI compliant.
         */

        extern _ARMABI void __use_no_semihosting_swi(void);
        extern _ARMABI void __use_no_semihosting(void);
        /*
         * Referencing either of these symbols will cause a link-time
         * error if any library functions that use semihosting SWI
         * calls are also present in the link, i.e. you define it if
         * you want to make sure you haven't accidentally used any such
         * SWIs.
         */

#ifdef __cplusplus
    } /* extern "C" */
} /* namespace std */
#endif
#endif /* __STDIO_DECLS */

#if _AEABI_PORTABILITY_LEVEL != 0 && !defined _AEABI_PORTABLE
#define _AEABI_PORTABLE
#endif

#if defined(__cplusplus) && !defined(__STDIO_NO_EXPORTS)
using ::std::_fprintf;
using ::std::_printf;
using ::std::_sprintf;
using ::std::fclose;
using ::std::fflush;
using ::std::FILE;
using ::std::fopen;
using ::std::fpos_t;
using ::std::fprintf;
using ::std::freopen;
using ::std::printf;
using ::std::remove;
using ::std::rename;
using ::std::setbuf;
using ::std::setvbuf;
using ::std::size_t;
using ::std::sprintf;
using ::std::tmpfile;
using ::std::tmpnam;
#if !defined(__STRICT_ANSI__) || (defined(__STDC_VERSION__) && 199901L <= __STDC_VERSION__) || (defined(__cplusplus) && 201103L <= __cplusplus)
using ::std::snprintf;
using ::std::vfscanf;
using ::std::vscanf;
using ::std::vsnprintf;
using ::std::vsscanf;
#endif
using ::std::_snprintf;
using ::std::_vsnprintf;
#if !defined(__STRICT_ANSI__)
using ::std::asprintf;
using ::std::vasprintf;
#endif
using ::std::__ARM_asprintf;
using ::std::__ARM_snprintf;
using ::std::__ARM_vasprintf;
using ::std::__ARM_vsnprintf;
using ::std::__ARM_vsscanf;
using ::std::__fread_bytes_avail;
using ::std::__use_no_semihosting;
using ::std::__use_no_semihosting_swi;
using ::std::_fisatty;
using ::std::_fscanf;
using ::std::_scanf;
using ::std::_sscanf;
using ::std::_vfprintf;
using ::std::_vfscanf;
using ::std::_vprintf;
using ::std::_vscanf;
using ::std::_vsprintf;
using ::std::_vsscanf;
using ::std::clearerr;
using ::std::feof;
using ::std::ferror;
using ::std::fgetc;
using ::std::fgetpos;
using ::std::fgets;
using ::std::fputc;
using ::std::fputs;
using ::std::fread;
using ::std::fscanf;
using ::std::fseek;
using ::std::fsetpos;
using ::std::ftell;
using ::std::fwrite;
using ::std::getc;
using ::std::getchar;
using ::std::gets;
using ::std::perror;
using ::std::putc;
using ::std::putchar;
using ::std::puts;
using ::std::rewind;
using ::std::scanf;
using ::std::sscanf;
using ::std::ungetc;
using ::std::vfprintf;
using ::std::vprintf;
using ::std::vsprintf;
#endif

#endif /* ndef __stdio_h */

/* end of stdio.h */
