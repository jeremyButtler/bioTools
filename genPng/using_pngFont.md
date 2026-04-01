# pngFont:

Has functions to print my font to a st\_mkPng structure
  (png). Also, has the function to reads in a font from an
  ascii font file.

Files: pngFont.h and pngFont.c

- Dependencies:
  - ../genLib/ulCp.h
  - ../genLib/base10str.h
  - mkPng.h
  - ../genLib/64bit.h (.h file only)
- Hidden dependencies:
  - ../genLib/checkSum.h
  - ../genLib/endin.h

This library stores the uploaded font in a font\_pngFont
  structure. You can then print this font using text and
  the functions. Valid characters for printing are ascii
  32 (space) to ascii 128 (~). New lines, carriage
  returns,  and tabs are not valid characters.

# font\_pngFont:

The font\_pngFont structure stores the font in a bitmap.

- Variables:
  - pixAryUC: is the unsigned char pointer array that has
    the bitmap for each each character in the font
    - length is 94 characters (all visible ascii + space)
    - each unsigned char array represents one character
    - each bit in the character array is one pixel
      - 1 is forground
      - 0 is bacground
  - widthArySS: array with the widith in pixels of each
    character in the font
  - lenArySS: number of bytes each character in the font
    has
  - hieghtSS: height in pixels of the font
  - gapSS: gap in pixels between each character in the
    font

# The font file

The font file for pngFont is an ascii file. The first line
  should start with `height:<font_height_in_pixels`. If
  you want to set the gap between characters you can do
  `height:<font_height>:gap:<font_gap_in_pixels>`.

The remaning lines have the font.

Each character starts with a header that has the character
  to use and the width of the character in pixels. This
  header is `:<character>:<width_in_pixels>:`.

Each line after the header is a row of pixels in the
  character. The character stops when the height is
  reached or the next characters header is found.

If you have more rows then the fonts height, then the
  extra rows at the end are ignored. If you have less rows
  then the fonts height, then the blank rows are added at
  the bottom of the character.

For each row of pixels, only the width\_in\_pixels number
  of characters are read in. Any characters past the width
  are ignored. The maximum number of characters allowed
  per row is 504. Otherwise fgets not get the full line
  and this will mess up the function.

If you have less columns then your characters width, then
  blank columns are added in.

In a row each pixels is represented by a space (` `) or a
  pound symbol (`#`). The space is for a blank pixel
  (background) and is used to positionthe pounds. The
  poud is the character (forground).

Any character that is not a space, pound, or the next
  header is invalid. This is done intentionally because it
  gives an idea of how the font will really look.

Here is the header and tfirst two characters from the
  default font (it is a 12pt font).

```
height:16:gap:4
:A:16:                               
       ##
      ####
     ######
    ###  ###
   ###    ###
  ###      ###
  ############
  ############
  ###      ###
  ###      ###
  ###      ###
  ###      ###
 #####    #####
#######  #######


:B:16:
#############
##############
###         ###
###          ###
###          ###
###         ###
##############
##############
###         ###
###          ###
###          ###
###         ###
##############
#############
```

# Using pngFont:

You first step is to create a font\_pngFont structure.
  After creating the structure you will initialize it.
  Then load your font. When finished with your font you
  need to free it.

## Creating a font\_pngFont structure

Creation of a font\_pngFont structure is done by you.

After creation you need to initialize the structure with
  the `init_font_pngFont()` function. The input is a
  pointer to your font\_pngFont structure.

```
#include "pngFont.h"

struct font_pngFont fontStackST;
init_font_pngFont(&fontStackST);
```

## Loading a font into a font\_pngFont structure

### Loading the font from a file

You can load a font into your font\_pngFont using a file
  with an ascii font or by using an auto generated font
  stored in a .c and .h file.

The `getFont_font_pngFont()` function will allow you to
  load a font from a file.

- Input
  1. is a pointer to your font\_pngFont structure
  2. the file with the font (a FILE pionter)
- Returns:
  - 0 for no errors
  - -1 if file did not list the height of the font
  - -2 for a memroy error
  - -1 x (line + 4) if had a duplicate character in the
    file (input 2)
    - get the line number with (line x -1) - 4
  - > 0 if had a file error (is the line with the problem)

```
#include "pngFont.h"

signed long errSL = 0;
struct font_pngFont fontStackST;
FILE *fontFILE = fopen("<your_font_file>", "r");

init_font_pngFont(&fontStackST);
if(! fontFILE)
   goto err_main:;


errSL = getFont_fong_pngFont(&fontStackST, fontFILE);
if(! errSL)
   ; /*no error*/
else
   goto err_main; /*some kind of error*/


errSL = 0;
goto ret_main;

err_main:;
   errSL = 1;

ret_main:;
   freeStack_font_pngFont(&fontStackST);

   if(fontFILE)
      fclose(fontFILE);
   fontFILE = 0;

   return errSL;
```

### Loading the font from an auto-generated function

Another way to load a font is from an auto-generated
  function made by the `fontToC_font_pngFont()` function.
  The function name will be `loadFont_<your_font_name>()`.

- Input
  1. pointer to your font\_fontPng structure.
- Returns:
  - 0 for no errors
  - 1 for a memory error

```
#include "pngFont.h"
#include "<your_font_name>.h"

struct font_pngFont fontStackST;

init_font_pngFont(&fontStackST);

if( loadFont_<your_font_name>(&fontStackST) )
{ /*If: memory error*/
   freeStack_font_pngFont(&fontStackST);
   return 1;
} /*If: memory error*/

return 0;
```

## Drawing text

You can draw vertical or horizontal text. Newlines,
  carriage returns, and tabs are not allowed.

To draw text horizontally use the
  `drawHorzText_font_pngFont()` function.

To draw text vertically use the
  `drawVertText_font_pngFont()` function.

Both the horizontal and vertical text draw functions take
  the same input and return the same errors.

- Input:
  1. c-string with the text to draw
  2. x coordinate to start drawing the text at
  3. y coordinate to start drawing the text at
  4. forground color (as a png pallete index) to use
     - use -1 to not print the forground
  5. background color (as a png pallete index) to use
     - use -1 to not print the background
  6. pointer to a font\_pngFont structure with the font to
     use
  7. pointer to a st\_mkPng structure with the png
- Returns:
  - 0 for no errors
  - 1 if the text when outside the png bounds (overflow)
  - 2 if the text had an invalid character
    - not ascii 32 to 126

```
#ifdef PLAN9
   #include <u.h>
   #include <libc.h>
#endif

#include "pngFont.h"
#include "pngDefaultFont.h"
#include "mkPng.h"

int
main(
   void
){
   signed char errSC = 0;
   struct st_mkPng *pngHeapST = mkPng(700, 700, 4);
   struct font_pngFont fontStackST;
   
   init_font_pngFont(&fontStackST);
   
   if(! pngSTPtr)
      goto err_main;
   if( loadFont_pngDefaultFont(&fontStackST) )
      goto err_main;
   
   drawHorzText_font_pngFont(
      (signed char *) "this is some vertical text",
      100, /*100 pixels on the x-axis*/
      20,  /*20 pixels up*/
      3,   /*set the forground color to the third color*/
      -1,  /*do not print the background*/
      &fontStackST,
      pngHeapST
   ); /*draw the text horizontally*/
   
   drawVertText_font_pngFont(
      (signed char *) "this is some text",
      0,  /*start at x-colomn 0*/
      20, /*20 pixels up*/
      -1, /*do not print the forground*/
      3,  /*set the background to the thrid color*/
      &fontStackST,
      pngHeapST
   ); /*draw the text vertically*/
   
   /*print the png out (not sure why you would do this)*/
   print_st_mkPng(pngHeapST, stdout);
   
   errSC = 0;
   goto ret_main;
   
   err_main;;
      errSC = 1;   
      goto ret_main;
   
   ret_main:;
      freeStack_font_pngFont(&fontStackST);
   
      if(pntHeapST)
         freeHeap_st_mkPng(pngHeapST);
      pngHeapST = 0;
   
      return errSC;
} /*main*/
```

## Freeing your structure

You can free your font\_pngFont structure using the
  `freeStack_font_pngFont()` function or the
  `freeHeap_font_pngFont()` function. The freeStack
  function frees the variables in the font\_pngFont
  structure, while the freeHeap function frees the
  structure (you must set to null or 0).

The input for both functions is a pointer to the
  font\_pngFont structure to free.

### An example using freeStack:

```
#include "pngFont.h"

struct font_pngFont fontStackST;
init_font_pngFont(&fontStackST);

/*your code goes here*/
freeStack_font_pngFont(&fontStackST);
```

### An example using freeHeap:

```
#include "pngFont.h"

struct font_pngFont *fontHeapST =
   malloc(sizeof(struct font_pngFont));

if(! fontHeapST)
   return 1;
init_font_pngFont(fontHeapST);

/*your code goes here*/
freeHeap_font_pngFont(&fontHeapST);
```

## Misc functions

These are functions that I can not put into a bin.

### Measure text

If you need to find the length of your printed text you
  can use the `measureText_font_pngFont()` function.

- Input:
   1. c-string with text to measure
   2. pointer to a font\_pngFont structure with the font
- Returns:
  - length of the text (input 1) when printed with the
    font (input 2)

```
#include <stdio.h>
#include "pngFont.h"

signed long textLengthSL = 0;
struct font_pngFont fontStackST;
   
init_font_pngFont(&fontStackST);

if( loadFont_pngDefaultFont(&fontStackST) )
{ /*If; failed to load my font*/
   freeStack_font_pngFont(&fontStackST);
   return 1;
} /*If; failed to load my font*/

testLengthSL =
   measureText_font_pngFont(
      (signed char *) "some text to measure",
      &fontStackST
   );
printf("My text is %li characters long\n", textLengthSL);

freeStack_font_pngFont(&fontStackST);
return 0;
```

### Convert a font to .c and .h file

Sometimes you want your font to be embed in your code.
  In this case we can make a .c and .h file to load the
  font for you using the `fontToC_font_pngFont()`
  function. The output creates a function that takes a
  font\_pngFont structure to load the font into and
  returns 0 for success and 1 for a memory error.

After making your font file you will need to open the
  `<font_name>.c` file and change the `#include pngFont.h`
  line to have the path to the pngFont.h file. The
  default output assumes this file is in genPng.

In the output font file change:

```
#ifdef PLAN9
   #include <u.h>
   #include <libc.h>
#else
   #include <stdlib.h>
#endif

#include "pngFont.h"
```

To:

```
#ifdef PLAN9
   #include <u.h>
   #include <libc.h>
#else
   #include <stdlib.h>
#endif

#include "path/to/pngFont.h"
```

- Input:
  1. pointer to a font\_pngFont structure with the font to
     embed
  2. c-string with the name of the font
     - must be a valid c function name
       - no numbers at the start
       - allowed characters a-z, A-Z, 0-9, and `_`
     - this is the name for your output .c and .h files
- Returns:
  - 0 for success
  - 1 if the c-string (input 2) was not valid
  - 2 if could not open the .c file
  - 3 if could not open the .h file

Example: saving a font to a .c and .h file

```
#ifdef PLAN9
   #include <u.h>
   #include <libc.h>
#endif

#include <stdio.h>
#include "pngFont.h"

int
main(
   void
){
   signed long errSL = 0;
   struct font_pngFont fontStackST;
   FILE *fontFILE = fopen("<your_font_file>", "r");
   
   init_font_pngFont(&fontStackST);
   if(! fontFILE)
      goto err_main:;
   
   
   errSL = getFont_fong_pngFont(&fontStackST, fontFILE);
   if(! errSL)
      ; /*no error*/
   else
      goto err_main; /*some kind of error*/
   fclose(fontFILE);
   fontFILE = 0;

   
   errSL =
      fontToC_font_pngFont(
         &fontStackST,
         (signed char *) "myCoolFont"
      ); /*saves font to myCoolFont.c and myCoolFont.h*/
         /*the function name is loadFont_myCoolFont()*/
   if(errSL)
      goto err_main;
   

   errSL = 0;
   goto ret_main;
   
   err_main:;
      errSL = 1;
      goto ret_main;
   
   ret_main:;
      freeStack_font_pngFont(&fontStackST);
   
      if(fontFILE)
         fclose(fontFILE);
      fontFILE = 0;
   
      return errSL;
} /*main*/
```

Loading the saved front (from .c and .h file):

```
#ifdef PLAN9
   #include <u.h>
   #include <libc.h>
#endif

#include <stdio.h>
#include "pngFont.h"
#include "myCoolFont.h"

int
main(
   void
){
   signed long errSL = 0;
   struct font_pngFont fontStackST;
   
   init_font_pngFont(&fontStackST);
   if(loadFont_myCoolFont(&fontStackST))
      goto err_main;
   
   errSL = 0;
   goto ret_main;
   
   err_main:;
      errSL = 1;
      goto ret_main;
   
   ret_main:;
      freeStack_font_pngFont(&fontStackST);
   
      if(fontFILE)
         fclose(fontFILE);
      fontFILE = 0;
   
      return errSL;
} /*main*/
```
