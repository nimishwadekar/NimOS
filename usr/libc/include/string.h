#ifndef _STRING_H
#define _STRING_H

#include <stddef.h>

/*
DESCRIPTION
       The  memcpy() function copies n bytes from memory area src to
       memory area dest.  The memory areas must  not  overlap.   Use
       memmove(3) if the memory areas do overlap.

RETURN VALUE
       The memcpy() function returns a pointer to dest.
*/
void *memcpy(void *dest, const void *src, size_t n);


/*
DESCRIPTION
       The  memmove()  function  copies  n bytes from memory area src to
       memory area dest.  The memory areas may  overlap:  copying  takes
       place  as  though the bytes in src are first copied into a tempo‐
       rary array that does not overlap src or dest, and the  bytes  are
       then copied from the temporary array to dest.

RETURN VALUE
       The memmove() function returns a pointer to dest.
*/
void *memmove(void *dest, const void *src, size_t n);


/*
DESCRIPTION
       The  memccpy() function copies no more than n bytes from mem‐
       ory area src to memory area dest, stopping when the character
       c is found.

       If the memory areas overlap, the results are undefined.

RETURN VALUE
       The  memccpy() function returns a pointer to the next charac‐
       ter in dest after c, or NULL if c was not found in the  first
       n characters of src.
*/
void *memccpy(void *dest, const void *src, int c, size_t n);

/*
DESCRIPTION
       The  memset() function fills the first n bytes of the memory area
       pointed to by s with the constant byte c.

RETURN VALUE
       The memset() function returns a pointer to the memory area s.
*/
void *memset(void *s, int c, size_t n);


/*
DESCRIPTION
       The  memcmp()  function  compares  the first n bytes (each inter‐
       preted as unsigned char) of the memory areas s1 and s2.

RETURN VALUE
       The memcmp() function returns an integer less than, equal to,  or
       greater  than  zero  if the first n bytes of s1 is found, respec‐
       tively, to be less than, to match, or be greater than the first n
       bytes of s2.

       For a nonzero return value, the sign is determined by the sign of
       the difference between the first pair of  bytes  (interpreted  as
       unsigned char) that differ in s1 and s2.

       If n is zero, the return value is zero.
*/
int memcmp(const void *s1, const void *s2, size_t n);


/*
DESCRIPTION
       The memchr() function scans the initial n  bytes  of  the  memory
       area pointed to by s for the first instance of c.  Both c and the
       bytes of the memory area pointed to by s are interpreted  as  un‐
       signed char.

RETURN VALUE
       The memchr() function  returns  a  pointer  to  the
       matching  byte  or  NULL  if  the character does not occur in the
       given memory area.
*/
void *memchr(const void *s, int c, size_t n);


/*
DESCRIPTION
       The memrchr() function is like the memchr() function, except that
       it searches backward from the end of the n bytes pointed to by  s
       instead of forward from the beginning.

RETURN VALUE
       The memrchr() function  returns  a  pointer  to  the
       matching  byte  or  NULL  if  the character does not occur in the
       given memory area.
*/
void *memrchr(const void *s, int c, size_t n);


/*
DESCRIPTION
       The  rawmemchr()  function  is  similar  to  memchr(): it assumes
       (i.e., the programmer knows for certain) that an  instance  of  c
       lies  somewhere  in  the  memory  area  starting  at the location
       pointed to by s, and so performs an optimized search for c (i.e.,
       no use of a count argument to limit the range of the search).  If
       an instance of c is not found,  the  results  are  unpredictable.

RETURN VALUE
       The rawmemchr() function returns a pointer to the matching  byte,
       if one is found.  If no matching byte is found, the result is un‐
       specified.
*/
void *rawmemchr(const void *s, int c);


/*
DESCRIPTION
       The  strcpy()  function  copies the string pointed to by src, in‐
       cluding the terminating null byte, to the  buffer  pointed
       to  by  dest.   The  strings may not overlap, and the destination
       string dest must be large enough to receive the copy.  Beware  of
       buffer overruns!

RETURN VALUE
       The strcpy() function returns a pointer to the des‐
       tination string dest.
*/
char *strcpy(char *dest, const char *src);


/*
DESCRIPTION
       The  strcat() function appends the src string to the dest string,
       overwriting the terminating null byte at the end of  dest,
       and then adds a terminating null byte.  The strings may not over‐
       lap, and the dest string must have enough space for  the  result.
       If  dest  is not large enough, program behavior is unpredictable;
       buffer overruns are a favorite avenue for attacking  secure  pro‐
       grams.

RETURN VALUE
       The  strcat() function returns a pointer to the re‐
       sulting string dest.
*/
char *strcat(char *dest, const char *src);


/*
DESCRIPTION
       The  strcmp()  function  compares the two strings s1 and s2.  The
       locale is not taken into account (for a locale-aware  comparison,
       see  strcoll(3)).   It returns an integer less than, equal to, or
       greater than zero if s1 is found, respectively, to be less  than,
       to match, or be greater than s2.

RETURN VALUE
       The strcmp() function returns an integer less than,
       equal  to,  or  greater  than  zero  if  s1 (or the first n bytes
       thereof) is found, respectively, to be less than, to match, or be
       greater than s2.
*/
int strcmp(const char *s1, const char *s2);


/*
DESCRIPTION
       The  strdup() function returns a pointer to a new string which is
       a duplicate of the string s.  Memory for the new  string  is  ob‐
       tained with malloc(3), and can be freed with free(3).

RETURN VALUE
       On success, the strdup() function returns a pointer to the dupli‐
       cated  string.  It returns NULL if insufficient memory was avail‐
       able, with errno set to indicate the cause of the error.
*/
char *strdup(const char *s);


/*
DESCRIPTION
       The  strchr()  function returns a pointer to the first occurrence
       of the character c in the string s.

       Here  "character"  means "byte"; these functions do not work with
       wide or multibyte characters.

RETURN VALUE
       The strchr() and strrchr() functions  return  a  pointer  to  the
       matched  character  or  NULL  if the character is not found.  The
       terminating null byte is considered part of the string,  so  that
       if  c  is  specified as 0, these functions return a pointer to
       the terminator.
*/
char *strchr(const char *s, int c);


/*
DESCRIPTION
       The strrchr() function returns a pointer to the  last  occurrence
       of the character c in the string s.

       Here  "character"  means "byte"; these functions do not work with
       wide or multibyte characters.

RETURN VALUE
       The strchr() and strrchr() functions  return  a  pointer  to  the
       matched  character  or  NULL  if the character is not found.  The
       terminating null byte is considered part of the string,  so  that
       if  c  is  specified as 0, these functions return a pointer to
       the terminator.
*/
char *strrchr(const char *s, int c);


/*
DESCRIPTION
       The strstr() function finds the first occurrence of the substring
       needle in the string haystack.  The terminating null bytes
       are not compared.

RETURN VALUE
       These functions return a pointer to the beginning of the  located
       substring, or NULL if the substring is not found.
*/
char *strstr(const char *haystack, const char *needle);


/*
DESCRIPTION
       The strtok() function breaks a string into a sequence of zero  or
       more  nonempty tokens.  On the first call to strtok(), the string
       to be parsed should be specified in str.  In each subsequent call
       that should parse the same string, str must be NULL.

       The  delim argument specifies a set of bytes that delimit the to‐
       kens in the parsed string.   The  caller  may  specify  different
       strings in delim in successive calls that parse the same string.

       Each  call  to  strtok()  returns  a pointer to a null-terminated
       string containing the next token.  This string does  not  include
       the  delimiting  byte.  If no more tokens are found, strtok() re‐
       turns NULL.

       A sequence of calls to strtok() that operate on the  same  string
       maintains a pointer that determines the point from which to start
       searching for the next token.  The first call  to  strtok()  sets
       this pointer to point to the first byte of the string.  The start
       of the next token is determined by scanning forward for the  next
       nondelimiter  byte  in str.  If such a byte is found, it is taken
       as the start of the next token.  If no such byte is  found,  then
       there  are  no more tokens, and strtok() returns NULL.  (A string
       that is empty or that contains only delimiters  will  thus  cause
       strtok() to return NULL on the first call.)

       The  end  of each token is found by scanning forward until either
       the next delimiter byte is found or until  the  terminating  null
       byte  is encountered.  If a delimiter byte is found, it is
       overwritten with a null byte to terminate the current token,  and
       strtok() saves a pointer to the following byte; that pointer will
       be used as the starting point when searching for the next  token.
       In  this  case,  strtok()  returns  a pointer to the start of the
       found token.

       From the above description, it follows that a sequence of two  or
       more  contiguous  delimiter bytes in the parsed string is consid‐
       ered to be a single delimiter, and that delimiter  bytes  at  the
       start or end of the string are ignored.  Put another way: the to‐
       kens returned by strtok() are always nonempty strings.  Thus, for
       example,  given  the string "aaa;;bbb,", successive calls to str‐
       tok() that specify the delimiter string  ";,"  would  return  the
       strings "aaa" and "bbb", and then a null pointer.

RETURN VALUE
       The  strtok()  and  strtok_r()  functions return a pointer to the
       next token, or NULL if there are no more tokens.
*/
char *strtok(char *str, const char *delim);


/*
DESCRIPTION
       The strlen() function calculates the length of the string pointed
       to by s, excluding the terminating null byte.

RETURN VALUE
       The strlen() function returns the number of bytes in  the  string
       pointed to by s.
*/
size_t strlen(const char *s);

#endif