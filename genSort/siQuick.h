/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' siQuick SOF: Start Of File
'   - holds quick select, quick sort, and quick heap
'     (not in yet) algorithms for signed ints
'   o header:
'     - guards for people who call .h files in .h files 
'   o fun01: select_siQuick
'     - quick select algorithm (partially sorts)
'   o fun02: sort_siQuick
'     - quick sort algorithm (sorts)
'   o license
'     - licensing for this code (public domain / mit)
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*-------------------------------------------------------\
| Header:
|   - guards for people who call .h files in .h files 
\-------------------------------------------------------*/

#ifndef SIGNED_INT_QUICK_SELECT_SORT_HEAP_H
#define SIGNED_INT_QUICK_SELECT_SORT_HEAP_H

/*-------------------------------------------------------\
| Fun01: select_siQuick
|   - quick select algorithm (partially sorts)
| Input:
|   - siAry:
|     o signed int array to find kth value for
|   - kthSI:
|     o index in array to find value for
|   - lenSI:
|     o number of items in the array (index 1)
| Output:
|   - Modifies:
|     o siAry to be partially sorted, with kthSI index to
|       have its value in the correct position
|   - Returns:
|     o value of the kthSI index item, or if kthSI is
|       longer then the length, then the closest item
\-------------------------------------------------------*/
signed int
select_siQuick(
   signed int *siAry,  /*array the get value from*/
   signed int kthSI,   /*index of value to find*/
   signed int lenSI    /*length of the array*/
);

/*-------------------------------------------------------\
| Fun02: sort_siQuick
|   - quick sort algorithm (sorts)
| Input:
|   - siAry:
|     o signed int array to sort
|   - lenSI:
|     o number of items in the array
| Output:
|   - Modifies:
|     o siAry to be sorted from least to greatest
\-------------------------------------------------------*/
void
sort_siQuick(
   signed int *siAry,  /*array the get value from*/
   signed int lenSI    /*last item to search*/
);

#endif

/*=======================================================\
: License:
: 
: This code is under the unlicense (public domain).
:   However, for cases were the public domain is not
:   suitable, such as countries that do not respect the
:   public domain or were working with the public domain
:   is inconveint / not possible, this code is under the
:   MIT license
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
: Copyright (c) 2026 jeremyButtler
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
