/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' mainAdjCoords SOF: Start Of File
'   - adjust coordinates in an sam file (main function)
'   o header:
'     - included libraries and version number
'   o fun01: pversion_mainAdjCoords
'     - prints the version number for adjCoords
'   o fun02: phelp_mainAdjCoords
'     - prints the help message for adjCoords
'   o fun03: input_mainAdjCoords
'     - gets user input in argAryStr
'   o main:
'     - driver function to adjust coordinates with
'   o license:
'     - licensing for this code (public domain / mit)
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*-------------------------------------------------------\
| Header:
|   - included libraries and version number
|   o header sec01:
|     - included libraries
|   o header sec02:
|     - defined variables (defaults)
\-------------------------------------------------------*/

/*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
^ Header Sec01:
^   - included libraries
\<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

#ifdef PLAN9
   #include <u.h>
   #include <libc.h>
#else
   #include <stdlib.h>
#endif

#include <stdio.h>

#include "../genLib/charCp.h"

#include "../genBio/geneCoord.h"
#include "../genBio/samEntry.h"
#include "../genBio/adjCoords.h"

/*These have no .c files*/
#include "../bioTools.h" /*version*/
#include "../genLib/endLine.h"

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\
! Hidden includes:
!   - .c  #include "../genLib/numToStr.h"
!   - .c  #include "../genLib/base10StrToNum.h"
!   - .c  #include "../genLib/ulCp.h"
!   - .c  #include "../genLib/strAry.h"
!   - .c  #include "../genLib/fileFun.h"
!   - .h  #include "../genLib/genMath.h" .h macro only
!   - .h  #include "../genBio/ntTo5Bit.h"
\%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

/*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
^ Header Sec02:
^   - defined variables (defaults)
\<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

/*-------------------------------------------------------\
| Fun01: pversion_mainAdjCoords
|   - prints the version number for adjCoords
| Input:
|   - outFILE:
|     o file to print the version number to
| Output:
|   - prints the version number to outFILE
\-------------------------------------------------------*/
void
pversion_mainAdjCoords(
   FILE *outFILE
){
   fprintf(
      outFILE,
      "adjCoords from bioTools version: %i-%02i-%02i%s",
      def_year_bioTools,
      def_month_bioTools,
      def_day_bioTools,
      str_endLine
    );
} /*pversion_mainAdjCoords*/

/*-------------------------------------------------------\
| Fun02: phelp_mainAdjCoords
|   - prints the help message for adjCoords
| Input:
|   - outFILE:
|     o file to print the help message to
| Output:
|   - prints the help message to outFILE
\-------------------------------------------------------*/
void
phelp_mainAdjCoords(
   FILE *outFILE
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun02 TOC:
   '   - prints the help message for adjCoords
   '   o fun02 sec01:
   '     - print the usage entry
   '   o fun02 sec02:
   '     - print the input entry
   '   o fun02 sec03:
   '     - print output block
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun02 Sec01:
   ^   - print the usage entry
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   fprintf(
      outFILE,
      "adjCoords -coords gene-coords.tsv -sam file.sam%s",
      str_endLine
    );

   fprintf(
      outFILE,
      "  - adjust coordinates in sam file to reference%s",
      str_endLine
    );

   fprintf(
      outFILE,
      "  - entries with genes not in gene-coords.tsv are"
    );

   fprintf(
      outFILE,
      " removed%s",
      str_endLine
    );

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun02 Sec02:
   ^   - print the input entry
   ^   o fun02 sec02 sub01:
   ^     - print input block header
   ^   o fun02 sec02 sub02:
   ^     - genome coordinates block
   ^   o fun02 sec02 sub03:
   ^     - sam file block
   ^   o fun02 sec02 sub04:
   ^     - help and version block
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun02 Sec02 Sub01:
   *   - print input block header
   \*****************************************************/

   fprintf(
      outFILE,
      "Input:%s",
      str_endLine
    );

   /*****************************************************\
   * Fun02 Sec02 Sub02:
   *   - genome coordinates block
   \*****************************************************/

   fprintf(
      outFILE,
      "  -coords gene-coordinates.tsv: [Required]%s",
      str_endLine
    );

   fprintf(
      outFILE,
      "    o tsv file with coordiantes of each gene%s",
      str_endLine
    );

   fprintf(
      (FILE *) outFILE,
      "     - example:%s",
      str_endLine
   );


   fprintf(
      (FILE *) outFILE,
      "       gene\tref_id\tdirection\tstart\tend%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "       dnaA\tNC000962.3\tF\t1\t1525%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "       pknB\tNC000962.3\tR\t15590\t17470%s",
      str_endLine
   );

   /*****************************************************\
   * Fun02 Sec02 Sub03:
   *   - sam file block
   \*****************************************************/

   fprintf(
      outFILE,
      "  -sam file.sam: [stdin]%s",
      str_endLine
    );

   fprintf(
      outFILE,
      "    o sam file to adjust coordinates on%s",
      str_endLine
    );

   fprintf(
      outFILE,
      "    o use `-sam -` for stdin input%s",
      str_endLine
    );

   /*****************************************************\
   * Fun02 Sec02 Sub04:
   *   - help and version block
   \*****************************************************/

   fprintf(
      outFILE,
      "  -h: print this help message%s",
      str_endLine
    );

   fprintf(
      outFILE,
      "  -v: print the versio number%s",
      str_endLine
    );

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun02 Sec03:
   ^   - print output block
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   fprintf(
      outFILE,
      "Output:%s",
      str_endLine
    );

   fprintf(
      outFILE,
      "  - prints adjusted sam file to stdout%s",
      str_endLine
    );
} /*phelp_mainAdjCoords*/

/*-------------------------------------------------------\
| Fun03: input_mainAdjCoord
|   - gets user input in argAryStr
| Input:
|   - argAryStr:
|     o array of c-strings with user input
|   - numArgsSI:
|     o number of c-strings in argAryStr
|   - tblFileStr:
|     o pointer to c-string to point to the gene
|       coordinates file
|   - samFileStr:
|     o pointer to c-string to point to the sam file
| Output:
|   - Modifies:
|     o tblFileStr to point to the coordinates file in
|       argAryStr
|     o samFileStr to point to the sam file in argAryStr
|     o refStr to point to the reference name in argAryStr
|   - Prints;
|     o help message or version to stdou if requested
|     o error messages to stderr
|   - Returns;
|     o 0 for no errors
|     o 1 for help message or version request
|     o 2 for errors
\-------------------------------------------------------*/
signed char
input_mainAdjCoord(
   char *argAryStr[],
   int numArgsSI,
   signed char **tblFileStr,
   signed char **samFileStr
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun03 TOC:
   '   o fun03 sec01:
   '     - variable declerations
   '   o fun03 sec02:
   '     - check if I have input and start loop
   '   o fun03 sec03:
   '     - get input
   '   o fun03 sec04:
   '     - return errors
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec01:
   ^   - variable declerations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   signed int siArg = 1;
   signed char errSC = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec02:
   ^   - check if I have input
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(siArg >= numArgsSI)
   { /*If: no arguments were input*/
      phelp_mainAdjCoords(stdout);
      goto phelp_fun03_sec04;
   } /*If: no arguments were input*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec03:
   ^   - get input
   ^   o fun03 sec03 sub01:
   ^     - start loop & check if input is coordinate file
   ^   o fun03 sec03 sub02:
   ^     - is sam file input
   ^   o fun03 sec03 sub03:
   ^     - is an help messge request
   ^   o fun03 sec03 sub04:
   ^     - is an version number request
   ^   o fun03 sec03 sub05:
   ^     - unkown input
   ^   o fun03 sec03 sub06:
   ^     - move to next input
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun03 Sec03 Sub01:
   *   - start loop and check if input is coordinate file
   \*****************************************************/

   while(siArg < numArgsSI)
   { /*Loop: process the user input*/
      if(!
          eql_charCp(
             (signed char *) "-coords",
             (signed char *) argAryStr[siArg],
             (signed char) '\0'
          )
      ){ /*If: the user provided coordinates*/
         ++siArg;
         *tblFileStr = (signed char *) argAryStr[siArg];
      } /*If: the user provided coordinates*/

      /**************************************************\
      * Fun03 Sec03 Sub02:
      *   - is sam file input
      \**************************************************/

      else if(!
          eql_charCp(
             (signed char *) "-sam",
             (signed char *) argAryStr[siArg],
             (signed char) '\0'
          )
      ){ /*Else If: the user provided an sam file*/
         ++siArg;
         *samFileStr = (signed char *) argAryStr[siArg];
      } /*Else If: the user provided an sam file*/

      /**************************************************\
      * Fun03 Sec03 Sub03:
      *   - is an help messge request
      \**************************************************/
      
      else if(!
          eql_charCp(
             (signed char *) "-h",
             (signed char *) argAryStr[siArg],
             (signed char) '\0'
          )
      ){ /*Else If: requested help message*/
         phelp_mainAdjCoords(stdout);
         goto phelp_fun03_sec04;
      } /*Else If: requested help message*/

      else if(!
          eql_charCp(
             (signed char *) "--h",
             (signed char *) argAryStr[siArg],
             (signed char) '\0'
          )
      ){ /*Else If: requested help message*/
         phelp_mainAdjCoords(stdout);
         goto phelp_fun03_sec04;
      } /*Else If: requested help message*/

      else if(!
          eql_charCp(
             (signed char *) "help",
             (signed char *) argAryStr[siArg],
             (signed char) '\0'
          )
      ){ /*Else If: requested help message*/
         phelp_mainAdjCoords(stdout);
         goto phelp_fun03_sec04;
      } /*Else If: requested help message*/

      else if(!
          eql_charCp(
             (signed char *) "-help",
             (signed char *) argAryStr[siArg],
             (signed char) '\0'
          )
      ){ /*Else If: requested help message*/
         phelp_mainAdjCoords(stdout);
         goto phelp_fun03_sec04;
      } /*Else If: requested help message*/

      else if(!
          eql_charCp(
             (signed char *) "--help",
             (signed char *) argAryStr[siArg],
             (signed char) '\0'
          )
      ){ /*Else If: requested help message*/
         phelp_mainAdjCoords(stdout);
         goto phelp_fun03_sec04;
      } /*Else If: requested help message*/

      /**************************************************\
      * Fun03 Sec03 Sub04:
      *   - is an version number request
      \**************************************************/

      else if(!
          eql_charCp(
             (signed char *) "-v",
             (signed char *) argAryStr[siArg],
             (signed char) '\0'
          )
      ){ /*Else If: requested version number*/
         pversion_mainAdjCoords(stdout);
         goto pversion_fun03_sec04;
      } /*Else If: requested version number*/

      else if(!
          eql_charCp(
             (signed char *) "--v",
             (signed char *) argAryStr[siArg],
             (signed char) '\0'
          )
      ){ /*Else If: requested version number*/
         pversion_mainAdjCoords(stdout);
         goto pversion_fun03_sec04;
      } /*Else If: requested version number*/

      else if(!
          eql_charCp(
             (signed char *) "version",
             (signed char *) argAryStr[siArg],
             (signed char) '\0'
          )
      ){ /*Else If: requested version number*/
         pversion_mainAdjCoords(stdout);
         goto pversion_fun03_sec04;
      } /*Else If: requested version number*/

      else if(!
          eql_charCp(
             (signed char *) "-version",
             (signed char *) argAryStr[siArg],
             (signed char) '\0'
          )
      ){ /*Else If: requested version number*/
         pversion_mainAdjCoords(stdout);
         goto pversion_fun03_sec04;
      } /*Else If: requested version number*/

      else if(!
          eql_charCp(
             (signed char *) "--version",
             (signed char *) argAryStr[siArg],
             (signed char) '\0'
          )
      ){ /*Else If: requested version number*/
         pversion_mainAdjCoords(stdout);
         goto pversion_fun03_sec04;
      } /*Else If: requested version number*/

      /**************************************************\
      * Fun03 Sec03 Sub05:
      *   - unkown input
      \**************************************************/

      else
      { /*Else: no idea what is*/
         fprintf(
            stderr,
            "%s is not recongnized%s",
            argAryStr[siArg],
            str_endLine
         ); /*No errors reported yet*/

         goto err_fun03_sec04;
      } /*Else: no idea what is*/

      /**************************************************\
      * Fun03 Sec03 Sub06:
      *   - move to next input
      \**************************************************/

      ++siArg;
   } /*Loop: process the user input*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec04:
   ^   - return errors
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   errSC = 0;
   goto ret_fun03_sec04;

   phelp_fun03_sec04:;
   pversion_fun03_sec04:;
   errSC = 1;
   goto ret_fun03_sec04;

   err_fun03_sec04:;
   errSC = 2;
   goto ret_fun03_sec04;

   ret_fun03_sec04:;
   return errSC;
} /*input_mainAdjCoord*/

/*-------------------------------------------------------\
| Main:
|   - driver function to adjust coordinates with
| Input:
|   - numArgsSI:
|     o number of arguments the user input
|   - argAryStr:
|     o pointer to array of c-strings with user input
| Output:
|   - Prints:
|     o ajdust sam file to stdout
\-------------------------------------------------------*/
int
main(
   int numArgsSI,
   char *argAryStr[]
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Main TOC:
   '   o main sec01:
   '     - variable declerations
   '   o main sec02:
   '     - initialize variables and get input
   '   o main sec03:
   '     - open sam file and read in genome coodinates
   '   o main sec04:
   '     - adjust genome coordinates
   '   o main sec05:
   '     - clean up
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec01:
   ^   - variable declerations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   signed char *tblFileStr = 0;
   signed char *samFileStr = 0;

   unsigned long errUL = 0;

   struct geneCoord *coordsHeapST = 0;
   signed int numGenesSI = 0;

   struct samEntry samStackST;
   signed char onReadsBl = 0; /*For printing the header*/

   FILE *samFILE = 0;
   FILE *outFILE = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec02:
   ^   - initialize variables and get input
   ^   o main sec02 sub01:
   ^     - initialize variables
   ^   o main sec02 sub02:
   ^     - get input
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Main Sec02 Sub01:
   *   - initialize variables
   \*****************************************************/

   init_samEntry(&samStackST);

   /*****************************************************\
   * Main Sec02 Sub02:
   *   - get input
   \*****************************************************/

   errUL =
      (unsigned long)
      input_mainAdjCoord(
         argAryStr,
         numArgsSI,
         &tblFileStr,
         &samFileStr
      );

   if(errUL)
   { /*If: I had an error*/
      --errUL; /*remove help message and vesion requests*/
      goto cleanUp_main_sec05;
   } /*If: I had an error*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec03:
   ^   - open sam file and read in genome coodinates
   ^   o main sec03 sub01:
   ^     - setup samEntru struct and open output file
   ^   o main sec03 sub02:
   ^     - open the sam file
   ^   o main sec03 sub03:
   ^     - read in the gene coordinates
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Main Sec03 Sub01:
   *   - setup samEntru struct and open output file
   \*****************************************************/

   if(setup_samEntry(&samStackST))
   { /*If: had memory err0r*/
      fprintf(
         stderr,
         "memory error in setup%s",
         str_endLine
      );

      goto cleanUp_main_sec05;
   } /*If: had memory err0r*/

   outFILE = stdout;

   /*****************************************************\
   * Main Sec03 Sub02:
   *   - open the sam file
   \*****************************************************/

   if(! samFileStr || *samFileStr == '-')
      samFILE = stdin;
   else
   { /*Else: I need to open the sam file*/
      samFILE = fopen((char *) samFileStr, "r");

      if(! samFILE)
      { /*If: I could not open the sam file*/
         fprintf(
            stderr,
            "Could not open -sam %s%s",
            samFileStr,
            str_endLine
         );

         goto cleanUp_main_sec05;
      } /*If: I could not open the sam file*/
   } /*Else: I need to open the sam file*/

   /*****************************************************\
   * Main Sec03 Sub03:
   *   - read in the gene coordinates
   \*****************************************************/

   coordsHeapST =
     getCoords_geneCoord(tblFileStr, &numGenesSI, &errUL);

   if(errUL)
   { /*If: I had an error*/
      if(errUL & def_fileErr_geneCoord)
         fprintf(
            stderr,
            "could not open -coords %s%s",
            tblFileStr,
            str_endLine
         );

      if(errUL & def_memErr_geneCoord)
         fprintf(
            stderr,
            "mem error reading -coords %s%s",
            tblFileStr,
            str_endLine
         );

      else
         fprintf(
            stderr,
            "line %lu in -coords %s is not valid%s",
            errUL >> 8,
            tblFileStr,
            str_endLine
          );

      errUL = (unsigned long) -1;
      goto cleanUp_main_sec05;
   } /*If: I had an error*/

   /*fprintf(
      outFILE,
      "@SQ\tSN:%s\tLN:%u%s",
      refStr,
      coordsHeapST->endAryUI[numGenesSI - 1],
      str_endLine
    );*/ /*print out the sequence and length header*/

   /*sort genes by name instead of start*/
   sortName_geneCoord(coordsHeapST, 0, numGenesSI);

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec04:
   ^   - adjust genome coordinates
   ^   o main sec04 sub01:
   ^     - read first sam file line and start loop
   ^   o main sec04 sub02:
   ^     - handle comment entries
   ^   o main sec04 sub03:
   ^     - print out the program header
   ^   o main sec04 sub04:
   ^     - adjust coordinates
   ^   o main sec04 sub05:
   ^     - print out the entry
   ^   o main sec04 sub06:
   ^     - move to next entry
   ^   o main sec04 sub07:
   ^     - check for errors
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Main Sec04 Sub01:
   *   - read first sam file line and start loop
   \*****************************************************/

   errUL = get_samEntry(&samStackST, samFILE);

   while(! errUL)
   { /*Loop: convert coordinates in the sam file*/

     /***************************************************\
     * Main Sec04 Sub02:
     *   - handle comment entries
     \***************************************************/

     if(
           samStackST.extraLenUI > 0
        && samStackST.extraStr[0] == '@'
     ){ /*If: have an comment*/
        if(
           ! eql_charCp(
              (signed char *) "@SQ\t",
              samStackST.extraStr,
              (signed char) '\t'
         )) goto nextEntry_main_sec04_sub06;
           /*this is an sequence header that I need to
           `  discard
           */

        goto printEntry_main_sec04_sub05;
     } /*If: have an comment*/

     /***************************************************\
     * Main Sec04 Sub03:
     *   - print out the program header
     \***************************************************/

     if(! onReadsBl)
     { /*If: I need to print the programs header*/
        fprintf(
           outFILE,
           "@PG\tID:adjCoords\tPN:adjCoords\tVN:%i-%02i",
           def_year_bioTools,
           def_month_bioTools
        );

        fprintf(
           outFILE,
           "-%02i\tCL:adjCoords -coords %s -sam",
           def_day_bioTools,
           tblFileStr
        );

        if(samFileStr)
        { /*If: An sam file was input*/
           fprintf(
              outFILE,
              " %s%s",
              samFileStr,
              str_endLine
           );
        } /*If: An sam file was input*/

        else
           fprintf(outFILE, " -%s", str_endLine);
           /*print stdin input*/

        onReadsBl = 1;
     } /*If: I need to print the programs header*/

     /***************************************************\
     * Main Sec04 Sub04:
     *   - adjust coordinates
     \***************************************************/

     errUL =
        (unsigned long)
        adjCoords(
           &samStackST,  /*sam entry to adjust*/
           coordsHeapST, /*has genes names & coordinates*/
           numGenesSI    /*number genes in coordsHeapST*/
        ); /*ajust the coordinates to the new reference*/

     /*check if I had coordinates for the gene*/
     if(errUL)
        goto nextEntry_main_sec04_sub06;

     /***************************************************\
     * Main Sec04 Sub05:
     *   - print the entry
     \***************************************************/

     printEntry_main_sec04_sub05:;
        p_samEntry(&samStackST, 0, outFILE);
           /*0 is to not print a line break, so I can add
           `  extra entries at end
           */
           
     /***************************************************\
     * Main Sec04 Sub06:
     *   - move to next entry
     \***************************************************/

     nextEntry_main_sec04_sub06:;
        errUL = get_samEntry(&samStackST, samFILE);
   } /*Loop: convert coordinates in the sam file*/

   /*****************************************************\
   * Main Sec04 Sub07:
   *   - check for errors
   \*****************************************************/

   --errUL; /*convert from EOF to 0*/

   if(errUL)
   { /*If: I had an error*/
      if(samFileStr)
         fprintf(
            stderr,
            "memory error reading -sam %s%s",
            samFileStr,
            str_endLine
          );
      else
         fprintf(
            stderr,
            "memory error reading -sam -%s",
            str_endLine
          );

      goto cleanUp_main_sec05;
   } /*If: I had an error*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec05:
   ^   - clean up
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   cleanUp_main_sec05:;
      if( ! outFILE) ;
      else if(outFILE == stdin) ;
      else if(outFILE == stdout) ;
      else if(outFILE == stderr) ;
      else
         fclose(outFILE);
      outFILE = 0;

      if( ! samFILE) ;
      else if(samFILE == stdin) ;
      else if(samFILE == stdout) ;
      else if(samFILE == stderr) ;
      else
         fclose(samFILE);
      samFILE = 0;

      freeHeap_geneCoord(coordsHeapST);
      coordsHeapST = 0;

      freeStack_samEntry(&samStackST);

      return (int) errUL;
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
