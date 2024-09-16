/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' getSeq SOF: Start Of File
'   - holds functions used to process sequence files,
'     such as fastq, fasta, and sam files
'   o fun01: pId_getSeq
'     - prints or adds ouput to buffer
'   o fun02 ulfq_getSeq:
'     - gets next fastq entry and prints out old fastq
'       entry if file provided (using longs)
'   o fun03 ulsam_getSeq:
'     - gets next sam entry and prints out old sam entry
'       if file provided using bytes
'   o license:
'     - licensing for this code (public domain / mit)
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#ifdef PLAN9
   #include <u.h>
   #include <libc.h>
#endif

#include <stdio.h>

#include "../genLib/ulCp.h"
#include "getSeq.h"
#include "vectScan.h"

/*.h files only*/
#include "../genLib/dataTypeShortHand.h"

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\
! Hidden Libraries:
!   - std #include <immintrin.h> #ifdef SSE/AVX2/AVX512
!   - std #include <arm_neon.h>  #ifdef NEON
\%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

/*-------------------------------------------------------\
| Fun01: pId_getSeq
|   - prints or adds ouput to buffer
| Input:
|   - buffStr:
|     o c-string with buffer to copy contents from
|   - posUL:
|     o end position of copy for buffStr
|   - startPosUL:
|     o start position to copy for buffStr
|   - outBuffStr:
|     o buffer to copy output to
|   - outPosUL:
|     o position to start copy to the output buffer
|   - lenBuffUI:
|     o length of input (buffStr) & output (outBuffStr)
|       buffers
|   - outFILE:
|     o file to print to if outBuffStr is full
| Output:
|   - Modifies:
|     o outPosUL to be at end of copy
|     o startPosUL to be at start of array
|     o outBuffStr to have new contents
|   - Prints:
|     o outBuffStr to outFILE if outBuffStr is full
\-------------------------------------------------------*/
void
pId_getSeq(
   signed char *buffStr,
   unsigned long posUL,
   unsigned long *startPosUL,
   signed char *outBuffStr,
   unsigned long *outPosUL, 
   unsigned int lenBuffUI,
   void *outFILE
){
   if(lenBuffUI - *outPosUL - 1 < posUL - *startPosUL)
   { /*If: I need to print the output buffer*/ 
      fprintf(
         (FILE *) outFILE,
         "%s",
         outBuffStr
      ); /*print out buffer*/

      *outPosUL = 0;
   } /*If: I need to print the output buffer*/

   cpLen_ulCp(
      &outBuffStr[*outPosUL],
      &buffStr[*startPosUL],
      posUL - *startPosUL
   );

   *outPosUL += posUL - *startPosUL;
   *startPosUL = 0;
} /*pId_getSeq*/

/*--------------------------------------------------------\
| Fun02 ulfq_getSeq:
|   - gets next fastq entry and prints out old fastq
|     entry if file provided using bytes
| Input:
|   - buffStr:
|     o c-string with fastq file buffer to find next entry
|       in or update
|   - lenBuffUI:
|     o length of buffStr
|   - bytesUL:
|     o pointer to a unsigned long with the number of
|       bytes in buffer (updated as needed)
|   - fqFILE:
|     o fastq FILE with reads to extract
|   - outFILE:
|     o FILE to print fastq entry to
| Output:
|  - Prints:
|    o Reads to outFILE if outFILE points to a file
|  - Modifies:
|    o buffStr to have the next buffer if empty
|    o incurments pointInBufferCStr to start of next read
|  - Returns:
|   o 0 if nothing went wrong
|   o 1 If the end of the file
|   o 4 If ran out of file
\--------------------------------------------------------*/
unsigned char
ulfq_getSeq(
    signed char *buffStr,   /*buffer with input to scan*/
    unsigned long *posUL,   /*Position in buffer*/
    signed char *outBuffStr,/*holds reads to print*/
    unsigned long *outPosUL,/*position at in outBuffStr*/
    unsigned int lenBuffUI, /*Size of buffer*/
    unsigned long *bytesUL, /*Number chars in buffStr*/
    FILE *fqFILE,           /*Fastq file with input*/
    FILE *outFILE           /*file to output reads to*/
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun02b TOC:
   '  - Moves the the next fastq entry and prints to the
   '    output file if an output file was provided.
   '  - This is the vector free version of
   '    fq_getSeq
   '  o fun02 sec01:
   '    -  Variable declerations
   '  o fun02 sec02:
   '    - check if need to get more buffer
   '  o fun02 sec03:
   '    - move past header
   '  o fun02 sec04:
   '    - find number of lines in sequence entry
   '  o fun02 sec05:
   '     - move past spacer entry
   '  o fun02 Sec06:
   '    - move past q-score entry
   '  o fun02 Sec07:
   '    - adjust positoin for the next read
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

    /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
    ^ Fun02 Sec01:
    ^   - variable declerations
    \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

    ulong startPosUL = *posUL;
    slong lenSeqSL = 0;

    /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
    ^ Fun02 Sec02:
    ^   - check if need to get more buffer
    \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

    if(buffStr[*posUL] == '\0')
    { /*If need to get more input*/
      if(*bytesUL < lenBuffUI)
         return 1; /*EOF*/

      *bytesUL =
        fread(
           (char *) buffStr,
           sizeof(char),
           lenBuffUI - 1,
           fqFILE
        ); /*Read in more of the file*/

      if(! *bytesUL)
         return 1; /*EOF*/

      *posUL = 0;
      buffStr[*bytesUL] = '\0';
    } /*If need to get more input*/

    if(*posUL == 0)
       startPosUL = 0;

    /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
    ^ Fun02 Sec03:
    ^   - move past header
    \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

    *posUL += endLine_vectScan(&buffStr[*posUL]);

    while(buffStr[*posUL] == '\0')
    { /*Loop: move past header*/

       if(*bytesUL == 0)
           return 4;      /*fastq has only header*/

       if(outFILE)
       { /*If: I am writing output to a file*/
          pId_getSeq(
             buffStr,
             *posUL,
             &startPosUL,
             outBuffStr,
             outPosUL,
             lenBuffUI,
             outFILE
         );
       } /*If: I am writing output to a file*/

       *bytesUL =
         fread(
            (char *) buffStr,
            sizeof(char),
            lenBuffUI - 1,
            fqFILE
         ); /*Read in more of the file*/

       *posUL = 0;
       buffStr[*bytesUL] = '\0';

       *posUL += endLine_vectScan(&buffStr[*posUL]);
    } /*Loop: move past header*/

    ++(*posUL); /*Get off new line*/

    /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
    ^ Fun02 Sec04:
    ^   - find number of lines in sequence entry
    \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

    lenSeqSL = (signed long) (*posUL - 1) * -1;

    while(buffStr[*posUL] != '+')
    { /*Loop: get past sequence entry*/
        *posUL += endLine_vectScan(&buffStr[*posUL]);

        if(buffStr[*posUL] == '\0')
        { /*If I need to read in more input*/
           lenSeqSL += *posUL;

           if(*bytesUL == 0)
               return 4;      /*fastq has only header*/

           if(outFILE)
           { /*If: I am writing output to a file*/
              pId_getSeq(
                 buffStr,
                 *posUL,
                 &startPosUL,
                 outBuffStr,
                 outPosUL,
                 lenBuffUI,
                 outFILE
             );
           } /*If: I am writing output to a file*/

            *bytesUL =
              fread(
                 (char *) buffStr,
                 sizeof(char),
                 lenBuffUI - 1,
                 fqFILE
              );

            *posUL = 0;
            buffStr[*bytesUL] = '\0';

            continue;
        } /*If: I need to read in more input*/

        ++(*posUL); /*is new line*/
    } /*Loop: get past sequence entry*/

    lenSeqSL += (*posUL) - 1;
    ++(*posUL); /*Get off '+'*/

    /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
    ^ Fun02 Sec05:
    ^  - move past spacer entry
    \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

    *posUL += endLine_vectScan(&buffStr[*posUL]);

    while(buffStr[*posUL] == '\0')
    { /*Loop: get past spacer entry*/

       if(*bytesUL == 0)
           return 4;         /*no Q-score entry*/

       if(outFILE)
       { /*If: I am writing output to a file*/
          pId_getSeq(
             buffStr,
             *posUL,
             &startPosUL,
             outBuffStr,
             outPosUL,
             lenBuffUI,
             outFILE
         );
       } /*If: I am writing output to a file*/

       *bytesUL =
         fread(
            (char *) buffStr,
            sizeof(char),
            lenBuffUI - 1,
            fqFILE
         );

       *posUL = 0;
       buffStr[*bytesUL] = '\0';

       *posUL += endLine_vectScan(&buffStr[*posUL]);
    } /*Loop: get past spacer entry*/

    ++(*posUL); /*Get off new line*/

    /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
    ^ Fun02 Sec06:
    ^  - move past q-score entry
    ^  - qscore entry has same number of lines as sequence
    \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

    while(lenSeqSL + *posUL >= *bytesUL)
    { /*Loop: read in rest of q-score entry*/
       lenSeqSL -= (*bytesUL - *posUL);

       if(outFILE)
       { /*If: I am writing output to a file*/
          pId_getSeq(
             buffStr,
             *bytesUL,
             &startPosUL,
             outBuffStr,
             outPosUL,
             lenBuffUI,
             outFILE
         );
       } /*If: I am writing output to a file*/

       if(*bytesUL == 0)
       { /*If: at end of file*/
           if(lenSeqSL <= 0)
              return 1;
           else
              return 4;  /*incomplete q-score entry*/
       } /*If: at end of file*/

       *bytesUL =
         fread(
            (char *) buffStr,
            sizeof(char),
            lenBuffUI - 1,
            fqFILE
         );

       *posUL = 0;
       buffStr[*bytesUL] = '\0';
    } /*Loop: read in rest of q-score entry*/

    *posUL += lenSeqSL; /*end of q-score entry*/
    /*new line is included*/

    /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
    ^ Fun02 Sec07:
    ^   - adjust position for next read
    \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

    if(outFILE)
    { /*If: I am writing output to a file*/
       pId_getSeq(
          buffStr,
          *posUL,
          &startPosUL,
          outBuffStr,
          outPosUL,
          lenBuffUI,
          outFILE
      );
    } /*If: I am writing output to a file*/

    /*This comparision is needed for extracting future
    `  values. Oddly enough it also makes it faster
    `  then the buffStr[*posUL] == '\0'. I am guessing
    `  the 255 makes it faster becuase it makes the other
    `  ifs less likely. Again branch prediction penalty
    */
    if(*bytesUL - *posUL < 255)
    { /*If: I need to get more input*/
      if(! *bytesUL && buffStr[*posUL] == '\0')
         return 1;

      cpLen_ulCp(
         buffStr,
         &buffStr[*posUL],
         *bytesUL - *posUL
      );

      *posUL = *bytesUL - *posUL;

      /*Read in more of the file*/
      *bytesUL =
        fread(
           &buffStr[*posUL],
           sizeof(char),
           lenBuffUI - *posUL,
           fqFILE
        );

      *bytesUL += *posUL;

      buffStr[*bytesUL] = '\0';
      *posUL = 0;

       if(! *bytesUL)
          return 1;
    } /*If: I need to get more input*/

    return 0;
} /*ulfq_getSeq*/

/*--------------------------------------------------------\
| Fun03 ulsam_getSeq:
|   - gets next sam entry and prints out old sam entry if
|     file provided using bytes
| Input:
|   - buffStr:
|     o c-string with fastq file buffer to find next entry
|       in or update
|   - lenBuffUI:
|     o length of buffStr
|   - bytesUL:
|     o pointer to a unsigned long with the number of
|       bytes in buffer (updated as needed)
|   - samFILE:
|     o sam FILE with reads to extract
|   - outFILE:
|     o FILE to print fastq entry to
| Output:
|  - Prints:
|    o Reads to outFILE if outFILE points to a file
|  - Modifies:
|    o buffStr to have the next buffer if empty
|    o incurments pointInBufferCStr to start of next read
|  - Returns:
|   o 0 if nothing went wrong
|   o 1 If the end of the file
|   o 4 If ran out of file
\--------------------------------------------------------*/
unsigned char
ulsam_getSeq(
    signed char *buffStr,   /*buffer with input to scan*/
    unsigned long *posUL,   /*Position in buffer*/
    signed char *outBuffStr,/*holds reads to print*/
    unsigned long *outPosUL,/*position at in outBuffStr*/
    unsigned int lenBuffUI, /*Size of buffer*/
    unsigned long *bytesUL, /*Number chars in buffStr*/
    FILE *samFILE,           /*Fastq file with input*/
    FILE *outFILE           /*file to output reads to*/
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun03b TOC:
   '  - Moves the the next sam entry and prints to the
   '    output file if an output file was provided.
   '  - This is the vector free version of
   '    fq_getSeq
   '  o fun03 sec01:
   '    -  Variable declerations
   '  o fun03 sec02:
   '    - check if need to get more buffer
   '  o fun03 sec03:
   '    - move past sam entry
   '  o fun03 Sec04:
   '    - adjust positoin for the next read
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

    /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
    ^ Fun03 Sec01:
    ^   - variable declerations
    \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

    ulong startPosUL = *posUL;

    /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
    ^ Fun03 Sec02:
    ^   - check if need to get more buffer
    \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

    if(buffStr[*posUL] == '\0')
    { /*If need to get more input*/
      if(*bytesUL < lenBuffUI)
         return 1; /*EOF*/

      *bytesUL =
        fread(
           (char *) buffStr,
           sizeof(char),
           lenBuffUI - 1,
           samFILE
        ); /*Read in more of the file*/

      if(! *bytesUL)
         return 1; /*EOF*/

      *posUL = 0;
      buffStr[*bytesUL] = '\0';
    } /*If need to get more input*/

    if(*posUL == 0)
       startPosUL = 0;

    /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
    ^ Fun03 Sec03:
    ^   - move past sam entry
    \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

    *posUL += endLine_vectScan(&buffStr[*posUL]);

    while(buffStr[*posUL] == '\0')
    { /*Loop: move past header*/

       if(*bytesUL == 0)
           return 4;      /*fastq has only header*/

       if(outFILE)
       { /*If: I am writing output to a file*/
          pId_getSeq(
             buffStr,
             *posUL,
             &startPosUL,
             outBuffStr,
             outPosUL,
             lenBuffUI,
             outFILE
         );
       } /*If: I am writing output to a file*/

       *bytesUL =
         fread(
            (char *) buffStr,
            sizeof(char),
            lenBuffUI - 1,
            samFILE
         ); /*Read in more of the file*/

       *posUL = 0;
       buffStr[*bytesUL] = '\0';

       *posUL += endLine_vectScan(&buffStr[*posUL]);
    } /*Loop: move past header*/

    ++(*posUL); /*Get off new line*/

    /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
    ^ Fun03 Sec04:
    ^   - adjust position for next read
    \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

    if(outFILE)
    { /*If: I am writing output to a file*/
       pId_getSeq(
          buffStr,
          *posUL,
          &startPosUL,
          outBuffStr,
          outPosUL,
          lenBuffUI,
          outFILE
      );
    } /*If: I am writing output to a file*/

    /*This comparision is needed for extracting future
    `  values. Oddly enough it also makes it faster
    `  then the buffStr[*posUL] == '\0'. I am guessing
    `  the 255 makes it faster becuase it makes the other
    `  ifs less likely. Again branch prediction penalty
    */
    if(*bytesUL - *posUL < 255)
    { /*If: I need to get more input*/
      if(! *bytesUL && buffStr[*posUL] == '\0')
         return 1;

      cpLen_ulCp(
         buffStr,
         &buffStr[*posUL],
         *bytesUL - *posUL
      );

      *posUL = *bytesUL - *posUL;

      /*Read in more of the file*/
      *bytesUL =
        fread(
           &buffStr[*posUL],
           sizeof(char),
           lenBuffUI - *posUL,
           samFILE
        );

      *bytesUL += *posUL;

      buffStr[*bytesUL] = '\0';
      *posUL = 0;

       if(! *bytesUL)
          return 1;
    } /*If: I need to get more input*/

    return 0;
} /*ulsam_getSeq*/

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
