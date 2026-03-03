/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' siQuick SOF: Start Of File
'   - holds quick select, quick sort, and quick heap
'     (not in yet) algorithms for signed ints
'   o header:
'     - included libraries
'   o fun01: select_siQuick
'     - quick select algorithm (partially sorts)
'   o fun02: sort_siQuick
'     - quick sort algorithm (sorts)
'   o fun03: sortWithStack_siQuick
'     - quick sort but uses a stack instead of recursion
'   o license
'     - licensing for this code (public domain / mit)
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*-------------------------------------------------------\
| Header:
|   - included libraries
\-------------------------------------------------------*/

#include "siQuick.h"

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\
! Hidden libraries:
\%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

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
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun01 TOC:
   '   - quick select algorithm (partially sorts)
   '   o fun01 sec01:
   '     - variable declarations
   '   o fun01 sec02:
   '     - deal with cases were there are two few elements
   '   o fun01 sec03:
   '     - find the kth value
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun01 Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   signed int midSI = 0; /*gets the midpoint*/
   signed int startSI = 0;
   signed int posSI = 0;
   signed int endSI = lenSI - 1;
   signed int swapSI = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun01 Sec02:
   ^   - deal with cases were there are two few elements
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(kthSI >= endSI)
      kthSI = endSI;

   if(lenSI < 2)
   { /*If: so few items it is easy to find the kth item*/
      if(endSI <= 0)
         return siAry[startSI]; /*only one item*/

      else
      { /*Else If: two values*/
         swapSI = -(siAry[0] > siAry[1]);
         siAry[0] ^= (siAry[1] & swapSI);
         siAry[1] ^= (siAry[0] & swapSI);
         siAry[0] ^= (siAry[1] & swapSI);
           /*brachless if > swap; see the next section for
           `  how this works
           */

         return siAry[kthSI];
      } /*Else If: two values*/
   } /*If: so few items it is easy to find the kth item*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun01 Sec03:
   ^   - find the kth value
   ^   o fun01 sec03 sub01:
   ^     - find the median value using the three median
   ^       method + start loop
   ^   o fun01 sec03 sub02:
   ^     - partially sort the array to position the
   ^       choosen median value
   ^   o fun01 sec03 sub03:
   ^     - check if found the kth value
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun01 Sec03 Sub01:
   *   - find the median value using the three median
   *     method + start loop
   \*****************************************************/

   while(startSI < endSI - 1)
   { /*Loop: find kth value and partially sort*/
      midSI = (startSI + endSI) >> 1;

      /*_________find_the_midpoint_using_3_medians______*/
      /*the branchless swap is faster then the same
      `  branched swap
      */
      swapSI = -(siAry[startSI] > siAry[endSI]);
      siAry[startSI] ^= (siAry[endSI] & swapSI);
      siAry[endSI] ^= (siAry[startSI] & swapSI);
      siAry[startSI] ^= (siAry[endSI] & swapSI);

      swapSI = -(siAry[startSI] > siAry[midSI]);
      siAry[startSI] ^= (siAry[midSI] & swapSI);
      siAry[midSI] ^= (siAry[startSI] & swapSI);
      siAry[startSI] ^= (siAry[midSI] & swapSI);

      swapSI = -(siAry[midSI] > siAry[endSI]);
      siAry[midSI] ^= (siAry[endSI] & swapSI);
      siAry[endSI] ^= (siAry[midSI] & swapSI);
      siAry[midSI] ^= (siAry[endSI] & swapSI);
      /*Logic:
      `   - swapSI = -(siAry[x] > siAry[y])
      `     o becomes -(1) = -1 if siAry[x] > siAry[y]
      `       * all bits set to 1
      `     o becomes -(0) = 0 if siAry[x] < siAry[y]
      `       * all bits set to 0
      `   - siAry[y or x] & swapSI:
      `     o value is kept if swapSI = -1
      `     o set to 0 if swapSI = 0
      `   - the rest of the logic is a regular swap using
      `     xors from the standford bitthacking guide
      */

      /**************************************************\
      * Fun01 Sec03 Sub02:
      *   - partially sort the array to position the
      *     choosen median value
      \**************************************************/

      /*_________position_the_midpoint__________________*/
      siAry[midSI] ^= siAry[endSI];
      siAry[endSI] ^= siAry[midSI];
      siAry[midSI] ^= siAry[endSI];
      midSI = endSI - 1;
      posSI = startSI;

      /*_______paritally_sort_to_see_midpiont_position__*/
      while(posSI < midSI)
      { /*Loop: shift elements around*/
         swapSI = -(siAry[endSI] <= siAry[posSI]);
         siAry[posSI] ^= (siAry[midSI] & swapSI);
         siAry[midSI] ^= (siAry[posSI] & swapSI);
         siAry[posSI] ^= (siAry[midSI] & swapSI);
            /*see previous logic note for an explanation*/
         midSI += swapSI;
         posSI += (! swapSI);
      } /*Loop: shift elements around*/

      midSI += (siAry[endSI] > siAry[midSI]);
         /*this incurments midSI by one if the value at
         `  midSI is one less then my midpiont.
         */
      swapSI = -(midSI != endSI);
      siAry[midSI] ^= (siAry[endSI] & swapSI);
      siAry[endSI] ^= (siAry[midSI] & swapSI);
      siAry[midSI] ^= (siAry[endSI] & swapSI);

      /**************************************************\
      * Fun01 Sec03 Sub03:
      *   - check if the midpoint was the kth value
      \**************************************************/

      if(midSI == kthSI)
         break;
      else if(midSI < kthSI)
         startSI = midSI + 1;
      else
         endSI = midSI - 1;
   } /*Loop: find kth value and partially sort*/

   if(startSI == endSI - 1)
   { /*If: I can do one more swap*/
      swapSI = -(siAry[endSI] < siAry[startSI]);
      siAry[startSI] ^= (siAry[endSI] & swapSI);
      siAry[endSI] ^= (siAry[startSI] & swapSI);
      siAry[startSI] ^= (siAry[endSI] & swapSI);
   } /*If: I can do one more swap*/

   return siAry[kthSI];
} /*select_siQuick*/

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
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun02 TOC:
   '   - quick select algorithm (partially sorts)
   '   o fun02 sec01:
   '     - variable declarations
   '   o fun02 sec02:
   '     - deal with cases were there are two few elements
   '   o fun02 sec03:
   '     - do one round of sorting
   '   o fun02 sec04:
   '     - call quick sort to do next rounds of sorting
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun02 Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   signed int startSI = 0;
   signed int midSI = 0;
   signed int endSI = lenSI - 1;
   signed int posSI = 0;
   signed int swapSI = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun02 Sec02:
   ^   - deal with cases were there are two few elements
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(endSI < 2)
   { /*If: so few items it is easy to find the kth item*/
      ++midSI; /*convert to the length*/

      if(endSI <= 0)
         return; /*only one item*/

      else
      { /*Else: two values*/
         swapSI = -(siAry[startSI] > siAry[endSI]);
         siAry[startSI] ^= (siAry[endSI] & swapSI);
         siAry[endSI] ^= (siAry[startSI] & swapSI);
         siAry[startSI] ^= (siAry[endSI] & swapSI);
           /*brachless if > swap; see the next section for
           `  how this works
           */

         return;
      } /*Else: two values*/
   } /*If: so few items it is easy to find the kth item*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun02 Sec03:
   ^   - do one round of sorting
   ^   o fun02 sec03 sub01:
   ^     - find median value to use as pivot + start loop
   ^   o fun02 sec03 sub02:
   ^     - position the midpoint in the array
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun02 Sec03 Sub01:
   *   - find median value to use as a pivot + start loop
   \*****************************************************/

   midSI = (endSI + startSI) >> 1;

   /*_________find_the_midpoint_using_3_medians______*/
   /*the branchless swap is faster then the same
   `  branched swap
   */
   swapSI = -(siAry[startSI] > siAry[endSI]);
   siAry[startSI] ^= (siAry[endSI] & swapSI);
   siAry[endSI] ^= (siAry[startSI] & swapSI);
   siAry[startSI] ^= (siAry[endSI] & swapSI);

   swapSI = -(siAry[startSI] > siAry[midSI]);
   siAry[startSI] ^= (siAry[midSI] & swapSI);
   siAry[midSI] ^= (siAry[startSI] & swapSI);
   siAry[startSI] ^= (siAry[midSI] & swapSI);

   swapSI = -(siAry[midSI] > siAry[endSI]);
   siAry[midSI] ^= (siAry[endSI] & swapSI);
   siAry[endSI] ^= (siAry[midSI] & swapSI);
   siAry[midSI] ^= (siAry[endSI] & swapSI);
   /*Logic:
   `   - swapSI = -(siAry[x] > siAry[y])
   `     o becomes -(1) = -1 if siAry[x] > siAry[y]
   `       * all bits set to 1
   `     o becomes -(0) = 0 if siAry[x] < siAry[y]
   `       * all bits set to 0
   `   - siAry[y or x] & swapSI:
   `     o value is kept if swapSI = -1
   `     o set to 0 if swapSI = 0
   `   - the rest of the logic is a regular swap using
   `     xors from the standford bitthacking guide
   */

   /**************************************************\
   * Fun02 Sec03 Sub02:
   *   - position the midpoint in the array
   \**************************************************/

   /*_________position_the_midpoint__________________*/
   siAry[midSI] ^= siAry[endSI];
   siAry[endSI] ^= siAry[midSI];
   siAry[midSI] ^= siAry[endSI];
   midSI = endSI - 1;
   posSI = startSI;

   while(posSI < midSI)
   { /*Loop: position the pivot (midpoint)*/
      swapSI = -(siAry[endSI] <= siAry[posSI]);
      siAry[posSI] ^= (siAry[midSI] & swapSI);
      siAry[midSI] ^= (siAry[posSI] & swapSI);
      siAry[posSI] ^= (siAry[midSI] & swapSI);
         /*see previous logic note for an explanation*/
      midSI += swapSI;
      posSI += (! swapSI);
   } /*Loop: position the pivot (midpoint)*/

   midSI += (siAry[endSI] > siAry[midSI]);
      /*this incurments midSI by one if the value at
      `  midSI is one less then my midpiont.
      */
   swapSI = -(midSI != endSI);
   siAry[midSI] ^= (siAry[endSI] & swapSI);
   siAry[endSI] ^= (siAry[midSI] & swapSI);
   siAry[midSI] ^= (siAry[endSI] & swapSI);

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   * Fun02 Sec04:
   *   - call quick sort to do the next rounds of sorting
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   sort_siQuick(&siAry[startSI], midSI - startSI);
   sort_siQuick(&siAry[midSI + 1], endSI - midSI);
      /*endSI - midSI should be the length when passing
      `  midSI + 1
      */
} /*sort_siQuick*/

/*-------------------------------------------------------\
| Fun03: sortWithStack_siQuick
|   - quick sort but uses a stack instead of recursion
| Input:
|   - siAry:
|     o signed int array to sort
|   - aryLenSI:
|     o number of items (length) in siAry (index 1)
| Output:
|   - Modifies:
|     o siAry to be sorted from least to greatest
| WARNING:
|   - the stack size is 64, so you have a smaller sort
|     range. For each round one pivot is added to the
|     start of the stack and one at the end. Once a pivot
|     has no more pivots, the pivot at the end is moved
|     to the start. So, this should resolve the pivots
|     that have less items to sort before moving to the
|     pivots with more items.
|     - there is likley a way to overload this stack and
|       segfault your program.
|   - this is slower then the recursive method, so I would
|     not recomend it. It is here because I got something
|     close and wanted to see if I could complete it
\-------------------------------------------------------*/
void
sortWithStack_siQuick(
   signed int *siAry,
   signed int aryLenSI
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun03 TOC:
   '   - quick sort but uses a stack instead of recursion
   '   o fun03 sec01:
   '     - variable declarations
   '   o fun03 sec02:
   '     - check if need to sort
   '   o fun03 sec03:
   '     - sort the array
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   signed int endSI = aryLenSI - 1;
   signed int startSI = 0;
   signed int midSI = 0;
   signed int swapSI = 0;

   signed int startArySI[64];
   signed int endArySI[64];
      /*64 may seems small, but it allows me to split
      `  the array up 64 times. A segfault case would be
      `  if every midpiont from the 3 medians split each
      `  section of the array into 3 ending items and
      `  the rest of the array. This would mean over 64
      `  pivots in an array of 200 items.
      `The three medians should be safe most of the time.
      `  Using quick select would prevent this probelm,
      `  but result in 2 * n * log2(n) time.
      */
   signed int stackEndSI = 0;   /*length of my stack*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec02:
   ^   - check if need to sort
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(aryLenSI < 3)
   { /*If: have two items, finish the sort*/
      if(aryLenSI < 2)
         return; /*empty or one item*/
      else
      { /*Else: two items*/
         swapSI = -(siAry[0] > siAry[1]);
         siAry[0] ^= (siAry[1] & swapSI);
         siAry[1] ^= (siAry[0] & swapSI);
         siAry[0] ^= (siAry[1] & swapSI);
           /*brachless if > swap; see previous function
           `  for the logic
           */
         return;
      } /*Else: two items*/
   } /*If: have two items, finish the sort*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec03:
   ^   - sort the array
   ^   o fun03 sec03 sub01:
   ^     - setup stacks, start loop, & check if have
   ^       enough items to sort
   ^   o fun03 sec03 sub02:
   ^     - find a midpoint of the array
   ^   o fun03 sec03 sub03:
   ^     - position the midpoint and sort
   ^   o fun03 sec03 sub04:
   ^     - add next midpionts (pivots) to the stack
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun03 Sec03 Sub01:
   *   - setup stacks, start loop, & check if have enough
   *     items to sort
   \*****************************************************/

   /*set up my stacks*/
   startArySI[0] = 0;
   endArySI[0] = aryLenSI - 1;
   stackEndSI = 0;

   while(stackEndSI >= 0)
   { /*Loop: sort the array*/
      startSI = startArySI[0];
      endSI = endArySI[0];

      if(startSI >= endSI - 1)
      { /*If: have two items*/
         if(startSI < endSI)
         { /*If: I only have two items*/
            swapSI = -(siAry[startSI] > siAry[endSI]);
            siAry[startSI] ^= (siAry[endSI] & swapSI);
            siAry[endSI] ^= (siAry[startSI] & swapSI);
            siAry[startSI] ^= (siAry[endSI] & swapSI);
         } /*If: I only have two items*/

         startArySI[0] = startArySI[stackEndSI];
         endArySI[0] = endArySI[stackEndSI];
         --stackEndSI;
         continue;
      } /*If: have two items*/

      /**************************************************\
      * Fun03 Sec03 Sub02:
      *   - find a midpoint of the array
      \**************************************************/

      else
      { /*Else: have three or more items*/
         midSI = (endSI + startSI) >> 1;

         swapSI = -(siAry[startSI] > siAry[endSI]);
         siAry[startSI] ^= (siAry[endSI] & swapSI);
         siAry[endSI] ^= (siAry[startSI] & swapSI);
         siAry[startSI] ^= (siAry[endSI] & swapSI);

         swapSI = -(siAry[startSI] > siAry[midSI]);
         siAry[startSI] ^= (siAry[midSI] & swapSI);
         siAry[midSI] ^= (siAry[startSI] & swapSI);
         siAry[startSI] ^= (siAry[midSI] & swapSI);

         swapSI = -(siAry[midSI] > siAry[endSI]);
         siAry[midSI] ^= (siAry[endSI] & swapSI);
         siAry[endSI] ^= (siAry[midSI] & swapSI);
         siAry[midSI] ^= (siAry[endSI] & swapSI);
         /*Logic:
         `   - swapSI = -(siAry[x] > siAry[y])
         `     o becomes -(1) = -1 if siAry[x] > siAry[y]
         `       * all bits set to 1
         `     o becomes -(0) = 0 if siAry[x] < siAry[y]
         `       * all bits set to 0
         `   - siAry[y or x] & swapSI:
         `     o value is kept if swapSI = -1
         `     o set to 0 if swapSI = 0
         `   - the rest of the logic is a regular swap using
         `     xors from the standford bitthacking guide
         */

         /***********************************************\
         * Fun03 Sec03 Sub03:
         *   - position the midpoint and sort
         \***********************************************/

         midSI = endSI - 1;

         while(startSI < midSI)
         { /*Loop: position the pivot (midpoint)*/
            swapSI = -(siAry[endSI] <= siAry[startSI]);
            siAry[startSI] ^= (siAry[midSI] & swapSI);
            siAry[midSI] ^= (siAry[startSI] & swapSI);
            siAry[startSI] ^= (siAry[midSI] & swapSI);

            midSI += swapSI;
            startSI += (! swapSI);
         } /*Loop: position the pivot (midpoint)*/

         midSI += (siAry[endSI] > siAry[midSI]);
            /*this incurments midSI by one if the value at
            `  midSI is one less then my midpiont.
            */
         swapSI = -(midSI != endSI);
         siAry[midSI] ^= (siAry[endSI] & swapSI);
         siAry[endSI] ^= (siAry[midSI] & swapSI);
         siAry[midSI] ^= (siAry[endSI] & swapSI);

         startSI = startArySI[0];

         /***********************************************\
         * Fun03 Sec03 Sub04:
         *   - add next midpionts (pivots) to the stack
         \***********************************************/

         if(midSI <= startSI + 1)
         { /*If: finished the sort for this pivot*/
            if(midSI < endSI)
            { /*If: have ending pivot*/
               startArySI[0] = midSI + 1;
               endArySI[0] = endSI;
            } /*If: have ending pivot*/

            else
            { /*Else: can reduce the stack by one*/
               startArySI[0] = startArySI[stackEndSI];
               endArySI[0] = endArySI[stackEndSI];
               --stackEndSI;
            } /*Else: can reduce the stack by one*/
         } /*If: finished the sort for this pivot*/

         else
         { /*Else: have at least one pivot*/
            endArySI[0] = midSI - 1;

            if(midSI < endSI - 1)
            { /*If: have ending pivot + 2 items*/
               ++stackEndSI;
               startArySI[stackEndSI] = midSI + 1;
               endArySI[stackEndSI] = endSI;
            } /*If: have ending pivot + 2 items*/
         } /*Else: have at least one pivot*/
      } /*Else: have three or more items*/
   } /*Loop: sort the array*/

} /*sortWithStack_siQuick*/

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
