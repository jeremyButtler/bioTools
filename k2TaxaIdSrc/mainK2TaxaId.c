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
'     - licensing for this code (CC0)
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
#include "../genLib/endLine.h"
#include "../bioTools.h" /*version*/

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\
! Hidden libraries:
!   o .c  #include "../genLib/genMath.h"
!   o .c  #include "../genLib/ptrAry.h"
\%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

signed char *glob_prefStr = (signed char *) "Hagrid";
signed char *glob_startLevStr = (signed char *) "F";
signed char *glob_endLevStr = (signed char *) "G";

#define def_minDepth_mainK2TaxaId 30 /*at least 30 reads*/
#define def_minPercDepth_mainK2TaxaId 0.0f
#define def_minimizeRep_mainK2TaxaId 0
   /*1: report includes both minimzer columns*/

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
      "k2TaxaId from bioTools version: %i-%02i-%02i%s",
      def_year_bioTools,
      def_month_bioTools,
      def_day_bioTools,
      str_endLine
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
       "k2TaxaId -report k2-report.tsv -id k2-out.tsv%s",
       str_endLine
    );

    fprintf(
       (FILE *) outFILE,
       "  - extracts read ids of target taxa level and%s",
       str_endLine
    );

    fprintf(
       (FILE *) outFILE,
       "    prints ids to individual files%s",
       str_endLine
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
      "Input:%s",
      str_endLine
   );

   /*****************************************************\
   * Fun02 Sec02 Sub02:
   *   - file IO
   \*****************************************************/

   fprintf(
      (FILE *) outFILE,
      "  File IO:%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "    -report k2-report.tsv: [Required]%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "      o tsv report made by kraken2%s",
      str_endLine
   );

   fprintf(
     (FILE *) outFILE,
     "      o \"kraken2 --report k2-report.tsv ...\"%s",
     str_endLine
   );


   if(def_minimizeRep_mainK2TaxaId)
      fprintf(
         (FILE *) outFILE,
         "    -minimize: [Optional; Yes]%s",
         str_endLine
      );
   else
      fprintf(
         (FILE *) outFILE,
         "    -minimize: [Optional; No]%s",
         str_endLine
      );

   fprintf(
     (FILE *) outFILE,
     "      o -report report.tsv has minimizer columns%s",
     str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "        (krakenUnique; --report-minimizer-data)%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "      o disable with -no-minimize%s",
      str_endLine
   );


   fprintf(
     (FILE *) outFILE,
     "      o \"kraken2 --report k2-report.tsv ...\"%s",
     str_endLine
   );


   fprintf(
      (FILE *) outFILE,
      "    -id k2-out.tsv: [Required]%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "      o tsv with kraken2 ids and assignments%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "      o \"kraken2 --out k2-out.tsv ...\"%s",
      str_endLine
   );


   fprintf(
      (FILE *) outFILE,
      "    -prefix %s: [Optional; %s]%s",
      glob_prefStr,
      glob_prefStr,
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "      o prefix for output files%s",
      str_endLine
   );

   /*****************************************************\
   * Fun02 Sec02 Sub03:
   *   - taxa print options
   \*****************************************************/

   fprintf(
      (FILE *) outFILE,
      "  Taxa Printing:%s",
      str_endLine
   );


   if(def_unclassifed_mainK2TaxaId)
      fprintf(
         (FILE *) outFILE,
         "    -un-class: [Optional; Yes]%s",
         str_endLine
      );
   else
      fprintf(
         (FILE *) outFILE,
         "    -un-class: [Optional; No]%s",
         str_endLine
      );

   fprintf(
      (FILE *) outFILE,
      "      o print unclassified reads%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "      o turn off with \"-no-un-class\"%s",
      str_endLine
   );


   if(def_mergeRoot_mainK2TaxaId)
      fprintf(
         (FILE *) outFILE,
         "    -merge-root: [Optional; Yes]%s",
         str_endLine
      );
   else
      fprintf(
         (FILE *) outFILE,
         "    -merge-root: [Optional; No]%s",
         str_endLine
      );

   fprintf(
     (FILE *) outFILE,
     "      o merge lower taxa (nearer root) with taxa%s",
     str_endLine
   );

   fprintf(
     (FILE *) outFILE,
     "        closer to tip of tree%s",
     str_endLine
   );

   fprintf(
     (FILE *) outFILE,
     "      o turn off with \"-no-merge-root\"%s",
     str_endLine
   );


   if(def_tip_maink2TaxaId)
      fprintf(
         (FILE *) outFILE,
         "    -merge-tip: [Optional; Yes]%s",
         str_endLine
      );
   else
      fprintf(
         (FILE *) outFILE,
         "    -merge-tip: [Optional; No]%s",
         str_endLine
      );

   fprintf(
     (FILE *) outFILE,
     "      o merge tip/near tip taxa ids not in range%s",
     str_endLine
   );

   fprintf(
     (FILE *) outFILE,
     "      o turn off with \"-no-merge-tip\"%s",
     str_endLine
   );

   /*****************************************************\
   * Fun02 Sec02 Sub04:
   *   - taxa filtering options
   \*****************************************************/

   fprintf(
      (FILE *) outFILE,
      "  Taxa Filtering:%s",
      str_endLine
   );


   fprintf(
      (FILE *) outFILE,
      "    -start %s: [Optional; %s]%s",
      glob_startLevStr,
      glob_startLevStr,
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "      o first taxonomic level to extract at%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "      o letter (R,D,P,O,F,G,S) + number (0-15)%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "    -end %s: [Optional; %s]%s",
      glob_endLevStr,
      glob_endLevStr,
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "      o last taxonomic level to extract at%s",
      str_endLine
   );



   fprintf(
      (FILE *) outFILE,
      "    -depth %u: [Optional; %u]%s",
      def_minDepth_mainK2TaxaId,
      def_minDepth_mainK2TaxaId,
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "      o minimum read depth to print ids for taxa%s",
      str_endLine
   );


   fprintf(
      (FILE *) outFILE,
      "    -perc-depth %0.04f: [Optional; %0.04f]%s",
      def_minPercDepth_mainK2TaxaId,
      def_minPercDepth_mainK2TaxaId,
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "      o minimum percent read depth to print ids%s",
      str_endLine
   );
   fprintf(
      (FILE *) outFILE,
      "      o first column in the kraken2 report,%s",
      str_endLine
   );
   fprintf(
      (FILE *) outFILE,
      "        which holds the percent of all reads%s",
      str_endLine
   );

   /*****************************************************\
   * Fun02 Sec02 Sub05:
   *   - misc options
   \*****************************************************/

   fprintf(
      (FILE *) outFILE,
      "  Misc:%s",
      str_endLine
   );


   fprintf(
      (FILE *) outFILE,
      "    -h: print this help message and exit%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "    -v: print version number and exit%s",
      str_endLine
   );

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun02 Sec03:
   ^   - print output
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   fprintf(
      (FILE *) outFILE,
      "Output:%s",
      str_endLine
   );

   fprintf(
     (FILE *) outFILE,
     "  - print read ids to prefix-taxa_code-taxon.ids%s",
     str_endLine
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
|   - miniRepBlPtr:
|     o pointer to signed char to be set to
|       - 1: if inputing kraken2 minimizer report
|       - 0: if normal kraken2 report
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
   signed char *miniRepBlPtr,
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

   signed int siArg = 1;
   signed char *tmpStr = 0;
   signed char errSC = 0;

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
            (signed char *) "-report",
            (signed char *) argAryStr[siArg]
         )
      ){ /*If: kraken report input*/
         ++siArg;
         *reportFileStrPtr =
            (signed char *) argAryStr[siArg];
      } /*If: kraken report input*/

      else if(
         ! eqlNull_ulCp(
            (signed char *) "-minimize",
            (signed char *) argAryStr[siArg]
         )
      ) *miniRepBlPtr = 1;

      else if(
         ! eqlNull_ulCp(
            (signed char *) "-no-minimize",
            (signed char *) argAryStr[siArg]
         )
      ) *miniRepBlPtr = 0;

      else if(
         ! eqlNull_ulCp(
            (signed char *) "-id",
            (signed char *) argAryStr[siArg]
         )
      ){ /*If: kraken read assignment (out) file*/
         ++siArg;
         *idFileStrPtr = (signed char *) argAryStr[siArg];
      } /*If: kraken read assignment (out) file*/

      else if(
         ! eqlNull_ulCp(
            (signed char *) "-prefix",
            (signed char *) argAryStr[siArg]
         )
      ){ /*If: prefix for output files*/
         ++siArg;
         *prefixStrPtr = (signed char *) argAryStr[siArg];
      } /*If: prefix for output files*/

      /**************************************************\
      * Fun03 Sec03 Sub02:
      *   - taxa printing options
      \**************************************************/

      else if(
         ! eqlNull_ulCp(
            (signed char *) "-un-class",
            (signed char *) argAryStr[siArg]
         )
      ) *pUnclassBlPtr = 1;

      else if(
         ! eqlNull_ulCp(
            (signed char *) "-no-un-class",
            (signed char *) argAryStr[siArg]
         )
      ) *pUnclassBlPtr = 0;

      else if(
         ! eqlNull_ulCp(
            (signed char *) "-merge-root",
            (signed char *) argAryStr[siArg]
         )
      ) *mergeRootBlPtr = 1;

      else if(
         ! eqlNull_ulCp(
            (signed char *) "-no-merge-root",
            (signed char *) argAryStr[siArg]
         )
      ) *mergeRootBlPtr = 0;

      else if(
         ! eqlNull_ulCp(
            (signed char *) "-merge-tip",
            (signed char *) argAryStr[siArg]
         )
      ) *mergeTipBlPtr = 1;

      else if(
         ! eqlNull_ulCp(
            (signed char *) "-no-merge-tip",
            (signed char *) argAryStr[siArg]
         )
      ) *mergeTipBlPtr = 0;

      /**************************************************\
      * Fun03 Sec03 Sub03:
      *   - taxa filtering options
      \**************************************************/

      else if(
         ! eqlNull_ulCp(
            (signed char *) "-start",
            (signed char *) argAryStr[siArg]
         )
      ){ /*Else If: frist taxa to start print*/
         ++siArg;

         *startSSPtr =
            getLevel_k2TaxaId(
               (signed char *) argAryStr[siArg]
            );

         if(*startSSPtr == 0)
         { /*If: unregonized taxa id*/
            fprintf(
               stderr,
               "-start %s is not a regonized taxa id%s",
               argAryStr[siArg],
               str_endLine
            );

            fprintf(
               stderr,
               "  recognized: D, P, C, O, F, G, and S%s",
               str_endLine
            );

            goto err_fun03_sec04;
         } /*If: unregonized taxa id*/
      } /*Else If: first taxa to print*/

      else if(
         ! eqlNull_ulCp(
            (signed char *) "-end",
            (signed char *) argAryStr[siArg]
         )
      ){ /*Else If: last taxa to print*/
         ++siArg;

         *endSSPtr =
            getLevel_k2TaxaId(
               (signed char *) argAryStr[siArg]
            );

         if(*startSSPtr == 0)
         { /*If: unregonized taxa id*/
            fprintf(
               stderr,
               "-end %s is not a regonized taxa id%s",
               argAryStr[siArg],
               str_endLine
            );

            fprintf(
               stderr,
               "  recognized: D, P, C, O, F, G, and S%s",
               str_endLine
            );

            goto err_fun03_sec04;
         } /*If: unregonized taxa id*/
      } /*Else If: last taxa to print*/


      else if(
         ! eqlNull_ulCp(
            (signed char *) "-depth",
            (signed char *) argAryStr[siArg]
         )
      ){ /*Else If: min read depth*/
         ++siArg;
         tmpStr = (signed char *) argAryStr[siArg];

         tmpStr +=
            strToUL_base10str(
               tmpStr,
               depthULPtr
            );

         if(*tmpStr != '\0')
         { /*If: non-numeric*/
            fprintf(
               stderr,
               "-depth %s is to large or non-numeric%s",
               argAryStr[siArg],
               str_endLine
            );

            goto err_fun03_sec04;
         } /*If: non-numeric*/
      } /*Else If: min read depth*/


      else if(
         ! eqlNull_ulCp(
            (signed char *) "-perc-depth",
            (signed char *) argAryStr[siArg]
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
            (signed char *) "-h",
            (signed char *) argAryStr[siArg]
         )
      ){ /*Else If: help message requested*/
         phelp_mainK2TaxaId(stdout);
         goto phelp_fun03_sec04;
      } /*Else If: help message requested*/

      else if(
         ! eqlNull_ulCp(
            (signed char *) "--h",
            (signed char *) argAryStr[siArg]
         )
      ){ /*Else If: help message requested*/
         phelp_mainK2TaxaId(stdout);
         goto phelp_fun03_sec04;
      } /*Else If: help message requested*/

      else if(
         ! eqlNull_ulCp(
            (signed char *) "help",
            (signed char *) argAryStr[siArg]
         )
      ){ /*Else If: help message requested*/
         phelp_mainK2TaxaId(stdout);
         goto phelp_fun03_sec04;
      } /*Else If: help message requested*/

      else if(
         ! eqlNull_ulCp(
            (signed char *) "-help",
            (signed char *) argAryStr[siArg]
         )
      ){ /*Else If: help message requested*/
         phelp_mainK2TaxaId(stdout);
         goto phelp_fun03_sec04;
      } /*Else If: help message requested*/

      else if(
         ! eqlNull_ulCp(
            (signed char *) "--help",
            (signed char *) argAryStr[siArg]
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
            (signed char *) "-v",
            (signed char *) argAryStr[siArg]
         )
      ){ /*Else If: version number requested*/
         pversion_mainK2TaxaId(stdout);
         goto pversion_fun03_sec04;
      } /*Else If: version number requested*/

      else if(
         ! eqlNull_ulCp(
            (signed char *) "--v",
            (signed char *) argAryStr[siArg]
         )
      ){ /*Else If: version number requested*/
         pversion_mainK2TaxaId(stdout);
         goto pversion_fun03_sec04;
      } /*Else If: version number requested*/

      else if(
         ! eqlNull_ulCp(
            (signed char *) "version",
            (signed char *) argAryStr[siArg]
         )
      ){ /*Else If: version number requested*/
         pversion_mainK2TaxaId(stdout);
         goto pversion_fun03_sec04;
      } /*Else If: version number requested*/

      else if(
         ! eqlNull_ulCp(
            (signed char *) "-version",
            (signed char *) argAryStr[siArg]
         )
      ){ /*Else If: version number requested*/
         pversion_mainK2TaxaId(stdout);
         goto pversion_fun03_sec04;
      } /*Else If: version number requested*/

      else if(
         ! eqlNull_ulCp(
            (signed char *) "--version",
            (signed char *) argAryStr[siArg]
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
            "%s is not recongnized%s",
            argAryStr[siArg],
            str_endLine
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
   '     - read in kraken2 report
   '   o main sec04:
   '     - print kraken2 ids
   '   o main sec05:
   '     - clean up and return
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   signed char *reportFileStr = (signed char *) "";
   signed char *idFileStr = (signed char *) "";
   signed char *prefixStr = glob_prefStr;

   signed short startLevSS =
      getLevel_k2TaxaId(glob_startLevStr);
   signed short endLevSS =
      getLevel_k2TaxaId(glob_endLevStr);

   unsigned long depthUL = def_minDepth_mainK2TaxaId;
   float percDepthF = def_minPercDepth_mainK2TaxaId;

   signed char pUnclassBl = def_unclassifed_mainK2TaxaId;
   signed char miniRepBl = def_minimizeRep_mainK2TaxaId;
   signed char mergeRootBl = def_mergeRoot_mainK2TaxaId;
   signed char mergeTipBl = def_tip_maink2TaxaId;

   signed char errSC = 0; /*for function errors*/
   signed long errSL = 0;
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
         &miniRepBl,
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
            "-report %s and -id %s from stdin%s",
            reportFileStr,
            idFileStr,
            str_endLine
         );

         goto err_main_sec05;
      } /*If: two stdin files*/

      /*from stdin, open later*/
   } /*If: report is from stdin*/

   else
   { /*Else: id file is input as file*/
      inFILE = fopen((char *) idFileStr, "r");

      if(! inFILE)
      { /*If: could not open file*/
         fprintf(
            stderr,
            "unable to open -id %s%s",
            idFileStr,
            str_endLine
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
            "-report %s and -id %s from stdin%s",
            reportFileStr,
            idFileStr,
            str_endLine
         );

         goto err_main_sec05;
      } /*If: two stdin files*/

      inFILE = stdin;
   } /*If: report is from stdin*/

   else
   { /*Else: report input as file*/
      inFILE = fopen((char *) reportFileStr, "r");

      if(! inFILE)
      { /*If: could not open file*/
         fprintf(
            stderr,
            "unable to open -report %s%s",
            reportFileStr,
            str_endLine
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
         miniRepBl,
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
            "no ids in -report %s%s",
            reportFileStr,
            str_endLine
         );
      else if(errSC == def_memErr_k2TaxaId)
         fprintf(
            stderr,
            "memory error reading -report %s%s",
            reportFileStr,
            str_endLine
         );
      else
         fprintf(
            stderr,
            "file error reading -report %s%s",
            reportFileStr,
            str_endLine
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
      inFILE = fopen((char *) idFileStr, "r");

      if(! inFILE)
      { /*If: could not open file*/
         fprintf(
            stderr,
            "unable to open (non-check) -id %s%s",
            idFileStr,
            str_endLine
         );

         goto err_main_sec05;
      } /*If: could not open file*/
   } /*Else: id file is input as file*/

   /*****************************************************\
   * Main Sec04 Sub02:
   *   - print read ids and clean up
   \*****************************************************/

   errSL =
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

   if(errSL)
   { /*If: error*/
      if(errSL == 1)
         fprintf(
           stderr,
           "file error making unclassifed file; -id %s%s",
           idFileStr,
           str_endLine
         );
      else
      fprintf(
         stderr,
         "file error on line %li in -id %s%s",
         errSL - 1,
         idFileStr,
         str_endLine
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
