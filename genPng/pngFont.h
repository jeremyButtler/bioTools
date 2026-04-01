/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' pngFont SOF: Start Of File
'   - handels loading and drawing png fonts
'   o header:
'     - forward declarations and guards
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
|   - forward declarations and guards
\-------------------------------------------------------*/

#ifndef PNG_FONT_LOADING_AND_DRAWING_H
#define PNG_FONT_LOADING_AND_DRAWING_H
struct st_mkPng;

#define def_asciiOffset_pngFont 32
   /*converts visible + space range from 32 to 126 to
   `   0 to 93
   */
#define def_numAsciiChar_pngFont 95
   /*number of ascii characters*/

/*-------------------------------------------------------\
| ST01: font_pngFont
|   - holds the font to draw into the png
\-------------------------------------------------------*/
typedef struct
font_pngFont{
   unsigned char *pixAryUC[def_numAsciiChar_pngFont];
      /*has all visible ascii characters in a bitmap
      `  uses 1 for forground and 0 for background
      `Each bit is one pixel
      */
   signed short widthArySS[def_numAsciiChar_pngFont];
      /*width of each character*/
   signed short lenArySS[def_numAsciiChar_pngFont];
      /*number of bytes in pixAryUC*/

   signed short heightSS;
      /*height of all characters*/
   signed short gapSS; /*gap between characters*/
}font_pngFont;

/*-------------------------------------------------------\
| Fun01: blank_font_pngFont
|   - blanks a font_pngFont struct
| Input:
|   - fontSTPtr:
|     o font_pngFont struct pointer to blank
| Output:
|   - Modifies:
|     o all elements in pixAryUC and widthAryUC to be
|       set to null/0
|     o frees all pionters in pixAryUC
\-------------------------------------------------------*/
void
blank_font_pngFont(
   struct font_pngFont *fontSTPtr
);

/*-------------------------------------------------------\
| Fun02: init_font_pngFont
|   - initializes a font_pngFont struct
| Input:
|   - fontSTPtr:
|     o font_pngFont struct pointer to initialize
| Output:
|   - Modifies:
|     o all elements in pixAryUC and widthAryUC to be
|       set to null/0
\-------------------------------------------------------*/
void
init_font_pngFont(
   struct font_pngFont *fontSTPtr
);

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
);

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
);

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
);

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
);

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
|     o column (x-axis) ended on (>= 0) for no errors
|     o -1 if the coordinates were out of bounds
|     o -2 if textStr has a non-ascii character
\-------------------------------------------------------*/
signed int
drawHorzText_font_pngFont(
   signed char *textStr,    /*text to draw*/
   unsigned short xUS,      /*x coordinate*/
   unsigned short yUS,      /*y coordinate*/
   signed char fgColSC,     /*index of forground color*/
   signed char bgColSC,     /*index of background color*/
   struct font_pngFont *fontSTPtr, /*has the font to use*/
   struct st_mkPng *pngSTPtr /*has png to draw on*/
);

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
|     o last row (y-axis position) on (>= 0) for no errors
|     o -1 if the coordinates were out of bounds
|     o -2 if textStr has a non-ascii character
\-------------------------------------------------------*/
signed int
drawVertText_font_pngFont(
   signed char *textStr,    /*text to draw*/
   unsigned short xUS,      /*x coordinate*/
   unsigned short yUS,      /*y coordinate*/
   signed char fgColSC,     /*index of forground color*/
   signed char bgColSC,     /*index of background color*/
   struct font_pngFont *fontSTPtr, /*has the font to use*/
   struct st_mkPng *pngSTPtr /*has png to draw on*/
);

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
);

#endif
