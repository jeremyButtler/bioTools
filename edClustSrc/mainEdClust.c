/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' mainEdClust SOF: Start Of File
'   - clusters reads by edit distance (main/driver)
'   o header:
'     - included libraries and defaults
'   o fun01: pversion_mainEdClust
'     - prints version number for edClust
'   o fun02: phelp_mainEdClust
'     - prints help message for edClust
'   o fun03: input_mainEdClust
'     - gets user input for edClust
'   o fun04: pgHead_mainEdClust
'     - builds program header for edClust
'   o main:
'     - driver function for edClust
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*-------------------------------------------------------\
| Header:
|   - included libraries and defaults
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
#include "../genLib/charCp.h"

#include "../genBio/samEntry.h"

#include "../genBio/tbCon.h"

#include "../genClust/clustST.h"
#include "../genClust/edClust.h"

/*.h files only (no .c files)*/
#include "../genLib/dataTypeShortHand.h"
#include "../genBio/tbConDefs.h"
#include "../bioTools.h"

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\
! Hidden libraries:
!   - .c  #include "../genLib/strAry.h"
!   - .c  #include "../genBio/tbCon.h"
!   - .c  #include "../genBio/edDist.h"
!   - .h  #include "../genLib/genMath.h" .h min macro only
!   - .h  #include "../genBio/ntTo5Bit.h"
\%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

/*-------------------------------------------------------\
| Fun01: pversion_mainEdClust
|   - prints version number for edClust
| Input:
|   - outFILE
|     o file to print version to
| Output:
|   - Prints:
|     o version to outFILE
\-------------------------------------------------------*/
void
pversion_mainEdClust(
   void *outFILE
){
   fprintf(
      (FILE *) outFILE,
      "edClust from bioTools version: %i-%02i-%02i\n",
      def_year_bioTools,
      def_month_bioTools,
      def_day_bioTools
   );
} /*pversion_mainEdClust*/

/*-------------------------------------------------------\
| Fun02: phelp_mainEdClust
|   - prints help message for edClust
| Input:
|   - outFILE
|     o file to print help message to
| Output:
|   - Prints:
|     o help message to outFILE
\-------------------------------------------------------*/
void
phelp_mainEdClust(
   void *outFILE
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun02 TOC:
   '   - prints help message for edClust
   '   o fun02 sec01:
   '     - usage
   '   o fun02 sec02:
   '     - input
   '   o fun02 sec03:
   '     - output
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun02 Sec01:
   ^   - usage
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   fprintf(
      (FILE *) outFILE,
      "edClust -sam reads.sam -prefix \"out\"\n"
   );

   fprintf(
      (FILE *) outFILE,
      "  - clusters reads using edit distance (edDist)\n"
   );

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun02 Sec02:
   ^   - input
   ^   o fun02 sec02 sub01:
   ^     - input header
   ^   o fun02 sec02 sub02:
   ^     - file input and prefix
   ^   o fun02 sec02 sub03:
   ^     - cluster depth, error rate, similarity settings
   ^   o fun02 sec02 sub04:
   ^     - scoring settings (length weight only)
   ^   o fun02 sec02 sub05:
   ^     - read filtering settings
   ^   o fun02 sec02 sub06:
   ^     - edit distance settings; also consensus min snpQ
   ^   o fun02 sec02 sub07:
   ^     - consensus uniuqe settings
   ^   o fun02 sec02 sub08:
   ^     - help message and version number
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
   *   - file input and prefix
   \*****************************************************/

   fprintf(
      (FILE *) outFILE,
      "  FILE IO:\n"
   );

   fprintf(
      (FILE *) outFILE,
      "    -sam reads.sam: [Required]\n"
   );

   fprintf(
      (FILE *) outFILE,
      "      o sam file with reads to cluster\n"
   );


   fprintf(
      (FILE *) outFILE,
      "    -prefix out: [Optional; out]\n"
   );

   fprintf(
      (FILE *) outFILE,
      "      o prefix for output file names\n"
   );

   /*****************************************************\
   * Fun02 Sec02 Sub03:
   *   - cluster depth, error rate, & similarity settings
   \*****************************************************/

   fprintf(
      (FILE *) outFILE,
      "  Cluster Filtering:\n"
   );

   fprintf(
      (FILE *) outFILE,
      "    -clust-depth %i: [Optional; %i]\n",
      def_minDepth_clustST,
      def_minDepth_clustST
   );

   fprintf(
      (FILE *) outFILE,
      "      o min cluster read depth to keep a cluster\n"
   );


   fprintf(
      (FILE *) outFILE,
      "    -clust-perc-depth %.04f: [Optional; %.04f]\n",
      def_minPercDepth_clustST,
      def_minPercDepth_clustST
   );

   fprintf(
      (FILE *) outFILE,
      "      o minimum percent read depth need to keep\n"
   );

   fprintf(
      (FILE *) outFILE,
      "        a cluster\n"
   );

   fprintf(
      (FILE *) outFILE,
      "    -read-err %0.3f: [Optional; %0.3f]\n",
      def_readErrRate_clustST,
      def_readErrRate_clustST
   );

   fprintf(
      (FILE *) outFILE,
      "      o error rate for read to read mapping\n"
   );


   fprintf(
      (FILE *) outFILE,
      "    -con-err %0.3f: [Optional; %0.3f]\n",
      def_conErrRate_clustST,
      def_conErrRate_clustST
   );

   fprintf(
      (FILE *) outFILE,
      "      o consensus to read mapping error rate\n"
   );


   fprintf(
      (FILE *) outFILE,
      "    -con-sim %0.4f: [Optional; %0.4f]\n",
      def_maxConSim_clustST,
      def_maxConSim_clustST
   );

   fprintf(
      (FILE *) outFILE,
      "      o maximum simularity between consensuses\n"
   );

   fprintf(
      (FILE *) outFILE,
      "        to not merge clusters\n"
   );

   fprintf(
      (FILE *) outFILE,
      "    -overlap %0.2f: [Optional; %0.2f]\n",
      def_percOverlap_clustST,
      def_percOverlap_clustST
   );

   fprintf(
      (FILE *) outFILE,
      "      o minimum percentage of overlap needed to\n"
   );

   fprintf(
      (FILE *) outFILE,
      "     merge clusters (start comparision)\n"
   );

   fprintf(
      (FILE *) outFILE,
      "      o used in final consensus comparison\n"
   );



   /*****************************************************\
   * Fun02 Sec02 Sub04:
   *   - scoring settings (length weight only)
   \*****************************************************/

   fprintf(
      (FILE *) outFILE,
      "  Read Scoring:\n"
   );

   fprintf(
      (FILE *) outFILE,
      "    -len-weight %0.2f: [Optional; %0.2f]\n",
      def_lenWeight_clustST,
      def_lenWeight_clustST
   );

   fprintf(
      (FILE *) outFILE,
      "      o how much to increase lengths impact on\n"
   );

   fprintf(
      (FILE *) outFILE,
      "        read scoring\n"
   );

   fprintf(
      (FILE *) outFILE,
      "      o log_length = floor[log10(length)]\n"
   );

   fprintf(
      (FILE *) outFILE,
      "      o length_score= floor[weigth * log_length]\n"
   );

   fprintf(
      (FILE *) outFILE,
      "      o score = length_score + floor[median Q]\n"
   );

   /*****************************************************\
   * Fun02 Sec02 Sub05:
   *   - read filtering settings
   \*****************************************************/

   fprintf(
      (FILE *) outFILE,
      "  Read Filtering:\n"
   );

   fprintf(
      (FILE *) outFILE,
      "    -min-len %i: [Optional; %i]\n",
      def_minLen_clustST,
      def_minLen_clustST
   );

   fprintf(
      (FILE *) outFILE,
      "      o minimum aligned length to keep read\n"
   );


   fprintf(
      (FILE *) outFILE,
      "    -mapq %i: [Optional; %i]\n",
      def_minMapq_clustST,
      def_minMapq_clustST
   );

   fprintf(
      (FILE *) outFILE,
      "      o minimum mapping quality to keep read\n"
   );


   fprintf(
      (FILE *) outFILE,
      "    -min-med-q %i: [Optional; %i]\n",
      def_minMedQ_clustST,
      def_minMedQ_clustST
   );

   fprintf(
      (FILE *) outFILE,
      "      o minimum median q-score to keep read\n"
   );


   fprintf(
      (FILE *) outFILE,
      "    -min-mean-q %i: [Optional; %i]\n",
      def_minAvgQ_clustST,
      def_minAvgQ_clustST
   );

   fprintf(
      (FILE *) outFILE,
      "      o minimum mean q-score to keep read\n"
   );

   /*****************************************************\
   * Fun02 Sec02 Sub06:
   *   - edit distance settings
   \*****************************************************/

   fprintf(
      (FILE *) outFILE,
      "  Edit Distance:\n"
   );

   if(def_depthProfile_clustST)
      fprintf(
         (FILE *) outFILE,
         "    -depth-prof: [Optional; Yes]\n"
      );
   else
      fprintf(
         (FILE *) outFILE,
         "    -depth-prof: [Optional; No]\n"
      );

   fprintf(
      (FILE *) outFILE,
      "      o -clust-depth to remove low read depth\n"
   );

   fprintf(
      (FILE *) outFILE,
      "        variants from edit distance\n"
   );

   fprintf(
      (FILE *) outFILE,
      "      o disable with \"no-depth-prof\"\n"
   );


   fprintf(
      (FILE *) outFILE,
      "    -err-to-var %i: [Optional; %i]\n",
      def_maxVarErrRatio_clustST,
      def_maxVarErrRatio_clustST
   );

   fprintf(
      (FILE *) outFILE,
      "      o maximum variant to error ratio to add\n"
   );

   fprintf(
      (FILE *) outFILE,
      "        read to a cluster (whole number)\n"
   );

   fprintf(
      (FILE *) outFILE,
      "      o 100 * (# variants)/(error rate * length)\n"
   );



   fprintf(
      (FILE *) outFILE,
      "    -win-len %i: [Optional; %i]\n",
      def_window_clustST,
      def_window_clustST
   );

   fprintf(
      (FILE *) outFILE,
      "      o size of window for window scanning\n"
   );

   fprintf(
      (FILE *) outFILE,
      "      o idea is to detect regions of high\n"
   );

   fprintf(
      (FILE *) outFILE,
      "        difference in very long reads\n"
   );


   fprintf(
      (FILE *) outFILE,
      "    -win-err %i: [Optional; %i]\n",
      def_windowError_clustST,
      def_windowError_clustST
   );

   fprintf(
      (FILE *) outFILE,
      "      o maximum variant to error ratio in window\n"
   );



   fprintf(
      (FILE *) outFILE,
      "    -indel-len %i: [Optional; %i]\n",
      def_indelLen_clustST,
      def_indelLen_clustST
   );

   fprintf(
      (FILE *) outFILE,
      "      o min length to add indel to edit distance\n"
   );


   fprintf(
      (FILE *) outFILE,
      "    -clust-q-snp %i: [Optional; %i]\n",
      def_minSnpQ_clustST,
      def_minSnpQ_clustST
   );

   fprintf(
      (FILE *) outFILE,
      "      o minimum q-score to keep snp (edit dist)\n"
   ); /*edit distance min snp q-score*/


   /*****************************************************\
   * Fun02 Sec02 Sub07:
   *   - cosensus unqiue settings
   \*****************************************************/

   fprintf(
      (FILE *) outFILE,
      "  Consensus Filtering + Building:\n"
   );

   fprintf(
      (FILE *) outFILE,
      "    -con-iter %i: [Optional; %i]\n",
      def_conRebuild_clustST,
      def_conRebuild_clustST
   );

   fprintf(
      (FILE *) outFILE,
      "      o number of consensus rebuilds and remaps\n"
   );


   fprintf(
      (FILE *) outFILE,
      "    -perc-n %0.4f: [Optional; %0.4f]\n",
      def_maxNPerc_clustST,
      def_maxNPerc_clustST
   );

   fprintf(
      (FILE *) outFILE,
      "      o maximum percent masked consnesus bases\n"
   ); /*both consensus and edit distance*/


   fprintf(
      (FILE *) outFILE,
      "    -con-depth %i: [Optional; %i]\n",
      def_minDepth_tbConDefs,
      def_minDepth_tbConDefs
   );

   fprintf(
      (FILE *) outFILE,
      "      o minimum read depth to not mask\n"
   );


   fprintf(
      (FILE *) outFILE,
      "    -con-q-snp %i: [Optional; %i]\n",
      def_minSnpQ_clustST,
      def_minSnpQ_clustST
   );


   fprintf(
      (FILE *) outFILE,
      "      o minimum q-score for snp in consensus\n"
   ); /*both consensus and edit distance*/

   fprintf(
      (FILE *) outFILE,
      "    -q-ins %i: [Optional; %i]\n",
      def_minInsQ_tbConDefs,
      def_minInsQ_tbConDefs
   );

   fprintf(
      (FILE *) outFILE,
      "      o minimum q-score for ins (consensus only)\n"
   );


   fprintf(
      (FILE *) outFILE,
      "    -snp %0.2f: [Optional; %0.2f]\n",
      def_minSnpPerc_tbConDefs,
      def_minSnpPerc_tbConDefs
   );

   fprintf(
      (FILE *) outFILE,
      "      o minimum percent support to keep SNP\n"
   );

   fprintf(
      (FILE *) outFILE,
      "        or a match\n"
   );


   fprintf(
      (FILE *) outFILE,
      "    -ins %0.2f: [Optional; %0.2f]\n",
      def_minInsPerc_tbConDefs,
      def_minInsPerc_tbConDefs
   );

   fprintf(
     (FILE *) outFILE,
     "      o minimum percent support to keep insertion\n"
   );


   fprintf(
      (FILE *) outFILE,
      "    -del %0.2f: [Optional; %0.2f]\n",
      def_minDelPerc_tbConDefs,
      def_minDelPerc_tbConDefs
   );

   fprintf(
      (FILE *) outFILE,
      "      o minimum percent support to keep deletion\n"
   );

   /*****************************************************\
   * Fun02 Sec02 Sub08:
   *   - help message and version number
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
   ^   - output
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   fprintf(
       (FILE *) outFILE,
       "Output:\n"
   );

   fprintf(
      (FILE *) outFILE,
      "   - reads in cluster to prefix-ref-cluster.sam\n"
   );

   fprintf(
      (FILE *) outFILE,
      "   - consenseuses as prefix-ref-cluster-con.sam\n"
   );
} /*phelp_mainEdClust*/

/*-------------------------------------------------------\
| Fun03: input_mainEdClust
|   - gets user input for edClust
| Input:
|   - numArgsSI:
|     o number of arguments user input
|   - argAryStr:
|     o pointer to c-string array with user input
|   - samFileStrPtr:
|     o pointer to c-string to be set to sam file entry in
|       argAryStr
|   - prefixStrPtr:
|     o pointer to c-string to be set to prefix entry in
|       argAryStr
|   - setSTPtr:
|     o pointer to set_clustST struct to hold user input
|       settings for edClust
|   - conSetSTPtr:
|     o pointer to set_tbCon struct to hold settings for
|       consensus building
| Output:
|   - Modifies:
|     o all input, except numArgsSI and argAryStr to
|       hold/point to user input
|   - Prints:
|     o error messages to stderr
|     o help message or version number to stdout;if wanted
|   - Returns:
|     o 0 for no errors
|     o 1 if printed help message or version number
|     o 2 if had error
\-------------------------------------------------------*/
signed char
input_mainEdClust(
   int numArgsSI,
   char *argAryStr[],
   signed char **samFileStrPtr,
   signed char **prefixStrPtr,
   struct set_clustST *setSTPtr,
   struct set_tbCon *conSetSTPtr
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun03 TOC:
   '   - gets user input for edClust
   '   o fun03 sec01:
   '     - varaible declarations
   '   o fun03 sec02:
   '     - get input
   '   o fun03 sec03:
   '     - return result
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec01:
   ^   - varaible declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   sint siArg = 1;
   schar errSC = 0;
   schar *tmpStr = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec02:
   ^   - get input
   ^   o fun03 sec02 sub01:
   ^     - check if have input, start loop, file IO vars
   ^   o fun03 sec02 sub02:
   ^     - clust depth, error rate, & simularity settings
   ^   o fun03 sec02 sub03:
   ^     - scoring setting (length weight only)
   ^   o fun03 sec02 sub04:
   ^     - read filtering settings
   ^   o fun03 sec02 sub05:
   ^     - edit distance + consensus snp-q
   ^   o fun03 sec02 sub06:
   ^     - consensus settings
   ^   o fun03 sec02 sub07:
   ^     - help message
   ^   o fun03 sec02 sub08:
   ^     - version number
   ^   o fun03 sec02 sub09:
   ^     - invalid input
   ^   o fun03 sec02 sub10:
   ^     - move to next argument
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun03 Sec02 Sub01:
   *   - check if have input, start loop, file IO vars
   \*****************************************************/

   if(numArgsSI <= 1)
      goto phelp_fun03_sec03; /*nothing input*/

   while(siArg < numArgsSI)
   { /*Loop: get input*/
      if(
         ! eql_charCp(
            (schar *) "-sam",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*If: sam file input*/
         ++siArg;
         *samFileStrPtr = (schar *) argAryStr[siArg];
      } /*If: sam file input*/

      else if(
         ! eql_charCp(
            (schar *) "-prefix",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else If: prefix input*/
         ++siArg;
         *prefixStrPtr = (schar *) argAryStr[siArg];
      } /*Else If: prefix input*/

      /**************************************************\
      * Fun03 Sec02 Sub02:
      *   - clust depth, error rate, & simularity settings
      \**************************************************/

      else if(
         ! eql_charCp(
            (schar *) "-depth-prof",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ) setSTPtr->depthProfBl = 1;

      else if(
         ! eql_charCp(
            (schar *) "-no-depth-prof",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ) setSTPtr->depthProfBl = 0;

      else if(
         ! eql_charCp(
            (schar *) "-clust-depth",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else If: min cluster read depth*/
         ++siArg;
         tmpStr = (schar *) argAryStr[siArg];

         tmpStr +=
            strToUI_base10str(
                tmpStr,
                &setSTPtr->minDepthUI
            );

         if(*tmpStr != '\0')
         { /*If: invalid input*/
            fprintf(
               stderr,
               "-clust-depth %s; non-numeric/to large\n",
               argAryStr[siArg]
            );

            goto err_fun03_sec03;
         } /*If: invalid input*/
      } /*Else If: min cluster read depth*/


      else if(
         ! eql_charCp(
            (schar *) "-clust-perc-depth",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else If: cluster min percent read depth*/
         ++siArg;
         setSTPtr->minPercDepthF = atof(argAryStr[siArg]);
      } /*Else If: cluster min percent read depth*/

      else if(
         ! eql_charCp(
            (schar *) "-read-err",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else If: error rate for read to read map*/
         ++siArg;

         setSTPtr->readErrRateF =
            atof(argAryStr[siArg]);
      } /*Else If: error rate for read to read map*/


      else if(
         ! eql_charCp(
            (schar *) "-con-err",
            (schar *) argAryStr[siArg],
            (schar) '\0'
        )
     ){ /*Else If: error rate for con to read map*/
        ++siArg;
        setSTPtr->conErrRateF = atof(argAryStr[siArg]);
     } /*Else If: error rate for con to read map*/


      else if(
         ! eql_charCp(
            (schar *) "-con-sim",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else If: maximum similarity between cons*/
         ++siArg;
         setSTPtr->maxConSimF = atof(argAryStr[siArg]);
      } /*Else If: maximum similarity between cons*/

      /**************************************************\
      * Fun03 Sec02 Sub03:
      *   - scoring setting (length weight only)
      \**************************************************/

      else if(
         ! eql_charCp(
            (schar *) "-len-weight",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else If: how much length influces score*/
         ++siArg;
         setSTPtr->lenWeightF = atof(argAryStr[siArg]);
      } /*Else If: how much length influces score*/

      /**************************************************\
      * Fun03 Sec02 Sub04:
      *   - read filtering settings
      *   o fun03 sec02 sub04 cat01:
      *     - min read length
      *   o fun03 sec02 sub04 cat02:
      *     - min mapq
      *   o fun03 sec02 sub04 cat03:
      *     - min median q
      *   o fun03 sec02 sub04 cat04:
      *     - min mean q
      \**************************************************/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun03 Sec02 Sub04 Cat01:
      +   - min read length
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      else if(
         ! eql_charCp(
            (schar *) "-min-len",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else If: minimum read length*/
         ++siArg;
         tmpStr = (schar *) argAryStr[siArg];

         tmpStr +=
            strToUI_base10str(
                tmpStr,
                &setSTPtr->minLenUI
            );

         if(*tmpStr != '\0')
         { /*If: invalid input*/
            fprintf(
               stderr,
               "-min-len %s; non-numeric or to large\n",
               argAryStr[siArg]
            );

            goto err_fun03_sec03;
         } /*If: invalid input*/

         conSetSTPtr->minLenSI = setSTPtr->minLenUI;
      } /*Else If: minimum read length*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun03 Sec02 Sub04 Cat02:
      +   - min mapq
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      else if(
         ! eql_charCp(
            (schar *) "-min-mapq",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else If: minimum mapping quality*/
         ++siArg;
         tmpStr = (schar *) argAryStr[siArg];

         tmpStr +=
            strToUC_base10str(
                tmpStr,
                &setSTPtr->minMapqUC
            );

         if(*tmpStr != '\0')
         { /*If: invalid input*/
            fprintf(
               stderr,
               "-min-mapq %s; non-numeric or to large\n",
               argAryStr[siArg]
            );

            goto err_fun03_sec03;
         } /*If: invalid input*/

         conSetSTPtr->minMapqUC = setSTPtr->minMapqUC;
      } /*Else If: minimum mapping quality*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun03 Sec02 Sub04 Cat03:
      +   - min median q
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      else if(
         ! eql_charCp(
            (schar *) "-min-med-q",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else If: minimum median q-score*/
         ++siArg;
         tmpStr = (schar *) argAryStr[siArg];

         tmpStr +=
            strToUI_base10str(
                tmpStr,
                &setSTPtr->minMedQUI
            );

         if(*tmpStr != '\0')
         { /*If: invalid input*/
            fprintf(
               stderr,
               "-min-med-q %s; non-numeric / to large\n",
               argAryStr[siArg]
            );

            goto err_fun03_sec03;
         } /*If: invalid input*/
      } /*Else If: minimum median q-score*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun03 Sec02 Sub04 Cat04:
      +   - min mean q
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      else if(
         ! eql_charCp(
            (schar *) "-min-mean-q",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else If: minimu mean q-score*/
         ++siArg;
         tmpStr = (schar *) argAryStr[siArg];

         tmpStr +=
            strToUI_base10str(
                tmpStr,
                &setSTPtr->minAvgQUI
            );

         if(*tmpStr != '\0')
         { /*If: invalid input*/
            fprintf(
               stderr,
               "-min-mean-q %s; non-numeric / to large\n",
               argAryStr[siArg]
            );

            goto err_fun03_sec03;
         } /*If: invalid input*/
      } /*Else If: minimum mean q-score*/

      /**************************************************\
      * Fun03 Sec02 Sub05:
      *   - edit distance + consensus snp-q
      *   o fun03 sec02 sub05 cat01:
      *     - max error to variant ratio
      *   o fun03 sec02 sub05 cat02:
      *     - minimum overlap
      *   o fun03 sec02 sub05 cat03:
      *     - window size
      *   o fun03 sec02 sub05 cat05:
      *     - min indel length
      *   o fun03 sec02 sub05 cat06:
      *     - min snp q-score (edDist and tbCon)
      \**************************************************/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun03 Sec02 Sub05 Cat01:
      +   - max error to variant ratio
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      else if(
         ! eql_charCp(
            (schar *) "-err-to-var",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else If: maximum error to variant ratio*/
         ++siArg;
         tmpStr = (schar *) argAryStr[siArg];

         tmpStr +=
            strToUI_base10str(
                tmpStr,
                &setSTPtr->varToErrUI
            );

         if(*tmpStr != '\0')
         { /*If: invalid input*/
            fprintf(
               stderr,
               "-err-to-var %s; non-numeric / to large\n",
               argAryStr[siArg]
            );

            goto err_fun03_sec03;
         } /*If: invalid input*/
      } /*Else If: maximum error to variant ratio*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun03 Sec02 Sub05 Cat02:
      +   - minimum overlap
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      else if(
         ! eql_charCp(
            (schar *) "-overlap",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else If: minimum overlap*/
         ++siArg;

         setSTPtr->percOverlapF =
            atof(argAryStr[siArg]);

         if(setSTPtr->percOverlapF == 0)
         { /*If: invalid input*/
            fprintf(
               stderr,
               "-overlap %s goes to 0; invalid input\n",
               argAryStr[siArg]
            );

            goto err_fun03_sec03;
         } /*If: invalid input*/
      } /*Else If: minimum overlap*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun03 Sec02 Sub05 Cat03:
      +   - window size
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      else if(
         ! eql_charCp(
            (schar *) "-win-len",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else If: window length for window scanning*/
         ++siArg;
         tmpStr = (schar *) argAryStr[siArg];

         tmpStr +=
            strToUI_base10str(
                tmpStr,
                &setSTPtr->winSizeUI
            );

         if(*tmpStr != '\0')
         { /*If: invalid input*/
            fprintf(
               stderr,
               "-win-len %s; non-numeric / to large\n",
               argAryStr[siArg]
            );

            goto err_fun03_sec03;
         } /*If: invalid input*/
      } /*Else If: window length for window scanning*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun03 Sec02 Sub05 Cat04:
      +   - window error to var ratio
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      else if(
         ! eql_charCp(
            (schar *) "-win-err",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else If: window error to varaint ratio*/
         ++siArg;
         tmpStr = (schar *) argAryStr[siArg];

         tmpStr +=
            strToUI_base10str(
                tmpStr,
                &setSTPtr->winErrUI
            );

         if(*tmpStr != '\0')
         { /*If: invalid input*/
            fprintf(
               stderr,
               "-win-err %s; non-numeric / to large\n",
               argAryStr[siArg]
            );

            goto err_fun03_sec03;
         } /*If: invalid input*/
      } /*Else If: window error to varaint ratio*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun03 Sec02 Sub05 Cat05:
      +   - min indel length
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      else if(
         ! eql_charCp(
            (schar *) "-indel-len",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else If: min indel length*/
         ++siArg;
         tmpStr = (schar *) argAryStr[siArg];

         tmpStr +=
            strToUI_base10str(
                tmpStr,
                &setSTPtr->indelLenUI
            );

         if(*tmpStr != '\0')
         { /*If: invalid input*/
            fprintf(
               stderr,
               "-indel-len %s; non-numeric / to large\n",
               argAryStr[siArg]
            );

            goto err_fun03_sec03;
         } /*If: invalid input*/
      } /*Else If: min indel length*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun03 Sec02 Sub05 Cat05:
      +   - min indel length
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      else if(
         ! eql_charCp(
            (schar *) "-indel-len",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else If: min indel length*/
         ++siArg;
         tmpStr = (schar *) argAryStr[siArg];

         tmpStr +=
            strToUI_base10str(
                tmpStr,
                &setSTPtr->indelLenUI
            );

         if(*tmpStr != '\0')
         { /*If: invalid input*/
            fprintf(
               stderr,
               "-indel-len %s; non-numeric / to large\n",
               argAryStr[siArg]
            );

            goto err_fun03_sec03;
         } /*If: invalid input*/
      } /*Else If: min indel length*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun03 Sec02 Sub05 Cat06:
      +   - min snp q-score (edDist and tbCon)
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      else if(
         ! eql_charCp(
            (schar *) "-clust-q-snp",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else If: min snp q-score*/
         ++siArg;
         tmpStr = (schar *) argAryStr[siArg];

         tmpStr +=
            strToUC_base10str(
                tmpStr,
                &setSTPtr->minSnpQUC
            );

         if(*tmpStr != '\0')
         { /*If: invalid input*/
            fprintf(
               stderr,
               "-q-snp %s; non-numeric or to large\n",
               argAryStr[siArg]
            );

            goto err_fun03_sec03;
         } /*If: invalid input*/
      } /*Else If: min snp q-score*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun03 Sec02 Sub05 Cat07:
      +   - maximum percent of consensus masked
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      else if(
         ! eql_charCp(
            (schar *) "-perc-n",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else If: maximum % masking*/
         ++siArg;
         setSTPtr->maxNPercF = atof(argAryStr[siArg]);
      } /*Else If: maximum % masking*/

      /**************************************************\
      * Fun03 Sec02 Sub06:
      *   - consensus settings
      *   o fun03 sec02 sub06 cat01:
      *     - min read depth (before masking)
      *   o fun03 sec02 sub06 cat02:
      *     - min consensus snp q-score
      *   o fun03 sec02 sub06 cat03:
      *     - min insertion q-score
      *   o fun03 sec02 sub06 cat04:
      *     - min snp, ins, and del support (as percent)
      \**************************************************/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun03 Sec02 Sub06 Cat01:
      +   - number times to rebuild consensus
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      else if(
         ! eql_charCp(
            (schar *) "-con-iter",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else If: number times to rebuild consensus*/
         ++siArg;
         tmpStr = (schar *) argAryStr[siArg];

         tmpStr +=
            strToUC_base10str(
                tmpStr,
                &setSTPtr->conRebuildUC
            );

         if(*tmpStr != '\0')
         { /*If: invalid input*/
            fprintf(
               stderr,
               "-con-iter %s; non-numeric / to large\n",
               argAryStr[siArg]
            );

            goto err_fun03_sec03;
         } /*If: invalid input*/
      } /*Else If: number times to rebuild consensus*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun03 Sec02 Sub06 Cat01:
      +   - min read depth (before masking)
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      else if(
         ! eql_charCp(
            (schar *) "-con-depth",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else If: min consnesus read depth*/
         ++siArg;
         tmpStr = (schar *) argAryStr[siArg];

         tmpStr +=
            strToSI_base10str(
                tmpStr,
                &conSetSTPtr->minDepthSI
            );

         if(*tmpStr != '\0')
         { /*If: invalid input*/
            fprintf(
               stderr,
               "-con-depth %s; non-numeric / to large\n",
               argAryStr[siArg]
            );

            goto err_fun03_sec03;
         } /*If: invalid input*/
      } /*Else If: min consnesus read depth*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun03 Sec02 Sub06 Cat02:
      +   - min consensus snp q-score
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      else if(
         ! eql_charCp(
            (schar *) "-con-q-snp",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else If: min snp q-score*/
         ++siArg;
         tmpStr = (schar *) argAryStr[siArg];

         tmpStr +=
            strToSI_base10str(
                tmpStr,
                &conSetSTPtr->minQSI
            );

         if(*tmpStr != '\0')
         { /*If: invalid input*/
            fprintf(
               stderr,
               "-con-q-snp %s; non-numeric or to large\n",
               argAryStr[siArg]
            );

            goto err_fun03_sec03;
         } /*If: invalid input*/

      } /*Else If: min snp q-score*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun03 Sec02 Sub06 Cat03:
      +   - min insertion q-score
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      else if(
         ! eql_charCp(
            (schar *) "-q-ins",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else If: min insetion q-score*/
         ++siArg;
         tmpStr = (schar *) argAryStr[siArg];

         tmpStr +=
            strToSI_base10str(
                tmpStr,
                &conSetSTPtr->minInsQSI
            );

         if(*tmpStr != '\0')
         { /*If: invalid input*/
            fprintf(
               stderr,
               "-q-ins %s; non-numeric or to large\n",
               argAryStr[siArg]
            );

            goto err_fun03_sec03;
         } /*If: invalid input*/
      } /*Else If: min insetion q-score*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun03 Sec02 Sub06 Cat04:
      +   - min snp support (as percent)
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      else if(
         ! eql_charCp(
            (schar *) "-snp",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else If: min snp support (as percent)*/
         ++siArg;

         conSetSTPtr->minPercSnpF =
            atof(argAryStr[siArg]);
      } /*Else If: min snp support (as percent)*/

      else if(
         ! eql_charCp(
            (schar *) "-ins",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else If: min ins support (as percent)*/
         ++siArg;

         conSetSTPtr->minPercInsF =
            atof(argAryStr[siArg]);
      } /*Else If: min ins support (as percent)*/

      else if(
         ! eql_charCp(
            (schar *) "-del",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else If: min del support (as percent)*/
         ++siArg;

         conSetSTPtr->minPercDelF =
            atof(argAryStr[siArg]);
      } /*Else If: min del support (as percent)*/

      /**************************************************\
      * Fun03 Sec02 Sub07:
      *   - help message
      \**************************************************/

      else if(
         ! eql_charCp(
            (schar *) "-h",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ) goto phelp_fun03_sec03;

      else if(
         ! eql_charCp(
            (schar *) "--h",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ) goto phelp_fun03_sec03;

      else if(
         ! eql_charCp(
            (schar *) "help",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ) goto phelp_fun03_sec03;

      else if(
         ! eql_charCp(
            (schar *) "-help",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ) goto phelp_fun03_sec03;

      else if(
         ! eql_charCp(
            (schar *) "--help",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ) goto phelp_fun03_sec03;

      /**************************************************\
      * Fun03 Sec02 Sub08:
      *   - version number
      \**************************************************/

      else if(
         ! eql_charCp(
            (schar *) "-v",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ) goto pversion_fun03_sec03;

      else if(
         ! eql_charCp(
            (schar *) "--v",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ) goto pversion_fun03_sec03;

      else if(
         ! eql_charCp(
            (schar *) "version",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ) goto pversion_fun03_sec03;

      else if(
         ! eql_charCp(
            (schar *) "-version",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ) goto pversion_fun03_sec03;

      else if(
         ! eql_charCp(
            (schar *) "--version",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ) goto pversion_fun03_sec03;

      /**************************************************\
      * Fun03 Sec02 Sub09:
      *   - invalid input
      \**************************************************/

      else
      { /*Else: invalid input*/
         fprintf(
            stderr,
            "%s is not recoginized\n",
            argAryStr[siArg]
         );

         goto err_fun03_sec03;
      } /*Else: invalid input*/

      /**************************************************\
      * Fun03 Sec02 Sub10:
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

   phelp_fun03_sec03:;
   phelp_mainEdClust(stdout);
   goto helpVersion_fun03_sec03;

   pversion_fun03_sec03:;
   pversion_mainEdClust(stdout);
   goto helpVersion_fun03_sec03;

   helpVersion_fun03_sec03:;
   errSC = 1;
   goto ret_fun03_sec03;

   err_fun03_sec03:;
   errSC = 2;
   goto ret_fun03_sec03;

   ret_fun03_sec03:;
   return errSC;
} /*input_mainEdClust*/

/*-------------------------------------------------------\
| Fun04: pgHead_mainEdClust
|   - builds program header for edClust
| Input:
|   - pgHeadStr:
|     o c-string to hold edClust header
|   - numArgsSI:
|     o number of arguments user input
|   - argAryStr:
|     o arguments user input
\-------------------------------------------------------*/
void
pgHead_mainEdClust(
   signed char pgHeadStr[], /*will have header*/
   signed int numArgsSI,    /*number arguments input*/
   char *argAryStr[]        /*arguments input*/
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun04 TOC:
   '   - builds program header for edClust
   '   o fun04 sec01:
   '     - variable declarations
   '   o fun04 sec02:
   '     - build program id and version number
   '   o fun04 sec03:
   '     - build program command part of header
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun04 Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   schar *tmpStr = 0;
   sint siArg = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun04 Sec02:
   ^   - build program id and version number
   ^   o fun04 sec02 sub01:
   ^     - build program id
   ^   o fun04 sec02 sub02:
   ^     - add version number
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun04 Sec02 Sub01:
   *   - build program id
   \*****************************************************/

   tmpStr = pgHeadStr;

   tmpStr +=
      cpDelim_ulCp(
         tmpStr,
         (schar *)
            "@PG\tID:edClust\tPN:edClust\tVN:bioTools_",
         0,
         0
      );

   /*****************************************************\
   * Fun04 Sec02 Sub02:
   *   - add version number
   \*****************************************************/

   tmpStr +=
      numToStr(
         tmpStr,
         def_year_bioTools
      );

   *tmpStr++ = '-';

   if(def_month_bioTools > 9)
   { /*If: month has two digits*/
      *tmpStr++ = 48 + (def_month_bioTools / 10);
      *tmpStr++ = 48 + (def_month_bioTools % 10);
   } /*If: month has two digits*/

   else
   { /*Else: month has one digit*/
      *tmpStr++ = '0';
      *tmpStr++ = 48 + def_month_bioTools;
   } /*Else: month has one digit*/

   *tmpStr++ = '-';

   if(def_day_bioTools > 9)
   { /*If: day has two digits*/
      *tmpStr++ = 48 + (def_day_bioTools / 10);
      *tmpStr++ = 48 + (def_day_bioTools % 10);
   } /*If: day has two digits*/

   else
   { /*Else: day has one digit*/
      *tmpStr++ = '0';
      *tmpStr++ = 48 + def_day_bioTools;
   } /*Else: day has one digit*/

   *tmpStr++ = '\t';

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun04 Sec03:
   ^   - build program command part of header
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   *tmpStr++ = 'C';
   *tmpStr++ = 'L';
   *tmpStr++ = ':';

   *tmpStr++ = 'e';
   *tmpStr++ = 'd';
   *tmpStr++ = 'C';
   *tmpStr++ = 'l';
   *tmpStr++ = 'u';
   *tmpStr++ = 's';
   *tmpStr++ = 't';

   for(
      siArg = 1;
      siArg < numArgsSI;
      ++siArg
   ){ /*Loop: copy user input*/
      *tmpStr++ = ' ';

      tmpStr +=
         cpDelim_charCp(
            tmpStr,
            (schar *) argAryStr[siArg],
            (schar) '\0'
         ); /*copy user input*/
   } /*Loop: copy user input*/

   *tmpStr++ = '\n';
   *tmpStr = '\0';
} /*pgHead_mainEdClust*/

/*-------------------------------------------------------\
| Main:
|   - driver function for edClust
| Input:
|   - numArgsSI:
|     o number input arguments (c provides)
|   - argAryStr:
|     o c-string array with user input (c provides)
| Output;
|   - prints clusters and cluster consnesus to output
|     files
\-------------------------------------------------------*/
int
main(
   int numArgsSI,
   char *argAryStr[]
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Main TOC:
   '   - driver function for edClust
   '   o main sec01:
   '     - variable declarations
   '   o main sec02:
   '     - initialize, get input, setup, open files
   '   o main sec03:
   '     - get headers (sam file and edClust)
   '   o main sec04:
   '     - cluster reads
   '   o main sec0x:
   '     - clean up
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   schar errSC = 0;

   schar *samFileStr = 0;
   schar *prefixStr = (schar *) "out";

   schar *tmpStr = 0;         /*for editing strings*/
   schar *samHeadHeapStr = 0; /*holds sam file header*/

   schar pgHeadStr[4096]; /*program header*/

   /*for sam file reading functions*/
   struct samEntry samStackST;
   schar *buffHeapStr = 0;
   ulong lenBuffUL = 0;

   /*settings for edClust*/
   struct set_clustST clustSetStackST;
   struct set_tbCon conSetStackST;

   /*for clustering*/
   struct con_clustST *conHeapListST = 0; /*consensuses*/
   struct index_clustST *indexHeapST = 0; /*has clusters*/

   FILE *samFILE = 0;

   schar fileNameStr[4096];
   FILE *logFILE = 0;
   FILE *conFILE = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec02:
   ^   - initialize, get input, setup, open files
   ^   o main sec02 sub01:
   ^     - initialize variables
   ^   o main sec02 sub02:
   ^     - get input
   ^   o main sec02 sub03:
   ^     - setup memory
   ^   o main sec02 sub03:
   ^     - open sam file
   ^   o main sec02 sub05:
   ^     - open log file
   ^   o main sec02 sub06:
   ^     - open consensus file
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Main Sec02 Sub01:
   *   - initialize variables
   \*****************************************************/

   init_set_clustST(&clustSetStackST);
   init_set_tbCon(&conSetStackST);

   conSetStackST.minLenSI = clustSetStackST.minLenUI;
   conSetStackST.minMapqUC = clustSetStackST.minMapqUC;
   conSetStackST.minQSI = clustSetStackST.minSnpQUC;

   init_samEntry(&samStackST);

   /*****************************************************\
   * Main Sec02 Sub02:
   *   - get input
   \*****************************************************/

   errSC =
      input_mainEdClust(
         numArgsSI,
         argAryStr,
         &samFileStr,
         &prefixStr,
         &clustSetStackST,
         &conSetStackST
      );

   if(errSC)
   { /*If: error*/
      --errSC; /*make help/version request no error*/
      goto cleanUp_main_sec0x;
   } /*If: error*/

   /*****************************************************\
   * Main Sec02 Sub03:
   *   - setup memory
   \*****************************************************/

   errSC = setup_samEntry(&samStackST);

   if(errSC)
   { /*If: memory error*/
      fprintf(
         stderr,
         "memory error setting up sam entry struct\n"
      );

      goto err_main_sec0x;
   } /*If: memory error*/

   /*****************************************************\
   * Main Sec02 Sub04:
   *   - open sam file
   \*****************************************************/

   if(! samFileStr)
   { /*If: nothing input*/
      fprintf(
         stderr,
         "no sam input with -sam\n"
       );

      goto err_main_sec0x;
   } /*If: nothing input*/

   samFILE =
      fopen(
         (char *) samFileStr,
         "r"
      );

   if(! samFILE)
   { /*If: could not open file*/
      if(! samFileStr)
         fprintf(
            stderr,
            "no sam input with -sam\n"
          );
       else
         fprintf(
            stderr,
            "could not open -sam %s\n",
            samFileStr
          );

      goto err_main_sec0x;
   } /*If: could not open file*/

   /*****************************************************\
   * Main Sec02 Sub05:
   *   - open log file
   \*****************************************************/

   tmpStr = fileNameStr;

   tmpStr +=
      cpDelim_ulCp(
         tmpStr,
         prefixStr,
         0,
         0
      );

   *tmpStr++ = '-';
   *tmpStr++ = 'l';
   *tmpStr++ = 'o';
   *tmpStr++ = 'g';
   *tmpStr++ = '.';
   *tmpStr++ = 't';
   *tmpStr++ = 'x';
   *tmpStr++ = 't';
   *tmpStr = '\0';

   logFILE =
      fopen(
         (char *) fileNameStr,
         "w"
      );

    if(! logFILE)
    { /*If: could not open log file*/
       fprintf(
          stderr,
          "could not open %s; %s missing permission\n",
          fileNameStr,
          prefixStr
       );

       goto err_main_sec0x;
    } /*If: could not open log file*/

    pversion_mainEdClust(logFILE);

   /*****************************************************\
   * Main Sec02 Sub06:
   *   - open consensus file
   \*****************************************************/

   tmpStr = fileNameStr;

   tmpStr +=
      cpDelim_ulCp(
         tmpStr,
         prefixStr,
         0,
         0
      );

   *tmpStr++ = '-';
   *tmpStr++ = 'c';
   *tmpStr++ = 'o';
   *tmpStr++ = 'n';
   *tmpStr++ = '.';
   *tmpStr++ = 's';
   *tmpStr++ = 'a';
   *tmpStr++ = 'm';
   *tmpStr = '\0';

   conFILE =
      fopen(
         (char *) fileNameStr,
         "w"
      );

    if(! logFILE)
    { /*If: could not open consensus file*/
       fprintf(
          stderr,
          "could not open %s; %s missing permission\n",
          fileNameStr,
          prefixStr
       );

       goto err_main_sec0x;
    } /*If: could not open consensus file*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec03:
   ^   - get headers (sam file and edClust)
   ^   o main sec03 sub01:
   ^     - get sam file headers
   ^   o main sec03 sub02:
   ^     - get edClust header
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Main Sec03 Sub01:
   *   - get sam file headers
   \*****************************************************/

   samHeadHeapStr =
      getHead_samEntry(
         &samStackST,
         &buffHeapStr,
         &lenBuffUL,
         samFILE
      );

   if(! samHeadHeapStr)
   { /*If: memory error*/
      fprintf(
         logFILE,
         "memory error geting sam file header\n"
      );

      fprintf(
         stderr,
         "memory error geting sam file header\n"
      );

      goto err_main_sec0x;
   } /*If: memory error*/

   fseek(
      samFILE,
      0,
      SEEK_SET
   ); /*go back to start of file*/

   /*****************************************************\
   * Main Sec03 Sub02:
   *   - get edClust header
   \*****************************************************/

   pgHead_mainEdClust(
      pgHeadStr,       /*will have program header*/
      numArgsSI,       /*number arguments input*/
      argAryStr        /*arguments input*/
   );

   /*get the command part of the header*/
   tmpStr = pgHeadStr;

   while(
         tmpStr[0] != 'C'
      && tmpStr[1] != 'L'
      && tmpStr[2] != ':'
   ) ++tmpStr;

   tmpStr += 3;

   fprintf(
      logFILE,
      "%s\n\n",
      tmpStr
   ); /*print command to log file*/
 
   fflush(logFILE);

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec04:
   ^   - cluster reads + print results
   ^   o main sec04 sub01:
   ^     - cluster reads
   ^   o main sec04 sub02:
   ^     - print consensus to file
   ^   o main sec04 sub03:
   ^     - print clusters
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   conHeapListST =
      cluster_edClust(
         &indexHeapST,
         &clustSetStackST,
         &conSetStackST,
         &samStackST,
         &buffHeapStr,
         &lenBuffUL,
         samFILE,
         logFILE,
         &errSC
     );

   if(errSC)
   { /*If: had error*/
      if(errSC == def_memErr_edClust)
      { /*If: memory error*/
         fprintf(
            logFILE,
            "memory error clustering reads\n"
         );

         fprintf(
            stderr,
            "memory error clustering reads\n"
         );
      } /*If: memory error*/

      else
      { /*Else: file error*/
         fprintf(
            logFILE,
            "file error clustering reads\n"
         );

         fprintf(
            stderr,
            "file error clustering reads\n"
         );
      } /*Else: file error*/

      goto err_main_sec0x;
   } /*If: had error*/

   /*****************************************************\
   * Main Sec04 Sub02:
   *   - print consensus to file
   \*****************************************************/

   errSC =
      plist_con_clustST(
         conHeapListST,
         samHeadHeapStr,
         pgHeadStr,
         &buffHeapStr,
         &lenBuffUL,
         conFILE
      );

   if(errSC)
   { /*If: memory error (very unlikely)*/
      fprintf(
         logFILE,
         "memory error printing consensuses\n"
      );

      fprintf(
         stderr,
         "memory error printing consensuses\n"
      );

      goto err_main_sec0x;
   } /*If: memory error (very unlikely)*/

   if(
      conFILE != stdout
   ) fclose(conFILE);

   conFILE = 0;

   freeHeapList_con_clustST(conHeapListST);
   conHeapListST = 0;

   /*****************************************************\
   * Main Sec04 Sub03:
   *   - print clusters
   \*****************************************************/

   errSC =
      pbins_clustST(
         prefixStr,
         clustSetStackST.clustSI,
         indexHeapST,
         pgHeadStr,
         &samStackST,
         &buffHeapStr,
         &lenBuffUL,
         samFILE
      );
 
   if(errSC)
   { /*If: error*/
      if(errSC == def_memErr_clustST)
      { /*If: memory error*/
         fprintf(
            stderr,
            "memory error while printing clusters\n"
         );

         fprintf(
            logFILE,
            "memory error while printing clusters\n"
         );
      } /*If: memory error*/

      if(errSC == def_memErr_edClust)
      { /*Else: file error*/
         fprintf(
            stderr,
            "file error while printing clusters\n"
         );

         fprintf(
            logFILE,
            "file error while printing clusters\n"
         );
      } /*Else: file error*/

      goto err_main_sec0x;
   } /*If: error*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec0x:
   ^   - clean up
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   errSC = 0;
   goto cleanUp_main_sec0x;


   err_main_sec0x:;
   errSC = 1;
   goto cleanUp_main_sec0x;


   cleanUp_main_sec0x:;

   fflush(logFILE);

   if(samHeadHeapStr)
      free(samHeadHeapStr);
   samHeadHeapStr = 0;

   if(indexHeapST)
      freeHeap_index_clustST(indexHeapST);
   indexHeapST = 0;

   if(buffHeapStr)
      free(buffHeapStr);
   buffHeapStr = 0;
   lenBuffUL = 0;

   freeStack_set_clustST(&clustSetStackST);
   freeStack_set_tbCon(&conSetStackST);

   freeStack_samEntry(&samStackST);

   if(conHeapListST)
      freeHeapList_con_clustST(conHeapListST);
   conHeapListST = 0;

   if(
         samFILE
      && samFILE != stdin
      && samFILE != stdout
   ) fclose(samFILE);

   samFILE = 0;

   if(
         logFILE
      && logFILE != stdin
      && logFILE != stdout
   ) fclose(logFILE);

   logFILE = 0;

   if(
         conFILE
      && conFILE != stdin
      && conFILE != stdout
   ) fclose(conFILE);

   conFILE = 0;

   return errSC;
} /*main*/
