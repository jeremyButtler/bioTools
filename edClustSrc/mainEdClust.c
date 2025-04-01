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

#define def_repInterval_mainEdClust 5000

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
#include "../genLib/endLine.h"
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
      "edClust from bioTools version: %i-%02i-%02i%s",
      def_year_bioTools,
      def_month_bioTools,
      def_day_bioTools,
      str_endLine
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
      "edClust -sam reads.sam -prefix \"out\"%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "  - clusters reads using edit distance (edDist)%s",
      str_endLine
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
      "Input:%s",
      str_endLine
   );

   /*****************************************************\
   * Fun02 Sec02 Sub02:
   *   - file input and prefix
   \*****************************************************/

   fprintf(
      (FILE *) outFILE,
      "  FILE IO:%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "    -sam reads.sam: [Required]%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "      o sam file with reads to cluster%s",
      str_endLine
   );


   fprintf(
      (FILE *) outFILE,
      "    -prefix out: [Optional; out]%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "      o prefix for output file names%s",
      str_endLine
   );


   fprintf(
      (FILE *) outFILE,
      "    -report %li: [Optional; %li]%s",
      (signed long) def_repInterval_mainEdClust,
      (signed long) def_repInterval_mainEdClust,
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "      o how often to report progress in number%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "        of reads clustered or discarded%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "      o use \"-report 0\" for no reports%s",
      str_endLine
   );

   /*****************************************************\
   * Fun02 Sec02 Sub03:
   *   - cluster depth, error rate, & similarity settings
   \*****************************************************/

   fprintf(
      (FILE *) outFILE,
      "  Cluster Filtering:%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "    -clust-depth %i: [Optional; %i]%s",
      def_minDepth_clustST,
      def_minDepth_clustST,
      str_endLine
   );

   fprintf(
     (FILE *) outFILE,
     "      o min cluster read depth to keep a cluster%s",
     str_endLine
   );


   fprintf(
      (FILE *) outFILE,
      "    -clust-perc-depth %.04f: [Optional; %.04f]%s",
      def_minPercDepth_clustST,
      def_minPercDepth_clustST,
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "      o minimum percent read depth need to keep%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "        a cluster%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "    -read-err %0.3f: [Optional; %0.3f]%s",
      def_readErrRate_clustST,
      def_readErrRate_clustST,
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "      o error rate for read to read mapping%s",
      str_endLine
   );


   fprintf(
      (FILE *) outFILE,
      "    -con-err %0.3f: [Optional; %0.3f]%s",
      def_conErrRate_clustST,
      def_conErrRate_clustST,
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "      o consensus to read mapping error rate%s",
      str_endLine
   );


   fprintf(
      (FILE *) outFILE,
      "    -con-sim %0.4f: [Optional; %0.4f]%s",
      def_maxConSim_clustST,
      def_maxConSim_clustST,
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "      o maximum simularity between consensuses%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "        to not merge clusters%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "    -overlap %0.2f: [Optional; %0.2f]%s",
      def_percOverlap_clustST,
      def_percOverlap_clustST,
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "      o minimum percentage of overlap needed to%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "     merge clusters (start comparision)%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "      o used in final consensus comparison%s",
      str_endLine
   );



   /*****************************************************\
   * Fun02 Sec02 Sub04:
   *   - scoring settings (length weight only)
   \*****************************************************/

   fprintf(
      (FILE *) outFILE,
      "  Read Scoring:%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "    -len-weight %0.2f: [Optional; %0.2f]%s",
      def_lenWeight_clustST,
      def_lenWeight_clustST,
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "      o how much to increase lengths impact on%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "        read scoring%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "      o log_length = floor[log10(length)]%s",
      str_endLine
   );

   fprintf(
     (FILE *) outFILE,
     "      o length_score= floor[weigth * log_length]%s",
     str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "      o score = length_score + floor[median Q]%s",
      str_endLine
   );

   /*****************************************************\
   * Fun02 Sec02 Sub05:
   *   - read filtering settings
   \*****************************************************/

   fprintf(
      (FILE *) outFILE,
      "  Read Filtering:%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "    -min-len %i: [Optional; %i]%s",
      def_minLen_clustST,
      def_minLen_clustST,
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "      o minimum aligned length to keep read%s",
      str_endLine
   );


   fprintf(
      (FILE *) outFILE,
      "    -min-mapq %i: [Optional; %i]%s",
      def_minMapq_clustST,
      def_minMapq_clustST,
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "      o minimum mapping quality to keep read%s",
      str_endLine
   );


   fprintf(
      (FILE *) outFILE,
      "    -min-med-q %i: [Optional; %i]%s",
      def_minMedQ_clustST,
      def_minMedQ_clustST,
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "      o minimum median q-score to keep read%s",
      str_endLine
   );


   fprintf(
      (FILE *) outFILE,
      "    -min-mean-q %i: [Optional; %i]%s",
      def_minAvgQ_clustST,
      def_minAvgQ_clustST,
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "      o minimum mean q-score to keep read%s",
      str_endLine
   );

   /*****************************************************\
   * Fun02 Sec02 Sub06:
   *   - edit distance settings
   \*****************************************************/

   fprintf(
      (FILE *) outFILE,
      "  Edit Distance:%s",
      str_endLine
   );

   if(def_depthProfile_clustST)
      fprintf(
         (FILE *) outFILE,
         "    -depth-prof: [Optional; Yes]%s",
         str_endLine
      );
   else
      fprintf(
         (FILE *) outFILE,
         "    -depth-prof: [Optional; No]%s",
         str_endLine
      );

   fprintf(
      (FILE *) outFILE,
      "      o -clust-depth to remove low read depth%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "        variants from edit distance%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "      o disable with \"no-depth-prof\"%s",
      str_endLine
   );


   fprintf(
      (FILE *) outFILE,
      "    -err-to-var %i: [Optional; %i]%s",
      def_maxVarErrRatio_clustST,
      def_maxVarErrRatio_clustST,
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "      o maximum variant to error ratio to add%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "        read to a cluster (whole number)%s",
      str_endLine
   );

   fprintf(
     (FILE *) outFILE,
     "      o 100 * (# variants)/(error rate * length)%s",
     str_endLine
   );



   fprintf(
      (FILE *) outFILE,
      "    -win-len %i: [Optional; %i]%s",
      def_window_clustST,
      def_window_clustST,
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "      o size of window for window scanning%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "      o idea is to detect regions of high%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "        difference in very long reads%s",
      str_endLine
   );


   fprintf(
      (FILE *) outFILE,
      "    -win-err %i: [Optional; %i]%s",
      def_windowError_clustST,
      def_windowError_clustST,
      str_endLine
   );

   fprintf(
     (FILE *) outFILE,
     "      o maximum variant to error ratio in window%s",
     str_endLine
   );



   fprintf(
      (FILE *) outFILE,
      "    -indel-len %i: [Optional; %i]%s",
      def_indelLen_clustST,
      def_indelLen_clustST,
      str_endLine
   );

   fprintf(
     (FILE *) outFILE,
     "      o min length to add indel to edit distance%s",
     str_endLine
   );


   fprintf(
      (FILE *) outFILE,
      "    -clust-q-snp %i: [Optional; %i]%s",
      def_minSnpQ_clustST,
      def_minSnpQ_clustST,
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "      o minimum q-score to keep snp (edit dist)%s",
      str_endLine
   ); /*edit distance min snp q-score*/


   /*****************************************************\
   * Fun02 Sec02 Sub07:
   *   - cosensus unqiue settings
   \*****************************************************/

   fprintf(
      (FILE *) outFILE,
      "  Consensus Filtering + Building:%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "    -con-iter %i: [Optional; %i]%s",
      def_conRebuild_clustST,
      def_conRebuild_clustST,
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "      o number of consensus rebuilds and remaps%s",
      str_endLine
   );


   fprintf(
      (FILE *) outFILE,
      "    -perc-n %0.4f: [Optional; %0.4f]%s",
      def_maxNPerc_clustST,
      def_maxNPerc_clustST,
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "      o maximum percent masked consnesus bases%s",
      str_endLine
   ); /*both consensus and edit distance*/


   fprintf(
      (FILE *) outFILE,
      "    -con-depth %i: [Optional; %i]%s",
      def_minDepth_tbConDefs,
      def_minDepth_tbConDefs,
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "      o minimum read depth to not mask%s",
      str_endLine
   );


   fprintf(
      (FILE *) outFILE,
      "    -con-q-snp %i: [Optional; %i]%s",
      def_minSnpQ_clustST,
      def_minSnpQ_clustST,
      str_endLine
   );


   fprintf(
      (FILE *) outFILE,
      "      o minimum q-score for snp in consensus%s",
      str_endLine
   ); /*both consensus and edit distance*/

   fprintf(
      (FILE *) outFILE,
      "    -q-ins %i: [Optional; %i]%s",
      def_minInsQ_tbConDefs,
      def_minInsQ_tbConDefs,
      str_endLine
   );

   fprintf(
     (FILE *) outFILE,
     "      o minimum q-score for ins (consensus only)%s",
     str_endLine
   );


   fprintf(
      (FILE *) outFILE,
      "    -snp %0.2f: [Optional; %0.2f]%s",
      def_minSnpPerc_tbConDefs,
      def_minSnpPerc_tbConDefs,
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "      o minimum percent support to keep SNP%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "        or a match%s",
      str_endLine
   );


   fprintf(
      (FILE *) outFILE,
      "    -ins %0.2f: [Optional; %0.2f]%s",
      def_minInsPerc_tbConDefs,
      def_minInsPerc_tbConDefs,
      str_endLine
   );

   fprintf(
    (FILE *) outFILE,
    "      o minimum percent support to keep insertion%s",
    str_endLine
   );


   fprintf(
      (FILE *) outFILE,
      "    -del %0.2f: [Optional; %0.2f]%s",
      def_minDelPerc_tbConDefs,
      def_minDelPerc_tbConDefs,
      str_endLine
   );

   fprintf(
     (FILE *) outFILE,
     "      o minimum percent support to keep deletion%s",
     str_endLine
   );

   /*****************************************************\
   * Fun02 Sec02 Sub08:
   *   - help message and version number
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
   ^   - output
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   fprintf(
       (FILE *) outFILE,
       "Output:%s",
       str_endLine
   );

   fprintf(
     (FILE *) outFILE,
     "   - reads in cluster to prefix-ref-cluster.sam%s",
     str_endLine
   );

   fprintf(
     (FILE *) outFILE,
     "   - consenseuses as prefix-ref-cluster-con.sam%s",
     str_endLine
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

   signed int siArg = 1;
   signed char errSC = 0;
   signed char *tmpStr = 0;

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
            (signed char *) "-sam",
            (signed char *) argAryStr[siArg],
            (signed char) '\0'
         )
      ){ /*If: sam file input*/
         ++siArg;
         *samFileStrPtr =
            (signed char *) argAryStr[siArg];
      } /*If: sam file input*/

      else if(
         ! eql_charCp(
            (signed char *) "-prefix",
            (signed char *) argAryStr[siArg],
            (signed char) '\0'
         )
      ){ /*Else If: prefix input*/
         ++siArg;
         *prefixStrPtr =
             (signed  char *) argAryStr[siArg];
      } /*Else If: prefix input*/

      else if(
         ! eql_charCp(
            (signed char *) "-report",
            (signed char *) argAryStr[siArg],
            (signed char) '\0'
         )
      ){ /*Else If: progress report interval*/
         ++siArg;

         tmpStr = (signed char *) argAryStr[siArg];

         tmpStr +=
            strToSL_base10str(
                tmpStr,
                &setSTPtr->repIntervalSL
            );

         if(*tmpStr != '\0')
         { /*If: invalid input*/
            fprintf(
               stderr,
               "-report %s; non-numeric/to large%s",
               argAryStr[siArg],
               str_endLine
            );

            goto err_fun03_sec03;
         } /*If: invalid input*/
      } /*Else If: progress report interval*/

      /**************************************************\
      * Fun03 Sec02 Sub02:
      *   - clust depth, error rate, & simularity settings
      \**************************************************/

      else if(
         ! eql_charCp(
            (signed char *) "-depth-prof",
            (signed char *) argAryStr[siArg],
            (signed char) '\0'
         )
      ) setSTPtr->depthProfBl = 1;

      else if(
         ! eql_charCp(
            (signed char *) "-no-depth-prof",
            (signed char *) argAryStr[siArg],
            (signed char) '\0'
         )
      ) setSTPtr->depthProfBl = 0;

      else if(
         ! eql_charCp(
            (signed char *) "-clust-depth",
            (signed char *) argAryStr[siArg],
            (signed char) '\0'
         )
      ){ /*Else If: min cluster read depth*/
         ++siArg;
         tmpStr = (signed char *) argAryStr[siArg];

         tmpStr +=
            strToUI_base10str(
                tmpStr,
                &setSTPtr->minDepthUI
            );

         if(*tmpStr != '\0')
         { /*If: invalid input*/
            fprintf(
               stderr,
               "-clust-depth %s; non-numeric/to large%s",
               argAryStr[siArg],
               str_endLine
            );

            goto err_fun03_sec03;
         } /*If: invalid input*/
      } /*Else If: min cluster read depth*/


      else if(
         ! eql_charCp(
            (signed char *) "-clust-perc-depth",
            (signed char *) argAryStr[siArg],
            (signed char) '\0'
         )
      ){ /*Else If: cluster min percent read depth*/
         ++siArg;
         setSTPtr->minPercDepthF = atof(argAryStr[siArg]);
      } /*Else If: cluster min percent read depth*/

      else if(
         ! eql_charCp(
            (signed char *) "-read-err",
            (signed char *) argAryStr[siArg],
            (signed char) '\0'
         )
      ){ /*Else If: error rate for read to read map*/
         ++siArg;

         setSTPtr->readErrRateF =
            atof(argAryStr[siArg]);
      } /*Else If: error rate for read to read map*/


      else if(
         ! eql_charCp(
            (signed char *) "-con-err",
            (signed char *) argAryStr[siArg],
            (signed char) '\0'
        )
     ){ /*Else If: error rate for con to read map*/
        ++siArg;
        setSTPtr->conErrRateF = atof(argAryStr[siArg]);
     } /*Else If: error rate for con to read map*/


      else if(
         ! eql_charCp(
            (signed char *) "-con-sim",
            (signed char *) argAryStr[siArg],
            (signed char) '\0'
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
            (signed char *) "-len-weight",
            (signed char *) argAryStr[siArg],
            (signed char) '\0'
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
            (signed char *) "-min-len",
            (signed char *) argAryStr[siArg],
            (signed char) '\0'
         )
      ){ /*Else If: minimum read length*/
         ++siArg;
         tmpStr = (signed char *) argAryStr[siArg];

         tmpStr +=
            strToUI_base10str(
                tmpStr,
                &setSTPtr->minLenUI
            );

         if(*tmpStr != '\0')
         { /*If: invalid input*/
            fprintf(
               stderr,
               "-min-len %s; non-numeric or to large%s",
               argAryStr[siArg],
               str_endLine
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
            (signed char *) "-min-mapq",
            (signed char *) argAryStr[siArg],
            (signed char) '\0'
         )
      ){ /*Else If: minimum mapping quality*/
         ++siArg;
         tmpStr = (signed char *) argAryStr[siArg];

         tmpStr +=
            strToUC_base10str(
                tmpStr,
                &setSTPtr->minMapqUC
            );

         if(*tmpStr != '\0')
         { /*If: invalid input*/
            fprintf(
               stderr,
               "-min-mapq %s; non-numeric or to large%s",
               argAryStr[siArg],
               str_endLine
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
            (signed char *) "-min-med-q",
            (signed char *) argAryStr[siArg],
            (signed char) '\0'
         )
      ){ /*Else If: minimum median q-score*/
         ++siArg;
         tmpStr = (signed char *) argAryStr[siArg];

         tmpStr +=
            strToUI_base10str(
                tmpStr,
                &setSTPtr->minMedQUI
            );

         if(*tmpStr != '\0')
         { /*If: invalid input*/
            fprintf(
               stderr,
               "-min-med-q %s; non-numeric / to large%s",
               argAryStr[siArg],
               str_endLine
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
            (signed char *) "-min-mean-q",
            (signed char *) argAryStr[siArg],
            (signed char) '\0'
         )
      ){ /*Else If: minimu mean q-score*/
         ++siArg;
         tmpStr = (signed char *) argAryStr[siArg];

         tmpStr +=
            strToUI_base10str(
                tmpStr,
                &setSTPtr->minAvgQUI
            );

         if(*tmpStr != '\0')
         { /*If: invalid input*/
            fprintf(
               stderr,
               "-min-mean-q %s; non-numeric / to large%s",
               argAryStr[siArg],
               str_endLine
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
            (signed char *) "-err-to-var",
            (signed char *) argAryStr[siArg],
            (signed char) '\0'
         )
      ){ /*Else If: maximum error to variant ratio*/
         ++siArg;
         tmpStr = (signed char *) argAryStr[siArg];

         tmpStr +=
            strToUI_base10str(
                tmpStr,
                &setSTPtr->varToErrUI
            );

         if(*tmpStr != '\0')
         { /*If: invalid input*/
            fprintf(
               stderr,
               "-err-to-var %s; non-numeric / to large%s",
               argAryStr[siArg],
               str_endLine
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
            (signed char *) "-overlap",
            (signed char *) argAryStr[siArg],
            (signed char) '\0'
         )
      ){ /*Else If: minimum overlap*/
         ++siArg;

         setSTPtr->percOverlapF =
            atof(argAryStr[siArg]);

         if(setSTPtr->percOverlapF == 0)
         { /*If: invalid input*/
            fprintf(
               stderr,
               "-overlap %s goes to 0; invalid input%s",
               argAryStr[siArg],
               str_endLine
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
            (signed char *) "-win-len",
            (signed char *) argAryStr[siArg],
            (signed char) '\0'
         )
      ){ /*Else If: window length for window scanning*/
         ++siArg;
         tmpStr = (signed char *) argAryStr[siArg];

         tmpStr +=
            strToUI_base10str(
                tmpStr,
                &setSTPtr->winSizeUI
            );

         if(*tmpStr != '\0')
         { /*If: invalid input*/
            fprintf(
               stderr,
               "-win-len %s; non-numeric / to large%s",
               argAryStr[siArg],
               str_endLine
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
            (signed char *) "-win-err",
            (signed char *) argAryStr[siArg],
            (signed char) '\0'
         )
      ){ /*Else If: window error to varaint ratio*/
         ++siArg;
         tmpStr = (signed char *) argAryStr[siArg];

         tmpStr +=
            strToUI_base10str(
                tmpStr,
                &setSTPtr->winErrUI
            );

         if(*tmpStr != '\0')
         { /*If: invalid input*/
            fprintf(
               stderr,
               "-win-err %s; non-numeric / to large%s",
               argAryStr[siArg],
               str_endLine
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
            (signed char *) "-indel-len",
            (signed char *) argAryStr[siArg],
            (signed char) '\0'
         )
      ){ /*Else If: min indel length*/
         ++siArg;
         tmpStr = (signed char *) argAryStr[siArg];

         tmpStr +=
            strToUI_base10str(
                tmpStr,
                &setSTPtr->indelLenUI
            );

         if(*tmpStr != '\0')
         { /*If: invalid input*/
            fprintf(
               stderr,
               "-indel-len %s; non-numeric / to large%s",
               argAryStr[siArg],
               str_endLine
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
            (signed char *) "-indel-len",
            (signed char *) argAryStr[siArg],
            (signed char) '\0'
         )
      ){ /*Else If: min indel length*/
         ++siArg;
         tmpStr = (signed char *) argAryStr[siArg];

         tmpStr +=
            strToUI_base10str(
                tmpStr,
                &setSTPtr->indelLenUI
            );

         if(*tmpStr != '\0')
         { /*If: invalid input*/
            fprintf(
               stderr,
               "-indel-len %s; non-numeric / to large%s",
               argAryStr[siArg],
               str_endLine
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
            (signed char *) "-clust-q-snp",
            (signed char *) argAryStr[siArg],
            (signed char) '\0'
         )
      ){ /*Else If: min snp q-score*/
         ++siArg;
         tmpStr = (signed char *) argAryStr[siArg];

         tmpStr +=
            strToUC_base10str(
                tmpStr,
                &setSTPtr->minSnpQUC
            );

         if(*tmpStr != '\0')
         { /*If: invalid input*/
            fprintf(
               stderr,
               "-q-snp %s; non-numeric or to large%s",
               argAryStr[siArg],
               str_endLine
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
            (signed char *) "-perc-n",
            (signed char *) argAryStr[siArg],
            (signed char) '\0'
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
            (signed char *) "-con-iter",
            (signed char *) argAryStr[siArg],
            (signed char) '\0'
         )
      ){ /*Else If: number times to rebuild consensus*/
         ++siArg;
         tmpStr = (signed char *) argAryStr[siArg];

         tmpStr +=
            strToUC_base10str(
                tmpStr,
                &setSTPtr->conRebuildUC
            );

         if(*tmpStr != '\0')
         { /*If: invalid input*/
            fprintf(
               stderr,
               "-con-iter %s; non-numeric / to large%s",
               argAryStr[siArg],
               str_endLine
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
            (signed char *) "-con-depth",
            (signed char *) argAryStr[siArg],
            (signed char) '\0'
         )
      ){ /*Else If: min consnesus read depth*/
         ++siArg;
         tmpStr = (signed char *) argAryStr[siArg];

         tmpStr +=
            strToSI_base10str(
                tmpStr,
                &conSetSTPtr->minDepthSI
            );

         if(*tmpStr != '\0')
         { /*If: invalid input*/
            fprintf(
               stderr,
               "-con-depth %s; non-numeric / to large%s",
               argAryStr[siArg],
               str_endLine
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
            (signed char *) "-con-q-snp",
            (signed char *) argAryStr[siArg],
            (signed char) '\0'
         )
      ){ /*Else If: min snp q-score*/
         ++siArg;
         tmpStr = (signed char *) argAryStr[siArg];

         tmpStr +=
            strToSI_base10str(
                tmpStr,
                &conSetSTPtr->minQSI
            );

         if(*tmpStr != '\0')
         { /*If: invalid input*/
            fprintf(
               stderr,
               "-con-q-snp %s; non-numeric or to large%s",
               argAryStr[siArg],
               str_endLine
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
            (signed char *) "-q-ins",
            (signed char *) argAryStr[siArg],
            (signed char) '\0'
         )
      ){ /*Else If: min insetion q-score*/
         ++siArg;
         tmpStr = (signed char *) argAryStr[siArg];

         tmpStr +=
            strToSI_base10str(
                tmpStr,
                &conSetSTPtr->minInsQSI
            );

         if(*tmpStr != '\0')
         { /*If: invalid input*/
            fprintf(
               stderr,
               "-q-ins %s; non-numeric or to large%s",
               argAryStr[siArg],
               str_endLine
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
            (signed char *) "-snp",
            (signed char *) argAryStr[siArg],
            (signed char) '\0'
         )
      ){ /*Else If: min snp support (as percent)*/
         ++siArg;

         conSetSTPtr->minPercSnpF =
            atof(argAryStr[siArg]);
      } /*Else If: min snp support (as percent)*/

      else if(
         ! eql_charCp(
            (signed char *) "-ins",
            (signed char *) argAryStr[siArg],
            (signed char) '\0'
         )
      ){ /*Else If: min ins support (as percent)*/
         ++siArg;

         conSetSTPtr->minPercInsF =
            atof(argAryStr[siArg]);
      } /*Else If: min ins support (as percent)*/

      else if(
         ! eql_charCp(
            (signed char *) "-del",
            (signed char *) argAryStr[siArg],
            (signed char) '\0'
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
            (signed char *) "-h",
            (signed char *) argAryStr[siArg],
            (signed char) '\0'
         )
      ) goto phelp_fun03_sec03;

      else if(
         ! eql_charCp(
            (signed char *) "--h",
            (signed char *) argAryStr[siArg],
            (signed char) '\0'
         )
      ) goto phelp_fun03_sec03;

      else if(
         ! eql_charCp(
            (signed char *) "help",
            (signed char *) argAryStr[siArg],
            (signed char) '\0'
         )
      ) goto phelp_fun03_sec03;

      else if(
         ! eql_charCp(
            (signed char *) "-help",
            (signed char *) argAryStr[siArg],
            (signed char) '\0'
         )
      ) goto phelp_fun03_sec03;

      else if(
         ! eql_charCp(
            (signed char *) "--help",
            (signed char *) argAryStr[siArg],
            (signed char) '\0'
         )
      ) goto phelp_fun03_sec03;

      /**************************************************\
      * Fun03 Sec02 Sub08:
      *   - version number
      \**************************************************/

      else if(
         ! eql_charCp(
            (signed char *) "-v",
            (signed char *) argAryStr[siArg],
            (signed char) '\0'
         )
      ) goto pversion_fun03_sec03;

      else if(
         ! eql_charCp(
            (signed char *) "--v",
            (signed char *) argAryStr[siArg],
            (signed char) '\0'
         )
      ) goto pversion_fun03_sec03;

      else if(
         ! eql_charCp(
            (signed char *) "version",
            (signed char *) argAryStr[siArg],
            (signed char) '\0'
         )
      ) goto pversion_fun03_sec03;

      else if(
         ! eql_charCp(
            (signed char *) "-version",
            (signed char *) argAryStr[siArg],
            (signed char) '\0'
         )
      ) goto pversion_fun03_sec03;

      else if(
         ! eql_charCp(
            (signed char *) "--version",
            (signed char *) argAryStr[siArg],
            (signed char) '\0'
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
            "%s is not recoginized%s",
            argAryStr[siArg],
            str_endLine
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

   signed char *tmpStr = 0;
   signed int siArg = 0;

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
         (signed char *)
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
            (signed char *) argAryStr[siArg],
            (signed char) '\0'
         ); /*copy user input*/
   } /*Loop: copy user input*/

   *tmpStr++ = str_endLine[0];

   if(str_endLine[1] != '\0')
      *tmpStr++ = str_endLine[1];
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

   signed char errSC = 0;

   signed char *samFileStr = 0;
   signed char *prefixStr = (signed char *) "out";

   signed char *tmpStr = 0;        /*for editing strings*/
   signed char *samHeadHeapStr = 0;/*get sam file header*/

   signed char pgHeadStr[4096]; /*program header*/

   /*for sam file reading functions*/
   struct samEntry samStackST;
   signed char *buffHeapStr = 0;
   unsigned long lenBuffUL = 0;

   /*settings for edClust*/
   struct set_clustST clustSetStackST;
   struct set_tbCon conSetStackST;

   /*for clustering*/
   struct con_clustST *conHeapListST = 0; /*consensuses*/
   struct index_clustST *indexHeapST = 0; /*has clusters*/

   FILE *samFILE = 0;

   signed char fileNameStr[4096];
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

   clustSetStackST.repIntervalSL = 
      def_repInterval_mainEdClust;

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
         "memory error setting up sam entry struct%s",
         str_endLine
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
         "no sam input with -sam%s",
         str_endLine
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
            "no sam input with -sam%s",
            str_endLine
          );
       else
         fprintf(
            stderr,
            "could not open -sam %s%s",
            samFileStr,
            str_endLine
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
          "could not open %s; %s missing permission%s",
          fileNameStr,
          prefixStr,
          str_endLine
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
          "could not open %s; %s missing permission%s",
          fileNameStr,
          prefixStr,
          str_endLine
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
         "memory error geting sam file header%s",
         str_endLine
      );

      fprintf(
         stderr,
         "memory error geting sam file header%s",
         str_endLine
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
      "%s%s%s",
      tmpStr,
      str_endLine,
      str_endLine
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
            "memory error clustering reads%s",
            str_endLine
         );

         fprintf(
            stderr,
            "memory error clustering reads%s",
            str_endLine
         );
      } /*If: memory error*/

      else
      { /*Else: file error*/
         fprintf(
            logFILE,
            "file error clustering reads%s",
            str_endLine
         );

         fprintf(
            stderr,
            "file error clustering reads%s",
            str_endLine
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
         "memory error printing consensuses%s",
         str_endLine
      );

      fprintf(
         stderr,
         "memory error printing consensuses%s",
         str_endLine
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
            "memory error while printing clusters%s",
            str_endLine
         );

         fprintf(
            logFILE,
            "memory error while printing clusters%s",
            str_endLine
         );
      } /*If: memory error*/

      if(errSC == def_memErr_edClust)
      { /*Else: file error*/
         fprintf(
            stderr,
            "file error while printing clusters%s",
            str_endLine
         );

         fprintf(
            logFILE,
            "file error while printing clusters%s",
            str_endLine
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
