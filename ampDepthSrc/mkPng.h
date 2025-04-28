/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' mkPng SOF:
'   - make graphs for ampDepth
'   - a good amount of this code came from misc0110's
'     libattopng (https://github.com/misc0110/libattpng),
'     which is under the MIT license
'   - the parts not from libattopng were inspired by it
'     and arise from different goals
'   o .h st01: st_mkPng
'     - has png image for mkPng
'   o .h st02: set_mkPng
'     - has print settings for st_mkPng struct
'     - really here for future use
'   o .c tbl01: crc32_mkPng
'     - crc32 lookup table
'     - copied from misc0110's libattpng repository
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
'   o fun11: addPixel_st_mkPng
'     - adds a single pixel to a st_mkPng image
'   o fun12: addBar_st_mkPng
'     - adds a bar to a st_mkPng image
'   o .c fun13: crc32_mkPng
'     - find new crc value after adding next byte
'     - copied from misc0110's libattpng repository, I
'       really do not know whats going on here
'   o fun14: strToCrc32_mkPng
'     - find new crc value for string
'   o .c fun15: bigendInt_mkPng
'     - converts unit to bigendin
'     - copied from misc0110's libattpng repository
'   o .c fun16: addUint_mkPng
'     - adds a uint to a png buffer
'   o .c fun17: addIhdr_st_mkPng
'     - add the IHDR header to a st_mkPng struct
'   o .c fun18: addPallete_st_mkPng
'     - add pallete (PLTE) header to a st_mkPng struct
'     - copied from misc0110's libattpng repository
'   o .c fun19: addImage_st_mkPng
'     - add image IDAT header
'   o .c fun20: addIend_st_mkPng
'     - add end header (IEND) for png 
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

/*maximum colors allowed by indexing (8bit)*/
#define def_maxCol_mkPng 256 /*one byte*/

#define def_extraBytes_mkPng 6 /*6 bytes at each row end*/
#define def_idatEnd_mkPng 8 /*idat ends in 8 extra bytes*/
#define def_width_mkPng 960
#define def_height_mkPng 720

/*so always have maximum value for cpu*/
#ifdef PLAN9_64
   typedef unsigned long long ul_mkPng;
#else
   typedef unsigned long ul_mkPng;
#endif

/*this is a trick I use in ulCp to figure out how to
`  convert number bytes to number longs
*/
#define def_shiftUL_mkPng ((sizeof(ul_mkPng) >> 1) - (sizeof(ul_mkPng) >> 3))

/*Logic:
`   - sizeof(ul_mkPng) >> 1:
`     o This gets the number of bytes to shift to get
`       an 32 bit or 16 bit number to one bytes. Basically
`       it is divide by 2
`   - (sizeof(ul_mkPng) >> 3)
`     o This adds in 1 if I have eight bytes. It is needed
`       as the 64 bit correction step
`   - (sizeof(ul_mkPng) >> 1) - (sizeof(ul_mkPng) >> 3)
`     o Gives me the number of bits I need to shift to
`       get the number of characters in an ul_mkPng
`   - (sizeof(ul_mkPng) >> 3) << (sizeof(ul_mkPng) >> 4)
`     o This is not used, but would correct for a 128 bit
`       number. Each time I increase the bit size I need
`       to add a correction in
*/

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

   /*headers to print out*/
   unsigned char pngStr[16];     /*png header*/
   unsigned char lenPngUC;       /*png header length*/

   unsigned char ihdrStr[32];    /*IDHR header*/
   unsigned char lenIhdrUC;      /*length of ihdr header*/

   unsigned char palleteStr[32 + def_maxCol_mkPng * 3];
      /*pallete header*/
   signed short lenPalSS;        /*length of pallete*/

   unsigned char idatStr[32];    /*image header (IDAT)*/
   unsigned char lenIdatUC;      /*length of IDAT header*/

   unsigned char endStr[16];     /*png end header (IEND)*/
   unsigned char lenEndUC;       /*lenght of end header*/
}st_mkPng;

/*-------------------------------------------------------\
| ST02: set_mkPng
|   - has print settings for st_mkPng struct
|   - really here for future use
\-------------------------------------------------------*/
typedef struct set_mkPng
{
}set_mkPng;

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
|   - maxColUC:
|     o maximum colors plan to be used in color pallete
|     o use 256 to force 8bit
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
|     o use 256 to force 8bit
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
| Fun11: addPixel_st_mkPng
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
|     o def_overflow_mkPng if coordinates are outside of
|       grpah range
\-------------------------------------------------------*/
signed char
addPixel_st_mkPng(
   struct st_mkPng *pngSTPtr, /*add bar to png*/
   signed long xSL,           /*x coordinate (pixels)*/
   signed long ySL,           /*y coordiante (pixels)*/
   signed char colUC          /*color of bar*/
);

/*-------------------------------------------------------\
| Fun12: addBar_st_mkPng
|   - adds a bar to a st_mkPng image
| Input:
|   - pngSTPtr:
|     o pointer to st_mkPng struct to add bar to
|   - xSL:
|     o x coordinate of left conner; start of bar; index 0
|   - ySL:
|     o y coordiante of bottom of bar (index 0)
|   - widthUI:
|     o width in pixels of bar
|   - heigthSL:
|     o heigth in pixels of bar
|   - colUC:
|     o color (0-4) to assign
| Output:
|   - Modifies:
|     o pixelAryUC in pngSTPtr to have bar
| Note:
|   o the minimum width is at least one byte worth,
|     otherwise, do single pixel modifications
|     with addPixel_st_mkPng
\-------------------------------------------------------*/
signed char
addBar_st_mkPng(
   struct st_mkPng *pngSTPtr, /*add bar to png*/
   signed long xSL,           /*x coordinate (pixels)*/
   signed long ySL,           /*y coordiante (pixels)*/
   signed long widthUI,       /*pixels wide of bar*/
   signed long heigthSL,      /*pixels high of bar*/
   signed char colUC          /*color of bar*/
);


/*-------------------------------------------------------\
| Fun20: print_st_mkPng
|   - prints a png to output file
| Input:
|   - pngSTPtr:
|     o pointer to st_mkPng struct to write to outFILE
|   - setSTPtr:
|     o pionter to set_mkPng struct with settings to save
|       png as (for easier future modifications)
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
   struct set_mkPng *setSTPtr, /*print settings*/
   void *outFILE               /*file to print to*/
);

#endif

