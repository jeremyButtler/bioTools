/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' mkPng SOF:
'   - creates the structure to hold a png (st_mkPng) and
'     also prints the png to a file
'   - a good amount of this code came from misc0110's
'     libattopng (https://github.com/misc0110/libattpng),
'     which is under the MIT license
'   - the parts not from libattopng were inspired by it
'     and arise from different goals
'   o .h st01: st_mkPng
'     - has png image for mkPng
'   o fun01: blank_st_mkPng
'     - blanks the pixel matrix in a st_mkPng struct
'   o .c fun02 calcDepth_st_mkPng
'     - finds minimum bit depth for a st_mkPng struct
'   o fun03: addCol_st_mkPng
'     - add a color to a st_mkPng struct
'   o fun04: rmCol_st_mkPng
'     - remove a color from a st_mkPng struct
'   o fun05: initCol_st_mkPng
'     - initialize color array with x greyscale colors
'   o fun06: init_st_mkPng
'     - initialize a st_mkPng struct
'   o fun07: freeStack_st_mkPng
'     - frees variables in a st_mkPng struct
'   o fun08: freeHeap_st_mkPng
'     - frees a st_mkPng struct
'   o fun09: setup_st_mkPng
'     - allocates memory for a st_mkPng struct
'   o fun10: mk_st_mkPng
'     - makes a st_mkPng struct on heap
'   o .c fun11: addUint_mkPng
'     - adds a uint to a png buffer
'   o .c fun12: addIhdr_st_mkPng
'     - add the IHDR header to a st_mkPng struct
'   o .c fun13: addPallete_st_mkPng
'     - add pallete (PLTE) header to a st_mkPng struct
'     - copied from misc0110's libattpng repository
'   o .c fun14: addImage_st_mkPng
'     - add image IDAT header
'   o .c fun15: addIend_st_mkPng
'     - add end header (IEND) for png 
'   o fun16: print_st_mkPng
'     - prints a png to output file
'   o license:
'     - licensing for this code (public domain / mit)
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#ifndef MAKE_GRAPH_PNG_H
#define MAKE_GRAPH_PNG_H

#define def_memErr_mkPng 1
#define def_overflow_mkPng 2
#define def_bitChange_mkPng 4

/*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
^ Header Sec0x:
^   - color scheme entry
\<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

/*default color scheme*/
   /*white*/
   #define def_1stColRed_mkPng 0xFF
   #define def_1stColBlu_mkPng 0xFF
   #define def_1stColGre_mkPng 0xFF
   
   /*magma yellow*/
   #define def_2ndColRed_mkPng 0xFD
   #define def_2ndColBlu_mkPng 0xE7
   #define def_2ndColGre_mkPng 0x25
   
   /*magma pink*/
   #define def_3rdColRed_mkPng 0xF1
   #define def_3rdColBlu_mkPng 0x60
   #define def_3rdColGre_mkPng 0x5D
   
   /*magma dark purple*/
   #define def_4thColRed_mkPng 0x00
   #define def_4thColBlu_mkPng 0x00
   #define def_4thColGre_mkPng 0x04


/*maximum colors allowed by indexing (8bit)*/
#define def_maxCol_mkPng 255 /*one byte*/

#define def_lenZlibHeader_mkPng 6
   /*how many bytes each zlib header adds to the total
   `  png size
   */
#define def_idatEnd_mkPng 8 /*idat ends in 8 extra bytes*/
#define def_width_mkPng 960
#define def_height_mkPng 720

/*-------------------------------------------------------\
| ST01: st_mkPng
|   - has png image for mkPng
\-------------------------------------------------------*/
typedef struct st_mkPng
{
   unsigned short widthUS;   /*number pixels in a row*/
   unsigned short heightUS;  /*number pixels in a column*/
   signed long numPixelSL;   /*number total pixels*/
   unsigned char pixDepthUC; /*bits per pixel*/
   unsigned char pixPerByteUC; /*number pixels in a byte*/

   unsigned char shiftUC;
       /*shift by to convert pixels count to byte count*/
   unsigned char modUC;
       /*mod by to get extra pixels at ends*/

   unsigned char *pixelAryUC;      /*matrix of pixels*/
      /*1d array, each x elements is a row*/
      /*always make sure this is a multiple of 8*/
   signed long sizePixelSL;      /*pixelAryUC array size*/
   signed long usedBytesSL;      /*number pix bytes used*/

   unsigned char redAryUC[def_maxCol_mkPng];/*red value*/
   unsigned char bluAryUC[def_maxCol_mkPng];/*blue value*/
   unsigned char greAryUC[def_maxCol_mkPng];/*green valu*/
   unsigned char numColUC; /*number colors in pallete*/
}st_mkPng;

/*-------------------------------------------------------\
| Fun01: blank_st_mkPng
|   - blanks the pixel matrix in a st_mkPng struct
| Input:
|   - pngSTPtr:
|     o pointer to a st_mkPng struct to blank
| Output:
|   - Modifies:
|     o pixelAryUC to be full of zeros
\-------------------------------------------------------*/
void
blank_st_mkPng(
   struct st_mkPng *pngSTPtr
);

/*-------------------------------------------------------\
| Fun03: addCol_st_mkPng
|   - add a color to a st_mkPng struct
| Input:
|   - pngSTPtr:
|     o pointer to st_mkPng struct to add color to
|   - redUC:
|     o 0-255 value for red channel
|   - bluUC:
|     o 0-255 value for blue channel
|   - greUC:
|     o 0-255 value for green channel
|   - indexSS:
|     o position to add index 0 (use -1 for end)
|       * -1 for append to end
|       * if > end, will append to end
|       * if > -1 and < end will overwrite color at index
| Output:
|   - Modifies:
|     o redAryUC, bluAryUC, and greAryUC in pngSTPtr to
|       have last value set to input color
|     o numColUC in pngSTPtr to have input color
|     o sets errSCPtr:
|       * 0 for no errors
|       * def_overflow_mkPng for color array overflow
|       * def_bitChange_mkPng if color bit depth incresed
|         (ex; from 2bit to 4bit)
|     o calls calcDepth_st_mkPng to reset bit values if
|       color is appened to end
|   - Returns:
|     o index of added color
|       * if no errors
|       * also if had an increase in bit depth
|     o -1 if had an error
\-------------------------------------------------------*/
signed short
addCol_st_mkPng(
   struct st_mkPng *pngSTPtr, /*add color to*/
   unsigned char redUC,       /*red color channel*/
   unsigned char bluUC,       /*blue color channel*/
   unsigned char greUC,       /*green color channel*/
   signed short indexSS,      /*index to add value to*/
   signed char *errSCPtr      /*holds errors*/
);

/*-------------------------------------------------------\
| Fun04: rmCol_st_mkPng
|   - remove a color from a st_mkPng struct
| Input:
|   - pngSTPtr:
|     o pointer to a st_mkPng struct to remove color from
|   - indexUC:
|     o index of color to remove (the next color will
|       be the color at indexUC)
| Output:
|   - Modifies:
|     o redAryUC, bluAryUC, and greAryUC in pngSTPtr to
|       have all values past color shifted to old position
|     o numColUC in pngSTPtr to be 1 less
|     o calls calcDepth_st_mkPng to reset bit values if
|       removed color reduces bit depth
|   - Returns:
|     o 0 if bit depth stayed same or index out of range
|     o def_bitChange_mkPng it bit depth decreased
\-------------------------------------------------------*/
signed char
rmCol_st_mkPng(
   struct st_mkPng *pngSTPtr,/*to remove color from*/
   unsigned char indexUC     /*index of color to remove*/
);

/*-------------------------------------------------------\
| Fun05: initCol_st_mkPng
|   - initialize color array with x greyscale colors
| Input:
|   - pngSTPtr:
|     o pointer to a st_mkPng struct to add colors to
|   - numColUC:
|     o number of colors to add
| Output:
|   - Modifies:
|     o redAryUC, bluAryUC, and greAryUC in pngSTPtr to
|       have numColUC grey colors
|     o numColUC in pngSTPtr to be input numColUC
|     o calls calcDepth_st_mkPng to reset bit values if
|       removed color reduces bit depth
|   - Returns:
|     o 0 if bit depth stayed same or index out of range
|     o def_bitChange_mkPng it bit depth decreased
\-------------------------------------------------------*/
signed char
initCol_st_mkPng(
   struct st_mkPng *pngSTPtr, /*to add all colors to*/
   unsigned char numColUC     /*number colors to add*/
);

/*-------------------------------------------------------\
| Fun06: init_st_mkPng
|   - initialize a st_mkPng struct
| Input:
|   - pngSTPtr:
|     o pointer to a st_mkPng struct to initialize
| Output:
|   - Modifies:
|     o sets pixelAryUC and sizePixeSL in pngSTPtr to 0
|     o all values in pngSTPtr to be defaults
\-------------------------------------------------------*/
void
init_st_mkPng(
   struct st_mkPng *pngSTPtr
);

/*-------------------------------------------------------\
| Fun07: freeStack_st_mkPng
|   - frees variables in a st_mkPng struct
| Input:
|   - pngSTPtr:
|     o pointer to st_mkPng with variables to free
| Output:
|   - Frees:
|     o pixelAryUC in pngSTPtr
|   - Modifies:
|     o pixelAryUC in pngSTPtr to be 0
|     o sizePixelSL in pngSTPtr to be 0
|     o all other values to default wiht init_st_mkPng
\-------------------------------------------------------*/
void
freeStack_st_mkPng(
   struct st_mkPng *pngSTPtr
);

/*-------------------------------------------------------\
| Fun08: freeHeap_st_mkPng
|   - frees a st_mkPng struct
| Input:
|   - pngSTPtr:
|     o pointer to st_mkPng struct to free
| Output:
|   - Frees:
|     o pngSTPtr (you must set to 0/null)
\-------------------------------------------------------*/
void
freeHeap_st_mkPng(
   struct st_mkPng *pngSTPtr
);

/*-------------------------------------------------------\
| Fun09: setup_st_mkPng
|   - allocates memory for a st_mkPng struct
| Input:
|   - pngSTPtr:
|     o pointer to st_mkPng struct to allocate memory
|   - widthUS:
|     o width of png in pixels
|   - heightUS:
|     o height of png in pixels
|   - maxColUC:
|     o maximum colors plan to be used in color pallete
|     o use 255 to force 8bit
|     o 0 to not change
| Output:
|   - Modifies:
|     o pixelAryUC in pngSTPtr to have needed memory
|     o sizePixeSL to have number bytes in pixelAryUC
|   - Returns:
|     o 0 for no errors
|     o def_memErr_mkPng for memory errors
\-------------------------------------------------------*/
signed char
setup_st_mkPng(
   struct st_mkPng *pngSTPtr,
   unsigned short widthUS,
   unsigned short heightUS,
   unsigned char maxColUC
);

/*-------------------------------------------------------\
| Fun10: mk_st_mkPng
|   - makes a st_mkPng struct on heap
| Input:
|   - widthUI:
|     o width of graph (0 for default)
|   - heigthSL:
|     o heigth of graph (0 for default)
|   - maxColUC:
|     o maximum colors plan to be used in color pallete
|     o use 255 to force 8bit
|     o 0 to not change
| Output:
|   - Returns:
|     o pointer to st_mkPng struct
|     o 0 for no errors
\-------------------------------------------------------*/
struct st_mkPng*
mk_st_mkPng(
   unsigned short widthUS,
   unsigned short heightUS,
   unsigned char maxColUC
);

/*-------------------------------------------------------\
| Fun16: print_st_mkPng
|   - prints a png to output file
| Input:
|   - pngSTPtr:
|     o pointer to st_mkPng struct to write to outFILE
|   - outFILE:
|     o pionter to FILE struct to write png to (binary
|       file ("wb"))
| Output:
|   - Prints:
|     o png header and contents of pngSTPtr to outFILE
\-------------------------------------------------------*/
void
print_st_mkPng(
   struct st_mkPng *pngSTPtr,  /*png to print*/
   void *outFILE               /*file to print to*/
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
