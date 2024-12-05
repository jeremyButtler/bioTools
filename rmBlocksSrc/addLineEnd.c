/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' addLineEnd SOF: Start Of File
'   - padds each line to 56 characters, adds a \n\ to the
'     end of line, and escapes strings ('"' to '\"')
'   o header:
'     - included libraries
'   o main:
'     - function to add padding and \n\ to end of lines
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
   #include <stdlib.h>
#endif

#include <stdio.h>

#include "../bioTools.h"

/*-------------------------------------------------------\
| Main:
|   - function to add padding and \n\ to end of lines
| Input:
|   - numArgsSI:
|     o number input arguments (if > 1 fires help message)
| Output:
|   - Prints:
|     o file from stdin as padded file to stdout
\-------------------------------------------------------*/
int
main(
   int numArgsSI,
   char * argAryStr[]
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Main TOC:
   '   - padd and add \n\ to end of line
   '   o main sec01:
   '     - variable declarations
   '   o main sec02:
   '     - print help mesage
   '   o main sec03:
   '     - read file into buffer
   '   o main sec04:
   '     - find number newlines
   '   o main sec05:
   '     - allocate output array memory (being lazy)
   '   o main sec06:
   '     - add padding and \n\ in
   '   o main sec07:
   '     - free memory and print output
   '   o main sec08:
   '     - clean up and return
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   signed char errSC = 0;

   unsigned long sizeFileUL = 4096;
   unsigned long lenFileUL = 0;
   unsigned long tmpUL = 0;
   unsigned long emptyUL = 0;
   unsigned long qouteCntUL = 0; /*number "'s*/

   unsigned int endLineUI = 55;

   signed char *fileHeapStr = 0;
   signed char *outHeapStr = 0;
   signed char *tmpStr = 0;
   signed char *dupStr = 0;

   FILE *inFILE = stdin;
   FILE *outFILE = stdout;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec02:
   ^   - print help mesage
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(numArgsSI > 1)
   { /*If: priting help message*/
      fprintf(
         stdout,
         "cat in.txt | addLineEnd > out.txt\n"
      );

      fprintf(
         stdout,
        "  - adds a \\n\\ at column 56, 57, and 58\n"
      );

      fprintf(
         stdout,
         "  - bioTools version: %i-%02i-%02i\n",
         def_year_bioTools,
         def_month_bioTools,
         def_day_bioTools
      );

      errSC = 0;
      goto cleanUp_main_sec08;
   } /*If: priting help message*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec03:
   ^   - read file into buffer
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   fileHeapStr =
      malloc((sizeFileUL + 1) * sizeof(signed char));

   if(! fileHeapStr)
      goto err_main_sec08;

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
         goto err_main_sec08;

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

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec04:
   ^   - find number newlines
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   tmpStr = fileHeapStr;
   tmpUL = 0;
   sizeFileUL = 0;

   while(*tmpStr != '\0')
   { /*Loop: find number of new lines*/
      if(*tmpStr == '\n')
         ++tmpUL;

      if(*tmpStr == '"')
         ++qouteCntUL; /*get number of '"'s*/

      if(*tmpStr == '\\')
         ++qouteCntUL; /*get number of '\\'s*/

      if(sizeFileUL >= endLineUI)
         ++emptyUL; /*extra characters past line break*/

      ++tmpStr;
      ++sizeFileUL;
   } /*Loop: find number of new lines*/

   ++tmpUL; /*account for index 1*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec05:
   ^   - allocate output array memory (being lazy)
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   sizeFileUL =
      ( emptyUL + qouteCntUL + (tmpUL * endLineUI + 3) );
      /* Logic:
      `    - tmpUL:
      `      o has the number of lines
      `    - pad: endLineUI * tmpUL:
      `      o tmpUL * endLineUI = padded file size
      `      o endLineUI has line break (point to stop
      `        adding padding)
      `    - maxLine: pad + 3:
      `      o file size after padding and adding \n\
      `      o +3 accoutns for adding \n\
      `    - extra: maxLine + quoteCntUL:
      `      o get file length with escaped strings
      `      o quoteCntUL has number of string quotes that
      `        need escaping
      `    - fileSize: emptyUL + maxLine:
      `      o total expected final file size
      `      o emptyUL has number of bytes exceding
      `        line break limit
      */

   outHeapStr =
      malloc((sizeFileUL + 1) * sizeof(signed char));

   if(! outHeapStr)
      goto err_main_sec08;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec06:
   ^   - add padding and \n\ in
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   tmpStr = fileHeapStr;
   dupStr = outHeapStr;
   lenFileUL = 0;

   while(*tmpStr != '\0')
   { /*Loop: though all lines in the file*/
      tmpUL = 0;

      while(*tmpStr != '\n')
      { /*Loop: find line length*/
         if(*tmpStr == '\0')
            break;

         if(*tmpStr == '"')
         { /*If: have a quote*/
            *dupStr++ = '\\';
            ++tmpUL;
         } /*If: have a quote*/

         if(*tmpStr == '\\')
         { /*If: have a backsash (need to escape)*/
            *dupStr++ = '\\';
            ++tmpUL;
         } /*If: have a backsash (need to escape)*/

         ++tmpUL;
         *dupStr++ = *tmpStr++;
      } /*Loop: find line length*/
      
      while(tmpUL < endLineUI)
      { /*Loop: add white space till break column*/
         *dupStr++ = ' ';
         ++tmpUL;
      } /*Loop: add white space till break column*/

      *dupStr++ = '\\';
      ++lenFileUL;

      *dupStr++ = 'n';
      ++lenFileUL;

      *dupStr++ = '\\';
      ++lenFileUL;

      *dupStr++ = '\n';
      ++lenFileUL;

      ++tmpStr;
      lenFileUL += tmpUL;
   } /*Loop: though all lines in the file*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec07:
   ^   - free memory and print output
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   free(fileHeapStr);
   fileHeapStr = 0;

   if(inFILE != stdin)
      fclose(inFILE);
   inFILE = 0;


   fwrite(
      outHeapStr,
      sizeof(signed char),
      lenFileUL,
      outFILE
   );


   free(outHeapStr);
   outHeapStr = 0;

   if(outFILE != stdout)
      fclose(outFILE);
   outFILE = 0;
    
   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec08:
   ^   - clean up and return
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   errSC = 0;
   goto cleanUp_main_sec08;

   err_main_sec08:;
      errSC = 1;
      fprintf(stderr, "memory error\n");
      goto cleanUp_main_sec08;

   cleanUp_main_sec08:;
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

      if(fileHeapStr)
         free(fileHeapStr);
      fileHeapStr = 0;

      if(outHeapStr)
         free(outHeapStr);
      outHeapStr = 0;

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
