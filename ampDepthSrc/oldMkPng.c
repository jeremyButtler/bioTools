/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' mkPng SOF:
'   - make graphs for ampDepth
'   - a good amount of this code came from misc0110's
'     libattopng (https://github.com/misc0110/libattpng),
'     which is under the MIT license
'   - the parts not from libattopng were inspired by it
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
'   o .c fun19: addImage_st_mkPng
'     - add image IDAT header
'     - copied from misc0110's libattpng repository
'   o .c fun20: addIend_st_mkPng
'     - add end header (IEND) for png 
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#ifdef PLAN9
   #inlcude <u.h>
   #include <libc.h>
#else
   #include <stdlib.h>
#endif

#include "mkPng.h"

#include <stdio.h>

/*from libattopng, is maximum value of unsigned short*/
#define def_adler_mkPng 65521
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

/*-------------------------------------------------------\
| Tbl01: crc32_mkPngUI
|   - crc32 lookup table
|   - copied from misc0110's libattpng repository
\-------------------------------------------------------*/
unsigned int
   crc32_mkPngUL[256] = {
      0x00000000, 0x77073096, 0xee0e612c, 0x990951ba,
      0x076dc419, 0x706af48f, 0xe963a535, 0x9e6495a3,
      0x0edb8832, 0x79dcb8a4, 0xe0d5e91e, 0x97d2d988,
      0x09b64c2b, 0x7eb17cbd, 0xe7b82d07, 0x90bf1d91,
      0x1db71064, 0x6ab020f2, 0xf3b97148, 0x84be41de,
      0x1adad47d, 0x6ddde4eb, 0xf4d4b551, 0x83d385c7,
      0x136c9856, 0x646ba8c0, 0xfd62f97a, 0x8a65c9ec,
      0x14015c4f, 0x63066cd9, 0xfa0f3d63, 0x8d080df5,
      0x3b6e20c8, 0x4c69105e, 0xd56041e4, 0xa2677172,
      0x3c03e4d1, 0x4b04d447, 0xd20d85fd, 0xa50ab56b,
      0x35b5a8fa, 0x42b2986c, 0xdbbbc9d6, 0xacbcf940,
      0x32d86ce3, 0x45df5c75, 0xdcd60dcf, 0xabd13d59,
      0x26d930ac, 0x51de003a, 0xc8d75180, 0xbfd06116,
      0x21b4f4b5, 0x56b3c423, 0xcfba9599, 0xb8bda50f,
      0x2802b89e, 0x5f058808, 0xc60cd9b2, 0xb10be924,
      0x2f6f7c87, 0x58684c11, 0xc1611dab, 0xb6662d3d,
      0x76dc4190, 0x01db7106, 0x98d220bc, 0xefd5102a,
      0x71b18589, 0x06b6b51f, 0x9fbfe4a5, 0xe8b8d433,
      0x7807c9a2, 0x0f00f934, 0x9609a88e, 0xe10e9818,
      0x7f6a0dbb, 0x086d3d2d, 0x91646c97, 0xe6635c01,
      0x6b6b51f4, 0x1c6c6162, 0x856530d8, 0xf262004e,
      0x6c0695ed, 0x1b01a57b, 0x8208f4c1, 0xf50fc457,
      0x65b0d9c6, 0x12b7e950, 0x8bbeb8ea, 0xfcb9887c,
      0x62dd1ddf, 0x15da2d49, 0x8cd37cf3, 0xfbd44c65,
      0x4db26158, 0x3ab551ce, 0xa3bc0074, 0xd4bb30e2,
      0x4adfa541, 0x3dd895d7, 0xa4d1c46d, 0xd3d6f4fb,
      0x4369e96a, 0x346ed9fc, 0xad678846, 0xda60b8d0,
      0x44042d73, 0x33031de5, 0xaa0a4c5f, 0xdd0d7cc9,
      0x5005713c, 0x270241aa, 0xbe0b1010, 0xc90c2086,
      0x5768b525, 0x206f85b3, 0xb966d409, 0xce61e49f,
      0x5edef90e, 0x29d9c998, 0xb0d09822, 0xc7d7a8b4,
      0x59b33d17, 0x2eb40d81, 0xb7bd5c3b, 0xc0ba6cad,
      0xedb88320, 0x9abfb3b6, 0x03b6e20c, 0x74b1d29a,
      0xead54739, 0x9dd277af, 0x04db2615, 0x73dc1683,
      0xe3630b12, 0x94643b84, 0x0d6d6a3e, 0x7a6a5aa8,
      0xe40ecf0b, 0x9309ff9d, 0x0a00ae27, 0x7d079eb1,
      0xf00f9344, 0x8708a3d2, 0x1e01f268, 0x6906c2fe,
      0xf762575d, 0x806567cb, 0x196c3671, 0x6e6b06e7,
      0xfed41b76, 0x89d32be0, 0x10da7a5a, 0x67dd4acc,
      0xf9b9df6f, 0x8ebeeff9, 0x17b7be43, 0x60b08ed5,
      0xd6d6a3e8, 0xa1d1937e, 0x38d8c2c4, 0x4fdff252,
      0xd1bb67f1, 0xa6bc5767, 0x3fb506dd, 0x48b2364b,
      0xd80d2bda, 0xaf0a1b4c, 0x36034af6, 0x41047a60,
      0xdf60efc3, 0xa867df55, 0x316e8eef, 0x4669be79,
      0xcb61b38c, 0xbc66831a, 0x256fd2a0, 0x5268e236,
      0xcc0c7795, 0xbb0b4703, 0x220216b9, 0x5505262f,
      0xc5ba3bbe, 0xb2bd0b28, 0x2bb45a92, 0x5cb36a04,
      0xc2d7ffa7, 0xb5d0cf31, 0x2cd99e8b, 0x5bdeae1d,
      0x9b64c2b0, 0xec63f226, 0x756aa39c, 0x026d930a,
      0x9c0906a9, 0xeb0e363f, 0x72076785, 0x05005713,
      0x95bf4a82, 0xe2b87a14, 0x7bb12bae, 0x0cb61b38,
      0x92d28e9b, 0xe5d5be0d, 0x7cdcefb7, 0x0bdbdf21,
      0x86d3d2d4, 0xf1d4e242, 0x68ddb3f8, 0x1fda836e,
      0x81be16cd, 0xf6b9265b, 0x6fb077e1, 0x18b74777,
      0x88085ae6, 0xff0f6a70, 0x66063bca, 0x11010b5c,
      0x8f659eff, 0xf862ae69, 0x616bffd3, 0x166ccf45,
      0xa00ae278, 0xd70dd2ee, 0x4e048354, 0x3903b3c2,
      0xa7672661, 0xd06016f7, 0x4969474d, 0x3e6e77db,
      0xaed16a4a, 0xd9d65adc, 0x40df0b66, 0x37d83bf0,
      0xa9bcae53, 0xdebb9ec5, 0x47b2cf7f, 0x30b5ffe9,
      0xbdbdf21c, 0xcabac28a, 0x53b39330, 0x24b4a3a6,
      0xbad03605, 0xcdd70693, 0x54de5729, 0x23d967bf,
      0xb3667a2e, 0xc4614ab8, 0x5d681b02, 0x2a6f2b94,
      0xb40bbe37, 0xc30c8ea1, 0x5a05df1b, 0x2d02ef8d
};

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
){
   signed long slPix = 0;
   ul_mkPng *pixAryUL = 0;

   if(! pngSTPtr)
      return;

   if(pngSTPtr->pixelAryUC)
   { /*If: need to blank pixel matrix*/

      /*not as fast as SIMD, but faster then byte by byte
      `  and is always present
      */
      pixAryUL = (ul_mkPng *) pngSTPtr->pixelAryUC;

      for(
         slPix = 0;
         slPix < pngSTPtr->sizePixelSL;
         ++slPix
      ) pixAryUL[slPix] = 0;

   } /*If: need to blank pixel matrix*/

   /*blank the headers (png header is always same)*/
   pngSTPtr->ihdrStr[0] = '\0';
   pngSTPtr->palleteStr[0] = '\0';
   pngSTPtr->idatStr[0] = '\0';
   pngSTPtr->endStr[0] = '\0';
} /*blank_st_mkPng*/

/*-------------------------------------------------------\
| Fun02 calcDepth_st_mkPng
|   - finds minimum bit depth for a st_mkPng struct
| Input:
|   - pngSTPtr:
|     o pointer to st_mkPng struct to get bitdept for
| Output:
|   - Modifies:
|     o pixDepthUC in pngSTPtr to have bit depth
|     o pixPerByteUC in pngSTPtr to have number pixels in
|       one byte
|     o shiftUC in pngSTPtr to have the correct shift
|       value in pngSTPtr to convert byte count to pixel
|       count
|     o modUC in pngSTPtr to have modvalue to convert
|       pixel coordinate into byte position; coord % modUC
|   - Returns:
|     o 0 if color depth is unchaged
|     o 1 if color depth is chaged
\-------------------------------------------------------*/
signed char
calcDepth_st_mkPng(
   struct st_mkPng *pngSTPtr /*has bit depth values*/
){
   signed char changeSC = 0;

   if(pngSTPtr->numColUC > 16)
   { /*If: 8 bit color scheme*/
      if(pngSTPtr->pixDepthUC != 8)
         changeSC = 1;

      pngSTPtr->pixDepthUC = 8;
      pngSTPtr->pixPerByteUC = 1;
      pngSTPtr->shiftUC = 0;
      pngSTPtr->modUC = 1;
   } /*If: 8 bit color scheme*/

   else if(pngSTPtr->numColUC > 4)
   { /*Else If: 4 bit color scheme*/
      if(pngSTPtr->pixDepthUC != 4)
         changeSC = 1;

      pngSTPtr->pixDepthUC = 4;
      pngSTPtr->pixPerByteUC = 2;
      pngSTPtr->shiftUC = 1;
      pngSTPtr->modUC = 2;
   } /*Else If: 4 bit color scheme*/

   else if(pngSTPtr->numColUC > 2)
   { /*Else If: 2 bit color scheme*/
      if(pngSTPtr->pixDepthUC != 2)
         changeSC = 1;

      pngSTPtr->pixDepthUC = 2;
      pngSTPtr->pixPerByteUC = 4;
      pngSTPtr->shiftUC = 2;
      pngSTPtr->modUC = 4;
   } /*Else If: 2 bit color scheme*/

   else
   { /*Else: 1 bit color scheme*/
      if(pngSTPtr->pixDepthUC != 1)
         changeSC = 1;

      pngSTPtr->pixDepthUC = 1;
      pngSTPtr->pixPerByteUC = 8;
      pngSTPtr->shiftUC = 3;
      pngSTPtr->modUC = 8;
   } /*Else: 1 bit color scheme*/

   return changeSC;
}  /*calcDepth_st_mkPng*/

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
){
   *errSCPtr = 0;

   if(indexSS < 0)
      indexSS = pngSTPtr->numColUC;
   else if(indexSS > pngSTPtr->numColUC)
      indexSS = pngSTPtr->numColUC;

   if(indexSS > def_maxCol_mkPng)
      goto overflow_fun03;


   pngSTPtr->redAryUC[indexSS] = redUC;
   pngSTPtr->bluAryUC[indexSS] = bluUC;
   pngSTPtr->greAryUC[indexSS] = greUC;


   if( indexSS >= pngSTPtr->numColUC )
   { /*If: added a new color*/
      ++(pngSTPtr->numColUC);

      /*check if bit depth is same*/
      if( calcDepth_st_mkPng(pngSTPtr) )
         *errSCPtr = def_bitChange_mkPng;
   } /*If: added a new color*/


   goto ret_fun03;

   overflow_fun03:;
      indexSS = -1;
      *errSCPtr = def_overflow_mkPng;
      goto ret_fun03;
   
   ret_fun03:;
      return indexSS;
} /*addCol_st_mkPng*/
   
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
){
   unsigned char ucCol = 0;

   if(indexUC >= pngSTPtr->numColUC)
      goto ret_fun04; /*nothing to do*/

   else if(pngSTPtr->numColUC == 1)
   { /*Else If: only one color to delete*/  
      pngSTPtr->numColUC = 0;
      goto ret_fun04; /*nothing to do*/
   } /*Else If: only one color to delete*/  

   --pngSTPtr->numColUC;

   for( 
      ucCol = 0;
      ucCol < pngSTPtr->numColUC;
      ++ucCol
   ){ /*Loop: shift next colors back*/
      pngSTPtr->redAryUC[ucCol] ^=
         pngSTPtr->redAryUC[ucCol + 1];
      pngSTPtr->redAryUC[ucCol + 1] ^=
         pngSTPtr->redAryUC[ucCol];
      pngSTPtr->redAryUC[ucCol] ^=
         pngSTPtr->redAryUC[ucCol + 1];

      pngSTPtr->bluAryUC[ucCol] ^=
         pngSTPtr->bluAryUC[ucCol + 1];
      pngSTPtr->bluAryUC[ucCol + 1] ^=
         pngSTPtr->bluAryUC[ucCol];
      pngSTPtr->bluAryUC[ucCol] ^=
         pngSTPtr->bluAryUC[ucCol + 1];

      pngSTPtr->greAryUC[ucCol] ^=
         pngSTPtr->greAryUC[ucCol + 1];
      pngSTPtr->greAryUC[ucCol + 1] ^=
         pngSTPtr->greAryUC[ucCol];
      pngSTPtr->greAryUC[ucCol] ^=
         pngSTPtr->greAryUC[ucCol + 1];
   }  /*Loop: shift next colors back*/

   ucCol = 0;

   if( calcDepth_st_mkPng(pngSTPtr) )
      ucCol = def_bitChange_mkPng;

   ret_fun04:;
      return (signed char) ucCol;
} /*rmCol_st_mkPng*/

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
){
   signed short ssCol = 0;

   for(
      ssCol = numColUC;
      ssCol > -1;
      --ssCol
   ){ /*Loop: add colors*/
      pngSTPtr->redAryUC[ssCol] = (unsigned char) ssCol;
      pngSTPtr->bluAryUC[ssCol] = (unsigned char) ssCol;
      pngSTPtr->greAryUC[ssCol] = (unsigned char) ssCol;
   }  /*Loop: add colors*/

   pngSTPtr->numColUC = numColUC;
   return calcDepth_st_mkPng(pngSTPtr);
} /*initCol_st_mkPng*/

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
){
   signed char errSC = 0;

   if(! pngSTPtr)
      return;

   pngSTPtr->pixelAryUC = 0;
   pngSTPtr->sizePixelSL = 0;


   pngSTPtr->widthUS = def_width_mkPng;
   pngSTPtr->heightUS = def_height_mkPng;

   pngSTPtr->numPixelSL =
      pngSTPtr->widthUS * pngSTPtr->heightUS;

   /*add png header*/
   pngSTPtr->pngStr[0] = 137;
   pngSTPtr->pngStr[1] = 'P';
   pngSTPtr->pngStr[2] = 'N';
   pngSTPtr->pngStr[3] = 'G';
   pngSTPtr->pngStr[4] = '\r';
   pngSTPtr->pngStr[5] = 10; /*line feed*/
   pngSTPtr->pngStr[6] = 26; /*CTRL-Z*/
   pngSTPtr->pngStr[7] = 10; /*line feed*/
   pngSTPtr->lenPngUC = 8;


   /*set the color scheme*/
   addCol_st_mkPng(
      pngSTPtr,
      def_1stColRed_mkPng,
      def_1stColBlu_mkPng,
      def_1stColGre_mkPng,
      0,                   /*first index*/
      &errSC               /*ignore*/
   );

   addCol_st_mkPng(
      pngSTPtr,
      def_2ndColRed_mkPng,
      def_2ndColBlu_mkPng,
      def_2ndColGre_mkPng,
      1,                   /*second index*/
      &errSC               /*ignore*/
   );

   addCol_st_mkPng(
      pngSTPtr,
      def_3rdColRed_mkPng,
      def_3rdColBlu_mkPng,
      def_3rdColGre_mkPng,
      2,                   /*third index*/
      &errSC               /*ignore*/
   );

   addCol_st_mkPng(
      pngSTPtr,
      def_4thColRed_mkPng,
      def_4thColBlu_mkPng,
      def_4thColGre_mkPng,
      3,                   /*fourth index*/
      &errSC               /*ignore*/
   );
     

   /*set number of used bytes*/
   pngSTPtr->usedBytesSL =
      pngSTPtr->widthUS * pngSTPtr->heightUS;
   pngSTPtr->usedBytesSL >>= pngSTPtr->shiftUC;
   pngSTPtr->usedBytesSL +=
      (pngSTPtr->heightUS * def_extraBytes_mkPng);
   pngSTPtr->usedBytesSL += def_idatEnd_mkPng;


   /*called in case of future additions, but not needed*/
   blank_st_mkPng(pngSTPtr);
} /*init_st_mkPng*/

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
){
   if(! pngSTPtr)
      return;   /*nothing input*/

   if(pngSTPtr->pixelAryUC)
      free(pngSTPtr->pixelAryUC);

   init_st_mkPng(pngSTPtr);
} /*freeStack_st_mkPng*/

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
){
   if(! pngSTPtr)
      return;

   freeStack_st_mkPng(pngSTPtr);
   free(pngSTPtr);
} /*freeHeap_st_mkPng*/

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
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun09 TOC:
   '   - allocates memory for a st_mkPng struct
   '   o fun09 sec01:
   '     - variable declarations
   '   o fun09 sec02:
   '     - set up color scheme
   '   o fun09 sec03:
   '     - setup pixel matrix memory
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun09 Sec01:
   ^   - variable declarations
   \>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/

   signed long numBytesSL = 0;

   if(! pngSTPtr)
      return 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun09 Sec02:
   ^   - set up color scheme
   \>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/

   if(maxColUC > pngSTPtr->numColUC)
   { /*If: need more colors then already added*/

      initCol_st_mkPng(
         pngSTPtr,
         maxColUC
      );

   } /*If: need more colors then already added*/

   else if(! maxColUC)
      ; /*no colors added*/

   else if(maxColUC < pngSTPtr->numColUC)
   { /*If: need to remove colors*/

      while(pngSTPtr->numColUC < maxColUC)
      { /*Loop: remove extra colors at end*/

         rmCol_st_mkPng(
            pngSTPtr,
            pngSTPtr->numColUC - 1
         );

      } /*Loop: remove extra colors at end*/

   } /*If: need to remove colors*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun09 Sec03:
   ^   - setup pixel matrix
   ^   o fun09 sec03 sub01:
   ^     - find matrix dimensions
   ^   o fun09 sec03 sub02:
   ^     - allocate memory
   \>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/

   /*****************************************************\
   * Fun09 Sec03 Sub01:
   *   - find matrix dimensions
   \*****************************************************/

   /*make sure every row ends in a byte*/
   pngSTPtr->widthUS +=
      pngSTPtr->widthUS % pngSTPtr->pixPerByteUC;


   /*find number of pixels in png*/
   pngSTPtr->numPixelSL = pngSTPtr->widthUS;
   pngSTPtr->numPixelSL *= pngSTPtr->heightUS;
   pngSTPtr->numPixelSL >>= pngSTPtr->shiftUC;


   /*find number of bytes needed for png size*/
   numBytesSL = pngSTPtr->numPixelSL;
   numBytesSL +=
     (pngSTPtr->widthUS * def_extraBytes_mkPng);
     /*extra bytes at end of each row*/
   numBytesSL += def_idatEnd_mkPng;
     /*extra bytes at end of idat entry*/

   /*find number used (to print) bytes*/
   pngSTPtr->usedBytesSL =
      pngSTPtr->widthUS * pngSTPtr->heightUS;
   pngSTPtr->usedBytesSL >>= pngSTPtr->shiftUC;
   pngSTPtr->usedBytesSL +=
      (pngSTPtr->heightUS * def_extraBytes_mkPng);
   pngSTPtr->usedBytesSL += def_idatEnd_mkPng;

   /*****************************************************\
   * Fun09 Sec03 Sub02:
   *   - allocate memory
   \*****************************************************/

   if(pngSTPtr->pixelAryUC)
   { /*If: already have memory*/

      if( numBytesSL <= pngSTPtr->sizePixelSL)
      { /*If: already have needed memory*/

         blank_st_mkPng(pngSTPtr);
         return 0;

      }  /*If: already have needed memory*/

      free(pngSTPtr->pixelAryUC);
      pngSTPtr->pixelAryUC = 0;

   } /*If: already have memory*/


   pngSTPtr->sizePixelSL = numBytesSL;

   pngSTPtr->pixelAryUC =
      calloc(
         (pngSTPtr->numPixelSL + 8), /*+8 for long copy*/
         sizeof(unsigned char)
      );


   if(! pngSTPtr->pixelAryUC)
      return def_memErr_mkPng;
   return 0;
} /*setup_st_mkPng*/

/*-------------------------------------------------------\
| Fun10: mk_st_mkPng
|   - makes a st_mkPng struct on heap
| Input:
|   - widthUS:
|     o width of graph (0 for default)
|   - heightUS:
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
){
   struct st_mkPng *retHeapST = 0;

   retHeapST = malloc(sizeof(struct st_mkPng));

   if(! retHeapST)
      goto memErr_fun10;


   init_st_mkPng(retHeapST);

   if(widthUS)
      retHeapST->widthUS = widthUS;
   if(heightUS)
      retHeapST->heightUS = heightUS;

   retHeapST->numPixelSL =
      retHeapST->widthUS * retHeapST->heightUS;


   if(
      setup_st_mkPng(
         retHeapST,
         maxColUC
      )
   ) goto memErr_fun10;


   goto ret_fun10;

   memErr_fun10:;
      if(retHeapST)
         freeHeap_st_mkPng(retHeapST);
      retHeapST = 0;
      goto ret_fun10;

   ret_fun10:;
     return retHeapST;
} /*mk_st_mkPng*/

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
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun11 addBar_st_mkPng
   '   - adds a single pixel to a st_mkPng image
   '   o fun11 sec01:
   '     - variable declarations
   '   o fun11 sec02:
   '     - set up mask
   '   o fun11 sec02:
   '     - create color stamp
   '   o fun11 sec03:
   '     - add color
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun11 Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*doing byte copies to get my system down, latter I
   `   plan to add multi-byte steps
   */
   signed long startSL = xSL + (ySL * xSL);
   unsigned char bitUC = 0;  /*bit pixel is at*/
   unsigned char maskUC = 0;
   unsigned char *byteUCPtr = 0;
   

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun11 Sec02:
   ^   - set up mask
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(startSL >= pngSTPtr->numPixelSL)
      return def_overflow_mkPng;

   /*find bit for pixel*/
   bitUC = (startSL & pngSTPtr->pixPerByteUC);
   bitUC <<= pngSTPtr->shiftUC;
   /*Logic:
   `   - targeting second pixel (xx) in 11xx 1111
   `   - or second pixel in 4 bit (xxxx) 1111 xxxx
   `   - pixPos: startSL & pixPerByteUC:
   `     o pixel position of byte, deals with larg index's
   `     o 1bit: 7 & 8 = 7
   `     o 1bit: 6 & 8 = 6
   `     o 1bit: 5 & 8 = 5
   `     o 1bit: 4 & 8 = 4
   `     o 1bit: 3 & 8 = 3
   `     o 1bit: 2 & 8 = 2
   `     o 1bit: 1 & 8 = 1
   `     o 1bit: 0 & 8 = 0

   `     o 2bit: 0 & 4 = 0
   `     o 2bit: 3 & 4 = 3
   `     o 2bit: 2 & 4 = 2
   `     o 2bit: 1 & 4 = 1
   `     o 2bit: 0 & 4 = 0
   `
   `     o 4bit: 1 & 2 = 1
   `     o 4bit: 0 & 2 = 0
   `
   `     o 8bit: 1 & 1 = 0
   `
   `     o large index example 102 & 0xff = 2
   `   - shift << shiftUC
   `     o this gives the position of the first bit of
   `       the target pixel
   `     o 1bit: 8 << 0 = 8
   `     o 1bit: 7 << 0 = 7
   `     o 1bit: 6 << 0 = 6
   `     o 1bit: 5 << 0 = 5
   `     o 1bit: 4 << 0 = 4
   `     o 1bit: 3 << 0 = 3
   `     o 1bit: 2 << 0 = 2
   `     o 1bit: 1 << 0 = 1
   `     o 1bit: 0 << 0 = 0

   `     o 2bit: 3 << 1 = 6
   `     o 2bit: 2 << 1 = 4
   `     o 2bit: 1 << 1 = 2
   `     o 2bit: 0 << 1 = 0

   `     o 4bit: 1 << 2 = 4
   `     o 4bit: 0 << 2 = 0
   `
   `     o 8bit: 0 << 2 = 0
   */

   maskUC =
          ( (unsigned char) -1 )
       >> ( (sizeof(unsigned char) << 3) - bitUC );
   /*Logic:
   `   - (sizeof(uchar) << 3) - bitUC
   `     o sizeof(uchar) << 3 goes to 8
   `     o 8 - keepBit is target pixel bit + bits after
   `     o 1bit, 7th bit: 8 - 7 = 1
   `     o 1bit, 6th bit: 8 - 6 = 2
   `     o 1bit, 5th bit: 8 - 5 = 3
   `     o 1bit, 4th bit: 8 - 4 = 4
   `     o 1bit, 3rd bit: 8 - 3 = 5
   `     o 1bit, 2nd bit: 8 - 2 = 6
   `     o 1bit, 1st bit: 8 - 1 = 7
   `     o 1bit, 0th bit: 8 - 0 = 8

   `     o 2bit, 3rd bit: 8 - 6 = 2
   `     o 2bit, 2nd bit: 8 - 4 = 4
   `     o 2bit, 1st bit: 8 - 2 = 6
   `     o 2bit, 0th bit: 8 - 0 = 8
   `
   `     o 4bit, 1st bit: 8 - 4 = 4
   `     o 4bit, 0th bit: 8 - 0 = 8

   `     o 8bit, 0th bit: 8 - 0 = 8
   `   - (uchar) -1 >> lastKeepBit:
   `     o adds 11's until startBit (first target pixel
   `       bit)
   `     o 1bit, 7th bit: -1 << 1 = 1111 1110
   `     o 1bit, 6th bit: -1 << 2 = 1111 1100
   `     o 1bit, 5th bit: -1 << 3 = 1111 1000
   `     o 1bit, 4th bit: -1 << 4 = 1111 0000
   `     o 1bit, 3rd bit: -1 << 5 = 1110 0000
   `     o 1bit, 2nd bit: -1 << 6 = 1100 0000
   `     o 1bit, 1st bit: -1 << 7 = 1000 0000
   `     o 1bit, 0th bit: -1 << 8 = 0000 0000

   `     o 2bit, 3rd bit: -1 << 2 = 1111 1100
   `     o 2bit, 2nd bit: -1 << 4 = 1111 0000
   `     o 2bit, 1st bit: -1 << 6 = 1100 0000
   `     o 2bit, 0th bit: -1 << 8 = 0000 0000

   `     o 4bit, 1st bit: -1 << 4 = 1111 0000
   `     o 4bit, 0th bit: -1 << 8 = 0000 0000

   `     o 8bit, 0th bit: -1 << 8 = 0000 0000
   */

   maskUC |= ( ((unsigned char) -1) << bitUC );
      /*Logic
      `   - endMask: (uchar) -1 << bitUC:
      `     o 2bit, 3rd bit: -1 << 2 = 0011 1111
      `     o 2bit, 2nd bit: -1 << 4  = 0000 1111
      `     o 2bit, 1st bit: -1 << 6  = 0000 0011
      `     o 2bit, 0th bit: -1 << 8  = 0000 0000
      `
      `     o bitUC is last bit in target pixel, by
      `       1 by this I get ones for every bit after
      `       bitUC, but zero for bitUC and before
      `   - maskUC | endMask:
      `     o sets the ending bits of maskUC to 1,
      `       this leaves only the target bits as 0
      */

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun11 Sec03:
   ^   - add color
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   byteUCPtr =
        pngSTPtr->pixelAryUC
      + (startSL >> pngSTPtr->shiftUC)
      + (ySL * def_extraBytes_mkPng);
      /*ySL * extraBytes
      `   - gives number extra bytes need to skip past at
      `     end of rows (number rows * bytes at row end)
      ` startSL >> shiftUC
      `   - byte I need to shift two (if no extra bytes)
      ` (startSL >> shiftUC) + (ySL * extraBytes)
      `   - byte I need to change
      */


   *byteUCPtr &= maskUC;          /*clear target bits*/
   maskUC = (colUC << bitUC);     /*add color to mask*/
   *byteUCPtr |= maskUC;          /*add color*/

   return 0;
}  /*addBar_st_mkPng*/

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
|   - widthUS:
|     o width in pixels of bar
|   - heightUS:
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
   signed long widthUS,       /*pixels wide of bar*/
   signed long heightUS,      /*pixels high of bar*/
   signed char colUC          /*color of bar*/
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun12 addBar_st_mkPng
   '   - adds a bar to a st_mkPng image
   '   o fun12 sec01:
   '     - variable declarations
   '   o fun12 sec02:
   '     - setup + create color stamp
   '   o fun12 sec03:
   '     - make bar
   '   o fun12 sec04:
   '     - return result
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun12 Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   ul_mkPng maskStartUL = 0;/*mask to clear start*/
   ul_mkPng colStartUL = 0; /*color stamp for mask start*/

   ul_mkPng maskEndUL = 0;  /*ending mask to clear bits*/
   ul_mkPng colEndUL = 0;   /*color stamp for mask end*/

   ul_mkPng colByteUL = 0;  /*byte full of color*/
   ul_mkPng *pixAryUL = (ul_mkPng *) pngSTPtr->pixelAryUC;
     /*pixel array as long*/

   signed long startSL = 0;    /*start of bar on row*/
   signed long endSL = 0;      /*start of bar on row*/
   signed long cpIndexSL = 0;  /*index copying at*/
   signed long lenRowSL = 0;   /*length of one row*/

   unsigned char bitUC = 0;    /*for building mask*/
   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun12 Sec02:
   ^   - setup + create color stamp
   ^   o fun12 sec02 sub01:
   ^     - check if have overflow and find row length
   ^   o fun12 sec02 sub02:
   ^     - find start, end, and setup masks
   ^   o fun12 sec02 sub03:
   ^     - setup color stamps
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun12 Sec02 Sub01:
   *   - check if have overflow and find row length
   \*****************************************************/

   startSL = (signed long) widthUS;
   startSL *= ySL;       /*gives pixels till row*/
   startSL += xSL;       /*gives frist pixel*/
   startSL += widthUS;   /*gives last pixel*/

   if( startSL > pngSTPtr->numPixelSL)
      goto overflow_fun12;


   /*this is safe because setup enforces that width must
   `   be a multiple of bits per byte
   */
   lenRowSL = pngSTPtr->widthUS >> pngSTPtr->shiftUC;
   lenRowSL += def_extraBytes_mkPng; /*6 bytes at end*/
   lenRowSL >>= sizeof(ul_mkPng); /*bytes to longs*/

   /*****************************************************\
   * Fun12 Sec02 Sub02:
   *   - find start, end, and setup masks
   \*****************************************************/

   startSL = xSL;
   endSL = startSL + widthUS;

   bitUC =
        startSL
      & (pngSTPtr->pixPerByteUC * sizeof(ul_mkPng));
   bitUC <<= (pngSTPtr->shiftUC * sizeof(ul_mkPng));

   /*build mask*/
   maskStartUL =
         ((ul_mkPng) -1)
      >> ((sizeof(ul_mkPng) << 3) - bitUC);

   maskEndUL = ((ul_mkPng) -1) << bitUC;


   /*convert start and end to bytes*/
   startSL >>= (pngSTPtr->shiftUC * sizeof(ul_mkPng));
   endSL >>= (pngSTPtr->shiftUC * sizeof(ul_mkPng));


   /*move to first pixel to change position*/
   pixAryUL +=
         (ySL * pngSTPtr->widthUS)
      >> (pngSTPtr->pixPerByteUC * sizeof(ul_mkPng));
      /*moves to y coordinate (ingores extra bytes)*/

   pixAryUL += ( ySL * def_extraBytes_mkPng );
      /*adjusts for extra bytes at row end*/

   /*****************************************************\
   * Fun12 Sec02 Sub03:
   *   - setup color stamps
   \*****************************************************/

   for(
      bitUC = 0;
      bitUC < (pngSTPtr->pixDepthUC  * sizeof(ul_mkPng));
      bitUC += pngSTPtr->pixDepthUC
   ) colByteUL |= ( ((ul_mkPng) colUC) << bitUC );

   colStartUL = ~maskStartUL;
   colStartUL &= colByteUL;

   colEndUL = ~maskEndUL;
   colEndUL &= colByteUL;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun12 Sec03:
   ^   - make bar
   ^   o fun12 sec03 sub01:
   ^     - one limb coloring
   ^   o fun12 sec03 sub02:
   ^     - two limb coloring
   ^   o fun12 sec03 sub03:
   ^     - three or more limb coloring
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
	* Fun12 Sec03 Sub01:
   *   - one limb coloring
   \*****************************************************/

   if(startSL == endSL)
   { /*If: only coloring one limb*/

      for(
        xSL = ySL;
        xSL < heightUS;
        ++xSL
      ){ /*Loop: apply color*/
         pixAryUL[startSL] &= maskStartUL;
         pixAryUL[startSL] |= colStartUL;

         pixAryUL += lenRowSL;
      }  /*Loop: apply color*/

   } /*If: only coloring one limb*/

   /*****************************************************\
	* Fun12 Sec03 Sub02:
   *   - two limb coloring
   \*****************************************************/

   else if((startSL + 1) == endSL)
   { /*Else If: only coloring two limbs*/

      for(
        xSL = ySL;
        xSL < heightUS;
        ++xSL
      ){ /*Loop: apply color*/
         pixAryUL[startSL] &= maskStartUL;
         pixAryUL[startSL] |= colStartUL;

         pixAryUL[endSL] &= maskEndUL;
         pixAryUL[endSL] |= colEndUL;

         pixAryUL += lenRowSL;
      }  /*Loop: apply color*/

   } /*Else If: only coloring two limbs*/

   /*****************************************************\
   * Fun12 Sec03 Sub03:
   *   - three or more limb coloring
   \*****************************************************/

   else
   { /*Else If: coloring three or more limbs*/

      for(
        xSL = ySL;
        xSL < heightUS;
        ++xSL
      ){ /*Loop: apply color*/
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
   ^ Fun12 Sec04:
   ^   - return result
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   return 0;

   overflow_fun12:;
      return def_overflow_mkPng;
}  /*addBar_st_mkPng*/

/*-------------------------------------------------------\
| Fun13: crc32_mkPng
|   - find new crc value after adding next byte
|   - copied from misc0110's libattpng repository, I
|     really do not know whats going on here
| Input:
|   - byte:
|     o byte to convert (
|   - crc32:
|     o current value of input crc
|     o unsigned int is always 32 bit on 32/64 bit OSs
|       or at least I have yet to see a case it is
|       not
| Output:
|   - Returns:
|     o updated crc value
\-------------------------------------------------------*/
#define crc32_mkPng(byte, crc32) ( crc32_mkPngUL[ ( ((unsigned int) (crc32)) ^ ((unsigned char) (byte)) ) & 255 ] ^ ( ((unsigned int) (crc32)) >> 8 ) )
/*not sure what is going on here, this is the best
`   I can understand it by, but it shows my ingorence
`   - invert: crc32 ^ byte:
`   - to8bit: invert & 255:
`     o converts inverted value to value in lookup table
`   - newVal: crc32_mkPngUL[to8bit]:
`     o find the new crc32 value in the lookup table
`   - clear: crc32 >> 8:
`     o clears old 8 bit value from crc32
`   - newVal ^ clear:
`     o inverts int lookup by remaing crc32 value
*/

/*-------------------------------------------------------\
| Fun14: strToCrc32_mkPng
|   - find new crc value for string
| Input:
|   - crcStr:
|     o string to convert
|   - lenSL:
|     o length of crcStr
|   - crc32UI:
|     o current value of input crc
|     o unsigned int is always 32 bit on 32/64 bit OSs
|       or at least I have yet to see a case it is
|       not
| Output:
|   - Returns:
|     o updated crc value
\-------------------------------------------------------*/
unsigned int
strToCrc32_mkPng(
   signed char *crcStr,
   signed long lenSL,
   unsigned int crc32UI
){
   signed long slIndex = 0;

   while(slIndex < lenSL)
      crc32UI =
         crc32_mkPng(
            crcStr[slIndex++],
            crc32UI
         );
   return crc32UI;
} /*strToCrc32_mkPng*/

/*-------------------------------------------------------\
| Fun15: bigendInt_mkPng
|   - converts unit to bigendin
|   - copied from misc0110's libattpng repository
| Input:
|   - valUI:
|     o int to convert
| Output:
|   - Returns:
|     o bigendin number as unsigned int
\-------------------------------------------------------*/
/*#define bigendInt_mkPng(valUI) ( ( (((unsigned int) (valUI)) >> 24) & 0xff ) | ( (((unsigned int) (valUI)) << 8) & 0xff0000 ) | ((( (unsigned int) (valUI) ) >> 8) & 0xff00 ) | ( (((unsigned int) (valUI)) << 24) & 0xff000000 ) )*/
unsigned int bigendInt_mkPng(valUI)
{
   unsigned int tmpUI = 0;

   tmpUI = ((((unsigned int) (valUI)) >> 24) & 0xff );
   tmpUI |= ( (((unsigned int) (valUI)) << 8) & 0xff0000 );
   tmpUI |= ((( (unsigned int) (valUI) ) >> 8) & 0xff00 );
   tmpUI |= ( (((unsigned int) (valUI)) << 24) & 0xff000000 );
   return tmpUI;
}
/*Logic
`   - ex 0xfe0dc123
`   - upper: (valUI >> 24) & 0xff
`     o move last 8 bits to first part of number,
`       shifts moves bits to start, while 0xff clears
`       any extra bits
`       * 0xfe0dc123 >> 24 = 0x000000fe
`       * 0x000000fe & 0xff = 0x000000fe
`   - lowMid: (valUI << 8) & 0xff0000
`     o moves upper middle bits up 8 bits
`       * 0xfe0dc123 << 8 = 0x0dc12300
`       * 0x0dc12300 & 0xff0000 = 0x00c10000
`   - upMid: (valUI >> 8) & 0xff00
`     o moves lower middle bits down 8 bits
`       * 0xfe0dc123 >> 8 = 0x00fe0dc1
`       * 0x00fe0dc10 & 0xff00 = 0x0000dc00
`   - lower: (valUI << 24) & 0xff000000
`     o moves lower two bits to start
`       * 0xfe0dc123 << 24 = 0x23000000
`       * 0x23000000 & 0xff000000 = 0x23000000
`   - merge bits: upper | lowMid | upMid | lower
`     o merges all shifts to get bigend 32bit number
`       * 0x000000fe| 0x00c10000 | 0x0000dc00 | 0x23000000
`         = 0x23c1dcfe
*/

/*-------------------------------------------------------\
| Fun16: addUint_mkPng
|   - adds a uint to a png buffer
| Input:
|   - buffStr:
|     o c-string to add uint to 
|   - inUI:
|     o unsigned int to add
|   - indexUI:
|     o index to add inUI to in buffStr
| Output:
|   - Modifies:
|     o buffStr to have inUI (converted to bigendin)
|   - Returns:
|     o next open index in buffStr
\-------------------------------------------------------*/
unsigned int
addUint_mkPng(
   unsigned char *buffStr,
   unsigned int inUI,
   unsigned int indexUI
){
   inUI = bigendInt_mkPng(inUI);

   buffStr[indexUI++] = ((unsigned char *) &inUI)[0];
   buffStr[indexUI++] = ((unsigned char *) &inUI)[1];
   buffStr[indexUI++] = ((unsigned char *) &inUI)[2];
   buffStr[indexUI++] = ((unsigned char *) &inUI)[3];

   return indexUI;
} /*addUint_mkPng*/

/*-------------------------------------------------------\
| Fun17: addIhdr_st_mkPng
|   - add the IHDR header to a st_mkPng struct
| Input:
|   - pngSTPtr:
|     o pointer to st_mkPng struct to add IHDR header to
|   - setSTPtr:
|     o pointer to set_mkPng struct with settings
|     o currently not used, here for future
| Output:
|   - Modifies:
|     o ihdrStr in pngSTPtr to have IHDR png header
\-------------------------------------------------------*/
void
addIhdr_st_mkPng(
   struct st_mkPng *pngSTPtr,  /*to add header to*/
   struct set_mkPng *setSTPtr  /*print settings*/
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun17 TOC:
   '   - add the IHDR header to a st_mkPng struct
   '   o fun17 sec01:
   '    - variable declarations
   '   o fun17 sec02:
   '    - add length of IHDR chunk
   '   o fun17 sec03:
   '    - add name of IHDR chunk
   '   o fun17 sec04:
   '    - add image dimesions to IHDR header
   '   o fun17 sec05:
   '    - add color pallete and other values (leave 0)
   '   o fun17 sec06:
   '    - add crc32 to IHDR chunk
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun17 Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   unsigned int tmpUI = 0;
   unsigned int indexUI = 0;
   unsigned int crc32UI = 0xffffffff;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun17 Sec02:
   ^   - add length of IHDR chunk
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   indexUI =
      addUint_mkPng(
         pngSTPtr->ihdrStr,
         13,
         indexUI
      ); /*CRC does not include chunk length section*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun17 Sec03:
   ^   - add name of IHDR chunk
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*add IHDR header*/
   crc32UI =
      strToCrc32_mkPng(
         (signed char *) "IHDR",
         4,
         crc32UI
      );

   pngSTPtr->ihdrStr[indexUI++] = 'I';
   pngSTPtr->ihdrStr[indexUI++] = 'H';
   pngSTPtr->ihdrStr[indexUI++] = 'D';
   pngSTPtr->ihdrStr[indexUI++] = 'R';


   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun17 Sec04:
   ^   - add image dimesions to IHDR header
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*add image width to header*/
   tmpUI = bigendInt_mkPng(pngSTPtr->widthUS);
   crc32UI =
      strToCrc32_mkPng(
         (signed char *) &tmpUI,
         4,
         crc32UI
      );
   pngSTPtr->ihdrStr[indexUI++] =
      ((unsigned char *) &tmpUI)[0];
   pngSTPtr->ihdrStr[indexUI++] =
      ((unsigned char *) &tmpUI)[1];
   pngSTPtr->ihdrStr[indexUI++] =
      ((unsigned char *) &tmpUI)[2];
   pngSTPtr->ihdrStr[indexUI++] =
      ((unsigned char *) &tmpUI)[3];


   /*add image height to header*/
   tmpUI = bigendInt_mkPng(pngSTPtr->heightUS);
   crc32UI =
      strToCrc32_mkPng(
         (signed char *) &tmpUI,
         4,
         crc32UI
      );
   pngSTPtr->ihdrStr[indexUI++] =
      ((unsigned char *) &tmpUI)[0];
   pngSTPtr->ihdrStr[indexUI++] =
      ((unsigned char *) &tmpUI)[1];
   pngSTPtr->ihdrStr[indexUI++] =
      ((unsigned char *) &tmpUI)[2];
   pngSTPtr->ihdrStr[indexUI++] =
      ((unsigned char *) &tmpUI)[3];

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun17 Sec05:
   ^   - add color pallete and other values (leave 0)
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*add bit depth to header*/
   crc32UI = crc32_mkPng(pngSTPtr->pixDepthUC, crc32UI);
   pngSTPtr->ihdrStr[indexUI++] = pngSTPtr->pixDepthUC;

   /*specifiy using index color pallete*/
   crc32UI = crc32_mkPng(
      (unsigned char) 3,
      crc32UI
   );
   pngSTPtr->ihdrStr[indexUI++] = 3;

   crc32UI = crc32_mkPng((unsigned char) 0, crc32UI);
   pngSTPtr->ihdrStr[indexUI++] = 0;
      /*compression; 0 is only option*/

   crc32UI = crc32_mkPng((unsigned char) 0, crc32UI);
   pngSTPtr->ihdrStr[indexUI++] = 0;
      /*filter; 0 is only option*/

   crc32UI = crc32_mkPng((unsigned char) 0, crc32UI);
   pngSTPtr->ihdrStr[indexUI++] = 0;
      /*0 = no interlace; 1 is adam 7*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun17 Sec06:
   ^   - add crc32 to IHDR chunk
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*add crc to IHDR header (end of header)*/
   crc32UI = ~crc32UI;

   indexUI =
      addUint_mkPng(
         pngSTPtr->ihdrStr,
         crc32UI,
         indexUI
      );

   pngSTPtr->lenIhdrUC = indexUI;
} /*pIHDR_mkPng*/

/*-------------------------------------------------------\
| Fun18: addPallete_st_mkPng
|   - add the pallete (PLTE) header to a st_mkPng struct
| Input:
|   - pngSTPtr:
|     o pointer to st_mkPng struct to add PLTE header to
| Output:
|   - Modifies:
|     o palleteStr in pngSTPtr to have pallete png header
\-------------------------------------------------------*/
void
addPallete_st_mkPng(
   struct st_mkPng *pngSTPtr  /*to add header to*/
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun18 TOC:
   '   - add the pallete (PLTE) header to st_mkPng struct
   '   o fun18 sec01:
   '     - variable declarations
   '   o fun18 sec02:
   '     - add length for PLTE chunk
   '   o fun18 sec03:
   '     - add PLTE chunk header
   '   o fun18 sec04:
   '     - add color pallete for PLTE chunk
   '   o fun18 sec05:
   '     - add crc32 to PLTE chunk
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun18 Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   unsigned int uiCol = 0;
   unsigned int uiIndex = 0;
   unsigned int crc32UI = 0xffffffff;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun18 Sec02:
   ^   - add length for PLTE chunk
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*add color pallete size to header*/
   uiCol = 4 + (pngSTPtr->numColUC * 3);
      /*4 for header, * 3 for color pallete*/

   uiIndex =
      addUint_mkPng(
         pngSTPtr->ihdrStr,
         uiCol,
         uiIndex
      ); /*CRC does not include chunk length section*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun18 Sec03:
   ^   - add PLTE chunk header
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*add PLTE header*/
   crc32UI =
      strToCrc32_mkPng(
         (signed char *) "PLTE",
         4,
         crc32UI
   );
   pngSTPtr->palleteStr[uiIndex++] = 'P';
   pngSTPtr->palleteStr[uiIndex++] = 'L';
   pngSTPtr->palleteStr[uiIndex++] = 'T';
   pngSTPtr->palleteStr[uiIndex++] = 'E';

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun18 Sec04:
   ^   - add color pallete for PLTE chunk
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*add color pallete*/
   uiCol = 0;
   while(uiCol < pngSTPtr->numColUC)
   { /*Loop: add color pallete to header*/
      crc32UI =
         crc32_mkPng(
            pngSTPtr->redAryUC[uiCol],
            crc32UI
         ); /*add red color value to crc*/
      pngSTPtr->palleteStr[uiIndex++] =
         pngSTPtr->redAryUC[uiCol];

      crc32UI =
         crc32_mkPng(
            pngSTPtr->bluAryUC[uiCol],
            crc32UI
         ); /*add blue color value to crc*/
      pngSTPtr->palleteStr[uiIndex++] =
         pngSTPtr->bluAryUC[uiCol];

      crc32UI =
         crc32_mkPng(
            pngSTPtr->greAryUC[uiCol],
            crc32UI
         ); /*add green color value to crc*/
      pngSTPtr->palleteStr[uiIndex++] =
         pngSTPtr->greAryUC[uiCol++];
   } /*Loop: add color pallete to header*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun18 Sec05:
   ^   - add crc32 to PLTE chunk
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*add crc to PTLE header (end of header)*/
   crc32UI = ~crc32UI;

   uiIndex =
      addUint_mkPng(
         pngSTPtr->ihdrStr,
         crc32UI,
         uiIndex
      ); /*CRC does not include chunk length section*/

   pngSTPtr->lenPalSS = (signed short) uiIndex;
} /*addPallete_st_mkPng*/

/*-------------------------------------------------------\
| Fun19: addImage_st_mkPng
|   - add image IDAT header
|   - not fully sure on this one
|   - copied from misc0110's libattpng repository
| Input:
|   - pngSTPtr:
|     o pointer to st_mkPng struct to add IDAT header to
|     o also has image to add
|   - setSTPtr:
|     o pointer to set_mkPng struct with settings
| Output:
|   - Modifies:
|     o idatStr in pngSTPtr to have IDAT header
\-------------------------------------------------------*/
void
addImage_st_mkPng(
   struct st_mkPng *pngSTPtr,/*add header to + has image*/
   struct set_mkPng *setSTPtr/*print settings*/
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun19 TOC:
   '   - add image IDAT header
   '   o fun19 sec01:
   '     - variable declarations
   '   o fun19 sec02:
   '     - write IDAT header
   '   o fun19 sec03:
   '     - add image to image header
   '   o fun19 sec04:
   '     - add adler and crc32 to image header
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\
   ^ Fun19 Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   unsigned short tmpUS = 0;
   unsigned short sumOneUS = 1;
   unsigned short sumTwoUS = 0;
   unsigned int indexUI = 0;

   signed long pixelSL = 0;
   signed long endSL = 0;

   unsigned int crc32UI = 0xffffffff;
   unsigned short pixPerRowUS =
      1 + (unsigned short) pngSTPtr->widthUS;

   unsigned short sizeUS =
       pngSTPtr->heightUS / pngSTPtr->pixPerByteUC;
      /*think recording number of bytes per column*/

   /*<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\
   ^ Fun19 Sec02:
   ^   - write IDAT header
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   indexUI =
      addUint_mkPng(
         pngSTPtr->ihdrStr,
         (unsigned int) pngSTPtr->usedBytesSL + 6,
         indexUI
      ); /*CRC does not include chunk length section*/
   
   crc32UI =
      strToCrc32_mkPng(
         (signed char *) "IDAT\170\332",
         6,
         crc32UI
      );
   
   pngSTPtr->idatStr[indexUI++] = 'I';
   pngSTPtr->idatStr[indexUI++] = 'D';
   pngSTPtr->idatStr[indexUI++] = 'A';
   pngSTPtr->idatStr[indexUI++] = 'T';
   pngSTPtr->idatStr[indexUI++] = '\170';
   pngSTPtr->idatStr[indexUI++] = '\332';

   pngSTPtr->lenIdatUC = indexUI;

   /*<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\
   ^ Fun19 Sec03:
   ^   - add image to image header
   ^   o fun19 sec03 sub01:
   ^     - start loop & deal with end of rows
   ^   o fun19 sec03 sub02:
   ^     - add pixe to row
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/
   
   /*****************************************************\
   * Fun19 Sec03 Sub01:
   *   - start loop & deal with end of rows
   *   o fun19 sec03 sub01 cat01:
   *     - start loop & deal with non-last row end of row
   *   o fun19 sec03 sub01 cat02:
   *     - deal with last row (end of file row)
   \*****************************************************/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun19 Sec03 Sub01 Cat01:
   +   - start loop & deal with non-last row end of row
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   endSL = pngSTPtr->numPixelSL >> pngSTPtr->shiftUC;
      /*number bytes in png image*/
   endSL += pngSTPtr->heightUS * def_extraBytes_mkPng;
      /*add in trailing bytes at end of each row*/

   indexUI = 0;
   pixelSL = 0;

   while(pixelSL < endSL)
   { /*Loop: add pixels to crc32*/
      
      if(! indexUI)
      { /*If: line header*/
         if(pixelSL > (signed long) pixPerRowUS)
         { /*If: not last row*/
            pngSTPtr->pixelAryUC[pixelSL++] = '\0';
            crc32UI = crc32_mkPng('\0', crc32UI);
             /*add pixel to crc32 value*/

            /*bytes per pixel (index so only 1) */
            tmpUS = 1; /*pixel is in entire byte*/
            crc32UI =
               strToCrc32_mkPng(
                  (signed char *) &tmpUS,
                  2,
                  crc32UI
               );

            pngSTPtr->pixelAryUC[pixelSL++] =
               ((unsigned char *) &tmpUS)[0];
            pngSTPtr->pixelAryUC[pixelSL++] =
               ((unsigned char *) &tmpUS)[1];


            tmpUS = ~1; /*pixel is in entire byte*/
            crc32UI =
               strToCrc32_mkPng(
                  (signed char *) &tmpUS,
                  2,
                  crc32UI
               );

            pngSTPtr->pixelAryUC[pixelSL++] =
               ((unsigned char *) &tmpUS)[0];
            pngSTPtr->pixelAryUC[pixelSL++] =
               ((unsigned char *) &tmpUS)[1];

            /*no filter marker*/
            pngSTPtr->pixelAryUC[pixelSL++] = 0;
            crc32UI = crc32_mkPng(0, crc32UI);
                /*row header; no pixel*/

            /*add no-fitler to adler*/
            sumOneUS += 0;
            sumOneUS %= def_adler_mkPng;

            sumTwoUS += sumOneUS;
            sumTwoUS %= def_adler_mkPng;

            --sizeUS;
         } /*If: not last row*/

         /*++++++++++++++++++++++++++++++++++++++++++++++\
         + Fun19 Sec03 Sub01 Cat02:
         +   - deal with last row (end of file row)
         \++++++++++++++++++++++++++++++++++++++++++++++*/

         else
         { /*Else: is last row*/
            pngSTPtr->pixelAryUC[pixelSL++] = '\1';
            crc32UI = crc32_mkPng('\1', crc32UI);
               /*add pixel to crc32 value*/

            /*write offset (number bytes in last row)*/
            tmpUS = sizeUS; /*pixel is in entire byte*/
            crc32UI =
               strToCrc32_mkPng(
                  (signed char *) &tmpUS,
                  2,
                  crc32UI
               );

            pngSTPtr->pixelAryUC[pixelSL++] =
               ((unsigned char *) &tmpUS)[0];
            pngSTPtr->pixelAryUC[pixelSL++] =
               ((unsigned char *) &tmpUS)[1];


            tmpUS = ~sizeUS; /*pixel is in entire byte*/
            crc32UI =
               strToCrc32_mkPng(
                  (signed char *) &tmpUS,
                  2,
                  crc32UI
               );

            pngSTPtr->pixelAryUC[pixelSL++] =
               ((unsigned short *) &tmpUS)[0];
            pngSTPtr->pixelAryUC[pixelSL++] =
               ((unsigned short *) &tmpUS)[1];


            pngSTPtr->pixelAryUC[pixelSL++] = 0;
            crc32UI = crc32_mkPng(0, crc32UI);
                /*row header; no pixel*/

            sumOneUS += sizeUS;
            sumOneUS %= def_adler_mkPng;

            sumTwoUS += sumOneUS;
            sumTwoUS %= def_adler_mkPng;

            --sizeUS;
         } /*Else: is last row*/
      } /*If: line header*/

      /**************************************************\
      * Fun19 Sec03 Sub02:
      *   - add pixe to row
      \**************************************************/

      crc32UI =
         crc32_mkPng(
            pngSTPtr->pixelAryUC[pixelSL],
            crc32UI
         ); /*add pixel to crc32 value*/


      /*add pixel to addler sum??*/
      sumOneUS += pngSTPtr->pixelAryUC[pixelSL];
      sumOneUS %= def_adler_mkPng;

      sumTwoUS += sumOneUS;
      sumTwoUS %= def_adler_mkPng;

      /*move to next pixel*/
      --sizeUS;
      indexUI = (indexUI + 1) % pngSTPtr->widthUS;

      ++pixelSL;
   }  /*Loop: add pixels to crc32*/

   /*<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\
   ^ Fun19 Sec04:
   ^   - add adler and crc32 to image header
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*add adler sum to crc32*/
   sumOneUS %= def_adler_mkPng;
   sumTwoUS %= def_adler_mkPng;
   
   indexUI =
        (((unsigned int) sumTwoUS) << 16)
      | ((unsigned int) sumOneUS);

   crc32UI =
      strToCrc32_mkPng(
         (signed char *) &indexUI,
         4,
         crc32UI
      );
   
   pngSTPtr->pixelAryUC[pixelSL++] =
      ((unsigned char *) &indexUI)[0];
   pngSTPtr->pixelAryUC[pixelSL++] =
      ((unsigned char *) &indexUI)[1];
   pngSTPtr->pixelAryUC[pixelSL++] =
      ((unsigned char *) &indexUI)[2];
   pngSTPtr->pixelAryUC[pixelSL++] =
      ((unsigned char *) &indexUI)[3];


   /*add crc to IDAT header (end of header)*/
   crc32UI = ~crc32UI;
   crc32UI = bigendInt_mkPng(crc32UI);

   pngSTPtr->pixelAryUC[pixelSL++] =
      ((unsigned char *) &crc32UI)[0];
   pngSTPtr->pixelAryUC[pixelSL++] =
      ((unsigned char *) &crc32UI)[1];
   pngSTPtr->pixelAryUC[pixelSL++] =
      ((unsigned char *) &crc32UI)[2];
   pngSTPtr->pixelAryUC[pixelSL++] =
      ((unsigned char *) &crc32UI)[3];
} /*addImage_st_mkPng*/

/*-------------------------------------------------------\
| Fun20: addIend_st_mkPng
|   - add end header (IEND) for png 
| Input:
|   - pngSTPtr:
|     o pointer to st_mkPng struct to add IEND header to
| Output:
|   - Modifies:
|     o endStr in pngSTPtr to have IEND header
\-------------------------------------------------------*/
void
addIend_st_mkPng(
   struct st_mkPng *pngSTPtr  /*to add header to*/
){
   unsigned int uiIndex = 0;
   unsigned int crc32UI = 0xffffffff;

   uiIndex =
      addUint_mkPng(
         pngSTPtr->ihdrStr,
         0,     /*IEND is always length 0*/
         uiIndex
      ); /*CRC does not include chunk length section*/

   crc32UI =
      strToCrc32_mkPng(
         (signed char *) "IEND",
         4,
         crc32UI
      );

   pngSTPtr->endStr[uiIndex++] = 'I';
   pngSTPtr->endStr[uiIndex++] = 'E';
   pngSTPtr->endStr[uiIndex++] = 'N';
   pngSTPtr->endStr[uiIndex++] = 'D';

   /*IEND has no data*/

   /*add crc to PTLE header (end of header)*/
   crc32UI = ~crc32UI;
   uiIndex =
      addUint_mkPng(
         pngSTPtr->endStr,
         crc32UI,
         uiIndex
      ); /*CRC does not include chunk length section*/

   pngSTPtr->lenEndUC = uiIndex;
} /*addIend_st_mkPng*/

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
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun20 TOC:
   '   - prints a png to output file
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   addIhdr_st_mkPng(
      pngSTPtr,
      setSTPtr
   );

   addPallete_st_mkPng(pngSTPtr);
   /*not including transparency*/

   addImage_st_mkPng(
      pngSTPtr,
      setSTPtr
   );

   addIend_st_mkPng(pngSTPtr);

   fwrite(
      pngSTPtr->pngStr,
      pngSTPtr->lenPngUC,
      sizeof(unsigned char),
      (FILE *) outFILE
   );

   fwrite(
      pngSTPtr->ihdrStr,
      pngSTPtr->lenIhdrUC,
      sizeof(unsigned char),
      (FILE *) outFILE
   );

   fwrite(
      pngSTPtr->palleteStr,
      pngSTPtr->lenPalSS,
      sizeof(unsigned char),
      (FILE *) outFILE
   );

   fwrite(
      pngSTPtr->idatStr,
      pngSTPtr->lenIdatUC,
      sizeof(unsigned char),
      (FILE *) outFILE
   );

   fwrite(
      pngSTPtr->pixelAryUC,
      pngSTPtr->usedBytesSL,
      sizeof(unsigned char),
      (FILE *) outFILE
   );

   fwrite(
      pngSTPtr->endStr,
      pngSTPtr->lenEndUC,
      sizeof(unsigned char),
      (FILE *) outFILE
   );
} /*print_st_mkPng*/
