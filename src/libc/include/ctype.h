#ifndef _CTYPE_H
#define _CTYPE_H 1

/*
DESCRIPTION
       These  functions  check  whether c, which must have the value of an unsigned char or EOF, falls
       into a certain character class according to the specified locale.  The  functions  without  the
       "_l" suffix perform the check based on the current locale.

       The  functions  with the "_l" suffix perform the check based on the locale specified by the lo‐
       cale object locale.  The behavior of these functions is undefined if locale is the special  lo‐
       cale object LC_GLOBAL_LOCALE (see duplocale(3)) or is not a valid locale object handle.

       The  list  below explains the operation of the functions without the "_l" suffix; the functions
       with the "_l" suffix differ only in using the locale object locale instead of the  current  lo‐
       cale.

       isalnum()
              checks for an alphanumeric character; it is equivalent to (isalpha(c) || isdigit(c)).

       isalpha()
              checks  for  an  alphabetic  character;  in the standard "C" locale, it is equivalent to
              (isupper(c) || islower(c)).  In some locales, there may  be  additional  characters  for
              which isalpha() is true—letters which are neither uppercase nor lowercase.

       isascii()
              checks whether c is a 7-bit unsigned char value that fits into the ASCII character set.

       isblank()
              checks for a blank character; that is, a space or a tab.

       iscntrl()
              checks for a control character.

       isdigit()
              checks for a digit (0 through 9).

       isgraph()
              checks for any printable character except space.

       islower()
              checks for a lowercase character.

       isprint()
              checks for any printable character including space.

       ispunct()
              checks for any printable character which is not a space or an alphanumeric character.

       isspace()
              checks  for  white-space  characters.  In the "C" and "POSIX" locales, these are: space,
              form-feed ('\f'), newline ('\n'), carriage return ('\r'),  horizontal  tab  ('\t'),  and
              vertical tab ('\v').

       isupper()
              checks for an uppercase letter.

       isxdigit()
              checks for hexadecimal digits, that is, one of
              0 1 2 3 4 5 6 7 8 9 a b c d e f A B C D E F.

RETURN VALUE
       The  values  returned  are  nonzero if the character c falls into the tested class, and zero if
       not.
*/
int isalnum(int c);
int isalpha(int c);
int iscntrl(int c);
int isdigit(int c);
int isgraph(int c);
int islower(int c);
int isprint(int c);
int ispunct(int c);
int isspace(int c);
int isupper(int c);
int isxdigit(int c);

int isascii(int c);
int isblank(int c);

int tolower(int c);
int toupper(int c);
int toascii(int c);

#endif