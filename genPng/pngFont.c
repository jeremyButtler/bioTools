/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' pngFont SOF: Start Of File
'   - handels loading and drawing png fonts
'   - I am avoidng using longs here in the drawing step to
'     avoid additional complexity
'   o header:
'     - libraries
'   o .h st01: font_pngFont
'     - holds the font to draw into the png
'   o fun01: blank_font_pngFont
'     - blanks a font_pngFont struct
'   o fun02: init_font_pngFont
'     - initializes a font_pngFont struct
'   o fun03: freeStack_font_pngFont
'     - frees all variables in a font_pngFont struct
'   o fun04: freeHeap_font_pngFont
'     - frees a font_pngFont struct
'   o fun05: getFont_font_pngFont
'     - get a font from my ascii font file
'   o fun06: measureText_font_pngFont
'     - measure how many pixels long a cstring is
'   o fun07: drawHorzText_font_pngFont
'     - draw horizontal text to a png
'   o fun08: drawVertText_font_pngFont
'     - draw text vertically to a png
'   o fun09: fontToC_font_pngFont
'     - convert a font_pngFont to to .c and .h files
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*-------------------------------------------------------\
| Header:
|   - libraries
\-------------------------------------------------------*/

#ifdef PLAN9
   #include <u.h>
   #include <libc.h>
#else
   #include <stdlib.h>
#endif

#include <stdio.h>

#include "pngFont.h"
#include "../genLib/base10str.h"
#include "../genLib/ulCp.h"
#include "mkPng.h"

/*only using the .h files*/
#include "../genLib/64bit.h"

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\
! Hidden libraries:
!   - .c  #include "../genLib/endin.h"
!   - .c  #include "../genLib/checksum.h"
\%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

/*-------------------------------------------------------\
| Fun01: blank_font_pngFont
|   - blanks a font_pngFont struct
| Input:
|   - fontSTPtr:
|     o font_pngFont struct pointer to blank
| Output:
|   - Modifies:
|     o all elements in pixAryUC and widthArySS to be
|       set to null/0
|     o frees all pionters in pixAryUC
\-------------------------------------------------------*/
void
blank_font_pngFont(
   struct font_pngFont *fontSTPtr
){
   signed int siPos = 0;

   if(! fontSTPtr)
      return;

   for(siPos=0; siPos < def_numAsciiChar_pngFont; ++siPos)
   { /*Loop: free all characters*/
      if(fontSTPtr->pixAryUC[siPos])
         free(fontSTPtr->pixAryUC[siPos]);
   } /*Loop: free all characters*/

   init_font_pngFont(fontSTPtr);
      /*calling init or freeing memory is unusal in a
      `  blank function for me. However, I do not want to
      `  deal with keeping track of memory size for each
      `  character. So, it is easier to free on blank and
      `  reallocate later
      */
} /*blank_font_pngFont*/

/*-------------------------------------------------------\
| Fun02: init_font_pngFont
|   - initializes a font_pngFont struct
| Input:
|   - fontSTPtr:
|     o font_pngFont struct pointer to initialize
| Output:
|   - Modifies:
|     o all elements in pixAryUC and widthArySS to be
|       set to null/0
\-------------------------------------------------------*/
void
init_font_pngFont(
   struct font_pngFont *fontSTPtr
){
   signed int siPos = 0;

   if(! fontSTPtr)
      return;

   fontSTPtr->heightSS = 0;
   fontSTPtr->gapSS = 0;

   for(siPos=0; siPos < def_numAsciiChar_pngFont; ++siPos)
   { /*Loop: set all values to 0*/
      fontSTPtr->pixAryUC[siPos] = 0;
      fontSTPtr->widthArySS[siPos] = 0;
      fontSTPtr->lenArySS[siPos] = 0;
   } /*Loop: set all values to 0*/

   /*I would often call a blank function, but in this case
   `  it is more efficent to initialize
   */
} /*init_font_pngFont*/

/*-------------------------------------------------------\
| Fun03: freeStack_font_pngFont
|   - frees all variables in a font_pngFont struct
| Input:
|   - fontSTPtr:
|     o font_pngFont struct pointer with variables to free
| Output:
|   - Modifies:
|     o frees all variables and initialize
\-------------------------------------------------------*/
void
freeStack_font_pngFont(
   struct font_pngFont *fontSTPtr
){
   if(! fontSTPtr)
      return;

   blank_font_pngFont(fontSTPtr);
      /*the blank function frees and initializes*/
} /*freeStack_font_pngFont*/

/*-------------------------------------------------------\
| Fun04: freeHeap_font_pngFont
|   - frees a font_pngFont struct
| Input:
|   - fontSTPtr:
|     o font_pngFont struct pointer to free
| Output:
|   - Modifies:
|     o frees hte fontSTPtr struct, you must set the
|       pointer to 0/null
\-------------------------------------------------------*/
void
freeHeap_font_pngFont(
   struct font_pngFont *fontSTPtr
){
   if(! fontSTPtr)
      return;

   freeStack_font_pngFont(fontSTPtr);
      /*the blank function frees and initializes*/
   free(fontSTPtr);
} /*freeHeap_font_pngFont*/

/*-------------------------------------------------------\
| Fun05: getFont_font_pngFont
|   - get a font from my ascii font file
| Input:
|   - fontSTPtr:
|     o font_pngFont struct pionter to get the new font
|   - fontFILE:
|     o FILE pointer to file to get the font from
| Output:
|   - Modifies:
|     o fontSTPtr to have the new font
|     o fontFILE to be at the end of the file
|   - Returns:
|     o 0 for no errors
|     o -1 for no height error
|     o -2 for memory errors
|     o -1 * (lineSL + 4) for duplicate character
|     o > 0 for line the problem is on
| File format:
|   - First line has `height:<size_in_pixels>` header
|     o or `height:<size>:gap:<size_in_pixels>` header
|   - Remaning lines in the file are blank or have a
|     character
|     o header: `:<character>:<width>:` (ex: `:A:16:`)
|     o character:
|       * a `#` is a filled pixel
|       * a ` ` is an empty pixel
|       * if  you have less then less then height lines
|         then they will be filled in at the bottom 
|       * max width is 504 pixels
\-------------------------------------------------------*/
signed long
getFont_font_pngFont(
   struct font_pngFont *fontSTPtr,
   void *fontFILE
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun05 TOC:
   '   - get a font from my ascii font file
   '   o fun05 sec01:
   '     - variable declarations
   '   o fun05 sec02:
   '     - get the height for the font
   '   o fun05 sec03:
   '     - get the font
   '   o fun05 sec04:
   '     - return the result
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun05 Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   #define buffSize_fun05 512
   signed char buffStr[buffSize_fun05];

   signed int posSI = 0;
   signed long lineSL = 0;

   unsigned char charUC = 0;
   signed short bytesSS = 0;
   signed short charByteSS = 0;
   signed short heightSS = 0;

   signed short pixSS = 0; /*pixel in byte I am on*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun05 Sec02:
   ^   - get the height for the font
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   blank_font_pngFont(fontSTPtr);

   while(
      ! fontSTPtr->heightSS
      &&
        fgets(
           (char *) buffStr,
           buffSize_fun05,
           (FILE *) fontFILE
        )
   ){ /*Loop: get the height*/
      ++lineSL;
      posSI = 0;
      while(buffStr[posSI] && buffStr[posSI] < 33)
         ++posSI; /*get off white space*/

      checkIfGap_fun05_sec02:;

      if( (buffStr[posSI] | 32) != 'g')
         ;
      else if( (buffStr[posSI + 1] | 32) != 'a')
         ;
      else if( (buffStr[posSI + 2] | 32) != 'p')
         ;
      else if( (buffStr[posSI + 3] | 32) != ':')
         ;
      else
      { /*Else: got width spacing*/
         charUC = 1;
         posSI += 4;
         posSI +=
            strToSS_base10str(
               &buffStr[posSI],
               &fontSTPtr->gapSS
            );

         if(buffStr[posSI] > 32 && buffStr[posSI] != ':')
            goto noHeight_fun05_sec04;
            /*invalid height*/
      } /*Else: got width spacing*/


      if( (buffStr[posSI] | 32) != 'h' )
         continue;
      else if( (buffStr[posSI + 1] | 32) != 'e' )
         continue;
      else if( (buffStr[posSI + 2] | 32) != 'i' )
         continue;
      else if( (buffStr[posSI + 3] | 32) != 'g' )
         continue;
      else if( (buffStr[posSI + 4] | 32) != 'h' )
         continue;
      else if( (buffStr[posSI + 5] | 32) != 't' )
         continue;
      else if( (buffStr[posSI + 6] | 32) != ':' )
         continue;
      else
      { /*Else: got height*/
         posSI += 7;
         posSI +=
            strToSS_base10str(
               &buffStr[posSI],
               &fontSTPtr->heightSS
            );

         if(buffStr[posSI] > 32 && buffStr[posSI] != ':')
            goto noHeight_fun05_sec04;

         else if(buffStr[posSI] == ':')
         { /*Else If: might have a gap size*/
            ++posSI;
            goto checkIfGap_fun05_sec02;
         } /*Else If: might have a gap size*/
      } /*Else: got height*/
   } /*Loop: get the height*/

   if(! fontSTPtr->heightSS)
      goto noHeight_fun05_sec04;
   if(! charUC)
      fontSTPtr->gapSS = fontSTPtr->heightSS / 4;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun05 Sec03:
   ^   - get the font
   ^   o fun05 sec03 sub01:
   ^     - find the height of all characters
   ^   o fun05 sec03 sub02:
   ^     - get the memory for the character
   ^   o fun05 sec03 sub03:
   ^     - get the bitmap for the character
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun05 Sec03 Sub01:
   *   - find the height of all characters
   \*****************************************************/

   while(
      fgets(
         (char *) buffStr,
         buffSize_fun05,
         (FILE *) fontFILE
      )
   ){ /*Loop: get the font*/
      ++lineSL;

      posSI = 0;
      while(buffStr[posSI] && buffStr[posSI] < 33)
         ++posSI; /*get off white space*/

      getChar_fun05_sec03_sub01:;
        if(! buffStr[posSI])
           continue; /*blank line*/
        else if(buffStr[posSI] != ':')
           continue;
        else if( buffStr[posSI + 1] > 126)
           continue; /*not visible or extened ascii*/
        else if( buffStr[posSI + 1] < 32)
           continue; /*invisible or not a space*/
        else
        { /*Else: have a character*/
           ++posSI;
           charUC = (unsigned char) buffStr[posSI];
           ++posSI;
        } /*Else: have a character*/


        if(buffStr[posSI] != ':')
           goto fileErr_fun05_sec04;
           /*no width given*/

        ++posSI;
        posSI +=
           strToSS_base10str(
              &buffStr[posSI],
              &fontSTPtr->widthArySS[charUC]
           );
        if(buffStr[posSI] != ':')
           goto fileErr_fun05_sec04;

        if(fontSTPtr->pixAryUC[charUC])
           goto duplicateChar_fun05_sec04;
           /*I have already read in this character*/

      /**************************************************\
      * Fun05 Sec03 Sub02:
      *   - get the memory for the character
      \**************************************************/

      bytesSS = fontSTPtr->widthArySS[charUC];
      bytesSS *= fontSTPtr->heightSS;

      if(bytesSS % def_bitsPerChar_64bit)
      { /*If: will loose pixels with a floor*/
         bytesSS /= def_bitsPerChar_64bit;
         ++bytesSS; /*account for flooring*/
      } /*If: will loose pixels with a floor*/

      else
         bytesSS /= def_bitsPerChar_64bit;

      fontSTPtr->pixAryUC[charUC] =
         calloc(bytesSS, sizeof(unsigned char));
      if(! fontSTPtr->pixAryUC[charUC])
         goto memErr_fun05_sec04;

      /**************************************************\
      * Fun05 Sec03 Sub03:
      *   - get the bitmap for the character
      *   o fun05 sec03 sub03 cat01:
      *     - get line and make sure not a new character
      *   o fun05 sec03 sub03 Cat02:
      *     - add the line to the character bitmap
      *   o fun05 sec03 sub03 cat03:
      *     - line may not end at the width of the
      *       character; move over the missing pixels
      \**************************************************/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun05 Sec03 Sub03 Cat01:
      +   - get line and make sure not a new character
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      charByteSS = 0;
      pixSS = 0;
      heightSS = 0;

      while(
         fgets(
            (char *) buffStr,
            buffSize_fun05,
            (FILE *) fontFILE
         )
      ){ /*Loop: get the bitmap*/
         ++heightSS;
         ++lineSL;
         posSI = 0;

         while(buffStr[posSI] && buffStr[posSI] < 33)
            ++posSI;
            /*get past white space to check if header*/

         if(! buffStr[posSI])
            continue; /*blank line*/
         else if(buffStr[posSI] == ':' )
             goto getChar_fun05_sec03_sub01;
         else
            posSI = 0;

         if(pixSS >= def_bitsPerChar_64bit)
         { /*If: need to move to the next byte*/
            pixSS = 0;
            ++charByteSS;
         } /*If: need to move to the next byte*/

         /*++++++++++++++++++++++++++++++++++++++++++++++\
         + Fun05 Sec03 Sub03 Cat02:
         +   - add the line to the character bitmap
         \++++++++++++++++++++++++++++++++++++++++++++++*/

         while(buffStr[posSI] > 31)
         { /*Loop: copy the bitmap*/

           for(; pixSS < def_bitsPerChar_64bit; ++pixSS)
           { /*Loop: add pixels*/
              if(posSI >= fontSTPtr->widthArySS[charUC])
                 break; /*the width of the character*/

              else if(buffStr[posSI] < 32)
                 break;

              else if(buffStr[posSI] == ' ')
                 ; /*blank cell*/

              else if(buffStr[posSI] == '#')
                fontSTPtr->pixAryUC[charUC][charByteSS] |=
                   (unsigned char) (1 << pixSS);

              else
                 goto fileErr_fun05_sec04;

              ++posSI;
           }  /*Loop: add pixels*/

           if(posSI >= fontSTPtr->widthArySS[charUC])
              break; /*the width of the character*/
           else if(! buffStr[posSI])
              break; /*end of line*/

           if(pixSS >= def_bitsPerChar_64bit)
           { /*If: need to move to the next byte*/
              pixSS = 0;
              ++charByteSS;
           } /*If: need to move to the next byte*/
         }  /*Loop: copy the bitmap*/

         /*++++++++++++++++++++++++++++++++++++++++++++++\
         + Fun05 Sec03 Sub03 Cat03:
         +   - line may not end at the width of the
         +     character; move over the missing pixels
         \++++++++++++++++++++++++++++++++++++++++++++++*/

         while(posSI < fontSTPtr->widthArySS[charUC])
         { /*Loop: account for jagged ends*/
            for( ; pixSS < def_bitsPerChar_64bit; ++pixSS)
            { /*Loop: move past pixel*/
               if(posSI >= fontSTPtr->widthArySS[charUC])
                  break; /*the width of the character*/
               ++posSI;
            } /*Loop: move past pixel*/

            if(posSI >= fontSTPtr->widthArySS[charUC])
               break; /*the width of the character*/
            else
            { /*Else: need to move to the next byte*/
               pixSS = 0;
               ++charByteSS;
            } /*Else: need to move to the next byte*/
         } /*Loop: account for jagged ends*/

         if(heightSS >= fontSTPtr->heightSS)
            break; /*finished getting the character*/
      }  /*Loop: get the bitmap*/
   } /*Loop: get the font*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun05 Sec04:
   ^   - return the result
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   return 0;

   memErr_fun05_sec04:;
      return -2;
   noHeight_fun05_sec04:;
      return -1;
   fileErr_fun05_sec04:;
      return lineSL;
   duplicateChar_fun05_sec04:;
      return -1 * (lineSL + 4);
} /*getFont_font_pngFont*/

/*-------------------------------------------------------\
| Fun06: measureText_font_pngFont
|   - measure how many pixels long a cstring is
| Input:
|   - textStr:
|     o cstring with text to get the length of
|   - fontSTPtr:
|     o font_pngFont struct pointer with the font to use
| Output:
|   - Returns:
|     o length in pixels of the text
\-------------------------------------------------------*/
signed short
measureText_font_pngFont(
   signed char *textStr,
   struct font_pngFont *fontSTPtr
){
   signed long lenSL = 0;

   while(*textStr)
   { /*Loop: get the length*/
      lenSL +=
        fontSTPtr->widthArySS[(unsigned char) *textStr];
      lenSL += fontSTPtr->gapSS;
   } /*Loop: get the length*/

   return lenSL;
} /*measureText_font_pngFont*/

/*-------------------------------------------------------\
| Fun07: drawHorzText_font_pngFont
|   - draw horizontal text to a png
| Input:
|   - textStr:
|     o cstring with a single line of text to draw
|   - xUS:
|     o x coordinate in the png (index 0)
|   - yUS:
|     o y coordinate in the png (index 0)
|   - fgColSC:
|     o index of the forground color to use with the font
|     o use -1 for no color
|   - bgColSC:
|     o index of the background color to use with the font
|     o use -1 for no color
|   - fontSTPtr:
|     o font_pngFont struct pionter to get the new font
|   - pngSTPtr:
|     o st_mkPng struct pointer to draw the text to
| Output:
|   - Modifies:
|     o pngSTPtr to have the drawn text
|   - Returns:
|     o 0 for no errors
|     o 1 if the coordinates were out of bounds
|     o 2 if textStr has a non-ascii character
\-------------------------------------------------------*/
signed char
drawHorzText_font_pngFont(
   signed char *textStr,    /*text to draw*/
   unsigned short xUS,      /*x coordinate*/
   unsigned short yUS,      /*y coordinate*/
   signed char fgColSC,     /*index of forground color*/
   signed char bgColSC,     /*index of background color*/
   struct font_pngFont *fontSTPtr, /*has the font to use*/
   struct st_mkPng *pngSTPtr /*has png to draw on*/
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun07 TOC:
   '   - draw text to a png
   '   o fun07 sec01:
   '     - variable declarations
   '   o fun07 sec02:
   '     - setup the color long and get initial position
   '   o fun07 Sec03:
   '     - draw the text
   '   o fun07 sec04:
   '     - return the result
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun07 Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   unsigned char charUC = 0;   /*character to add*/
   unsigned char pixelsUC = 0; /*has pixels for font*/
   signed char colSC = 0;

   signed long byteSL = 0;     /*byte adding to*/
   signed long endRowSL = 0;   /*byte adding to*/
   signed long nextRowSL = 0;  /*start of the next row*/
   signed long lastPixSL = 0;  /*last pixel in row*/

   signed short charByteSS = 0;/*has one byte of pixel*/
   signed short heightSS = 0;  /*number of rows printed*/
   signed short pixSS = 0;     /*the pix on in my byte*/
   signed short widthSS = 0;   /*width of the gap*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun07 Sec02:
   ^   - setup the color long and get initial position
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(xUS >= pngSTPtr->widthUS)
      goto overflowErr_fun07_sec04;
   if(yUS >= pngSTPtr->heightUS)
      goto overflowErr_fun07_sec04;

   byteSL = (yUS * pngSTPtr->widthUS) + xUS;
   nextRowSL = byteSL + pngSTPtr->widthUS;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun07 Sec03:
   ^   - draw the text
   ^   o fun07 sec03 sub01:
   ^     - draw each character
   ^   o fun07 sec03 sub02:
   ^     - draw the gap between characters
   ^   o fun07 sec03 sub03:
   ^     - move to the next character
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun07 Sec03 Sub01:
   *   - draw each character
   \*****************************************************/

   while(textStr)
   { /*Loop: add the text to the png*/
      if(byteSL >= pngSTPtr->usedBytesSL)
         goto overflowErr_fun07_sec04; /*out of bounds*/
      else if(*textStr < 32 || *textStr > 126)
         goto nonAsciiChar_fun07_sec04;

      charUC = (unsigned char) *textStr;
      ++textStr;
      endRowSL = 0;
      pixSS = 0;
      lastPixSL = byteSL + fontSTPtr->widthArySS[charUC];

      for(
         charByteSS = 0;
         charByteSS < fontSTPtr->lenArySS[charUC];
         ++charByteSS
      ){ /*Loop: fill in pixels for the character*/
         pixelsUC =
            fontSTPtr->pixAryUC[charUC][charByteSS];

         for(
            pixSS = 0;
            pixSS < def_bitsPerChar_64bit;
            ++pixSS
         ){ /*Loop: fill each row of the character*/
            if(byteSL > lastPixSL)
              break;

            addPixels_fun07_sec03_sub01:;
              if(pixelsUC & 1)
                 colSC = fgColSC;
              else
                 colSC = bgColSC;

              if(colSC >= 0)
                 pngSTPtr->pixelAryUC[byteSL] = colSC;

              pixelsUC >>= 1;
              ++byteSL;
         }  /*Loop: fill each row of the character*/

         /***********************************************\
         * Fun07 Sec03 Sub02:
         *   - draw the gap between characters
         \***********************************************/

         if(*textStr && byteSL > lastPixSL)
         { /*If: need to print the gap*/
            for(
               widthSS = 0;
               widthSS < fontSTPtr->gapSS;
               ++widthSS
            ){ /*Loop: add the gap between characters*/
               if(bgColSC >= 0)
                  pngSTPtr->pixelAryUC[byteSL] = bgColSC;
               ++byteSL;
            }  /*Loop: add the gap between characters*/

            if(! endRowSL)
               endRowSL = byteSL;
         } /*If: need to print the gap*/

         byteSL = nextRowSL;
         nextRowSL += pngSTPtr->widthUS;
         lastPixSL=byteSL + fontSTPtr->widthArySS[charUC];
         ++heightSS;

         if(heightSS >= fontSTPtr->heightSS)
            break; /*done with the character*/
         else if(nextRowSL > pngSTPtr->usedBytesSL)
            goto overflowErr_fun07_sec04;
         else if(pixSS < def_bitsPerChar_64bit)
            goto addPixels_fun07_sec03_sub01;
            /*the byte still has pixels left*/
      }  /*Loop: fill in pixels for the character*/

      /**************************************************\
      * Fun07 Sec03 Sub03:
      *   - move to the next character
      \**************************************************/

      ++textStr;
      byteSL = endRowSL;
      nextRowSL = endRowSL + pngSTPtr->widthUS;

      if(! *textStr && nextRowSL > pngSTPtr->usedBytesSL)
         goto overflowErr_fun07_sec04;
   } /*Loop: add the text to the png*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun07 Sec04:
   ^   - return the result
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   return 0;

   overflowErr_fun07_sec04:;
      return 1;
   nonAsciiChar_fun07_sec04:;
      return 2;
} /*drawHorzText_font_pngFont*/

/*-------------------------------------------------------\
| Fun08: drawVertText_font_pngFont
|   - draw text vertically to a png
| Input:
|   - textStr:
|     o cstring with a single line of text to draw
|   - xUS:
|     o x coordinate in the png (index 0)
|   - yUS:
|     o y coordinate in the png (index 0)
|   - fgColSC:
|     o index of the forground color to use with the font
|     o use -1 for no color
|   - bgColSC:
|     o index of the background color to use with the font
|     o use -1 for no color
|   - fontSTPtr:
|     o font_pngFont struct pionter to get the new font
|   - pngSTPtr:
|     o st_mkPng struct pointer to draw the text to
| Output:
|   - Modifies:
|     o pngSTPtr to have the drawn text
|   - Returns:
|     o 0 for no errors
|     o 1 if the coordinates were out of bounds
|     o 2 if textStr has a non-ascii character
\-------------------------------------------------------*/
signed char
drawVertText_font_pngFont(
   signed char *textStr,    /*text to draw*/
   unsigned short xUS,      /*x coordinate*/
   unsigned short yUS,      /*y coordinate*/
   signed char fgColSC,     /*index of forground color*/
   signed char bgColSC,     /*index of background color*/
   struct font_pngFont *fontSTPtr, /*has the font to use*/
   struct st_mkPng *pngSTPtr /*has png to draw on*/
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun08 TOC:
   '   - draw text to a png
   '   o fun08 sec01:
   '     - variable declarations
   '   o fun08 sec02:
   '     - setup the color long and get initial position
   '   o fun08 sec03:
   '     - draw the text
   '   o fun08 sec04:
   '     - return the result
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun08 Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   unsigned char charUC = 0;  /*character to add*/
   unsigned char pixelsUC = 0;/*has pixels to add*/
   signed char colSC = 0;     /*color to use for a pixel*/

   signed long byteSL = 0;    /*byte adding to*/
   signed long nextRowSL = 0; /*start of the next row*/
   signed long lastPixSL = 0;
      /*last pixel for char in a row*/

   signed short charByteSS = 0;/*has one byte of pixel*/
   signed short heightSS = 0; /*number of rows printed*/
   signed short pixSS = 0;    /*the pix on in my byte*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun08 Sec02:
   ^   - setup the color long and get initial position
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(xUS >= pngSTPtr->widthUS)
      goto overflowErr_fun08_sec04;
   if(yUS >= pngSTPtr->heightUS)
      goto overflowErr_fun08_sec04;

   byteSL = (yUS * pngSTPtr->widthUS) + xUS;
   nextRowSL = byteSL + pngSTPtr->widthUS;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun08 Sec03:
   ^   - draw the text
   ^   o fun08 sec03 sub01:
   ^     - print the character
   ^   o fun08 sec03 sub02:
   ^     - print the gap between characters
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun08 Sec03 Sub01:
   *   - print the character
   \*****************************************************/

   while(textStr)
   { /*Loop: add the text to the png*/
      if(byteSL >= pngSTPtr->usedBytesSL)
         goto overflowErr_fun08_sec04;
      else if(*textStr < 32 || *textStr > 126)
         goto nonAsciiChar_fun08_sec04;

      charUC = (unsigned char) *textStr;
      ++textStr;
      lastPixSL = byteSL + fontSTPtr->widthArySS[charUC];
         /*get the last pixel to fill in*/

      for(
         charByteSS = 0;
         charByteSS < fontSTPtr->lenArySS[charUC];
         ++charByteSS
      ){ /*Loop: fill in pixels for the character*/
         pixelsUC =
            fontSTPtr->pixAryUC[charUC][charByteSS];

         for(
            pixSS = 0;
            pixSS < def_bitsPerChar_64bit;
            ++pixSS
         ){ /*Loop: fill each row of the character*/
            if(byteSL > lastPixSL)
               break;

            addPixels_fun08_sec03_sub01:;
               if(pixelsUC & 1)
                  colSC = fgColSC;
               else
                  colSC = bgColSC;

               if(colSC >= 0)
                  pngSTPtr->pixelAryUC[byteSL] = colSC;

               pixelsUC >>= 1;
               ++byteSL;
         }  /*Loop: fill each row of the character*/


         if(byteSL > lastPixSL)
         { /*If: I need to move to the next line*/
            byteSL = nextRowSL;
            nextRowSL += pngSTPtr->widthUS;

            lastPixSL =
               byteSL + fontSTPtr->widthArySS[charUC];
            ++heightSS;
         } /*If: I need to move to the next line*/

         if(heightSS >= fontSTPtr->heightSS)
            break; /*done with the character*/
         if(nextRowSL > pngSTPtr->usedBytesSL)
            goto overflowErr_fun08_sec04;
         if(pixSS < def_bitsPerChar_64bit)
            goto addPixels_fun08_sec03_sub01;
            /*this reduces the amount of duplicate code*/
      }  /*Loop: fill in pixels for the character*/

      /**************************************************\
      * Fun08 Sec03 Sub02:
      *   - print the gap between characters
      \**************************************************/

      if(*textStr)
      { /*If: need to print the gap*/
         for(
            heightSS = 0;
            heightSS < fontSTPtr->gapSS;
            ++heightSS
         ){ /*Loop: add in the gap for vertical text*/
            if(bgColSC >= 0)
            { /*If: have a background color*/
              for(
                pixSS = 0;
                pixSS < fontSTPtr->widthArySS[charUC];
                ++pixSS
              ){ /*Loop: add the gap between characters*/
                  pngSTPtr->pixelAryUC[byteSL] = bgColSC;
                  ++byteSL;
              }  /*Loop: add the gap between characters*/
            } /*If: have a background color*/

            /*I need to keep track of the next row*/
            byteSL = nextRowSL;
            nextRowSL += pngSTPtr->widthUS;

            lastPixSL =
               byteSL + fontSTPtr->widthArySS[charUC];

            if(nextRowSL > pngSTPtr->usedBytesSL)
               goto overflowErr_fun08_sec04;
         }  /*Loop: add in the gap for vertical text*/
      } /*If: need to print the gap*/
   } /*Loop: add the text to the png*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun08 Sec04:
   ^   - return the result
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   return 0;

   overflowErr_fun08_sec04:;
      return 1;

   nonAsciiChar_fun08_sec04:;
      return 2;
} /*drawVertText_font_pngFont*/

/*-------------------------------------------------------\
| Fun09: fontToC_font_pngFont
|   - convert a font_pngFont to to .c and .h files
| Input:
|   - fontSTPtr:
|     o font_pngFont struct pointer to convert to a .c and
|       .h file
|   - prefixStr:
|     o cstring with name of the font file
|     o must be a valid function name for c
|       * valid characters are letters (a-z, A-Z), numeric
|         (0-9), or an `_`
|       * the first character can not be a number
| Output:
|   - Prints:
|     o code to auto load the font to a .c and .h file
|   - Returns:
|     o 0 for no errors
|     o 1 if prefixStr is not a valid function name
|       * valid characters are letters (a-z, A-Z), numeric
|         (0-9), or an `_`
|       * the first character can not be a number
|     o 2 if could not open the .c file
|     o 3 if could not open the .h file
\-------------------------------------------------------*/
signed char
fontToC_font_pngFont(
   struct font_pngFont *fontSTPtr,
   signed char *prefixStr
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun09 TOC:
   '   - convert pngFont ascii font file to .c & .h files
   '   o fun09 sec01:
   '     - variable declarations
   '   o fun09 sec02:
   '     - print to the .c file
   '   o fun09 sec03:
   '     - print the main function in the .c file
   '   o fun09 sec04:
   '     - print the .h file
   '   o fun09 sec05:
   '     - return the result
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun09 Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   signed char outStr[1024];
   signed short lenSS = 0;   /*end of output file name*/
   unsigned char charUC = 0; /*character I am on*/
   signed short pixSS = 0;   /*pixel I am on*/

   FILE *outFILE = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun09 Sec02:
   ^   - print to the .c file
   ^   o fun09 sec02 sub01:
   ^     - open the .c file
   ^   o fun09 sec02 sub02:
   ^     - print the libraries used
   ^   o fun09 sec02 sub03:
   ^     - print the header for each character
   ^   o fun09 sec02 sub04:
   ^     - print function name and input
   ^   o fun09 sec02 sub05:
   ^     - add memory allocation
   ^   o fun09 sec02 sub06:
   ^     - add each pixel and end the function
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun09 Sec02 Sub01:
   *   - open the .c file
   \*****************************************************/

   lenSS = 0;
   if(prefixStr[0] >= 'A' && prefixStr[0] <= 'Z')
      ;
   else if(prefixStr[0] >='a' && prefixStr[0] <='z')
      ;
   else if(prefixStr[0] =='_')
      ;
   else
      goto badPrefix_fun08_sec05;

   outStr[0] = prefixStr[0];
   lenSS = 1;
   while(prefixStr[lenSS])
   { /*Loop: check for invalid characters*/
      if(
        prefixStr[lenSS] >= 'A' && prefixStr[lenSS] <= 'Z'
      ) outStr[lenSS] = prefixStr[lenSS];

      else if(
         prefixStr[lenSS] >='a' && prefixStr[lenSS] <='z'
      ) outStr[lenSS] = prefixStr[lenSS];

      else if(
         prefixStr[lenSS] >='0' && prefixStr[lenSS] <='9'
      ) outStr[lenSS] = prefixStr[lenSS];

      else if(prefixStr[lenSS] =='_')
         outStr[lenSS] = prefixStr[lenSS];

      else
         goto badPrefix_fun08_sec05;

      ++lenSS;
   } /*Loop: check for invalid characters*/

   outStr[lenSS] = '.';
   outStr[lenSS + 1] = 'c';
   outStr[lenSS + 2] = 0;

   outFILE = fopen((char *) outStr, "w");
   if(! outFILE)
      goto noCFile_fun09_sec05;
 
   /*****************************************************\
   * Fun09 Sec02 Sub02:
   *   - print the libraries used
   \*****************************************************/

   fprintf(outFILE, "#ifdef PLAN9\n");
   fprintf(outFILE, "   #include <u.h>\n");
   fprintf(outFILE, "   #include <libc.h>\n");
   fprintf(outFILE, "#else\n");
   fprintf(outFILE, "   #include <stdlib.h>\n");
   fprintf(outFILE, "#endif\n\n");

   fprintf(outFILE, "#include \"pngFont.h\"\n\n");

   /*these are libraries that are included in the
   `  pngFont library
   */
   fprintf(
     outFILE,
     "/*_______________________________________________\n"
   );
   fprintf(outFILE, " Hidden libraries:\n");
   fprintf(outFILE, "   - std #include <stdio.h>\n");
   fprintf(
      outFILE,
      "   - .c  #include \"../genLib/ulCp.h\"\n"
   );
   fprintf(
      outFILE,
      "   - .c  #include \"../genLib/base10str.h\"\n"
   );
   fprintf(
      outFILE,
      "   - .c  #include \"../genLib/endin.h\"\n"
   );
   fprintf(
      outFILE,
      "   - .c  #include \"../genLib/checksum.h\"\n"
   );
   fprintf( outFILE, "   - .c  #include \"mkPng.h\"\n");
   fprintf(
      outFILE,
      "   - .h  #include \"../genLib/64bit.h\"\n"
   );
   fprintf(
     outFILE,
     "_______________________________________________*/\n"
   );
   fprintf(outFILE, "\n");

   /*****************************************************\
   * Fun09 Sec02 Sub03:
   *   - print the header for each character
   \*****************************************************/

   for(
      charUC = 0;
      charUC < def_numAsciiChar_pngFont;
      ++charUC
   ){ /*Loop: print functions to print each character*/
      fprintf(
         outFILE,
         "/*Fun%2i: add%c_%s _______________\n",
         charUC,
         charUC + 32,
         prefixStr
      );
      fprintf(
         outFILE,
         "`  - adds character %c to the font\n",
         charUC + 32
      );
      fprintf(
         outFILE,
         "`  - returns 1 for success or 0 for failure\n"
      );
      fprintf(
         outFILE,
         "___________________________________________*/n"
      );

      /**************************************************\
      * Fun09 Sec02 Sub04:
      *   - print function name and input
      \**************************************************/

      fprintf(outFILE, "signed char\n");

      fprintf(
         outFILE,
         "add%c_%s(\n",
         charUC + 32,
         prefixStr
      );

      fprintf(
         outFILE,
         "   struct font_pngFont *fontSTPtr\n"
      );
      fprintf(outFILE, "){\n");

      /**************************************************\
      * Fun09 Sec02 Sub05:
      *   - add memory allocation
      \**************************************************/

      fprintf(
         outFILE,
         "   if(fontSTPtr->pixAryUC[%u])\n",
         charUC + 32
      ); /*check if need to free memory*/

      fprintf(
         outFILE,
         "      free(fontSTPtr->pixAryUC[%u]);\n\n",
         charUC + 32
      ); /*free memory*/

      fprintf(
         outFILE,
         "  fontSTPtr->pixAryUC[%u] =\n",
         charUC + 32
      ); /*add memroy*/

      fprintf(
         outFILE,
         "     calloc(%i, sizeof(unsigned char));\n",
         fontSTPtr->widthArySS[charUC + 32]
      ); /*add memory calloc call*/

      fprintf(
         outFILE,
         "   if(! fontSTPtr->pixAryUC[%u])\n",
         charUC + 32
      ); /*check if failed to add memroy*/

      fprintf(outFILE, "      return 1;\n\n");
         /*check if failed to add memroy*/

      fprintf(
         outFILE,
         "    fontSTPtr->widthArySS[%i] = %i;\n",
         charUC + 32,
         fontSTPtr->widthArySS[charUC + 32]
      ); /*add the width of the character*/

      fprintf(
         outFILE,
         "    fontSTPtr->lenArySS[%i] = %i;\n",
         charUC + 32,
         fontSTPtr->lenArySS[charUC + 32]
      ); /*add thh number bytes used to store the char*/

      /**************************************************\
      * Fun09 Sec02 Sub06:
      *   - add each pixel and end the function
      \**************************************************/

      for(
         pixSS = 0;
         pixSS < fontSTPtr->widthArySS[charUC];
         ++pixSS
      ){ /*Loop: add each pixel indivdually*/
         fprintf(
            outFILE,
            "   fontSTPtr->pixAryUC[%u][%i] = %i;\n",
            charUC + 32,
            pixSS,
            fontSTPtr->pixAryUC[charUC + 32][pixSS]
         );
      }  /*Loop: add each pixel indivdually*/

      fprintf(outFILE, "   return 0;\n");
      fprintf(
         outFILE,
         "} /*add%c_%s*/\n\n",
         charUC + 32,
         prefixStr
      );
   }  /*Loop: print functions to print each character*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun09 Sec03:
   ^   - print the main function in the .c file
   ^   o fun09 sec03 sub01:
   ^     - print the header for the function
   ^   o fun09 sec03 sub02:
   ^     - print the function
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun09 Sec03 Sub01:
   *   - print the header for the function
   \*****************************************************/

   fprintf(
      outFILE,
      "/*Fun%2i: loadFont_%s _______________\n",
      charUC,
      prefixStr
   );
   fprintf(
      outFILE,
      "`  - loads the font in %s.c/h to the input\n",
      prefixStr
   );
   fprintf(
      outFILE,
      "`    font_pngFont struct (as pionter)\n"
   );
   fprintf(
      outFILE,
      "`  - returns 1 for success or 0 for failure\n"
   );
   fprintf(
      outFILE,
      "___________________________________________*/n"
   );

   /*****************************************************\
   * Fun09 Sec03 Sub02:
   *   - print the function
   \*****************************************************/

   fprintf(outFILE, "signed char\n");
   fprintf(outFILE, "loadFont_%s(\n", prefixStr);
   fprintf(
      outFILE,
      "   struct font_pngFont *fontSTPtr\n"
   );
   fprintf(outFILE, "){\n");


   fprintf(
      outFILE,
      "   blank_font_pngFont(fontSTPtr);\n"
   );
   fprintf(
      outFILE,
      "   fontSTPtr->heightSS = %i;\n",
      fontSTPtr->heightSS
   );
   fprintf(
      outFILE,
      "   fontSTPtr->gapSS = %i;\n",
      fontSTPtr->gapSS
   );

   for(
      charUC = 0;
      charUC < def_numAsciiChar_pngFont;
      ++charUC
   ){ /*Loop: print the user function*/
      fprintf(
         outFILE,
         "   if( add%c_%s(fontSTPtr) )\n",
         charUC + 32,
         prefixStr
      ); /*add each character to the function*/

      fprintf(
         outFILE,
         "      return 1;\n"
      ); /*if had a memory error*/
   }  /*Loop: print the user function*/

   fprintf(outFILE, "\n   return 0;\n");
   fprintf(outFILE, "} /*loadFont_%s*/\n", prefixStr);

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun09 Sec04:
   ^   - print the .h file
   ^   o fun09 sec04 sub01:
   ^     - print the header for the .h file
   ^   o fun09 sec04 sub02:
   ^     - print the font load function
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun09 Sec04 Sub01:
   *   - print the header for the .h file
   \*****************************************************/

   fclose(outFILE);
   outFILE = 0;

   outStr[lenSS] = '.';
   outStr[lenSS + 1] = 'h';
   outStr[lenSS + 2] = 0;

   outFILE = fopen((char *) outStr, "w");
   if(! outFILE)
      goto noHFile_fun09_sec05;

   outStr[lenSS] = 0;
   lenSS = 0;
   while(outStr[lenSS])
   { /*Loop: convert the prefix to uppercase*/
      if(outStr[lenSS] >= 'a')
         outStr[lenSS] &= ~32; /*lower case letter*/
   } /*Loop: convert the prefix to uppercase*/


   prefixStr[lenSS] = 0;

   fprintf(
      outFILE,
      "#ifndef LOAD_THE_%s_FONT_H\n",
      outStr
   );
   fprintf(
      outFILE,
      "#define LOAD_THE_%s_FONT_H\n",
      outStr
   );
   fprintf(outFILE, "struct font_pngFont;\n\n");

   /*****************************************************\
   * Fun09 Sec04 Sub02:
   *   - print the font load function
   \*****************************************************/

   fprintf(
      outFILE,
      "/*Fun%2i: loadFont_%s _______________\n",
      charUC,
      prefixStr
   );
   fprintf(
      outFILE,
      "`  - loads the font in %s.c/h to the input\n",
      prefixStr
   );
   fprintf(
      outFILE,
      "`    font_pngFont struct (as pionter)\n"
   );
   fprintf(
      outFILE,
      "`  - returns 1 for success or 0 for failure\n"
   );
   fprintf(
      outFILE,
      "___________________________________________*/n"
   );

   fprintf(outFILE, "signed char\n");
   fprintf(outFILE, "loadFont_%s(\n", prefixStr);
   fprintf(
      outFILE,
      "   struct font_pngFont *fontSTPtr\n"
   );
   fprintf(outFILE, ");\n\n");
   fprintf(outFILE, "#endif\n");


   fclose(outFILE);
   outFILE = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun09 Sec05:
   ^   - return the result
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   lenSS = 0;
   goto ret_fun09_sec05;

   badPrefix_fun08_sec05:;
      lenSS = 1;
      goto ret_fun09_sec05;

   noCFile_fun09_sec05:;
      lenSS = 2;
      goto ret_fun09_sec05;

   noHFile_fun09_sec05:;
      lenSS = 3;
      goto ret_fun09_sec05;

   ret_fun09_sec05:;
      if(outFILE)
         fclose(outFILE);
      outFILE = 0;

      return (signed char) lenSS;
} /*fontToC_font_pngFont*/
