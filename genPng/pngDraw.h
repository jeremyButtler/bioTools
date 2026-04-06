/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' pngDraw SOF: Start Of File
'   - has functions to draw lines, pixels, or object on
'     a png
'   o header:
'     - forward declarations and guards
'   o fun01: addPixel_pngDraw
'     - adds a single pixel to a png (in st_mkPng struct)
'   o fun02: addBar_pngDraw
'     - adds a bar (vertical or hoziontal line) to a png
'       (in st_mkPng struct)
'   o license:
'     - licensing for this code (public domain / mit)
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*-------------------------------------------------------\
| Header:
|   - forward declarations and guards
\-------------------------------------------------------*/

#ifndef PNG_DRAWING_FUNCTIONS_H
#define PNG_DRAWING_FUNCTIONS_H

struct st_mkPng;
#define def_overflow_pngDraw 2

/*-------------------------------------------------------\
| Fun01: addPixel_pngDraw
|   - adds a single pixel to a st_mkPng image
| Input:
|   - pngSTPtr:
|     o pointer to st_mkPng struct to add bar to
|   - xSL:
|     o x coordinate of pixel (index 0)
|   - ySL:
|     o y coordiante of pixel (index 0)
|   - colUC:
|     o color (0-4) to assign
| Output:
|   - Modifies:
|     o pixelAryUC in pngSTPtr to have pixel
|   - Returns:
|     o 0 if no errors
|     o def_overflow_pngDraw if coordinates are outside of
|       grpah range
\-------------------------------------------------------*/
signed char
addPixel_pngDraw(
   struct st_mkPng *pngSTPtr, /*add bar to png*/
   signed long xSL,           /*x coordinate (pixels)*/
   signed long ySL,           /*y coordiante (pixels)*/
   signed char colUC          /*color of bar*/
);

/*-------------------------------------------------------\
| Fun02: addBar_pngDraw
|   - adds a bar to a st_mkPng image
| Input:
|   - pngSTPtr:
|     o pointer to st_mkPng struct to add bar to
|   - xSL:
|     o x coordinate of left conner; start of bar; index 0
|   - ySL:
|     o y coordiante of bottom of bar (index 0)
|   - widthUS:
|     o width in pixels of bar
|   - heightUS:
|     o heigth in pixels of bar
|   - colUC:
|     o index of color in pallete to assign
| Output:
|   - Modifies:
|     o pixelAryUC in pngSTPtr to have bar
|   - Returns:
|     o 0 for no errors
|     o def_overflow_pngDraw if went out of bounds
| Note:
|   o the minimum width is at least one byte worth,
|     otherwise, do single pixel modifications
|     with addPixel_pngDraw
\-------------------------------------------------------*/
signed char
addBar_pngDraw(
   struct st_mkPng *pngSTPtr, /*add bar to png*/
   signed long xSL,           /*x coordinate (pixels)*/
   signed long ySL,           /*y coordiante (pixels)*/
   signed long widthSL,       /*pixels wide of bar*/
   signed long heightSL,      /*pixels high of bar*/
   signed char colUC          /*color of bar*/
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
