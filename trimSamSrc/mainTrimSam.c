/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' mainTrimSam SOF: Start Of File
'   - driver for triming sam file entries 
'   o fun01: pversion_mainTrimSam
'     - prints version number
'   o fun02: phelp_mainTrimSam
'     - prints help message
'   o fun03: input_mainTrimSam
'     - gets user input from input arguments
'   o main:
'     -  main function to glue everything together
'   o license:
'     - Licensing for this code (public domain / mit)
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*I still will have issues with fprintf on plan9, so this
`  is just one part of the problem
*/
#ifdef PLAN9
   #include <u.h>
   #include <libc.h> /*think includes <string.h>*/
#else
   #include <stdlib.h>
   #include <string.h>
#endif

#include <stdio.h>

#include "../genBio/trimSam.h"
#include "../genBio/samEntry.h"

/*no .c files*/
#include "../genLib/endLine.h"
#include "../bioTools.h"

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\
! Hidden libraries:
!   - .c  #include "../genLib/ulCp.h"
!   - .c  #include "../genLib/numToStr.h"
!   - .c  #include "../genLib/base10str.h"
!   - .c  #include "../genLib/fileFun.h"
!   - .h  #include "../genBio/ntTo5Bit.h"
\%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

#define def_keepNoMap_mainTrimSam 0

/*-------------------------------------------------------\
| Fun01: pversion_mainTrimSam
|   - prints version number
| Input:
|   - outFILE:
|     o file to print version number to
| Output:
|   - Prints:
|     o version number to outFILE
\-------------------------------------------------------*/
void
pversion_mainTrimSam(
   void *outFILE
){
   fprintf(
      (FILE *) outFILE,
      "trimSam from bioTools version: %i-%02i-%02i%s",
      def_year_bioTools,
      def_month_bioTools,
      def_day_bioTools,
      str_endLine
   );
} /*pversion_mainTrimSam*/

/*-------------------------------------------------------\
| Fun02: phelp_mainTrimSam
|   - prints help message
| Input:
|   - outFILE:
|     o file to print help message to
| Output:
|   - Prints:
|     o help message to outFILE
\-------------------------------------------------------*/
void
phelp_mainTrimSam(
   void *outFILE
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun02 TOC:
   '   - prints help message
   '   o fun02 sec01:
   '      - usage block
   '   o fun02 sec02:
   '      - input block
   '   o fun02 sec03:
   '      - output block
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun02 Sec01:
   ^   - usage block
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   fprintf(
      (FILE *) outFILE,
      "trimSam -sam reads.sam [options ...] > out.sam%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "  - removes softmaskng from reads in a sam file%s",
      str_endLine
   );

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun02 Sec02:
   ^   - input block
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   fprintf(
      (FILE *) outFILE,
      "Input:%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "  -sam reads.sam: [Required; stdin]%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "    o sam file with reads to trim%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "  -out out.sam: [stdout]%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "    o sam file to print trimmed reads to%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "  -keep-no-map: [Optional; No]%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "    o keep unmapped reads%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "    o disable with -no-unmapped%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "  -h: prints this help message and exits%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "  -v: prints version number and exits%s",
      str_endLine
   );

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun02 Sec03:
   ^   - output block
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   fprintf(
      (FILE *) outFILE,
      "Output:%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "   - prints trimmed reads to stdout or -out%s",
      str_endLine
   );
} /*phelp_mainTrimSam*/

/*-------------------------------------------------------\
| Fun03: input_mainTrimSam
|   - gets user input from input arguments
| Input:
|   - lenArgsSI:
|     o number of aguments input
|   - argAryStr:
|     o c-string array with user input
|   - samFileStr:
|     o pointer to c-string to point to sam file name in
|       argAryStr
|   - outFileStr:
|     o pointer to c-string to point to output file name
|       in argAryStr
|   - keepNoMapBl:
|     o set to 1 if keeping unmapped reads
|     o set to 0 if discarding unmapped reads
| Output:
|   - Modifies:
|     o input variables (except lenArgsSI and argAryStr)
|       to hold or point to user input
|   - Returns:
|     o 0: if valid input
|     o 1: if printed help message/version number
|     o 2: if invalid input
\-------------------------------------------------------*/
char input_mainTrimSam(
    int lenArgsSI,           /*number arguments input*/
    char *argAryStr[],       /*array with user arguments*/
    signed char **samFileStr,/*file name of input file*/
    signed char **outFileStr,/*file name of output file*/
    signed char *keepNoMapBl /*keeping unmapped reads?*/
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun03 TOC:
   '   - gets user input from input arguments
   '   o fun03 sec01:
   '     - variable declerations
   '   o fun03 sec02:
   '     - check if no input provided
   '   o fun03 sec03:
   '     - get user input
   '   o fun03 sec04:
   '     - clean up
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

    /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
    ^ Fun03 Sec01:
    ^   - variable declerations
    \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

    signed char errSC = 0;
    signed int siArg = 1;

    /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
    ^ Fun03 Sec02:
    ^   - check if no input provided
    \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

    if(lenArgsSI <= 1)
    { /*If: no user input; print help message*/
       phelp_mainTrimSam(stdout);
       goto phelp_fun03_sec04;
    } /*If: no user input; print help message*/

    /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
    ^ Fun03 Sec03:
    ^   - get user input
    ^   o fun03 sec03 sub01:
    ^     - start loop and check for normal input
    ^   o fun03 sec03 sub02:
    ^     - check for help message requests
    ^   o fun03 sec03 sub03:
    ^     - check for version number requests
    ^   o fun03 sec03 sub04:
    ^     - handel invalid input
    ^   o fun03 sec03 sub05:
    ^     - move to next argument
    <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

    /****************************************************\
    * Fun03 Sec03 Sub01:
    *   - start loop and check for normal input
    \****************************************************/

    while(siArg < lenArgsSI)
    { /*loop through all user input arguments*/
        if(! strcmp("-sam", argAryStr[siArg]))
        { /*If: This is the input file*/
           ++siArg;
           *samFileStr = (signed char *) argAryStr[siArg];
        } /*If: This is the input file*/

        else if(! strcmp("-out", argAryStr[siArg]))
        { /*Else If: This is the output file*/
           ++siArg;
           *outFileStr = (signed char *) argAryStr[siArg];
        } /*Else If: This is the output file*/

        else if(! strcmp("-keep-no-map",argAryStr[siArg]))
           *keepNoMapBl = 1;

        else if(! strcmp("-no-unmapped",argAryStr[siArg]))
           *keepNoMapBl = 0;

        /************************************************\
        * Fun03 Sec03 Sub02:
        *   - check for help message requests
        \************************************************/

        else if(! strcmp("-h", argAryStr[siArg]))
        { /*Else If: was help message*/
           phelp_mainTrimSam(stdout);
           goto phelp_fun03_sec04;
        } /*Else If: was help message*/

        else if(! strcmp("--h", argAryStr[siArg]))
        { /*Else If: was help message*/
           phelp_mainTrimSam(stdout);
           goto phelp_fun03_sec04;
        } /*Else If: was help message*/

        else if(! strcmp("help", argAryStr[siArg]))
        { /*Else If: was help message*/
           phelp_mainTrimSam(stdout);
           goto phelp_fun03_sec04;
        } /*Else If: was help message*/

        else if(! strcmp("-help", argAryStr[siArg]))
        { /*Else If: was help message*/
           phelp_mainTrimSam(stdout);
           goto phelp_fun03_sec04;
        } /*Else If: was help message*/

        else if(! strcmp("--help", argAryStr[siArg]))
        { /*Else If: was help message*/
           phelp_mainTrimSam(stdout);
           goto phelp_fun03_sec04;
        } /*Else If: was help message*/

        /************************************************\
        * Fun03 Sec03 Sub03:
        *   - check for version number requests
        \************************************************/

        else if(! strcmp("-v", argAryStr[siArg]))
        { /*Else If: was version number*/
           pversion_mainTrimSam(stdout);
           goto pversion_fun03_sec04;
        } /*Else If: was version number*/

        else if(! strcmp("--v", argAryStr[siArg]))
        { /*Else If: was version number*/
           pversion_mainTrimSam(stdout);
           goto pversion_fun03_sec04;
        } /*Else If: was version number*/

        else if(! strcmp("version", argAryStr[siArg]))
        { /*Else If: was version number*/
           pversion_mainTrimSam(stdout);
           goto pversion_fun03_sec04;
        } /*Else If: was version number*/

        else if(! strcmp("-version", argAryStr[siArg]))
        { /*Else If: was version number*/
           pversion_mainTrimSam(stdout);
           goto pversion_fun03_sec04;
        } /*Else If: was version number*/

        else if(! strcmp("--version", argAryStr[siArg]))
        { /*Else If: was version number*/
           pversion_mainTrimSam(stdout);
           goto pversion_fun03_sec04;
        } /*Else If: was version number*/

        /************************************************\
        * Fun03 Sec03 Sub04:
        *   - handel invalid input
        \************************************************/

        else
        { /*Else: invalid input*/
           fprintf(
              stderr,
              "%s is not recognized%s",
              argAryStr[siArg],
              str_endLine
           );

           goto err_fun03_sec04;
        } /*Else: invalid input*/

        /************************************************\
        * Fun03 Sec03 Sub05:
        *   - move to next argument
        \************************************************/

       ++siArg;
    } /*loop through all user input arguments*/

    /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
    ^ Fun03 Sec04:
    ^   - clean up
    <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

    errSC = 0;
    goto cleanUp_fun03_sec04;

    phelp_fun03_sec04:;
       errSC = 1;
       goto cleanUp_fun03_sec04;

    pversion_fun03_sec04:;
       errSC = 1;
       goto cleanUp_fun03_sec04;

    err_fun03_sec04:;
       errSC = 2;
       goto cleanUp_fun03_sec04;

    cleanUp_fun03_sec04:;
       return errSC;
} /*input_mainTrimSam*/

/*-------------------------------------------------------\
| Main:
|   - driver function for trimSam
| Input:
|   - lenArgsSI:
|     o number of aguments input
|   - argAryStr:
|     o c-string array with user input
| Output:
|   - prints trimmed reads to output file
\-------------------------------------------------------*/
int
main(
   int lenArgsSI,
   char *argsPtrCStr[]
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Main TOC: main
   '   o main sec01:
   '     - variable declarations
   '   o main sec02:
   '     - get input and open files
   '   o main sec03:
   '     - trim reads
   '   o main Sec04:
   '     - clean up
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

    /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
    ^ Main Sec01:
    ^   - variable declarations
    \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

    signed char *samFileStr = 0;     /*sam file to trim*/
    signed char *outFileStr = 0;     /*output sam file*/
    signed char keepNoMapBl = def_keepNoMap_mainTrimSam;
    signed char errSC = 0;

    struct samEntry samStackST;

    FILE *samFILE = 0;
    FILE *outFILE = 0;

    /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
    ^ Main Sec02:
    ^   - get input and open files
    ^   o main sec02 sub01:
    ^     - get input
    ^   o main sec02 sub02:
    ^     - open input sam file
    ^   o main sec02 sub03:
    ^     - open ouput file (sam)
    \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

    /****************************************************\
    * Main Sec02 Sub01:
    *   - get input
    \****************************************************/

    init_samEntry(&samStackST);

    errSC =
        input_mainTrimSam(
            lenArgsSI,     /*number argumnets input*/
            argsPtrCStr,   /*input arguments*/
            &samFileStr,   /*input sam file*/
            &outFileStr,   /*output file*/
            &keepNoMapBl   /*Keep unammped reads?*/
    ); /*Get user input*/

    if(errSC)
    { /*If: had error*/
       --errSC; /*reduce help/versoin numbers to 0*/
       goto cleanUp_main_sec04;
    } /*If: had error*/

    errSC = setup_samEntry(&samStackST);

    if(errSC)
    { /*If: memory error*/
       fprintf(
          stderr,
          "memory error setting up sam entry%s",
          str_endLine
       );

       goto memErr_main_sec04;
    } /*If: memory error*/
       
    /****************************************************\
    * Main Sec02 Sub02:
    *   - open input sam file
    \****************************************************/

    if(! samFileStr || samFileStr[0] == '-')
       samFILE = stdin;

    else
    { /*Else: using a file for input*/
        samFILE =
           fopen(
              (char *) samFileStr,
              "r"
           );
    
        if(samFILE == 0)
        { /*If: could not open sam file*/
            fprintf(
                stderr,
                "could not open -sam %s%s",
                samFileStr,
                str_endLine
            ); /*If file was not valid*/

            goto fileErr_main_sec04;
        } /*If: could not open sam file*/
    } /*Else: using a file for input*/

    /****************************************************\
    * Main Sec02 Sub03:
    *   - open ouput file (sam)
    \****************************************************/

    if(! outFILE || outFileStr[0] == '-')
       outFILE = stdout;

    else
    { /*Else: user provided output file*/
        outFILE =
           fopen(
              (char *) outFileStr,
              "w"
           );

        if(outFILE == 0)
        { /*If: could no open output file*/
            fprintf(
                stderr,
                "could not open -out (%s)%s",
                outFileStr,
                str_endLine
            );
                
            goto fileErr_main_sec04;
        } /*If: could no open output file*/
    } /*Else: user provided output file*/

    /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
    ^ Main Sec03:
    ^   - print headers
    ^   o main sec03 sub01:
    ^     - get first line in sam file
    ^   o main sec03 sub02:
    ^     - print headers in sam file
    ^   o main sec03 sub03:
    ^     - check for errors
    ^   o main sec03 sub04:
    ^     - print trimSam header
    \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

    /****************************************************\
    * Main Sec03 Sub01:
    *   - get first line in sam file
    \****************************************************/

    errSC = get_samEntry(&samStackST, samFILE);

    if(errSC)
    { /*If: error*/
       if(errSC == def_memErr_samEntry)
       { /*If: memory error*/
          fprintf(
             stderr,
             "memory error first line in -sam %s%s",
             samFileStr,
             str_endLine
          );

          goto memErr_main_sec04;
       } /*If: memory error*/

       /*EOF*/
       fprintf(
          stderr,
          "nothing in -sam %s%s",
          samFileStr,
          str_endLine
       );

       goto fileErr_main_sec04;
    } /*If: error*/

    /****************************************************\
    * Main Sec03 Sub02:
    *   - print headers in sam file
    \****************************************************/

    while(! errSC)
    { /*Loop: print headers*/
       if(samStackST.extraStr[0] != '@')
          break;
       p_samEntry(&samStackST, 0, outFILE);
       errSC = get_samEntry(&samStackST, samFILE);
    } /*Loop: print headers*/

    /****************************************************\
    * Main Sec03 Sub03:
    *   - check for errors
    \****************************************************/

    if(errSC)
    { /*If: error*/
       if(errSC == def_memErr_samEntry)
       { /*If: memory error*/
          fprintf(
             stderr,
             "memory error printing -sam %s headers%s",
             samFileStr,
             str_endLine
          );

          goto memErr_main_sec04;
       } /*If: memory error*/

       /*EOF*/
       fprintf(
          stderr,
          "no reads or bad line in -sam %s%s",
          samFileStr,
          str_endLine
       );

       goto fileErr_main_sec04;
    } /*If: error*/

    /****************************************************\
    * Main Sec03 Sub04:
    *   - print trimSam header
    \****************************************************/

    fprintf( 
       (FILE *) outFILE,
       "@PG\tID:trimSam\tPN:trimSam"
    );

    fprintf(
       (FILE *) outFILE,
       "\tVN:bioTools_%i-%02i-%02i\tCL:trimSam",
       def_year_bioTools,
       def_month_bioTools,
       def_day_bioTools
    );

    if(keepNoMapBl)
       fprintf((FILE *) outFILE, " -keep-no-map");

    if(samFILE != stdout)
       fprintf((FILE *) outFILE, " -sam %s", samFileStr);
    else
       fprintf((FILE *) outFILE, " -sam -");

    if(outFILE != stdout)
       fprintf((FILE *) outFILE, " -out %s", outFileStr);
    else
       fprintf((FILE *) outFILE, " -out -");

    fprintf((FILE *) outFILE, "%s", str_endLine);
    
    /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
    ^ Main Sec03:
    ^   - trim reads
    \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

    while(! errSC)
    { /*Loop: trim reads*/
        /*Convert & print out sam file entry*/
        errSC = seq_trimSam(&samStackST);

        if(
              keepNoMapBl
           && errSC & def_noMap_trimSam
        ) errSC = 0;

        if(! errSC) 
           p_samEntry(&samStackST, 0, outFILE);
        
        errSC = get_samEntry(&samStackST, samFILE);
    } /*Loop: trim reads*/

    if(errSC == def_memErr_samEntry)
    { /*If: had an error*/
       fprintf(
          stderr,
          "memory error when trimming reads%s",
          str_endLine
       );

       goto memErr_main_sec04;
    } /*If: had an error*/

    if(errSC != def_EOF_samEntry)
    { /*If: had an error*/
       fprintf(
          stderr,
          "file error when trimming reads%s",
          str_endLine
       );

       goto fileErr_main_sec04;
    } /*If: had an error*/

    /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
    ^ Main Sec04:
    ^   - clean up
    \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

    errSC = 0;
    goto cleanUp_main_sec04;

    memErr_main_sec04:;
       errSC = def_memErr_trimSam;
       goto cleanUp_main_sec04;

    fileErr_main_sec04:;
       errSC = def_fileErr_trimSam;
       goto cleanUp_main_sec04;

    cleanUp_main_sec04:;
       freeStack_samEntry(&samStackST);

       if(
             samFILE
          && samFILE != stdin
          && samFILE != stdout
       ) fclose(samFILE);

       samFILE = 0;

       if(
             outFILE
          && outFILE != stdin
          && outFILE != stdout
       ) fclose(outFILE);

       outFILE = 0;

       return errSC;
} /*main function*/


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
