/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' mainNeedle SOF: Start Of File
'   - does a using a waterman smith aligment
'   o header:
'     - included libraries
'   o fun01: pversion_mainNeedle
'     - prints version number for mainNeedle
'   o fun02: phelp_mainNeedle
'     - prints help message for mainNeedle
'   o fun03: input_mainNeedle
'     - gets user input
'   o main:
'     - driver function to do a waterman alignment
'   o license:
'     - Licensing for this code (public domain / mit)
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

#include "../genLib/charCp.h"
#include "../genLib/base10str.h"

#include "../genBio/kmerCnt.h"
#include "../genBio/seqST.h"
#include "../genBio/samEntry.h"

#include "../genAln/alnSet.h"
#include "../genAln/dirMatrix.h"
#include "../genAln/needle.h"

/*.h files only*/
#include "../genLib/dataTypeShortHand.h"
#include "../genAln/alnDefs.h"
#include "../bioTools.h"

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\
! Hidden files
!   o .c  #include "../genLib/ulCp.h"
!   o .c  #include "../genLib/numToStr.h"
!   o .c  #include "../genLib/shellSort.h"
!   o .c  #include "../genLib/strAry.h"
!   o .c  #include "../genAln/indexToCoord.h"
!   o .h  #include "../genLib/genMath.h" (not using .c)
!   o .h  #include "../genBio/ntTo2Bit.h"
!   o .h  #include "../genBio/ntTo5Bit.h"
\%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

#define def_fqFile_mainNeedle 0
#define def_faFile_mainNeedle 1

#define def_lenKmer_mainNeedle 5
#define def_kmerScan_mainNeedle 0 /*scan for kmers*/

/*-------------------------------------------------------\
| Fun01: pversion_mainNeedle
|   - prints version number for mainNeedle
| Input:
|   - outFILE:
|     o file to print version number to
| Output:
|   - Prints:
|     o version number to outFILE
\-------------------------------------------------------*/
void
pversion_mainNeedle(
   void *outFILE
){
   fprintf(
      (FILE *) outFILE,
      "alnNeedle from bioTools version: %i-%02i-%02i\n",
      def_year_bioTools,
      def_month_bioTools,
      def_day_bioTools
   );
} /*pversion_mainNeedle*/

/*-------------------------------------------------------\
| Fun02: phelp_mainNeedle
|   - prints help message for mainNeedle
| Input:
|   - outFILE:
|     o file to print help message to
| Output:
|   - Prints:
|     o help message to outFILE
\-------------------------------------------------------*/
void
phelp_mainNeedle(
   void *outFILE
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun02 TOC:
   '   - prints help message for mainNeedle
   '   o fun02 sec01:
   '     - print usage block
   '   o fun02 sec02:
   '     - print input block
   '   o fun02 sec03:
   '     - print output block
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun02 Sec01:
   ^   - print usage block
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   fprintf(
      (FILE *) outFILE,
      "mainNeedle -qry query.fa -ref ref.fa > out.aln\n"
   );

   fprintf(
      (FILE *) outFILE,
      "  - does waterman alignment on two sequences\n"
   );

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun02 Sec02:
   ^   - print input block
   ^   o fun02 sec02 sub01:
   ^     - input block header
   ^   o fun02 sec02 sub02:
   ^     - reference sequence options
   ^   o fun02 sec02 sub03:
   ^     - query sequence options
   ^   o fun02 sec02 sub04:
   ^     - output file
   ^   o fun02 sec02 sub05:
   ^     - alignment settings
   ^   o fun02 sec02 sub06:
   ^     - help/version number
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
   *   - reference sequence options
   \*****************************************************/

   fprintf(
      (FILE *) outFILE,
      "  -ref reference.fasta: [Required]\n"
   );

   fprintf(
      (FILE *) outFILE,
      "    o fasta file with reference sequence\n"
   );

   fprintf(
      (FILE *) outFILE,
      "  -ref-fq reference.fastq: [Replaces -ref]\n"
   );

   fprintf(
      (FILE *) outFILE,
      "    o fastq file with reference sequence\n"
   );

   /*****************************************************\
   * Fun02 Sec02 Sub03:
   *   - query sequence options
   \*****************************************************/

   fprintf(
      (FILE *) outFILE,
      "  -qry reference.fasta: [Required]\n"
   );

   fprintf(
      (FILE *) outFILE,
      "    o fasta file with query sequence(s)\n"
   );

   fprintf(
      (FILE *) outFILE,
      "  -qry-fq query.fastq: [Replaces -qry]\n"
   );

   fprintf(
      (FILE *) outFILE,
      "    o fastq file with query sequence(s)\n"
   );

   /*****************************************************\
   * Fun02 Sec02 Sub04:
   *   - output file
   \*****************************************************/

   fprintf(
      (FILE *) outFILE,
      "  -out out.aln: [stdout]\n"
   );

   fprintf(
      (FILE *) outFILE,
      "    o file to print alignment to\n"
   );

   fprintf(
      (FILE *) outFILE,
      "    o use \"-out -\" for stdout\n"
   );

   /*****************************************************\
   * Fun02 Sec02 Sub05:
   *   - alignment settings
   *   o fun02 sec02 sub05 cat01:
   *     - kmer scanning
   *   o fun02 sec02 sub05 cat02:
   *     - gap penalties
   *   o fun02 sec02 sub05 cat03:
   *     - score and match matrixes
   \*****************************************************/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun02 Sec02 Sub05 Cat01:
   +   - kmer scanning
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   if(def_kmerScan_mainNeedle)
      fprintf(
         (FILE *) outFILE,
         "  -scan: [Optional; Yes]\n"
      );

   else
      fprintf(
         (FILE *) outFILE,
         "  -scan: [Optional; No]\n"
      );

   fprintf(
      (FILE *) outFILE,
      "    o scan for direction using kmers\n"
   );

   fprintf(
      (FILE *) outFILE,
      "    o reference and query need to be similar\n"
   );

   fprintf(
      (FILE *) outFILE,
      "      lengths\n"
   );

   fprintf(
      (FILE *) outFILE,
      "    o disable with \"-no-scan\"\n"
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun02 Sec02 Sub05 Cat02:
   +   - gap penalties
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "  -gap %i: [Optional]\n",
      def_gapOpen_alnDefs
   );

   fprintf(
      (FILE *) outFILE,
      "    o gap opening penalty\n"
   );

   fprintf(
      (FILE *) outFILE,
      "  -extend %i: [Optional]\n",
      def_gapExtend_alnDefs
   );

   fprintf(
      (FILE *) outFILE,
      "    o gap extension penalty\n"
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun02 Sec02 Sub05 Cat03:
   +   - score and match matrixes
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "  -score scoring-matrix.txt: [Optional]\n"
   );

   fprintf(
      (FILE *) outFILE,
      "    o scoring matrix (alnDefs.h has defaults)\n"
   );

   fprintf(
      (FILE *) outFILE,
      "  -score match-matrix.txt: [Optional]\n"
   );

   fprintf(
      (FILE *) outFILE,
      "    o match matrix (alnDefs.h has defaults)\n"
   );

   /*****************************************************\
   * Fun02 Sec02 Sub06:
   *   - help/version number
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
      "  - prints alignment(s) to -out file or stdout\n"
   );
} /*phelp_mainNeedle*/

/*-------------------------------------------------------\
| Fun03: input_mainNeedle
|   - gets user input
| Input:
|   - numArgsSI:
|     o number of arguments the user input
|   - argAryStr:
|     o array of c-strings with user input
|   - refFileStrPtr:
|     o c-string pionter to point to reference file name
|   - refTypeSCPtr:
|     o pointer to signed char to hold if reference was
|       a fasta or fastq file
|   - revRefBlPtr:
|     o reverse complement the reference sequence
|   - qryFileStrPtr:
|     o c-string pionter to point to query file name
|   - qryTypeSCPtr:
|     o pointer to signed char to hold if query was
|       a fasta or fastq file
|   - revQryBlPtr:
|     o reverse complement the query sequence
|   - outFileStrPtr:
|     o c-string pionter to point to output file name
|   - kmerScanBl:
|     o 1: use kmer counting to find reference/direction
|     o 0: assume user knows what they are doing
|   - alnSetSTPtr:
|     o pointer to alnSet struct with alingment settings
| Output:
|   - Modifies:
|     o all input variables to hold/point to user input
|   - Returns:
|     o 0 for no errors
|     o 1 for help message/version number
|     o 2 for unkown input
\-------------------------------------------------------*/
schar
input_mainNeedle(
   int numArgsSI,
   char *argAryStr[],
   schar **refFileStrPtr,
   schar *refTypeSCPtr,
   schar **qryFileStrPtr,
   schar *qryTypeSCPtr,
   schar **outFileStrPtr,
   schar *kmerScanBl,
   struct alnSet *alnSetSTPtr
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun03 TOC:
   '   - gets user input
   '   o fun03 sec01:
   '     - variable declerations
   '   o fun03 sec02:
   '     - check if user input something
   '   o fun03 sec03:
   '     - get user input
   '   o fun03 sec04:
   '     - return
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec01:
   ^   - variable declerations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   ulong errUL = 0;
   schar errSC = 0;
   schar *errStr = 0;
   sint siArg = 1;

   FILE *matrixFILE = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec02:
   ^   - check if user input something
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(numArgsSI == 1)
   { /*If: nothing input*/
      phelp_mainNeedle(stdout);
      goto phelp_fun03_sec04;
   } /*If: nothing input*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec03:
   ^   - get user input
   ^   o fun03 sec03 sub01:
   ^     - start loop and check file input
   ^   o fun03 sec03 sub02:
   ^     - check alignment settings
   ^   o fun03 sec03 sub03:
   ^     - check if help message requested
   ^   o fun03 sec03 sub04:
   ^     - check if version number print
   ^   o fun03 sec03 sub05:
   ^     - invalid entry
   ^   o fun03 sec03 sub06:
   ^     - move to next entry
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun03 Sec03 Sub01:
   *   - start loop and check file input
   \*****************************************************/

   while(siArg < numArgsSI)
   { /*Loop: get user input*/
      if(
         ! eql_charCp(
            (schar *) "-ref",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*If: is reference (as fasta)*/
         ++siArg;
         *refFileStrPtr = (schar *) argAryStr[siArg];
         *refTypeSCPtr = def_faFile_mainNeedle;
      } /*If: is reference (as fasta)*/

      else if(
         ! eql_charCp(
            (schar *) "-ref-fq",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else If: is reference (as fastq)*/
         ++siArg;
         *refFileStrPtr = (schar *) argAryStr[siArg];
         *refTypeSCPtr = def_fqFile_mainNeedle;
      } /*Else If: is reference (as fastq)*/

      else if(
         ! eql_charCp(
            (schar *) "-qry",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else If: is query (as fasta)*/
         ++siArg;
         *qryFileStrPtr = (schar *) argAryStr[siArg];
         *qryTypeSCPtr = def_faFile_mainNeedle;
      } /*Else If: is query (as fasta)*/

      else if(
         ! eql_charCp(
            (schar *) "-qry-fq",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else If: is query (as fastq)*/
         ++siArg;
         *qryFileStrPtr = (schar *) argAryStr[siArg];
         *qryTypeSCPtr = def_fqFile_mainNeedle;
      } /*Else If: is query (as fastq)*/

      else if(
         ! eql_charCp(
            (schar *)"-out",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else If: is output file*/
         ++siArg;
         *outFileStrPtr = (schar *) argAryStr[siArg];
      } /*Else If: is output file*/

      /**************************************************\
      * Fun03 Sec03 Sub02:
      *   - check alignment settings
      *   o fun03 sec03 sub02 cat01:
      *     - check general settings
      *   o fun03 sec03 sub02 cat02:
      *     - check scoring matrix input
      *   o fun03 sec03 sub02 cat03:
      *     - check scoring match input
      \**************************************************/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun03 Sec03 Sub02 Cat01:
      +   - check general settings
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      else if(
         ! eql_charCp(
            (schar *) "-scan",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ) *kmerScanBl = 1;

      else if(
         ! eql_charCp(
            (schar *) "-no-scan",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ) *kmerScanBl = 0;

      else if(
         ! eql_charCp(
            (schar *) "-gap",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else If: is gap opening penalty*/
         ++siArg;

         errStr = (schar *) argAryStr[siArg];

         errStr +=
            strToSS_base10str(
               errStr,
               &alnSetSTPtr->gapSS
            );

         if(*errStr)
         { /*If: non-numeric*/
            fprintf(
               stderr,
               "-gap %s is non-numeric\n",
               argAryStr[siArg]
            );
               
            goto err_fun03_sec04;     
         } /*If: non-numeric*/

         changeGap_alnSet(
            alnSetSTPtr,
            alnSetSTPtr->gapSS,
            alnSetSTPtr->extendSS
         );
      } /*Else If: kmer scanning*/

      else if(
         ! eql_charCp(
            (schar *)"-extend",
            (schar *) argAryStr[siArg],
            (schar) '\0'
      )
      ){ /*Else If: is gap extension penalty*/
         ++siArg;

         errStr = (schar *) argAryStr[siArg];

         errStr +=
            strToSS_base10str(
               errStr,
               &alnSetSTPtr->extendSS
            );

         if(*errStr)
         { /*If: non-numeric*/
            fprintf(
               stderr,
               "-extend %s is non-numeric\n",
               argAryStr[siArg]
            );
               
            goto err_fun03_sec04;     
         } /*If: non-numeric*/

         changeGap_alnSet(
            alnSetSTPtr,
            alnSetSTPtr->gapSS,
            alnSetSTPtr->extendSS
         );
      } /*Else If: is gap extension penalty*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun03 Sec03 Sub02 Cat02:
      +   - check scoring matrix input
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      else if(
         ! eql_charCp(
            (schar *) "-score",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else If: is scoring matrix*/
         ++siArg;

         matrixFILE =
            fopen(
               argAryStr[siArg],
               "r"
           );

         if(! matrixFILE)
         { /*If: could not open scoring matrix*/
            fprintf(
               stderr,
               "unable to open -score %s\n",
               argAryStr[siArg]
            );

            goto err_fun03_sec04;     
         } /*If: could not open scoring matrix*/

         errUL =
            readScoreFile_alnSet(
               alnSetSTPtr,
               matrixFILE
            );

         fclose(matrixFILE);
         matrixFILE = 0;

         if(errUL)
         { /*If: invalid entry*/
            fprintf(
               stderr,
               "entry %lu in -score %s is invalid\n",
               errUL,
               argAryStr[siArg]
            );

            goto err_fun03_sec04;     
         } /*If: invalid entry*/
      } /*Else If: is scoring matrix*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun03 Sec03 Sub02 Cat03:
      +   - check match matrix input
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      else if(
         ! eql_charCp(
            (schar *) "-match",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else If: is match matrix*/
         ++siArg;

         matrixFILE =
            fopen(
               argAryStr[siArg],
               "r"
           );

         if(! matrixFILE)
         { /*If: could not open scoring matrix*/
            fprintf(
               stderr,
               "unable to open -match %s\n",
               argAryStr[siArg]
            );

            goto err_fun03_sec04;     
         } /*If: could not open scoring matrix*/

         errUL =
            readMatchFile_alnSet(
               alnSetSTPtr,
               matrixFILE
            );

         fclose(matrixFILE);
         matrixFILE = 0;

         if(errUL)
         { /*If: invalid entry*/
            fprintf(
               stderr,
               "entry %lu in -match %s is invalid\n",
               errUL,
               argAryStr[siArg]
            );

            goto err_fun03_sec04;     
         } /*If: invalid entry*/
      } /*Else If: is match matrix*/

      /**************************************************\
      * Fun03 Sec03 Sub03:
      *   - check if help message requested
      \**************************************************/
      
      else if(
         ! eql_charCp(
            (schar *) "-h",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else If: help message requested*/
         phelp_mainNeedle(stdout);
         goto pversion_fun03_sec04;
      } /*Else If: help message requested*/

      else if(
         ! eql_charCp(
            (schar *) "--h",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else If: help message requested*/
         phelp_mainNeedle(stdout);
         goto pversion_fun03_sec04;
      } /*Else If: help message requested*/

      else if(
         ! eql_charCp(
            (schar *) "help",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else If: help message requested*/
         phelp_mainNeedle(stdout);
         goto pversion_fun03_sec04;
      } /*Else If: help message requested*/

      else if(
         ! eql_charCp(
            (schar *) "-help",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else If: help message requested*/
         phelp_mainNeedle(stdout);
         goto pversion_fun03_sec04;
      } /*Else If: help message requested*/

      else if(
         ! eql_charCp(
            (schar *) "--help",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else If: help message requested*/
         phelp_mainNeedle(stdout);
         goto pversion_fun03_sec04;
      } /*Else If: help message requested*/

      /**************************************************\
      * Fun03 Sec03 Sub04:
      *   - check if version number print
      \**************************************************/

      else if(
         ! eql_charCp(
            (schar *) "-v",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else If: version number requested*/
         pversion_mainNeedle(stdout);
         goto pversion_fun03_sec04;
      } /*Else If: version number requested*/

      else if(
         ! eql_charCp(
            (schar *) "--v",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else If: version number requested*/
         pversion_mainNeedle(stdout);
         goto pversion_fun03_sec04;
      } /*Else If: version number requested*/

      else if(
         ! eql_charCp(
            (schar *) "version",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else If: version number requested*/
         pversion_mainNeedle(stdout);
         goto pversion_fun03_sec04;
      } /*Else If: version number requested*/

      else if(
         ! eql_charCp(
            (schar *) "-version",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else If: version number requested*/
         pversion_mainNeedle(stdout);
         goto pversion_fun03_sec04;
      } /*Else If: version number requested*/

      else if(
         ! eql_charCp(
            (schar *) "--version",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else If: version number requested*/
         pversion_mainNeedle(stdout);
         goto pversion_fun03_sec04;
      } /*Else If: version number requested*/

      /**************************************************\
      * Fun03 Sec03 Sub05:
      *   - invalid entry
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
      * Fun03 Sec03 Sub06:
      *   - move to next entry
      \**************************************************/

      ++siArg;
   } /*Loop: get user input*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec04:
   ^   - return
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*no errors*/
   errSC = 0;
   goto cleanUp_fun03_sec04;

   phelp_fun03_sec04:;
   errSC = 1;
   goto cleanUp_fun03_sec04;

   pversion_fun03_sec04:;
   errSC = 1;
   goto cleanUp_fun03_sec04;

   err_fun03_sec04:;
   errSC = 2;
   goto cleanUp_fun03_sec04;

   cleanUp_fun03_sec04:;

   if(
         matrixFILE
      && matrixFILE != stdin
      && matrixFILE != stdout
   ) fclose(matrixFILE);

   matrixFILE = 0;
   return errSC;
} /*input_mainNeedle*/

/*-------------------------------------------------------\
| Main:
|   - driver function to do a waterman alignment
| Input:
|   - numArgsSI:
|     o number of arguments the user input
|   - argAryStr:
|     o array of c-strings with user input
| Output:
|   - prints alignment as sam file
\-------------------------------------------------------*/

int
main(
   int numArgsSI,
   char *argAryStr[]
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Main TOC:
   '   - driver function to do a waterman alignment
   '   o main sec01:
   '     - variable declerations
   '   o main sec02:
   '     - initialize structs, get input, and check input
   '   o main sec03:
   '     - align sequences
   '   o main sec04:
   '     - clean up
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec01:
   ^   - variable declerations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   schar errSC = 0;
   slong scoreSL = 0;
   slong seqSL = 0;   /*query sequence on*/
   uint numAnonUI = 0; /*through away*/
   schar *tmpStr = 0;
   schar kmerScanBl = def_kmerScan_mainNeedle;
     /*scan for direction using kmers*/

   schar *qryFileStr = 0;
   schar qryTypeSC = def_fqFile_mainNeedle;

   schar *refFileStr = 0;
   schar refTypeSC = def_fqFile_mainNeedle;

   schar *outFileStr = 0;

   struct seqST qryStackST;
   struct seqST *refSTPtr = 0;
   struct dirMatrix matrixStackST;
   struct alnSet setStackST;

   struct samEntry samStackST;
   schar *buffHeapStr = 0;
   ulong lenBuffUL = 0;

   struct kmerCnt kmerStackST;
   uchar lenKmerUC = def_lenKmer_mainNeedle;
   uint maxKmerUI = 0;
   sint numKmersSI = 0;
   sint *kmerHeapArySI = 0;
   sint *cntHeapArySI = 0;

   FILE *seqFILE = 0;
   FILE *outFILE = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec02:
   ^   - initialize structures, get input, and check input
   ^   o main sec02 sub01:
   ^     - initialize structures
   ^   o main sec02 sub02:
   ^     - get input
   ^   o main sec02 sub03:
   ^     - allocate memory for kmer arrays
   ^   o main sec02 sub04:
   ^     - get first reference sequence
   ^   o main sec02 sub05:
   ^     - get first query sequence
   ^   o main sec02 sub06:
   ^     - open output file
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Main Sec02 Sub01:
   *   - initialize structures
   \*****************************************************/

   init_seqST(&qryStackST);
   init_kmerCnt(&kmerStackST);
   init_dirMatrix(&matrixStackST);
   init_alnSet(&setStackST);
   init_samEntry(&samStackST);

   maxKmerUI = 1;

   for(
      scoreSL = 0;
      scoreSL < lenKmerUC;
      ++scoreSL
   ) maxKmerUI <<= 2; /*find max number kmers in table*/

   /*****************************************************\
   * Main Sec02 Sub02:
   *   - get input
   \*****************************************************/

   errSC =
      input_mainNeedle(
         numArgsSI,
         argAryStr,
         &refFileStr,
         &refTypeSC,
         &qryFileStr,
         &qryTypeSC,
         &outFileStr,
         &kmerScanBl,
         &setStackST
      );

   if(errSC)
   { /*If: had input error*/
      --errSC;
      goto cleanUp_main_sec04_sub04;
   } /*If: had input error*/

   /*****************************************************\
   * Main Sec02 Sub03:
   *   - allocate array memory
   \*****************************************************/

   kmerHeapArySI = malloc((maxKmerUI + 1) * sizeof(sint));

   if(! kmerHeapArySI)
      goto memErr_main_sec04_sub02;

   cntHeapArySI = malloc((maxKmerUI + 1) * sizeof(sint));

   if(! cntHeapArySI)
      goto memErr_main_sec04_sub02;

   /*****************************************************\
   * Main Sec02 Sub04:
   *   - get reference sequence
   *   o main sec02 sub04 cat01:
   *     - open reference file
   *   o main sec02 sub04 cat02:
   *     - get reference sequence
   *   o main sec02 sub04 cat03:
   *     - deal with errors
   \*****************************************************/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Main Sec02 Sub04 Cat01:
   +   - open reference file
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   if(! refFileStr)
   { /*If: no reference file input*/
      fprintf(
         stderr,
         "no reference file provied with -ref\n"
      );

      goto fileErr_main_sec04_sub03;
   } /*If: no reference file input*/

   seqFILE =
      fopen(
         (char *) refFileStr,
         "r"
      );

   if(! seqFILE)
   { /*If: file error*/
      if(refTypeSC == def_fqFile_mainNeedle)
         fprintf( stderr,
            "could not open -ref-fq %s\n",
            refFileStr
         );

      else
         fprintf( stderr,
            "could not open -ref %s\n",
            refFileStr
         );

      goto fileErr_main_sec04_sub03;
   } /*If: file error*/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Main Sec02 Sub04 Cat02:
   +   - get reference sequence
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   setup_kmerCnt(
      &kmerStackST,
      lenKmerUC
   );

   if(refTypeSC == def_fqFile_mainNeedle)
   { /*If: reading from fastq file*/
      errSC =
         getFqSeq_seqST(
             seqFILE,
             kmerStackST.forSeqST
         ); /*read in reference sequence*/
   } /*If: reading from fastq file*/

   else
   { /*Else: from fasta file*/
      errSC =
         getFaSeq_seqST(
             seqFILE,
             kmerStackST.forSeqST
         ); /*read in reference sequence*/
   } /*Else: from fasta file*/

   if(
         ! errSC
      || errSC == def_EOF_seqST
   ){ /*If: had no problems*/
      tmpStr = (schar *) kmerStackST.forSeqST->idStr;
      while(*tmpStr++ > 32) ;
      *(tmpStr - 1) = '\0';

      if(kmerScanBl)
         errSC =
            addSeq_kmerCnt(
               &kmerStackST,
               kmerStackST.forSeqST
            );

      fclose(seqFILE);
      seqFILE = 0;
   } /*If: had no problems*/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Main Sec02 Sub04 Cat03:
   +   - deal with errors
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   else
   { /*Else: had an error*/
      if(errSC & def_memErr_seqST)
         fprintf(
            stderr,
            "MEMORY ERROR reading"
         );
         /*memory errors are same for seqST and kmerCnt*/

      else
         fprintf(
            stderr,
            "file error with"
         );

      if(refTypeSC == def_fqFile_mainNeedle)
         fprintf(
           stderr,
           " -ref-fq %s\n",
           refFileStr
         );

      else
         fprintf(
           stderr,
           " -ref %s\n",
           refFileStr
         );

      if(errSC & def_memErr_seqST)
         goto memErr_main_sec04_sub02;
      else
         goto fileErr_main_sec04_sub03;
   } /*Else: had an error*/

   /*****************************************************\
   * Main Sec02 Sub05:
   *   - get frist query sequence
   *   o main sec02 sub04 cat01:
   *     - open query file
   *   o main sec02 sub04 cat02:
   *     - get query sequence
   *   o main sec02 sub04 cat03:
   *     - deal with errors
   \*****************************************************/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Main Sec02 Sub04 Cat01:
   +   - open query file
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   if(! qryFileStr)
   { /*If: no query file input*/
      fprintf(
         stderr,
         "no query file provied with -qry\n"
      );

      goto fileErr_main_sec04_sub03;
   } /*If: no query file input*/

   seqFILE =
      fopen(
         (char *) qryFileStr,
         "r"
      );

   if(! seqFILE)
   { /*If: file error*/
      if(qryTypeSC == def_fqFile_mainNeedle)
         fprintf( stderr,
            "could not open -qry-fq %s\n",
            qryFileStr
         );

      else
         fprintf( stderr,
            "could not open -qry %s\n",
            qryFileStr
         );

      goto fileErr_main_sec04_sub03;
   } /*If: file error*/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Main Sec02 Sub04 Cat02:
   +   - get query sequence
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   if(qryTypeSC == def_fqFile_mainNeedle)
   { /*If: reading from fastq file*/
      errSC =
         getFqSeq_seqST(
             seqFILE,
             &qryStackST
         ); /*read in first query sequence*/
   } /*If: reading from fastq file*/

   else
   { /*Else: from fasta file*/
      errSC =
         getFaSeq_seqST(
             seqFILE,
             &qryStackST
         ); /*read in first query sequence*/
   } /*Else: from fasta file*/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Main Sec02 Sub04 Cat03:
   +   - deal with errors
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   if(
         errSC
      && errSC != def_EOF_seqST
   ){ /*If: had an error*/
      if(errSC & def_memErr_seqST)
         fprintf(
            stderr,
            "MEMORY ERROR reading"
         );

      else
         fprintf(
            stderr,
            "file error with"
         );

      if(qryTypeSC == def_fqFile_mainNeedle)
         fprintf(
           stderr,
           " -qry-fq %s\n",
           qryFileStr
         );

      else
         fprintf(
           stderr,
           " -qry %s\n",
           qryFileStr
         );

      if(errSC & def_memErr_seqST)
         goto memErr_main_sec04_sub02;
      else
         goto fileErr_main_sec04_sub03;
   } /*If: had an error*/

   /*****************************************************\
   * Main Sec02 Sub06:
   *   - open output file
   \*****************************************************/

   if(! outFileStr || *outFileStr == '-')
      outFILE = stdout;

   else
   { /*Else: output file input*/
      outFILE =
         fopen(
            (char *) outFileStr,
            "w"
         );

      if(! outFILE)
      { /*If: could not open output file*/
         fprintf(
            stderr,
            "unable to open -out %s\n",
            outFileStr
         );

         goto fileErr_main_sec04_sub03;
      } /*If: could not open output file*/
   } /*Else: output file input*/
  
   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec03:
   ^   - align sequences
   ^   o main sec03 sub01:
   ^     - print out header (sam file)
   ^   o main sec03 sub02:
   ^     - setup/start loop, aligned sequences
   ^   o main sec03 sub03:
   ^     - get alignment
   ^   o main sec03 sub04:
   ^     - print alignment
   ^   o main sec03 sub05:
   ^     - get next query sequence
   ^   o main sec03 sub06:
   ^     - finished alingments, check for errors
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Main Sec03 Sub01:
   *   - print out header (sam file)
   \*****************************************************/

   if(outFILE)
   { /*If: printing to sam file*/
      fprintf(
         outFILE,
         "@HD\tVN:1.6\tSO:unsorted\tGO:none\n"
      );

      fprintf(
         outFILE,
         "@SQ\tSN:%s\tLN:%lu\n",
         kmerStackST.forSeqST->idStr + 1, /*get off >*/
         kmerStackST.forSeqST->lenSeqUL
      );

      fprintf(
        outFILE,
        "@PG\tID:%s\tVN:bioTools_%i-%02i-%02i\t%s",
        "alnNeedle",
        def_year_bioTools,
        def_month_bioTools,
        def_day_bioTools,
        "alnNeedle"
     ); /*print out first part of program id tag*/

      for(
         seqSL = 1;
         seqSL < numArgsSI;
         ++seqSL
      ){ /*Loop: print user arguments*/
         fprintf(
            outFILE,
            " %s",
            argAryStr[seqSL]
         );
      } /*Loop: print user arguments*/

      seqSL = 0;

      fprintf(
         outFILE,
         "\n"
      );
   } /*If: printing to sam file*/

   /*****************************************************\
   * Main Sec03 Sub02:
   *   - setup/start loop, aligned sequences
   \*****************************************************/

   setup_samEntry(&samStackST); /*allocate memory*/

   seqToIndex_alnSet(kmerStackST.forSeqST->seqStr);

   if(kmerScanBl)
      seqToIndex_alnSet(kmerStackST.revSeqST->seqStr);
   else
       refSTPtr = kmerStackST.forSeqST;

   do
   { /*Loop: align all query sequences*/
      ++seqSL;

      if(kmerScanBl)
      { /*If: doing a kmer scan*/
         numKmersSI =
            ntToKmerAry_kmerCnt(
               &qryStackST,
               lenKmerUC,
               kmerHeapArySI,
               cntHeapArySI
            ); /*get kmers in sequence*/
               
         /*find the reference direction*/
         numKmersSI =
            get_kmerCnt(
               &kmerStackST,
               kmerHeapArySI,
               cntHeapArySI
            ); /*get number kmers shared between ref/qry*/
            
         /*find the direction*/
         if(numKmersSI < 0)
            refSTPtr = kmerStackST.revSeqST;
   
         else
            refSTPtr = kmerStackST.forSeqST;
      } /*If: doing a kmer scan*/

      seqToIndex_alnSet(qryStackST.seqStr);

      scoreSL =
         needle(
            &qryStackST,
            refSTPtr,
            &matrixStackST,
            &setStackST
         );

      if(! scoreSL)
      { /*If: memory error*/
         fprintf(
            stderr,
            "MEMORY ERROR aligning sequence %li\n",
            seqSL
         );

         goto memErr_main_sec04_sub02;
      } /*If: memory error*/

      /**************************************************\
      * Main Sec03 Sub03:
      *   - get alignment
      \**************************************************/

      errSC =
         getAln_dirMatrix(
            &matrixStackST,
            0,              /*use index in matrixStackST*/
            (numKmersSI < 0), /*direction of alignment*/
            &qryStackST,
            refSTPtr,
            &samStackST,
            &numAnonUI,    /*through away*/
            &setStackST
         ); /*get the alignment*/

      if(errSC)
      { /*If: memory error*/
         fprintf(
            stderr,
            "MEMORY ERROR sequence %li\n",
            seqSL
         );

         goto memErr_main_sec04_sub02;
      } /*If: memory error*/

      /**************************************************\
      * Main Sec03 Sub04:
      *   - print alignment
      \**************************************************/

      errSC =
         p_samEntry(
            &samStackST,
            &buffHeapStr,
            &lenBuffUL,
            0,
            outFILE
         );

      if(errSC)
      { /*If: memory error*/
         fprintf(
            stderr,
            "MEMORY ERROR sequence %li\n",
            seqSL
         );

         goto memErr_main_sec04_sub02;
      } /*If: memory error*/

      /**************************************************\
      * Main Sec03 Sub05:
      *   - get next query sequence
      \**************************************************/

      if(qryTypeSC == def_fqFile_mainNeedle)
      { /*If: reading from fastq file*/
         errSC =
            getFqSeq_seqST(
                seqFILE,
                &qryStackST
            ); /*read in next query sequence*/
      } /*If: reading from fastq file*/

      else
      { /*Else: from fasta file*/
         errSC =
            getFaSeq_seqST(
                seqFILE,
                &qryStackST
            ); /*read in next query sequence*/
      } /*Else: from fasta file*/
   } while(! errSC);
   /*Loop: align all query sequences*/

   /*****************************************************\
   * Main Sec03 Sub06:
   *   - finished alingments, check for errors
   \*****************************************************/

   if(errSC > 1)
   { /*If: memory error*/
      fprintf(
         stderr,
         "MEMORY ERROR on %li\n",
         seqSL
      );

      goto memErr_main_sec04_sub02;
   } /*If: memory error*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec04:
   ^   - clean up
   ^   o main sec04 sub01:
   ^     - no error cases
   ^   o main sec04 sub02:
   ^     - memory error cases
   ^   o main sec04 sub03:
   ^     - file error cases
   ^   o main sec04 sub04:
   ^     - clean up (error or no error)
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Main Sec04 Sub01:
   *   - no error cases
   \*****************************************************/

   goto cleanUp_main_sec04_sub04;

   /*****************************************************\
   * Main Sec04 Sub02:
   *   - memory error cases
   \*****************************************************/

   memErr_main_sec04_sub02:;
   errSC = 1;
   goto cleanUp_main_sec04_sub04;
   
   /*****************************************************\
   * Main Sec04 Sub03:
   *   - file error cases
   \*****************************************************/

   fileErr_main_sec04_sub03:;
   errSC = 2;
   goto cleanUp_main_sec04_sub04;

   /*****************************************************\
   * Main Sec04 Sub04:
   *   - clean up (error or no error)
   \*****************************************************/

   cleanUp_main_sec04_sub04:;

   if(
         seqFILE
      && seqFILE != stdin
      && seqFILE != stdout
   ) fclose(seqFILE);

   seqFILE = 0;

   if(
         outFILE
      && outFILE != stdin
      && outFILE != stdout
   ) fclose(outFILE);

   outFILE = 0;
     
   freeStack_seqST(&qryStackST);
   freeStack_kmerCnt(&kmerStackST);
   freeStack_dirMatrix(&matrixStackST);
   freeStack_alnSet(&setStackST);
   freeStack_samEntry(&samStackST);

   free(buffHeapStr);
   buffHeapStr = 0;

   free(kmerHeapArySI);
   kmerHeapArySI = 0;

   free(cntHeapArySI);
   cntHeapArySI = 0;

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
