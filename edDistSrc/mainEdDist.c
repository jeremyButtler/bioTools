/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' mainEdDist TOC:
'   - driver for finding edit distances
'   o fun01: pversion_mainEdDist
'     - prints version number for edDist
'   o fun02: phelp_mainEdDist
'     - prints help message for edDist
'   o fun03: input_mainEdDist
'     - gets input for edDist
'   o main:
'     - driver function for edDist
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

#include "../genLib/base10str.h"
#include "../genLib/charCp.h"
#include "../genLib/samEntry.h"
#include "../genLib/edDist.h"

/*.h files only*/
#include "../genLib/dataTypeShortHand.h"

#define def_readRate_mainEdDist 0
#define def_minIndelLen_mainEdDist 10
#define def_minSnpQ_mainEdDist 7
#define def_errRate_mainEdDist 0.023f

#define def_year_mainEdDist 2024
#define def_month_mainEdDist 9
#define def_day_mainEdDist 10

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\
! Hidden libraries:
!   o .c  #include "../genLib/ulCp.h"
!   o .c   #include "../genLib/base10str.h"
!   o .c   #include "../genLib/numToStr.h"
!   o .h   #include "../genLib/ntTo5Bit.h"
!   o .h   #include "../genLib/genMath.h" not using .c
\%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

/*-------------------------------------------------------\
| Fun01: pversion_mainEdDist
|   - prints version number for edDist
| Input:
|   - outFILE:
|     o file to print version number to
| Output:
|   - Prints:
|     o version number to outFILE
\-------------------------------------------------------*/
void
pversion_mainEdDist(
   void *outFILE
){
   fprintf(
      (FILE *) outFILE,
      "edDist version: %i-%02i-%02i\n",
      def_year_mainEdDist,
      def_month_mainEdDist,
      def_day_mainEdDist
   );
} /*pversion_mainEdDist*/

/*-------------------------------------------------------\
| Fun02: phelp_mainEdDist
|   - prints help message for edDist
| Input:
|   - outFILE:
|     o file to print help message to
| Output:
|   - Prints:
|     o help message to outFILE
\-------------------------------------------------------*/
void
phelp_mainEdDist(
   void *outFILE
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun02 TOC:
   '   - prints help message for edDiist
   '   o fun02 sec01:
   '     - print usage block
   '   o fun02 sec02:
   '     - print input block
   '   o fun02 sec02:
   '     - print output block
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun02 Sec01:
   ^   - print usage block
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   fprintf(
      (FILE *) outFILE,
      "edDist -sam reads.sam\n"
   );

   fprintf(
      (FILE *) outFILE,
      "  - prints an table of adjusted edit distances\n"
   );

   fprintf(
      (FILE *) outFILE,
      "  - edit distance:\n"
   );

   fprintf(
      (FILE *) outFILE,
      "    (SNPs with q-score > %i) + (indels > %i)\n",
      def_minSnpQ_mainEdDist - 1,
      def_minIndelLen_mainEdDist - 1
   );

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun02 Sec02:
   ^   - print input block
   ^   o fun02 sec02 sub01:
   ^     - input block
   ^   o fun02 sec02 sub02:
   ^     - read input and file output
   ^   o fun02 sec02 sub03:
   ^     - reference input
   ^   o fun02 sec02 sub04:
   ^     - filtering input
   ^   o fun02 sec02 sub05:
   ^     - error rate varaibles
   ^   o fun02 sec02 sub06:
   ^     - help and version
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun02 Sec02 Sub01:
   *   - input block
   \*****************************************************/

   fprintf(
      (FILE *) outFILE,
      "Input:\n"
   );

   /*****************************************************\
   * Fun02 Sec02 Sub02:
   *   - read input and file output
   \*****************************************************/

   fprintf(
      (FILE *) outFILE,
      "  -sam reads.sam: [Required]\n"
   );

   fprintf(
      (FILE *) outFILE,
      "    o sam file with reads to find edit distance\n"
   );

   fprintf(
      (FILE *) outFILE,
      "    o use \"-sam -\" for stdin input\n"
   );


   fprintf(
      (FILE *) outFILE,
      "  -out stats.tsv: [Optional; stdout]\n"
   );

   fprintf(
      (FILE *) outFILE,
      "    o file to print distance table to\n"
   );

   fprintf(
      (FILE *) outFILE,
      "    o use \"-out -\" for stdout\n"
   );

   /*****************************************************\
   * Fun02 Sec02 Sub03:
   *   - reference input
   \*****************************************************/

   fprintf(
      (FILE *) outFILE,
      "  -ref ref.sam: [Optional; No]\n"
   );

   fprintf(
      (FILE *) outFILE,
      "    o sam file with reference mapped to same\n"
   );

   fprintf(
      (FILE *) outFILE,
      "      reference as read\n"
   );

   fprintf(
      (FILE *) outFILE,
      "    o edit distence is for -ref ref.sam not\n"
   );

   fprintf(
      (FILE *) outFILE,
      "      orignal reference\n"
   );

   fprintf(
      (FILE *) outFILE,
      "    o insertions are not re-aligned, so only\n"
   );

   fprintf(
      (FILE *) outFILE,
      "      number of insertions present is checked\n"
   );

   /*****************************************************\
   * Fun02 Sec02 Sub04:
   *   - filtering input
   \*****************************************************/

   fprintf(
      (FILE *) outFILE,
      "  -q %i: [Optional; %i]\n",
      def_minSnpQ_mainEdDist,
      def_minSnpQ_mainEdDist
   );

   fprintf(
      (FILE *) outFILE,
      "    o minimum q-score to count a SNP\n"
   );


   fprintf(
      (FILE *) outFILE,
      "  -indel %i: [Optional; %i]\n",
      def_minIndelLen_mainEdDist,
      def_minIndelLen_mainEdDist
   );

   fprintf(
      (FILE *) outFILE,
      "    o minimum indel length to count an indel\n"
   );

   fprintf(
      (FILE *) outFILE,
      "    o the full indel length is added to distance\n"
   );

   /*****************************************************\
   * Fun02 Sec02 Sub05:
   *   - error rate varaibles
   \*****************************************************/

   fprintf(
      (FILE *) outFILE,
      "  -err %0.2f: [Optional; %0.2f]\n",
      def_errRate_mainEdDist,
      def_errRate_mainEdDist
   );
   
   fprintf(
      (FILE *) outFILE,
      "    o expected error rates for reads\n"
   );


   if(def_readRate_mainEdDist)
      fprintf(
         (FILE *) outFILE,
         "  -cmp-read: [Optional; Yes]\n"
      );
   else
      fprintf(
         (FILE *) outFILE,
         "  -cmp-read: [Optional; No]\n"
      );
   
   fprintf(
      (FILE *) outFILE,
      "    o use read error rate (error rate * 2)\n"
   );

   fprintf(
      (FILE *) outFILE,
      "    o disable with \"-cmp-con\"\n"
   );

   /*****************************************************\
   * Fun02 Sec02 Sub06:
   *   - help and version
   \*****************************************************/

   fprintf(
      (FILE *) outFILE,
      "  -h: print this help message and exit\n"
   );

   fprintf(
      (FILE *) outFILE,
      "  -v: print version number and exit\n"
   );

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun02 Sec03:
   ^   - print output block
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   fprintf(
      (FILE *) outFILE,
      "Output:\n"
   );

   fprintf(
      (FILE *) outFILE,
      "  - prints read id, reference id, edit distance,\n"
   );

   fprintf(
      (FILE *) outFILE,
      "    edit distance / error rate, overlap length,\n"
   );

   fprintf(
      (FILE *) outFILE,
      "    number indels counted, and number indel\n"
   );

   fprintf(
      (FILE *) outFILE,
      "    events to stdout\n"
   );
} /*phelp_mainEdDist*/

/*-------------------------------------------------------\
| Fun03: input_mainEdDist
|   - gets input for edDist
| Input:
|   - numArgsSI:
|     o number of arguments the user input
|   - argAryStr:
|     o array of c-strings with user input
|   - samFileStrPtr:
|     o pointer to c-string to point to sam file path
|   - refFileStrPtr:
|     o pointer to c-string to point to reference file
|   - outFileStrPtr:
|     o pointer to c-string to point to output file path
|   - minQUCPtr:
|     o pointer to unsigned char to hold min q-score
|   - minIndelLenUIPtr:
|     o pointer to unsigned int to hold min indel length
|   - errRateFPtr:
|     o pointer to float to hold error rate
|   - readCmpBlPtr:
|     o pointer to signed char to be set to
|     o 1: apply read comaprsion error rate
|     o 0: do not apply read comaprsion error rate
| Output:
|   - Modfies:
|     o all input, except numArgsSI and argAryStr, to hold
|       user input
|   - Prints:
|     o error message to stderr (if had errors)
|     o help message or version number to stdout
|       (if requested)
|   - Returns:
|     o 0 for no errors
|     o 1 if printed help message or version number
|     o 2 if had an error
\-------------------------------------------------------*/
signed char
input_mainEdDist(
   int numArgsSI,
   char *argAryStr[],
   signed char **samFileStrPtr,
   signed char **refFileStrPtr,
   signed char **outFileStrPtr,
   unsigned char *minQUCPtr,
   unsigned int *minIndelLenUIPtr,
   float *errRateFPtr,
   signed char *readCmpBlPtr
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun03 TOC:
   '   - gets input for edDist
   '   o fun03 sec01:
   '     - variable declarations
   '   o fun03 sec02:
   '     - check if have input
   '   o fun03 sec03:
   '     - get input
   '   o fun03 sec04:
   '     - return result
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   sint siArg = 1;
   schar errSC = 0;
   schar *tmpStr = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec02:
   ^   - check if have input
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(numArgsSI <= 1)
   { /*If: nothing input*/
      phelp_mainEdDist(stdout);
      goto phelp_fun03_sec04;
   } /*If: nothing input*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec03:
   ^   - get input
   ^   o fun03 sec03 sub01:
   ^     - file IO + start loop
   ^   o fun03 sec03 sub02:
   ^     - filter input
   ^   o fun03 sec03 sub03:
   ^     - error rate input
   ^   o fun03 sec03 sub04:
   ^     - help message
   ^   o fun03 sec03 sub05:
   ^     - version number
   ^   o fun03 sec03 sub06:
   ^     - invalid input
   ^   o fun03 sec03 sub07:
   ^     - move to next argument
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun03 Sec03 Sub01:
   *   - file IO + start loop
   \*****************************************************/

   while(siArg < numArgsSI)
   { /*Loop: get input*/
      if(
         ! eql_charCp(
            (schar *) "-sam",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*If: sam file with reads input*/
         ++siArg;
         *samFileStrPtr = (schar *) argAryStr[siArg];
      } /*If: sam file with reads input*/

      else if(
         ! eql_charCp(
            (schar *) "-ref",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else If: reference file*/
         ++siArg;
         *refFileStrPtr = (schar *) argAryStr[siArg];
      } /*Else If: reference file*/

      else if(
         ! eql_charCp(
            (schar *) "-out",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else If: output file*/
         ++siArg;
         *outFileStrPtr = (schar *) argAryStr[siArg];
      } /*Else If: output file*/

      /**************************************************\
      * Fun03 Sec03 Sub02:
      *   - filter input
      \**************************************************/

      else if(
         ! eql_charCp(
            (schar *) "-q",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else If: minimum q-score for snp*/
         ++siArg;
         tmpStr = (schar *) argAryStr[siArg];

         tmpStr +=
            strToUC_base10str(
               tmpStr,
               minQUCPtr     
            );

         if(*tmpStr != '\0')
         { /*If: non-numeric or to large*/
            fprintf(
               stderr,
               "-q %s is to large or non-numeric\n",
               argAryStr[siArg]
            );

            goto err_fun03_sec04;
         } /*If: non-numeric or to large*/
      } /*Else If: minimum q-score for snp*/

      else if(
         ! eql_charCp(
            (schar *) "-indel",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else If: min indel length*/
         ++siArg;
         tmpStr = (schar *) argAryStr[siArg];

         tmpStr +=
            strToUI_base10str(
               tmpStr,
               minIndelLenUIPtr
            );

         if(*tmpStr != '\0')
         { /*If: non-numeric or to large*/
            fprintf(
               stderr,
               "-indel %s is to large or non-numeric\n",
               argAryStr[siArg]
            );

            goto err_fun03_sec04;
         } /*If: non-numeric or to large*/
      } /*Else If: min indel length*/

      /**************************************************\
      * Fun03 Sec03 Sub03:
      *   - error rate input
      \**************************************************/

      else if(
         ! eql_charCp(
            (schar *) "-err",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else If: error rate input*/
         ++siArg;
         *errRateFPtr = atof(argAryStr[siArg]);
      } /*Else If: error rate input*/

      else if(
         ! eql_charCp(
            (schar *) "-cmp-read",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ) *readCmpBlPtr = 1;

      else if(
         ! eql_charCp(
            (schar *) "-cmp-con",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ) *readCmpBlPtr = 0;

      /**************************************************\
      * Fun03 Sec03 Sub04:
      *   - help message
      \**************************************************/

      else if(
         ! eql_charCp(
            (schar *) "-h",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else If: help message*/
         phelp_mainEdDist(stdout);
         goto phelp_fun03_sec04;
      } /*Else If: help message*/

      else if(
         ! eql_charCp(
            (schar *) "--h",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else If: help message*/
         phelp_mainEdDist(stdout);
         goto phelp_fun03_sec04;
      } /*Else If: help message*/

      else if(
         ! eql_charCp(
            (schar *) "help",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else If: help message*/
         phelp_mainEdDist(stdout);
         goto phelp_fun03_sec04;
      } /*Else If: help message*/

      else if(
         ! eql_charCp(
            (schar *) "-help",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else If: help message*/
         phelp_mainEdDist(stdout);
         goto phelp_fun03_sec04;
      } /*Else If: help message*/

      else if(
         ! eql_charCp(
            (schar *) "--help",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else If: help message*/
         phelp_mainEdDist(stdout);
         goto phelp_fun03_sec04;
      } /*Else If: help message*/

      /**************************************************\
      * Fun03 Sec03 Sub05:
      *   - version number
      \**************************************************/

      else if(
         ! eql_charCp(
            (schar *) "-v",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else If: version number*/
         pversion_mainEdDist(stdout);
         goto pversion_fun03_sec04;
      } /*Else If: version number*/

      else if(
         ! eql_charCp(
            (schar *) "--v",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else If: version number*/
         pversion_mainEdDist(stdout);
         goto pversion_fun03_sec04;
      } /*Else If: version number*/

      else if(
         ! eql_charCp(
            (schar *) "version",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else If: version number*/
         pversion_mainEdDist(stdout);
         goto pversion_fun03_sec04;
      } /*Else If: version number*/

      else if(
         ! eql_charCp(
            (schar *) "-version",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else If: version number*/
         pversion_mainEdDist(stdout);
         goto pversion_fun03_sec04;
      } /*Else If: version number*/

      else if(
         ! eql_charCp(
            (schar *) "--version",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else If: version number*/
         pversion_mainEdDist(stdout);
         goto pversion_fun03_sec04;
      } /*Else If: version number*/

      /**************************************************\
      * Fun03 Sec03 Sub06:
      *   - invalid input
      \**************************************************/

      else
      { /*Else: invalid input*/
         fprintf(
            stderr,
            "%s is not recognized\n",
            argAryStr[siArg]
         );

         goto err_fun03_sec04;
      } /*Else: invalid input*/

      /**************************************************\
      * Fun03 Sec03 Sub07:
      *   - move to next argument
      \**************************************************/

      ++siArg;
   } /*Loop: get input*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec04:
   ^   - return result
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
} /*input_mainEdDist*/

/*-------------------------------------------------------\
| Main:
|   - driver function for edDist
| Input:
|   - numArgsSI:
|     o number of arguments user input
|   - qrgAryStr:
|     o c-string array with user arguments
| Output:
|   - Prints:
|     o edit distances to -out (default stdout)
\-------------------------------------------------------*/
int
main(
   signed int numArgsSI,
   char *argAryStr[]
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Main TOC:
   '   - runs clustering program
   '   o main sec01:
   '     - variable declerations
   '   o main sec02:
   '     - get user input and open filesj
   '   o main sec03:
   '     - get and print edit distances
   '   o main sec04:
   '     - clean up
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec01:
   ^   - variable declerations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   schar *refFileStr = 0;
   schar *samFileStr = 0;
   schar *outFileStr = 0;

   /*edit distance settings*/
   uint minIndelLenUI = def_minIndelLen_mainEdDist;
   uchar minQUC = def_minSnpQ_mainEdDist;

   float percErrF = def_errRate_mainEdDist;
   schar readCmpBl = def_readRate_mainEdDist;

   schar errSC = 0;
   slong distSL = 0;
   int probSI = 0;

   uint indelEventsUI = 0;
   uint numIndelsUI = 0;
   uint lenOverlapUI = 0;

   FILE *samFILE = 0;
   FILE *outFILE = 0;

   struct samEntry refStackST;
   schar refBl = 0;

   struct samEntry qryStackST;
   schar *buffHeapStr = 0;
   ulong lenBuffUL = 0;
   ulong entryUL = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec02:
   ^   - get user input and open files
   ^   o main sec02 sub01:
   ^     - initialize variables
   ^   o main sec02 sub02:
   ^     - get input
   ^   o main sec02 sub03:
   ^     - set up input
   ^   o main sec02 sub04:
   ^     - open read sam file
   ^   o main sec02 sub05:
   ^     - get reference sequence (if provided)
   ^   o main sec02 sub06:
   ^     - open output file
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Main Sec02 Sub01:
   *   - initialize variables
   \*****************************************************/

   init_samEntry(&refStackST);
   init_samEntry(&qryStackST);

   /*****************************************************\
   * Main Sec02 Sub02:
   *   - get input
   \*****************************************************/

   errSC =
      input_mainEdDist(
         numArgsSI,
         argAryStr,
         &samFileStr,
         &refFileStr,
         &outFileStr,
         &minQUC,
         &minIndelLenUI,
         &percErrF,
         &readCmpBl
   ); /*get user input*/

   if(errSC)
   { /*If: had input error*/
      --errSC; /*reduce help/version error to no error*/
      goto cleanUp_main_sec04;
   } /*If: had input error*/

   /*****************************************************\
   * Main Sec02 Sub03:
   *   - set up input
   \*****************************************************/

   if(readCmpBl)
      percErrF *= 2; /*comparing reads (double rate)*/

   errSC = setup_samEntry(&refStackST);

   if(errSC)
   { /*If: had memory error*/
      fprintf(
         stderr,
         "MEMORY error\n"
      );

      goto err_main_sec04_sub02;
   } /*If: had memory error*/
   
   errSC = setup_samEntry(&qryStackST);

   if(errSC)
   { /*If: had memory error*/
      fprintf(
         stderr,
         "MEMORY error\n"
      );

      goto err_main_sec04_sub02;
   } /*If: had memory error*/

   /*****************************************************\
   * Main Sec02 Sub04:
   *   - get reference sequence (if provided)
   *   o main sec02 sub04 cat01:
   *     - open the reference file
   *   o main sec02 sub04 cat02:
   *     - read in first sam file entry
   *   o main sec02 sub04 cat03:
   *     - get reference sequence and handel errors
   *   o main sec02 sub04 cat04:
   *     - no reference; set id to NA
   \*****************************************************/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Main Sec02 Sub04 Cat01:
   +   - open the reference file
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   if(refFileStr)
   { /*If: user provided a reference to compare*/
      refBl = 1;

      samFILE =
         fopen(
            (char *) refFileStr,
            "r"
         );

      if(! samFILE)
      { /*If: error*/
         fprintf(
            stderr,
            "could not open -ref %s\n",
            refFileStr
         );

         goto err_main_sec04_sub02;
      } /*If: error*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Main Sec02 Sub04 Cat02:
      +   - read in first sam file entry
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      errSC =
         get_samEntry(
            &refStackST,
            &buffHeapStr,
            &lenBuffUL,
            samFILE
         );

      if(errSC)
      { /*If: error*/
         if(errSC == def_memErr_samEntry)
            fprintf(
               stderr,
               "memory error reading reference\n"
            );

         else
            fprintf(
               stderr,
               "-ref %s has nothing\n",
               refFileStr
            );

         goto err_main_sec04_sub02;
      } /*If: error*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Main Sec02 Sub04 Cat03:
      +   - get reference sequence and handel errors
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      while(! errSC)
      { /*Loop: get reference sequence*/
         if(*refStackST.extraStr != '@')
            break;

         errSC =
            get_samEntry(
               &refStackST,
               &buffHeapStr,
               &lenBuffUL,
               samFILE
            );
      } /*Loop: get reference sequence*/

      if(errSC)
      { /*If: error*/
         if(errSC == def_memErr_samEntry)
            fprintf(
               stderr,
               "memory error reading reference\n"
            );

         else if(*refStackST.extraStr == '@')
            fprintf(
               stderr,
               "-ref %s has no sequences\n",
               refFileStr
            );

         goto err_main_sec04_sub02;
      } /*If: error*/

      fclose(samFILE);
      samFILE = 0;
   } /*If: user provided a reference to compare*/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Main Sec02 Sub04 Cat04:
   +   - no reference; set id to NA
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   else
   { /*Else: no reference*/
      refStackST.qryIdStr[0] = 'N';
      refStackST.qryIdStr[1] = 'A';
      refStackST.qryIdStr[2] = '\0';
   } /*Else: no reference*/

   /*****************************************************\
   * Main Sec02 Sub05:
   *   - open read sam file
   \*****************************************************/

   if(
         ! samFileStr
      || *samFileStr == '-'
   ) samFILE = stdin;

   else
   { /*Else: user provided a sam file*/
      samFILE = 
         fopen(
            (char *) samFileStr,
            "r"
         );
          
      if(! samFILE)
      { /*If: could not open sam file*/
         fprintf(
            stderr,
            "could not open -sam %s\n",
            samFileStr
         );

         goto err_main_sec04_sub02;
      } /*If: could not open sam file*/
   } /*Else: user provided a sam file*/

   /*****************************************************\
   * Main Sec02 Sub06:
   *   - open output file
   \*****************************************************/

   if(
         ! outFileStr
      || *outFileStr == '-'
   ) outFILE = stdout;

   else
   { /*Else: user provided an output file*/
      outFILE = 
         fopen(
            (char *) outFileStr,
            "w"
         );
          
      if(! outFILE)
      { /*If: could not open output file*/
         fprintf(
            stderr,
            "could not open -out %s\n",
            outFileStr
         );

         goto err_main_sec04_sub02;
      } /*If: could not open output file*/
   } /*Else: user provided an output file*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec03:
   ^   - get and print edit distances
   ^   o main sec03 sub01:
   ^     - print header
   ^   o main sec03 sub02:
   ^     - get first sam file entry
   ^   o main sec03 sub03:
   ^     - get and print edit distances
   ^   o main sec03 sub04:
   ^     - print out file errors
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Main Sec03 Sub01:
   *   - print header
   \*****************************************************/

   entryUL = 1;

   fprintf(
      stdout,
     "qry\tref\tdist\tdist_div_err\taln_len"
   );

   fprintf(
      stdout,
     "\tnum_indels\tindel_events\n"
   );

   /*****************************************************\
   * Main Sec03 Sub02:
   *   - get first sam file entry
   \*****************************************************/

   errSC =
      (schar)
      get_samEntry(
         &qryStackST,
         &buffHeapStr,
         &lenBuffUL,
         samFILE
      );

   if(errSC)
   { /*If: error*/
      if(errSC == def_memErr_samEntry)
         fprintf(
            stderr,
            "memory error first len of -sam %s\n",
            samFileStr
         );

      else
         fprintf(
            stderr,
            "-sam %s has nothing\n",
            samFileStr
         );

      goto err_main_sec04_sub02;
   } /*If: error*/

   /*****************************************************\
   * Main Sec03 Sub03:
   *   - get and print edit distances
   \*****************************************************/

   while(! errSC)
   { /*Loop: get edit distances*/
      ++entryUL;

      if(refBl)
      { /*If: comparing reference and read*/
         distSL =
            readCmpDist_edDist(
               &qryStackST,
               &refStackST,
               minIndelLenUI,
               minQUC,
               &lenOverlapUI,
               &numIndelsUI,
               &indelEventsUI
            );
      } /*If: comparing reference and read*/

      else
      { /*Else: compare read to mapped ref*/
         distSL =
            dist_edDist(
               &qryStackST,
               minIndelLenUI,
               minQUC,
               &numIndelsUI,
               &indelEventsUI
            );

         lenOverlapUI = qryStackST.alnReadLenUI;
      } /*Else: compare read to mapped ref*/

      if(distSL >= 0)
      { /*If: have edit distance*/
         probSI =
            percDist_edDist(
               distSL,
               lenOverlapUI,
               percErrF
            ); /*probablity is true*/

         printf(
            "%s\t%s\t%li\t%i\t%u\t%u\t%u\n",
            qryStackST.qryIdStr,
            refStackST.qryIdStr,
            distSL,
            probSI,
            lenOverlapUI,
            numIndelsUI,
            indelEventsUI
         );
      } /*If: have edit distance*/

      errSC =
         (schar)
         get_samEntry(
            &qryStackST,
            &buffHeapStr,
            &lenBuffUL,
            samFILE
         );
   } /*Loop: get edit distances*/

   /*****************************************************\
   * Main Sec03 Sub04:
   *   - print out file errors
   \*****************************************************/

   if(errSC == 64)
   { /*If: had an error*/
      fprintf(
         stderr,
         "MEMORY ERROR reading %lu in -sam %s\n",
         entryUL,
         samFileStr
      );

      goto err_main_sec04_sub02;
   } /*If: had an error*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec04:
   ^   - clean up
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   errSC = 0;
   goto cleanUp_main_sec04;

   err_main_sec04_sub02:;
   errSC = -1;
   goto cleanUp_main_sec04;

   cleanUp_main_sec04:;

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

   freeStack_samEntry(&refStackST);
   freeStack_samEntry(&qryStackST);

   free(buffHeapStr);
   buffHeapStr = 0;

   exit(0); 
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
