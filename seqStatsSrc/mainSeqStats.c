/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' mainSeqStats SOF: Start Of File
'   - driver function to get sequence stats
'   o header:
'     - included libraries
'   o fun01: pversion_mainSeqStats
'     - prints the version number for seqStats
'   o fun02: phelp_mainSeqStats
'     - prints the help message for seqStats
'   o fun03: input_mainSeqStats
'     - get input from users command line input
'   o license:
'     - licensing for this code (CC0)
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

#include "../genLib/ulCp.h"
#include "../genLib/base10str.h"
#include "../genLib/inflate.h"
#include "../genBio/seqST.h"
#include "../genBio/gzSeqST.h"
#include "../genBio/seqStats.h"

/*.h files only*/
#include "../genLib/endLine.h"
#include "../bioTools.h"

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\
! Hidden Libraries:
!   - .c  #include "../genLib/genMath.h"
!   - .c  #include "../genLib/checkSum.h"
!   - .c  #include "../genLib/endin.h"
!   - .c  #include "../genLib/fileFun.h"
\%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

#define def_ontMinMeanQ_mainSeqStats 13.0
#define def_ontMinMedianQ_mainSeqStats 7.0

#define def_regMinMeanQ_mainSeqStats 0.0f
#define def_regMinMedianQ_mainSeqStats 0.0f

/*bases to skip in q-score calculation*/
#define def_ontSkip_mainSeqStats 60
#define def_regSkip_mainSeqStats 60

#define def_minLen_mainSeqStats 500
#define def_maxLen_mainSeqStats 0

#define def_statsMode_mainSeqStats 1
#define def_filterMode_mainSeqStats 2
#define def_mode_mainSeqStats def_statsMode_mainSeqStats

/*-------------------------------------------------------\
| Fun01: pversion_mainSeqStats
|   - prints the version number for seqStats
| Input:
|   - outFILE:
|     o FILE pointer to print version number to
| Output:
|   - Prints:
|     o prints version number to outFILE
\-------------------------------------------------------*/
void
pversion_mainSeqStats(
   void *outFILE
){
   fprintf(
      (FILE *) outFILE,
      "seqStats from bioTools version: %i-%02i-%02i%s",
      def_year_bioTools,
      def_month_bioTools,
      def_day_bioTools,
      str_endLine
   );
} /*pversion_mainSeqStats*/

/*-------------------------------------------------------\
| Fun02: phelp_mainSeqStats
|   - prints the help message for seqStats
| Input:
|   - outFILE:
|     o FILE pointer to print help message to
| Output:
|   - Prints:
|     o prints help message to outFILE
\-------------------------------------------------------*/
void
phelp_mainSeqStats(
   void *outFILE
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun02 TOC:
   '   - prints the help message for seqStats
   '   o fun02 sec01:
   '     - print usage line
   '   o fun02 sec02:
   '     - print input
   '   o fun02 sec03:
   '     - print output
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun02 Sec01:
   ^   - print usage line
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   fprintf(
      (FILE *) outFILE,
      "Get stats as tsv file (id, length, q-scores):%s",
      str_endLine
   );
   fprintf(
      (FILE *) outFILE,
      "   seqStats reads.fastq > out.tsv%s",
      str_endLine
   );
   fprintf(
      (FILE *) outFILE,
      "Get Filtered Stats (id, length, q-scores):%s",
      str_endLine
   );
   fprintf(
     (FILE *) outFILE,
     "   seqStats -filter [options...] reads.fastq ... > out.tsv%s",
     str_endLine
   );
   fprintf(
      (FILE *) outFILE,
      "Filter Reads:%s",
      str_endLine
   );
   fprintf(
      (FILE *) outFILE,
      "   seqStats -filter -no-stats [options...] reads.fastq ... > out.fastq%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "Use:%s",
      str_endLine
   );
   fprintf(
      (FILE *) outFILE,
      "  - prints the read id, length, mean q-score,%s",
      str_endLine
   );
   fprintf(
      (FILE *) outFILE,
      "    and median q-score for all reads%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "  - Can filter reads by q-score/length%s",
      str_endLine
   );

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun02 Sec02:
   ^   - print input
   ^   o fun02 sec02 sub01:
   ^     - input header and mode settings
   ^   o fun02 sec02 sub02:
   ^     - min length setting
   ^   o fun02 sec02 sub03:
   ^     - min mean q-score
   ^   o fun02 sec02 sub04:
   ^     - min median q-score
   ^   o fun02 sec02 sub05:
   ^     - fastq file input
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun02 Sec02 Sub01:
   *   - input header and mode settings
   \*****************************************************/

   fprintf((FILE *) outFILE, "Input:%s", str_endLine);

   if(def_mode_mainSeqStats == def_statsMode_mainSeqStats)
      fprintf(
         (FILE *) outFILE,
         "  -stats: [Yes]%s",
         str_endLine
      );
   else
      fprintf(
         (FILE *) outFILE,
         "  -stats: [no]%s",
         str_endLine
      );

   fprintf(
      (FILE *) outFILE,
      "    o print read stats to stdout, do not filter%s",
      str_endLine
   );
   fprintf(
      (FILE *) outFILE,
      "    o disable with `-no-stats`%s",
      str_endLine
   );


   if(def_mode_mainSeqStats == def_statsMode_mainSeqStats)
      fprintf(
         (FILE *) outFILE,
         "  -filter: [no]%s",
         str_endLine
      );
   else
      fprintf(
         (FILE *) outFILE,
         "  -filter: [yes]%s",
         str_endLine
      );

   fprintf(
      (FILE *) outFILE,
      "    o print filtred reads to stdout%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "    o disable with `-no-filter`%s",
      str_endLine
   );

   /*****************************************************\
   * Fun02 Sec02 Sub02:
   *   - min length setting
   \*****************************************************/

   fprintf(
      (FILE *) outFILE,
      "  -min-len %i: [Optional]%s",
      def_minLen_mainSeqStats,
      str_endLine
   );
   fprintf(
     (FILE *) outFILE,
     "    o minimum length to keep read with `-filter`%s",
     str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "  -max-len %i: [Optional]%s",
      def_maxLen_mainSeqStats,
      str_endLine
   );
   fprintf(
     (FILE *) outFILE,
     "    o maximum length to keep read with `-filter`%s",
     str_endLine
   );
   fprintf(
     (FILE *) outFILE,
     "    o use 0 for any length (no maximum length)%s",
     str_endLine
   );

   /*****************************************************\
   * Fun02 Sec02 Sub03:
   *   - min mean q-score
   \*****************************************************/

   fprintf(
      (FILE *) outFILE,
      "  -ont-mean-q %f: [Optional]%s",
      def_ontMinMeanQ_mainSeqStats,
      str_endLine
   );
   fprintf(
     (FILE *) outFILE,
     "    o ont's minimum mean q-score for `-filter`%s",
     str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "  -reg-mean-q %f: [Optional]%s",
      def_regMinMeanQ_mainSeqStats,
      str_endLine
   );
   fprintf(
     (FILE *) outFILE,
     "    o regular minimum mean q-score for `-filter`%s",
     str_endLine
   );

   /*****************************************************\
   * Fun02 Sec02 Sub04:
   *   - min median q-score
   \*****************************************************/

   fprintf(
      (FILE *) outFILE,
      "  -ont-median-q %f: [Optional]%s",
      def_ontMinMedianQ_mainSeqStats,
      str_endLine
   );
   fprintf(
     (FILE *) outFILE,
     "    o ont's minimum median q-score for `-filter`%s",
     str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "  -reg-median-q %f: [Optional]%s",
      def_ontMinMedianQ_mainSeqStats,
      str_endLine
   );
   fprintf(
     (FILE *) outFILE,
     "    o regular minimum median q-score for filter%s",
     str_endLine
   );

   /*****************************************************\
   * Fun02 Sec02 Sub05:
   *   - fastq file input
   \*****************************************************/

   fprintf(
      (FILE *) outFILE,
      "  -ont-skip %i: [Optional]%s",
      def_ontSkip_mainSeqStats,
      str_endLine
   );
   fprintf(
     (FILE *) outFILE,
     "    o first x bases to ignore in the ONT%s",
     str_endLine
   );
   fprintf(
     (FILE *) outFILE,
     "      q-score calculation%s",
     str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "  -reg-skip %i: [Optional]%s",
      def_regSkip_mainSeqStats,
      str_endLine
   );
   fprintf(
     (FILE *) outFILE,
     "    o first x bases to ignore in the regular%s",
     str_endLine
   );
   fprintf(
     (FILE *) outFILE,
     "      q-score calculation%s",
     str_endLine
   );

   /*****************************************************\
   * Fun02 Sec02 Sub06:
   *   - fastq file input
   \*****************************************************/

   fprintf(
      (FILE *) outFILE,
      "  - reads.fastq: [Required]%s",
      str_endLine
   );
   fprintf(
      (FILE *) outFILE,
      "    o fastq or fastq.gz file to filter or get%s",
      str_endLine
   );
   fprintf(
      (FILE *) outFILE,
      "      stats for%s",
      str_endLine
   );
 
   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun02 Sec03:
   ^   - print output
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   fprintf((FILE *) outFILE, "Output:%s", str_endLine);
   fprintf(
      (FILE *) outFILE,
      "  - read stats or filtered reads to stdout%s",
      str_endLine
   );
} /*phelp_mainSeqStats*/

/*-------------------------------------------------------\
| Fun03: input_mainSeqStats
|   - get input from users command line input
| Input:
|   - argLenSI:
|     o number of arguments the user input
|   - argAryStr:
|     o c-string array with user input
|   - minLenSIPtr:
|     o signed int pointer to get minimum length
|   - maxLenSIPtr:
|     o signed int pointer to get the maximum length
|   - ontMeanQFPtr:
|     o float pointer to get ont minimum mean q-score
|   - ontMedianQFPtr:
|     o float pointer to get ont minimum median q-score
|   - regMeanQFPtr:
|     o float pointer to get ont minimum mean q-score
|   - regMedianQFPtr:
|     o float pointer to get ont minimum median q-score
|   - ontNtSkipSIPtr:
|     o signed int pointer to get number of first bases to 
|       skip in ONT q-score calculation
|   - regNtSkipSIPtr:
|     o signed int pointer to get number of first bases to 
|       skip in regular q-score calculation
|   - modeFlagSCPtr:
|     o signed char pointer to get mode working in
|     o def_statsMode_mainSeqStats (1) if printing stats
|       (not reads)
|     o def_filterMode_mainSeqStats (2) if filtering
| Output:
|   - Prints:
|     o errors to stderr
|   - Modifies:
|     o lenSIPtr, meanQFPtr, and medianQFPtr to have user
|       input
|   - Returns:
|     - position of first fastq file to scan
|     - 0 if printed help message or version number
|     - -1 if had error
\-------------------------------------------------------*/
signed int
input_mainSeqStats(
   signed int argLenSI,  /*number of arguments*/
   char *argAryStr[],    /*input arguments*/
   signed int *minLenSIPtr, /*gets minimum length*/
   signed int *maxLenSIPtr, /*gets maximum length*/
   float *ontMeanQFPtr,  /*gets minimum mean q-score*/
   float *ontMedianQFPtr,/*gets minimum median q-score*/
   float *regMeanQFPtr,  /*gets minimum mean q-score*/
   float *regMedianQFPtr,/*gets minimum median q-score*/
   signed int *ontNtSkipSIPtr, /*ont bases to skip*/
   signed int *regNtSkipSIPtr, /*regular bases to skip*/
   signed char *modeFlagSCPtr /*mode to work in*/
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun03 TOC:
   '   - get input from users command line input
   '   o fun03 sec01:
   '     - variable declarations and check if have input
   '   o fun03 sec02:
   '     - get user input
   '   o fun03 sec03:
   '     - return
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec01:
   ^   - variable declarations and check if have input
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   signed int siArg = 1;
   signed char *tmpStr = 0;
   FILE *inFILE = 0;

   if(argLenSI < 2)
      goto phelp_fun03_sec03;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec02:
   ^   - get user input
   ^   o fun03 sec02 sub01:
   ^     - get minimum length input + start input loop
   ^   o fun03 sec02 sub02:
   ^     - get minimum mean q-score
   ^   o fun03 sec02 sub03:
   ^     - get minimum median q-score
   ^   o fun03 sec02 sub04:
   ^     - get mode using
   ^   o fun03 sec02 sub05:
   ^     - get help message
   ^   o fun03 sec02 sub06:
   ^     - get version number
   ^   o fun03 sec02 sub07:
   ^     - check if on files or invalid input
   ^   o fun03 sec02 sub08:
   ^     - move to next argument
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun03 Sec02 Sub01:
   *   - get minimum length input + start input loop
   \*****************************************************/

   siArg = 1;
   while(siArg < argLenSI)
   { /*Loop: get user input*/
      if(
         ! eqlNull_ulCp(
            (signed char *) "-min-len",
            (signed char *) argAryStr[siArg]
         )
      ){ /*If: minimum length*/
         tmpStr = (signed char *) argAryStr[++siArg];
         tmpStr += strToSI_base10str(tmpStr, minLenSIPtr);

         if(*tmpStr)
         { /*If: non-numeric or to large*/
            fprintf(
               stderr,
               "-min-len %s is non-numeric or to large%s",
               argAryStr[siArg],
               str_endLine
            );
            goto err_fun03_sec03;
         } /*If: non-numeric or to large*/
      }  /*If: minimum length*/

      else if(
         ! eqlNull_ulCp(
            (signed char *) "-max-len",
            (signed char *) argAryStr[siArg]
         )
      ){ /*If: minimum length*/
         tmpStr = (signed char *) argAryStr[++siArg];
         tmpStr += strToSI_base10str(tmpStr, maxLenSIPtr);

         if(*tmpStr)
         { /*If: non-numeric or to large*/
            fprintf(
               stderr,
               "-max-len %s is non-numeric or to large%s",
               argAryStr[siArg],
               str_endLine
            );
            goto err_fun03_sec03;
         } /*If: non-numeric or to large*/
      }  /*If: minimum length*/

      /**************************************************\
      * Fun03 Sec02 Sub02:
      *   - get minimum mean q-score
      \**************************************************/

      else if(
         ! eqlNull_ulCp(
            (signed char *) "-ont-mean-q",
            (signed char *) argAryStr[siArg]
         )
      ){ /*If: minimum ont mean q-score*/
         tmpStr = (signed char *) argAryStr[++siArg];
         tmpStr += strToF_base10str(tmpStr, ontMeanQFPtr);

         if(*tmpStr)
         { /*If: non-numeric or to large*/
            fprintf(
              stderr,
              "-ont-mean-q %s; non-numeric or to large%s",
              argAryStr[siArg],
              str_endLine
            );
            goto err_fun03_sec03;
         } /*If: non-numeric or to large*/
      }  /*If: minimum ont mean q-score*/

      else if(
         ! eqlNull_ulCp(
            (signed char *) "-reg-mean-q",
            (signed char *) argAryStr[siArg]
         )
      ){ /*Else If: regular minimum mean q-score*/
         tmpStr = (signed char *) argAryStr[++siArg];
         tmpStr += strToF_base10str(tmpStr, regMeanQFPtr);

         if(*tmpStr)
         { /*If: non-numeric or to large*/
            fprintf(
              stderr,
              "-reg-mean-q %s; non-numeric or to large%s",
              argAryStr[siArg],
              str_endLine
            );
            goto err_fun03_sec03;
         } /*If: non-numeric or to large*/
      }  /*Else If: regular minimum mean q-score*/

      /**************************************************\
      * Fun03 Sec02 Sub03:
      *   - get minimum median q-score
      \**************************************************/

      else if(
         ! eqlNull_ulCp(
            (signed char *) "-ont-median-q",
            (signed char *) argAryStr[siArg]
         )
      ){ /*Else If: ont minimum median q-score*/
         tmpStr = (signed char *) argAryStr[++siArg];
         tmpStr +=
            strToF_base10str(tmpStr, ontMedianQFPtr);

         if(*tmpStr)
         { /*If: non-numeric or to large*/
            fprintf(
               stderr,
               "-ont-median-q %s; non-numeric/to large%s",
               argAryStr[siArg],
               str_endLine
            );
            goto err_fun03_sec03;
         } /*If: non-numeric or to large*/
      }  /*Else If: ont minimum median q-score*/

      else if(
         ! eqlNull_ulCp(
            (signed char *) "-reg-median-q",
            (signed char *) argAryStr[siArg]
         )
      ){ /*Else If: regular minimum median q-score*/
         tmpStr = (signed char *) argAryStr[++siArg];
         tmpStr +=
            strToF_base10str(tmpStr, regMedianQFPtr);

         if(*tmpStr)
         { /*If: non-numeric or to large*/
            fprintf(
               stderr,
               "-reg-median-q %s; non-numeric/to large%s",
               argAryStr[siArg],
               str_endLine
            );
            goto err_fun03_sec03;
         } /*If: non-numeric or to large*/
      }  /*Else If: regular minimum median q-score*/

      /**************************************************\
      * Fun03 Sec02 Sub04:
      *   - get number bases to skip for q-score
      \**************************************************/

      else if(
         ! eqlNull_ulCp(
            (signed char *) "-ont-skip",
            (signed char *) argAryStr[siArg]
         )
      ){ /*Else If: ont number of bases to skip at start*/
         tmpStr = (signed char *) argAryStr[++siArg];
         tmpStr +=
            strToSI_base10str(tmpStr, ontNtSkipSIPtr);

         if(*tmpStr)
         { /*If: non-numeric or to large*/
            fprintf(
               stderr,
               "-ont-skip %s; non-numeric or to large%s",
               argAryStr[siArg],
               str_endLine
            );
            goto err_fun03_sec03;
         } /*If: non-numeric or to large*/
      }  /*Else If: ont number of bases to skip at start*/

      else if(
         ! eqlNull_ulCp(
            (signed char *) "-reg-skip",
            (signed char *) argAryStr[siArg]
         )
      ){ /*Else If: number of first bases to skip reg*/
         tmpStr = (signed char *) argAryStr[++siArg];
         tmpStr +=
            strToSI_base10str(tmpStr, regNtSkipSIPtr);

         if(*tmpStr)
         { /*If: non-numeric or to large*/
            fprintf(
               stderr,
               "-reg-skip %s; non-numeric or to large%s",
               argAryStr[siArg],
               str_endLine
            );
            goto err_fun03_sec03;
         } /*If: non-numeric or to large*/
      }  /*Else If: number of first bases to skip reg*/

      /**************************************************\
      * Fun03 Sec02 Sub05:
      *   - get mode using
      \**************************************************/

      else if(
         ! eqlNull_ulCp(
            (signed char *) "-filter",
            (signed char *) argAryStr[siArg]
         )
      ) *modeFlagSCPtr |= def_filterMode_mainSeqStats;

      else if(
         ! eqlNull_ulCp(
            (signed char *) "-no-filter",
            (signed char *) argAryStr[siArg]
         )
      ) *modeFlagSCPtr &= ~def_filterMode_mainSeqStats;

      else if(
         ! eqlNull_ulCp(
            (signed char *) "-stats",
            (signed char *) argAryStr[siArg]
         )
      ) *modeFlagSCPtr |= def_statsMode_mainSeqStats;

      else if(
         ! eqlNull_ulCp(
            (signed char *) "-no-stats",
            (signed char *) argAryStr[siArg]
         )
      ) *modeFlagSCPtr &= ~def_statsMode_mainSeqStats;

      /**************************************************\
      * Fun03 Sec02 Sub06:
      *   - get help message
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
      * Fun03 Sec02 Sub07:
      *   - get version number
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
            (signed char *) "-version",
            (signed char *) argAryStr[siArg]
         )
      ) goto pversion_fun03_sec03;

      /**************************************************\
      * Fun03 Sec02 Sub08:
      *   - check if on files or invalid input
      \**************************************************/

      else
      { /*Else: if unkown input*/
         if(
               argAryStr[siArg][0] == '-'
            && ! argAryStr[siArg][1]
         ) goto ret_fun03_sec03; /*stdin input*/

         inFILE = fopen(argAryStr[siArg], "r");
         if(inFILE)
            goto ret_fun03_sec03;
         fprintf(
            stderr,
            "%s is not recgonized%s",
            argAryStr[siArg],
            str_endLine
         );
         goto err_fun03_sec03;
      } /*Else: if unkown input*/

      /**************************************************\
      * Fun03 Sec02 Sub09:
      *   - move to next argument
      \**************************************************/

      ++siArg;
   } /*Loop: get user input*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec03:
   ^   - return
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   goto ret_fun03_sec03;

   phelp_fun03_sec03:;
      phelp_mainSeqStats(stdout);
      siArg = 0;
      goto ret_fun03_sec03;

   pversion_fun03_sec03:;
      pversion_mainSeqStats(stdout);
      siArg = 0;
      goto ret_fun03_sec03;

   err_fun03_sec03:;
      siArg = -1;
      goto ret_fun03_sec03;

   ret_fun03_sec03:;
      if(inFILE)
         fclose(inFILE);
      inFILE = 0;

      return siArg;
} /*input_mainSeqStats*/

/*-------------------------------------------------------\
| Main:
|   - driver function for seqStats
| Input:
|   - argLenSI:
|     o number arguments the user input
|   - argAryStr:
|     o c-string array with user input
| Output:
|   - Prints:
|     o read stats to stdout (terminal)
|     o errors to stderr
|   - Returns:
|     o 0 for no errors
|     o 1 errors
\-------------------------------------------------------*/
int
main(
   int argLenSI,
   char *argAryStr[]
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Main TOC:
   '   - driver function for seqStats
   '   o main sec01:
   '     - variable declarations
   '   o main sec02:
   '     - initialize and check for help message
   '   o main sec03:
   '     - print read stats
   '   o main sec04:
   '     - return and clean up
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   signed char pHeadBl = 1;
   signed char stdinBl = 0;
   signed int siArg = 1;
   struct seqST seqStackST;

   struct file_inflate fileSTPtr;
   signed char fxTypeSC = 0;
   signed char errSC = 0;
   signed long seqSL = 0;

   float ontMinMeanQF = def_ontMinMeanQ_mainSeqStats;
   float ontMinMedianQF = def_ontMinMedianQ_mainSeqStats;
   signed int ontNtSkipSI = def_ontSkip_mainSeqStats;

   float regMinMeanQF = def_regMinMeanQ_mainSeqStats;
   float regMinMedianQF = def_regMinMedianQ_mainSeqStats;
   signed int regNtSkipSI = def_regSkip_mainSeqStats;

   signed int minLenSI = def_minLen_mainSeqStats;
   signed int maxLenSI = def_maxLen_mainSeqStats;
   signed char modeFlagSC = def_mode_mainSeqStats;

   float ontMeanQF = 0;
   float ontMedianQF = 0;
   float regMeanQF = 0;
   float regMedianQF = 0;

   FILE *seqFILE = 0;
   FILE *outFILE = stdout;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec02:
   ^   - initialize and check for help message
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   init_file_inflate(&fileSTPtr);
   init_seqST(&seqStackST);

   siArg =
      input_mainSeqStats(
         argLenSI,
         argAryStr,
         &minLenSI,
         &maxLenSI,
         &ontMinMeanQF,
         &ontMinMedianQF,
         &regMinMeanQF,
         &regMinMedianQF,
         &ontNtSkipSI,
         &regNtSkipSI,
         &modeFlagSC
      );

   if(siArg < 1)
      goto ret_main_sec04;
      /*siArg is 0 for help message and version number*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec03:
   ^   - print read stats
   ^   o main sec03 sub01:
   ^     - open each file and get first read + start loop
   ^   o main sec03 sub02:
   ^     - get stats for stats mode + start read loop
   ^   o main sec03 sub03:
   ^     - filter reads for filter reads mode
   ^   o main sec03 sub04:
   ^     - move to next read
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Main Sec03 Sub01:
   *   - open each file and get first read + start loop
   \*****************************************************/

   while(siArg < argLenSI)
   { /*Loop: print read stats*/
      if(argAryStr[siArg][0] == '-')
      { /*If: taking stdin input*/
        if(stdinBl)
        { /*If: already read stdin*/
            ++siArg;
            continue; /*already read stdin input*/
        } /*If: already read stdin*/

        seqFILE = stdin;
        stdinBl = 1;
      } /*If: taking stdin input*/

      else
      { /*Else: open the file*/
         seqFILE = fopen(argAryStr[siArg], "r");
         if(! seqFILE)
            goto fileOpenErr_main_sec04;
      } /*Else: open the file*/

      errSC =
         get_gzSeqST(
            &fileSTPtr,
            &fxTypeSC,
            &seqStackST,
            seqFILE
         );
      seqFILE = 0;
      seqSL = 1;

      if(errSC == def_EOF_seqST && seqStackST.idStr[0])
         errSC = 0;

      /**************************************************\
      * Main Sec03 Sub02:
      *   - get stats for stats mode + start read loop
      \**************************************************/

      while(! errSC)
      { /*Loop: get stats for fastq file*/
         if(modeFlagSC & def_filterMode_mainSeqStats)
         { /*If: filtering the read*/
            if(seqStackST.seqLenSL < minLenSI)
               goto nextRead_main_sec03_sub04;
            else if(maxLenSI <= 0)
               ; /*not filtering by length*/
            else if(seqStackST.seqLenSL > maxLenSI)
               goto nextRead_main_sec03_sub04;

            regMeanQF =
               meanMedQ_seqStats(
                  seqStackST.qStr,
                  seqStackST.seqLenSL,
                  &regMedianQF,
                  regNtSkipSI
               );

            ontMeanQF =
               ontMeanMedQ_seqStats(
                  seqStackST.qStr,
                  seqStackST.seqLenSL,
                  &ontMedianQF,
                  ontNtSkipSI
               );

            if(ontMeanQF < ontMinMeanQF)
               goto nextRead_main_sec03_sub04;
            else if(ontMedianQF < ontMinMedianQF)
               goto nextRead_main_sec03_sub04;
            else if(regMeanQF < regMinMeanQF)
               goto nextRead_main_sec03_sub04;
            else if(regMedianQF < regMinMedianQF)
               goto nextRead_main_sec03_sub04;
         } /*If: filtering the read*/

         if(modeFlagSC & def_statsMode_mainSeqStats)
         { /*If: only printing stats*/
            pReadStats_seqStats(
               seqStackST.qStr,
               seqStackST.seqLenSL,
               seqStackST.idStr,
               pHeadBl,
               ontNtSkipSI,
               regNtSkipSI,
               3, /*both ONT and regular*/
               outFILE
            );

            if(pHeadBl)
               pHeadBl = 0;
         } /*If: only printing stats*/

         /***********************************************\
         * Main Sec03 Sub03:
         *   - filter reads for filter reads mode
         \***********************************************/

         else
         { /*Else If: printing reads*/
            fprintf(
               (FILE *) outFILE,
               "@%s\tontMeanQ=%.2f\tontMedianQ=%.2f",
               seqStackST.idStr,
               ontMeanQF,
               ontMedianQF
            );

            fprintf(
               (FILE *) outFILE,
               "\tregMeanQ=%.2f\tregMedianQ=%.2f",
               regMeanQF,
               regMedianQF
            );


            fprintf(
               (FILE *) outFILE,
               "\tlength=%li%s%s%s+%s%s%s",
               seqStackST.seqLenSL,
               str_endLine,
               seqStackST.seqStr,
               str_endLine,
               str_endLine,
               seqStackST.qStr,
               str_endLine
            );
         } /*Else If: printing reads*/

         /***********************************************\
         * Main Sec03 Sub04:
         *   - move to next read
         \***********************************************/

         nextRead_main_sec03_sub04:;
            errSC =
               get_gzSeqST(
                  &fileSTPtr,
                  &fxTypeSC,
                  &seqStackST,
                  seqFILE
               );
            ++seqSL;
      } /*Loop: get stats for fastq file*/

      if(errSC != def_EOF_seqST)
         goto fileErr_main_sec04;
      else
         ++siArg;
   } /*Loop: print read stats*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec04:
   ^   - return and clean up
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   siArg = 0;
   goto ret_main_sec04;

   fileOpenErr_main_sec04:;
      fprintf(
        stderr,
        "could not open %s%s",
        argAryStr[siArg],
        str_endLine
      );
      siArg = 1;
      goto ret_main_sec04;

   fileErr_main_sec04:;
      fprintf(
        stderr,
        "error on sequence %li in file %s%s",
        seqSL,
        argAryStr[siArg],
        str_endLine
      );
      siArg = 1;
      goto ret_main_sec04;

   ret_main_sec04:;
      freeStack_file_inflate(&fileSTPtr);
      freeStack_seqST(&seqStackST);

      if(! seqFILE) ;
      else if(seqFILE == stdin) ;
      else if(seqFILE == stdout) ;
      else if(seqFILE == stderr) ;
      else fclose(seqFILE);
      seqFILE = 0;

      if(! outFILE) ;
      else if(outFILE == stdin) ;
      else if(outFILE == stdout) ;
      else if(outFILE == stderr) ;
      else fclose(outFILE);
      outFILE = 0;

      return siArg;
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
