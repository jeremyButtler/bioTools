/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' mainClustSam SOF: Start Of File
'   - clusters reads in a sam file
'   o header:
'     - included libraries and default settings
'   o fun01: pversion_mainClustSam
'     - prints version number for clustSam
'   o fun02: phelp_mainClustSam
'     - prints help message for clustSam
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*-------------------------------------------------------\
| Header:
|   - included libraries and default settings
\-------------------------------------------------------*/

#ifdef PLAN9
   #include <u.h>
   #include <libc.h>
#else
   #include <stdlib.h>
#endif

#include <stdio.h>

#include "../genLib/charCp.h"
#include "../genLib/samEntry.h"

#include "../genLib/alnSet.h"

#include "samClust.h"

/*.h files only (no .c files)*/
#include "../genLib/dataTypeShortHand.h"

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\
! Hidden libraries:
!   - .c  #include "../genLib/base10str.h"
!   - .c  #include "../genLib/numToStr.h"
!   - .c  #include "../genLib/ulCp.h"
!   - .c  #include "../genLib/seqST.h"
!   - .c  #include "../genLib/edDist.h"
!   - .c  #include "../genAln/indexToCoord.h"
!   - .c  #include "../genLib/dirMatrix.h"
!   - .c  #include "../genLib/needle.h"
!   - .h  #include "../genLib/ntTo5Bit.h"
!   - .h  #include "../genLib/genMath.h" only .h min macro
!   - .h  #include "../genAln/alnDefs.h"
\%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

#define def_maxPercDist_mainClustSam 10
   /*maximum (edit distance) / (errors) before treating
   `  as separate clusters
   */
#define def_maxPercProb_mainClustSam 0.01f
   /*maximum probability score for cluster to not be
   `  do to error
   */
#define def_minDepth_mainClustSam 20 /*min read depth*/

#define def_errRate_mainClustSam 0.023f

#define def_year_mainClustSam 2024
#define def_month_mainClustSam 9
#define def_day_mainClustSam 17

/*-------------------------------------------------------\
| Fun01: pversion_mainClustSam
|   - prints version number for clustSam
| Input:
|   - outFILE:
|     o file to print version to
| Output:
|   - Prints:
|     o version number to outFILE
\-------------------------------------------------------*/
void
pversion_mainClustSam(
   void *outFILE
){
   fprintf(
      (FILE *) outFILE,
      "clustSam version: %i-%02i-%02i\n",
      def_year_mainClustSam,
      def_month_mainClustSam,
      def_day_mainClustSam
   );
} /*pversion_mainClustSam*/

/*-------------------------------------------------------\
| Fun02: phelp_mainClustSam
|   - prints help message for clustSam
| Input:
|   - outFILE:
|     o file to print help message
| Output:
|   - Prints:
|     o help message to outFILE
\-------------------------------------------------------*/
void
phelp_mainClustSam(
   void *outFILE
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun02 TOC:
   '   - prints help message for clustSam
   '   o fun02 sec01:
   '     - print usage block
   '   o fun02 sec01:
   '     - print input block
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun02 Sec01:
   ^    - print usage block
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   fprintf(
      (FILE *) outFILE,
      "clustSam -sam reads.sam -prefix out\n"
   );

   fprintf(
      (FILE *) outFILE,
      "  - clusters reads by similarity\n"
   );

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun02 Sec02:
   ^   - print input block
   ^   o fun02 sec02 sub01:
   ^     - input header
   ^   o fun02 sec02 sub02:
   ^     - input file
   ^   o fun02 sec02 sub03:
   ^     - prefix for output files
   ^   o fun02 sec02 sub04:
   ^     - minimum read depth
   ^   o fun02 sec02 sub05:
   ^     - error rate
   ^   o fun02 sec02 sub06:
   ^     - min edit distance
   ^   o fun02 sec02 sub07:
   ^     - probability + reference
   ^   o fun02 sec02 sub08:
   ^     - help and version
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun02 Sec02 Sub01:
   *   - input header
   \*****************************************************/

   fprintf(
      (FILE *) outFILE,
      "Input:\n"
   );

   /*****************************************************\
   * Fun02 Sec02 Sub02:
   *   - input file
   \*****************************************************/

   fprintf(
      (FILE *) outFILE,
      "  -sam reads.sam: [Required]\n"
   );

   fprintf(
      (FILE *) outFILE,
      "    o sam file with reads to cluster\n"
   );

   /*****************************************************\
   * Fun02 Sec02 Sub03:
   *   - prefix for output files
   \*****************************************************/

   fprintf(
      (FILE *) outFILE,
      "  -prefix out: [Optional; out]\n"
   );

   fprintf(
      (FILE *) outFILE,
      "    o name for output sam files\n"
   );

   /*****************************************************\
   * Fun02 Sec02 Sub04:
   *   - minimum read depth
   \*****************************************************/

   fprintf(
      (FILE *) outFILE,
      "  -depth %i: [Optional; %i]\n",
      def_minDepth_mainClustSam,
      def_minDepth_mainClustSam
   );

   fprintf(
      (FILE *) outFILE,
      "    o minimum read depth to keep a cluster\n"
   );

   /*****************************************************\
   * Fun02 Sec02 Sub05:
   *   - error rate
   \*****************************************************/

   fprintf(
      (FILE *) outFILE,
      "  -err %0.4f: [Optional; %0.4f]\n",
      def_errRate_mainClustSam,
      def_errRate_mainClustSam
   );

   fprintf(
      (FILE *) outFILE,
      "    o expected error rate for SNPs and large\n"
   );

   fprintf(
      (FILE *) outFILE,
      "    o indels in reads\n"
   );

   /*****************************************************\
   * Fun02 Sec02 Sub06:
   *   - min edit distance
   \*****************************************************/

   fprintf(
      (FILE *) outFILE,
      "  -dist %i: [Optional; %i = %i%% of errors]\n",
      def_maxPercDist_mainClustSam,
      def_maxPercDist_mainClustSam,
      def_maxPercDist_mainClustSam
   );

   fprintf(
      (FILE *) outFILE,
      "    o maximum (edit distance / error rate) to\n"
   );

   fprintf(
      (FILE *) outFILE,
      "      keep reads in same cluster\n"
   );

   fprintf(
      (FILE *) outFILE,
      "    o input is whole number (no decimals)\n"
   );

   /*****************************************************\
   * Fun02 Sec02 Sub07:
   *   - probability + reference
   \*****************************************************/

   fprintf(
      (FILE *) outFILE,
      "  -prob %f: [Optional; %f = %0.2f%%\n",
      def_maxPercProb_mainClustSam,
      def_maxPercProb_mainClustSam,
      def_maxPercProb_mainClustSam * 100
   );

   fprintf(
      (FILE *) outFILE,
      "    o maximum chance of cluster being from\n"
   );

   fprintf(
      (FILE *) outFILE,
      "      errors allowed\n"
   );

   fprintf(
      (FILE *) outFILE,
      "        - (n choose a) * (m choose b) * e^[a*b]\n"
   );

   fprintf(
      (FILE *) outFILE,
      "    o stat is from:\n"
   );

   fprintf(
      (FILE *) outFILE,
      "      Roland Faure, Dominique Lavenier, and \n"
   );

   fprintf(
      (FILE *) outFILE,
      "      Jean-Francios Flot; HairSplittler:\n"
   );

   fprintf(
      (FILE *) outFILE,
      "      haplotype assembly from long, nosiy reads;\n"
   );

   fprintf(
      (FILE *) outFILE,
      "      preprint; 2024\n");

   fprintf(
      (FILE *) outFILE,
      "      https://doi.org/10.1101/2024.02/13.580067\n"
   );

   /*****************************************************\
   * Fun02 Sec02 Sub08:
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
      "  - Prints: clustered reads to separate sam files\n"
   );

   fprintf(
      (FILE *) outFILE,
      "    o prefix-ref.sam for mapped reference\n"
   );

   fprintf(
      (FILE *) outFILE,
      "    o prefix-ref-start-end.sam for coordinates\n"
   );

   fprintf(
      (FILE *) outFILE,
      "    o prefix-ref-start-end-cluster.sam for\n"
   );

   fprintf(
      (FILE *) outFILE,
      "      each cluster (cluster is cluster number)\n"
   );
} /*phelp_mainClustSam*/

/*-------------------------------------------------------\
| Fun03: input_mainClustSam
|   - gets input for clustSam
| Input:
|   - numArgsSI:
|     o number of arguments user input
|   - argAryStr:
|     o pionter to c-string array with user input
|   - samFileStrPtr:
|     o pointer to c-string to point to input sam file
|   - prefStrPtr:
|     o pointer to c-string to point to input prefix
|   - minDepthUI:
|     o pointer to unsigned int to hold min read depth
|   - errFPtr:
|     o pointer to float to hold read error rate
|   - maxDistUIPtr:
|     o pointer to unsigned int to hold max percent
|       difference
|   - maxProbFPtr:
|     o pointer to float to hold maximum probable chance
|       of clust being due to error
| Output:
|   - Prints:
|     o help message to outFILE
|   - Returns:
|     o 0 for no errors
|     o 1 if printed help message or version number
|     o 2 if had error
\-------------------------------------------------------*/
signed char
input_mainClustSam(
   signed int numArgsSI,
   char *argAryStr[],
   signed char **samFileStrPtr,
   signed char **prefStrPtr,
   unsigned int *minDepthUIPtr,
   float *errFPtr,
   unsigned int *maxDistUIPtr,
   float *maxProbFPtr
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun03 TOC:
   '   - gets input for clustSam
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
   ^   - variable declaration
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   sint siArg = 1;
   schar errSC = 0;
   schar *tmpStr = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec02:
   ^   - check if have input
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(numArgsSI <= 1)
   { /*If: no input*/
      phelp_mainClustSam(stdout);
      goto phelp_fun03_sec04:;
   } /*If: no input*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec03:
   ^   - get user input
   ^   o fun03 sec03 sub01:
   ^     - file IO + start loop
   ^   o fun03 sec03 sub02:
   ^     - min depth
   ^   o fun03 sec03 sub03:
   ^     - error rate
   ^   o fun03 sec03 sub04:
   ^     - edit distance (ratio difference to error)
   ^   o fun03 sec03 sub05:
   ^     - max probability due to error percent
   ^   o fun03 sec03 sub06:
   ^     - help message
   ^   o fun03 sec03 sub07:
   ^     - version number
   ^   o fun03 sec03 sub08:
   ^     - invalid input
   ^   o fun03 sec03 sub09:
   ^     - move to next argument
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun03 Sec03 Sub01:
   *   - file IO + start loop
   \*****************************************************/

   while(siArg < numArgsSI)
   { /*Loop: get user input*/
      if(
         ! eql_charCp(
            (schar *) "-sam",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*If: sam file to cluster was input*/
         ++siArg;
         *samFileStrPtr = argAryStr[siArg];
      } /*If: sam file to cluster was input*/

      else if(
         ! eql_charCp(
            (schar *) "-prefix",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else If: output prefix*/
         ++siArg;
         *prefStrPtr = argAryStr[siArg];
      } /*Else If: output prefix*/

      /**************************************************\
      * Fun03 Sec03 Sub02:
      *   - min depth
      \**************************************************/

      else if(
         ! eql_charCp(
            (schar *) "-depth",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else If: maximum edit distance*/
         ++siArg;
         tmpStr = (schar *) argAryStr[siArg];

         tmpStr +=
            strToUI_base10str(
               argAryStr[siArg],
               minDepthUIPtr
            );

         if(*tmpStr != '\0')
         { /*If: invalid input*/
            else
               fprintf(
                  stderr,
                  "-depth %s non-numeric, or to large\n",
                  argAryStr[siArg]
               );

            goto err_fun03_sec04;
         } /*If: invalid input*/
      } /*Else If: maximum edit distance*/

      /**************************************************\
      * Fun03 Sec03 Sub03:
      *   - error rate
      \**************************************************/

      else if(
         ! eql_charCp(
            (schar *) "-err",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else If: error rate*/
         ++siArg;
         *errFPtr = atof(argAryStr[siArg]);

         if(*errFPtr == 0)
         { /*If: invalid input*/
            fprintf(
               stderr,
               "-err %s is 0, non-numeric, or to large\n",
               argAryStr[siArg]
            );

            goto err_fun03_sec04;
         } /*If: invalid input*/
      } /*Else If: err rate*/

      /**************************************************\
      * Fun03 Sec03 Sub04:
      *   - edit distance (ratio difference to error)
      \**************************************************/

      else if(
         ! eql_charCp(
            (schar *) "-dist",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else If: maximum edit distance*/
         ++siArg;
         tmpStr = (schar *) argAryStr[siArg];

         tmpStr +=
            strToUI_base10str(
               argAryStr[siArg],
               maxDistUIPtr
            );

         if(*tmpStr != '\0')
         { /*If: invalid input*/
            if(*tmpStr == '.')
               fprintf(
                  stderr,
                  "-dist %s must be a whole number\n",
                  argAryStr[siArg]
               );

            else
               fprintf(
                  stderr,
                  "-dist %s non-numeric, or to large\n",
                  argAryStr[siArg]
               );

            goto err_fun03_sec04;
         } /*If: invalid input*/
      } /*Else If: maximum edit distance*/

      /**************************************************\
      * Fun03 Sec03 Sub05:
      *   - max probability due to error percent
      \**************************************************/

      else if(
         ! eql_charCp(
            (schar *) "-prob",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else If: maximum probability*/
         ++siArg;
         *errFPtr = atof(argAryStr[siArg]);

         if(*errFPtr == 0)
         { /*If: invalid input*/
            fprintf(
               stderr,
               "-prob %s is 0, non-numeric, or to large\n",
               argAryStr[siArg]
            );

            goto err_fun03_sec04;
         } /*If: invalid input*/
      } /*Else If: maximum probability*/

      /**************************************************\
      * Fun03 Sec03 Sub06:
      *   - help message
      \**************************************************/

      else if(
         ! eql_charCp(
            (schar *) "-h",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else If: help message*/
         phelp_mainClustSam(stdout);
         goto phelp_fun03_sec04;
      } /*Else If: help message*/

      else if(
         ! eql_charCp(
            (schar *) "--h",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else If: help message*/
         phelp_mainClustSam(stdout);
         goto phelp_fun03_sec04;
      } /*Else If: help message*/

      else if(
         ! eql_charCp(
            (schar *) "help",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else If: help message*/
         phelp_mainClustSam(stdout);
         goto phelp_fun03_sec04;
      } /*Else If: help message*/

      else if(
         ! eql_charCp(
            (schar *) "-help",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else If: help message*/
         phelp_mainClustSam(stdout);
         goto phelp_fun03_sec04;
      } /*Else If: help message*/

      else if(
         ! eql_charCp(
            (schar *) "--help",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else If: help message*/
         phelp_mainClustSam(stdout);
         goto phelp_fun03_sec04;
      } /*Else If: help message*/

      /**************************************************\
      * Fun03 Sec03 Sub07:
      *   - version number
      \**************************************************/

      else if(
         ! eql_charCp(
            (schar *) "-v",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else If: version number*/
         pversion_mainClustSam(stdout);
         goto pversion_fun03_sec04;
      } /*Else If: version number*/

      else if(
         ! eql_charCp(
            (schar *) "--v",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else If: version number*/
         pversion_mainClustSam(stdout);
         goto pversion_fun03_sec04;
      } /*Else If: version number*/

      else if(
         ! eql_charCp(
            (schar *) "version",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else If: version number*/
         pversion_mainClustSam(stdout);
         goto pversion_fun03_sec04;
      } /*Else If: version number*/

      else if(
         ! eql_charCp(
            (schar *) "-version",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else If: version number*/
         pversion_mainClustSam(stdout);
         goto pversion_fun03_sec04;
      } /*Else If: version number*/

      else if(
         ! eql_charCp(
            (schar *) "--version",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else If: version number*/
         pversion_mainClustSam(stdout);
         goto pversion_fun03_sec04;
      } /*Else If: version number*/

      /**************************************************\
      * Fun03 Sec03 Sub08:
      *   - invalid input
      \**************************************************/

      else
      { /*Else: invalid input*/
         fprintf(
            stderr,
            "%s is not recongnized\n",
            argAryStr[siArg]
         );

         goto err_fun03_sec04;
      } /*Else: invalid input*/

      /**************************************************\
      * Fun03 Sec03 Sub09:
      *   - move to next argument
      \**************************************************/

      ++siArg;
   } /*Loop: get user input*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec04:
   ^   - return result
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   phelp_fun03_sec04:;
   pversion_fun03_sec04:;
   errSC = 1;
   goto ret_fun03_sec04;

   err_fun03_sec04:;
   errSC = 2;
   goto ret_fun03_sec04;

   ret_fun03_sec04:;
   return errSC;
} /*pinput_mainClustSam*/

/*-------------------------------------------------------\
| Main:
|   - driver function for clustSam
| Input:
|   - numArgsSI:
|     o number arguments user input (C handels)
|   - argAryStr:
|     o pointer to c-string array of user input; C handels
| Output:
|   - Prints:
|     o binned reads to sam files
\-------------------------------------------------------*/
int
main(
   int numArgsSI,
   char *argAryStr[]
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Main TOC:
   '   o main sec01:
   '     - variable declarations
   '   o main sec02:
   '     - initialze, get input, setup, and check input
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   schar *samFileStr = 0;
   schar *prefixStr = (schar *) "out";
   uint maxDistUI = def_maxPerDist_mainClustSam;
   uint minDepthUI = def_minDepth_mainClustSam;
   float maxProbF = def_maxPercProb_mainClustSam;
   float errRateF = def_errRate_mainClustSam;

   schar errSC = 0;
   unsigned long entryUL = 0;

   stuct samEntry samStackST;
   schar *buffHeapStr = 0;
   ulong lenBuffUL = 0;

   struct profile_clustSam profStackST;

   FILE *samFILE = 0;
   FILE *outFILE = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec02:
   ^   - initialze, get input, setup, and check input
   ^   o main sec02 sub01:
   ^     - initialize variables
   ^   o main sec02 sub02:
   ^     - get input
   ^   o main sec02 sub03:
   ^     - set up
   ^   o main sec02 sub04:
   ^     - open if sam file (also check if exists)
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Main Sec02 Sub01:
   *   - initialize variables
   \*****************************************************/

   init_samEntry(&samStackST);
   init_profile_clustSam(&profStackST);

   /*****************************************************\
   * Main Sec02 Sub02:
   *   - get input
   \*****************************************************/

   errSC =
      input_mainSamClust(
         numArgsSI,
         argAryStr,
         &samFileStr,
         &prefixStr,
         &minDepthUI,
         &errRateF,
         &maxDistUI,
         &maxProbF
      );

   if(errSC)
   { /*If: input error*/
      --errSC;/*converts help/vesion message to no error*/
      got ret_main_sec0x_sub03;
   } /*If: input error*/

   /*****************************************************\
   * Main Sec02 Sub03:
   *   - set up
   \*****************************************************/

   errSC = setup_samEntry(&samStackST);

   if(errSC)
   { /*If: memory error*/
      fprintf(
         stderr,
         "memory error setting up samEntry struct\n"
      );

      goto err_main_sec0x_sub02;
   } /*If: memory error*/

   /*****************************************************\
   * Main Sec02 Sub04:
   *   - open if sam file (also check if exists)
   \*****************************************************/

   samFILE =
      fopen(
         (char *) samFileStr,
         "r"
      );

   if(! samFILE)
   { /*If: could not open sam file*/
      if(! samFileStr)
         fprintf(
            stderr,
            "no sam file input with -sam\n"
         );

      else
         fprintf(
            stderr,
            "could not open -sam %s\n",
            samFileStr
         );

      goto err_main_sec0x_sub02;
   } /*If: could not open sam file*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec03:
   ^   - bin reads
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*for binning; after find best read*/
   errSC =
      setupReadAry_profile_samClust(
         &profStackST,
         entryUL
      );

   if(errSC)
   { /*If: memory error*/
      fprintf(
         stderr,
         "memory error setting up read hit array\n"
      );

      goto err_main_sec0x_sub02;
   } /*If: memory error*/

   errSC = 0;
   goto cleanUp_main_sec0x_sub03;

   err_main_sec0x_sub02:;
   errSC = 1;
   goto cleanUp_main_sec0x_sub03;

   cleanUp_main_sec0x_sub03:;
   freeStack_profile_clustSam(&profStackST);
   freeStack_samEntry(&samStackST);

   if(buffHeapStr)
      free(buffHeapStr);

   buffHeapStr = 0;

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
} /*main*/
