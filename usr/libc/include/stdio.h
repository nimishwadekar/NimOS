#ifndef _STDIO_H
#define _STDIO_H 1

#include <stddef.h>
#include <stdarg.h>

#include <bits/types/FILE.h>
#include <bits/types/struct_FILE.h>

#define BUFSIZ 8192
#define EOF (-1)

#define SEEK_SET	0
#define SEEK_CUR	1
#define SEEK_END	2

/*
DESCRIPTION
       The fclose() function flushes the stream pointed to by stream (writing any buffered output data
       using fflush(3)) and closes the underlying file descriptor.

       The behaviour of fclose() is undefined if the stream parameter is an illegal pointer, or  is  a
       descriptor already passed to a previous invocation of fclose().

RETURN VALUE
       Upon  successful completion, 0 is returned.  Otherwise, EOF is returned and errno is set to in‐
       dicate the error.  In either case, any further access (including another call to  fclose())  to
       the stream results in undefined behavior.
*/
int fclose(FILE *stream);


/*
DESCRIPTION
       The  fopen()  function opens the file whose name is the string pointed to by pathname and asso‐
       ciates a stream with it.

       The argument mode points to a string beginning with one of the  following  sequences  (possibly
       followed by additional characters, as described below):

       r      Open text file for reading.  The stream is positioned at the beginning of the file.

       r+     Open for reading and writing.  The stream is positioned at the beginning of the file.

       w      Truncate  file to zero length or create text file for writing.  The stream is positioned
              at the beginning of the file.

       w+     Open for reading and writing.  The file is created if it does not exist, otherwise it is
              truncated.  The stream is positioned at the beginning of the file.

       a      Open  for appending (writing at end of file).  The file is created if it does not exist.
              The stream is positioned at the end of the file.

       a+     Open for reading and appending (writing at end of file).  The file is created if it does
              not  exist.   Output is always appended to the end of the file.  POSIX is silent on what
              the initial read position is when using this mode.  For glibc, the initial file position
              for reading is at the beginning of the file, but for Android/BSD/MacOS, the initial file
              position for reading is at the end of the file.
*/
FILE *fopen(const char *pathname, const char *mode);


/*
Print format string.
*/
int printf(const char *format, ...);
/**/
int sprintf(char *str, const char *format, ...);

/*
DESCRIPTION
       The  scanf() family of functions scans input according to format as described below.  This for‐
       mat may contain conversion specifications; the results  from  such  conversions,  if  any,  are
       stored  in  the locations pointed to by the pointer arguments that follow format.  Each pointer
       argument must be of a type that is appropriate for the value returned by the corresponding con‐
       version specification.

       If  the  number of conversion specifications in format exceeds the number of pointer arguments,
       the results are undefined.  If the number of pointer arguments exceeds the number of conversion
       specifications, then the excess pointer arguments are evaluated, but are otherwise ignored.

       The  scanf()  function  reads  input from the standard input stream stdin, fscanf() reads input
       from the stream pointer stream, and sscanf() reads its input from the character string  pointed
       to by str.

       The  vfscanf()  function  is  analogous  to vfprintf(3) and reads input from the stream pointer
       stream using a variable argument list of pointers (see stdarg(3).  The vscanf() function  scans
       a  variable  argument  list  from the standard input and the vsscanf() function scans it from a
       string; these are analogous to the vprintf(3) and vsprintf(3) functions respectively.

       The format string consists of a sequence of directives which describe how to  process  the  se‐
       quence  of input characters.  If processing of a directive fails, no further input is read, and
       scanf() returns.  A "failure" can be either of the following: input failure, meaning that input
       characters were unavailable, or matching failure, meaning that the input was inappropriate (see
       below).

        A directive is one of the following:

       •      A sequence of white-space characters (space, tab, newline, etc.; see isspace(3)).   This
              directive matches any amount of white space, including none, in the input.

       •      An  ordinary  character  (i.e., one other than white space or '%').  This character must
              exactly match the next character of input.

       •      A conversion specification, which commences with a '%' (percent) character.  A  sequence
              of  characters  from the input is converted according to this specification, and the re‐
              sult is placed in the corresponding pointer argument.  If the next item  of  input  does
              not match the conversion specification, the conversion fails—this is a matching failure.

       Each  conversion  specification in format begins with either the character '%' or the character
       sequence "%n$" (see below for the distinction) followed by:

       •      An optional '*' assignment-suppression character: scanf() reads input as directed by the
              conversion  specification, but discards the input.  No corresponding pointer argument is
              required, and this specification is not included in the count of successful  assignments
              returned by scanf().

       •      For decimal conversions, an optional quote character (').  This specifies that the input
              number may include thousands' separators as defined by the LC_NUMERIC  category  of  the
              current  locale.  (See setlocale(3).)  The quote character may precede or follow the '*'
              assignment-suppression character.

       •      An optional 'm' character.  This is used with string conversions (%s, %c, %[),  and  re‐
              lieves  the caller of the need to allocate a corresponding buffer to hold the input: in‐
              stead, scanf() allocates a buffer of sufficient size, and assigns the  address  of  this
              buffer  to  the  corresponding  pointer  argument, which should be a pointer to a char *
              variable (this variable does not need to be initialized before the  call).   The  caller
              should subsequently free(3) this buffer when it is no longer required.

       •      An optional decimal integer which specifies the maximum field width.  Reading of charac‐
              ters stops either when this maximum is reached or when a nonmatching character is found,
              whichever  happens  first.  Most conversions discard initial white space characters (the
              exceptions are noted below), and these discarded characters don't count toward the maxi‐
              mum  field width.  String input conversions store a terminating null byte ('\0') to mark
              the end of the input; the maximum field width does not include this terminator.

       •      An optional type modifier character.  For example, the l type modifier is used with  in‐
              teger  conversions  such as %d to specify that the corresponding pointer argument refers
              to a long int rather than a pointer to an int.

       •      A conversion specifier that specifies the type of input conversion to be performed.

       The conversion specifications in format are of two forms, either beginning with '%'  or  begin‐
       ning  with  "%n$".   The two forms should not be mixed in the same format string, except that a
       string containing "%n$" specifications can include %% and %*.  If format contains '%'  specifi‐
       cations,  then  these correspond in order with successive pointer arguments.  In the "%n$" form
       (which is specified in POSIX.1-2001, but not C99), n is a decimal integer that  specifies  that
       the  converted  input should be placed in the location referred to by the n-th pointer argument
       following format.

       Conversions
       The following type modifier characters can appear in a conversion specification:

       h      Indicates that the conversion will be one of d, i, o, u, x, X, or n and the next pointer
              is a pointer to a short int or unsigned short int (rather than int).

       hh     As for h, but the next pointer is a pointer to a signed char or unsigned char.

       j      As  for  h, but the next pointer is a pointer to an intmax_t or a uintmax_t.  This modi‐
              fier was introduced in C99.

       l      Indicates either that the conversion will be one of d, i, o, u, x, X, or n and the  next
              pointer  is  a pointer to a long int or unsigned long int (rather than int), or that the
              conversion will be one of e, f, or g and the next pointer is a pointer to double (rather
              than  float).   Specifying  two l characters is equivalent to L.  If used with %c or %s,
              the corresponding parameter is considered as a pointer to a wide character or wide-char‐
              acter string respectively.

       L      Indicates  that  the  conversion  will  be  either  e, f, or g and the next pointer is a
              pointer to long double or the conversion will be d, i, o, u, or x and the  next  pointer
              is a pointer to long long.

       q      equivalent to L.  This specifier does not exist in ANSI C.

       t      As  for  h,  but the next pointer is a pointer to a ptrdiff_t.  This modifier was intro‐
              duced in C99.

       The following conversion specifiers are available:

       %      Matches  a  literal  '%'.   That  is, %% in the format string matches a single input '%'
              character.  No conversion is done (but initial white space  characters  are  discarded),
              and assignment does not occur.

       d      Matches an optionally signed decimal integer; the next pointer must be a pointer to int.

       D      Equivalent  to  ld;  this  exists  only for backward compatibility.  (Note: thus only in
              libc4.  In libc5 and glibc the %D is silently ignored, causing old programs to fail mys‐
              teriously.)

       i      Matches  an  optionally  signed integer; the next pointer must be a pointer to int.  The
              integer is read in base 16 if it begins with 0x or 0X, in base 8 if it  begins  with  0,
              and in base 10 otherwise.  Only characters that correspond to the base are used.

       o      Matches an unsigned octal integer; the next pointer must be a pointer to unsigned int.

       u      Matches an unsigned decimal integer; the next pointer must be a pointer to unsigned int.

       x      Matches  an unsigned hexadecimal integer; the next pointer must be a pointer to unsigned
              int.

       X      Equivalent to x.

       f      Matches an optionally signed floating-point number; the next pointer must be  a  pointer
              to float.

       e      Equivalent to f.

        g      Equivalent to f.

       E      Equivalent to f.

       a      (C99) Equivalent to f.

       s      Matches  a sequence of non-white-space characters; the next pointer must be a pointer to
              the initial element of a character array that is long enough to hold the input  sequence
              and  the  terminating  null byte ('\0'), which is added automatically.  The input string
              stops at white space or at the maximum field width, whichever occurs first.

       c      Matches a sequence of characters whose length is specified by the  maximum  field  width
              (default  1);  the next pointer must be a pointer to char, and there must be enough room
              for all the characters (no terminating null byte is added).  The usual skip  of  leading
              white space is suppressed.  To skip white space first, use an explicit space in the for‐
              mat.

        p      Matches  a  pointer  value  (as  printed  by %p in printf(3); the next pointer must be a
              pointer to a pointer to void.

       n      Nothing is expected; instead, the number of characters consumed thus far from the  input
              is  stored through the next pointer, which must be a pointer to int.  This is not a con‐
              version and does not increase the count returned by the function.  The assignment can be
              suppressed  with  the  *  assignment-suppression character, but the effect on the return
              value is undefined.  Therefore %*n conversions should not be used.

RETURN VALUE
       On success, these functions return the number of input items successfully matched and assigned;
       this can be fewer than provided for, or even zero, in the event of an early matching failure.

       The  value  EOF  is  returned if the end of input is reached before either the first successful
       conversion or a matching failure occurs.  EOF is also returned if a read error occurs, in which
       case  the  error  indicator for the stream (see ferror(3)) is set, and errno is set to indicate
       the error.
*/
int scanf(const char *format, ...);
/**/
int sscanf(const char *str, const char *format, ...);

/*
Reads a single character from stdin.
*/
int getchar(void);

/*
Writes a single character to stdout.
*/
int putchar(int c);

/*
Writes a line to stdout.
*/
int puts(const char *s);

/*
DESCRIPTION
       The  function  fread() reads nmemb items of data, each size bytes long, from the stream pointed
       to by stream, storing them at the location given by ptr.

       The function fwrite() writes nmemb items of data, each size bytes long, to the  stream  pointed
       to by stream, obtaining them from the location given by ptr.

       For nonlocking counterparts, see unlocked_stdio(3).

RETURN VALUE
       On  success,  fread()  and  fwrite()  return  the number of items read or written.  This number
       equals the number of bytes transferred only when size is 1.  If an error occurs, or the end  of
       the file is reached, the return value is a short item count (or zero).
*/
size_t fread(void *ptr, size_t size, size_t nmemb, FILE *stream);


/*
DESCRIPTION
       The fseek() function sets the file position indicator for the stream pointed to by stream.  The
       new position, measured in bytes, is obtained by adding offset bytes to the  position  specified
       by  whence.  If whence is set to SEEK_SET, SEEK_CUR, or SEEK_END, the offset is relative to the
       start of the file, the current position indicator, or end-of-file, respectively.  A  successful
       call to the fseek() function clears the end-of-file indicator for the stream and undoes any ef‐
       fects of the ungetc(3) function on the same stream.

       The ftell() function obtains the current value of the file position indicator  for  the  stream
       pointed to by stream.

RETURN VALUE
       The  rewind() function returns no value.  Upon successful completion, fgetpos(), fseek(), fset‐
       pos() return 0, and ftell() returns the current offset.  Otherwise, -1 is returned and errno is
       set to indicate the error.
*/
int fseek(FILE *stream, long offset, int whence);
/**/
long ftell(FILE *stream);


/*
DESCRIPTION
       The  function  clearerr() clears the end-of-file and error indicators for the stream pointed to
       by stream.

       The function feof() tests the end-of-file indicator for the stream pointed to  by  stream,  re‐
       turning  nonzero  if  it is set.  The end-of-file indicator can be cleared only by the function
       clearerr().

       The function ferror() tests the error indicator for the stream pointed to by stream,  returning
       nonzero if it is set.  The error indicator can be reset only by the clearerr() function.

       The function fileno() examines the argument stream and returns the integer file descriptor used
       to implement this stream.  The file descriptor is still owned by stream and will be closed when
       fclose(3)  is called.  Duplicate the file descriptor with dup(2) before passing it to code that
       might close it.

       For nonlocking counterparts, see unlocked_stdio(3).

ERRORS
       These functions should not fail and do not set the external variable errno.  (However, in  case
       fileno()  detects  that  its argument is not a valid stream, it must return -1 and set errno to
       EBADF.)
*/
void clearerr(FILE *stream);
/**/
int feof(FILE *stream);
/**/
int ferror(FILE *stream);
/**/
int fileno(FILE *stream);

#endif