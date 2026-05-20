/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' gbAccBin SOF: Start Of File
'   - bins genbank accessions into bins of assembly id
'   o fun01: pversion_gbAccBin
'     - prints the version number
'   o fun02: phelp_gbAccBin
'     - prints the help message
'   o fun03: input_gbAccBin
'     - gets user input for gbAccBin
'   o license:
'     - licensing for this code (CC0)
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#ifdef PLAN9
   #include <u.h>
   #include <libc.h>
#else
   #include <stdlib.h>
#endif

#include <stdio.h>

#include "../genBio/gzSeqST.h"
#include "../genBio/seqST.h"
#include "../genBio/accession.h"

#include "../genLib/inflate.h"
#include "../genLib/fileFun.h"
#include "../genLib/ulCp.h"
#include "../genLib/numToStr.h"

/*only has a .h file*/
#include "../genLib/endLine.h"
#include "../bioTools.h"

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\
! Hidden Libraries:
!   -  .c #include "../genLib/checkSum.h"
!   -  .c #include "../genLib/genMath.h"
!   -  .c #include "../genLib/endin.h"
\%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

/*-------------------------------------------------------\
| Fun01: pversion_gbAccBin
|   - prints the version number
| Input:
|   - outFILE:
|     o FILE pointer to print version number to
| Output:
|   - Prints:
|     o version number to outFILE
\-------------------------------------------------------*/
void
pversion_gbAccBin(
   void *outFILE
){
   fprintf(
      (FILE *) outFILE,
      "gbAccBin from bioTools version: %i-%02i-%02i%s",
      def_year_bioTools,
      def_month_bioTools,
      def_day_bioTools,
      str_endLine
   );
} /*pversion_gbAccBin*/

/*-------------------------------------------------------\
| Fun02: phelp_gbAccBin
|   - prints the help message
| Input:
|   - outFILE:
|     o FILE pointer to print help message to
| Output:
|   - Prints:
|     o help message to outFILE
\-------------------------------------------------------*/
void
phelp_gbAccBin(
   void *outFILE
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun02 TOC:
   '   - prints the help message
   '   o fun02 sec01:
   '     - usage
   '   o fun02 sec02:
   '     - input
   '   o fun02 sec03:
   '     - output
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun02 Sec01:
   ^   - usage
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   fprintf(
      (FILE *) outFILE,
      "gbAccBin -fa <file>.fa%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "Or gbAccBin -txt <file_with_accesions>.txt%s",
      str_endLine
   );

   fprintf(
     (FILE *) outFILE,
     "  - splits genbank accessions into assembly bins%s",
     str_endLine
   );
   fprintf(
     (FILE *) outFILE,
     "  - stops at white space at end of accession%s",
     str_endLine
   );

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun02 Sec02:
   ^   - input
   ^   o fun02 sec02 sub01:
   ^     - fasta file input + input block marker
   ^   o fun02 sec02 sub02:
   ^     - text file input
   ^   o fun02 sec02 sub03:
   ^     - prefix
   ^   o fun02 sec02 sub04:
   ^     - help and version message
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun02 Sec02 Sub01:
   *   - fasta file input + input block marker
   \*****************************************************/

   fprintf((FILE *) outFILE, "Input:%s", str_endLine);

   fprintf(
      (FILE *) outFILE,
      "  -fa <file>.fa:%s",
      str_endLine
   );
   fprintf(
     (FILE *) outFILE,
     "    - fasta file to bin%s",
     str_endLine
   );
   fprintf(
     (FILE *) outFILE,
     "    - a space or tab should come after the%s",
     str_endLine
   );
   fprintf(
     (FILE *) outFILE,
     "    - accesion number (ex: `>accession extra`)%s",
     str_endLine
   );
   fprintf(
     (FILE *) outFILE,
     "    - use `-fa -` for stdin input%s",
     str_endLine
   );

   /*****************************************************\
   * Fun02 Sec02 Sub02:
   *   - text file input
   \*****************************************************/

   fprintf(
      (FILE *) outFILE,
      "  -txt <accession_numbers>.fa:%s",
      str_endLine
   );
   fprintf(
     (FILE *) outFILE,
     "    - text file with genbank accesions to bin%s",
     str_endLine
   );
   fprintf(
     (FILE *) outFILE,
     "    - a space or tab should come after the%s",
     str_endLine
   );
   fprintf(
     (FILE *) outFILE,
     "    - accesion number (ex: `accession extra`)%s",
     str_endLine
   );
   fprintf(
     (FILE *) outFILE,
     "    - use a `@` to comment lines (ex: `@igonre`)%s",
     str_endLine
   );
   fprintf(
     (FILE *) outFILE,
     "    - use `-txt -` for stdin input%s",
     str_endLine
   );

   /*****************************************************\
   * Fun02 Sec02 Sub03:
   *   - prefix
   \*****************************************************/

   fprintf(
      (FILE *) outFILE,
      "  -prefix out: [Optional]%s",
      str_endLine
   );
   fprintf(
     (FILE *) outFILE,
     "    - prefix to name all output files with%s",
     str_endLine
   );

   /*****************************************************\
   * Fun02 Sec02 Sub04:
   *   - help and version message
   \*****************************************************/

   fprintf(
      (FILE *) outFILE,
      "  -h: print this help message and exit%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "  -v: version number and exit%s",
      str_endLine
   );

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun02 Sec03:
   ^   - output
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   fprintf((FILE *) outFILE, "Output:%s", str_endLine);

   fprintf(
      (FILE *) outFILE,
      "  - prints binned sequences to prefix-accession.fa"
   );
   fprintf((FILE *) outFILE, "%s", str_endLine);

   fprintf(
      (FILE *) outFILE,
      "  - or binned accessions to prefix-accession.txt"
   );
   fprintf((FILE *) outFILE, "%s", str_endLine);
} /*phelp_gbAccBin*/

/*-------------------------------------------------------\
| Fun03: input_gbAccBin
|   - gets user input for gbAccBin
| Input:
|   - argLenSI:
|     o number of arguments the user input
|   - argAryStr:
|     o c-string array with the user input
|   - inFileStrPtr:
|     o c-string pointer to get the input file
|     o DO NOT FREE
|   - prefStrPtr:
|     o c-string pointer to get the prefix for the output
|     o DO NOT FREE
|   - txtBlPtr:
|     o signed char to get 1 if readin in a text file
|     o signed char to get 0 if readin in a fasta file
| Output:
|   - Modifies:
|     o all input (except argLenSI and argAryStr) to have
|       user input
|   - Returns:
|     - 0 for no errors
|     - 1 if printed the help message or version number
|     - 2 if had an error
\-------------------------------------------------------*/
signed char
input_gbAccBin(
   signed int argLenSI,
   char *argAryStr[],
   signed char **inFileStrPtr,
   signed char **prefixStrPtr,
   signed char *txtBlPtr
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun03 TOC:
   '   - gets user input for gbAccBin
   '   o fun03 sec01:
   '     - variable declare and check if have arguments
   '   o fun03 sec03:
   '     - print messages (if requested) and return
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec01:
   ^   - variable declare and check if have arguments
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   signed int siArg = 1;

   if(argLenSI < 1)
      goto phelp_fun03_sec03;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec02:
   ^   - get user input
   ^   o fun03 sec02 sub01:
   ^     - get file input and prefix + start loop
   ^   o fun03 sec02 sub02:
   ^     - help message checks
   ^   o fun03 sec02 sub03:
   ^     - version number checks
   ^   o fun03 sec02 sub04:
   ^     - invalid input call + move to next argument
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun03 Sec02 Sub01:
   *   - get file input and prefix + start loop
   \*****************************************************/

   while(siArg < argLenSI)
   { /*Loop: get user input*/
      if(
         ! eqlNull_ulCp(
            (signed char *) "-fa",
            (signed char *) argAryStr[siArg]
         )
      ){ /*If: fasta input*/
         ++siArg;
         *inFileStrPtr = (signed char *) argAryStr[siArg];
         *txtBlPtr = 0;
      }  /*If: fasta input*/

      else if(
         ! eqlNull_ulCp(
            (signed char *) "-txt",
            (signed char *) argAryStr[siArg]
         )
      ){ /*Else If: text input*/
         ++siArg;
         *inFileStrPtr = (signed char *) argAryStr[siArg];
         *txtBlPtr = 1;
      }  /*Else If: text input*/

      else if(
         ! eqlNull_ulCp(
            (signed char *) "-prefix",
            (signed char *) argAryStr[siArg]
         )
      ){ /*Else If: prefix for output*/
         ++siArg;
         *prefixStrPtr = (signed char *) argAryStr[siArg];
      }  /*Else If: prefix for output*/

      /**************************************************\
      * Fun03 Sec02 Sub02:
      *   - help message checks
      \**************************************************/

      else if(
         ! eqlNull_ulCp(
            (signed char *) "-h",
            (signed char *) argAryStr[siArg]
         )
      ) goto phelp_fun03_sec03;

      else if(
         ! eqlNull_ulCp(
            (signed char *) "--h",
            (signed char *) argAryStr[siArg]
         )
      ) goto phelp_fun03_sec03;

      else if(
         ! eqlNull_ulCp(
            (signed char *) "help",
            (signed char *) argAryStr[siArg]
         )
      ) goto phelp_fun03_sec03;

      else if(
         ! eqlNull_ulCp(
            (signed char *) "-help",
            (signed char *) argAryStr[siArg]
         )
      ) goto phelp_fun03_sec03;

      else if(
         ! eqlNull_ulCp(
            (signed char *) "--help",
            (signed char *) argAryStr[siArg]
         )
      ) goto phelp_fun03_sec03;

      /**************************************************\
      * Fun03 Sec02 Sub03:
      *   - version number checks
      \**************************************************/

      else if(
         ! eqlNull_ulCp(
            (signed char *) "-v",
            (signed char *) argAryStr[siArg]
         )
      ) goto pversion_fun03_sec03;

      else if(
         ! eqlNull_ulCp(
            (signed char *) "--v",
            (signed char *) argAryStr[siArg]
         )
      ) goto pversion_fun03_sec03;

      else if(
         ! eqlNull_ulCp(
            (signed char *) "version",
            (signed char *) argAryStr[siArg]
         )
      ) goto pversion_fun03_sec03;

      else if(
         ! eqlNull_ulCp(
            (signed char *) "-version",
            (signed char *) argAryStr[siArg]
         )
      ) goto pversion_fun03_sec03;

      else if(
         ! eqlNull_ulCp(
            (signed char *) "--version",
            (signed char *) argAryStr[siArg]
         )
      ) goto pversion_fun03_sec03;

      /**************************************************\
      * Fun03 Sec02 Sub04:
      *   - invalid input call + move to next argument
      \**************************************************/

      else
      { /*Else: unknown input*/
         fprintf(
            stderr,
            "%s is not recoginzed%s",
            argAryStr[siArg],
            str_endLine
         );

         goto err_fun03_sec03;
      } /*Else: unknown input*/

      ++siArg;
   } /*Loop: get user input*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec03:
   ^   - print messages (if requested) and return
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   siArg = 0;
   goto ret_fun03_sec03;

   phelp_fun03_sec03:;
      siArg = 1;
      phelp_gbAccBin(stdout);
      goto ret_fun03_sec03;

   pversion_fun03_sec03:;
      siArg = 1;
      pversion_gbAccBin(stdout);
      goto ret_fun03_sec03;

   err_fun03_sec03:;
      siArg = 2;
      goto ret_fun03_sec03;

   ret_fun03_sec03:;
      return (signed char) siArg;
} /*input_gbAccBin*/

/*-------------------------------------------------------\
| Main:
|   - driver function for gbAccBin
| Input:
|   - argLenSI:
|     o number of arguments the user input
|   - argAryStr:
|     o c-string array with user input
| Output:
|   - Prints:
|     o accessions or fasta sequences to output files
|       named <prefix>-<accession>.txt or
|       <prefix>-<accession>.fa
|   - Returns:
|     o 0 for no errors
|     o 1 for errors
\-------------------------------------------------------*/
signed int
main(
   int argLenSI,
   char *argAryStr[]
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Main TOC:
   '   - driver function for gbAccBin
   '   o main sec01:
   '     - variable declarations
   '   o main sec02:
   '     - get and check input
   '   o main sec03:
   '     - bin accesion numbers
   '   o main sec04:
   '     - clean up and return
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   signed char *inFileStr = 0;
   signed char *prefixStr = (signed char *) "hedgewidge";
   signed char txtBl = 0;

   signed long lineSL = 0;
   signed long errSL = 0;
   signed char *buffHeapStr = 0;
   signed long buffSizeSL = 0;
   signed long buffLenSL = 0;

   /*for processing the accession number*/
   signed char outStr[4096];
   signed char accStr[16];
   signed char refseqStr[4];
   signed int accVersSI = 0;
   signed int asmVersSI = 0;
   signed int contigNumSI = 0;

   /*for fastq readin*/
   struct seqST seqStackST;
   struct file_inflate inflateStackST;
   signed char seqTypeSC = 0;

   FILE *inFILE = 0;
   FILE *outFILE = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec02:
   ^   - get and check input
   ^   o main sec02 sub01:
   ^     - get input
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Main Sec02 Sub01:
   *   - get input
   \*****************************************************/

   init_seqST(&seqStackST);
   init_file_inflate(&inflateStackST);
  
   errSL =
      input_gbAccBin(
         argLenSI,
         argAryStr,
         &inFileStr,
         &prefixStr,
         &txtBl
      );

   if(errSL)
   { /*If: help message, version, or error*/
      --errSL; /*help/version is reduced to 0 (no error)*/
      goto ret_main_sec04;
   } /*If: help message, version, or error*/

   /*****************************************************\
   * Main Sec02 Sub02:
   *   - open input file
   \*****************************************************/

   if(! inFileStr || inFileStr[0] == '-')
      inFILE = stdin;
   else
   { /*Else: input file is a file*/
      inFILE = fopen((char *) inFileStr, "r");

      if(! inFILE)
      { /*If: could not open the input file*/
         if(txtBl)
            fprintf(
               stderr,
               "could not open -txt %s%s",
               inFileStr,
               str_endLine
            );
         else
            fprintf(
               stderr,
               "could not open -fa %s%s",
               inFileStr,
               str_endLine
            );
         goto err_main_sec04;
      } /*If: could not open the input file*/
   } /*Else: input file is a file*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec03:
   ^   - bin accesion numbers
   ^   o main sec03 sub01:
   ^     - get first line from text file
   ^   o main sec03 sub02:
   ^     - get first line from fasta file
   ^   o main sec03 sub03:
   ^     - get accession bin for txt files + start loop
   ^   o main sec03 sub04:
   ^     - get accession bin for fasta files
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Main Sec03 Sub01:
   *   - get first line from text file
   \*****************************************************/

   if(txtBl)
   { /*If: getting first line from a text file*/
      errSL =
         getFullLine_fileFun(
            inFILE,
            &buffHeapStr,
            &buffSizeSL,
            &buffLenSL,
            0
         );
      if(errSL <= 0)
      { /*If: had a problem*/
         if(errSL == 0)
            fprintf(
               stderr,
               "nothing in -txt %s%s",
               inFileStr,
               str_endLine
            );
         else
            fprintf(
               stderr,
               "memory error on first line in -txt %s%s",
               inFileStr,
               str_endLine
            );
         goto err_main_sec04;
      } /*If: had a problem*/

      buffHeapStr[errSL - 1] = 0;
      errSL = 0;
   } /*If: getting first line from a text file*/

   /*****************************************************\
   * Main Sec03 Sub02:
   *   - get first line from fasta file
   \*****************************************************/

   else
   { /*Else: getting first line from a fasta file*/
      errSL =
         get_gzSeqST(
            &inflateStackST,
            &seqTypeSC,
            &seqStackST,
            inFILE
         );
      if(errSL)
      { /*If: had a problem*/
         if(errSL == def_EOF_seqST)
            fprintf(
               stderr,
               "nothing in -fa %s%s",
               inFileStr,
               str_endLine
            );
         else
            fprintf(
               stderr,
               "error on first sequence  in -fa %s%s",
               inFileStr,
               str_endLine
            );
         goto err_main_sec04;
      } /*If: had a problem*/

      else
         inFILE = 0; /*clean up functions will free*/
   } /*Else: getting first line from a fasta file*/

   /*****************************************************\
   * Main Sec03 Sub03:
   *   - get accession bin for txt files + start loop
   *   o main sec03 sub03 cat01:
   *     - check if have an accession to bin
   *   o main sec03 sub03 cat02:
   *     - print text accesion to its bin
   \*****************************************************/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Main Sec03 Sub03 Cat01:
   +   - text check if have an accession to bin
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   lineSL = 0;
   while(! errSL)
   { /*Loop: bin accessions*/
      ++lineSL;

      if(txtBl)
      { /*If: text file*/
         if(buffHeapStr[0] == '@')
            goto nextTxtLine_main_sec03_sub0x;

         accVersSI = 0;
         while(
               buffHeapStr[accVersSI]
            && buffHeapStr[accVersSI] < 32
         ) ++accVersSI;

         if(! buffHeapStr[accVersSI])
            goto nextTxtLine_main_sec03_sub0x;
            /*blank line*/

         errSL =
            genbankAccExtract_accession(
               &buffHeapStr[accVersSI],
               accStr,
               refseqStr,
               &accVersSI,
               &asmVersSI,
               &contigNumSI
            );
         if(errSL <= 0)
         { /*If: non-genbank accession*/
            fprintf(
               stderr,
               "%s is not a genbank accession%s",
               buffHeapStr,
               str_endLine
            );

            goto nextTxtLine_main_sec03_sub0x;
            /*not genbank accession*/
         } /*If: non-genbank accession*/

         /*++++++++++++++++++++++++++++++++++++++++++++++\
         + Main Sec03 Sub03 Cat02:
         +   - print text accesion to its bin
         \++++++++++++++++++++++++++++++++++++++++++++++*/

         errSL = cpStr_ulCp(outStr, prefixStr);
         outStr[errSL++] = '-';
         errSL += cpStr_ulCp(&outStr[errSL], accStr);

         if(asmVersSI >= 0)
         { /*If: have an assembly version*/
            outStr[errSL++] = '_';
            errSL += numToStr(&outStr[errSL], asmVersSI);
         } /*If: have an assembly version*/

         outStr[errSL++] = '.';
         outStr[errSL++] = 't';
         outStr[errSL++] = 'x';
         outStr[errSL++] = 't';
         outStr[errSL] = 0;

         outFILE = fopen((char *) outStr, "a");
         if(! outFILE)
         { /*If: could not open the output file*/
            fprintf(
               stderr,
               "could not open output %s%s",
               outStr,
               str_endLine
            );

            goto err_main_sec04;
         } /*If: could not open the output file*/

         fprintf(
            outFILE,
            "%s%s",
            buffHeapStr,
            str_endLine
         );

         fclose(outFILE);
         outFILE = 0;
      } /*If: text file*/

      /*************************************************\
      * Main Sec03 Sub04:
      *   - get accession bin for fasta files
      *   o main sec03 sub04 cat01:
      *     - fasta check if have an accession to bin
      *   o main sec03 sub04 cat02:
      *     - print fasta accession to bin
      \*************************************************/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Main Sec03 Sub04 Cat01:
      +   - fasta check if have an accession to bin
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      else
      { /*Else: fasta file*/
         /*make sure of header, seqST should remove this*/
         if(seqStackST.idStr[accVersSI] == '>')
            accVersSI = 1;
         else
            accVersSI = 0;

         while(
               seqStackST.idStr[accVersSI]
            && seqStackST.idStr[accVersSI] < 32
         ) ++accVersSI;

         errSL =
            genbankAccExtract_accession(
               &seqStackST.idStr[accVersSI],
               accStr,
               refseqStr,
               &accVersSI,
               &asmVersSI,
               &contigNumSI
            );
         if(errSL <= 0)
         { /*If: non-genbank accession*/
            fprintf(
               stderr,
               "%s is not a genbank accession%s",
               seqStackST.idStr,
               str_endLine
            );

            goto nextSeqLine_main_sec03_sub0x;
            /*not genbank accession*/
         } /*If: non-genbank accession*/

         /*++++++++++++++++++++++++++++++++++++++++++++++\
         + Main Sec03 Sub04 Cat02:
         +   - print fasta accession to bin
         \++++++++++++++++++++++++++++++++++++++++++++++*/

         errSL = cpStr_ulCp(outStr, prefixStr);
         outStr[errSL++] = '-';
         errSL += cpStr_ulCp(&outStr[errSL], accStr);

         if(asmVersSI >= 0)
         { /*If: have an assembly version*/
            outStr[errSL++] = '_';
            errSL += numToStr(&outStr[errSL], asmVersSI);
         } /*If: have an assembly version*/

         outStr[errSL++] = '.';
         outStr[errSL++] = 'f';
         outStr[errSL++] = 'a';
         outStr[errSL] = 0;

         outFILE = fopen((char *) outStr, "a");
         if(! outFILE)
         { /*If: could not open the output file*/
            fprintf(
               stderr,
               "could not open output %s%s",
               outStr,
               str_endLine
            );

            goto err_main_sec04;
         } /*If: could not open the output file*/

         fprintf(
            outFILE,
            ">%s%s%s%s",
            seqStackST.idStr,
            str_endLine,
            seqStackST.seqStr,
            str_endLine
         );

         fclose(outFILE);
         outFILE = 0;
      } /*Else: fasta file*/

      /*************************************************\
      * Main Sec03 Sub05:
      *   - get the next line
      *   o main sec03 sub05 cat01:
      *     - text file; get next line
      *   o main sec03 sub05 cat02:
      *     - fasta file; get next line
      \*************************************************/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Main Sec03 Sub05 Cat01:
      +   - text file; get next line
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      if(txtBl)
      { /*If: getting accession from a text file*/
         nextTxtLine_main_sec03_sub0x:;
            errSL =
               getFullLine_fileFun(
                  inFILE,
                  &buffHeapStr,
                  &buffSizeSL,
                  &buffLenSL,
                  0
               );
            if(errSL < 0)
            { /*If: had a problem*/
               fprintf(
                  stderr,
                  "memory error on line %li in -txt %s%s",
                  lineSL,
                  inFileStr,
                  str_endLine
               );
               goto err_main_sec04;
            } /*If: had a problem*/

            else if(! errSL)
               errSL = 1; /*end of file*/
            else
            { /*Else: on the next line*/
               buffHeapStr[errSL - 1] = 0;
               errSL = 0;
            } /*Else: on the next line*/
      } /*If: getting accession from a text file*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Main Sec03 Sub05 Cat02:
      +   - fasta file; get next line
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      else
      { /*Else: getting next sequence*/
         nextSeqLine_main_sec03_sub0x:;
            errSL =
               get_gzSeqST(
                  &inflateStackST,
                  &seqTypeSC,
                  &seqStackST,
                  0
               );
            if(! errSL)
               ; /*read in next fasta entry*/
            else if(errSL == def_EOF_seqST)
               ; /*end of file; loop will self terminate*/
            else
            { /*Else: read in error*/
               fprintf(
                  stderr,
                  "error on sequence  %li in -fa %s%s",
                  lineSL,
                  inFileStr,
                  str_endLine
               );
               goto err_main_sec04;
            } /*Else: read in error*/
      } /*Else: getting next sequence*/
   } /*Loop: bin accessions*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec04:
   ^   - clean up and return
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   errSL = 0;
   goto ret_main_sec04;

   err_main_sec04:;
      errSL = 1;
      goto ret_main_sec04;

   ret_main_sec04:;
      if(buffHeapStr)
         free(buffHeapStr);
      buffHeapStr = 0;

      freeStack_seqST(&seqStackST);
      freeStack_file_inflate(&inflateStackST);

      if(! inFILE) ;
      else if(inFILE == stdin) ;
      else fclose(inFILE);
      inFILE = 0;

      if(! outFILE) ;
      else if(outFILE == stdout) ;
      else fclose(outFILE);
      outFILE = 0;

      return errSL;
} /*main*/

/*=======================================================\
: License:
: 
: Creative Commons Legal Code
: 
: CC0 1.0 Universal
: 
:     CREATIVE COMMONS CORPORATION IS NOT A LAW FIRM AND
:     DOES NOT PROVIDE LEGAL SERVICES. DISTRIBUTION OF
:     THIS DOCUMENT DOES NOT CREATE AN ATTORNEY-CLIENT
:     RELATIONSHIP. CREATIVE COMMONS PROVIDES THIS
:     INFORMATION ON AN "AS-IS" BASIS. CREATIVE COMMONS
:     MAKES NO WARRANTIES REGARDING THE USE OF THIS
:     DOCUMENT OR THE INFORMATION OR WORKS PROVIDED
:     HEREUNDER, AND DISCLAIMS LIABILITY FOR DAMAGES
:     RESULTING FROM THE USE OF THIS DOCUMENT OR THE
:     INFORMATION OR WORKS PROVIDED HEREUNDER.
: 
: Statement of Purpose
: 
: The laws of most jurisdictions throughout the world
: automatically confer exclusive Copyright and Related
: Rights (defined below) upon the creator and subsequent
: owner(s) (each and all, an "owner") of an original work
: of authorship and/or a database (each, a "Work").
: 
: Certain owners wish to permanently relinquish those
: rights to a Work for the purpose of contributing to a
: commons of creative, cultural and scientific works
: ("Commons") that the public can reliably and without
: fear of later claims of infringement build upon, modify,
: incorporate in other works, reuse and redistribute as
: freely as possible in any form whatsoever and for any
: purposes, including without limitation commercial
: purposes. These owners may contribute to the Commons to
: promote the ideal of a free culture and the further
: production of creative, cultural and scientific works,
: or to gain reputation or greater distribution for their
: Work in part through the use and efforts of others.
: 
: For these and/or other purposes and motivations, and
: without any expectation of additional consideration or
: compensation, the person associating CC0 with a Work
: (the "Affirmer"), to the extent that he or she is an
: owner of Copyright and Related Rights in the Work,
: voluntarily elects to apply CC0 to the Work and publicly
: distribute the Work under its terms, with knowledge of
: his or her Copyright and Related Rights in the Work and
: the meaning and intended legal effect of CC0 on those
: rights.
: 
: 1. Copyright and Related Rights. A Work made available
:    under CC0 may be protected by copyright and related
:    or neighboring rights ("Copyright and Related
:    Rights"). Copyright and Related Rights include, but
:    are not limited to, the following:
: 
:   i. the right to reproduce, adapt, distribute, perform,
:      display, communicate, and translate a Work;
:  ii. moral rights retained by the original author(s)
:      and/or performer(s);
: iii. publicity and privacy rights pertaining to a
:      person's image or likeness depicted in a Work;
:  iv. rights protecting against unfair competition in
:      regards to a Work, subject to the limitations in
:      paragraph 4(a), below;
:   v. rights protecting the extraction, dissemination,
:      use and reuse of data in a Work;
:  vi. database rights (such as those arising under
:      Directive 96/9/EC of the European Parliament and of
:      the Council of 11 March 1996 on the legal
:      protection of databases, and under any national
:      implementation thereof, including any amended or
:      successor version of such directive); and
: vii. other similar, equivalent or corresponding rights
:      throughout the world based on applicable law or
:      treaty, and any national implementations thereof.
: 
: 2. Waiver. To the greatest extent permitted by, but not
:    in contravention of, applicable law, Affirmer hereby
:    overtly, fully, permanently, irrevocably and
:    unconditionally waives, abandons, and surrenders all
:    of Affirmer's Copyright and Related Rights and
:    associated claims and causes of action, whether now
:    known or unknown (including existing as well as
:    future claims and causes of action), in the Work (i)
:    in all territories worldwide, (ii) for the maximum
:    duration provided by applicable law or treaty
:    (including future time extensions), (iii) in any
:    current or future medium and for any number of
:    copies, and (iv) for any purpose whatsoever,
:    including without limitation commercial, advertising
:    or promotional purposes (the "Waiver"). Affirmer
:    makes the Waiver for the benefit of each member of
:    the public at large and to the detriment of
:    Affirmer's heirs and successors, fully intending that
:    such Waiver shall not be subject to revocation,
:    rescission, cancellation, termination, or any other
:    legal or equitable action to disrupt the quiet
:    enjoyment of the Work by the public as contemplated
:    by Affirmer's express Statement of Purpose.
: 
: 3. Public License Fallback. Should any part of the
:    Waiver for any reason be judged legally invalid or
:    ineffective under applicable law, then the Waiver
:    shall be preserved to the maximum extent permitted
:    taking into account Affirmer's express Statement of
:    Purpose. In addition, to the extent the Waiver is so
:    judged Affirmer hereby grants to each affected person
:    a royalty-free, non transferable, non sublicensable,
:    non exclusive, irrevocable and unconditional license
:    to exercise Affirmer's Copyright and Related Rights
:    in the Work (i) in all territories worldwide, (ii)
:    for the maximum duration provided by applicable law
:    or treaty (including future time extensions), (iii)
:    in any current or future medium and for any number of
:    copies, and (iv) for any purpose whatsoever,
:    including without limitation commercial, advertising
:    or promotional purposes (the "License"). The License
:    shall be deemed effective as of the date CC0 was
:    applied by Affirmer to the Work. Should any part of
:    the License for any reason be judged legally invalid
:    or ineffective under applicable law, such partial
:    invalidity or ineffectiveness shall not invalidate
:    the remainder of the License, and in such case
:    Affirmer hereby affirms that he or she will not (i)
:    exercise any of his or her remaining Copyright and
:    Related Rights in the Work or (ii) assert any
:    associated claims and causes of action with respect
:    to the Work, in either case contrary to Affirmer's
:    express Statement of Purpose.
: 
: 4. Limitations and Disclaimers.
: 
:  a. No trademark or patent rights held by Affirmer are
:     waived, abandoned, surrendered, licensed or
:     otherwise affected by this document.
:  b. Affirmer offers the Work as-is and makes no
:     representations or warranties of any kind concerning
:     the Work, express, implied, statutory or otherwise,
:     including without limitation warranties of title,
:     merchantability, fitness for a particular purpose,
:     non infringement, or the absence of latent or other
:     defects, accuracy, or the present or absence of
:     errors, whether or not discoverable, all to the
:     greatest extent permissible under applicable law.
:  c. Affirmer disclaims responsibility for clearing
:     rights of other persons that may apply to the Work
:     or any use thereof, including without limitation any
:     person's Copyright and Related Rights in the Work.
:     Further, Affirmer disclaims responsibility for
:     obtaining any necessary consents, permissions or
:     other rights required for any use of the Work.
:  d. Affirmer understands and acknowledges that Creative
:     Commons is not a party to this document and has no
:     duty or obligation with respect to this CC0 or use
:     of the Work.
\=======================================================*/
