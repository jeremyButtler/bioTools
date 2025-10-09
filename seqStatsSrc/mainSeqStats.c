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

#define def_minMeanQ_mainSeqStats 13.0
#define def_minMedianQ_mainSeqStats 7.0
#define def_minLen_mainSeqStats 500

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
      "seqStats reads.fastq%s",
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
      "  - also can filter reads by q-score/length%s",
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

   /*****************************************************\
   * Fun02 Sec02 Sub02:
   *   - min length setting
   \*****************************************************/

   fprintf(
      (FILE *) outFILE,
      "  -len %i: [Optional]%s",
      def_minLen_mainSeqStats,
      str_endLine
   );
   fprintf(
     (FILE *) outFILE,
     "    o minimum length to keep read with `-filter`%s",
     str_endLine
   );

   /*****************************************************\
   * Fun02 Sec02 Sub03:
   *   - min mean q-score
   \*****************************************************/

   fprintf(
      (FILE *) outFILE,
      "  -meanq %f: [Optional]%s",
      def_minMeanQ_mainSeqStats,
      str_endLine
   );
   fprintf(
     (FILE *) outFILE,
     "    o minimum mean q-score for `-filter`%s",
     str_endLine
   );

   /*****************************************************\
   * Fun02 Sec02 Sub04:
   *   - min median q-score
   \*****************************************************/

   fprintf(
      (FILE *) outFILE,
      "  -medianq %f: [Optional]%s",
      def_minMedianQ_mainSeqStats,
      str_endLine
   );
   fprintf(
     (FILE *) outFILE,
     "    o minimum median q-score for `-filter`%s",
     str_endLine
   );

   /*****************************************************\
   * Fun02 Sec02 Sub05:
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
|   - lenSIPtr:
|     o signed int pointer to get minimum length
|   - meanQFPtr:
|     o float pointer to get minimum mean q-score
|   - medianQFPtr:
|     o float pointer to get minimum median q-score
|   - modeFlagSCPtr:
|     o signed char pointer to get mode working in
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
   signed int *lenSIPtr, /*gets minimum length*/
   float *meanQFPtr,     /*gets minimum mean q-score*/
   float *medianQFPtr,   /*gets minimum median q-score*/
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
            (signed char *) "-len",
            (signed char *) argAryStr[siArg]
         )
      ){ /*If: minimum length*/
         tmpStr = (signed char *) argAryStr[++siArg];
         tmpStr += strToSI_base10str(tmpStr, lenSIPtr);

         if(*tmpStr)
         { /*If: non-numeric or to large*/
            fprintf(
               stderr,
               "-len %s is non-numeric or to large%s",
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
            (signed char *) "-meanq",
            (signed char *) argAryStr[siArg]
         )
      ){ /*If: minimum mean q-score*/
         tmpStr = (signed char *) argAryStr[++siArg];
         tmpStr += strToF_base10str(tmpStr, meanQFPtr);

         if(*tmpStr)
         { /*If: non-numeric or to large*/
            fprintf(
               stderr,
               "-meanq %s is non-numeric or to large%s",
               argAryStr[siArg],
               str_endLine
            );
            goto err_fun03_sec03;
         } /*If: non-numeric or to large*/
      }  /*If: minimum mean q-score*/

      /**************************************************\
      * Fun03 Sec02 Sub03:
      *   - get minimum median q-score
      \**************************************************/

      else if(
         ! eqlNull_ulCp(
            (signed char *) "-medianq",
            (signed char *) argAryStr[siArg]
         )
      ){ /*Else If: minimum median q-score*/
         tmpStr = (signed char *) argAryStr[++siArg];
         tmpStr += strToF_base10str(tmpStr, medianQFPtr);

         if(*tmpStr)
         { /*If: non-numeric or to large*/
            fprintf(
               stderr,
               "-medianq %s is non-numeric or to large%s",
               argAryStr[siArg],
               str_endLine
            );
            goto err_fun03_sec03;
         } /*If: non-numeric or to large*/
      }  /*Else If: minimum median q-score*/

      /**************************************************\
      * Fun03 Sec02 Sub04:
      *   - get mode using
      \**************************************************/

      else if(
         ! eqlNull_ulCp(
            (signed char *) "-filter",
            (signed char *) argAryStr[siArg]
         )
      ) *modeFlagSCPtr = def_filterMode_mainSeqStats;

      else if(
         ! eqlNull_ulCp(
            (signed char *) "-stats",
            (signed char *) argAryStr[siArg]
         )
      ) *modeFlagSCPtr = def_statsMode_mainSeqStats;

      /**************************************************\
      * Fun03 Sec02 Sub05:
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
      * Fun03 Sec02 Sub06:
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
      * Fun03 Sec02 Sub07:
      *   - check if on files or invalid input
      \**************************************************/

      else
      { /*Else: if unkown input*/
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
      * Fun03 Sec02 Sub08:
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

   float minMeanQF = def_minMeanQ_mainSeqStats;
   float minMedianQF = def_minMedianQ_mainSeqStats;
   signed int minLenSI = def_minLen_mainSeqStats;
   signed char modeFlagSC = def_mode_mainSeqStats;

   float meanQF = 0;
   float medianQF = 0;

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
         &minMeanQF,
         &minMedianQF,
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

      /**************************************************\
      * Main Sec03 Sub02:
      *   - get stats for stats mode + start read loop
      \**************************************************/

      while(! errSC)
      { /*Loop: get stats for fastq file*/
         if(modeFlagSC == def_statsMode_mainSeqStats)
            pReadStats_seqStats(
               seqStackST.qStr,
               seqStackST.seqLenSL,
               seqStackST.idStr,
               &pHeadBl,
               0,
               1, /*ONT only*/
               outFILE
            );

         /***********************************************\
         * Main Sec03 Sub03:
         *   - filter reads for filter reads mode
         \***********************************************/

         else if(modeFlagSC==def_filterMode_mainSeqStats)
         { /*Else If: filtering the read*/
            if(seqStackST.seqLenSL < minLenSI)
               goto nextRead_main_sec03_sub04;

            meanQF =
               ontMeanMedQ_seqStats(
                  seqStackST.qStr,
                  seqStackST.seqLenSL,
                  &medianQF,
                  0
               );

            if(meanQF < minMeanQF)
               goto nextRead_main_sec03_sub04;
            if(medianQF < minMedianQF)
               goto nextRead_main_sec03_sub04;

            fprintf(
               (FILE *) outFILE,
               "%s\tmeanQ=%.2f\tmedianQ=%.2f",
               seqStackST.idStr,
               meanQF,
               medianQF
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
         } /*Else If: filtering the read*/

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
