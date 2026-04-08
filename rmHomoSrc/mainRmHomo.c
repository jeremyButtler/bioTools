/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' mainRmHomo SOF: Start Of File
'   - driver function for rmHomo, which removes
'     homopolymer indels
'   o header:
'     - included libraries and defined variables
'   o fun01: pversion_mainRmHomo
'     - prints version number for rmHomo
'   o fun02: phelp_mainRmHomo
'     - prints help message for rmHomo
'   o fun03: input_mainRmHomo
'     - gets user input from argAryStr
'   o license:
'     - licensing for this code (CC0)
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*-------------------------------------------------------\
| Header:
|   - included libraries and defined variables
\-------------------------------------------------------*/

#ifdef PLAN9
   #include <u.h>
   #include <libc.h>
#else
   #include <stdlib.h>
#endif

#include <stdio.h>

#include "../genLib/base10str.h"
#include "../genLib/ulCp.h"

#include "../genBio/seqST.h"
#include "../genBio/samEntry.h"
#include "../genBio/rmHomo.h"

/*.h files only (no .c)*/
#include "../bioTools.h"
#include "../genLib/endLine.h"

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\
! Hidden Libraries:
!   - .c  #include "../genLib/numToStr.h"
!   - .c  #include "../genLib/fileFun.h"
!   - .h  #include "../genLib/ntTo5Bit.h"
\%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

#define def_minHomo_mainRmHomo 4
   /*start removing indels if in homopolymers larger then
   `  4 bases (were R10.4 flow cells are good at indels)
   */
#define def_maxIndel_mainRmHomo 4
   /*stop removing indels when indels are longer than
   `  5 bases
   */

#define def_mask_mainRmHomo 0
  /*0 is no mask, any other character is mask*/

#define def_scan_mainRmHomo 1
  /*scan both neighbor bases to see if they are
  `  part of a homopolymer
  */

/*-------------------------------------------------------\
| Fun01: pversion_mainRmHomo
|   - prints version number for rmHomo
| Input:
|   - outFILE:
|     o FILE pionter to print version to
| Ouput:
|   - Prints:
|     o version number to outFILE
\-------------------------------------------------------*/
void
pversion_mainRmHomo(
   FILE *outFILE /*prints version number to*/
){
   fprintf(
      (FILE *) outFILE,
      "rmHomo from bioTools version: %i-%02i-%02i%s",
      def_year_bioTools,
      def_month_bioTools,
      def_day_bioTools,
      str_endLine
   );
} /*pversion_mainRmHomo*/

/*-------------------------------------------------------\
| Fun02: phelp_mainRmHomo
|   - prints help message for rmHomo
| Input:
|   - outFILE:
|     o FILE pionter to print help message to
| Ouput:
|   - Prints:
|     o help message to outFILE
\-------------------------------------------------------*/
void
phelp_mainRmHomo(
   FILE *outFILE /*prints help message to*/
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun02 TOC:
   '   - prints help message for rmHomo
   '   o fun02 sec01:
   '     - usage entry
   '   o fun02 sec02:
   '     - input entry
   '   o fun02 sec03:
   '     - output entry
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun02 Sec01:
   ^   - usage entry for rmHomo
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   fprintf(
      (FILE *) outFILE,
      "rmHomo -ref reference.fasta -sam reads.sam%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "  - removes indels in homopolymers%s",
      str_endLine
   );

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun02 Sec02:
   ^   - input entry for rmHomo
   ^   o fun02 sec02 sub01:
   ^     - input header
   ^   o fun02 sec02 sub02:
   ^     - file IO
   ^   o fun02 sec02 sub03:
   ^     - indel removal settings
   ^   o fun02 sec02 sub04:
   ^     - help and version arguments
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun02 Sec02 Sub01:
   *   - input header
   \*****************************************************/

   fprintf(
      (FILE *) outFILE,
      "Input:%s",
      str_endLine
   );

   /*****************************************************\
   * Fun02 Sec02 Sub02:
   *   - file IO
   *   o fun02 sec02 sub02 cat01:
   *     - sam file (reads)
   *   o fun02 sec02 sub02 cat02:
   *     - reference file
   *   o fun02 sec02 sub02 cat03:
   *     - output file
   \*****************************************************/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun02 Sec02 Sub02 Cat01:
   +   - sam file (reads)
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "  -sam reads.sam: [Required]%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "    o mapped reads to remove indes from%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "    o use \"-sam -\" for stdin input%s",
      str_endLine
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun02 Sec02 Sub02 Cat02:
   +   - reference file
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "  -ref reference.fasta: [Required]%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "    o reference sequence reads were mapped to%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "    o use \"-ref -\" for stdin input%s",
      str_endLine
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun02 Sec02 Sub02 Cat03:
   +   - output file
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "  -out file.sam: [Optional; stdout]%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "    o file to save corrected reads to%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "    o use \"-out -\" for stdout input%s",
      str_endLine
   );

   /*****************************************************\
   * Fun02 Sec02 Sub03:
   *   - indel removal settings
   *   o fun02 sec02 sub03 cat01:
   *     - homoplymer length (minimum)
   *   o fun02 sec02 sub03 cat02:
   *     - indel length (maximum)
   *   o fun02 sec02 sub03 cat03:
   *     - mask deletions with base
   \*****************************************************/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun02 Sec02 Sub03 Cat01:
   +   - homopolymer length (minimum)
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "  -homo %i: [Optional; %i]%s",
      def_minHomo_mainRmHomo,
      def_minHomo_mainRmHomo,
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "    o minimum homopolymer length needed to%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "      start removing indels%s",
      str_endLine
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun02 Sec02 Sub03 Cat02:
   +   - indel length (maximum)
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "  -indel %i: [Optional; %i]%s",
      def_maxIndel_mainRmHomo,
      def_maxIndel_mainRmHomo,
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "    o largest indel to remove%s",
      str_endLine
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun02 Sec02 Sub03 Cat03:
   +   - mask deletions with base
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   if(def_mask_mainRmHomo)
      fprintf(
         (FILE *) outFILE,
         "  -mask %c: [enabled using; %c]%s",
         def_mask_mainRmHomo,
         def_mask_mainRmHomo,
         str_endLine
      );

   else
      fprintf(
         (FILE *) outFILE,
         "  -mask N: [disabled; use reference base]%s",
         str_endLine
      );

   fprintf(
      (FILE *) outFILE,
      "    o base to replace deletions with%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "    o if off, then uses reference base%s",
      str_endLine
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun02 Sec02 Sub03 Cat04:
   +   - scan neighbor homopolymers
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   if(def_scan_mainRmHomo)
      fprintf(
         (FILE *) outFILE,
         "  -scan: [Yes]%s",
         str_endLine
      );

   else
      fprintf(
         (FILE *) outFILE,
         "  -scan: [No]%s",
         str_endLine
      );

   fprintf(
      (FILE *) outFILE,
      "    o scan both neighbor bases to see if they%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "      are part of a homopolymer%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "    o disable with `-no-scan`%s",
      str_endLine
   );


   /*****************************************************\
   * Fun02 Sec02 Sub04:
   *   - help and version arguments
   \*****************************************************/

   fprintf(
      (FILE *) outFILE,
      "  -h: print this help message and exit%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "  -v: print version number and exit%s",
      str_endLine
   );

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun02 Sec03:
   ^   - output entry for rmHomo
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   fprintf(
      (FILE *) outFILE,
      "Output:%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "  - prints corrected reads to -out%s",
      str_endLine
   );
} /*phelp_mainRmHomo*/

/*-------------------------------------------------------\
| Fun03: input_mainRmHomo
|   - gets user input from argAryStr
| Input:
|   - numArgsSI:
|     o number arguments user input
|   - argAryStr:
|     o c-string array with user input
|   - samFileStrPtr:
|     o c-string pionter to be set to sam file path
|   - refFileStrPtr:
|     o c-string pionter to be set to reference file path
|   - outFileStrPtr:
|     o c-string pionter to be set to output file path
|   - homoLenSIPtr:
|     o signed int pointer to be set to minimum
|       homopolymer length
|   - indelLenSIPtr:
|     o signed int pointer to be set to maximum indel
|       length
|   - maskSCPtr:
|     o signed char pointer to get base to mask deletions
|       with
|   - scanBlPtr:
|     o signed char pointer to get if scanning neighbor
|       bases for homopolymers
| Ouput:
|   - Modifies:
|     o all input variables, except numArgsSI and
|       argAryStr, to have user input
|   - Prints:
|     o if requested, help message or version to stdout
|     o errors to stderr (if had error)
|   - Returns:
|     o 0 for no errors
|     o 1 if printed help message or version number
|     o 2 if had error
| Note:
|   - do not free any variables
\-------------------------------------------------------*/
signed char
input_mainRmHomo(
   int numArgsSI,     /*number arguments in argAryStr*/
   char *argAryStr[], /*user input*/
   signed char **samFileStrPtr, /*gets sam file path*/
   signed char **refFileStrPtr, /*gets reference path*/
   signed char **outFileStrPtr, /*gets output file path*/
   signed int  *homoLenSIPtr,   /*min homopolymer len*/
   signed int  *indelLenSIPtr,  /*gets max indel lenght*/
   signed char *maskSCPtr,      /*gets base to mask with*/
   signed char *scanBlPtr       /*scan around indels*/
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun03 TOC:
   '   - gets user input from argAryStr
   '   o fun03 sec01:
   '     - variable declarations and check if have input
   '   o fun03 sec02:
   '     - get user input
   '   o fun03 sec03:
   '     - return result (and print help/version)
   '   o fun03 sec04:
   '     - remove indels
   '   o fun03 sec05:
   '     - clean up and return
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec01:
   ^   - variable declarations and check if have input
   \>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/

   signed int siArg = 1;
   signed char errSC = 0;
   signed char *tmpStr = 0;

   if(numArgsSI <= 1)
      goto phelp_fun03_sec03;
      /*no user input*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec02:
   ^   - get user input
   ^   o fun03 sec02 sub01:
   ^     - file io + start get input loop
   ^   o fun03 sec02 sub02:
   ^     - indel removal settings
   ^   o fun03 sec02 sub03:
   ^     - help message
   ^   o fun03 sec02 sub04:
   ^     - version number
   ^   o fun03 sec02 sub05:
   ^     - unknown input
   ^   o fun03 sec02 sub06:
   ^     - move to next argument
   \>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/

   /*****************************************************\
   * Fun03 Sec02 Sub01:
   *   - file io + start get input loop
   \*****************************************************/

   while(siArg < numArgsSI)
   { /*Loop: get user input*/

      if(
         ! eqlNull_ulCp(
            (signed char *) "-sam",
            (signed char *) argAryStr[siArg]
         )
      ){ /*If: sam file input*/
         ++siArg;
         *samFileStrPtr =
            (signed char *) argAryStr[siArg];
      }  /*If: sam file input*/

      else if(
         ! eqlNull_ulCp(
            (signed char *) "-ref",
            (signed char *) argAryStr[siArg]
         )
      ){ /*Else If: reference file input*/
         ++siArg;
         *refFileStrPtr =
            (signed char *) argAryStr[siArg];
      }  /*Else If: reference file input*/

      else if(
         ! eqlNull_ulCp(
            (signed char *) "-out",
            (signed char *) argAryStr[siArg]
         )
      ){ /*Else If: output file input*/
         ++siArg;
         *outFileStrPtr =
            (signed char *) argAryStr[siArg];
      }  /*Else If: output file input*/

      /**************************************************\
      * Fun03 Sec02 Sub02:
      *   - indel removal settings
      *   o fun03 sec02 sub02 cat01:
      *     - minimum homopolymer length
      *   o fun03 sec02 sub02 cat02:
      *     - maximum indel length
      *   o fun03 sec02 sub02 cat03:
      *     - mask setting
      \**************************************************/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun03 Sec02 Sub02 Cat01:
      +   - minimum homopolymer length
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      else if(
         ! eqlNull_ulCp(
            (signed char *) "-homo",
            (signed char *) argAryStr[siArg]
         )
      ){ /*Else If: homopolymer min length*/
         ++siArg;
         tmpStr = (signed char *) argAryStr[siArg];

         tmpStr +=
            strToSI_base10str(
               tmpStr,
               homoLenSIPtr
            );

         if(*tmpStr != '\0')
         { /*If: non-numeric input*/
            fprintf(
               stderr,
               "-homo %s is non-numer or to large%s",
               argAryStr[0],
               str_endLine
            );
            goto err_fun03_sec03;
         } /*If: non-numeric input*/

         if(*homoLenSIPtr < 0)
         { /*If: invalid input*/
            fprintf(
               stderr,
               "-homo %s is less than 0%s",
               argAryStr[0],
               str_endLine
            );
            goto err_fun03_sec03;
         } /*If: invalid input*/
      }  /*Else If: homopolymer min length*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun03 Sec02 Sub02 Cat02:
      +   - maximum indel length
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      else if(
         ! eqlNull_ulCp(
            (signed char *) "-indel",
            (signed char *) argAryStr[siArg]
         )
      ){ /*Else If: indel maximum length*/
         ++siArg;
         tmpStr = (signed char *) argAryStr[siArg];

         tmpStr +=
            strToSI_base10str(
               tmpStr,
               indelLenSIPtr
            );

         if(*tmpStr != '\0')
         { /*If: non-numeric input*/
            fprintf(
               stderr,
               "-indel %s is non-numer or to large%s",
               argAryStr[0],
               str_endLine
            );
            goto err_fun03_sec03;
         } /*If: non-numeric input*/

         if(*homoLenSIPtr <= 0)
         { /*If: invalid input*/
            fprintf(
               stderr,
               "-indel %s is less than or equal to 0%s",
               argAryStr[0],
               str_endLine
            );
            goto err_fun03_sec03;
         } /*If: invalid input*/
      }  /*Else If: indel maximum length*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun03 Sec02 Sub02 Cat03:
      +   - mask setting
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      else if(
         ! eqlNull_ulCp(
            (signed char *) "-mask",
            (signed char *) argAryStr[siArg]
         )
      ){ /*Else If: base to mask deletions with*/
         ++siArg;
         *maskSCPtr = (signed char) argAryStr[siArg][0];
      }  /*Else If: base to mask deletions with*/

      else if(
         ! eqlNull_ulCp(
            (signed char *) "-no-mask",
            (signed char *) argAryStr[siArg]
         )
      ) *maskSCPtr = 0;

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun03 Sec02 Sub02 Cat04:
      +   - scan setting
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      else if(
         ! eqlNull_ulCp(
            (signed char *) "-scan",
            (signed char *) argAryStr[siArg]
         )
      ) *scanBlPtr = 1;

      else if(
         ! eqlNull_ulCp(
            (signed char *) "-no-scan",
            (signed char *) argAryStr[siArg]
         )
      ) *scanBlPtr = 0;

      /**************************************************\
      * Fun03 Sec02 Sub03:
      *   - help message
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
      * Fun03 Sec02 Sub04:
      *   - version number
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
      * Fun03 Sec02 Sub05:
      *   - unknown input
      \**************************************************/

      else
      { /*Else: unkown input*/
         fprintf(
            stderr,
            "%s is not recongized%s",
            argAryStr[siArg],
            str_endLine
         );

         goto err_fun03_sec03;
      } /*Else: unkown input*/

      /**************************************************\
      * Fun03 Sec02 Sub06:
      *   - move to next argument
      \**************************************************/

      ++siArg;
   } /*Loop: get user input*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec03:
   ^   - return result (and print help/version)
   \>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/

   errSC = 0;
   goto ret_fun03_sec03;

   phelp_fun03_sec03:;
      phelp_mainRmHomo(stdout);
      errSC = 1;
      goto ret_fun03_sec03;

   pversion_fun03_sec03:;
      pversion_mainRmHomo(stdout);
      errSC = 1;
      goto ret_fun03_sec03;

   err_fun03_sec03:;
      errSC = 2;
      goto ret_fun03_sec03;

   ret_fun03_sec03:;
     return errSC;
} /*input_mainRmHomo*/

/*-------------------------------------------------------\
| Main:
|   - driver function for rmHomo
| Input:
|   - numArgsSI:
|     o number arguments user input (in argAryStr)
|   - argAryStr:
|     o c-string array with user input
| Output:
|   - Prints:
|     o corrected sam file to -out or stdout (if no -out)
|     o if requested; help message or version to stdout
|     o errors to stderr (if have errors)
|   - Returns:
|     o 0 for no errors
|     o 1 for errors
\-------------------------------------------------------*/
int
main(
   int numArgsSI,    /*number arguments in argAryStr*/
   char *argAryStr[] /*user input arguments*/
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Main TOC:
   '   - driver function for rmHomo
   '   o main sec01:
   '     - variable declarations
   '   o main sec02:
   '     - initialize, get input, and check input
   '   o main sec05:
   '     - clean up and return
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   signed char errSC = 0;
   signed char *samFileStr = (signed char *) "-";
   signed char *refFileStr = (signed char *) "-";
   signed char *outFileStr = (signed char *) "-";

   signed int homoLenSI = def_minHomo_mainRmHomo;
   signed int indelLenSI = def_maxIndel_mainRmHomo;
   signed char maskSC = def_mask_mainRmHomo;
   signed char scanBl = def_scan_mainRmHomo;

   /*variables for refernce sequences*/
   struct seqST *refHeapAryST = 0;
   signed long refLenSL = 0;
   signed long refSizeSL = 0;
   signed long indexSL = 0; /*index of reference*/

   /*for reading sam files*/
   struct samEntry samStackST;
   signed char *buffHeapStr = 0;
   unsigned long lenBuffUL = 0;
   signed long seqSL = 0;

   /*for read triming*/
   unsigned int lenSeqBuffUI = 0;
   unsigned int lenQBuffUI = 0;
   signed char *qBuffNoFreeStr = 0;
      /*gets sets to buffHeapStr*/

   FILE *samFILE = 0;
   FILE *outFILE = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec02:
   ^   - initialize, get input, and check input
   ^   o main sec02 sub01
   ^     - initialize variables
   ^   o main sec02 sub02
   ^     - get input
   ^   o main sec02 sub03:
   ^     - allocate memory
   ^   o main sec02 sub04:
   ^     - make sure reference and sam file are different
   ^   o main sec02 sub05:
   ^     - get reference
   ^   o main sec02 sub06:
   ^     - open sam (read) file
   ^   o main sec02 sub07:
   ^     - open output file
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

   errSC =
      input_mainRmHomo(
         numArgsSI,
         argAryStr,
         &samFileStr,
         &refFileStr,
         &outFileStr,
         &homoLenSI,
         &indelLenSI,
         &maskSC,
         &scanBl
      );

   if(errSC)
   { /*If: had error or printed help/version*/
      --errSC; /*convert help/version error to no error*/
      goto ret_main_sec05;
         /*input_mainRmHomoe printed error message */
   } /*If: had error or printed help/version*/

   /*****************************************************\
   * Main Sec02 Sub03:
   *   - allocate memory
   \*****************************************************/
   
   if( setup_samEntry(&samStackST) )
   { /*If: memory error*/
      fprintf(
         stderr,
         "memory error setting up samEntry%s",
         str_endLine
      );
      goto err_main_sec05;
   } /*If: memory error*/

   /*****************************************************\
   * Main Sec02 Sub04:
   *   - make sure reference and sam file are different
   \*****************************************************/

   if(
      ! eqlNull_ulCp(
         refFileStr,
         samFileStr
      )
   ){ /*If: sam file and reference from same source*/
      fprintf(
         stderr,
         "-ref %s and -sam %s are same file%s",
         refFileStr,
         samFileStr,
         str_endLine
      );
      goto err_main_sec05;
   }  /*If: sam file and reference from same source*/

   /*****************************************************\
   * Main Sec02 Sub05:
   *   - get reference
   *   o main sec02 sub05 cat01:
   *     - open reference file
   *   o main sec02 sub05 cat02:
   *     - get reference sequence
   \*****************************************************/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Main Sec02 Sub05 Cat01:
   +   - open reference file
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   if(refFileStr[0] == '-')
      samFILE = stdin;
   else
   { /*Else: user input reference*/
      samFILE =
         fopen(
            (char *) refFileStr,
            "r"
         );

      if(! samFILE)
      { /*If: could not open reference file*/
         fprintf(
            stderr,
            "could not open -ref %s%s",
            refFileStr,
            str_endLine
         );
         goto err_main_sec05;
      } /*If: could not open reference file*/
   } /*Else: user input reference*/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Main Sec02 Sub05 Cat02:
   +   - get reference sequence
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   refHeapAryST =
      readFaFile_seqST(
         samFILE,
         &refLenSL,
         &refSizeSL,
         &errSC
      ); /*get all references*/

   if(samFILE != stdin)
     fclose(samFILE);
   samFILE = 0;

   if(errSC)
   { /*If: had error*/
      if(errSC == def_memErr_seqST)
         fprintf(
            stderr,
            "memory error reading -ref %s%s",
            refFileStr,
            str_endLine
         );
      else if(errSC == def_fileErr_seqST)
         fprintf(
            stderr,
            "-ref %s is not a fasta file%s",
            refFileStr,
            str_endLine
         );
      else
         fprintf(
            stderr,
            "fist entry in -ref %s has bad line%s",
            refFileStr,
            str_endLine
         );

      goto err_main_sec05;
   } /*If: had error*/

   /*I need to convert the reference id to what would be
   `  seen in a sam file; no >, and ends after first
   `  white space
   */
   for(seqSL = 0; seqSL < refLenSL; ++seqSL)
   { /*Loop: remove white space from names*/
      errSC = refHeapAryST[seqSL].idStr[0] == '>';
      errSC |= refHeapAryST[seqSL].idStr[0] == '@';

      refHeapAryST[seqSL].idLenSL =
         cpWhite_ulCp(
            refHeapAryST[seqSL].idStr,
            &refHeapAryST[seqSL].idStr[
              (unsigned char) errSC
            ]
         );
   } /*Loop: remove white space from names*/

   seqSL = 0;
   sort_seqST(refHeapAryST, refLenSL);

   /*****************************************************\
   * Main Sec02 Sub06:
   *   - open sam (read) file
   \*****************************************************/

   if(samFileStr[0] == '-')
      samFILE = stdin;
   else
   { /*Else: user input sam file*/
      samFILE =
         fopen(
            (char *) samFileStr,
            "r"
         );

      if(! samFILE)
      { /*If: could not open sam file*/
         fprintf(
            stderr,
            "could not open -sam %s%s",
            samFileStr,
            str_endLine
         );
         goto err_main_sec05;
      } /*If: could not open sam file*/
   } /*Else: user input samput file*/

   /*****************************************************\
   * Main Sec02 Sub07:
   *   - open output file
   \*****************************************************/

   if(outFileStr[0] == '-')
      outFILE = stdout;
   else
   { /*Else: user input output file*/
      outFILE =
         fopen(
            (char *) outFileStr,
            "w"
         );

      if(! outFILE)
      { /*If: could not open output file*/
         fprintf(
            stderr,
            "could not open -out %s%s",
            outFileStr,
            str_endLine
         );
         goto err_main_sec05;
      } /*If: could not open output file*/
   } /*Else: user input output file*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec03:
   ^   - print the sam file header
   ^   o main sec03 sub01:
   ^     - get first sam flie line + start rm indel loop
   ^   o main sec03 sub02:
   ^     - print header in sam file
   ^   o main sec03 sub03:
   ^     - print rmHomo program header entry
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Main Sec03 Sub01:
   *   - get first line in sam file + start rm indel loop
   \*****************************************************/

   errSC = get_samEntry(&samStackST, samFILE);

   if(errSC)
   { /*If: had error*/
      if(errSC == def_memErr_samEntry)
         fprintf(
            stderr,
            "memory error reading 1st line in -sam %s%s",
            samFileStr,
            str_endLine
         );
      else
         fprintf(
            stderr,
            "nothing in -sam %s%s",
            samFileStr,
            str_endLine
         );
      goto err_main_sec05;
   } /*If: had error*/

   /*****************************************************\
   * Main Sec03 Sub02:
   *   - print header in sam file
   *   o main sec03 sub02 cat01:
   *     - print headers
   *   o main sec03 sub02 cat02:
   *     - check for errors
   \*****************************************************/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Main Sec03 Sub02 Cat01:
   +   - print headers
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   while(! errSC)
   { /*Loop: print headers*/
      if(samStackST.extraStr[0] != '@')
         break; /*end of header*/
      else if(
            samStackST.extraStr[1] == 'S'
         && samStackST.extraStr[2] == 'Q'
         && samStackST.extraStr[3] == '\t'
      ){ /*Else If: sequence entry (check if reference)*/
         
         for(
				indexSL = 7;
            samStackST.extraStr[indexSL] > 33;
            ++indexSL
         ) ;

         errSC = samStackST.extraStr[indexSL];
         samStackST.extraStr[indexSL] = 0;

         if(
            search_seqST(
               refHeapAryST,
               &samStackST.extraStr[7],
               refLenSL
            ) < 0
         ) goto nextEntry_main_sec03_sub02_cat01;
           /*not reference sequence entry*/

         samStackST.extraStr[indexSL] = errSC;
         errSC = 0;
      }  /*Else If: sequence entry (check if reference)*/

      /*print the header*/
      p_samEntry(&samStackST, 0, outFILE);

      nextEntry_main_sec03_sub02_cat01:;
         errSC = get_samEntry(&samStackST, samFILE);
   } /*Loop: print headers*/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Main Sec03 Sub02 Cat02:
   +   - check for errors
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   if(errSC)
   { /*If: had error*/
      if(errSC == def_memErr_samEntry)
         fprintf(
            stderr,
            "memory error printing headers%s",
            str_endLine
         );
      else
         fprintf(
            stderr,
            "file error reading header in -sam %s%s",
            samFileStr,
            str_endLine
         );
   } /*If: had error*/

   /*****************************************************\
   * Main Sec03 Sub03:
   *   - print rmHomo program header entry
   \*****************************************************/

   fprintf(
      outFILE,
      "@PG\tID:rmHomo\tPN:rmHomo\tVN:%i-%02i-%02i",
      def_year_bioTools,
      def_month_bioTools,
      def_day_bioTools
   );

   fprintf(
      outFILE,
      "\tCL:rmHomo -homo %i -indel %i",
      homoLenSI,
      indelLenSI
   );

   if(maskSC)
      fprintf(
         outFILE,
         " -mask %c",
         maskSC
      );
   else
      fprintf(
         outFILE,
         " -no-mask"
      );

   if(scanBl)
      fprintf(outFILE, " -scan");
   else
      fprintf(outFILE, " -no-scan");

   fprintf(
      outFILE,
      " -sam %s -ref %s -out %s%s",
      samFileStr,
      refFileStr,
      outFileStr,
      str_endLine
   );

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec04:
   ^   - remove indels
   ^   o main sec04 sub01:
   ^     - check if can remove indels from read
   ^   o main sec04 sub02:
   ^     - make sure have enough memory to remove indels
   ^   o main sec04 sub03:
   ^     - remove indels
   ^   o main sec04 sub04:
   ^     - get next read
   ^   o main sec04 sub05:
   ^     - check for errors
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Main Sec04 Sub01:
   *   - check if can remove indels from read
   \*****************************************************/

   while(! errSC)
   { /*Loop: remove indels*/
      ++seqSL;

      /*checking to see if can remove indels*/
      if(samStackST.seqStr[0] == '\0')
         goto nextRead_main_sec04_sub04;
      else if(samStackST.seqStr[0] == '*')
         goto nextRead_main_sec04_sub04;
      else if(samStackST.flagUS & 4)
         goto nextRead_main_sec04_sub04;

      indexSL =
         search_seqST(
            refHeapAryST,
            samStackST.refIdStr,
            refLenSL
         );
      if(indexSL < 0)
         goto nextRead_main_sec04_sub04;
         /*read mapped to different reference*/

      /**************************************************\
      * Main Sec04 Sub02:
      *   - make sure have enough memory to remove indels
      \**************************************************/

      /*this trick allows me to decrease memory usage and
      `  reduces memory allocations
      */
      lenSeqBuffUI = samStackST.readLenUI;
      lenSeqBuffUI += samStackST.delCntUI;
      lenSeqBuffUI += 8;

      if(lenBuffUL < (lenSeqBuffUI << 1))
      { /*If: need more memory*/
         free(buffHeapStr);
         buffHeapStr = 0;
         buffHeapStr =
            malloc(
               (lenSeqBuffUI << 1) * sizeof(signed char)
            );
         if(! buffHeapStr)
         { /*If: had memory error*/
            fprintf(
               stderr,
               "memory error on read %li in -sam %s%s",
               seqSL,
               samFileStr,
               str_endLine
            );
            goto err_main_sec05;
         } /*If: had memory error*/
      } /*If: need more memory*/

      lenQBuffUI = lenSeqBuffUI;
      qBuffNoFreeStr = &buffHeapStr[lenSeqBuffUI];

      /**************************************************\
      * Main Sec04 Sub03:
      *   - remove indels
      \**************************************************/

      if(
         indel_rmHomo(
            &samStackST,
            refHeapAryST[indexSL].seqStr,
            homoLenSI,
            indelLenSI,
            maskSC,
            scanBl,
            &buffHeapStr,
            &lenSeqBuffUI,
            &qBuffNoFreeStr,
            &lenQBuffUI
         )
         /*I set this up so that qBuffNoFreeStr and
         `   buffHeapStr will never be resized by
         `   indel_rmHomo
         */  
      ){ /*If: memory error removing indels*/
            fprintf(
               stderr,
               "error removing indels in read %li%s",
               seqSL,
               str_endLine
            );
            goto err_main_sec05;
      }  /*If: memory error removing indels*/

      p_samEntry(&samStackST, 0, outFILE);
      
      /**************************************************\
      * Main Sec04 Sub04:
      *   - get next read
      \**************************************************/

      nextRead_main_sec04_sub04:;
         errSC = get_samEntry(&samStackST, samFILE);
   } /*Loop: remove indels*/

   /*****************************************************\
   * Main Sec04 Sub05:
   *   - check for errors
   \*****************************************************/

   if(errSC == def_memErr_samEntry)
   { /*If: had error*/
      fprintf(
         stderr,
         "memory error on read %li%s",
         seqSL,
         str_endLine
      );
      goto err_main_sec05;
   } /*If: had error*/

   else if(errSC != def_EOF_samEntry)
   { /*If: had error*/
      fprintf(
         stderr,
         "file error on read %li%s",
         seqSL,
         str_endLine
      );
      goto err_main_sec05;
   } /*If: had error*/


   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec05:
   ^   - clean up and return
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   errSC = 0;
   goto ret_main_sec05;

   err_main_sec05:;
      errSC = 1;
      goto ret_main_sec05;

   ret_main_sec05:;
      freeStack_samEntry(&samStackST);
      freeHeapAry_seqST(refHeapAryST, refLenSL);

      if(buffHeapStr)
         free(buffHeapStr);
      buffHeapStr = 0;
      lenBuffUL = 0;

      if(
            samFILE
         && samFILE != stdin
         && samFILE != stdout
         && samFILE != stderr
      ) fclose(samFILE);
      samFILE = 0;

      if(
            outFILE
         && outFILE != stdin
         && outFILE != stdout
         && outFILE != stderr
      ) fclose(outFILE);
      outFILE = 0;

      return errSC;
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
