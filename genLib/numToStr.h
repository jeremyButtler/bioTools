/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' numToStr SOF: Start Of File
'   - function(s) to convert a number to a string
'   o header:
'     - guards
'   o fun01: numToStr
'     - converts a number to a c-string
'   o fun02: backNumToStr
'     - converts a number to a c-string backwards
'   o license:
'     - licensing for this code (public domain / mit)
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*-------------------------------------------------------\
| Header:
|   - guards
\-------------------------------------------------------*/

#ifndef NUMBER_TO_CSTRING
#define NUMBER_TO_CSTRING

#define max_dblDecimal_numToStr 17
   /*maximum percsion of double*/

/*-------------------------------------------------------\
| Fun01: numToStr
|   - converts a number to a c-string
| Input:
|   - cstr:
|     o c-string to hold the converted number
|   - numUL:
|     o number to convert
| Output:
|   - Modifies:
|     o cstr to have the number (adds a '\0' at the end)
|   - Returns:
|     o number of characters copied to cstr
\-------------------------------------------------------*/
unsigned int
numToStr(
   signed char *cstr,
   unsigned long numUL
);

/*-------------------------------------------------------\
| Fun02: backNumToStr
|   - converts a number to a c-string backwards
| Input:
|   - cstr:
|     o c-string to hold the converted number
|   - numUL:
|     o number to convert
| Output:
|   - Modifies:
|     o cstr to have the backwards number (adds a '\0' at
|       the end)
|   - Returns:
|     o number of characters copied to cstr
\-------------------------------------------------------*/
unsigned int
backwards_numToStr(
   signed char *cstr,
   unsigned long numUL
);

/*-------------------------------------------------------\
| Fun03: double_numToStr
|   - converts a double to a c-string
| Input:
|   - cstr:
|     o c-string to hold the converted number
|   - numDbl:
|     o number to convert
|   - decUC:
|     o number decimal digits to keep
|       (max_dblDecimal_numToStr)
| Output:
|   - Modifies:
|     o cstr to have the number (adds a '\0' at the end)
|   - Returns:
|     o number of characters copied to cstr
\-------------------------------------------------------*/
unsigned int
double_numToStr(
   signed char *cstr,
   double numDbl,
   unsigned char decUC
);

#endif

/*=======================================================\
: License:
: 
: This code is under the unlicense (public domain).
:   However, for cases were the public domain is not
:   suitable, such as countries that do not respect the
:   public domain or were working with the public domain
:   is inconvient / not possible, this code is under the
:   MIT license.
: 
: Public domain:
: 
: This is free and unencumbered software released into the
:   public domain.
: 
: Anyone is free to copy, modify, publish, use, compile,
:   sell, or distribute this software, either in source
:   code form or as a compiled binary, for any purpose,
:   commercial or non-commercial, and by any means.
: 
: In jurisdictions that recognize copyright laws, the
:   author or authors of this software dedicate any and
:   all copyright interest in the software to the public
:   domain. We make this dedication for the benefit of the
:   public at large and to the detriment of our heirs and
:   successors. We intend this dedication to be an overt
:   act of relinquishment in perpetuity of all present and
:   future rights to this software under copyright law.
: 
: THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF
:   ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT
:   LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
:   FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO
:   EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM,
:   DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
:   CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR
:   IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
:   DEALINGS IN THE SOFTWARE.
: 
: For more information, please refer to
:   <https://unlicense.org>
: 
: MIT License:
: 
: Copyright (c) 2024 jeremyButtler
: 
: Permission is hereby granted, free of charge, to any
:   person obtaining a copy of this software and
:   associated documentation files (the "Software"), to
:   deal in the Software without restriction, including
:   without limitation the rights to use, copy, modify,
:   merge, publish, distribute, sublicense, and/or sell
:   copies of the Software, and to permit persons to whom
:   the Software is furnished to do so, subject to the
:   following conditions:
: 
: The above copyright notice and this permission notice
:   shall be included in all copies or substantial
:   portions of the Software.
: 
: THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF
:   ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT
:   LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
:   FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO
:   EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
:   FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN
:   AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
:   FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE
:   USE OR OTHER DEALINGS IN THE SOFTWARE.
\=======================================================*/
