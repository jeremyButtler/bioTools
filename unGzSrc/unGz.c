/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' unGz SOF: Start Of File
'   - uncompresses gz files
'   o header:
'     - included libraries
'   o fun01: pversion_unGz
'     - prints version for unGz
'   o fun02: phelp_unGz
'     - prints help message for unGz
'   o license:
'     - licensing for this code (public domain / mit)
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*-------------------------------------------------------\
| Header:
|   - included libraries
\-------------------------------------------------------*/

#ifdef PLAN9
   #include <u.h>
   #include <libc.h>
#else
#endif

#include <stdio.h>

#include "../genLib/ulCp.h"
#include "../genLib/inflate.h"

/*using only the .h file*/
#include "../genLib/endLine.h"
#include "../bioTools.h"

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\
! Hidden Libraries:
!   - std #include <stdlib.h>
!   - .c  #include "../genLib/endin.h"
!   - .c  #include "../genLib/checksum.h"
!   - .c  #include "../genLib/genMath.h"
\%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

/*-------------------------------------------------------\
| Fun01: pversion_unGz
|   - prints version for unGz
| Input:
|   - outFILE:
|     o FILE pointer to print version number to
| Output:
|   - Prints:
|     o version number to outFILE
\-------------------------------------------------------*/
void
pversion_unGz(
   void *outFILE
){
   fprintf(
      (FILE *) outFILE,
      "unGz from bioTools version: %i-%02i-%02i%s",
      def_year_bioTools,
      def_month_bioTools,
      def_day_bioTools,
      str_endLine
   );
} /*pversion_unGz*/

/*-------------------------------------------------------\
| Fun02: phelp_unGz
|   - prints help message for unGz
| Input:
|   - outFILE:
|     o FILE pointer to print help message to
| Output:
|   - Prints:
|     o help message to outFILE
\-------------------------------------------------------*/
void
phelp_unGz(
   void *outFILE
){
   fprintf(
      (FILE *) outFILE,
      "unGz file1.gz file2.gz file3.gz ... filen.gz%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "  - prints uncompressed output to stdout%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "  - use `-` for stdin input%s",
      str_endLine
   );
} /*phelp_unGz*/

/*-------------------------------------------------------\
| Main:
|   - driver function for unGz
| Input:
|   - argAryStr:
|     o c-string array with files to uncompress
|   - numArgsSI:
|     o number of files input
| Output:
|   - Prints:
|     o file contents to stdout
|     o errors to stderr
\-------------------------------------------------------*/
int
main(
   int numArgsSI,
   char *argAryStr[]
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Main TOC:
   '   - driver function for unGz
   '   o main sec01:
   '     - variable declarations
   '   o main sec02:
   '     - initialize and check for version/help message
   '   o main sec03:
   '     - unzip files
   '   o main sec04:
   '     - print errors, clean up, and return
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec01:
   ^   - variable declarations and initialize
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   #define def_buffSize_main 4096

   signed char errSC = 0;

   struct file_inflate zipStackST;
   signed char outStr[def_buffSize_main];
   signed long bytesSL = 0;

   int argSI = 1;
   FILE *inFILE = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec02:
   ^   - initialize and check for version/help message
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   for(bytesSL =0; bytesSL < def_buffSize_main; ++bytesSL)
      outStr[bytesSL] = '\0';
   bytesSL = 0;
   init_file_inflate(&zipStackST);

   if(numArgsSI <= 1)
   { /*If: nothing input*/
      phelp_unGz(stdout);
      goto noInput_main_sec04;
   } /*If: nothing input*/

   if(
         argAryStr[1][0] == '-'
      && argAryStr[1][1] == 'h'
   ){ /*If: user wanted help message*/
      errSC = 0;
      phelp_unGz(stdout);
      goto ret_main_sec04;
   } /*If: user wanted help message*/

   if(
         argAryStr[1][0] == '-'
      && argAryStr[1][1] == '-'
      && argAryStr[1][2] == 'h'
   ){ /*If: user wanted help message*/
      errSC = 0;
      phelp_unGz(stdout);
      goto ret_main_sec04;
   } /*If: user wanted help message*/

   if(
         argAryStr[1][0] == 'h'
      && argAryStr[1][1] == 'e'
      && argAryStr[1][2] == 'l'
      && argAryStr[1][3] == 'p'
      && argAryStr[1][4] == '\0'
   ){ /*If: user wanted help message*/
      errSC = 0;
      phelp_unGz(stdout);
      goto ret_main_sec04;
   } /*If: user wanted help message*/

   if(
         argAryStr[1][0] == '-'
      && argAryStr[1][1] == 'v'
   ){ /*If: user wanted version number*/
      errSC = 0;
      pversion_unGz(stdout);
      goto ret_main_sec04;
   } /*If: user wanted version number*/

   if(
         argAryStr[1][0] == '-'
      && argAryStr[1][1] == '-'
      && argAryStr[1][2] == 'v'
   ){ /*If: user wanted version number*/
      errSC = 0;
      pversion_unGz(stdout);
      goto ret_main_sec04;
   } /*If: user wanted version number*/

   if(
         argAryStr[1][0] == 'v'
      && argAryStr[1][1] == 'e'
      && argAryStr[1][2] == 'r'
      && argAryStr[1][3] == 's'
      && argAryStr[1][4] == 'i'
      && argAryStr[1][5] == 'o'
      && argAryStr[1][6] == 'n'
      && argAryStr[1][7] == '\0'
   ){ /*If: user wanted version number*/
      errSC = 0;
      pversion_unGz(stdout);
      goto ret_main_sec04;
   } /*If: user wanted version number*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec03:
   ^   - unzip files
   ^   o main sec03 sub01:
   ^     - add gz file to zipStackST + start loop
   ^   o main sec03 sub02:
   ^     - inflate file (inflate loop)
   ^   o main sec03 sub03:
   ^     - move to next file
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Main Sec03 Sub01:
   *   - add gz file to zipStackST + start loop
   \*****************************************************/

   while(argSI < numArgsSI)
   { /*Loop: inflate gz files*/
      inFILE = fopen(argAryStr[argSI], "rb");
      if(! inFILE)
         goto openErr_main_sec04;

      errSC = add_file_inflate(inFILE, &zipStackST, 0);
      inFILE = 0;

      if(errSC == def_eof_inflate)
      { /*If: nothing in file (likely)*/
         fprintf(
            stderr,
            "nothing in %s; moving to next file%s",
            argAryStr[argSI],
            str_endLine
         );
      } /*If: nothing in file (likely)*/

      else if(errSC)
         goto fileErr_main_sec04;

      else if(zipStackST.typeSC != def_gz_inflate)
         goto notGz_main_sec04;

      /**************************************************\
      * Main Sec03 Sub02:
      *   - inflate file (inflate loop)
      \**************************************************/

      while(! errSC)
      { /*Loop: inflate file*/
         uncompress_main_sec03:;

         bytesSL =
            get_inflate(
               &zipStackST,
               def_buffSize_main,
               0,
               outStr,
               &errSC
            );

         if(bytesSL >= 0)
            ; /*no errors*/
         else if(errSC == def_memErr_inflate)
            goto memErr_main_sec04;
         else if(errSC != def_eof_inflate)
            goto fileErr_main_sec04;

         printf("%s", outStr);
      } /*Loop: inflate file*/

      /**************************************************\
      * Main Sec03 Sub03:
      *   - move to next file
      \**************************************************/

      errSC = next_file_inflate(&zipStackST, 0, 0);

      if(! errSC)
         goto uncompress_main_sec03; /*more in file*/
      else if(errSC == def_badCheck_inflate)
         goto badCheck_main_sec04;
      else if(errSC != def_eof_inflate)
         goto fileErr_main_sec04;
      else
         ++argSI;

      blank_file_inflate(&zipStackST, 1);
   } /*Loop: inflate gz files*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec04:
   ^   - print errors, clean up, and return
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   errSC = 0;
   goto ret_main_sec04;

   noInput_main_sec04:;
      fprintf(stderr, "nothing input%s", str_endLine);
      errSC = 1;
      goto ret_main_sec04;

   notGz_main_sec04:;
      fprintf(
         stderr,
         "%s is not a gz file%s",
         argAryStr[argSI],
         str_endLine
      );
      errSC = 2;
      goto ret_main_sec04;

   memErr_main_sec04:;
      fprintf(
         stderr,
         "memory error ucompressing %s%s",
         argAryStr[argSI],
         str_endLine
      );
      errSC = 3;
      goto ret_main_sec04;

   fileErr_main_sec04:;
      fprintf(
         stderr,
         "%s is not in gz format%s",
         argAryStr[argSI],
         str_endLine
      );
      errSC = 4;
      goto ret_main_sec04;

   openErr_main_sec04:;
      fprintf(
         stderr,
         "could not open %s%s",
         argAryStr[argSI],
         str_endLine
      );
      errSC = 5;
      goto ret_main_sec04;

   badCheck_main_sec04:;
      fprintf(
         stderr,
         "gz file %s is corupted%s",
         argAryStr[argSI],
         str_endLine
      );
      errSC = 6;
      goto ret_main_sec04;

   ret_main_sec04:;
      freeStack_file_inflate(&zipStackST);
      if(
            inFILE
         && inFILE != stdin
         && inFILE != stdout
         && inFILE != stderr
      ) fclose(inFILE);
      inFILE = 0;

      return errSC;
} /*main*/

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
: Copyright (c) 2025 jeremyButtler
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
