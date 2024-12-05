/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' mainRmBlocks SOF: Start Of File
'   - remove blocks (start ---, end ---) from stdin file
'     and merge block contents onto a line
'   - also removes all comments
'     - formats: #.*\n; #---.*---#; or #---.*---#
'   o header:
'     - included libraries
'   o main:
'     - driver function to remove blocks
'   o license:
'     - licensing for this code (public domain / mit)
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#ifdef PLAN9
   #include <u.h>
   #include <libc.h>
#else
   #include <stdlib.h>
#endif

#include <stdio.h>

#include "../genLib/rmBlocks.h"
#include "../bioTools.h"

/*-------------------------------------------------------\
| Main:
|   - driver function to remove blocks
| Input:
| Output:
|   - Prints:
|     o parsed file to stdout
\-------------------------------------------------------*/
int
main(
   int numArgsSI,
   char *argAryStr[]
){
   signed char errSC = 0;

   unsigned long sizeFileUL = 4096;
   unsigned long lenFileUL = 0;
   unsigned long tmpUL = 0;
   unsigned long emptyUL = 0;

   signed char keepBl = 0;
   signed char *fileHeapStr = 0;
   signed char *tmpStr = 0;
   FILE *inFILE = stdin;
   FILE *outFILE = stdout;

   if(numArgsSI > 1)
   { /*If: priting help message*/
      if(
            argAryStr[1][0] == '-'
         && argAryStr[1][1] == 'k'
      ){ /*If: user wanted to keep comments*/
          keepBl = 1; /*keeping comments*/
          goto getFile_main;
      } /*If: user wanted to keep comments*/

      fprintf(
         stdout,
         "cat in.txt | rmBlocks > out.txt\n"
      );

      fprintf(
         stdout,
        "  - merges all lines in a block (---.*---) and\n"
      );

      fprintf(
         stdout,
         "    removes block entries\n"
      );

      fprintf(
         stdout,
        "  - also removes comments (#.*\\n; #---.*---#)\n"
      );

      fprintf(
         stdout,
         "    o use `-k` to keep comments not in blocks\n"
      );

      fprintf(
         stdout,
         "  - bioTools version: %i-%02i-%02i\n",
         def_year_bioTools,
         def_month_bioTools,
         def_day_bioTools
      );

      errSC = 0;
      goto cleanUp_main;
   } /*If: priting help message*/
  
   getFile_main:;

   fileHeapStr =
      malloc((sizeFileUL + 1) * sizeof(signed char));

   if(! fileHeapStr)
      goto err_main;

   tmpUL =
      fread(
         (char *) fileHeapStr,
         sizeof(signed char),
         sizeFileUL,
         inFILE
      );

   lenFileUL = tmpUL;
    
   while(tmpUL)
   { /*Loop: read in file*/
      emptyUL =
          ((sizeFileUL << 1) >> 1)
         + sizeFileUL
         - lenFileUL;

      sizeFileUL += ((sizeFileUL << 1) >> 1);

      tmpStr =
         realloc(
            fileHeapStr,
            (sizeFileUL + 1) * sizeof(signed char)
         );

      if(! tmpStr)
         goto err_main;

      fileHeapStr = tmpStr;

      tmpUL =
         fread(
            (char *) &fileHeapStr[lenFileUL],
            sizeof(signed char),
            emptyUL,
            inFILE
         );

      lenFileUL += tmpUL;
   } /*Loop: read in file*/

   fileHeapStr[lenFileUL] = '\0';

   tmpStr =
   str_rmBlocks(
      fileHeapStr,
      0,           /*overwriting file*/
      &lenFileUL,
      keepBl
   );

   fwrite(
      fileHeapStr,
      sizeof(signed char),
      lenFileUL,
      outFILE
   );

   free(fileHeapStr);
   fileHeapStr = 0;
   lenFileUL = 0;

   inFILE = 0;
   outFILE = 0;

   errSC = 0;
   goto cleanUp_main;

   err_main:;
      errSC = 1;
      fprintf(stderr, "memory error\n");
      goto cleanUp_main;

   cleanUp_main:;
      if(fileHeapStr)
         free(fileHeapStr);
      fileHeapStr = 0;

      if(
            inFILE
         && inFILE != stdin
      ) fclose(inFILE);
      inFILE = 0;

      if(
            outFILE
         && outFILE != stdout
      ) fclose(outFILE);
      outFILE = 0;

      return errSC;
} /*main*/

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
: Copyright (c) 2024 jeremyButtler
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
