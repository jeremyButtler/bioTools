/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' mainDemux SOF: Start Of File
'   - is driver code to demultiplex reads
'   o header:
'     - included libraries and default settings
'   o fun01: pversion_mainDemux
'     - print version number for demux
'   o fun02: phelp_mainDemux
'     - print help message for demux
'   o fun03: input_mainDemux
'     - get user input from the aguments array
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

#include "../genLib/ulCp.h"
#include "../genLib/base10str.h"
#include "../genLib/inflate.h"
#include "../genBio/seqST.h"
#include "../genBio/gzSeqST.h"
#include "../genAln/alnSet.h"
#include "../genAln/kmerFind.h"
#include "../genAln/demux.h"


/*.h files only*/
#include "../genLib/endLine.h"
#include "../bioTools.h"

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\
! Hidden Libraries:
!   - .c  #include "../genLib/ulCp.h"
!   - .c  #include "../genLib/base10Str.h"
!   - .c  #include "../genLib/endin.h"
!   - .c  #include "../genLib/checkSum.h"
!   - .c  #include "../genAln/indexToCoord.h"
!   - .c  #include "../genLib/shellSort.h"
!   - .c  #include "../genLib/fileFun.h"
!   - .c  #include "memwater.h"
!   - .h  #include "../genLib/genMath.h"
!   - .h  #include "alnDefs.h"
!   - .h  #include "../genBio/kmerBit.h"
\%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

#define def_minPercScore_mainDemux def_minPercScore_kmerFind
#define def_minScore_mainDemux 111
   /*for ONT barcodes (24bp) allows 2 snps, no gaps*/
#define def_maxSplits_mainDemux 1
#define def_minDist_mainDemux 100
#define def_trimBarcodes_mainDemux 0

/*-------------------------------------------------------\
| Fun01: pversion_mainDemux
|   - print version number for demux
| Input:
|   - outFILE:
|     o FILE pointer to file to print version number to
| Output:
|   - Prints:
|     o version number to outFILE
\-------------------------------------------------------*/
void
pversion_mainDemux(
   void *outFILE
){
   fprintf(
      (FILE *) outFILE,
      "demux from bioTools version: %i-%02i-%02i%s",
      def_year_bioTools,
      def_month_bioTools,
      def_day_bioTools,
      str_endLine
   );
} /*pversion_mainDemux*/

/*-------------------------------------------------------\
| Fun02: phelp_mainDemux
|   - print help message for demux
| Input:
|   - outFILE:
|     o FILE pointer to file to print help message to
| Output:
|   - Prints:
|     o help message to outFILE
\-------------------------------------------------------*/
void
phelp_mainDemux(
   void *outFILE
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun02 TOC:
   '   - print help message for demux
   '   o fun02 sec01:
   '     - program usage header
   '   o fun02 sec02:
   '     - input
   '   o fun02 sec03:
   '     - output
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun02 Sec01:
   ^   - program usage header
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   fprintf(
      (FILE *) outFILE,
      "demux -bar barcodes.fasta -prefix \"ouput_prefix\""
   );
   fprintf(
      (FILE *) outFILE,
      " reads.fastq%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "  - demultiplexes reads into separate files%s",
      str_endLine
   );

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun02 Sec02:
   ^   - input
   ^   o fun02 sec02 sub01:
   ^     - input header and barcode fasta file
   ^   o fun02 sec02 sub02:
   ^     - prefix for output
   ^   o fun02 sec02 sub03:
   ^     - min percent score
   ^   o fun02 sec02 sub04:
   ^     - min score
   ^   o fun02 sec02 sub05:
   ^     - max splits
   ^   o fun02 sec02 sub06:
   ^     - min dist
   ^   o fun02 sec02 sub07:
   ^     - trim
   ^   o fun02 sec02 sub08:
   ^     - fastx files at end
   ^   o fun02 sec02 sub09:
   ^     - help and version
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun02 Sec02 Sub01:
   *   - input header and barcode fasta file
   \*****************************************************/

   fprintf((FILE *) outFILE, "Input:%s", str_endLine);

   fprintf(
      (FILE *) outFILE,
      "  -bar barcodes.fasta: [Required]%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "    o fasta file with barcode sequences%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "    o headers are used for the output suffix%s",
      str_endLine
   );

   /*****************************************************\
   * Fun02 Sec02 Sub02:
   *   - prefix for output
   \*****************************************************/

   fprintf(
      (FILE *) outFILE,
      "  -prefix out: [Optional]%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "    o prefix to call the output files%s",
      str_endLine
   );

   /*****************************************************\
   * Fun02 Sec02 Sub03:
   *   - min percent score
   \*****************************************************/

   fprintf(
      (FILE *) outFILE,
      "  -perc-score %f: [Optional]%s",
      def_minPercScore_mainDemux,
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "    o minimum percent score to keep match%s",
      str_endLine
   );
   fprintf(
      (FILE *) outFILE,
      "    o this is the initial filter step,%s",
      str_endLine
   );
   fprintf(
      (FILE *) outFILE,
      "      so keep somewhat high to keep fast%s",
      str_endLine
   );

   /*****************************************************\
   * Fun02 Sec02 Sub04:
   *   - min score
   \*****************************************************/

   fprintf(
      (FILE *) outFILE,
      "  -score %i: [Optional]%s",
      def_minScore_mainDemux,
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "    o minimum score to keep match (0 for none)%s",
      str_endLine
   );

   /*****************************************************\
   * Fun02 Sec02 Sub05:
   *   - max splits
   \*****************************************************/

   fprintf(
      (FILE *) outFILE,
      "  -splits %i: [Optional]%s",
      def_maxSplits_mainDemux,
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "    o maximum number of barcodes to split in a%s",
      str_endLine
   );
   fprintf(
      (FILE *) outFILE,
      "      read before discarding%s",
      str_endLine
   );

   /*****************************************************\
   * Fun02 Sec02 Sub06:
   *   - min dist
   \*****************************************************/

   fprintf(
      (FILE *) outFILE,
      "  -dist %i: [Optional]%s",
      def_minDist_mainDemux,
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "    o minimum distance between barcodes to not%s",
      str_endLine
   );
   fprintf(
      (FILE *) outFILE,
      "      discard a read%s",
      str_endLine
   );

   /*****************************************************\
   * Fun02 Sec02 Sub07:
   *   - trim
   \*****************************************************/

   if(def_trimBarcodes_mainDemux)
      fprintf(
         (FILE *) outFILE,
         "  -trim: [Yes]%s",
         str_endLine
      );
   else
      fprintf(
         (FILE *) outFILE,
         "  -trim: [No]%s",
         str_endLine
      );

   fprintf(
      (FILE *) outFILE,
      "    o remove barcodes from the sequence%s",
      str_endLine
   );
   fprintf(
      (FILE *) outFILE,
      "    o disable with -no-trim%s",
      str_endLine
   );

   /*****************************************************\
   * Fun02 Sec02 Sub08:
   *   - fastx files at end
   \*****************************************************/

   fprintf(
      (FILE *) outFILE,
      "  reads.fastq: [at least one Required]%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "    o fastq files with reads to demultiplex%s",
      str_endLine
   );
   fprintf(
      (FILE *) outFILE,
      "    o input: *.fastq, *.fastq.gz, *.fq, *.fq.gz%s",
      str_endLine
   );

   /*****************************************************\
   * Fun02 Sec02 Sub09:
   *   - help and version
   \*****************************************************/

   fprintf(
      (FILE *) outFILE,
      "  -h: print this help message and exit%s",
      str_endLine
   );
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
   ^   - output
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   fprintf((FILE *) outFILE, "Output:%s", str_endLine);

   fprintf(
      (FILE *) outFILE,
      "  prefix-suffix.fq:%s",
      str_endLine
   );
   fprintf(
      (FILE *) outFILE,
      "    o fastq files with demultiplexed reads%s",
      str_endLine
   );
   fprintf(
      (FILE *) outFILE,
      "    o the suffix is the header of the barcodes%s",
      str_endLine
   );
} /*phelp_mainDemux*/

/*-------------------------------------------------------\
| Fun03: input_mainDemux
|   - get user input from the aguments array
| Input:
|   - argLenSI:
|     o number of input arguments
|   - argAryStr:
|     o c-string array with user input
|   - barFileStrPtr:
|     o c-string pointer to get the barcodes fasta file
|   - prefixStrPtr:
|     o c-string pointer to get the prefix for the output
|       file
|   - splitSIPtr:
|     o signed int pointer to get maximum splits to do
|   - distSIPtr:
|     o signed int pointer to get minimum distance
|   - trimBlPtr:
|     o signed char pointer to get if trimming
|   - minScoreSLPtr:
|     o singed long pointer to get the minimum score 
|   - minPercScoreFPtr:
|     o float pointer to get the minimum percent score 
| Output:
|   - Prints:
|     o help message to outFILE
|   - Returns:
|     o index of first fastq file to read in
|     o -1 if printed help message or version number
|     o -2 if had input error
\-------------------------------------------------------*/
signed int
input_mainDemux(
   signed int argLenSI,
   char *argAryStr[],
   signed char **barFileStrPtr, /*barcode fasta file*/
   signed char **prefixStrPtr,   /*output file name*/
   signed int *splitSIPtr,      /*max splits to do*/
   signed int *distSIPtr,       /*minimum distance*/
   signed char *trimBlPtr,      /*set to if trim*/
   signed long *minScoreSLPtr,  /*minimum score*/
   float *minPercScoreFPtr      /*minimum percent score*/
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun03 TOC:
   '   - get user input from the aguments array
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec01:
   ^   - variable declarations and check no input case
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
   ^     - get input and output files + start loop
   ^   o fun03 sec02 sub02:
   ^     - get percent mininmum score
   ^   o fun03 sec02 sub02:
   ^     - get percent mininmum score
   ^   o fun03 sec02 sub03:
   ^     - get mininmum score
   ^   o fun03 sec02 sub04:
   ^     - get maximum splits to do
   ^   o fun03 sec02 sub05:
   ^     - get minimum distance after barcodes
   ^   o fun03 sec02 sub06:
   ^     - check if trimming reads
   ^   o fun03 sec02 sub07:
   ^     - help message checks
   ^   o fun03 sec02 sub08:
   ^     - verision number print checks
   ^   o fun03 sec02 sub09:
   ^     - check if on fastq files or if invalid input
   ^   o fun03 sec02 sub10:
   ^     - move to the next argument
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun03 Sec02 Sub01:
   *   - get input and output files + start loop
   \*****************************************************/

   while(siArg < argLenSI)
   { /*Loop: get user input*/
      if(
         ! eqlNull_ulCp(
            (signed char *) "-bar",
            (signed char *) argAryStr[siArg]
         )
      ){ /*If: barcode fasta file input*/
         ++siArg;
         *barFileStrPtr=(signed char *) argAryStr[siArg];
      }  /*If: barcode fasta file input*/

      else if(
         ! eqlNull_ulCp(
            (signed char *) "-prefix",
            (signed char *) argAryStr[siArg]
         )
      ){  /*Else If: get fasta file with barcodes*/
         ++siArg;
         *prefixStrPtr = (signed char *) argAryStr[siArg];
      }  /*Else If: get fasta file with barcodes*/

      /**************************************************\
      * Fun03 Sec02 Sub02:
      *   - get percent mininmum score
      \**************************************************/

      else if(
         ! eqlNull_ulCp(
            (signed char *) "-perc-score",
            (signed char *) argAryStr[siArg]
         )
      ){  /*Else If: get percent minumum score*/
         ++siArg;
         tmpStr = (signed char *) argAryStr[siArg];
         tmpStr +=
            strToF_base10str(tmpStr, minPercScoreFPtr);

         if(*tmpStr)
         { /*If; non-numeric or to large*/
            fprintf(
               stderr,
               "-perc-score %s; non-numeric/to large%s",
               argAryStr[siArg],
               str_endLine
            );
            goto err_fun03_sec03;
         } /*If; non-numeric or to large*/

         else if(
               *minPercScoreFPtr > 1
            && *minPercScoreFPtr <= 100
         ) *minPercScoreFPtr /= 100;

         else
         { /*Else: not a percentage*/
            fprintf(
               stderr,
               "-perc-score %s is > 100%s",
               argAryStr[siArg],
               str_endLine
            );
            goto err_fun03_sec03;
         } /*Else: not a percentage*/
      }  /*Else If: get percent minimum score*/

      /**************************************************\
      * Fun03 Sec02 Sub03:
      *   - get mininmum score
      \**************************************************/

      else if(
         ! eqlNull_ulCp(
            (signed char *) "-score",
            (signed char *) argAryStr[siArg]
         )
      ){  /*Else If: get minimum score*/
         ++siArg;
         tmpStr = (signed char *) argAryStr[siArg];
         tmpStr+=strToSL_base10str(tmpStr, minScoreSLPtr);

         if(*tmpStr)
         { /*If; non-numeric or to large*/
            fprintf(
               stderr,
               "-score %s is non-numeric or to large%s",
               argAryStr[siArg],
               str_endLine
            );
            goto err_fun03_sec03;
         } /*If; non-numeric or to large*/
      }  /*Else If: get minimum score*/

      /**************************************************\
      * Fun03 Sec02 Sub04:
      *   - get maximum splits to do
      \**************************************************/

      else if(
         ! eqlNull_ulCp(
            (signed char *) "-splits",
            (signed char *) argAryStr[siArg]
         )
      ){  /*Else If: maximum splits to do*/
         ++siArg;
         tmpStr = (signed char *) argAryStr[siArg];
         tmpStr += strToSI_base10str(tmpStr, splitSIPtr);

         if(*tmpStr)
         { /*If; non-numeric or to large*/
            fprintf(
               stderr,
               "-splits %s is non-numeric or to large%s",
               argAryStr[siArg],
               str_endLine
            );
            goto err_fun03_sec03;
         } /*If; non-numeric or to large*/
      }  /*Else If: maximum splits to do*/

      /**************************************************\
      * Fun03 Sec02 Sub05:
      *   - get minimum distance after barcodes
      \**************************************************/

      else if(
         ! eqlNull_ulCp(
            (signed char *) "-dist",
            (signed char *) argAryStr[siArg]
         )
      ){  /*Else If: get minimum distance*/
         ++siArg;
         tmpStr = (signed char *) argAryStr[siArg];
         tmpStr += strToSI_base10str(tmpStr, distSIPtr);

         if(*tmpStr)
         { /*If; non-numeric or to large*/
            fprintf(
               stderr,
               "-dist %s is non-numeric or to large%s",
               argAryStr[siArg],
               str_endLine
            );
            goto err_fun03_sec03;
         } /*If; non-numeric or to large*/
      }   /*Else If: get minimum distance*/

      /**************************************************\
      * Fun03 Sec02 Sub06:
      *   - check if trimming reads
      \**************************************************/

      else if(
         ! eqlNull_ulCp(
            (signed char *) "-trim",
            (signed char *) argAryStr[siArg]
         )
      ) *trimBlPtr = 1;

      else if(
         ! eqlNull_ulCp(
            (signed char *) "-no-trim",
            (signed char *) argAryStr[siArg]
         )
      ) *trimBlPtr = 0;

      /**************************************************\
      * Fun03 Sec02 Sub07:
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
      * Fun03 Sec02 Sub08:
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
      * Fun03 Sec02 Sub09:
      *   - check if on fastq files or if invalid input
      \**************************************************/

      else
      { /*Else: invalid input or fastq file*/
         inFILE = fopen(argAryStr[siArg], "r");

         if(inFILE)
         { /*If: is a fastq file at end*/
            fclose(inFILE);
            inFILE = 0;
            goto ret_fun03_sec03;
         } /*If: is a fastq file at end*/

         else
         { /*Else: invalid input*/
            fprintf(
             stderr,
             "%s is not recognized or not a fastq file%s",
             argAryStr[siArg],
             str_endLine
            );

            goto err_fun03_sec03;
         } /*Else: invalid input*/
      } /*Else: invalid input or fastq file*/

      /**************************************************\
      * Fun03 Sec02 Sub10:
      *   - move to the next argument
      \**************************************************/

      ++siArg;
   } /*Loop: get user input*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec03:
   ^   - return result
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   goto ret_fun03_sec03;

   phelp_fun03_sec03:;
      phelp_mainDemux(stdout);
      siArg = -1;
      goto ret_fun03_sec03;
   pversion_fun03_sec03:;
      pversion_mainDemux(stdout);
      siArg = -1;
      goto ret_fun03_sec03;
   err_fun03_sec03:;
      siArg = -2;
      goto ret_fun03_sec03;

   ret_fun03_sec03:;
      if(! inFILE) ;
      else if(inFILE == stdin) ;
      else if(inFILE == stdout) ;
      else if(inFILE == stderr) ;
      else fclose(inFILE);
      inFILE = 0;

      return siArg;
} /*input_mainDemux*/

/*-------------------------------------------------------\
| Main:
|   - driver function for demux
| Input:
|   - argLenSI:
|     o number of arguments the user input
|   - argAryStr:
|     o c-string array with user input
| Output:
|   - Prints:
|     o demuxed reads to output fastq files
|     o errors to stderr
|   - Returns:
|     o 0 for no errors
|     o 1 for an errors
\-------------------------------------------------------*/
signed int
main(
   signed int argLenSI,
   char *argAryStr[]
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Main TOC:
   '   - driver function for demux
   '   o main sec01:
   '     - variable declarations
   '   o main sec02:
   '     - initialiaze, get input, and open files
   '   o main sec03:
   '     - demux the fastq files
   '   o main sec04:
   '     - clean up and return
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*___________________user_input______________________*/
   signed char *prefixStr = (signed char *) "out";
   signed char *barcodesFileStr = 0;

   signed int fqFileSI = 0;
   signed int splitSI = def_maxSplits_mainDemux;
   signed int distSI = def_minDist_mainDemux;
   signed char trimBl = def_trimBarcodes_mainDemux;

   signed long minScoreSL = def_minScore_mainDemux;
   float minPercScoreF = def_minPercScore_mainDemux;

   /*______________fastrq_reading_variables_____________*/
   struct file_inflate fileStackST;
   signed char fxTypeSC = 0;
   signed long seqSL = 0;
   FILE *inFILE = 0;

   signed char *tmpStr = 0;
   signed char stdoutBl = 0;
   signed char tmpFileStr[1024];

   /*______________kmer_find_variables__________________*/
   signed char errSC = 0;

   unsigned char kmerLenUC = def_lenKmer_kmerFind;
   float extraNtF = def_extraNtInWin_kmerFind;
   float winShiftF = def_percShift_kmerFind;
   float kmerPercF = def_minKmerPerc_kmerFind;

   struct refST_kmerFind *barHeapAryST = 0;
   signed int barLenSI;

   struct tblST_kmerFind tblStackST;
   struct seqST seqStackST;

   struct alnSet alnStackST;

   /*______________demuxing_variables___________________*/
   signed int *coordHeapArySI = 0;
   signed int coordLenSI = 0;
   signed int siCoord = 0;

   signed char **outFileHeapStrAry = 0;
   FILE *logFILE = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec02:
   ^   - initialiaze, get input, and open files
   ^   o main sec02 sub01:
   ^     - initialize structures
   ^   o main sec02 sub02:
   ^     - get user input
   ^   o main sec02 sub03:
   ^     - setup structure memory
   ^   o main sec02 sub04:
   ^     - open files
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Main Sec02 Sub01:
   *   - initialize structures
   \*****************************************************/

   init_file_inflate(&fileStackST);
   init_tblST_kmerFind(&tblStackST);
   init_alnSet(&alnStackST);
   init_seqST(&seqStackST);

   /*****************************************************\
   * Main Sec02 Sub02:
   *   - get user input
   \*****************************************************/

   fqFileSI =
      input_mainDemux(
         argLenSI,
         argAryStr,
         &barcodesFileStr,
         &prefixStr,
         &splitSI,
         &distSI,
         &trimBl,
         &minScoreSL,
         &minPercScoreF
      );

   if(fqFileSI < 0)
   { /*If: printed help, version, or had error*/
      ++fqFileSI; /*convert help/version to 0 (no error)*/
      goto ret_main_sec04;
   } /*If: printed help, version, or had error*/

   /*****************************************************\
   * Main Sec02 Sub03:
   *   - setup structure memory
   \*****************************************************/

   if(setup_tblST_kmerFind(&tblStackST, kmerLenUC))
   { /*If: memory error*/
      fprintf(stderr,"setup memory error%s",str_endLine);
      goto memErr_main_sec04;
   } /*If: memory error*/

   /*****************************************************\
   * Main Sec02 Sub04:
   *   - open files
   *   o main sec02 sub04 cat01:
   *     - read in the barcodes
   \*****************************************************/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Main Sec02 Sub04 Cat01:
   +   - read in the barcodes
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   barHeapAryST =
      faToAry_refST_kmerFind(
         barcodesFileStr,
         kmerLenUC,
         &barLenSI,
         kmerPercF,
         &tblStackST,
         extraNtF,
         winShiftF,
         &alnStackST,
         &errSC
      );

   if(errSC)
   { /*If: had an error*/
      if(errSC == def_memErr_kmerFind)
      { /*If: had a memory error*/
         fprintf(
            stderr,
            "memory error reading in barcodes%s",
            str_endLine
         );
         goto memErr_main_sec04;
      } /*If: had a memory error*/

      else
      { /*Else: file error*/
         fprintf(
            stderr,
            "file error reading -bar %s%s",
            barcodesFileStr,
            str_endLine
         );
         goto fileErr_main_sec04;
      } /*Else: file error*/
   } /*If: had an error*/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Main Sec02 Sub04 Cat02:
   +   - open the output and log files
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   /*_________allocate_memory_and_initialize____________*/
   outFileHeapStrAry =
      malloc((barLenSI) * sizeof(outFileHeapStrAry));
   if(! outFileHeapStrAry)
   { /*If: memory error*/
      fprintf(stderr, "ran out of memory%s", str_endLine);
      goto memErr_main_sec04;
   } /*If: memory error*/

   for(seqSL = 0; seqSL < barLenSI; ++seqSL)
      outFileHeapStrAry[seqSL] = 0;

   /*_________open_all_the_output_files_________________*/
   for(seqSL = 0; seqSL < barLenSI; ++seqSL)
   { /*Loop: open output files*/
      outFileHeapStrAry[seqSL] =
         malloc(128 * sizeof(signed char *));
      if(! outFileHeapStrAry[seqSL])
      { /*If: memory error*/
         fprintf(
             stderr,
             "ran out of memory%s",
             str_endLine
          );
         goto memErr_main_sec04;
      } /*If: memory error*/

      tmpStr = outFileHeapStrAry[seqSL];
      tmpStr += cpWhite_ulCp(tmpStr, prefixStr);
      *tmpStr++ = '-';
      tmpStr +=
         cpWhite_ulCp(
            tmpStr,
            barHeapAryST[seqSL].forSeqST->idStr
         );
      *tmpStr++ = '.';
      *tmpStr++ = 'f';
      *tmpStr++ = 'q';
      *tmpStr = 0;

      inFILE =
         fopen((char *) outFileHeapStrAry[seqSL], "w");

      if(! inFILE)
      { /*If: could not open the file*/
         fprintf(
            stderr,
            "could not open %s for output%s",
            outFileHeapStrAry[seqSL],
            str_endLine
         );
         goto fileErr_main_sec04;
      } /*If: could not open the file*/

      fclose(inFILE);
      inFILE = 0;
   } /*Loop: open output files*/

   /*_________open_the_log_file_________________________*/
   tmpStr = tmpFileStr;
   tmpStr += cpWhite_ulCp(tmpStr, prefixStr);
   *tmpStr++ = '-';
   *tmpStr++ = 'l';
   *tmpStr++ = 'o';
   *tmpStr++ = 'g';
   *tmpStr++ = '.';
   *tmpStr++ = 't';
   *tmpStr++ = 's';
   *tmpStr++ = 'v';
   *tmpStr = 0;

   logFILE = fopen((char *) tmpFileStr, "w");
   if(! logFILE)
   { /*If: could not open the file*/
      fprintf(
         stderr,
         "could not open log file (%s) for output%s",
         tmpFileStr,
         str_endLine
      );

      goto fileErr_main_sec04;
   } /*If: could not open the file*/

   fprintf(logFILE, "id\tlen\tnum_barcodes\tstatus");
   fprintf(logFILE, "\tid_1\tstart_1\tend_1\tscore_1\t*");
   fprintf(logFILE, "%s", str_endLine);

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec03:
   ^   - demux the fastq files
   ^   o main sec03 sub01:
   ^     - open fastq file for reading
   ^   o main sec03 sub02:
   ^     - find barcode coordiantes for the read
   ^   o main sec03 sub03:
   ^     - demux the read
   ^   o main sec03 sub04:
   ^     - move to next read
   ^   o main sec03 sub05:
   ^     - move to next fastq file
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Main Sec03 Sub01:
   *   - open fastq file for reading
   \*****************************************************/

   while(fqFileSI < argLenSI)
   { /*Loop: go through all fastq files*/
      if(
            argAryStr[fqFileSI][0] == '-'
         && ! argAryStr[fqFileSI][1] 
      ){ /*If: stdin input*/
         if(stdoutBl)
            goto nextFile_sec03_sub04;
         stdoutBl = 1;
         inFILE = stdin;
      }  /*If: stdin input*/

      else
      { /*Else: open a fastq file*/
         inFILE = fopen(argAryStr[fqFileSI], "r");
         if(! inFILE)
            goto fqFileErr_main_sec04;
      } /*Else: open a fastq file*/

      errSC =
         get_gzSeqST(
            &fileStackST,
            &fxTypeSC,
            &seqStackST,
            inFILE
         );
      inFILE = 0; /*pointer is in fileStackST structure*/
      seqSL = 1;

      /**************************************************\
      * Main Sec03 Sub02:
      *   - find barcode coordiantes for the read
      \**************************************************/

      while(! errSC)
      { /*Loop: demux the sequences*/
         coordHeapArySI =
            barcodeCoords_demux(
               &coordLenSI, /*gets # of mapped barcodes*/
               &seqStackST, /*has read to demux*/
               barHeapAryST,/*has barcodes*/
               barLenSI,    /*number of barcodes*/
               &tblStackST, /*for kmerFind*/
               minPercScoreF,/*minimum perc score*/
               minScoreSL,  /*minimum score to keep map*/
               &alnStackST
            );

         cpWhite_ulCp(tmpFileStr, seqStackST.idStr);
         fprintf(
            logFILE,
            "%s\t%li\t%i",
            tmpFileStr,
            seqStackST.seqLenSL,
            coordLenSI >> 2
         );

         if(coordLenSI <= 0)
         { /*If: had no coords or error*/
            if(! coordLenSI)
               fprintf(
                  logFILE,
                  "\tno-barcodes\tNA%s",
                  str_endLine
               );
            else if(coordLenSI > -2)
               fprintf(
                  logFILE,
                  "\toverlap-barcodes\tNA%s",
                  str_endLine
               );
            else
            { /*Else: memory error*/
               fprintf(
                  logFILE,
                  "\tmemory-error\tNA%s",
                  str_endLine
               );
               goto memErr_main_sec04;
            } /*Else: memory error*/

            goto getNextSeq_main_sec03_sub03;
         } /*If: had no coords or error*/

         if(coordLenSI / 4 > splitSI)
         { /*If: to many barcodes*/
            fprintf(logFILE, "\tto-many-barcodes");
            goto pcoords_main_sec03_sub03;
         } /*If: to many barcodes*/

         /***********************************************\
         * Main Sec03 Sub03:
         *   - demux the read
         \***********************************************/

         errSC =
            read_demux(
               &seqStackST,
               splitSI,
               distSI,
               trimBl,
               coordHeapArySI,
               coordLenSI,
               outFileHeapStrAry
            );

         if(errSC == 3)
            fprintf(logFILE, "\tbarcodes-at-both-ends");
         else if(errSC == 4)
            fprintf(
               logFILE,
                "\tbarcodes-next-to-each-other"
            );
         else if(errSC)
            fprintf(logFILE, "\tdemux-error");
         else
            fprintf(logFILE, "\tkept");

         pcoords_main_sec03_sub03:;
            for(siCoord=0; siCoord<coordLenSI; siCoord+=4)
            { /*Loop: print out the coordinates*/
               cpWhite_ulCp(
                  tmpFileStr,
                  barHeapAryST[
                    coordHeapArySI[siCoord]
                  ].forSeqST->idStr
               );
               fprintf(
                  logFILE,
                  "\t%s\t%i\t%i\t%i",
                  tmpFileStr, /*barcode assigned*/
                  coordHeapArySI[siCoord+1],/*start*/
                  coordHeapArySI[siCoord+2],/*end*/
                  coordHeapArySI[siCoord+3] /*score*/
               );
            } /*Loop: print out the coordinates*/

            fprintf(logFILE, "\t*%s", str_endLine);

         /***********************************************\
         * Main Sec03 Sub04:
         *   - move to next read
         \***********************************************/

         getNextSeq_main_sec03_sub03:;
            errSC =
               get_gzSeqST(
                  &fileStackST,
                  &fxTypeSC,
                  &seqStackST,
                  0
               );
            ++seqSL;

            if(coordHeapArySI)
               free(coordHeapArySI);
            coordHeapArySI = 0;
      } /*Loop: demux the sequences*/

      /**************************************************\
      * Main Sec03 Sub05:
      *   - move to next fastq file
      \**************************************************/

      if(errSC != def_EOF_seqST)
         goto fqFileErr_main_sec04;

      nextFile_sec03_sub04:;
         ++fqFileSI;
	} /*Loop: go through all fastq files*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec04:
   ^   - clean up and return
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   fqFileSI = 0;
   goto ret_main_sec04;

   memErr_main_sec04:;
      fqFileSI = 1;
      goto ret_main_sec04;

   fileErr_main_sec04:;
      fqFileSI = 2;
      goto ret_main_sec04;

   fqFileErr_main_sec04:;
      fprintf(
         stderr,
         "could not open %s or problem on read %li%s",
         argAryStr[fqFileSI],
         seqSL,
         str_endLine
      );
      fqFileSI = 3;
      goto ret_main_sec04;

   ret_main_sec04:;
      freeStack_file_inflate(&fileStackST);
      freeStack_seqST(&seqStackST);
      freeStack_tblST_kmerFind(&tblStackST);
      freeStack_alnSet(&alnStackST);

      if(coordHeapArySI)
         free(coordHeapArySI);
      coordHeapArySI = 0;

      if(barHeapAryST)
        freeHeapAry_refST_kmerFind(barHeapAryST,barLenSI);
      barHeapAryST = 0;

      if(outFileHeapStrAry)
      { /*If: have string array to free*/
         for(distSI = 0; distSI < barLenSI; ++distSI)
         { /*Loop: free strings*/
            if(outFileHeapStrAry[distSI])
               free(outFileHeapStrAry[distSI]);
             outFileHeapStrAry[distSI] = 0;
         } /*Loop: free strings*/

         free(outFileHeapStrAry);
         outFileHeapStrAry = 0;
      } /*If: have string array to free*/

      if(! inFILE) ;
      else if(inFILE == stdin) ;
      else if(inFILE == stdout) ;
      else if(inFILE == stderr) ;
      else fclose(inFILE);
      inFILE = 0; 

      if(! logFILE) ;
      else if(logFILE == stdin) ;
      else if(logFILE == stdout) ;
      else if(logFILE == stderr) ;
      else fclose(logFILE);
      logFILE = 0; 

      return fqFileSI;
} /*main*/
