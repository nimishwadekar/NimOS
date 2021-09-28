#ifndef _STDLIB_H
#define _STDLIB_H 1

#include <stddef.h>

// The largest number rand will return (same as INT_MAX).
#define RAND_MAX    2147483647

#define	EXIT_FAILURE	1	// Failing exit status.
#define	EXIT_SUCCESS	0	// Successful exit status.

typedef struct
{
    int quot, rem;
} div_t;

typedef struct
{
    long quot, rem;
} ldiv_t;

typedef struct
{
    long long quot, rem;
} lldiv_t;


/*
DESCRIPTION
       The  atof()  function  converts the initial portion of the string
       pointed to by nptr to double.  The behavior is the same as

           strtod(nptr, NULL);

       except that atof() does not detect errors.

RETURN VALUE
       The converted value.
*/
double atof(const char *nptr);


/*
DESCRIPTION
       The  atoi()  function  converts the initial portion of the string
       pointed to by nptr to int.  The behavior is the same as

           strtol(nptr, NULL, 10);

       except that atoi() does not detect errors.

RETURN VALUE
       The converted value.
*/
int atoi(const char *nptr);
/**/
long atol(const char *nptr);
/**/
long long atoll(const char *nptr);


/*
       Converts integer to string.
*/
char *itoa(int value, char *str, int base);
/**/
char *ltoa(long int value, char *str, int base);
/**/
char *lltoa(long long int value, char *str, int base);


/*
DESCRIPTION
       The  strtod(),  strtof(),  and  strtold()  functions  convert the initial portion of the string
       pointed to by nptr to double, float, and long double representation, respectively.

       The expected form of the (initial portion of the) string is optional  leading  white  space  as
       recognized by isspace(3), an optional plus ('+') or minus sign ('-') and then either (i) a dec‐
       imal number, or (ii) a hexadecimal number, or (iii) an infinity, or (iv) a NAN (not-a-number).

       A decimal number consists of a nonempty sequence of decimal digits possibly containing a  radix
       character  (decimal point, locale-dependent, usually '.'), optionally followed by a decimal ex‐
       ponent.  A decimal exponent consists of an 'E' or 'e', followed by an optional  plus  or  minus
       sign,  followed  by  a  nonempty  sequence of decimal digits, and indicates multiplication by a
       power of 10.

       A hexadecimal number consists of a "0x" or "0X" followed by a nonempty sequence of  hexadecimal
       digits  possibly containing a radix character, optionally followed by a binary exponent.  A bi‐
       nary exponent consists of a 'P' or 'p', followed by an optional plus or minus sign, followed by
       a  nonempty sequence of decimal digits, and indicates multiplication by a power of 2.  At least
       one of radix character and binary exponent must be present.

       An infinity is either "INF" or "INFINITY", disregarding case.

       A NAN is "NAN" (disregarding case) optionally followed by a string, (n-char-sequence), where n-
       char-sequence specifies in an implementation-dependent way the type of NAN (see NOTES).

RETURN VALUE
       These functions return the converted value, if any.

       If  endptr is not NULL, a pointer to the character after the last character used in the conver‐
       sion is stored in the location referenced by endptr.

       If no conversion is performed, zero is returned and (unless endptr is null) the value  of  nptr
       is stored in the location referenced by endptr.

       If the correct value would cause overflow, plus or minus HUGE_VAL (HUGE_VALF, HUGE_VALL) is re‐
       turned (according to the sign of the value), and ERANGE is stored in  errno.   If  the  correct
       value would cause underflow, zero is returned and ERANGE is stored in errno.

ERRORS
       ERANGE Overflow or underflow occurred.
*/
double strtod(const char *nptr, char **endptr);
/**/
float strtof(const char *nptr, char **endptr);
/**/
long double strtold(const char *nptr, char **endptr);


/*
DESCRIPTION
       The  strtol()  function converts the initial part of the string in nptr to a long integer value
       according to the given base, which must be between 2 and 36 inclusive, or be the special  value
       0.

       The string may begin with an arbitrary amount of white space (as determined by isspace(3)) fol‐
       lowed by a single optional '+' or '-' sign.  If base is zero or 16, the string may then include
       a  "0x" or "0X" prefix, and the number will be read in base 16; otherwise, a zero base is taken
       as 10 (decimal) unless the next character is '0', in which case it is taken as 8 (octal).

       The remainder of the string is converted to a long int value in the obvious manner, stopping at
       the first character which is not a valid digit in the given base.  (In bases above 10, the let‐
       ter 'A' in either uppercase or lowercase represents 10, 'B' represents 11, and so  forth,  with
       'Z' representing 35.)

       If  endptr  is not NULL, strtol() stores the address of the first invalid character in *endptr.
       If there were no digits at all, strtol() stores the original value of nptr in *endptr (and  re‐
       turns  0).   In  particular,  if  *nptr  is not '\0' but **endptr is '\0' on return, the entire
       string is valid.

       The strtoll() function works just like the strtol() function but returns a  long  long  integer
       value.

RETURN VALUE
       The strtol() function returns the result of the conversion, unless the value would underflow or
       overflow.  If an underflow occurs, strtol() returns LONG_MIN.  If an overflow occurs,  strtol()
       returns  LONG_MAX.   In  both cases, errno is set to ERANGE.  Precisely the same holds for str‐
       toll() (with LLONG_MIN and LLONG_MAX instead of LONG_MIN and LONG_MAX).

ERRORS
       EINVAL (not in C99) The given base contains an unsupported value.

       ERANGE The resulting value was out of range.

       The implementation may also set errno to EINVAL in case no conversion was performed (no  digits
       seen, and 0 returned).
*/
long int strtol(const char *nptr, char **endptr, int base);
/**/
long long int strtoll(const char *nptr, char **endptr, int base);


/*
DESCRIPTION
       The  strtoul() function converts the initial part of the string in nptr to an unsigned long int
       value according to the given base, which must be between 2 and 36 inclusive, or be the  special
       value 0.

       The string may begin with an arbitrary amount of white space (as determined by isspace(3)) fol‐
       lowed by a single optional '+' or '-' sign.  If base is zero or 16, the string may then include
       a  "0x"  prefix,  and the number will be read in base 16; otherwise, a zero base is taken as 10
       (decimal) unless the next character is '0', in which case it is taken as 8 (octal).

       The remainder of the string is converted to an unsigned long int value in the  obvious  manner,
       stopping  at the first character which is not a valid digit in the given base.  (In bases above
       10, the letter 'A' in either uppercase or lowercase represents 10, 'B' represents  11,  and  so
       forth, with 'Z' representing 35.)

       If  endptr is not NULL, strtoul() stores the address of the first invalid character in *endptr.
       If there were no digits at all, strtoul() stores the original value of nptr in *endptr (and re‐
       turns  0).   In  particular,  if  *nptr  is not '\0' but **endptr is '\0' on return, the entire
       string is valid.

       The strtoull() function works just like the strtoul() function but  returns  an  unsigned  long
       long int value.

RETURN VALUE
       The  strtoul()  function returns either the result of the conversion or, if there was a leading
       minus sign, the negation of the result of the conversion represented as an unsigned value,  un‐
       less  the  original  (nonnegated)  value  would overflow; in the latter case, strtoul() returns
       ULONG_MAX and sets errno to ERANGE.  Precisely the same holds for strtoull()  (with  ULLONG_MAX
       instead of ULONG_MAX).

ERRORS
       EINVAL (not in C99) The given base contains an unsupported value.

       ERANGE The resulting value was out of range.

       The  implementation may also set errno to EINVAL in case no conversion was performed (no digits
       seen, and 0 returned).
*/
unsigned long int strtoul(const char *nptr, char **endptr, int base);
/**/
unsigned long long int strtoull(const char *nptr, char **endptr, int base);


/*
DESCRIPTION
       These  functions  convert  a floating-point value, fp, into a string of characters, str, with a
       configurable format string.  At most n characters are stored into str.

       The terminating null character is written if and only if n is sufficiently large, other‐
       wise the written string is truncated at n characters.

       The strfromd(), strfromf(), and strfroml() functions are equivalent to

           snprintf(str, n, format, fp);

       except for the format string.

   Format of the format string
       The format string must start with the character '%'.  This is followed by an optional precision
       which starts with the period character (.), followed by an optional decimal integer.  If no in‐
       teger  is specified after the period character, a precision of zero is used.  Finally, the for‐
       mat string should have one of the conversion specifiers a, A, e, E, f, F, g, or G.

       The conversion specifier is applied based on the floating-point type indicated by the  function
       suffix.   Therefore, unlike snprintf(), the format string does not have a length modifier char‐
       acter.  See snprintf(3) for a detailed description of these conversion specifiers.

       The implementation conforms to the C99 standard on conversion of NaN and infinity values:

              If fp is a NaN, +NaN, or -NaN, and f (or a, e, g) is the conversion specifier, the  con‐
              version  is  to "nan", "nan", or "-nan", respectively.  If F (or A, E, G) is the conver‐
              sion specifier, the conversion is to "NAN" or "-NAN".

              Likewise if fp is infinity, it is converted to [-]inf or [-]INF.

       A malformed format string results in undefined behavior.

RETURN VALUE
       The strfromd(), strfromf(), and strfroml() functions return the number of characters that would
       have  been  written  in str if n had enough space, not counting the terminating null character.
       Thus, a return value of n or greater means that the output was truncated.
*/
int strfromd(char *restrict str, size_t n, const char *restrict format, double fp);
/**/
int strfromf(char *restrict str, size_t n, const char *restrict format, float fp);
/**/
int strfroml(char *restrict str, size_t n, const char *restrict format, long double fp);


/*
DESCRIPTION
       The rand() function returns a pseudo-random integer in the range 0 to RAND_MAX inclusive (i.e.,
       the mathematical range [0, RAND_MAX]).

       The srand() function sets its argument as the seed for a new sequence of pseudo-random integers
       to be returned by rand().  These sequences are repeatable by calling srand() with the same seed
       value.

       If no seed value is provided, the rand() function is automatically seeded with a value of 1.

       The function rand() is not reentrant, since it uses hidden state that is modified on each call.
       This  might  just  be the seed value to be used by the next call, or it might be something more
       elaborate.  In order to get reproducible behavior in a threaded application, this state must be
       made explicit; this can be done using the reentrant function rand_r().

       Like  rand(),  rand_r()  returns a pseudo-random integer in the range [0, RAND_MAX].  The seedp
       argument is a pointer to an unsigned int that  is  used  to  store  state  between  calls.   If
       rand_r()  is  called  with the same initial value for the integer pointed to by seedp, and that
       value is not modified between calls, then the same pseudo-random sequence will result.

       The value pointed to by the seedp argument of rand_r() provides only a  very  small  amount  of
       state, so this function will be a weak pseudo-random generator.  Try drand48_r(3) instead.

RETURN VALUE
       The  rand()  and  rand_r()  functions  return  a value between 0 and RAND_MAX (inclusive).  The
       srand() function returns no value.
*/
int rand(void);
/**/
void srand(unsigned int seed);


/*
DESCRIPTION
       The  malloc() function allocates size bytes and returns a pointer to the allocated memory.  The
       memory is not initialized.  If size is 0, then  malloc()  returns  either  NULL,  or  a  unique
       pointer value that can later be successfully passed to free().

       The  free() function frees the memory space pointed to by ptr, which must have been returned by
       a previous call to malloc(), calloc(), or realloc().  Otherwise, or if  free(ptr)  has  already
       been called before, undefined behavior occurs.  If ptr is NULL, no operation is performed.

       The  calloc()  function  allocates memory for an array of nmemb elements of size bytes each and
       returns a pointer to the allocated memory.  The memory is set to zero.  If nmemb or size is  0,
       then  calloc()  returns  either  NULL, or a unique pointer value that can later be successfully
       passed to free().  If the multiplication of nmemb and size would result  in  integer  overflow,
       then  calloc() returns an error.  By contrast, an integer overflow would not be detected in the
       following call to malloc(), with the result that an incorrectly sized block of memory would  be
       allocated:

           malloc(nmemb * size);

       The  realloc()  function  changes the size of the memory block pointed to by ptr to size bytes.
       The contents will be unchanged in the range from the start of the region up to the  minimum  of
       the  old and new sizes.  If the new size is larger than the old size, the added memory will not
       be initialized.  If ptr is NULL, then the call is equivalent to malloc(size), for all values of
       size;  if size is equal to zero, and ptr is not NULL, then the call is equivalent to free(ptr).
       Unless ptr is NULL, it must have been returned by an earlier call to malloc(), calloc(), or re‐
       alloc().  If the area pointed to was moved, a free(ptr) is done.

       The  reallocarray() function changes the size of the memory block pointed to by ptr to be large
       enough for an array of nmemb elements, each of which is size bytes.  It is  equivalent  to  the
       call

               realloc(ptr, nmemb * size);

       However,  unlike  that realloc() call, reallocarray() fails safely in the case where the multi‐
       plication would overflow.  If such an overflow occurs, reallocarray() returns NULL, sets  errno
       to ENOMEM, and leaves the original block of memory unchanged.

RETURN VALUE
       The malloc() and calloc() functions return a pointer to the allocated memory, which is suitably
       aligned for any built-in type.  On error, these functions return NULL.  NULL may  also  be  re‐
       turned  by  a  successful call to malloc() with a size of zero, or by a successful call to cal‐
       loc() with nmemb or size equal to zero.

       The free() function returns no value.

       The realloc() function returns a pointer to the  newly  allocated  memory,  which  is  suitably
       aligned  for any built-in type, or NULL if the request failed.  The returned pointer may be the
       same as ptr if the allocation was not moved (e.g., there was room to expand the allocation  in-
       place),  or different from ptr if the allocation was moved to a new address.  If size was equal
       to 0, either NULL or a pointer suitable to be passed  to  free()  is  returned.   If  realloc()
       fails, the original block is left untouched; it is not freed or moved.

       On  success,  the  reallocarray() function returns a pointer to the newly allocated memory.  On
       failure, it returns NULL and the original block of memory is left untouched.

ERRORS
       calloc(), malloc(), realloc(), and reallocarray() can fail with the following error:

       ENOMEM Out of memory.  Possibly, the application hit the RLIMIT_AS  or  RLIMIT_DATA  limit  de‐
              scribed in getrlimit(2).
*/
void *malloc(size_t size);
/**/
void free(void *ptr);
/**/
void *calloc(size_t nmemb, size_t size);
/**/
void *realloc(void *ptr, size_t size);
/**/
void *reallocarray(void *ptr, size_t nmemb, size_t size);


/*
DESCRIPTION
       The  abs() function computes the absolute value of the integer argument j.  The labs(), llabs()
       and imaxabs() functions compute the absolute value of the argument j of the appropriate integer
       type for the function.

RETURN VALUE
       Returns  the  absolute  value  of the integer argument, of the appropriate integer type for the
       function.
*/
int abs(int j);
/**/
long int labs(long int j);
/**/
long long int llabs(long long int j);


/*
DESCRIPTION
       The  div()  function  computes the value numerator/denominator and returns the quotient and re‐
       mainder in a structure named div_t that contains two integer  members  (in  unspecified  order)
       named  quot and rem.  The quotient is rounded toward zero.  The result satisfies quot*denomina‐
       tor+rem = numerator.

       The ldiv(), lldiv(), and imaxdiv() functions do the same, dividing  numbers  of  the  indicated
       type  and  returning  the result in a structure of the indicated name, in all cases with fields
       quot and rem of the same type as the function arguments.

RETURN VALUE
       The div_t (etc.) structure.
*/
div_t div(int numerator, int denominator);
/**/
ldiv_t ldiv(long numerator, long denominator);
/**/
lldiv_t lldiv(long long numerator, long long denominator);

#endif