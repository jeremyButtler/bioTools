/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' mainCgMLST SOF: Start Of File
'   - find cgMLST lineage of sequence/reads
'   o header:
'     - included libraries
'   o fun01: pversion_mainCgMLST
'     - prints version number for cgMLST
'   o fun02: phelp_mainCgMLST
'     - prints help message
'   o fun03: input_mainCgMLST
'     - gets user input
'   o main:
'     - driver function for cgMLST
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

#include "../genLib/base10str.h"
#include "../genLib/ulCp.h"

#include "../genBio/samEntry.h"

#include "../genLin/cgMLST.h"

/*.h files only*/
#include "../bioTools.h" /*version*/
#include "../genLin/defsCgMLST.h"   /*default values*/

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\
! Hidden Libraries:
!   o .c  #include "../genLib/strAry.h"
!   o .c  #include "../genLib/ptrAry.h"
!   o .c  #include "../genBio/seqST.h"
!   o .h  #include "../genBio/edDist.h"
!   o .h  #include "../genBio/ntTo5Bit.h"
!   o .h  #include "../genLib/genMath.h" only using macros
!   o .h  #include "../genLib/dataTypeShortHand.h"
\%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

/*-------------------------------------------------------\
| Fun01: pversion_mainCgMLST
|   - prints version number for cgMLST
| Input:
|   - outFILE:
|     o pointer to FILE to print version number to
| Output:
|   - Prints:
|     o version number to outFILE
\-------------------------------------------------------*/
void
pversion_mainCgMLST(
   void *outFILE
){
   fprintf(
      (FILE *) outFILE,
      "cgMLST from bioTools version: %i-%02i-%02i\n",
      def_year_bioTools,
      def_month_bioTools,
      def_day_bioTools
   ); /*print version number message*/
} /*pversion_mainCgMLST*/

/*-------------------------------------------------------\
| Fun02: phelp_mainCgMLST
|   - prints help message
| Input:
|   - outFILE:
|     o pointer to file to print help message to
| Output:
|   - Prints:
|     o help message to outFILE
\-------------------------------------------------------*/
void
phelp_mainCgMLST(
   void *outFILE
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun02: phelp_mainCgMLST
   '   - prints help message
   '   o fun02 sec01:
   '     - usage block
   '   o fun02 sec02:
   '     - input block
   '   o fun02 sec03:
   '     - output block
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun02 Sec01:
   ^   - usage block
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   fprintf(
      (FILE *) outFILE,
      "cgMLST -db cgMLST-db.sam -sam file.sam <options>\n"
   );

   fprintf(
      (FILE *) outFILE,
      "  - prints cgMLST lineage for a sam file\n"
   );

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun02 Sec02:
   ^   - input block
   ^   o fun02 sec02 sub01:
   ^     - input block header
   ^   o fun02 sec02 sub03:
   ^     - file IO
   ^   o fun02 sec02 sub04:
   ^     - lineage table building variables
   ^   o fun02 sec02 sub05:
   ^     - help and version numbers
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
   *   o fun02 sec02 sub02 cat01:
   *     - sam file input
   *   o fun02 sec02 sub02 cat02:
   *     - database file input
   *   o fun02 sec02 sub02 cat03:
   *     - output file
   *   o fun02 sec02 sub02 cat04:
   *     - id column for printing
   \*****************************************************/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun02 Sec02 Sub02 cat01:
   +   - sam file input
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "  -sam file.sam: [Required]\n"
   );

   fprintf(
      (FILE *) outFILE,
      "    o sam file with mapped sequence/reads to\n"
   );

   fprintf(
      (FILE *) outFILE,
      "      lineage for\n"
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun02 Sec02 Sub02 cat02:
   +   - database file input
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "  -db cgMLST-db.sam: [Required]\n"
   );

   fprintf(
      (FILE *) outFILE,
      "    o sam file with mapped cgMLST database\n"
   );

   fprintf(
      (FILE *) outFILE,
      "      * includes all loci and alleles\n"
   );

   fprintf(
      (FILE *) outFILE,
      "      * allele names are \"loci%c%callele\"\n",
      def_lociEndOne_cgMLST,
      def_lociEndTwo_cgMLST
   );

   fprintf(
      (FILE *) outFILE,
      "    o needs to be same reference as -sam file.sam\n"
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun02 Sec02 Sub02 cat03:
   +   - output file
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "  -out file.tsv: [recommended; stdout]\n"
   );

   fprintf(
      (FILE *) outFILE,
      "    o file to print lineage to\n"
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun02 Sec02 Sub02 Cat04:
   +   - id column for printing
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "  -id \"out\" : [Optional; out]\n"
   );

   fprintf(
      (FILE *) outFILE,
      "    o id to assign to report (first column)\n"
   );

   /*****************************************************\
   * Fun02 Sec02 Sub03:
   *   - lineage table building variables
   *   o fun02 sec02 sub03 cat01:
   *     - maximum edit distance
   *   o fun02 sec02 sub03 cat02:
   *     - minimum indel length
   *   o fun02 sec02 sub03 cat03:
   *     - minimum snp quality score
   *   o fun02 sec02 sub03 cat04:
   *     - minimum overlap
   \*****************************************************/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun02 Sec02 Sub03 cat01:
   +   - maximum edit distance
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "  -max-dist %i: [Optional; %i]\n",
      def_maxDist_defsCgMLST,
      def_maxDist_defsCgMLST
   );

   fprintf(
      (FILE *) outFILE,
      "    o max edit distance to no call unique allele\n"
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun02 Sec02 Sub03 cat02:
   +   - minimum indel length
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "  -min-indel-len %u: [Optional; %u]\n",
      def_indelLen_defsCgMLST,
      def_indelLen_defsCgMLST
   );

   fprintf(
      (FILE *) outFILE,
      "    o minimum length for indel to be counted\n"
   );

   fprintf(
      (FILE *) outFILE,
      "      in edit distance (short lengths discarded)\n"
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun02 Sec02 Sub03 cat03:
   +   - minimum snp quality score
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "  -min-snp-q %u: [Optional; %u]\n",
      def_minQ_defsCgMLST,
      def_minQ_defsCgMLST
   );

   fprintf(
      (FILE *) outFILE,
      "    o min q-score to count snp in edit distance\n"
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun02 Sec02 Sub03 cat04:
   +   - minimum overlap
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "  -min-overlap %f: [Optional; %f]\n",
      def_minOverlap_defsCgMLST,
      def_minOverlap_defsCgMLST
   );

   fprintf(
      (FILE *) outFILE,
      "    o min overlap between loci/allele and read\n"
   );

   fprintf(
      (FILE *) outFILE,
      "      to count read as mapped\n"
   );

   /*****************************************************\
   * Fun02 Sec02 Sub04:
   *   - lineage print settings
   *   o fun02 sec02 sub04 cat01:
   *     - minimum read depth for printing
   *   o fun02 sec02 sub04 cat02:
   *     - minimum percent depth of lineage to print
   *   o fun02 sec02 sub04 cat03:
   *     - minimum percent depth of lineage to print
   \*****************************************************/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun02 Sec02 Sub04 Cat01:
   +   - minimum read depth for printing
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "  -min-depth %u: [Optional; %u]\n",
      def_minPDepth_defsCgMLST,
      def_minPDepth_defsCgMLST
   );

   fprintf(
      (FILE *) outFILE,
      "    o min read depth to keep allele (else NA)\n"
   );

   fprintf(
      (FILE *) outFILE,
      "    o use \"-min-depth 1\" for consensuse\n"
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun02 Sec02 Sub04 Cat02:
   +   - minimum percent depth of lineage to print
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "  -min-perc %f: [Optional; %f]\n",
      def_minPPerc_defsCgMLST,
      def_minPPerc_defsCgMLST
   );

   fprintf(
      (FILE *) outFILE,
      "    o min percent of total reads supporting\n"
   );

   fprintf(
      (FILE *) outFILE,
      "       allele needed to print allele (else NA)\n"
   );

   fprintf(
      (FILE *) outFILE,
      "    o use \"-min-perc 0\" for consensuse\n"
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun02 Sec02 Sub04 Cat03:
   +   - minimum percent depth of lineage to print
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   if(def_pHead_defsCgMLST)
      fprintf(
         (FILE *) outFILE,
         "  -p-head: [Optional; Yes]\n"
      );
   else
      fprintf(
         (FILE *) outFILE,
         "  -p-head: [Optional; No]\n"
      );

   fprintf(
      (FILE *) outFILE,
      "    o print lineage header\n"
   );

   fprintf(
      (FILE *) outFILE,
      "    o disable with \"-no-p-head\"\n"
   );

   /*****************************************************\
   * Fun02 Sec02 Sub05:
   *   - help and version numbers
   \*****************************************************/

   fprintf(
      (FILE *) outFILE,
      "  -h: print this help message and exit\n"
   );

   fprintf(
      (FILE *) outFILE,
      "  -v: print version and exit\n"
   );

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun02 Sec03:
   ^   - output block
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   fprintf(
      (FILE *) outFILE,
      "Output:\n"
   );

   fprintf(
      (FILE *) outFILE,
      "  - prints the allele (lingeage) for each loci\n"
   );

   fprintf(
      (FILE *) outFILE,
      "    \"-out file.tsv\" or stdout if no \"-out\"\n"
   );

   fprintf(
      (FILE *) outFILE,
      "  - cgMLST often has over 500 to 1000+ loci, so\n"
   );

   fprintf(
      (FILE *) outFILE,
      "    output tsvs are not spread sheet editable\n"
   );
} /*phelp_mainCgMLST*/

/*-------------------------------------------------------\
| Fun03: input_mainCgMLST
|   - gets user input
| Input:
|   - numArgsSI:
|     o number arguments user input
|   - argAryStr:
|     o c-string array with user inpu
|   - samFileStrPtr:
|     o pointer to c-string to be set to sam file
|       name (do not free)
|   - dbFileStrPtr:
|     o pointer to c-string to be set to database file
|       name (do not free)
|   - outFileStrPtr:
|     o pointer to c-string to be set to output file
|       name (do not free)
|   - idStrPtr:
|     o pointer to c-string to be set to id (do not free)
|   - maxDistSLPtr:
|     o signed long pointer to hold max edit distance
|   - minQUCPtr:
|     o unsigned char pointer to hold min q-score
|   - minOverlapFPtr:
|     o float pointer to be set to min overlap requred
|       to check cmMLST
|   - minIndelUIPtr:
|     o unsigned int pointer to get min indel length
|   - minDepthUIPtr:
|     o unsigned int pionter to hold min read depth
|   - minPercFPtr:
|     o float pointer to hold min percent support
|   - pHeadBlPtr:
|     o signed char pointer to hold if printing header
|       * set to 1 if printing header
|       * set to 0 if not printing header
| Output:
|   - Modifies:
|     o parameters (except argAryStr and numArgsSI) to
|       have user input
|   - Prints:
|     o error/version/help messages to stderr
|   - Returns:
|     o 0 for no errors
|     o 1 if printed help message or version number
|     o 2 if had error
\-------------------------------------------------------*/
signed char
input_mainCgMLST(
   int numArgsSI,
   char *argAryStr[],
   signed char **samFileStrPtr, /*assigned sam file*/
   signed char **dbFileStrPtr,  /*assigned database file*/
   signed char **outFileStrPtr, /*assigned output file*/
   signed char **idStrPtr,      /*assigned entry id*/
   signed long *maxDistSLPtr,   /*max edit distance*/
   unsigned char *minQUCPtr,    /*min edit snp q-score*/
   float *minOverlapFPtr,       /*min overlap to check*/
   unsigned int *minIndelUIPtr, /*min indel length*/
   unsigned int *minDepthUIPtr, /*min read depth*/
   float *minPercFPtr,          /*min % support depth*/
   signed char *pHeadBlPtr      /*set if printing header*/
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun03: input_mainCgMLST
   '   - gets user input
   '   o fun03 sec01:
   '     - variable declarations + check if have input
   '   o fun03 sec02:
   '     - get input
   '   o fun03 sec03:
   '     - return result
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec01:
   ^   - variable declarations + check if have input
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   signed int siArg = 1;
   signed char errSC = 0;
   signed char *tmpStr = 0;

   if(numArgsSI < 1)
      goto phelp_fun03_sec03;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec02:
   ^   - get input
   ^   o fun03 sec02 sub01:
   ^     - file input/output + start loop
   ^   o fun03 sec02 sub02:
   ^     - lineage table building settings
   ^   o fun03 sec02 sub03:
   ^     - printing settings
   ^   o fun03 sec02 sub04:
   ^     - help message
   ^   o fun03 sec02 sub05:
   ^     - version number
   ^   o fun03 sec02 sub06:
   ^     - deal with errors
   ^   o fun03 sec02 sub07:
   ^     - move to next argument
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun03 Sec02 Sub01:
   *   - file input/output + start loop
   *   o fun03 sec02 sub01 cat01:
   *     - sam file input + start loop
   *   o fun03 sec02 sub01 cat02:
   *     - database file input
   *   o fun03 sec02 sub01 cat03:
   *     - output file input
   *   o fun03 sec02 sub01 cat04:
   *     - analysis id (frist column)
   \*****************************************************/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun03 Sec02 sub01 cat01:
   +   - sam file input + start loop
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   while(siArg < numArgsSI)
   { /*Loop: get input*/
      if(
         ! eqlNull_ulCp(
            (signed char *) "-sam",
            (signed char *) argAryStr[siArg]
         )
      ){ /*If: sam file input*/
         ++siArg;
         *samFileStrPtr= (signed char *) argAryStr[siArg];
      }  /*If: sam file input*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun03 Sec02 sub01 cat02:
      +   - database file input
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      else if(
         ! eqlNull_ulCp(
            (signed char *) "-db",
            (signed char *) argAryStr[siArg]
         )
      ){ /*Else If: data base file input*/
         ++siArg;
         *dbFileStrPtr = (signed char *) argAryStr[siArg];
      }  /*Else If: database file input*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun03 Sec02 sub01 cat03:
      +   - output file input
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      else if(
         ! eqlNull_ulCp(
            (signed char *) "-out",
            (signed char *) argAryStr[siArg]
         )
      ){ /*Else If: output file input*/
         ++siArg;
         *outFileStrPtr= (signed char *) argAryStr[siArg];
      }  /*Else If: output file input*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun03 Sec02 sub01 cat04:
      +   - analysis id (frist column)
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      else if(
         ! eqlNull_ulCp(
            (signed char *) "-id",
            (signed char *) argAryStr[siArg]
         )
      ){  /*Else If: id for analysis input*/
         ++siArg;
         *idStrPtr = (signed char *) argAryStr[siArg];
      }  /*Else If: id for analysis input*/

      /**************************************************\
      * Fun03 Sec02 Sub02:
      *   - lineage table building settings
      *   o fun03 sec02 sub02 cat01:
      *     - maximum edit distance to count as match
      *   o fun03 sec02 sub02 cat02:
      *     - snp minimum q-score to keep
      *   o fun03 sec02 sub02 cat03:
      *     - minimum overlap between read and allele
      *   o fun03 sec02 sub02 cat04:
      \**************************************************/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun03 Sec02 sub02 cat01:
      +   - maximum edit distance to count as match
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      else if(
         ! eqlNull_ulCp(
            (signed char *) "-max-dist",
            (signed char *) argAryStr[siArg]
         )
      ){  /*Else If: max edit distance*/
         ++siArg;
         tmpStr = (signed char *) argAryStr[siArg];

         tmpStr +=
            strToSL_base10str(
               tmpStr,
               maxDistSLPtr
            );

         if(*tmpStr != '\0')
         { /*If: non-numeric input*/
            fprintf(
               stderr,
               "-max-dist %s; non-numeric or to large\n",
               argAryStr[siArg]
            );

            goto error_fun03_sec03;
         } /*If: non-numeric input*/
      }  /*Else If: max edit distance*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun03 Sec02 sub02 cat02:
      +   - snp minimum q-score to keep
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      else if(
         ! eqlNull_ulCp(
            (signed char *) "-min-snp-q",
            (signed char *) argAryStr[siArg]
         )
      ){ /*Else If: min snp q-score input*/
         ++siArg;
         tmpStr = (signed char *) argAryStr[siArg];

         tmpStr +=
            strToUC_base10str(
               tmpStr,
               minQUCPtr
            );

         if(*tmpStr != '\0')
         { /*If: non-numeric input*/
            fprintf(
               stderr,
               "-min-snp-q %s; non-numeric or to large\n",
               argAryStr[siArg]
            );

            goto error_fun03_sec03;
         } /*If: non-numeric input*/
      }  /*Else If: min snp q-score input*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun03 Sec02 sub02 cat03:
      +   - minimum overlap between read and allele
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      else if(
         ! eqlNull_ulCp(
            (signed char *) "-min-overlap",
            (signed char *) argAryStr[siArg]
         )
      ){ /*Else If: minimum overlap input*/
         ++siArg;
         *minOverlapFPtr = atof(argAryStr[siArg]);
      }  /*Else If: minimum overlap input*/

      /**************************************************\
      * Fun03 Sec02 Sub03:
      *   - printing settings
      *   o fun03 sec02 sub03 cat01:
      *     - min indel length
      *   o fun03 sec02 sub03 cat02:
      *     - min depth
      *   o fun03 sec02 sub03 cat03:
      *     - minimum allele percent read support
      *   o fun03 sec02 sub03 cat04:
      *     - printing loci header
      \**************************************************/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun03 Sec02 sub03 cat01:
      +   - min indel length
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      else if(
         ! eqlNull_ulCp(
            (signed char *) "-min-indel-len",
            (signed char *) argAryStr[siArg]
         )
      ){  /*Else If: min depth to print input*/
         ++siArg;
         tmpStr = (signed char *) argAryStr[siArg];

         tmpStr +=
            strToUI_base10str(
               tmpStr,
               minIndelUIPtr
            );

         if(*tmpStr != '\0')
         { /*If: non-numeric input*/
            fprintf(
               stderr,
               "-min-indel-len %s non-numeric/to large\n",
               argAryStr[siArg]
            );

            goto error_fun03_sec03;
         } /*If: non-numeric input*/
      }  /*Else If: min depth to print input*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun03 Sec02 sub03 cat02:
      +   - min depth
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      else if(
         ! eqlNull_ulCp(
            (signed char *) "-min-depth",
            (signed char *) argAryStr[siArg]
         )
      ){  /*Else If: min depth to print input*/
         ++siArg;
         tmpStr = (signed char *) argAryStr[siArg];

         tmpStr +=
            strToUI_base10str(
               tmpStr,
               minDepthUIPtr
            );

         if(*tmpStr != '\0')
         { /*If: non-numeric input*/
            fprintf(
               stderr,
               "-min-depth %s; non-numeric or to large\n",
               argAryStr[siArg]
            );

            goto error_fun03_sec03;
         } /*If: non-numeric input*/
      }  /*Else If: min depth to print input*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun03 Sec02 sub03 cat02:
      +   - minimum allele percent read support
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      else if(
         ! eqlNull_ulCp(
            (signed char *) "-min-perc",
            (signed char *) argAryStr[siArg]
         )
      ){ /*Else If: minimum overlap input*/
         ++siArg;
         *minPercFPtr = atof(argAryStr[siArg]);
      }  /*Else If: minimum overlap input*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun03 Sec02 sub03 cat03:
      +   - printing loci header
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      else if(
         ! eqlNull_ulCp(
            (signed char *) "-p-head",
            (signed char *) argAryStr[siArg]
         )
      ) *pHeadBlPtr = 1;

      else if(
         ! eqlNull_ulCp(
            (signed char *) "-no-p-head",
            (signed char *) argAryStr[siArg]
         )
      ) *pHeadBlPtr = 0;

      /**************************************************\
      * Fun03 Sec02 Sub04:
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
      * Fun03 Sec02 Sub05:
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
      * Fun03 Sec02 Sub06:
      *   - deal with errors
      \**************************************************/

      else
      { /*Else: unkown input*/
         fprintf(
            stderr,
            "%s is not recognized\n",
            argAryStr[siArg]
         );

         goto error_fun03_sec03;
      } /*Else: unkown input*/

      /**************************************************\
      * Fun03 Sec02 Sub07:
      *   - move to next argument
      \**************************************************/

      ++siArg;
   } /*Loop: get input*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec03:
   ^   - return result
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   errSC = 0;
   goto ret_fun03_sec03;

   error_fun03_sec03:;
      errSC = 2;
      goto ret_fun03_sec03;

   phelp_fun03_sec03:;
      phelp_mainCgMLST(stdout);
      errSC = 1;
      goto ret_fun03_sec03;

   pversion_fun03_sec03:;
      pversion_mainCgMLST(stdout);
      errSC = 1;
      goto ret_fun03_sec03;

   ret_fun03_sec03:;
      return errSC;
} /*input_mainCgMLST*/

/*-------------------------------------------------------\
| Main:
|   - driver function for cgMLST
| Input:
|   - argAryStr:
|     o c-string array with user input
|   - numArgSI:
|     o number arguments user input
| Output:
|   - Prints:
|     o lineage to stdout/file (no errors)
|     o errors to sdterr
\-------------------------------------------------------*/
int
main(
   int numArgsSI,
   char * argAryStr[]
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Main TOC:
   '   - driver function for cgMLST
   '   o main sec01:
   '     - variable declarations
   '   o main sec02:
   '     - initialize, get input, setup, check input
   '   o main sec03:
   '     - find and print lineage
   '   o main sec04:
   '     - report errors, clean up, and return
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   signed char *samFileStr = 0; /*sam file to process*/
   signed char *dbFileStr = 0; /*mapped cgMLST database*/
   signed char *outFileStr = 0;/*output file to print to*/
   signed char *idStr = (signed char *) "out";

   signed long maxDistSL = def_maxDist_defsCgMLST;

   /*edit distance settings*/
   unsigned int minIndelUI = def_indelLen_defsCgMLST;
   unsigned char minQUC = def_minQ_defsCgMLST;
   float minOverlapF = def_minOverlap_defsCgMLST;

   /*lineage printing settings (minimum values)*/
   unsigned int minDepthUI = def_minPDepth_defsCgMLST;
   float minPercF = def_minPPerc_defsCgMLST;
   signed char pHeadBl = def_pHead_defsCgMLST;
      /*print header*/

   /*internal variables*/

   signed char errSC = 0;
   signed long seqSL = 0; /*sequence on in sam file*/
   signed long lociSL = 0;/*number sequences with loci*/ 

   /*sam file reading variables*/
   signed char *buffHeapStr = 0;
   unsigned long buffLenUL = 0;
   struct samEntry samStackST; /*reads*/

   struct allele_cgMLST *cgMLSTHeapST = 0;

   FILE *samFILE = 0;
   FILE *outFILE = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec02:
   ^   - initialize, get input, setup, check input
   ^   o main sec02 sub01:
   ^     - initialize structs
   ^   o main sec02 sub02:
   ^     - get input
   ^   o main sec02 sub03:
   ^     - setup structs
   ^   o main sec02 sub04:
   ^     - open input/output files + get database
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Main Sec02 Sub01:
   *   - initialize structs
   \*****************************************************/

   init_samEntry(&samStackST);

   /*****************************************************\
   * Main Sec02 Sub02:
   *   - get input
   \*****************************************************/

   errSC =
      input_mainCgMLST(
         numArgsSI,    /*number user input arguments*/
         argAryStr,    /*user arguments*/
         &samFileStr,  /*input sam file*/
         &dbFileStr,   /*cgMLST lineage database*/
         &outFileStr,  /*output file*/
         &idStr,       /*id to name read*/
         &maxDistSL,   /*max edit distance to count*/
         &minQUC,      /*min quality score to keep snp*/
         &minOverlapF, /*min read/reference overlap*/
         &minIndelUI,  /*max indel length to keep indel*/
         &minDepthUI,  /*min depth to print allele*/
         &minPercF,    /*min percent depth to print*/
         &pHeadBl      /*1: print header; 0 do not*/
      ); /*get user input*/

   if(errSC)
   { /*If: had error*/
      --errSC; /*1 from help/version goes to 0; no error*/
      goto ret_main_sec04;
   } /*If: had error*/

   /*****************************************************\
   * Main Sec02 Sub03:
   *   - setup structs
   \*****************************************************/

   if( setup_samEntry(&samStackST) )
   { /*If: memory error on samEntry setup*/
      fprintf(
         stderr,
         "ran out of memory setting up samStackST\n"
      );

      goto err_main_sec04;
   } /*If: memory error on samEntry setup*/

   /*****************************************************\
   * Main Sec02 Sub04:
   *   - open input/output files + get database
   *   o main sec02 sub04 cat01:
   *     - open output file
   *   o main sec02 sub04 cat02:
   *     - get database
   *   o main sec02 sub04 cat03:
   *     - open input sam file
   \*****************************************************/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Main Sec02 Sub04 Cat01:
   +   - open output file
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   if(
         ! outFileStr
      || outFileStr[0] == '-'
   ){ /*If: printing to stdout*/
       outFILE = stdout;
       outFileStr = (signed char *) "-";
   } /*If: printing to stdout*/

   else
   { /*Else: user supplied output file*/
      outFILE =
         fopen(
            (char *) outFileStr,
            "r"
         );

      if(! outFILE)
      { /*If: could not open output file*/
         fprintf(
            stderr,
            "unable to open -out %s\n",
            outFileStr
         );

         goto err_main_sec04;
      } /*If: could not open output file*/
   } /*Else: user supplied output file*/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Main Sec02 Sub04 Cat02:
   +   - get database
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   samFILE =
      fopen(
         (char *) dbFileStr,
         "r"
      );

   if(! samFILE)
   { /*If: could not open database file*/
      fprintf(
         stderr,
         "unable to open -db %s\n",
         dbFileStr
      );

      goto err_main_sec04;
   } /*If: could not open database file*/

   cgMLSTHeapST =
      getLoci_allele_cgMLST(
         samFILE,
         &errSC
      );

   if(! cgMLSTHeapST)
   { /*If: had error*/
      if(errSC == def_memErr_cgMLST)
      { /*If: had memory error*/
         fprintf(
            stderr,
            "ran out of memory reading -db %s\n",
            dbFileStr
         );
      } /*If: had memory error*/

      else
      { /*Else: file error*/
         fprintf(
            stderr,
            "file error reading -db %s\n",
            dbFileStr
         );
      } /*Else: file error*/

      goto err_main_sec04;
   } /*If: had error*/

   if(samFILE != stdin)
      fclose(samFILE);
   samFILE = 0;

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Main Sec02 Sub04 Cat03:
   +   - open input sam file
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   if(
         ! samFileStr
      || samFileStr[0] == '-'
   ){ /*If: input is from stdin*/
      samFILE = stdin;
      samFileStr = (signed char *) "-";
   }  /*If: input is from stdin*/

   else
   { /*Else: user supplied input file*/
      samFILE =
         fopen(
            (char *) samFileStr,
            "r"
         );

      if(! outFILE)
      { /*If: could not open input sam file*/
         fprintf(
            stderr,
            "unable to open -sam %s\n",
            samFileStr
         );

         goto err_main_sec04;
      } /*If: could not open input sam file*/
   } /*Else: user supplied inpu file*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec03:
   ^   - find and print lineage
   ^   o main sec03 sub01:
   ^     - skip headers in sam file
   ^   o main sec03 sub02:
   ^     - get lineages
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Main Sec03 Sub01:
   *   - skip headers in sam file
   \*****************************************************/

   do{ /*Loop: get sam headers*/
      errSC =
         get_samEntry(
            &samStackST,
            &buffHeapStr,
            &buffLenUL,
            samFILE
         );

      if(samStackST.extraStr[0] != '@')
         break; /*end of sam file*/
   } while(! errSC); /*Loop: get sam headers*/

   if(errSC)
   { /*If: had error skipping headers*/
      if(errSC == def_memErr_samEntry)
      { /*If: memory error*/
         fprintf(
            stderr,
            "ran out of memory reading -sam %s headers\n",
            samFileStr
         );
      } /*If: memory error*/

      else
      { /*Else: EOF or file error*/
         fprintf(
            stderr,
            "-sam %s is empty, no reads, or file error\n",
            samFileStr
         );
      } /*Else: EOF or file error*/

      goto err_main_sec04; 
   } /*If: had error skipping headers*/

   /*****************************************************\
   * Main Sec03 Sub02:
   *   - get lineages
   \*****************************************************/

   while(! errSC)
   { /*Loop: find lineage*/
      ++seqSL; /*count number of reads in file*/

      /*check if we have mapped reads and/or sequences*/
      if(samStackST.flagUS & 4)
         goto nextEntry_main_sec03_sub02;

      else if(samStackST.seqStr[0] == '*')
         goto nextEntry_main_sec03_sub02;

      else if(! samStackST.readLenUI)
         goto nextEntry_main_sec03_sub02;

      errSC =
         seqToLoci_allele_cgMLST(
            cgMLSTHeapST,/*has allele table*/
            &samStackST, /*read/sequence to check*/
            maxDistSL,   /*max edit distance for match*/
            minIndelUI,  /*min length to count indel*/
            minQUC,      /*min q-score to count SNP*/
            minOverlapF  /*min overlap to check loci*/
         ); /*add sequence/read to allele table*/

      if(! errSC)
         ++lociSL; /*number sequences with loci*/

      nextEntry_main_sec03_sub02:;
         errSC =
            get_samEntry(
               &samStackST,
               &buffHeapStr,
               &buffLenUL,
               samFILE
            );
   } /*Loop: find lineage*/

   if(errSC != def_EOF_samEntry)
   { /*If: had memory error or file error*/
      fprintf(
         stderr,
         "ran out of memory on read %li in -sam %s\n",
         seqSL,
         samFileStr
      );

      goto err_main_sec04;
   } /*If: had memory error or file error*/

   /*****************************************************\
   * Main Sec03 Sub03:
   *   - print linage
   \*****************************************************/

   plin_allele_cgMLST(
      cgMLSTHeapST,/*has allele table to get lineage*/
      idStr,       /*id to add to first column*/
      minDepthUI,  /*min read depth to print allele*/
      minPercF,    /*min percent support to print allele*/
      pHeadBl,     /*1: print header; 0: do not*/
      outFILE      /*file to print to*/
   ); /*print detected majority lineage*/

   if(outFILE != stdout)
   { /*If: can print number of sequences*/
      fprintf(
         stdout,
         "%li of %li sequences had a least one loci\n",
         lociSL,
         seqSL
      );
   } /*If: can print number of sequences*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec04:
   ^   - report errors, clean up, and return
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   errSC = 0;
   goto ret_main_sec04;

   err_main_sec04:;
      errSC = 1;
      goto ret_main_sec04;

   ret_main_sec04:;

      freeStack_samEntry(&samStackST);

      if(buffHeapStr)
         free(buffHeapStr);
      buffHeapStr = 0;

      if(cgMLSTHeapST)
         freeHeap_allele_cgMLST(cgMLSTHeapST);
      cgMLSTHeapST = 0;

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

      samFILE = 0;

      return errSC;
} /*main*/
