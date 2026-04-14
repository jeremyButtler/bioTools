#ifdef PLAN9
   #include <u.h>
   #include <libc.h>
#else
   #include <stdlib.h>
#endif

#include <stdio.h>
#include "../../genFont/fontST.h"
#include "../mkPng.h"
#include "../pngDraw.h"

int
main(
   void
){
   signed long errSL = 0;
   signed char forSC = 3;

   signed short offsetSS = 0;
   signed char backSC = -1;

   struct st_mkPng *pngHeapST = 0;
   struct font_fontST fontStackST;

   FILE *outFILE =
      fopen("numberTest.txt", "r");

   init_font_fontST(&fontStackST);
   if(! outFILE)
      goto err_main;
   errSL = getFont_font_fontST(&fontStackST, outFILE);
   if(errSL)
      goto err_main;
   fclose(outFILE);
   outFILE = 0;

   pngHeapST = mk_st_mkPng(0, 0, 0);
   if(! pngHeapST)
      goto err_main;

   /*draw some text*/
   errSL =
      drawHorizText_pngDraw(
         (signed char *) "0123456789",
         offsetSS, /*x-axis (column)*/
         100, /*y-axis (row)*/
         forSC,
         backSC,
         &fontStackST,
         pngHeapST
      );
   outFILE = fopen("delete-number-font.png", "w");
   if(! outFILE)
      goto err_main;
   print_st_mkPng(pngHeapST, outFILE);
   fclose(outFILE);
   outFILE = 0;

   if(errSL < 0)
      goto err_main;
   errSL = 0;
   goto ret_main;

   err_main:;
      errSL = 1;
      goto ret_main;

   ret_main:;
      freeStack_font_fontST(&fontStackST);

      if(pngHeapST)
         freeHeap_st_mkPng(pngHeapST);
      pngHeapST = 0;

      if(outFILE)
         fclose(outFILE);
      outFILE = 0;

      return errSL;
} /*main*/
