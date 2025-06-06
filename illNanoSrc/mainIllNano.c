/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' mainIllNano SOF: Start Of File
'   - driver for illNano, which finds Nanopore reads
'     supporting Illumina variants
'   o header:
'     - included libraries and defined variables
'   o fun01: pversion_mainIllNano
'     - prints version number for illNano
'   o fun02: phelp_mainIllNano
'     - prints help message for illNano
'   o fun03: input_mainIllNano
'     - gets user input from array
'   o main:
'     - driver function for illNano
'   o license:
'     - licensing for this code (public domain / mit)
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
#include "../genBio/samEntry.h"
#include "../genClust/illNano.h"

/*.h files only*/
#include "../genLib/endLine.h"
#include "../bioTools.h"

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\
! Hidden libraries
!   o .c   #include "../genLib/numToStr.h"
!   o .c   #include "../genLib/fileFun.h"
!   o .h   #include "ntTo5Bit.h"
\%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

#define def_minDepth_mainIllNano 10
#define def_minPercDepth_mainIllNano 0.001f
#define def_minPercDel_mainIllNano 0.01f

#define def_transition_mainIllNano 0
   /*1: if unkown variant, assign to transition (if one)*/

#define def_partOverlapMerge_mainIllNano 0
   /*1: merge if overlaps are not complete*/

/*-------------------------------------------------------\
| Fun01: pversion_mainIllNano
|   - prints version number for illNano
| Input:
|   - outFILE:
|     o file to print version number to
| Output:
|   - Prints:
|     o version number to outFILE
\-------------------------------------------------------*/
void
pversion_mainIllNano(
   void *outFILE
){
   fprintf(
      (FILE *) outFILE,
      "illNano from bioTools version: %i-%02i-%02i%s",
      def_year_bioTools,
      def_month_bioTools,
      def_day_bioTools,
      str_endLine
   );
} /*pversion_mainIllNano*/

/*-------------------------------------------------------\
| Fun02: phelp_mainIllNano
|   - prints help message for illNano
| Input:
|   - outFILE:
|     o file to print help message to
| Output:
|   - Prints:
|     o help message to outFILE
\-------------------------------------------------------*/
void
phelp_mainIllNano(
   void *outFILE
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun02 TOC:
   '   o fun02 sec01:
   '     - print usage entry
   '   o fun02 sec02:
   '     - print input entry
   '   o fun02 sec03:
   '     - print ouput entry
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun02 Sec01:
   ^   - print usage entry
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   fprintf(
     (FILE *) outFILE,
     "illNano -ill-tsv variants.tsv -sam-ont reads.sam%s",
     str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "  - extracts read ids for ONT sequence reads%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "    that have Illumina variants%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "  - this is inteaded for sick (ill) nanopores%s",
      str_endLine
   );

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun02 Sec02:
   ^   - print input entry
   ^   o fun02 sec02 sub01:
   ^     - print input block header
   ^   o fun02 sec02 sub02:
   ^     - print file IO
   ^   o fun02 sec02 sub03:
   ^     - print settings
   ^   o fun02 sec02 sub04:
   ^     - help message and version number
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun02 Sec02 Sub01:
   *   - print input block header
   \*****************************************************/

   fprintf(
      (FILE *) outFILE,
      "Input:%s",
      str_endLine
   );

   /*****************************************************\
   * Fun02 Sec02 Sub02:
   *   - print file IO
   *   o fun02 sec02 sub02 cat01:
   *     - print file IO header
   *   o fun02 sec02 sub02 cat02:
   *     - print illumina tsv input
   *   o fun02 sec02 sub02 cat03:
   *     - print ONT sam file input
   *   o fun02 sec02 sub02 cat04:
   *     - print output file
   \*****************************************************/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun02 Sec02 Sub02 Cat01:
   +   - print file IO header
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "   FILE IO:%s",
      str_endLine
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun02 Sec02 Sub02 Cat02:
   +   - print illumina tsv input
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "     -ill-tsv variants.tsv: [Required]%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "       o tbCon variant tsv for Illumina reads%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "       o use \"-ill-tsv -\" for stdin input %s",
      str_endLine
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun02 Sec02 Sub02 Cat03:
   +   - print ONT sam file input
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "    -sam-ont reads.sam: [Required]%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "       o sam file with ONT reads mapped to same%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "         reference as -ill-tsv%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "       o use \"-sam-ont -\" for stdin input %s",
      str_endLine
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun02 Sec02 Sub02 Cat04:
   +   - print output file
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "    -out filt.tsv: [Optional; stdout]%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "       o file name to print output to%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "       o use \"-out -\" for stdout output %s",
      str_endLine
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun02 Sec02 Sub02 Cat05:
   +   - print unique output file
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
     (FILE *) outFILE,
     "    -uniq-out uniqe_profiles.tsv: [Optional; No]%s",
     str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "       o file name to print unique profiles to%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "       o use \"-uniq-out -\" for stdout output %s",
      str_endLine
   );

   /*****************************************************\
   * Fun02 Sec02 Sub03:
   *   - print settings
   *   o fun02 sec02 sub03 cat01:
   *     - print settings header
   *   o fun02 sec02 sub03 cat02:
   *     - minimum read depth for illumina variant
   *   o fun02 sec02 sub03 cat03:
   *     - minimum percent read depth for illumina variant
   *   o fun02 sec02 sub03 cat04:
   *     - minimum percent read depth for illumina del
   *   o fun02 sec02 sub03 cat05:
   *     - transition setting
   \*****************************************************/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun02 Sec02 Sub03 Cat01:
   +   - print settings header
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "   SETTINGS:%s",
      str_endLine
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun02 Sec02 Sub03 Cat02:
   +   - minimum read depth for illumina variant
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "     -min-depth %i: [Optional; %i]%s",
      def_minDepth_mainIllNano,
      def_minDepth_mainIllNano,
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "       o minimum read depth to keep an Illumina%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "         SNP%s",
      str_endLine
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun02 Sec02 Sub03 Cat03:
   +   - minimum percent read depth for illumina variant
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "     -min-perc-depth %04f: [Optional; %04f]%s",
      def_minPercDepth_mainIllNano,
      def_minPercDepth_mainIllNano,
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "       o minimum percent read depth to keep%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "         an Illumina SNP%s",
      str_endLine
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun02 Sec02 Sub03 Cat04:
   +   - minimum percent read depth for illumina del
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "     -min-perc-del %04f: [Optional; %04f]%s",
      def_minPercDel_mainIllNano,
      def_minPercDel_mainIllNano,
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "       o minimum percent read depth to keep%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "         an Illumina deletion%s",
      str_endLine
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun02 Sec02 Sub03 Cat05:
   +   - transition setting
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   if(def_transition_mainIllNano)
      fprintf(
         (FILE *) outFILE,
         "     -tran: [Optional; Yes]%s",
         str_endLine
      );
   else
      fprintf(
         (FILE *) outFILE,
         "     -tran: [Optional; No]%s",
         str_endLine
      );

   fprintf(
      (FILE *) outFILE,
      "       o if nanopore read has unkown variant%s",
      str_endLine
   );

   fprintf(
     (FILE *) outFILE,
     "         assign to illumina transition (if have)%s",
     str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "         before calling unkown (%c) variant%s",
      def_unkown_illNano,
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "       o use \"-no-tran\" to disable%s",
      str_endLine
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun02 Sec02 Sub03 Cat06:
   +   - partial ovlerap setting
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   if(def_partOverlapMerge_mainIllNano)
      fprintf(
         (FILE *) outFILE,
         "     -part-overlap: [Optional; Yes]%s",
         str_endLine
      );
   else
      fprintf(
         (FILE *) outFILE,
         "     -part-overlap: [Optional; No]%s",
         str_endLine
      );

   fprintf(
      (FILE *) outFILE,
      "       o merge profiles were one read does not%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "         fully cover the other (can be longer)%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "       o disable with \"-full-overlap\"%s",
      str_endLine
   );

   /*****************************************************\
   * Fun02 Sec02 Sub04:
   *   - help message and version number
   \*****************************************************/

   fprintf(
      (FILE *) outFILE,
      "   OTHER:%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "     -h: print this help message and exit%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "     -v: print version number and exit%s",
      str_endLine
   );

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun02 Sec03:
   ^   - print output entry
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   fprintf(
      (FILE *) outFILE,
      "Output:%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "  - Prints: tsv file with ids of reads having%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "    variants to \"-out\" (default stdout)%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "    o column 1: read id%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "    o column 2: reference id%s",
      str_endLine
   );

   fprintf(
     (FILE *) outFILE,
     "    o column 3: number variant positions in read%s",
     str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "    o column 4: variant profile%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "      - format: reference_position_base%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "        - A, C, G, T: nucleotide at position%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "        - D: is deletion at position%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "        - X: does not match Illumina profile%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "      - example: 123A_130D_140X_150G_200T_250C%s",
      str_endLine
   );
} /*phelp_mainIllNano*/

/*-------------------------------------------------------\
| Fun03: input_mainIllNano
|   - gets user input from array
| Input:
|   - numArgsSI:
|     o number of arguments user input
|   - argAryStr:
|     o c-string array with user input arguments
|   - illTsvFileStrPtr:
|     o pointer to c-string to pointer to illumin tsv file
|       name
|   - ontSamFileStrPtr:
|     o pointer to c-string to pointer to sam file name
|       with nanopore reads
|   - outFileStrPtr:
|     o pointer to c-string to pointer to output file name
|   - uniqFileStrPtr:
|     o pointer to c-string to pointer to output file for
|       unique profiles
|   - minDepthUIPtr:
|     o pointer to usigned int to hold minimum depth
|   - minPercDepthFPtr:
|     o pointer to float to hold minimum percent snp depth
|   - minPercDelFPtr:
|     o pointer to float to hold minimum percent del depth
|   - tranBlPtr:
|     o pointer to signed char set to
|       - 1: if assing unkown variants to transitions
|       - 0: if not trying to guess unkown varaints
|   - partOverlapBlPtr
|     o pointer to signed char set to
|       - 1: merge parital overlaps (both profiles have
|            variants not present in other)
|       - 0: require one profile to have all variants of
|            other
| Output:
|   - Prints:
|     o version number to outFILE
\-------------------------------------------------------*/
signed char
input_mainIllNano(
   int numArgsSI,               /*number arguments input*/
   char *argAryStr[],             /*user input arguments*/
   signed char **illTsvFileStrPtr,/*illumina tsv file*/
   signed char **ontSamFileStrPtr,/*sam file with reads*/
   signed char **outFileStrPtr,   /*output file*/
   signed char **uniqFileStrPtr,  /*uniqe output file*/
   unsigned int *minDepthUIPtr,   /*minimum read depth*/
   float *minPercDepthFPtr,       /*minimum % snp depth*/
   float *minPercDelFPtr,         /*minimum % del depth*/
   signed char *tranBlPtr,       /*assign to tranistions*/
   signed char *partOverlapBlPtr /*parital overlap merge*/
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun03 TOC:
   '   - gets user input from array
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

   signed char errSC = 0;
   signed char *tmpStr = 0;
   signed int siArg = 1;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec02:
   ^   - check if have input
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(numArgsSI <= 1)
   { /*If: nothing input*/
      phelp_mainIllNano(stdout);
      goto phelp_fun03_sec04;
   } /*If: nothing input*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec03:
   ^   - get input
   ^   o fun03 sec03 sub01:
   ^     - file io options
   ^   o fun03 sec03 sub02:
   ^     - settings
   ^   o fun03 sec03 sub03:
   ^     - help message
   ^   o fun03 sec03 sub04:
   ^     - version number
   ^   o fun03 sec03 sub05:
   ^     - invalid input
   ^   o fun03 sec03 sub06:
   ^     - move to next argument
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun03 Sec03 Sub01:
   *   - file io options
   \*****************************************************/

   /*using eqlNull_ulCp is not very efficent, since most
   `   strings are under 8 char. however, makes life
   `   easier at a point that is not a bottle neck
   */
   while(siArg < numArgsSI)
   { /*Loop: get user input*/
      if(
         ! eqlNull_ulCp(
            (signed char *) "-ill-tsv",
            (signed char *) argAryStr[siArg]
         )
      ){ /*If: illumina tsv file*/
         ++siArg;
         *illTsvFileStrPtr =
            (signed char *) argAryStr[siArg];
      } /*If: illumina tsv file*/

      else if(
         ! eqlNull_ulCp(
            (signed char *) "-sam-ont",
            (signed char *) argAryStr[siArg]
         )
      ){ /*Else If: nanopore reads in sam file*/
         ++siArg;
         *ontSamFileStrPtr =
           (signed char *) argAryStr[siArg];
      } /*Else If: nanopore reads in sam file*/

      else if(
         ! eqlNull_ulCp(
            (signed char *) "-out",
            (signed char *) argAryStr[siArg]
         )
      ){ /*Else If: output file*/
         ++siArg;
         *outFileStrPtr =
            (signed char *) argAryStr[siArg];
      } /*Else If: output file*/

      else if(
         ! eqlNull_ulCp(
            (signed char *) "-uniq-out",
            (signed char *) argAryStr[siArg]
         )
      ){ /*Else If: output file*/
         ++siArg;
         *uniqFileStrPtr =
            (signed char *) argAryStr[siArg];
      } /*Else If: output file*/

      /**************************************************\
      * Fun03 Sec03 Sub02:
      *   - settings
      *   o fun03 sec03 sub02 cat01:
      *     - minimum Illumina read depth
      *   o fun03 sec03 sub02 cat02:
      *     - snp minimum percent Illumina read depth
      *   o fun03 sec03 sub02 cat03:
      *     - del minimum percent Illumina read depth
      *   o fun03 sec03 sub02 cat04:
      *     - transition check setting
      *   o fun03 sec03 sub02 cat05:
      *     - partail overlap setting
      \**************************************************/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun03 Sec03 Sub02 Cat01:
      +   - minimum Illumina read depth
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      else if(
         ! eqlNull_ulCp(
            (signed char *) "-min-depth",
            (signed char *) argAryStr[siArg]
         )
      ){ /*Else If: min read depth (Illumina)*/
         ++siArg;
         tmpStr = (signed char *) argAryStr[siArg];

         tmpStr +=
            strToUI_base10str(
               tmpStr,
               minDepthUIPtr
            );

         if(*tmpStr != '\0')
         { /*If: error*/
            fprintf(
               stderr,
               "-min-depth %s; non-numeric or to large%s",
               argAryStr[siArg],
               str_endLine
            );

            goto err_fun03_sec04;
         } /*If: error*/
      } /*Else If: min read depth (Illumina)*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun03 Sec03 Sub02 Cat02:
      +   - snp minimum percent Illumina read depth
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      else if(
         ! eqlNull_ulCp(
            (signed char *) "-min-perc-depth",
            (signed char *) argAryStr[siArg]
         )
      ){ /*Else If: min percent read depth (Illumina)*/
         ++siArg;
         *minPercDepthFPtr = atof(argAryStr[siArg]);
      } /*Else If: min percent read depth (Illumina)*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun03 Sec03 Sub02 Cat03:
      +   - del minimum percent Illumina read depth
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      else if(
         ! eqlNull_ulCp(
            (signed char *) "-min-perc-del",
            (signed char *) argAryStr[siArg]
         )
      ){ /*Else If: min percent read depth (Illumina)*/
         ++siArg;
         *minPercDelFPtr = atof(argAryStr[siArg]);
      } /*Else If: min percent read depth (Illumina)*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun03 Sec03 Sub02 Cat04:
      +   - transition check setting
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      else if(
         ! eqlNull_ulCp(
            (signed char *) "-tran",
            (signed char *) argAryStr[siArg]
         )
      ) *tranBlPtr = 1;

      else if(
         ! eqlNull_ulCp(
            (signed char *) "-no-tran",
            (signed char *) argAryStr[siArg]
         )
      ) *tranBlPtr = 0;

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun03 Sec03 Sub02 Cat05:
      +   - partail overlap setting
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      else if(
         ! eqlNull_ulCp(
            (signed char *) "-part-overlap",
            (signed char *) argAryStr[siArg]
         )
      ) *partOverlapBlPtr = 1;

      else if(
         ! eqlNull_ulCp(
            (signed char *) "-full-overlap",
            (signed char *) argAryStr[siArg]
         )
      ) *partOverlapBlPtr = 0;

      /**************************************************\
      * Fun03 Sec03 Sub03:
      *   - help message
      \**************************************************/

      else if(
         ! eqlNull_ulCp(
            (signed char *) "-h",
            (signed char *) argAryStr[siArg]
         )
      ){ /*Else If: help message request*/
         phelp_mainIllNano(stdout);
         goto phelp_fun03_sec04;
      } /*Else If: help message request*/

      else if(
         ! eqlNull_ulCp(
            (signed char *) "--h",
            (signed char *) argAryStr[siArg]
         )
      ){ /*Else If: help message request*/
         phelp_mainIllNano(stdout);
         goto phelp_fun03_sec04;
      } /*Else If: help message request*/

      else if(
         ! eqlNull_ulCp(
            (signed char *) "help",
            (signed char *) argAryStr[siArg]
         )
      ){ /*Else If: help message request*/
         phelp_mainIllNano(stdout);
         goto phelp_fun03_sec04;
      } /*Else If: help message request*/

      else if(
         ! eqlNull_ulCp(
            (signed char *) "-help",
            (signed char *) argAryStr[siArg]
         )
      ){ /*Else If: help message request*/
         phelp_mainIllNano(stdout);
         goto phelp_fun03_sec04;
      } /*Else If: help message request*/

      else if(
         ! eqlNull_ulCp(
            (signed char *) "--help",
            (signed char *) argAryStr[siArg]
         )
      ){ /*Else If: help message request*/
         phelp_mainIllNano(stdout);
         goto phelp_fun03_sec04;
      } /*Else If: help message request*/

      /**************************************************\
      * Fun03 Sec03 Sub04:
      *   - version number
      \**************************************************/

      else if(
         ! eqlNull_ulCp(
            (signed char *) "-v",
            (signed char *) argAryStr[siArg]
         )
      ){ /*Else If: version number request*/
         pversion_mainIllNano(stdout);
         goto pversion_fun03_sec04;
      } /*Else If: version number request*/

      else if(
         ! eqlNull_ulCp(
            (signed char *) "--v",
            (signed char *) argAryStr[siArg]
         )
      ){ /*Else If: version number request*/
         pversion_mainIllNano(stdout);
         goto pversion_fun03_sec04;
      } /*Else If: version number request*/

      else if(
         ! eqlNull_ulCp(
            (signed char *) "version",
            (signed char *) argAryStr[siArg]
         )
      ){ /*Else If: version number request*/
         pversion_mainIllNano(stdout);
         goto pversion_fun03_sec04;
      } /*Else If: version number request*/

      else if(
         ! eqlNull_ulCp(
            (signed char *) "-version",
            (signed char *) argAryStr[siArg]
         )
      ){ /*Else If: version number request*/
         pversion_mainIllNano(stdout);
         goto pversion_fun03_sec04;
      } /*Else If: version number request*/

      else if(
         ! eqlNull_ulCp(
            (signed char *) "--version",
            (signed char *) argAryStr[siArg]
         )
      ){ /*Else If: version number request*/
         pversion_mainIllNano(stdout);
         goto pversion_fun03_sec04;
      } /*Else If: version number request*/

      /**************************************************\
      * Fun03 Sec03 Sub05:
      *   - invalid input
      \**************************************************/

      else
      { /*Else: ivalid input*/
         fprintf(
            stderr,
            "%s is not recongnized%s",
            argAryStr[siArg],
            str_endLine
         );

         goto err_fun03_sec04;
      } /*Else: ivalid input*/

      /**************************************************\
      * Fun03 Sec03 Sub06:
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
} /*input_mainIllNano*/

/*-------------------------------------------------------\
| Main:
|   - driver function for illNano
| Input:
|   - numArgsSI:
|     o number of arguments user input (c provides)
|   - argAryStr:
|     o c-string array with user input (c provides)
| Output:
|   - Prints:
|     o read ids of reads having varaint positons to tsv
|       file
\-------------------------------------------------------*/
int
main(
   int numArgsSI,
   char *argAryStr[]
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Main TOC:
   '   - driver function for illNano
   '   o main sec01:
   '     - variable declarations
   '   o main sec02:
   '     - get and check (open files) user input 
   '   o main sec03:
   '     - run illNano (find reads for Illumina varaints)
   '   o main sec04:
   '     - clean up and return
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   signed char errSC = 0;

   signed char *illTsvFileStr = 0;
   signed char *ontSamFileStr = 0;
   signed char *outFileStr = 0;
   signed char *uniqFileStr = 0;/*printing uniq varaints*/

   unsigned int minDepthUI = def_minDepth_mainIllNano;
   float minPercDepthF  = def_minPercDepth_mainIllNano;
   float minPercDelF  = def_minPercDel_mainIllNano;
   signed char tranBl  = def_transition_mainIllNano;
   signed char partOverlapBl =
      def_partOverlapMerge_mainIllNano;

   FILE *illFILE = 0;
   FILE *ontFILE = 0;
   FILE *outFILE = 0;
   FILE *uniqFILE = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec02:
   ^   - get and check (open files) user input
   ^   o main sec02 sub01:
   ^     - get user input
   ^   o main sec02 sub02:
   ^     - open illumina tsv file
   ^   o main sec02 sub03:
   ^     - open ont sam file
   ^   o main sec02 sub04:
   ^     - open output file
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Main Sec02 Sub01:
   *   - get user input
   \*****************************************************/

   errSC =
      input_mainIllNano(
         numArgsSI,
         argAryStr,
         &illTsvFileStr,
         &ontSamFileStr,
         &outFileStr,
         &uniqFileStr,
         &minDepthUI,
         &minPercDepthF,
         &minPercDelF,
         &tranBl,
         &partOverlapBl
      ); /*get input*/

   if(errSC)
   { /*If: had error*/
      --errSC; /*help/version requests to no error (0)*/
      goto ret_main_sec04;
   } /*If: had error*/

   /*****************************************************\
   * Main Sec02 Sub02:
   *   - open illumina tsv file
   \*****************************************************/

   if(
         ! illTsvFileStr
      || illTsvFileStr[0] == '-' 
   ) illFILE = stdin;

   else
   { /*Else: user provided illumina file*/
      illFILE =
         fopen(
            (char *) illTsvFileStr,
            "r"
         );

      if(! illFILE)
      { /*If: file error*/
         fprintf(
            stderr,
            "could not open -ill-tsv %s%s",
            illTsvFileStr,
            str_endLine
         );

         goto err_main_sec04;
      } /*If: file error*/
   } /*Else: user provided illumina file*/

   /*****************************************************\
   * Main Sec02 Sub03:
   *   - open ont sam file
   \*****************************************************/

   if(
         ! ontSamFileStr
      || ontSamFileStr[0] == '-' 
   ) ontFILE = stdin;

   else
   { /*Else: user provided ont sam file*/
      ontFILE =
         fopen(
            (char *) ontSamFileStr,
            "r"
         );

      if(! ontFILE)
      { /*If: file error*/
         fprintf(
            stderr,
            "could not open -sam-ont %s%s",
            ontSamFileStr,
            str_endLine
         );

         goto err_main_sec04;
      } /*If: file error*/
   } /*Else: user provided ont sam file*/

   if(
         illFILE == stdin
      && ontFILE == stdin
   ){ /*If: two stdin files*/
      fprintf(
         stderr,
         "-ill-tsv %s and -sam-ont %s are from stdin%s",
         illTsvFileStr,
         ontSamFileStr,
         str_endLine
      );

      goto err_main_sec04;
   } /*If: two stdin files*/

   /*****************************************************\
   * Main Sec02 Sub04:
   *   - open output file
   \*****************************************************/

   if(
         ! outFileStr
      || outFileStr[0] == '-' 
   ) outFILE = stdout;

   else
   { /*Else: user provided output file*/
      outFILE =
         fopen(
            (char *) outFileStr,
            "w"
         );

      if(! outFILE)
      { /*If: file error*/
         fprintf(
            stderr,
            "could not open -out %s%s",
            outFileStr,
            str_endLine
         );

         goto err_main_sec04;
      } /*If: file error*/
   } /*Else: user provided output file*/

   /*****************************************************\
   * Main Sec02 Sub05:
   *   - open uniqe profile output file
   \*****************************************************/

   if(! uniqFileStr)
      uniqFILE = 0;

   else if(uniqFileStr[0] == '-')
   { /*Else If: stdout output*/
      if(outFILE == stdout)
      { /*If: dual stdout output*/
         fprintf(
            stderr,
            "printing uniqe profiles and ids tsv's to%s",
            str_endLine
          );

         fprintf(
            stderr,
            "  stdout%s",
            str_endLine
          );
      } /*If: dual stdout output*/

      uniqFILE = stdout;
   } /*Else If: stdout output*/

   else
   { /*Else: user provided output file*/
      uniqFILE =
         fopen(
            (char *) uniqFileStr,
            "w"
         );

      if(! uniqFILE)
      { /*If: file error*/
         fprintf(
            stderr,
            "could not open -uniq-out %s%s",
            outFileStr,
            str_endLine
         );

         goto err_main_sec04;
      } /*If: file error*/
   } /*Else: user provided output file*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec03:
   ^   - run illNano (find reads for Illumina varaints)
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   errSC =
      run_illNano(
         minDepthUI,
         minPercDepthF,
         minPercDelF,
         tranBl,
         partOverlapBl,
         illFILE,
         ontFILE,
         outFILE,
         uniqFILE
      ); /*get and print variant profiles for ont reads*/

   if(errSC)
   { /*If: had error*/
      if(errSC == def_noVar_illNano)
      { /*If: no variants found*/
         fprintf(
            stderr,
            "-ill-tsv %s has no variations%s",
            illTsvFileStr,
            str_endLine
         );

         errSC = 0;
         goto ret_main_sec04;
      } /*If: no variants found*/

      else if(errSC == def_memErr_illNano)
         fprintf(
            stderr,
            "memory error while running illNano%s",
            str_endLine
         );
      else
         fprintf(
            stderr,
            "file error while running illNano%s",
            str_endLine
         );

      goto err_main_sec04;
   } /*If: had error*/
   
   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec04:
   ^   - clean up and return
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   errSC = 0;
   goto ret_main_sec04;
   
   err_main_sec04:;
      errSC = -1;
      goto ret_main_sec04;

   ret_main_sec04:;
      if(
            illFILE
         && illFILE != stdin
         && illFILE != stdout
      ) fclose(illFILE);

      illFILE = 0;


      if(
            ontFILE
         && ontFILE != stdin
         && ontFILE != stdout
      ) fclose(ontFILE);

      ontFILE = 0;


      if(
            outFILE
         && outFILE != stdin
         && outFILE != stdout
      ) fclose(outFILE);

      if(
            uniqFILE
         && uniqFILE != stdin
         && uniqFILE != stdout
      ) fclose(uniqFILE);

      uniqFILE = 0;

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
