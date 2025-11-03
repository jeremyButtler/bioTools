/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' mainFiltsam SOF: Start Of File
'   - filters sam files (slower than samtools)
'   o header:
'     - included libraries and defaults/defined variables
'   o st01: coord_mainFiltsam
'     - has coordinates to filter by
'   o st02: set_mainFiltsam
'     - holds input settings from user
'   o fun01: pversion_mainFiltsam
'     - prints out the version number for mainFiltsam
'   o fun02: phelp_mainFiltsam
'     - prints the help message for mainFiltsam
'   o fun03: strToCoord_mainFiltsam
'     - converts input line to coordinate
'   o fun04: getCoords_mainFiltsam
'     - gets filtering coordinates from an input file
'   o fun05: input_mainFiltsam
'     - gets user input from argAryStr
'   o main:
'     - filters sam files and converts sam output to the
'       desiered format
'   o license:
'     - licensing for this code (public domain / mit)
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*-------------------------------------------------------\
| Header:
|   - Included libraries and defaults / defined variables
|   o header sec01:
|     - Included libraries
|   o header sec02:
|     - Default variables or defined variables
\-------------------------------------------------------*/

/*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
^ Header Sec01:
^   - Included libraries
\<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

#ifdef PLAN9
   #include <u.h>
   #include <libc.h>
#else
   #include <stdlib.h>
#endif

#include <stdio.h>

#include "../genLib/ulCp.h"
#include "../genLib/charCp.h"
#include "../genLib/base10str.h"
#include "../genLib/fileFun.h"

#include "../genBio/samEntry.h"
#include "../genBio/trimSam.h"

/*These do not have .c files*/
#include "../genLib/endLine.h"
#include "../bioTools.h"

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\
! Hidden dependencies
!   o .c  #include "../genLib/numToStr.h"
!   o .h  #include "../genBio/ntTo5Bit.h"
\%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

/*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
^ Header Sec02:
^   - Default variables or defined variables
\<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

#define def_minLen_mainFiltsam 0
#define def_maxLen_mainFiltsam 0
#define def_minAlnLen_mainFiltsam 0

#define def_minMapq_mainFiltsam 0
#define def_minMedQ_mainFiltsam 0.0f
#define def_minMeanQ_mainFiltsam 0.0f

#define def_trimMaskBl_mainFiltsam 0
#define def_coordsTrimBl_mainFiltsam 0

/*number of bases to trim off start and end*/
#define def_startTrim_mainFiltsam 0
#define def_endTrim_mainFiltsam 0
#define def_startStrictTrim_mainFiltsam 0
   /*set to 1 to trim exact number of bases*/
#define def_endStrictTrim_mainFiltsam 0
   /*set to 2 to trim exact number of bases*/

#define def_inRange_mainFiltsam 1
   /*1: if read is in range*/
#define def_invertCoord_mainFiltsam 0
   /*1: if discarding input coordaintes*/

/*Flags for the printout type*/
#define def_psam_mainFiltsam 0
#define def_pfq_mainFiltsam 1
#define def_pfa_mainFiltsam 2
#define def_pstats_mainFiltsam 3

/*Default output type*/
#define def_pdefault_mainFiltsam def_psam_mainFiltsam

/*print out anonymous counts when doing an stats print*/
#define def_nPrint_mainFiltsam 1 /*1: yes, 0 is no*/

#define def_lenStr_mainFiltsam 120

/*-------------------------------------------------------\
| ST01: coord_mainFiltsam
|   - has coordinates to filter by
\-------------------------------------------------------*/
typedef struct
coord_mainFiltsam{
   unsigned int startUI;
   unsigned int endUI;
   signed char refStr[def_lenStr_mainFiltsam + 8];
}coord_mainFiltsam;

/*-------------------------------------------------------\
| ST02: set_mainFiltsam
|   - holds input settings from user
\-------------------------------------------------------*/
typedef struct
set_mainFiltsam
{
   signed char outFlagSC; /*tells what file type to print*/
   signed char pNsBl;   /*for stats, print N count*/
   signed char inRangeBl; /*coords in ragne*/

   unsigned short rmAryUS[256]; /*flags to remove*/
   unsigned short rmLenUS;
   unsigned short keepAryUS[256]; /*flags to keep*/
   unsigned short keepLenUS;

   signed char trimMaskBl;/*1: trim softmasking off ends*/
   signed int trimStartSI;/*bases to trim from start*/
   signed int trimEndSI;/*bases to trim from end*/
   signed char trimStrictBl;/*bases to trim from end*/
   signed char coordTrimBl; /*trim by coordinates*/

   signed char invertCoordBl;
      /*keep reads not at input coordiantes*/

   /*minimum and maximum lengths to keep reads*/
   unsigned int minLenUI;
   unsigned int maxLenUI;

   /*minimum aligned length to keep read*/
   unsigned int minAlnLenUI;

   /*min mean and median q-scores to keep reads*/
   float minMedianQF;
   float minMeanQF;

   /*min mapping qaulity to keep a read*/
   unsigned char minMapqUC;
}set_mainFiltsam;

/*-------------------------------------------------------\
| Fun01: pversion_mainFiltsam
|   - prints out the version number for mainFiltsam
| Input:
|   - outFILE:
|     o file to print the version numbe to
| Output:
|   - Prints;
|     o version number to outFILE
\-------------------------------------------------------*/
void
pversion_mainFiltsam(
   void *outFILE
){

   fprintf(
      (FILE *) outFILE,
      "mainFiltsam from bioTools version: %i-%02i-%02i%s",
      def_year_bioTools,
      def_month_bioTools,
      def_day_bioTools,
      str_endLine
   );
} /*pversion_mainFiltsam*/

/*-------------------------------------------------------\
| Fun02: phelp_mainFiltsam
|   - prints the help message for mainFiltsam
| Input:
|   - outFILE:
|     o file to print help message to
| Output:
|   - Prints:
|     o help message to outFILE
\-------------------------------------------------------*/
void
phelp_mainFiltsam(
   FILE *outFILE
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun02 TOC:
   '   o fun02 sec01:
   '     - usage block of help message
   '   o fun02 sec02:
   '     - input block of help message
   '   o fun02 sec03:
   '     - print the output block
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun02 Sec01:
   ^   - usage block of help message
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   fprintf(
      outFILE,
      "mainFiltsam -sam reads.sam [options]%s",
      str_endLine
   );

   fprintf(
      outFILE,
      "  - Filters sam file from user%s",
      str_endLine
   );

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun02 Sec02:
   ^   - input block of help message
   ^   o fun02 Sec02 sub01:
   ^     - print the input header
   ^   o fun02 Sec02 sub02:
   ^     - print sam file input
   ^   o fun02 Sec02 sub03:
   ^     - Print flag filtering options
   ^   o fun02 Sec02 sub04:
   ^     - print min, min aligned, and max length options
   ^   o fun02 Sec02 sub05:
   ^     - print min median Q, mean Q, and mapq options
   ^   o fun02 Sec02 sub07:
   ^     - print coodinate options
   ^   o fun02 Sec02 sub07:
   ^     - print output options
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun02 sec02 Sub01:
   *   - print the input header
   \*****************************************************/

   fprintf(
      (FILE *) outFILE,
      "Input:%s",
      str_endLine
   );

   /*****************************************************\
   * Fun02 sec02 Sub02:
   *   - print sam file input
   \*****************************************************/

   fprintf(
      outFILE,
      "  -sam: [Required; default stdin]%s",
      str_endLine
   );

   fprintf(
      outFILE,
      "    o Sam file to filter%s",
      str_endLine
   );

   fprintf(
      outFILE,
      "    o Use `-sam -` for stdin%s",
      str_endLine
   );

   /*****************************************************\
   * Fun02 sec02 Sub03:
   *   - print flag filtering options
   \*****************************************************/

   fprintf(outFILE, "  Flag filtering:%s", str_endLine);
   fprintf(
      outFILE, "    -F flag: [Nothing]%s", str_endLine
   );

   fprintf(
      outFILE,
      "      o Filter out reads with flag%s",
      str_endLine
   );

   fprintf(
      outFILE,
      "      o -F can be input multiple times%s",
      str_endLine
   );

   fprintf(
      outFILE,
      "    -f flag: [Nothing]%s",
      str_endLine
   );
   fprintf(
      outFILE,
      "      o Keep only reads with input flags%s",
      str_endLine
   );
   fprintf(
      outFILE,
      "      o -f can be input multiple times%s",
      str_endLine
   );

   fprintf(
      outFILE,
      "    - Some usefull flags%s",
      str_endLine
   );
   fprintf(
      (FILE *) outFILE,
      "       o 0: forward (not as flag)%s",
      str_endLine
   );
   fprintf(
      (FILE *) outFILE,
      "       o 4: unmapped read%s",
      str_endLine
   );
   fprintf(
      (FILE *) outFILE,
      "       o 16: reverse complement%s",
      str_endLine
   );
   fprintf(
      (FILE *) outFILE,
      "       o 256: secondary alignment%s",
      str_endLine
   );
   fprintf(
      (FILE *) outFILE,
      "       o 2048: supplemental align%s",
      str_endLine
   );

   /*****************************************************\
   * Fun02 sec02 Sub04:
   *   - print min, min aligned, and max length options
   \*****************************************************/

   fprintf(outFILE, "  Length filtering:%s", str_endLine);

   fprintf(
      outFILE,
      "    -min-aln-len length: [%i]%s",
     def_minAlnLen_mainFiltsam,
     str_endLine
   );

   fprintf(
     outFILE,
     "      o Minimum aligned read length to keep read%s",
     str_endLine
   );
   fprintf(
      outFILE,
      "    -min-len length: [%i]%s",
     def_minLen_mainFiltsam,
     str_endLine
   );

   fprintf(
      outFILE,
      "      o Minimum read length to keep an read%s",
      str_endLine
   );

   fprintf(
      outFILE,
      "    -max-len length: [%i]%s",
     def_maxLen_mainFiltsam,
     str_endLine
   );

   fprintf(
      outFILE,
      "      o Maximum read length to keep an read%s",
      str_endLine
   );

   /*****************************************************\
   * Fun02 sec02 Sub05:
   *   - print min median Q, mean Q, and mapq options
   \*****************************************************/

   fprintf(outFILE, "  Q-score filtering:%s", str_endLine);

   fprintf(
      outFILE,
      "    -min-mapq mapq: [%i]%s",
     def_minMapq_mainFiltsam,
     str_endLine
   );

   fprintf(
      outFILE,
      "      o Minimum mapping quality to keep an read%s",
      str_endLine
   );

   fprintf(
      outFILE,
      "    -min-median-q medianQ: [%f]%s",
     def_minMedQ_mainFiltsam,
     str_endLine
   );
   fprintf(
      outFILE,
      "      o Minimum median Q-score to keep an read%s",
      str_endLine
   );

   fprintf(
      outFILE,
      "    -min-mean-q meanQ: [%f]%s",
     def_minMeanQ_mainFiltsam,
     str_endLine
   );
   fprintf(
      outFILE,
      "      o Minimum mean Q-score to keep an read%s",
      str_endLine
   );

   /*****************************************************\
   * Fun02 sec02 Sub07:
   *   - print anonymous base options
   \*****************************************************/

   fprintf(
      outFILE,
      "  Stats printing option:%s",
      str_endLine
   );

   if(def_nPrint_mainFiltsam)
      fprintf(outFILE, "    -p-n: [Yes]%s", str_endLine);
   else
      fprintf(outFILE, "    -p-n: [No]%s", str_endLine);

   
   fprintf(
      outFILE,
      "      o Print anonymous base counts with"
   );

   fprintf(outFILE, " -out-stats%s", str_endLine);

   fprintf(
      outFILE,
      "      o Disable with `-no-p-n`%s",
      str_endLine
   );

   /*****************************************************\
   * Fun02 Sec02 Sub06:
   *   - trimming options
   *   o fun02 sec02 sub05 cat01:
   *     - coordinate trimming + trim header
   *   o fun02 sec02 sub05 cat02:
   *     - soft mask trimming
   *   o fun02 sec02 sub05 cat03:
   *     - start trim
   *   o fun02 sec02 sub05 cat04:
   *     - end trim
   \*****************************************************/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun02 Sec02 Sub06 Cat01:
   +   - coordinate trimming + trim header
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      outFILE,
      "  trimming:%s",
      str_endLine
   );

   if(def_coordsTrimBl_mainFiltsam)
      fprintf(
         outFILE,
         "    -coord-trim: [Yes]%s",
         str_endLine
      );
   else
      fprintf(
         outFILE,
         "    -coord-trim: [No]%s",
         str_endLine
      );

   fprintf(
      outFILE,
      "      o Trim read to coordinates input by"
   );

   fprintf(outFILE, "  -coords start,end%s", str_endLine);

   fprintf(
      outFILE,
      "      o Disable with `-no-coord-trim`%s",
      str_endLine
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun02 Sec02 Sub05 Cat02:
   +   - soft mask trimming
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   if(def_trimMaskBl_mainFiltsam)
      fprintf(outFILE, "    -trim: [Yes]%s", str_endLine);
   else
      fprintf(outFILE, "    -trim: [No]%s", str_endLine);

   fprintf(
      outFILE,
      "      o Removes soft masked bases at read ends%s",
      str_endLine
   );

   fprintf(
      outFILE,
      "      o disble with -no-trim%s",
      str_endLine
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun02 Sec02 Sub05 Cat03:
   +   - start trim
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      outFILE,
      "    -trim-start %i: [Optional; %i]%s",
      def_startTrim_mainFiltsam,
      def_startTrim_mainFiltsam,
      str_endLine
   );

   fprintf(
      outFILE,
      "      o Removes input number of bases from start"
   );
   fprintf(outFILE, "        of read%s", str_endLine);

   fprintf(
      outFILE,
      "      o `-trim` is done before -trim-start%s",
      str_endLine
   );

   fprintf(
      outFILE,
      "      o use `-trim-start-strict number` to trim%s",
      str_endLine
   );
   fprintf(
      outFILE,
      "        the exact number of bases input%s",
      str_endLine
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun02 Sec02 Sub05 Cat04:
   +   - end trim
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      outFILE,
      "    -trim-end %i: [Optional; %i]%s",
      def_endTrim_mainFiltsam,
      def_endTrim_mainFiltsam,
      str_endLine
   );

   fprintf(
      outFILE,
      "      o Removes input number of bases from end"
   );
   fprintf(outFILE, "        of read%s", str_endLine);

   fprintf(
      outFILE,
      "      o `-trim` is done before -trim-end%s",
      str_endLine
   );

   fprintf(
      outFILE,
      "      o use `-trim-end-strict number` to trim%s",
      str_endLine
   );
   fprintf(
      outFILE,
      "        the exact number of bases input%s",
      str_endLine
   );

   /*****************************************************\
   * Fun02 sec02 Sub07:
   *   - print coodinate options
   *   o fun02 sec02 sub07 cat01:
   *     - coord trimming + coord filtering header
   *   o fun02 sec02 sub07 cat02:
   *     - in base range coordiane filter option
   *   o fun02 sec02 sub07 cat03:
   *     - invert coordiante filtering (discard hits)
   *   o fun02 sec02 sub07 cat04:
   *     - command line coordiantes input
   *   o fun02 sec02 sub07 cat05:
   *     - file coordiantes input
   \*****************************************************/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun02 Sec02 Sub07 Cat01:
   +   - in base range coordiane filter option
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      outFILE,
      "  Coordinate filtering:%s",
      str_endLine
   );


   if(def_inRange_mainFiltsam)
      fprintf(
         outFILE,
         "    -in-range: [Yes]%s",
         str_endLine
      );

   else
      fprintf(
         outFILE,
         "    -in-range: [No]%s",
         str_endLine
      );

   fprintf(
      outFILE,
      "      o treat coordinates as a range instead%s",
      str_endLine
   );

   fprintf(
      outFILE,
      "        of both being required%s",
      str_endLine
   );

   fprintf(
      outFILE,
      "      o disable with -no-range%s",
      str_endLine
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun02 Sec02 Sub07 Cat02:
   +   - invert coordiante filtering (discard hits)
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   if(def_invertCoord_mainFiltsam)
      fprintf(
         outFILE,
         "    -v-coords: [Yes]%s",
         str_endLine
      );

   else
      fprintf(
         outFILE,
         "    -v-coords: [No]%s",
         str_endLine
      );

   fprintf(
      outFILE,
      "      o discard reads at input coordinates%s",
      str_endLine
   );

   fprintf(
      outFILE,
      "      o disable with -no-v-coords%s",
      str_endLine
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun02 Sec02 Sub07 Cat03:
   +   - command line coordiantes input
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      outFILE,
      "    -coords start,end: [None]%s",
      str_endLine
   );

   fprintf(
      outFILE,
      "      * or -coords start,end,reference:%s",
      str_endLine
   );

   fprintf(
    outFILE,
    "      o removes reads not having input coordinate%s",
    str_endLine
   );

   fprintf(
       outFILE,
       "      o use `0,$,name` for an entire reference%s",
       str_endLine
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun02 Sec02 Sub07 Cat04:
   +   - file coordiantes input
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      outFILE,
      "    -coords-file coords.tsv: [None]%s",
      str_endLine
   );

   fprintf(
      outFILE,
      "      o Read coordaintes (-coords) from an file%s",
      str_endLine
   );

   fprintf(
      outFILE,
      "        - first line is header (not used)%s",
      str_endLine
   );

   fprintf(
      outFILE,
      "        - next lines: skip\tstart,end,reference%s",
      str_endLine
   );


   /*****************************************************\
   * Fun02 sec02 Sub08:
   *   - print output options
   \*****************************************************/

   fprintf(outFILE, "  Output options:%s", str_endLine);

   fprintf(
      outFILE,
      "    -out file.ext: [stdout]%s",
      str_endLine
   );
   fprintf(
      outFILE,
      "      o File to print output to%s",
      str_endLine
   );
   fprintf(
      outFILE,
      "      o Use `-out -` for stdout%s",
      str_endLine
   );


   if(def_pdefault_mainFiltsam == def_psam_mainFiltsam)
      fprintf(
         outFILE,
         "    -out-sam: [Default]%s",
         str_endLine
      );
   else
      fprintf(
         outFILE,
         "    -out-sam: [No]%s",
         str_endLine
      );
   fprintf(
      outFILE,
      "      o Print -out as sam file%s",
      str_endLine
   );

   if(def_pdefault_mainFiltsam == def_pfq_mainFiltsam)
      fprintf(
         outFILE,
         "    -out-fastq: [Default]%s",
         str_endLine
      );
   else
      fprintf(
         outFILE,
         "    -out-fastq: [No]%s",
         str_endLine
      );
   fprintf(
      outFILE,
      "      o Print -out as fastq file%s",
      str_endLine
   );

   if(def_pdefault_mainFiltsam == def_pfa_mainFiltsam)
      fprintf(
         outFILE,
         "    -out-fasta: [Default]%s",
         str_endLine
      );
   else
      fprintf(
         outFILE,
         "    -out-fasta: [No]%s",
         str_endLine
      );
   fprintf(
      outFILE,
      "      o Print -out as fasta file%s",
      str_endLine
   );

   if(def_pdefault_mainFiltsam == def_pstats_mainFiltsam)
      fprintf(
         outFILE,
         "    -out-stats: [Default]%s",
         str_endLine
      );
   else
      fprintf(
         outFILE,
         "    -out-stats: [No]%s",
         str_endLine
      );
   fprintf(
      outFILE,
      "      o Print -out as stats file%s",
      str_endLine
   );

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun02 Sec03:
   ^   - print the output block
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   fprintf(outFILE, "Output:%s", str_endLine);
   fprintf(
      outFILE,
      "  - Prints filtered reads to selected file type%s",
      str_endLine
   );
} /*phelp_mainFiltsam*/

/*-------------------------------------------------------\
| Fun03: strToCoord_mainFiltsam
|   - converts input line to coordinate
| Input:
|   - lineStr:
|     o c-string to get coordinates and reference from
|   - coordArySTPtr:
|     o pointer to coord_mainFiltsam array to add to
|   - posSI:
|     o index in coordArySTPtr to add new coordinate to
|   - sizeSIPtr:
|     o signed int pointer with max elements in
|       coordsSTPtr
|   - fromFileBl:
|     o 1: is input from file (extra column)
|     o 0: is input from cmd (no column)
| Output:
|   - Modifies:
|     o coordsSTPtr to have new coordiantes
|       * also is resized if needed
|     o sizeSIPtr to have new coordsSTPtr size
|   - Returns:
|     o 0 for no errors
|     o 1 for no coordinates (empty/no coordiante) line
|     o 2 for memory error
|     o 4 for file error
\-------------------------------------------------------*/
signed char
strToCoord_mainFiltsam(
   signed char *lineStr, /*line to get coordinates from*/
   struct coord_mainFiltsam **coordArySTPtr,
   signed int posSI,     /*index to insert coordinate*/
   signed int *sizeSIPtr,   /*max elements in coordsAry*/
   signed char fromFileBl
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun03 TOC:
   '   - converts input line to coordinate
   '   o fun03 sec01:
   '     - variable declarations
   '   o fun03 sec02:
   '     - allocate memory (if needed)
   '   o fun03 sec03:
   '     - get past extra column (file only)
   '   o fun03 sec04:
   '     - get start coordinate
   '   o fun03 sec05:
   '     - get end coordinate
   '   o fun03 sec06:
   '     - get reference id
   '   o fun03 sec07:
   '     - return result
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   signed char errSC = 0;
   signed char *tmpStr = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec02:
   ^   - allocate memory (if needed)
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(! *coordArySTPtr)
   { /*If: need to allocate memory for structure*/
      *coordArySTPtr =
        malloc(
          (posSI + 16) * sizeof(struct coord_mainFiltsam)
        );
      if(! *coordArySTPtr)
         goto memErr_fun03_sec07;
      *sizeSIPtr = posSI + 16;
   } /*If: need to allocate memory for structure*/

   else if(posSI >= *sizeSIPtr)
   { /*If: need more memory*/
      tmpStr =
        (signed char *)
        realloc(
          *coordArySTPtr,
          (posSI + 16) * sizeof(struct coord_mainFiltsam)
        );
      if(! tmpStr)
         goto memErr_fun03_sec07;
      *coordArySTPtr =
         (struct coord_mainFiltsam *) tmpStr;
      *sizeSIPtr = posSI + 16;
   } /*If: need more memory*/


   (*coordArySTPtr)[posSI].startUI = 0;
   (*coordArySTPtr)[posSI].endUI = 0;
   (*coordArySTPtr)[posSI].refStr[0] = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec03:
   ^   - get past extra column (file only)
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(fromFileBl)
   { /*If: line was from file*/
      while(*lineStr && *lineStr < 33) ;
         /*get off white space at start*/

      if(*lineStr == '\n')
         goto noCoord_fun03_sec07;
      if(*lineStr == '\r')
         goto noCoord_fun03_sec07;
      if(! *lineStr)
         goto noCoord_fun03_sec07;


      while(*lineStr++ > 32) ;
         /*get off first column*/

      if(*lineStr == '\n')
         goto noCoord_fun03_sec07;
      if(*lineStr == '\r')
         goto noCoord_fun03_sec07;
      if(! *lineStr)
         goto noCoord_fun03_sec07;
   } /*If: line was from file*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec04:
   ^   - get start coordinate
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(*lineStr == '^')
   { /*If: end of line*/
      (*coordArySTPtr)[posSI].startUI = 0;
      ++lineStr;
   } /*If: end of line*/

   else
   { /*Else: number input*/      
      lineStr +=
         strToUI_base10str(
            lineStr,
            &(*coordArySTPtr)[posSI].startUI
         );
   } /*Else: number input*/      

   if(! *lineStr)
      goto fileErr_fun03_sec07;
   else if(*lineStr == '\n')
      goto fileErr_fun03_sec07;
   else if(*lineStr == '\r')
      goto fileErr_fun03_sec07;
   else if(*lineStr != ',')
      goto fileErr_fun03_sec07;

   if((*coordArySTPtr)[posSI].startUI > 0)
      --(*coordArySTPtr)[posSI].startUI; /*to index 0*/
   ++lineStr;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec05:
   ^   - get end coordinate
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(*lineStr == '$')
   { /*If: end of line*/
      (*coordArySTPtr)[posSI].endUI = 0;
         /*using subtract step to set to maximum uint*/
      ++lineStr;
   } /*If: end of line*/

   else
   { /*Else: number input*/      
      lineStr +=
         strToUI_base10str(
            lineStr,
            &(*coordArySTPtr)[posSI].endUI
         );
   } /*Else: number input*/      

   if(! *lineStr)
      goto done_fun03_sec07;
   else if(*lineStr == '\n')
      goto done_fun03_sec07;
   else if(*lineStr == '\r')
      goto done_fun03_sec07;
   else if(*lineStr != ',')
      goto fileErr_fun03_sec07;

   --(*coordArySTPtr)[posSI].endUI; /*to index 0*/
   ++lineStr;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec06:
   ^   - get reference id
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   tmpStr = (*coordArySTPtr)[posSI].refStr;
   errSC = 0;

   while(*lineStr > 32)
   { /*Loop: copy reference name/id*/
      ++errSC;
      if(errSC >= def_lenStr_mainFiltsam)
         goto fileErr_fun03_sec07;

      *tmpStr++ = *lineStr++;
   } /*Loop: copy reference name/id*/

   *tmpStr = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec07:
   ^   - return result
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   done_fun03_sec07:;
      errSC = 0;
      goto ret_fun03_sec07;

   noCoord_fun03_sec07:;
      errSC = 1;
      goto ret_fun03_sec07;

   memErr_fun03_sec07:;
      errSC = 2;
      goto ret_fun03_sec07;

   fileErr_fun03_sec07:;
      errSC = 2;
      goto ret_fun03_sec07;

   ret_fun03_sec07:;
      return errSC;
} /*strToCoord_mainFiltsam*/

/*-------------------------------------------------------\
| Fun04: getCoords_mainFiltsam
|   - gets filtering coordinates from an input file
| Input:
|   - fileStr:
|     o path to file to get coordinates from
|   - numCoordsSI:
|     o pointer to integer with number of coordinates in
|       startAryUI and endAryUI
|   - sizeSIPtr:
|     o pointer to signed int to get size of returned
|       struct array
|   - coordArySTPtr:
|     o coord_mainFiltsam struct array pointer to add
|       coordiantes to
| Output:
|   - Modifies:
|     o coordArySTPtr to coordiantes from file
|       * resized if needed
|     o numCoordsSI to have number of coordinates
|       * also includes previous coordinates
|     o sizeSIPtr to have current coordArySTPtr size
|   - Returns:
|     o 0 for no errors
|     o 1 for memory errors
|     o 2 for file errors
|     o (line << 2) | 2 for line error was on
\-------------------------------------------------------*/
signed int
getCoords_mainFiltsam(
   signed char *fileStr,
   signed int *numCoordsSI,
   signed int *sizeSIPtr,
   struct coord_mainFiltsam **coordArySTPtr
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun04 TOC
   '   - gets filtering coordinates from an input file
   '   o fun04 sec01:
   '     - variable declerations
   '   o fun04 sec02:
   '     - open input file
   '   o fun04 sec03:
   '     - read in the coordaintes
   '   o fun04 sec04:
   '     - clean up
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun04 Sec01:
   ^   - variable declerations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   signed char errSC = 0;
   signed int lineSI = 0;
   signed long lenSL = 0;

   #define buffSize_fun04 512
   signed char buffStr[buffSize_fun04];

   FILE *inFILE = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun04 Sec02:
   ^   - open input file
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(! fileStr)
      inFILE = stdin;
   else if(fileStr[0] == '-')
      inFILE = stdin;
   else
   { /*Else: input file provided*/
      inFILE = fopen((char *) fileStr, "r");
      if(! inFILE)
         goto fileErr_fun04_sec04;
   } /*Else: input file provided*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun04 Sec03:
   ^   - read in the coordaintes
   ^   o fun04 sec03 sub01:
   ^     - get past header and start loop
   ^   o fun04 sec03 sub02:
   ^     - get past the ignored entry
   ^   o fun04 sec03 sub03:
   ^     - read in the starting coordiante
   ^   o fun04 sec03 sub04:
   ^     - read in the ending coordiante
   ^   o fun04 sec03 sub05:
   ^     - convert to index 0 and move to next coordinate
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun04 Sec03 Sub01:
   *   - get past header and start loop
   \*****************************************************/

   lenSL =
      getLine_fileFun(
         inFILE,
         buffStr,
         buffSize_fun04,
         &lenSL
      ); /*get and skip header*/

   ++lineSI;

   while(
      getLine_fileFun(
         inFILE,
         buffStr,
         buffSize_fun04,
         &lenSL
      )
   ){ /*Loop: Read in the file*/
      errSC =
         strToCoord_mainFiltsam(
            buffStr,
            coordArySTPtr,
            *numCoordsSI,
            sizeSIPtr,
            1
         );
            
      if(errSC == 1)
         continue; /*no coordiantes on line*/
      else if(errSC == 2)
         goto memErr_fun04_sec04;
      else if(errSC == 4)
         goto fileErr_fun04_sec04;
      else
         ++*numCoordsSI;
   } /*Loop: Read in the file*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun04 Sec04:
   ^   - Clean up
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   lineSI = 0;
   goto cleanUp_fun04_sec04;

   memErr_fun04_sec04:;
      lineSI = 1;
      goto errCleanUp_fun04_sec04;

   fileErr_fun04_sec04:;
      lineSI <<= 2;
      lineSI |= 2;
      goto errCleanUp_fun04_sec04;

   errCleanUp_fun04_sec04:;
      goto cleanUp_fun04_sec04;

   cleanUp_fun04_sec04:;
      if(inFILE)
         fclose(inFILE);
      inFILE = 0;

      return lineSI;
} /*getCoords_mainFiltsam*/

/*-------------------------------------------------------\
| Fun05: input_mainFiltsam
|   - gets user input from argAryStr
| Input:
|   - numArgsSI:
|     o number of arguments and parameters the user input
|   - argAryStr:
|     o pointer to c-string array with user input
|   - samStr:
|     o will point to path to the sam file to process
|   - outStr:
|     o will point to path to the output file
|   - setSTPtr:
|     o set_mainFiltsam structure pointer will all
|       non-heap settings to set
|   - coordArySTPtr:
|     o coord_mainFiltsam struct array pointer to hold
|       user input coordiantes
|   - numCoordsSI:
|     o pointer to an integer to hold the number of
|       input coordinates
| Output:
|   - Modifies:
|     o all input variables except numArgsSI and argAryStr
|       to hold user input.
\-------------------------------------------------------*/
signed char
input_mainFiltsam(
   int numArgsSI,
   char *argAryStr[],
   signed char **samStr,
   signed char **outStr,
   struct set_mainFiltsam *setSTPtr,
   struct coord_mainFiltsam **coordArySTPtr,
   signed int *numCoordsSI
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun05 TOC:
   '   - get user input
   '   o fun05 sec01:
   '     - variable declerations
   '   o fun05 sec02:
   '     - check if anything was input
   '   o fun05 sec03:
   '     - get user input
   '   o fun05 sec04:
   '     - return errors (or no error)
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun05 Sec01:
   ^   - variable declerations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   signed int siArg = 1;
   signed int errSI = 0;
   signed char errSC = 0;
   signed char *tmpStr = 0;
   signed int sizeSI = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun05 Sec02:
   ^   - check if anything was input
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(numArgsSI <= 1)
   { /*If: no user input*/
      phelp_mainFiltsam(stdout);
      goto phelp_fun05_sec04;
   } /*If: no user input*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun05 Sec03:
   ^   - get user input
   ^   o fun05 sec03 sub01:
   ^     - file io + start loop
   ^   o fun05 sec03 sub02:
   ^     - file output type
   ^   o fun05 sec03 sub03:
   ^     - flag (keep/remove)
   ^   o fun05 sec03 sub04:
   ^     - length filtering
   ^   o fun05 sec03 sub05:
   ^     - quality score filtering
   ^   o fun05 sec03 sub06:
   ^     - booleans (trimming/how to treat masked bases)
   ^   o fun05 sec03 sub07:
   ^     - invert coordinates
   ^   o fun05 sec03 sub08:
   ^     - getting coordiantes (from input)
   ^   o fun05 sec03 sub09:
   ^     - file, get coordiantes
   ^   o fun05 sec03 sub10:
   ^     - help message
   ^   o fun05 sec03 sub11:
   ^     - version number
   ^   o fun05 sec03 sub12:
   ^     - invalid input
   ^   o fun05 sec03 sub13:
   ^     - get next argument
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun05 Sec03 Sub01:
   *   - file io + start loop
   \*****************************************************/

   while(siArg < numArgsSI)
   { /*Loop: Get user input*/
      if(
         ! eql_charCp(
            (signed char *) "-sam",
            (signed char *) argAryStr[siArg],
            (signed char) '\0'
         )
      ){ /*If: The user input a sam file*/
         ++siArg;
         *samStr = (signed char *) argAryStr[siArg];
      } /*If: The user input a sam file*/

      else if(
         ! eql_charCp(
            (signed char *) "-out",
            (signed char *) argAryStr[siArg],
            (signed char) '\0'
         )
      ){ /*Else If: The user provided an output file*/
         ++siArg;
         *outStr = (signed char *) argAryStr[siArg];
      } /*Else If: The user provided an output file*/

      /**************************************************\
      * Fun05 Sec03 Sub02:
      *   - file output type
      \**************************************************/

      else if(
         ! eql_charCp(
            (signed char *) "-out-sam",
            (signed char *) argAryStr[siArg],
            (signed char) '\0'
         )
      ) setSTPtr->outFlagSC = def_psam_mainFiltsam;

      else if(
         ! eql_charCp(
            (signed char *) "-out-fastq",
            (signed char *) argAryStr[siArg],
            (signed char) '\0'
         )
      ) setSTPtr->outFlagSC = def_pfq_mainFiltsam;

      else if(
         ! eql_charCp(
            (signed char *) "-out-fasta",
            (signed char *) argAryStr[siArg],
            (signed char) '\0'
         )
      ) setSTPtr->outFlagSC = def_pfa_mainFiltsam;

      else if(
         ! eql_charCp(
            (signed char *) "-out-stats",
            (signed char *) argAryStr[siArg],
            (signed char) '\0'
         )
      ) setSTPtr->outFlagSC = def_pstats_mainFiltsam;

      /**************************************************\
      * Fun05 Sec03 Sub03:
      *   - flag (keep/remove)
      \**************************************************/

      else if(
            argAryStr[siArg][0] == '-'
         && argAryStr[siArg][1] == 'F'
      ){ /*Else if: this is a removal entry*/
         ++siArg;
         tmpStr = (signed char *) argAryStr[siArg];   

         tmpStr +=
            strToUS_base10str(
               (signed char *) argAryStr[siArg],
               &setSTPtr->rmAryUS[setSTPtr->rmLenUS]
            );

         if(*tmpStr != '\0')
         { /*If: invalid number*/
            fprintf(
               stderr,
               "-F %s is non-numeric or to large%s",
               argAryStr[siArg],
               str_endLine
            );

            goto err_fun05_sec04;
         } /*If: invalid number*/

         ++setSTPtr->rmLenUS;
      } /*Else if: this is a removal entry*/

      else if(
            argAryStr[siArg][0] == '-'
         && argAryStr[siArg][1] == 'f'
      ){ /*Else if: this is a removal entry*/
         ++siArg;
         tmpStr = (signed char *) argAryStr[siArg];   

         tmpStr +=
            strToUS_base10str(
               (signed char *) argAryStr[siArg],
               &setSTPtr->keepAryUS[setSTPtr->keepLenUS]
            );

         if(*tmpStr != '\0')
         { /*If: invalid number*/
            fprintf(
               stderr,
               "-f %s is non-numeric or to large%s",
               argAryStr[siArg],
               str_endLine
            );

            goto err_fun05_sec04;
         } /*If: invalid number*/

         ++setSTPtr->keepLenUS;
      } /*Else if: this is a removal entry*/

      /**************************************************\
      * Fun05 Sec03 Sub04:
      *   - length filtering
      \**************************************************/

      else if(
         ! eql_charCp(
            (signed char *) "-min-len",
            (signed char *) argAryStr[siArg],
            (signed char) '\0'
         )
      ){ /*Else If: the min length was input*/
         ++siArg;
         tmpStr = (signed char *) argAryStr[siArg];   

         tmpStr +=
            strToUI_base10str(
               (signed char *) argAryStr[siArg],
               &setSTPtr->minLenUI
            );

         if(*tmpStr != '\0')
         { /*If: invalid number*/
            fprintf(
               stderr,
               "-min-len %s is non-numeric or to large%s",
               argAryStr[siArg],
               str_endLine
            );

            goto err_fun05_sec04;
         } /*If: invalid number*/
      } /*Else If: the min length was input*/

      else if(
         ! eql_charCp(
            (signed char *) "-max-len",
            (signed char *) argAryStr[siArg],
            (signed char) '\0'
         )
      ){ /*Else If: the max length was input*/
         ++siArg;
         tmpStr = (signed char *) argAryStr[siArg];   

         tmpStr +=
            strToUI_base10str(
               (signed char *) argAryStr[siArg],
               &setSTPtr->maxLenUI
            );

         if(*tmpStr != '\0')
         { /*If: invalid number*/
            fprintf(
               stderr,
               "-max-len %s is non-numeric or to large%s",
               argAryStr[siArg],
               str_endLine
            );

            goto err_fun05_sec04;
         } /*If: invalid number*/
      } /*Else If: the max length was input*/

      else if(
         ! eql_charCp(
            (signed char *) "-min-aln-len",
            (signed char *) argAryStr[siArg],
            (signed char) '\0'
         )
      ){ /*Else If: min aligned length was input*/
         ++siArg;
         tmpStr = (signed char *) argAryStr[siArg];   

         tmpStr +=
            strToUI_base10str(
               (signed char *) argAryStr[siArg],
               &setSTPtr->minAlnLenUI
            );

         if(*tmpStr != '\0')
         { /*If: invalid number*/
            fprintf(
               stderr,
               "-min-aln-len %s; non-numeric/to large%s",
               argAryStr[siArg],
               str_endLine
            );

            goto err_fun05_sec04;
         } /*If: invalid number*/
      } /*Else If: min aligned length was input*/

      /**************************************************\
      * Fun05 Sec03 Sub05:
      *   - quality score filtering
      \**************************************************/

      else if(
         ! eql_charCp(
            (signed char *) "-min-median-q",
            (signed char *) argAryStr[siArg],
            (signed char) '\0'
         )
      ){ /*Else If: the min medain Q score was input*/
         ++siArg;
         setSTPtr->minMedianQF = atof(argAryStr[siArg]);
      } /*Else If: the min medain Q score was input*/

      else if(
         ! eql_charCp(
            (signed char *) "-min-mean-q",
            (signed char *) argAryStr[siArg],
            (signed char) '\0'
         )
      ){ /*Else If: the min medain Q score was input*/
         ++siArg;
         setSTPtr->minMeanQF = atof(argAryStr[siArg]);
      } /*Else If: the min medain Q score was input*/

      else if(
         ! eql_charCp(
            (signed char *) "-min-mapq",
            (signed char *) argAryStr[siArg],
            (signed char) '\0'
         )
      ){ /*Else If: the min mapq was input*/
         ++siArg;
         tmpStr = (signed char *) argAryStr[siArg];   

         tmpStr +=
            strToUC_base10str(
               (signed char *) argAryStr[siArg],
               &setSTPtr->minMapqUC
            );

         if(*tmpStr != '\0')
         { /*If: invalid number*/
            fprintf(
               stderr,
               "-min-mapq %s; non-numeric or to large%s",
               argAryStr[siArg],
               str_endLine
            );

            goto err_fun05_sec04;
         } /*If: invalid number*/
      } /*Else If: the min mapq was input*/

      /**************************************************\
      * Fun05 Sec03 Sub06:
      *   - booleans (trimming/how to treat masked bases)
      *   o fun05 sec03 sub06 cat01:
      *     - for stats; print anonymous base count
      *   o fun05 sec03 sub06 cat02:
      *     - trim masked bases at ends
      *   o fun05 sec03 sub06 cat03:
      *     - trim bases not in coordinate range
      *   o fun05 sec03 sub06 cat04:
      *     - trim starting bases
      *   o fun05 sec03 sub06 cat05:
      *     - trim ending bases
      \**************************************************/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun05 Sec03 Sub06 Cat01:
      +   - for stats; print anonymous base count
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      else if(
         ! eql_charCp(
            (signed char *) "-p-n",
            (signed char *) argAryStr[siArg],
            (signed char) '\0'
         )
      ) setSTPtr->pNsBl = 1;

      else if(
         ! eql_charCp(
            (signed char *) "-no-p-n",
            (signed char *) argAryStr[siArg],
            (signed char) '\0'
         )
      ) setSTPtr->pNsBl = 0;

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun05 Sec03 Sub06 Cat02:
      +   - trim masked bases at ends
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      else if(
         ! eql_charCp(
            (signed char *) "-trim",
            (signed char *) argAryStr[siArg],
            (signed char) '\0'
         )
      ) setSTPtr->trimMaskBl = 1;

      else if(
         ! eql_charCp(
            (signed char *) "-no-trim",
            (signed char *) argAryStr[siArg],
            (signed char) '\0'
         )
      ) setSTPtr->trimMaskBl = 0;

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun05 Sec03 Sub06 Cat03:
      +   - trim bases not in coordinate range
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      else if(
         ! eql_charCp(
            (signed char *) "-coord-trim",
            (signed char *) argAryStr[siArg],
            (signed char) '\0'
         )
      ) setSTPtr->coordTrimBl = 1;

      else if(
         ! eql_charCp(
            (signed char *) "-no-coord-trim",
            (signed char *) argAryStr[siArg],
            (signed char) '\0'
         )
      ) setSTPtr->coordTrimBl = 0;


      else if(
         ! eql_charCp(
            (signed char *) "-in-range",
            (signed char *) argAryStr[siArg],
            (signed char) '\0'
         )
      ) setSTPtr->inRangeBl = 1;

      else if(
         ! eql_charCp(
            (signed char *) "-no-range",
            (signed char *) argAryStr[siArg],
            (signed char) '\0'
         )
      ) setSTPtr->inRangeBl = 0;

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun05 Sec03 Sub06 Cat04:
      +   - trim starting bases
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      else if(
         ! eql_charCp(
            (signed char *) "-trim-start-strict",
            (signed char *) argAryStr[siArg],
            (signed char) '\0'
         )
      ){ /*Else If: doing a strict trim*/
            setSTPtr->trimStrictBl |= 1;
            goto trimStart_fun05_sec03_sub06_cat04;
      }  /*Else If: doing a strict trim*/

      else if(
         ! eql_charCp(
            (signed char *) "-trim-start",
            (signed char *) argAryStr[siArg],
            (signed char) '\0'
         )
      ){ /*Else If: the min length was input*/
         setSTPtr->trimStrictBl &= ~1;
         trimStart_fun05_sec03_sub06_cat04:;

         ++siArg;
         tmpStr = (signed char *) argAryStr[siArg];   

         tmpStr +=
            strToSI_base10str(
               (signed char *) argAryStr[siArg],
               &setSTPtr->trimStartSI
            );

         if(*tmpStr != '\0')
         { /*If: invalid number*/
            fprintf(
               stderr,
               "-trim-start %s; non-numeric/to large%s",
               argAryStr[siArg],
               str_endLine
            );

            goto err_fun05_sec04;
         } /*If: invalid number*/

         else if(setSTPtr->trimEndSI < 0)
         { /*Else If: negative number*/
            fprintf(
               stderr,
               "-trim-start %s; must be postive%s",
               argAryStr[siArg],
               str_endLine
            );

            goto err_fun05_sec04;
         } /*Else If: negative number*/
      } /*Else If: the min length was input*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun05 Sec03 Sub06 Cat05:
      +   - trim ending bases
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      else if(
         ! eql_charCp(
            (signed char *) "-trim-end-strict",
            (signed char *) argAryStr[siArg],
            (signed char) '\0'
         )
      ){ /*Else If: doing a strict trim*/
            setSTPtr->trimStrictBl |= 2;
            goto trimEnd_fun05_sec03_sub06_cat05;
      }  /*Else If: doing a strict trim*/

      else if(
         ! eql_charCp(
            (signed char *) "-trim-end",
            (signed char *) argAryStr[siArg],
            (signed char) '\0'
         )
      ){ /*Else If: the min length was input*/
         setSTPtr->trimStrictBl &= ~1;
         trimEnd_fun05_sec03_sub06_cat05:;

         ++siArg;
         tmpStr = (signed char *) argAryStr[siArg];   

         tmpStr +=
            strToSI_base10str(
               (signed char *) argAryStr[siArg],
               &setSTPtr->trimEndSI
            );

         if(*tmpStr != '\0')
         { /*If: invalid number*/
            fprintf(
               stderr,
               "-trim-end %s; non-numeric/to large%s",
               argAryStr[siArg],
               str_endLine
            );

            goto err_fun05_sec04;
         } /*If: invalid number*/

         else if(setSTPtr->trimEndSI < 0)
         { /*Else If: negative number*/
            fprintf(
               stderr,
               "-trim-end %s; must be postive%s",
               argAryStr[siArg],
               str_endLine
            );

            goto err_fun05_sec04;
         } /*Else If: negative number*/
      } /*Else If: the min length was input*/

      /**************************************************\
      * Fun05 Sec03 Sub07:
      *   - invert coordinates
      \**************************************************/

      else if(
         ! eql_charCp(
            (signed char *) "-v-coords",
            (signed char *) argAryStr[siArg],
            (signed char) '\0'
         )
      ) setSTPtr->invertCoordBl = 1;

      else if(
         ! eql_charCp(
            (signed char *) "-no-v-coords",
            (signed char *) argAryStr[siArg],
            (signed char) '\0'
         )
      ) setSTPtr->invertCoordBl = 0;

      /**************************************************\
      * Fun05 Sec03 Sub08:
      *   - getting coordiantes (from input)
      \**************************************************/

      else if(
         ! eql_charCp(
            (signed char *) "-coords",
            (signed char *) argAryStr[siArg],
            (signed char) '\0'
         )
      ){ /*Else If: an target coordinate was input*/
         ++siArg;
         errSC =
            strToCoord_mainFiltsam(
               (signed char *) argAryStr[siArg],
               coordArySTPtr,
               *numCoordsSI,
               &sizeSI,
               0
            );
         ++*numCoordsSI;

         if(errSC == 2)
         { /*If: The user only input one coordinate*/
            fprintf(
               stderr,
               "memory error getting -coords %s%s",
               argAryStr[siArg],
               str_endLine
            );

            goto err_fun05_sec04;
         } /*If: The user only input one coordinate*/

         else if(errSC)
         { /*If: The user only input one coordinate*/
            fprintf(
               stderr,
               "-coords %s is not a coordiante%s",
               argAryStr[siArg],
               str_endLine
            );

            goto err_fun05_sec04;
         } /*If: The user only input one coordinate*/
      } /*Else If: an target coordinate was input*/

      /**************************************************\
      * Fun05 Sec03 Sub09:
      *   - file, get coordiantes
      \**************************************************/

      else if(
         ! eql_charCp(
            (signed char *) "-coords-file",
            (signed char *) argAryStr[siArg],
            (signed char) '\0'
         )
      ){ /*Else If: An coordinates file was input*/
         ++siArg;

         errSI =
            getCoords_mainFiltsam(
               (signed char *) argAryStr[siArg],
               numCoordsSI,
               &sizeSI,
               coordArySTPtr
            );

         if(errSI == 1)
         { /*If: memory error*/
            fprintf(
               stderr, 
               "memory error reading -coords-file %s%s",
                argAryStr[siArg],
                str_endLine
            );

            goto err_fun05_sec04;
         } /*If: memory error*/

         else if(errSI )
         { /*Else If: file error*/
            fprintf(
             stderr, 
             "%i is a invalid lineat (-coords-file %s)%s",
             errSI >> 2,
             argAryStr[siArg],
             str_endLine
            );

            goto err_fun05_sec04;
         } /*Else If: file error*/
      } /*Else If: An coordinates file was input*/

      /**************************************************\
      * Fun05 Sec03 Sub10:
      *   - help message
      \**************************************************/

      else if(
         ! eql_charCp(
            (signed char *) "-h",
            (signed char *) argAryStr[siArg],
            (signed char) '\0'
         )
      ){ /*Else If: wanted help message*/
         phelp_mainFiltsam(stdout);
         goto phelp_fun05_sec04;
      } /*Else If: wanted help message*/

      else if(
         ! eql_charCp(
            (signed char *) "--h",
            (signed char *) argAryStr[siArg],
            (signed char) '\0'
         )
      ){ /*Else If: wanted help message*/
         phelp_mainFiltsam(stdout);
         goto phelp_fun05_sec04;
      } /*Else If: wanted help message*/

      else if(
         ! eql_charCp(
            (signed char *) "help",
            (signed char *) argAryStr[siArg],
            (signed char) '\0'
         )
      ){ /*Else If: wanted help message*/
         phelp_mainFiltsam(stdout);
         goto phelp_fun05_sec04;
      } /*Else If: wanted help message*/

      else if(
         ! eql_charCp(
            (signed char *) "-help",
            (signed char *) argAryStr[siArg],
            (signed char) '\0'
         )
      ){ /*Else If: wanted help message*/
         phelp_mainFiltsam(stdout);
         goto phelp_fun05_sec04;
      } /*Else If: wanted help message*/

      else if(
         ! eql_charCp(
            (signed char *) "--help",
            (signed char *) argAryStr[siArg],
            (signed char) '\0'
         )
      ){ /*Else If: wanted help message*/
         phelp_mainFiltsam(stdout);
         goto phelp_fun05_sec04;
      } /*Else If: wanted help message*/

      /**************************************************\
      * Fun05 Sec03 Sub11:
      *   - version number
      \**************************************************/

      else if(
         ! eql_charCp(
            (signed char *) "-v",
            (signed char *) argAryStr[siArg],
            (signed char) '\0'
         )
      ){ /*Else If: wanted version number*/
         pversion_mainFiltsam(stdout);
         goto pversion_fun05_sec04;
      } /*Else If: wanted version number*/

      else if(
         ! eql_charCp(
            (signed char *) "--v",
            (signed char *) argAryStr[siArg],
            (signed char) '\0'
         )
      ){ /*Else If: wanted version number*/
         pversion_mainFiltsam(stdout);
         goto pversion_fun05_sec04;
      } /*Else If: wanted version number*/

      else if(
         ! eql_charCp(
            (signed char *) "version",
            (signed char *) argAryStr[siArg],
            (signed char) '\0'
         )
      ){ /*Else If: wanted version number*/
         pversion_mainFiltsam(stdout);
         goto pversion_fun05_sec04;
      } /*Else If: wanted version number*/

      else if(
         ! eql_charCp(
            (signed char *) "-version",
            (signed char *) argAryStr[siArg],
            (signed char) '\0'
         )
      ){ /*Else If: wanted version number*/
         pversion_mainFiltsam(stdout);
         goto pversion_fun05_sec04;
      } /*Else If: wanted version number*/

      else if(
         ! eql_charCp(
            (signed char *) "--version",
            (signed char *) argAryStr[siArg],
            (signed char) '\0'
         )
      ){ /*Else If: wanted version number*/
         pversion_mainFiltsam(stdout);
         goto pversion_fun05_sec04;
      } /*Else If: wanted version number*/

      /**************************************************\
      * Fun05 Sec03 Sub12:
      *   - invalid input
      \**************************************************/

      else
      { /*Else: invalid input*/
         fprintf(
            stderr,
            "%s not recognized%s",
            argAryStr[siArg],
            str_endLine
         );

         goto err_fun05_sec04;
      } /*Else: invalid input*/

      /**************************************************\
      * Fun05 Sec03 Sub13:
      *   - get next argument
      \**************************************************/

      ++siArg;
   } /*Loop: Get user input*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun05 Sec04:
   ^   - return errors (or no error)
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   errSC = 0;
   goto ret_fun05_sec04;

   phelp_fun05_sec04:;
   pversion_fun05_sec04:;
   errSC = 1;
   goto ret_fun05_sec04;

   err_fun05_sec04:;
   errSC = 2;
   goto ret_fun05_sec04;

   ret_fun05_sec04:;
   return errSC;
} /*input_mainFiltsam*/

/*-------------------------------------------------------\
| Main:
|   - filters sam files and converts sam output to the
|     desiered format
| Input:
| Output:
|   - Prints:
|     o filtered sam filt to format speficified by
|       "-out-format"
\-------------------------------------------------------*/
int main(
   int numArgsSI,
   char *argAryStr[]
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Main TOC:
   '   - filters sam files and converts sam output to the
   '     desiered format
   '   o main sec01:
   '     - variable declerations
   '   o main sec02:
   '     - check user input
   '   o main sec03:
   '     - print/skip headers
   '   o main sec04:
   '     - filter sam file
   '   o main sec05:
   '     - clean up and exit
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec01:
   ^   - variable declerations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   signed char *samStr = 0;
   signed char *outStr = 0;
   struct set_mainFiltsam setST;
   unsigned int usFlag = 0; /*loop iterator for flags*/

   /*For coordinate filtering*/
   struct coord_mainFiltsam *coordHeapArySTPtr = 0;
   signed int numCoordsSI = 0;
   signed int siPos = 0;

   FILE *samFILE = 0;
   FILE *outFILE = 0;

   struct samEntry samStackST;

   signed char errSC = 0;    /*error reporting*/
   signed char pHeadBl = 1;  /*printing a stats tsv file*/
   unsigned long ulEntry = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec02:
   ^   - check user input
   ^   o main sec02 sub01:
   ^     - initialize variables
   ^   o main sec02 sub02:
   ^     - get user input
   ^   o main sec02 sub03:
   ^     - open input sam file
   ^   o main sec02 sub04:
   ^     - open provided output file
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Main Sec02 Sub01:
   *   - initialize variables
   \*****************************************************/

   init_samEntry(&samStackST);

   setST.outFlagSC = def_pdefault_mainFiltsam;
   setST.pNsBl = def_nPrint_mainFiltsam;
   setST.inRangeBl = def_inRange_mainFiltsam;

   setST.rmAryUS[0] = 0;
   setST.rmLenUS = 0;
   setST.keepAryUS[0] = 0;
   setST.keepLenUS = 0;

   setST.trimMaskBl = def_trimMaskBl_mainFiltsam;
   setST.trimStartSI = def_startTrim_mainFiltsam;
   setST.trimEndSI = def_endTrim_mainFiltsam;
   setST.trimStrictBl = def_startStrictTrim_mainFiltsam;
   setST.trimStrictBl |= def_endStrictTrim_mainFiltsam;

   setST.minLenUI = def_minLen_mainFiltsam;
   setST.maxLenUI = def_maxLen_mainFiltsam;
   setST.minAlnLenUI = def_minAlnLen_mainFiltsam;
   setST.minMedianQF = def_minMedQ_mainFiltsam;
   setST.minMeanQF = def_minMeanQ_mainFiltsam;
   setST.minMapqUC = def_minMapq_mainFiltsam;

   setST.invertCoordBl = def_invertCoord_mainFiltsam;
   setST.coordTrimBl = def_coordsTrimBl_mainFiltsam;

   /*****************************************************\
   * Main Sec02 Sub02:
   *   - get user input
   \*****************************************************/

   errSC =
      input_mainFiltsam(
         numArgsSI,
         argAryStr,
         &samStr,
         &outStr,
         &setST,
         &coordHeapArySTPtr,
         &numCoordsSI
      ); /*Get the user input*/

   if(errSC)
   { /*If: I had an error*/
      --errSC; /*reduce help/version print to no error*/
      goto cleanUp_main_sec05_sub04;
   } /*If: I had an error*/

   /*****************************************************\
   * Main Sec02 Sub03:
   *   - open input sam file
   \*****************************************************/

   if(! samStr)
      samFILE = stdin;
   else if(*samStr == '-')
      samFILE = stdin;
   else
   { /*Else: sam file input*/
      samFILE = fopen((char *) samStr, "r");

      if(!samFILE)
      { /*If: I could not open the sam file*/
         fprintf(
             stderr,
             "-sam %s could not be opened%s",
             samStr,
             str_endLine
         );

         goto fileErr_main_sec05_sub03;
      } /*If: I could not open the sam file*/
   } /*Else: sam file input*/

   /*****************************************************\
   * Main Sec02 Sub04:
   *   - open provided output file
   \*****************************************************/

   if(! outStr)
      outFILE = stdout;
   else if(outStr[0] == '-')
      outFILE = stdout;
   else
   { /*Else: output file input*/
      outFILE = fopen((char *) outStr, "w");

      if(!outFILE)
      { /*If: I could not open the out file*/
         fprintf(
             stderr,
             "-out %s could not be opened%s",
             outStr,
             str_endLine
         );

         goto fileErr_main_sec05_sub03;
      } /*If: I could not open the out file*/
   } /*Else: output file input*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec03:
   ^   - print/skip headers
   ^   o main sec04 sub01:
   ^     - set up samEntry struct and read first line
   ^   o main sec04 sub02:
   ^     - print headers in sam file (or get past)
   ^   o main sec04 sub03:
   ^     - print this programs header (sam output only)
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Main Sec03 Sub01:
   *   - set up samEntry struct and read first line
   \*****************************************************/

   errSC = setup_samEntry(&samStackST);

   if(errSC)
   { /*If: I had a memory error*/
      fprintf(
         stderr,
         "memory error setting up samEntry struct%s",
         str_endLine
      );

      goto memErr_main_sec05_sub02;
   } /*If: I had a memory error*/

   errSC = get_samEntry(&samStackST, samFILE);

   /*****************************************************\
   * Main Sec03 Sub02:
   *   - print headers in sam file (or get past)
   \*****************************************************/

   while(! errSC)
   { /*Loop: print header*/
      if(samStackST.extraStr[0] != '@')
         break; /*finished headers*/

      if(setST.outFlagSC == def_psam_mainFiltsam)
         p_samEntry(&samStackST, 0, outFILE);
         /*printing sam file out*/

      errSC = get_samEntry(&samStackST, samFILE);
   } /*Loop: print header*/

   /*****************************************************\
   * Main Sec03 Sub03:
   *   - print this programs header (sam output only)
   \*****************************************************/

   if(setST.outFlagSC == def_psam_mainFiltsam)
   { /*If: printing program header*/
      fprintf(
        outFILE,
        "@PG\tID:filtsam\tPN:filtsam"
      );

      fprintf(
        outFILE,
        "\tVN:%i-%02i-%02i\tCL:filtsam",
        def_year_bioTools,
        def_month_bioTools,
        def_day_bioTools
      );

      for(
         ulEntry = 1;
         ulEntry < (unsigned int) numArgsSI;
         ++ulEntry
      ) fprintf(outFILE, " %s", argAryStr[ulEntry]);
        /*print input commands*/

      ulEntry = 0;
      fprintf(outFILE, "%s", str_endLine);
   } /*If: printing program header*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec04:
   ^   - filter sam file
   ^   o main sec03 sub01:
   ^     - filter by length and q-scores + start loop
   ^   o main sec03 sub02:
   ^     - trim reads (if requested)
   ^   o main sec03 sub03:
   ^     - discard reads without wanted (-f) flags
   ^   o main sec03 sub04:
   ^     - discard reads with unwanted (-F) flags
   ^   o main sec03 sub05:
   ^     - check coorinates of read (against input range)
   ^   o main sec03 sub06:
   ^     - print kept reads
   ^   o main sec03 sub07:
   ^     - get next sam file entry
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Main Sec03 Sub01:
   *   - filter by length and q-scores + start loop
   \*****************************************************/

   ulEntry = 0;

   while(! errSC)
   { /*Loop: filter reads in sam file*/
      ++ulEntry;         

      if(samStackST.readLenUI < setST.minLenUI)
         goto nextEntry_main_sec04_sub07;

      if(samStackST.readLenUI < setST.maxLenUI)
         ;
      else if(setST.maxLenUI > 0)
         goto nextEntry_main_sec04_sub07;
      
      if(samStackST.medianQF < setST.minMedianQF)
         goto nextEntry_main_sec04_sub07;

      if(samStackST.meanQF < setST.minMeanQF)
         goto nextEntry_main_sec04_sub07;

      if(samStackST.mapqUC < setST.minMapqUC)
         goto nextEntry_main_sec04_sub07;

      if(samStackST.alnReadLenUI < setST.minAlnLenUI)
         goto nextEntry_main_sec04_sub07;

      /**************************************************\
      * Main Sec03 Sub02:
      *   - trim reads (if requested)
      \**************************************************/

      if(setST.trimMaskBl)
      { /*If: trimming reads*/
         errSC = seq_trimSam(&samStackST);

         if(errSC)
            goto nextEntry_main_sec04_sub07;
            /*no sequehce to trim*/
      } /*If: trimming reads*/

      if(samStackST.readLenUI < setST.minLenUI)
         goto nextEntry_main_sec04_sub07;
      if(samStackST.medianQF < setST.minMedianQF)
         goto nextEntry_main_sec04_sub07;
      if(samStackST.meanQF < setST.minMeanQF)
         goto nextEntry_main_sec04_sub07;

 
      if(setST.trimStartSI || setST.trimEndSI)
      { /*If: trimming reads*/
         errSC =
            ends_trimSam(
               &samStackST,
               setST.trimStartSI,
               setST.trimEndSI,
               setST.trimStrictBl
            );

         if(errSC)
            goto nextEntry_main_sec04_sub07;
            /*no sequehce to trim*/


         if(samStackST.readLenUI < setST.minLenUI)
            goto nextEntry_main_sec04_sub07;
         if(samStackST.medianQF < setST.minMedianQF)
            goto nextEntry_main_sec04_sub07;
         if(samStackST.meanQF < setST.minMeanQF)
            goto nextEntry_main_sec04_sub07;
         if(samStackST.alnReadLenUI < setST.minAlnLenUI)
            goto nextEntry_main_sec04_sub07;
      } /*If: trimming reads*/

      /**************************************************\
      * Main Sec03 Sub03:
      *   - discard reads without wanted (-f) flags
      \**************************************************/

      /*Ideally I would sort by flag and then reduce to
      ` the minimum flag
      */
      for(usFlag = 0; usFlag < setST.keepLenUS; ++usFlag)
      { /*Loop: Check all keep read flags*/
         if(samStackST.flagUS & setST.keepAryUS[usFlag])
            goto keepFlagFound_main_sec03_sub03;
         else if(
            samStackST.flagUS == setST.keepAryUS[usFlag]
         ) goto keepFlagFound_main_sec03_sub03;
            /*for 0 flag*/
      } /*Loop: Check all keep read flags*/

      if(setST.keepLenUS > 0)
         goto nextEntry_main_sec04_sub07;

      keepFlagFound_main_sec03_sub03:;

      /**************************************************\
      * Main Sec03 Sub04:
      *   - discard reads with unwanted (-F) flags
      \**************************************************/

      for(usFlag = 0; usFlag < setST.rmLenUS; ++usFlag)
      { /*Loop: Check all keep read flags*/
         if(samStackST.flagUS & setST.rmAryUS[usFlag])
            goto nextEntry_main_sec04_sub07;
         else if(
            samStackST.flagUS == setST.rmAryUS[usFlag]
         ) goto nextEntry_main_sec04_sub07;
            /*this is to catch the 0 flag*/
      } /*Loop: Check all keep read flags*/

      /**************************************************\
      * Main Sec03 Sub05:
      *   - check coorinates of read (against input range)
      \**************************************************/

      for(siPos = 0; siPos < numCoordsSI; ++siPos)
      { /*Loop: See if have requested coordinates*/
         if(coordHeapArySTPtr[siPos].refStr[0])
         { /*If: have reference id to check*/
            if(
               eqlNull_ulCp(
                  samStackST.refIdStr,
                  coordHeapArySTPtr[siPos].refStr
               )
            ) continue; /*If: different reference*/
         } /*If: have reference id to check*/

         if(setST.inRangeBl)
         { /*If: read is in coordinate range*/
            if(
                  samStackST.refStartUI
               > coordHeapArySTPtr[siPos].endUI
            ) continue; /*out of range*/
            else if(
                  samStackST.refEndUI
               >= coordHeapArySTPtr[siPos].startUI
            ){ /*If: read covers the range*/
               if(setST.coordTrimBl)
               { /*If: I am trimming to coordiantes*/
                  errSC =
                     coords_trimSam(
                        &samStackST,
                        coordHeapArySTPtr[siPos].startUI,
                        coordHeapArySTPtr[siPos].endUI
                     );

                  if(errSC)
                     goto nextEntry_main_sec04_sub07;
                     /*no sequece to trim/out of range*/
               } /*If: I am trimming to coordiantes*/

               break;
            } /*If: read covers the range*/
         } /*If: read is in coordinate range*/

         else if(
               samStackST.refStartUI
            > coordHeapArySTPtr[siPos].startUI
         ) ; /*read does not cover entire range*/

         else if(
               samStackST.refStartUI
            >= coordHeapArySTPtr[siPos].endUI
         ){ /*Else If: read covers coordinates*/
            if(setST.coordTrimBl)
            { /*If: I am trimming to coordiantes*/
               errSC =
                  coords_trimSam(
                     &samStackST,
                     coordHeapArySTPtr[siPos].startUI,
                     coordHeapArySTPtr[siPos].endUI
                  );

               if(errSC)
                  goto nextEntry_main_sec04_sub07;
                  /*no sequece to trim/out of range*/
            } /*If: I am trimming to coordiantes*/

            break;
         } /*Else If: read covers coordinates*/
      } /*Loop: See if have requested coordinates*/

      if(numCoordsSI <= 0)
         ; /*not filtering by coordaintes*/

      else if(siPos >= numCoordsSI)
      { /*Else If: no match found*/
         if(! setST.invertCoordBl)
            goto nextEntry_main_sec04_sub07;
            /*read did not match any coordinates*/
      } /*Else If: no match found*/

      else
      { /*Else: match found*/
         if(setST.invertCoordBl)
            goto nextEntry_main_sec04_sub07;
            /*read did not match any coordinates*/
      } /*Else: match found*/

      /**************************************************\
      * Main Sec03 Sub06:
      *   - print kept reads
      \**************************************************/

      switch(setST.outFlagSC)
      { /*Switch; decide printing method*/
         case def_psam_mainFiltsam:
         /*Case: sam file printing*/
            p_samEntry(&samStackST, 0, outFILE);
            break;
         /*Case: sam file printing*/

         case def_pfq_mainFiltsam:
         /*Case: fastq print*/
            pfq_samEntry(&samStackST, outFILE);
            break;
         /*Case: fastq print*/

         case def_pfa_mainFiltsam:
         /*Case: fasta print*/
            pfa_samEntry(&samStackST, outFILE);
            break;
         /*Case: fasta print*/

         case def_pstats_mainFiltsam:
         /*Case: stats print*/
            pstats_samEntry(
               &samStackST,
               &pHeadBl,
               setST.pNsBl,
               outFILE
            );
            break;
         /*Case: stats print*/

      } /*Switch; decide printing method*/

      /**************************************************\
      * Main Sec03 Sub07:
      *   - get next sam file entry
      \**************************************************/

      nextEntry_main_sec04_sub07:
         errSC = get_samEntry(&samStackST, samFILE);
   } /*Loop: filter reads in sam file*/

   /*****************************************************\
   * Main Sec03 Sub08:
   *   - check error types
   \*****************************************************/

   if(errSC == def_memErr_samEntry)
   { /*If: had memory error*/
      fprintf(
         stderr,
         "ran out of memory on read %lu%s",
         ulEntry,
         str_endLine
      );

      goto memErr_main_sec05_sub02;
   } /*If: had memory error*/

   else if(errSC == def_fileErr_samEntry)
   { /*If: had memory error*/
      fprintf(
         stderr,
         "file error on read %lu%s",
         ulEntry,
         str_endLine
      );

      goto fileErr_main_sec05_sub03;
   } /*If: had memory error*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec05:
   ^   - clean up and exit
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   errSC = 0;
   goto cleanUp_main_sec05_sub04;

   memErr_main_sec05_sub02:;
      errSC = 1;
      goto cleanUp_main_sec05_sub04;

   fileErr_main_sec05_sub03:;
      errSC = 2;
      goto cleanUp_main_sec05_sub04;

   cleanUp_main_sec05_sub04:;
      /*do not need to free stST; everything on stack*/

      freeStack_samEntry(&samStackST);
      free(coordHeapArySTPtr);
      coordHeapArySTPtr = 0;

      if(! samFILE) ;
      else if(samFILE == stdin) ;
      else if(samFILE == stdout) ;
      else
         fclose(samFILE);
      samFILE = 0;

      if(! outFILE) ;
      else if(outFILE == stdin) ;
      else if(outFILE == stdout) ;
      else
         fclose(outFILE);
      outFILE = 0;

      return(errSC);
} /*main*/

/*=======================================================\
: License:
: 
: This code is under the unlicense (public domain).
:   However, for cases were the public domain is not
:   suitable, such as countries that do not respect the
:   public domain or were working with the public domain
:   is inconveint / not possible, this code is under the
:   MIT license
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
