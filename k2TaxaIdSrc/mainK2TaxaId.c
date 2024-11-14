/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' mainK2TaxaId SOF: Start Of File
'   - driver function for printing kraken read ids
'   o header:
'     - included libaries
'   o fun01: pversion_mainK2TaxaId
'     - prints version number for k2TaxaId
'   o fun02: phelp_mainK2TaxaId
'     - prints help message for k2TaxaId
'   o fun03: input_maink2TaxaId
'     - gets user input from array
'   o main:
'     - driver fuction to get taxa ids from kraken output
'   o license:
'     - licensing for this code (public domain / mit)
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*-------------------------------------------------------\
| Header:
|   - included libaries
\-------------------------------------------------------*/

#ifdef PLAN9
   #include <u.h>
   #include <libc.h>
#else
   #include <stdlib.h>
#endif

#include <stdio.h>

#include "../genLib/base10str.h"
#include "../genLib/numToStr.h"
#include "../genLib/ulCp.h"

#include "k2TaxaId.h"

/*no .c files*/
#include "../genLib/dataTypeShortHand.h"
#include "../bioTools.h" /*version*/

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\
! Hidden libraries:
!   o .c  #include "../genLib/genMath.h"
!   o .c  #include "../genLib/strAry.h"
\%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

signed char *glob_prefStr = (schar *) "Hagrid";
signed char *glob_startLevStr = (schar *) "G";
signed char *glob_endLevStr = (schar *) "S";

#define def_minDepth_mainK2TaxaId 10 /*at least 10 reads*/
#define def_minPercDepth_mainK2TaxaId 0.0f

#define def_unclassifed_mainK2TaxaId 0
   /*1: print unclassified reads*/
#define def_mergeRoot_mainK2TaxaId 1
   /*1: do not merge lower (root) taxa into higher (tip)*/
#define def_tip_maink2TaxaId 1
   /*1: merge tips out of user input range*/

/*-------------------------------------------------------\
| Fun01: pversion_mainK2TaxaId
|   - prints version number for k2TaxaId
| Input
|   - outFILE
|     o pointer to FILE to print version to
| Output:
|   - Prints:
|     o version number to outFILE
\-------------------------------------------------------*/
void
pversion_mainK2TaxaId(
   void *outFILE
){
   fprintf(
      (FILE *) outFILE,
      "k2TaxaId from bioTools version: %i-%02i-%02i\n",
      def_year_bioTools,
      def_month_bioTools,
      def_day_bioTools
   );
} /*pversion_mainK2TaxaId*/

/*-------------------------------------------------------\
| Fun02: phelp_mainK2TaxaId
|   - prints help message for k2TaxaId
| Input
|   - outFILE
|     o pointer to FILE to print help message to
| Output:
|   - Prints:
|     o help message to outFILE
\-------------------------------------------------------*/
void
phelp_mainK2TaxaId(
   void *outFILE
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun02 TOC:
   '   - prints help message for k2TaxaId
   '   o fun02 sec01:
   '     - print usage entry
   '   o fun02 sec02:
   '     - print input
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun02 Sec01:
   ^   - print usage entry
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

    fprintf(
       (FILE *) outFILE,
       "k2TaxaId -report k2-report.tsv -id k2-out.tsv\n"
    );

    fprintf(
       (FILE *) outFILE,
       "  - extracts read ids of target taxa level and\n"
    );

    fprintf(
       (FILE *) outFILE,
       "    prints ids to individual files\n"
    );

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun02 Sec02:
   ^   - print input
   ^   o fun02 sec02 sub01:
   ^     - input block header
   ^   o fun02 sec02 sub02:
   ^     - file IO
   ^   o fun02 sec02 sub03:
   ^     - taxa print options
   ^   o fun02 sec02 sub04:
   ^     - taxa filtering options
   ^   o fun02 sec02 sub05:
   ^     - misc options
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun02 Sec02 Sub01:
   *   - input block header
   \*****************************************************/

   fprintf(
      (FILE *) outFILE,
      "Input:\n"
   );

   /*****************************************************\
   * Fun02 Sec02 Sub02:
   *   - file IO
   \*****************************************************/

   fprintf(
      (FILE *) outFILE,
      "  File IO:\n"
   );

   fprintf(
      (FILE *) outFILE,
      "    -report k2-report.tsv: [Required]\n"
   );

   fprintf(
      (FILE *) outFILE,
      "      o tsv report made by kraken2\n"
   );

   fprintf(
     (FILE *) outFILE,
     "      o \"kraken2 --report k2-report.tsv ...\"\n"
   );


   fprintf(
      (FILE *) outFILE,
      "    -id k2-out.tsv: [Required]\n"
   );

   fprintf(
    (FILE *) outFILE,
    "      o tsv with ids & assignments made by kraken2\n"
   );

   fprintf(
      (FILE *) outFILE,
      "      o \"kraken2 --out k2-out.tsv ...\"\n"
   );


   fprintf(
      (FILE *) outFILE,
      "    -prefix %s: [Optional; %s]\n",
      glob_prefStr,
      glob_prefStr
   );

   fprintf(
      (FILE *) outFILE,
      "      o prefix for output files\n"
   );

   /*****************************************************\
   * Fun02 Sec02 Sub03:
   *   - taxa print options
   \*****************************************************/

   fprintf(
      (FILE *) outFILE,
      "  Taxa Printing:\n"
   );


   if(def_unclassifed_mainK2TaxaId)
      fprintf(
         (FILE *) outFILE,
         "    -un-class: [Optional; Yes]\n"
      );
   else
      fprintf(
         (FILE *) outFILE,
         "    -un-class: [Optional; No]\n"
      );

   fprintf(
      (FILE *) outFILE,
      "      o print unclassified reads\n"
   );

   fprintf(
      (FILE *) outFILE,
      "      o turn off with \"-no-un-class\"\n"
   );


   if(def_mergeRoot_mainK2TaxaId)
      fprintf(
         (FILE *) outFILE,
         "    -merge-root: [Optional; Yes]\n"
      );
   else
      fprintf(
         (FILE *) outFILE,
         "    -merge-root: [Optional; No]\n"
      );

   fprintf(
     (FILE *) outFILE,
     "      o merge lower taxa (nearer root) with taxa\n"
   );

   fprintf(
     (FILE *) outFILE,
     "        closer to tip of tree\n"
   );

   fprintf(
     (FILE *) outFILE,
     "      o turn off with \"-no-merge-root\"\n"
   );


   if(def_tip_maink2TaxaId)
      fprintf(
         (FILE *) outFILE,
         "    -merge-tip: [Optional; Yes]\n"
      );
   else
      fprintf(
         (FILE *) outFILE,
         "    -merge-tip: [Optional; No]\n"
      );

   fprintf(
     (FILE *) outFILE,
     "      o merge tip/near tip taxa ids not in range\n"
   );

   fprintf(
     (FILE *) outFILE,
     "      o turn off with \"-no-merge-tip\"\n"
   );
   /*****************************************************\
   * Fun02 Sec02 Sub04:
   *   - taxa filtering options
   \*****************************************************/

   fprintf(
      (FILE *) outFILE,
      "  Taxa Filtering:\n"
   );


   fprintf(
      (FILE *) outFILE,
      "    -start %s: [Optional; %s]\n",
      glob_startLevStr,
      glob_startLevStr
   );

   fprintf(
      (FILE *) outFILE,
      "      o first taxonomic level to extract at\n"
   );

   fprintf(
      (FILE *) outFILE,
      "      o is a letter and (optional) a number\n"
   );

   fprintf(
     (FILE *) outFILE,
     "      o D(0-7),P(0-7),C(0-7),F(0-7),G(0-7),S(0-7)\n"
   );


   fprintf(
      (FILE *) outFILE,
      "    -end %s: [Optional; %s]\n",
      glob_endLevStr,
      glob_endLevStr
   );

   fprintf(
      (FILE *) outFILE,
      "      o last taxonomic level to extract at\n"
   );



   fprintf(
      (FILE *) outFILE,
      "    -depth %u: [Optional; %u]\n",
      def_minDepth_mainK2TaxaId,
      def_minDepth_mainK2TaxaId
   );

   fprintf(
      (FILE *) outFILE,
      "      o minimum read depth to print ids for taxa\n"
   );


   fprintf(
      (FILE *) outFILE,
      "    -perc-depth %0.04f: [Optional; %0.04f]\n",
      def_minPercDepth_mainK2TaxaId,
      def_minPercDepth_mainK2TaxaId
   );

   fprintf(
      (FILE *) outFILE,
      "      o minimum percent read depth to print ids\n"
   );

   /*****************************************************\
   * Fun02 Sec02 Sub05:
   *   - misc options
   \*****************************************************/

   fprintf(
      (FILE *) outFILE,
      "  Misc:\n"
   );


   fprintf(
      (FILE *) outFILE,
      "    -h: print this help message and exit\n"
   );

   fprintf(
      (FILE *) outFILE,
      "    -v: print version number and exit\n"
   );

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun02 Sec03:
   ^   - print output
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   fprintf(
      (FILE *) outFILE,
      "Output:\n"
   );

   fprintf(
     (FILE *) outFILE,
     "  - prints read ids to prefix-taxa_code-taxon.ids\n"
   );
} /*phelp_mainK2TaxaId*/

/*-------------------------------------------------------\
| Fun03: input_maink2TaxaId
|   - gets user input from array
| Input:
|   - argAryStr:
|     o c-string array with user input
|   - numArgsSI:
|     o number of input arguments
|   - reportFileStrPtr:
|     o pointer to c-string to point to kraken report
|   - idFileStrPtr:
|     o pointer to c-string to point to kraken output
|   - prefixStrPtr:
|     o pointer to c-string to point to prefix
|   - startSSPtr:
|     o pointer to unsigned short to have starting taxa
|   - endStrPtr:
|     o pointer to unsigned short to have ending taxa
|   - depthULPtr:
|     o pointer to unsigned long to hold min read depth
|   - percDepthFPtr:
|     o pointer to float to hold min percent read depth
|   - pUnclassBlPtr:
|     o pointer to signed char to be set to
|       - 1: if printing unclassified reads
|       - 0: if not printing unclassified reads
|   - mergeRootBlPtr:
|     o pointer to signed char to be set to
|       - 1: if not merging taxa
|       - 0: if am merging taxa (when possible)
|   - mergeTipBlPtr:
|     o pointer to signed char to be set to
|       - 1: if not merging out of range tip taxa
|       - 0: if not merging
| Output:
|   - Modifies:
|     o all input (except aryAryStr and numArgsSI) to have
|       user input
|   - Prints:
|     o help message to stdout if requested; then return
|     o version number to stdout if requested; then return
|     o error to stderr; then return
|   - Returns:
|     o 0 for no errors
|     o 1 if printed help message or version number
|     o 2 if had error
\-------------------------------------------------------*/
signed char
input_mainK2TaxaId(
   char *argAryStr[],
   int numArgsSI,
   signed char **reportFileStrPtr,
   signed char **idFileStrPtr,
   signed char **prefixStrPtr,
   signed short *startSSPtr,
   signed short *endSSPtr,
   unsigned long *depthULPtr,
   float *percDepthFPtr,
   signed char *pUnclassBlPtr,
   signed char *mergeRootBlPtr,
   signed char *mergeTipBlPtr
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun03 TOC:
   '   - gets user input from array
   '   o fun03 sec01:
   '     - variable declarations
   '   o fun03 sec02:
   '     - check if have input
   '   o fun03 sec03:
   '     - get user input
   '   o fun03 sec04:
   '     - return result
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   sint siArg = 1;
   schar *tmpStr = 0;
   schar errSC = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec02:
   ^   - check if have input
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(numArgsSI < 1)
   { /*If: nothing input*/
      phelp_mainK2TaxaId(stdout);
      goto phelp_fun03_sec04;
   } /*If: nothing input*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec03:
   ^   - get user input
   ^   o fun03 sec03 sub01:
   ^     - file IO
   ^   o fun03 sec03 sub02:
   ^     - taxa printing options
   ^   o fun03 sec03 sub03:
   ^     - taxa filtering options
   ^   o fun03 sec03 sub0w:
   ^     - help message requests
   ^   o fun03 sec03 sub0x:
   ^     - version number requests
   ^   o fun03 sec03 sub0y:
   ^     - unknown input
   ^   o fun03 sec03 sub0z:
   ^     - move to next argument
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun03 Sec03 Sub01:
   *   - file IO
   \*****************************************************/

   while(siArg < numArgsSI)
   { /*Loop: get user input*/

      if(
         ! eqlNull_ulCp(
            (schar *) "-report",
            (schar *) argAryStr[siArg]
         )
      ){ /*If: kraken report input*/
         ++siArg;
         *reportFileStrPtr = (schar *) argAryStr[siArg];
      } /*If: kraken report input*/

      else if(
         ! eqlNull_ulCp(
            (schar *) "-id",
            (schar *) argAryStr[siArg]
         )
      ){ /*If: kraken read assignment (out) file*/
         ++siArg;
         *idFileStrPtr = (schar *) argAryStr[siArg];
      } /*If: kraken read assignment (out) file*/

      else if(
         ! eqlNull_ulCp(
            (schar *) "-prefix",
            (schar *) argAryStr[siArg]
         )
      ){ /*If: prefix for output files*/
         ++siArg;
         *prefixStrPtr = (schar *) argAryStr[siArg];
      } /*If: prefix for output files*/

      /**************************************************\
      * Fun03 Sec03 Sub02:
      *   - taxa printing options
      \**************************************************/

      else if(
         ! eqlNull_ulCp(
            (schar *) "-un-class",
            (schar *) argAryStr[siArg]
         )
      ) *pUnclassBlPtr = 1;

      else if(
         ! eqlNull_ulCp(
            (schar *) "-no-un-class",
            (schar *) argAryStr[siArg]
         )
      ) *pUnclassBlPtr = 0;

      else if(
         ! eqlNull_ulCp(
            (schar *) "-merge-root",
            (schar *) argAryStr[siArg]
         )
      ) *mergeRootBlPtr = 1;

      else if(
         ! eqlNull_ulCp(
            (schar *) "-no-merge-root",
            (schar *) argAryStr[siArg]
         )
      ) *mergeRootBlPtr = 0;

      else if(
         ! eqlNull_ulCp(
            (schar *) "-merge-tip",
            (schar *) argAryStr[siArg]
         )
      ) *mergeTipBlPtr = 1;

      else if(
         ! eqlNull_ulCp(
            (schar *) "-no-merge-tip",
            (schar *) argAryStr[siArg]
         )
      ) *mergeTipBlPtr = 0;

      /**************************************************\
      * Fun03 Sec03 Sub03:
      *   - taxa filtering options
      \**************************************************/

      else if(
         ! eqlNull_ulCp(
            (schar *) "-start",
            (schar *) argAryStr[siArg]
         )
      ){ /*Else If: frist taxa to start print*/
         ++siArg;

         *startSSPtr =
            getLevel_k2TaxaId((schar *) argAryStr[siArg]);

         if(*startSSPtr == 0)
         { /*If: unregonized taxa id*/
            fprintf(
               stderr,
               "-start %s is not a regonized taxa id\n",
               argAryStr[siArg]
            );

            fprintf(
               stderr,
               "  recognized: D, P, C, O, F, G, and S\n"
            );

            goto err_fun03_sec04;
         } /*If: unregonized taxa id*/
      } /*Else If: first taxa to print*/

      else if(
         ! eqlNull_ulCp(
            (schar *) "-end",
            (schar *) argAryStr[siArg]
         )
      ){ /*Else If: last taxa to print*/
         ++siArg;

         *endSSPtr =
            getLevel_k2TaxaId((schar *) argAryStr[siArg]);

         if(*startSSPtr == 0)
         { /*If: unregonized taxa id*/
            fprintf(
               stderr,
               "-end %s is not a regonized taxa id\n",
               argAryStr[siArg]
            );

            fprintf(
               stderr,
               "  recognized: D, P, C, O, F, G, and S\n"
            );

            goto err_fun03_sec04;
         } /*If: unregonized taxa id*/
      } /*Else If: last taxa to print*/


      else if(
         ! eqlNull_ulCp(
            (schar *) "-depth",
            (schar *) argAryStr[siArg]
         )
      ){ /*Else If: min read depth*/
         ++siArg;
         tmpStr = (schar *) argAryStr[siArg];

         tmpStr +=
            strToUL_base10str(
               tmpStr,
               depthULPtr
            );

         if(*tmpStr != '\0')
         { /*If: non-numeric*/
            fprintf(
               stderr,
               "-depth %s is to large or non-numeric\n",
               argAryStr[siArg]
            );

            goto err_fun03_sec04;
         } /*If: non-numeric*/
      } /*Else If: min read depth*/


      else if(
         ! eqlNull_ulCp(
            (schar *) "-perc-depth",
            (schar *) argAryStr[siArg]
         )
      ){ /*Else If: min percent read depth*/
         ++siArg;
         *percDepthFPtr = atof(argAryStr[siArg]);
      } /*Else If: min percent read depth*/

      /**************************************************\
      * Fun03 Sec03 Sub0w:
      *   - help message requests
      \**************************************************/

      else if(
         ! eqlNull_ulCp(
            (schar *) "-h",
            (schar *) argAryStr[siArg]
         )
      ){ /*Else If: help message requested*/
         phelp_mainK2TaxaId(stdout);
         goto phelp_fun03_sec04;
      } /*Else If: help message requested*/

      else if(
         ! eqlNull_ulCp(
            (schar *) "--h",
            (schar *) argAryStr[siArg]
         )
      ){ /*Else If: help message requested*/
         phelp_mainK2TaxaId(stdout);
         goto phelp_fun03_sec04;
      } /*Else If: help message requested*/

      else if(
         ! eqlNull_ulCp(
            (schar *) "help",
            (schar *) argAryStr[siArg]
         )
      ){ /*Else If: help message requested*/
         phelp_mainK2TaxaId(stdout);
         goto phelp_fun03_sec04;
      } /*Else If: help message requested*/

      else if(
         ! eqlNull_ulCp(
            (schar *) "-help",
            (schar *) argAryStr[siArg]
         )
      ){ /*Else If: help message requested*/
         phelp_mainK2TaxaId(stdout);
         goto phelp_fun03_sec04;
      } /*Else If: help message requested*/

      else if(
         ! eqlNull_ulCp(
            (schar *) "--help",
            (schar *) argAryStr[siArg]
         )
      ){ /*Else If: help message requested*/
         phelp_mainK2TaxaId(stdout);
         goto phelp_fun03_sec04;
      } /*Else If: help message requested*/

      /**************************************************\
      * Fun03 Sec03 Sub0x:
      *   - version number requests
      \**************************************************/

      else if(
         ! eqlNull_ulCp(
            (schar *) "-v",
            (schar *) argAryStr[siArg]
         )
      ){ /*Else If: version number requested*/
         pversion_mainK2TaxaId(stdout);
         goto pversion_fun03_sec04;
      } /*Else If: version number requested*/

      else if(
         ! eqlNull_ulCp(
            (schar *) "--v",
            (schar *) argAryStr[siArg]
         )
      ){ /*Else If: version number requested*/
         pversion_mainK2TaxaId(stdout);
         goto pversion_fun03_sec04;
      } /*Else If: version number requested*/

      else if(
         ! eqlNull_ulCp(
            (schar *) "version",
            (schar *) argAryStr[siArg]
         )
      ){ /*Else If: version number requested*/
         pversion_mainK2TaxaId(stdout);
         goto pversion_fun03_sec04;
      } /*Else If: version number requested*/

      else if(
         ! eqlNull_ulCp(
            (schar *) "-version",
            (schar *) argAryStr[siArg]
         )
      ){ /*Else If: version number requested*/
         pversion_mainK2TaxaId(stdout);
         goto pversion_fun03_sec04;
      } /*Else If: version number requested*/

      else if(
         ! eqlNull_ulCp(
            (schar *) "--version",
            (schar *) argAryStr[siArg]
         )
      ){ /*Else If: version number requested*/
         pversion_mainK2TaxaId(stdout);
         goto pversion_fun03_sec04;
      } /*Else If: version number requested*/

      /**************************************************\
      * Fun03 Sec03 Sub0y:
      *   - unknown input
      \**************************************************/

      else
      { /*Else: no idea what was input*/
         fprintf(
            stderr,
            "%s is not recongnized\n",
            argAryStr[siArg]
         );

         goto err_fun03_sec04;
      } /*Else: no idea what was input*/

      /**************************************************\
      * Fun03 Sec03 Sub0z:
      *   - move to next argument
      \**************************************************/

      ++siArg;
   } /*Loop: get user input*/

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
} /*input_mainK2TaxaId*/

/*-------------------------------------------------------\
| Main:
|   - driver fuction to get taxa ids from kraken output
| Input:
|   - argAryStr:
|     o c-string array with user input
|   - numArgsSI:
|     o number of argumetns in argAryStr
| Output:
|   - Prints:
|     o ids for each taxa to prefix-taxa_code_taxa.id
|   - Returns:
|     o 0 for no errors
|     o 1 for errors
\-------------------------------------------------------*/
int
main(
   int numArgsSI,
   char *argAryStr[]
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Main TOC:
   '   - driver fuction to get taxa ids from kraken output
   '   o main sec01:
   '     - variable declarations
   '   o main sec02:
   '     - get and check input
   '   o main sec03:
   '     - print kraken2 ids
   '   o main sec04:
   '     - read in kraken2 report
   '   o main sec05:
   '     - clean up and return
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   schar *reportFileStr = (schar *) "";
   schar *idFileStr = (schar *) "";
   schar *prefixStr = glob_prefStr;

   sshort startLevSS= getLevel_k2TaxaId(glob_startLevStr);
   sshort endLevSS = getLevel_k2TaxaId(glob_endLevStr);

   ulong depthUL = def_minDepth_mainK2TaxaId;
   float percDepthF = def_minPercDepth_mainK2TaxaId;

   schar pUnclassBl = def_unclassifed_mainK2TaxaId;
   schar mergeRootBl = def_mergeRoot_mainK2TaxaId;
   schar mergeTipBl = def_tip_maink2TaxaId;

   schar errSC = 0; /*for function errors*/
   struct taxa_k2TaxaId *taxaHeapST = 0;

   FILE *inFILE = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec02:
   ^   - get input
   ^   o main sec02 sub01:
   ^     - get user input
   ^   o main sec02 sub02:
   ^     - check if can open id file
   ^   o main sec02 sub03:
   ^     - open report file
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Main Sec02 Sub01:
   *   - get user input
   \*****************************************************/

   errSC =
      input_mainK2TaxaId(
         argAryStr,
         numArgsSI,
         &reportFileStr,
         &idFileStr,
         &prefixStr,
         &startLevSS,
         &endLevSS,
         &depthUL,
         &percDepthF,
         &pUnclassBl,
         &mergeRootBl,
         &mergeTipBl
      ); /*get user input*/

   if(errSC)
   { /*If: error*/
      --errSC;   /*converts help/version to no error*/
      goto ret_main_sec05;
   } /*If: error*/

   /*****************************************************\
   * Main Sec02 Sub02:
   *   - check if can open id file
   \*****************************************************/

   if(
         ! idFileStr
      || *idFileStr == '-'
   ){ /*If: id file from stdin*/
      if(
            ! reportFileStr
         || *reportFileStr == '-'
      ){ /*If: two stdin files*/
         fprintf(
            stderr,
            "-report %s and -id %s from stdin\n",
            reportFileStr,
            idFileStr
         );

         goto err_main_sec05;
      } /*If: two stdin files*/

      /*from stdin, open later*/
   } /*If: report is from stdin*/

   else
   { /*Else: id file is input as file*/
      inFILE =
         fopen(
            (char *) idFileStr,
            "r"
         );

      if(! inFILE)
      { /*If: could not open file*/
         fprintf(
            stderr,
            "unable to open -id %s\n",
            idFileStr
         );

         goto err_main_sec05;
      } /*If: could not open file*/
   } /*Else: id file is input as file*/

   if(inFILE)
      fclose(inFILE);
   inFILE = 0;

   /*****************************************************\
   * Main Sec02 Sub03:
   *   - open report file
   \*****************************************************/

   if(
         ! reportFileStr
      || *reportFileStr == '-'
   ){ /*If: report from stdin*/
      if(
            ! idFileStr
         || *idFileStr == '-'
      ){ /*If: two stdin files*/
         fprintf(
            stderr,
            "-report %s and -id %s from stdin\n",
            reportFileStr,
            idFileStr
         );

         goto err_main_sec05;
      } /*If: two stdin files*/

      inFILE = stdin;
   } /*If: report is from stdin*/

   else
   { /*Else: report input as file*/
      inFILE =
         fopen(
            (char *) reportFileStr,
            "r"
         );

      if(! inFILE)
      { /*If: could not open file*/
         fprintf(
            stderr,
            "unable to open -report %s\n",
            reportFileStr
         );

         goto err_main_sec05;
      } /*If: could not open file*/
   } /*Else: report input as file*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec03:
   ^   - read in kraken2 report
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/
     
   taxaHeapST =
      readReport_k2TaxaId(
         startLevSS,  /*frist taxanonic level to print*/
         endLevSS,    /*last taxonomic level to print*/
         depthUL,     /*min read depth*/
         percDepthF,  /*min percent read depth*/
         mergeRootBl, /*merging root taxa into tip*/
         mergeTipBl,  /*merge tip taxa out of range*/
         &errSC,      /*get errors*/
         inFILE
     );

   if(inFILE != stdin)
      fclose(inFILE);
   inFILE = 0;

   if(! taxaHeapST)
   { /*If: had error*/
      if(errSC == def_noIds_k2TaxaId)
         fprintf(
            stderr,
            "no ids in -report %s\n",
            reportFileStr
         );
      else if(errSC == def_memErr_k2TaxaId)
         fprintf(
            stderr,
            "memory error reading -report %s\n",
            reportFileStr
         );
      else
         fprintf(
            stderr,
            "file error reading -report %s\n",
            reportFileStr
         );

      goto err_main_sec05;
   } /*If: had error*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec04:
   ^   - print kraken2 ids
   ^   o main sec04 sub01:
   ^     - open id file (non-check open)
   ^   o main sec04 sub02:
   ^     - print read ids and clean up
   ^   o main sec04 sub03:
   ^     - check for errors
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Main Sec04 Sub01:
   *   - open id file (non-check open)
   \*****************************************************/

   if(
         ! idFileStr
      || *idFileStr == '-'
   ) inFILE = stdin;

   else
   { /*Else: id file is input as file*/
      inFILE =
         fopen(
            (char *) idFileStr,
            "r"
         );

      if(! inFILE)
      { /*If: could not open file*/
         fprintf(
            stderr,
            "unable to open (non-check) -id %s\n",
            idFileStr
         );

         goto err_main_sec05;
      } /*If: could not open file*/
   } /*Else: id file is input as file*/

   /*****************************************************\
   * Main Sec04 Sub02:
   *   - print read ids and clean up
   \*****************************************************/

   errSC =
      pIds_k2TaxaId(
         taxaHeapST, /*has taxa list to print*/
         prefixStr,  /*prefix for output files*/
         pUnclassBl, /*1: print unclassified reads*/
         inFILE
      ); /*print kraken ids by taxa*/

   if(inFILE != stdin)
      fclose(inFILE);
   inFILE = 0;

   freeHeap_taxa_k2TaxaId(taxaHeapST);
   taxaHeapST = 0;

   /*****************************************************\
   * Main Sec04 Sub03:
   *   - check for errors
   \*****************************************************/

   if(errSC)
   { /*If: error*/
      fprintf(
         stderr,
         "file error printing ids from -id %s\n",
         idFileStr
      );

      goto err_main_sec05;
   } /*If: error*/

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
      if(taxaHeapST)
         freeHeap_taxa_k2TaxaId(taxaHeapST);
      taxaHeapST = 0;

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
