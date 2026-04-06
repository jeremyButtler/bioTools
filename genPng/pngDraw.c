/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' pngDraw SOF: Start Of File
'   - has functions to draw lines, pixels, or object on
'     a png
'   o header:
'     - included libraries
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
|   - included libraries
\-------------------------------------------------------*/

#ifdef PLAN9
   #include <u.h>
#endif

#include "pngDraw.h"
#include "../genLib/endin.h"
#include "mkPng.h"

/*.h files only*/
#include "../genLib/64bit.h"

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\
! Hidden Libraries:
!   - std #include <stdlib.h> or for plan9 <libc.h>
!   - std #include <stdio.h.h>
!   - .c  #include "../genLib/checkSum.h"
\%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

/*-------------------------------------------------------\
| Fun01: addPixel_pngDraw
|   - adds a single pixel to a png (in st_mkPng struct)
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
){
   if(xSL > (signed long) pngSTPtr->widthUS)
      goto overflow_fun01;
   if(ySL > (signed long) pngSTPtr->heightUS)
      goto overflow_fun01;

   ySL *= pngSTPtr->widthUS;
   ySL += xSL;
   pngSTPtr->pixelAryUC[ySL] = colUC;

   return 0;

   overflow_fun01:;
      return def_overflow_pngDraw;
}  /*addPixel_pngDraw*/

#ifndef ULONG_MODE
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
|   - widthSL:
|     o width in pixels of bar
|   - heightSL:
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
){
   signed long yPosSL = 0;
   signed long byteSL = 0;
   signed long xPosSL = 0;

   if(widthSL + xSL >= (signed int) pngSTPtr->widthUS)
      goto overflow_fun02;
   if(heightSL + ySL >= (signed int) pngSTPtr->heightUS)
      goto overflow_fun02;

   byteSL = (pngSTPtr->widthUS * ySL) + xSL;

   for(yPosSL = ySL; yPosSL < ySL + heightSL; ++yPosSL)
   { /*Loop: draw a line for each row (the bar)*/
      for(xPosSL = 0; xPosSL < widthSL; ++xPosSL)
         pngSTPtr->pixelAryUC[byteSL++] = colUC;
      byteSL += pngSTPtr->widthUS - xPosSL;
   } /*Loop: draw a line for each row (the bar)*/

   return 0;

   overflow_fun02:;
      return def_overflow_pngDraw;
} /*addBar_pngDraw*/

/*I never got this to work correctly, but was supposed to
`  use longs like SIMD to save time
*/
#else
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
|   - widthSL:
|     o width in pixels of bar
|   - heightSL:
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
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun02 addBar_pngDraw
   '   - adds a bar to a st_mkPng image
   '   o fun02 sec01:
   '     - variable declarations
   '   o fun02 sec02:
   '     - setup + create color stamp
   '   o fun02 sec03:
   '     - make bar
   '   o fun02 sec04:
   '     - return result
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun02 Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   ul_64bit maskStartUL = 0;/*mask to clear start*/
   ul_64bit colStartUL = 0; /*color stamp for mask start*/

   ul_64bit maskEndUL = 0;  /*ending mask to clear bits*/
   ul_64bit colEndUL = 0;   /*color stamp for mask end*/

   ul_64bit colByteUL = 0;  /*byte full of color*/
   ul_64bit *pixAryUL = (ul_64bit *) pngSTPtr->pixelAryUC;
     /*pixel array as long*/

   signed long startSL = 0;    /*start of bar on row*/
   signed long endSL = 0;      /*start of bar on row*/
   signed long cpIndexSL = 0;  /*index copying at*/
   signed long lenRowSL = 0;   /*length of one row*/


   unsigned char bitUC = 0;    /*for building mask*/
   unsigned char *swapStartUCPtr = 0;
   unsigned char *swapEndUCPtr = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun02 Sec02:
   ^   - setup + create color stamp
   ^   o fun02 sec02 sub01:
   ^     - check if have overflow and find row length
   ^   o fun02 sec02 sub02:
   ^     - find start, end, and setup masks
   ^   o fun02 sec02 sub03:
   ^     - setup color stamps
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun02 Sec02 Sub01:
   *   - check if have overflow and find row length
   \*****************************************************/

   if(widthSL + xSL >= (signed int) pngSTPtr->widthUS)
      goto overflow_fun02;
   if(heightSL + ySL >= (signed int) pngSTPtr->heightUS)
      goto overflow_fun02;

   /*this is safe because setup enforces that width must
   `   be a multiple of bits per byte
   */
   lenRowSL = bytesToNumUL_64bit(pngSTPtr->widthUS) - 1;
      /*get number of longs in a row*/

   /*****************************************************\
   * Fun02 Sec02 Sub02:
   *   - find start, end, and setup masks
   \*****************************************************/

   startSL = xSL;
   endSL = startSL + widthSL - 1;

   /*_________________build_start_mask__________________*/
   maskStartUL = xSL & def_modUL_64bit;
      /*find number of pixels not changing at start*/

   if((unsigned long) widthSL < maskStartUL)
   { /*If: the starting long can hold all bits*/
      maskStartUL = widthSL;
      if(maskStartUL)
         maskStartUL = ((ul_64bit) -1)>>(maskStartUL <<3);
      maskStartUL = ulToLittle_endin(maskStartUL);

      startSL = byteLenToFullULLen_64bit(startSL);
      endSL = startSL;
      goto moveToFirstPixel_fun02_sec02_sub02;
   } /*If: the starting long can hold all bits*/

   if(maskStartUL)
      maskStartUL = ((ul_64bit) -1) >> (maskStartUL << 3);
      /*shifts out the bytes to change, sets bytes not to
      `   change to 0xff and bytes to change to 0x00
      */
      /*I need the if to handel the 0 case always messing
      `  up
      */
   maskStartUL = ulToLittle_endin(maskStartUL);
      /*make sure is in little endin format*/

   /*_________________build_end_mask____________________*/
   maskEndUL =
      ((xSL + widthSL) % def_bytesInUL_64bit);
      /*gives number of extra pixels at end*/
   if(maskEndUL)
      maskEndUL = ((ul_64bit) -1) >> (maskEndUL << 3);
      /*shifts out the bytes to change, setting sets bytes
      `  not to change to 0xff and bytes to change to 0x00
      */
      /*I need the if to handel the 0 case always messing
      `  up
      */
   maskEndUL = ulToLittle_endin(maskEndUL);
      /*make sure is in little endin format*/

   /*_________________find_positions____________________*/
   /*convert start and end to bytes*/
   startSL = byteLenToFullULLen_64bit(startSL);
   endSL = bytesToNumUL_64bit(endSL) - 1;

   /*move to first pixel to change position*/
   moveToFirstPixel_fun02_sec02_sub02:;
      pixAryUL += ySL * lenRowSL;

   /*****************************************************\
   * Fun02 Sec02 Sub03:
   *   - setup color stamps
   \*****************************************************/

   for(
      bitUC = 0;
      bitUC < def_bitsInUL_64bit;
      bitUC += def_bitsPerChar_64bit
   ){ /*Loop: build color long*/
      colByteUL |= (((ul_64bit) colUC) << bitUC);
   }  /*Loop: build color long*/

   swapStartUCPtr = (unsigned char *) &maskStartUL;
   swapEndUCPtr = (unsigned char *) &maskEndUL;

   for(bitUC=0; bitUC < def_bytesInUL_64bit>>1;++bitUC)
   { /*Loop: invert masks*/
      swapStartUCPtr[bitUC] ^=
         swapStartUCPtr[def_bytesInUL_64bit - bitUC-1];
      swapStartUCPtr[def_bytesInUL_64bit - bitUC-1] ^=
         swapStartUCPtr[bitUC];
      swapStartUCPtr[bitUC] ^=
         swapStartUCPtr[def_bytesInUL_64bit - bitUC-1];

      swapEndUCPtr[bitUC] ^=
         swapEndUCPtr[def_bytesInUL_64bit - bitUC - 1];
      swapEndUCPtr[def_bytesInUL_64bit - bitUC - 1] ^=
         swapEndUCPtr[bitUC];
      swapEndUCPtr[bitUC] ^=
         swapEndUCPtr[def_bytesInUL_64bit - bitUC - 1];
   } /*Loop: invert masks*/

   colStartUL = colByteUL & ~maskStartUL;
   colEndUL = colByteUL & ~maskEndUL;
   maskStartUL = maskStartUL;
   maskEndUL = maskEndUL;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun02 Sec03:
   ^   - make bar
   ^   o fun02 sec03 sub01:
   ^     - one limb coloring
   ^   o fun02 sec03 sub02:
   ^     - two limb coloring
   ^   o fun02 sec03 sub03:
   ^     - three or more limb coloring
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
	* Fun02 Sec03 Sub01:
   *   - one limb coloring
   \*****************************************************/

   if(startSL == endSL)
   { /*If: only coloring one limb*/

      for(xSL = ySL; xSL < heightSL + ySL; ++xSL)
      { /*Loop: apply color*/
         pixAryUL[startSL] &= maskStartUL;
         pixAryUL[startSL] |= colStartUL;

         pixAryUL += lenRowSL;
      }  /*Loop: apply color*/
   } /*If: only coloring one limb*/

   /*****************************************************\
	* Fun02 Sec03 Sub02:
   *   - two limb coloring
   \*****************************************************/

   else if((startSL + 1) == endSL)
   { /*Else If: only coloring two limbs*/

      for(xSL = ySL; xSL < heightSL + ySL; ++xSL)
      { /*Loop: apply color*/
         pixAryUL[startSL] &= maskStartUL;
         pixAryUL[startSL] |= colStartUL;

         pixAryUL[endSL] &= maskEndUL;
         pixAryUL[endSL] |= colEndUL;

         pixAryUL += lenRowSL;
      }  /*Loop: apply color*/
   } /*Else If: only coloring two limbs*/

   /*****************************************************\
   * Fun02 Sec03 Sub03:
   *   - three or more limb coloring
   \*****************************************************/

   else
   { /*Else If: coloring three or more limbs*/

      for(xSL = ySL; xSL < heightSL + ySL; ++xSL)
      { /*Loop: apply color*/
         pixAryUL[startSL] &= maskStartUL;
         pixAryUL[startSL] |= colStartUL;

         cpIndexSL = startSL;
         while(cpIndexSL < endSL)
            pixAryUL[cpIndexSL++] = colByteUL;

         pixAryUL[endSL] &= maskEndUL;
         pixAryUL[endSL] |= colEndUL;

         pixAryUL += lenRowSL;
      }  /*Loop: apply color*/
   } /*Else If: coloring three or more  limbs*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun02 Sec04:
   ^   - return result
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   return 0;

   overflow_fun02:;
      return def_overflow_pngDraw;
}  /*addBar_pngDraw*/
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
