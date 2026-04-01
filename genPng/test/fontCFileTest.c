/*goal is to find errors in my font output function for
`  .c and .h files
*/

#ifdef PLAN9
   #include <u.h>
   #include <libc.h>
#else
   #include <stdlib.h>
#endif

#include <stdio.h>
#include "../pngFont.h"

int
main(
   void
){
   signed long errSL = 0;
   struct font_pngFont fontStackST;
   FILE *outFILE = fopen("../asciifont.txt", "r");

   init_font_pngFont(&fontStackST);
   if(! outFILE)
      goto err_main;

   errSL = getFont_font_pngFont(&fontStackST, outFILE);
   if(errSL)
      goto err_main;
   fclose(outFILE);
   outFILE = 0;

   /*save the font to delete.c and delete.h*/
   if(
      fontToC_font_pngFont(
         &fontStackST,
         (signed char *) "delete"
      )
   ) goto err_main;

   errSL = 0;
   goto ret_main;

   err_main:;
      errSL = 1;
      goto ret_main;

   ret_main:;
      freeStack_font_pngFont(&fontStackST);

      if(outFILE)
         fclose(outFILE);
      outFILE = 0;

      return (int) errSL;
} /*main*/
