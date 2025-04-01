/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' mainMapRead SOF: Start Of File
'   - maps reads to reference
'   o header:
'     - included libraries
'   o fun01: pversion_mainMapRead
'     - prints version number for map read
'   o fun02: phelp_mainMapRead
'     - prints help message for mapRead
'   o fun03: phelp_mainMapRead
'     - gets user input for mapRead
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*TODO: work on memory efficancy for kmer scan, add in
`  min(sequence length length, kmer space) for kmer array
`  so only using maximum kmer space for count table
*/

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
#include "../genLib/ptrAry.h"

#include "../genBio/seqST.h"
#include "../genBio/samEntry.h"

#include "../genAln/alnSet.h"
#include "../genAln/mapRead.h"

/*only .h files*/
#include "../bioTools.h"
#include "../genAln/defsMapRead.h"
#include "../genAln/alnDefs.h"

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\
! Hidden libraries
!   - std #include <stdio.h>
!   - .c  #include "../genLib/base10Str.h"
!   - .c  #include "../genLib/numToStr.h"
!   - .c  #include "../genLib/charCp.h"
!   - .c  #include "../genLib/shellSort.h"
!   - .c  #include "../genLib/strAry.h"
!   - .c  #include "../genAln/indexToCoord.h"
!   - .c  #include "../genAln/memwater.h"
!   - .c  #include "../genAln/dirMatrix.h"
!   - .c  #include "../genAln/needle.h"
!   - .c  #include "../genAln/water.h"
!   - .c  #include "../genAln/kmerFind.h"
!   - .h  #include "../genBio/ntTo5Bit.h"
!   - .h  #include "../genLib/dataTypeShortHand.h"
\%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

#define def_fqQry_mainMapRead 0
#define def_gzQry_mainMapRead 1
#define def_faQry_mainMapRead 2

#define def_quick_mainMapRead 1 /*1: quick method*/

/*-------------------------------------------------------\
| Fun01: pversion_mainMapRead
|   - prints version number for map read
| Input:
|   - outFILE:
|     o FILE pointer to print version number to
| Output:
|   - Prints:
|     o version number to outFILE
\-------------------------------------------------------*/
void
pversion_mainMapRead(
   void *outFILE
){
   fprintf(
      (FILE *) outFILE,
      "mapRead from bioTools version: %i-%02i-%02i\n",
      def_year_bioTools,
      def_month_bioTools,
      def_day_bioTools
   );
} /*pversion_mainMapRead*/

/*-------------------------------------------------------\
| Fun02: phelp_mainMapRead
|   - prints help message for mapRead
| Input:
|   - outFILE:
|     o FILE pointer to print help message to
| Output:
|   - Prints:
|     o help message to outFILE
\-------------------------------------------------------*/
void
phelp_mainMapRead(
   void *outFILE
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun02 TOC:
   '   - prints help message for mapRead
   '   o fun02 sec01:
   '     - usage entry
   '   o fun02 sec02:
   '     - input entry
   '   o fun02 sec03:
   '     - output entry
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun02 Sec01:
   ^   - usage entry
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   fprintf(
      (FILE *) outFILE,
      "mapRead -fq reads.fq -ref ref.fa > out.sam\n"
   );

   fprintf(
      (FILE *) outFILE,
      "  - maps reads to reference\n"
   );

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun02 Sec02:
   ^   - input entry
   ^   o fun02 sec02 sub01:
   ^     - input block header
   ^   o fun02 sec02 sub02:
   ^     - query input
   ^   o fun02 sec02 sub03:
   ^     - reference input and output file
   ^   o fun02 sec02 sub04:
   ^     - kmerScan variables
   ^   o fun02 sec02 sub05:
   ^     - chaining and scoring variables
   ^   o fun02 sec02 sub06:
   ^     - help message and version number
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun02 Sec02 Sub01:
   *   - input block header and quick map input
   \*****************************************************/

   fprintf(
      (FILE *) outFILE,
      "Input:\n"
   );


   fprintf(
      (FILE *) outFILE,
      "  -quick: [Option; Yes]\n"
   );

   fprintf(
      (FILE *) outFILE,
      "    o scan for best kmer window, the align\n"
   );

   fprintf(
      (FILE *) outFILE,
      "    o quicker, but not as percise\n"
   );

   fprintf(
      (FILE *) outFILE,
      "    o disable with \"-slow\"\n"
   );

   /*****************************************************\
   * Fun02 Sec02 Sub02:
   *   - fastq query file
   '   o fun02 sec02 sub02 cat01:
   '     - fastq entry header
   *   o fun02 sec02 sub02 cat02:
   *     - fastq query file
   *   o fun02 sec02 sub02 cat03:
   *     - fastq.gz query file
   *   o fun02 sec02 sub02 cat04:
   *     - fasta query file
   *   o fun02 sec02 sub02 cat05:
   *     - fasta.gz query file
   \*****************************************************/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun02 Sec02 Sub02 Cat01:
   +   - fastq entry header
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "  Query input:\n"
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun02 Sec02 Sub02 Cat02:
   +   - fastq query file
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "    -fq reads.fq: [Default: \"-fq -\"]\n"
   );

   fprintf(
      (FILE *) outFILE,
      "      o fastq file with reads to map\n"
   );

   fprintf(
      (FILE *) outFILE,
      "      o can add multiple times\n"
   );

   fprintf(
      (FILE *) outFILE,
      "      o use \"-fq -\" for stdin input\n"
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun02 Sec02 Sub02 Cat03:
   +   - fastq.gz query file
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "    -fq-gz reads.fq.gz: [Default: \"-fq -\"]\n"
   );

   fprintf(
      (FILE *) outFILE,
      "      o (TODO) fastq.gz file with reads to map\n"
   );

   fprintf(
      (FILE *) outFILE,
      "      o can add multiple times\n"
   );

   fprintf(
      (FILE *) outFILE,
      "      o use \"-fq-gz -\" for stdin input\n"
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun02 Sec02 Sub02 Cat04:
   +   - fasta query file
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "    -fa query.fa: [Default: \"-fq -\"]\n"
   );

   fprintf(
      (FILE *) outFILE,
      "      o fasta file with query to map\n"
   );

   fprintf(
      (FILE *) outFILE,
      "      o can add multiple times\n"
   );

   fprintf(
      (FILE *) outFILE,
      "      o use \"-fa -\" for stdin input\n"
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun02 Sec02 Sub02 Cat05:
   +   - fasta.gz query file
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "    -fa-gz query.fa.gz: [Default: \"-fq -\"]\n"
   );

   fprintf(
      (FILE *) outFILE,
      "      o (TODO) fasta.gz file with query to map\n"
   );

   fprintf(
      (FILE *) outFILE,
      "      o can add multiple times\n"
   );

   fprintf(
      (FILE *) outFILE,
      "      o use \"-fa-gz -\" for stdin input\n"
   );

   /*****************************************************\
   * Fun02 Sec02 Sub03:
   *   - reference input and output file
   *   o fun02 sec02 sub03 cat01:
   *     - reference fasta input file
   *   o fun02 sec02 sub03 cat02:
   *     - output file
   \*****************************************************/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun02 Sec02 Sub03 Cat01:
   +   - reference fasta input file
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "  File IO (non-query):\n"
   );


   fprintf(
      (FILE *) outFILE,
      "    -ref reference.fa: [Required]\n"
   );

   fprintf(
      (FILE *) outFILE,
      "      o fasta file with reference(s) to map\n"
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun02 Sec02 Sub03 Cat02:
   +   - output file
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "    -out file.sam: [Default: stdout]\n"
   );

   fprintf(
      (FILE *) outFILE,
      "      o file to save mapped reads to (sam file)\n"
   );

   fprintf(
      (FILE *) outFILE,
      "      o use \"-out -\" for stdin input\n"
   );

   /*****************************************************\
   * Fun02 Sec02 Sub04:
   *   - kmerScan variables
   *   o fun02 sec02 sub04 cat01:
   *     - kmerScan header
   *   o fun02 sec02 sub04 cat02:
   *     - extra nucleotides in window
   *   o fun02 sec02 sub04 cat03:
   *     - window percent shift
   *   o fun02 sec02 sub04 cat04:
   *     - variable kmerScan variables (length,size,%)
   \*****************************************************/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun02 Sec02 Sub04 Cat01:
   +   - kmerScan header
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "  kmerScan variables:\n"
   );

   fprintf(
      (FILE *) outFILE,
      "    - scans reference for windows with similar\n"
   );

   fprintf(
      (FILE *) outFILE,
      "      kmer counts\n"
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun02 Sec02 Sub04 Cat02:
   +   - extra nucleotides in window
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "    -kmer-extra-nt %0.2f: [Optional; %0.2f]\n",
      def_percExtraNt_defsMapRead,
      def_percExtraNt_defsMapRead
   );

   fprintf(
      (FILE *) outFILE,
      "      o used to find scan window size, which is\n"
   );

   fprintf(
      (FILE *) outFILE,
      "        a multiple of the query sequence length\n"
   );

   fprintf(
      (FILE *) outFILE,
      "      o length + (length * -kmer-extra-nt)\n"
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun02 Sec02 Sub04 Cat03:
   +   - window percent shift
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "    -kmer-shift %0.2f: [Optional; %0.2f]\n",
      def_percShift_defsMapRead,
      def_percShift_defsMapRead
   );

   fprintf(
      (FILE *) outFILE,
      "      o percentage of query length to shift\n"
   );

   fprintf(
      (FILE *) outFILE,
      "      o widow by each scan\n"
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun02 Sec02 Sub04 Cat04:
   +   - variable kmerScan variables (length,size,%,chain)
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "    -kmer-val length,size,perc,chain: [Optional]\n"
   );

   fprintf(
      (FILE *) outFILE,
      "      o kmer scan values to use based on query\n"
   );

   fprintf(
      (FILE *) outFILE,
      "        length (up to 16 values supported)\n"
   );


   fprintf(
      (FILE *) outFILE,
      "        * length: min query length to use\n"
   );

   fprintf(
      (FILE *) outFILE,
      "        * size: kmer size (ex 7mer) to use\n"
   );

   fprintf(
      (FILE *) outFILE,
      "        * perc: percentage of matching kmers\n"
   );

   fprintf(
      (FILE *) outFILE,
      "                needed to do an alignment\n"
   );


   fprintf(
      (FILE *) outFILE,
      "        * chain: minimum kmers in chain needed\n"
   );

   fprintf(
      (FILE *) outFILE,
      "                 to keep a chain\n"
   );

   fprintf(
      (FILE *) outFILE,
      "      o Defaults:\n"
   );

   fprintf(
      (FILE *) outFILE,
      "        * -kmer-val %i,%i,%0.2f,%i\n",
      def_minLen_defsMapRead,
      def_minKmer_defsMapRead,
      def_minPerc_defsMapRead,
      def_minChain_defsMapRead
   );

   fprintf(
      (FILE *) outFILE,
      "        * -kmer-val %i,%i,%0.2f,%i\n",
      def_secLen_defsMapRead,
      def_secKmer_defsMapRead,
      def_secPerc_defsMapRead,
      def_secChain_defsMapRead
   );

   fprintf(
      (FILE *) outFILE,
      "        * -kmer-val %i,%i,%0.2f,%i\n",
      def_thirdLen_defsMapRead,
      def_thirdKmer_defsMapRead,
      def_thirdPerc_defsMapRead,
      def_thirdChain_defsMapRead
   );

   fprintf(
      (FILE *) outFILE,
      "        * -kmer-val %i,%i,%0.2f,%i\n",
      def_fourthLen_defsMapRead,
      def_fourthKmer_defsMapRead,
      def_fourthPerc_defsMapRead,
      def_fourthChain_defsMapRead
   );

   fprintf(
      (FILE *) outFILE,
      "        * -kmer-val %i,%i,%0.2f,%i\n",
      def_fifthLen_defsMapRead,
      def_fifthKmer_defsMapRead,
      def_fifthPerc_defsMapRead,
      def_fifthChain_defsMapRead
   );

   fprintf(
      (FILE *) outFILE,
      "        * -kmer-val %i,%i,%0.2f,%i\n",
      def_sixthLen_defsMapRead,
      def_sixthKmer_defsMapRead,
      def_sixthPerc_defsMapRead,
      def_sixthChain_defsMapRead
   );

   fprintf(
      (FILE *) outFILE,
      "        * -kmer-val %i,%i,%0.2f,%i\n",
      def_finalLen_defsMapRead,
      def_finalKmer_defsMapRead,
      def_finalPerc_defsMapRead,
      def_finalChain_defsMapRead
   );

   /*****************************************************\
   * Fun02 Sec02 Sub05:
   *   - chaining and scoring variables
   *   o fun02 sec02 sub05 cat01:
   *     - chain and score header
   *   o fun02 sec02 sub05 cat02:
   *     - sub-alignment
   *   o fun02 sec02 sub05 cat03:
   *     - match score
   *   o fun02 sec02 sub05 cat04:
   *     - snp score
   *   o fun02 sec02 sub05 cat05:
   *     - gap open score
   *   o fun02 sec02 sub05 cat06:
   *     - gap extend score
   *   o fun02 sec02 sub05 cat07:
   *     - two anonymous base score
   *   o fun02 sec02 sub05 cat08:
   *     - three anonymous base score
   *   o fun02 sec02 sub05 cat09:
   *     - four anonymous base score
   *   o fun02 sec02 sub05 cat10:
   *     - min percent score
   *   o fun02 sec02 sub05 cat11:
   *     - min percent length
   \*****************************************************/
   
   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun02 Sec02 Sub05 Cat01:
   +   - chain and score header
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "  Chain and score:\n"
   );

   fprintf(
      (FILE *) outFILE,
      "    - alignment step (chaining & scoring step)\n"
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun02 Sec02 Sub05 Cat02:
   +   - sub-alignment
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   if(def_subAln_defsMapRead)
      fprintf(
         (FILE *) outFILE,
         "    -sub-aln: [Optional; Yes]\n"
      );
   else
      fprintf(
         (FILE *) outFILE,
         "    -sub-aln: [Optional; No]\n"
      );

   fprintf(
      (FILE *) outFILE,
      "      o during scoring step find and return the\n"
   );

   fprintf(
      (FILE *) outFILE,
      "        best sub-alignment\n"
   );

   fprintf(
      (FILE *) outFILE,
      "      o disable with \"-no-sub-aln\"\n"
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun02 Sec02 Sub05 Cat03:
   +   - match score
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "    -match 5: [Optional; 5]\n"
   );

   fprintf(
      (FILE *) outFILE,
      "      o score for a single match\n"
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun02 Sec02 Sub05 Cat04:
   +   - snp score
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "    -snp -4: [Optional; -4]\n"
   );

   fprintf(
      (FILE *) outFILE,
      "      o score for a single snp\n"
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun02 Sec02 Sub05 Cat05:
   +   - gap open score
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "    -gap-open %i: [Optional; %i]\n",
      def_gapOpen_alnDefs,
      def_gapOpen_alnDefs
   );

   fprintf(
      (FILE *) outFILE,
      "      o score for starting a gap (indel)\n"
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun02 Sec02 Sub05 Cat06:
   +   - gap extension score
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "    -gap-extend %i: [Optional; %i]\n",
      def_gapExtend_alnDefs,
      def_gapExtend_alnDefs
   );

   fprintf(
      (FILE *) outFILE,
      "      o score for extending a gap (indel)\n"
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun02 Sec02 Sub05 Cat07:
   +   - two anonymous base score
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "    -two-anon 1: [Optional; 1]\n"
   );

   fprintf(
      (FILE *) outFILE,
      "      o score for A,T,C,G matched to a W,S,M,K,\n"
   );

   fprintf(
      (FILE *) outFILE,
      "        R, or Y\n"
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun02 Sec02 Sub05 Cat08:
   +   - three anonymous base score
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "    -three-anon -1: [Optional; -1]\n"
   );

   fprintf(
      (FILE *) outFILE,
      "      o score for A,T,C,G matched to a B,D,H,V\n"
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun02 Sec02 Sub05 Cat09:
   +   - four anonymous base score
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "    -four-anon -2: [Optional; -2]\n"
   );

   fprintf(
      (FILE *) outFILE,
      "      o score for matching a base to a N\n"
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun02 Sec02 Sub05 Cat10:
   +   - min percent score
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "    -min-perc %0.2f: [Optional; %0.2f]\n",
      def_minScore_defsMapRead,
      def_minScore_defsMapRead
   );

   fprintf(
      (FILE *) outFILE,
      "      o mininum percent score to keep aligment\n"
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun02 Sec02 Sub05 Cat11:
   +   - min percent length
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "    -min-len-perc %0.2f: [Optional; %0.2f]\n",
      def_minPercLen_defsMapRead,
      def_minPercLen_defsMapRead
   );

   fprintf(
      (FILE *) outFILE,
      "      o mininum percent or query needed to map\n"
   );

   fprintf(
      (FILE *) outFILE,
      "        refference (TODO: add in)\n"
   );

   /*****************************************************\
   * Fun02 Sec02 Sub06:
   *   - help message and version number
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
   ^   - output entry
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   fprintf(
      (FILE *) outFILE,
      "Output:\n"
   );

   fprintf(
      (FILE *) outFILE,
      "  - prints aligned (mapped) reads to -out\n"
   );
} /*phelp_mainMapRead*/

/*-------------------------------------------------------\
| Fun03: phelp_mainMapRead
|   - gets user input for mapRead
| Input:
|   - numArgsSI:
|     o number arugments user input
|   - argAryStr:
|     o c-string array with user input
|   - qryFileStrPtr:
|     o str_ptrAry struct to hold fastq files, will need
|       to free
|   - qryFlagSCPtr:
|     o signed char pointer to hold file type, will need
|       to free
|       * def_fqQry_mainMapRead if fastq file
|       * def_fqQry_mainMapRead | def_gzQry_mainMapRead if
|         fastq.gz file
|       * def_faQry_mainMapRead if fasta file
|       * def_faQry_mainMapRead | def_gzQry_mainMapRead if
|         fasta.gz file
|   - refFileStrPtr:
|     o c-string pointer to get ref file (do not free)
|   - outFileStrPtr:
|     o c-string pointer to get output file (do not free)
|   - qckMapBlPtr:
|     o if doing quick or slower mapping
|       * 1: quicker mapping
|       * 0: slower mapping
|   - setSTPtr:
|     o set_mapRead pointer with settings to modify
| Output:
|   - Modifies:
|     o all input except numArgsSI and argAryStr to hold
|       user input
|   - Prints:
|     o error messages to stderr
|   - Returns:
|     o 0 for no errors
|     o 1 if requested help message or version number
|     o 2 if had error
\-------------------------------------------------------*/
signed char
input_mainMapRead(
   int numArgsSI,               /*number arguments input*/
   char *argAryStr[],           /*input argument array*/
   struct str_ptrAry *qryFileSTPtr, /*gets query files*/
   signed char **qryFlagSCPtr,  /*query file type flag*/
   signed long *sizeFlagSL,     /*size of qryFlagSCPtr*/
   signed char **refFileStrPtr, /*gets reference file*/
   signed char **outFileStrPtr, /*gets output file*/
   signed char *qckMapBlPtr,    /*1: do quick mapping*/
   struct set_mapRead *setSTPtr /*gets settings*/
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun03 TOC:
   '   - gets user input for mapRead
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

   signed char errSC = 0;
   signed char *tmpStr = 0;
   signed int tmpSI = 0;
   signed int siArg = 1;

   signed char ntArySC[] = {'A', 'T', 'G', 'C'};
   signed char numNtSC = 4;

   signed char twoAnonNtArySC[] =
      {'W', 'S', 'M', 'K', 'R', 'Y'};
   signed char numTwoAnonSC = 6;

   signed char threeAnonNtArySC[] = {'B', 'D', 'H', 'V'};
   signed char numThreeAnonNtSC = 4;

   signed char maskAnonNtArySC[] = {'N', 'X'};
   signed char numMaskAnonNtSC = 2;

   signed char allNtArySC[] =
      {'A', 'T', 'G', 'C', 'W', 'S', 'M', 'K',
       'R', 'Y', 'B', 'D', 'H', 'V', 'N', 'X'
      };
   signed char numAllNtSC = 16;

   signed int refNtSI = 0;
   signed int qryNtSI = 0;
  

   if(numArgsSI <= 1)
      goto phelp_fun03_sec03; /*nothing input*/


   if(! qryFileSTPtr->lenSL)
   { /*If: need more memory*/
      if(
         resize_str_ptrAry(
            qryFileSTPtr,
            15
         )
      ){ /*If: had memory error during resize*/
         fprintf(
            stderr,
            "memory error setting up input memory\n"
         ); 

         goto err_fun03_sec03;
      } /*If: had memory error during resize*/
   }  /*If: had memory error*/

   if(! *qryFlagSCPtr)
   { /*If: need memory*/
      *qryFlagSCPtr = malloc(16 * sizeof(signed char));

      if(! *qryFlagSCPtr)
      { /*If: had memory error*/
         fprintf(
            stderr,
            "memory error reading input\n"
         );

         goto err_fun03_sec03;
      } /*If: had memory error*/

      *sizeFlagSL = 15;
   } /*If: need memory*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec02:
   ^   - get input
   ^   o fun03 sec02 sub01:
   ^     - fastq file array memory allaction + start loop
   ^   o fun03 sec02 sub02:
   ^     - check for fastq files
   ^   o fun03 sec02 sub03:
   ^     - reference and output file
   ^   o fun03 sec02 sub04:
   ^     - kmerScan variables
   ^   o fun03 sec02 sub05:
   ^     - chaing and scoring variables
   ^   o fun03 sec02 sub06:
   ^     - help message
   ^   o fun03 sec02 sub07:
   ^     - version number
   ^   o fun03 sec02 sub08:
   ^     - unkown input case
   ^   o fun03 sec02 sub09:
   ^     - move to next arugment
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun03 Sec02 Sub01:
   *   - fastq file array memory allaction + start loop
   \*****************************************************/

   while(siArg < numArgsSI)
   { /*Loop: get user input*/

      if(qryFileSTPtr->lenSL >= qryFileSTPtr->sizeSL - 1)
      { /*If: need more memory*/
         if(! qryFileSTPtr->lenSL)
            qryFileSTPtr->lenSL = 4;

         if(
            resize_str_ptrAry(
               qryFileSTPtr,
               (qryFileSTPtr->lenSL << 1)
            )
         ){ /*If: had memory error during resize*/
            fprintf(
               stderr,
               "memory error adding fastq file %s\n",
               argAryStr[siArg]
            ); 

            goto err_fun03_sec03;
         } /*If: had memory error during resize*/
      }  /*If: had memory error*/


      if(qryFileSTPtr->lenSL >= *sizeFlagSL - 1)
      { /*If: need more memory*/
         tmpStr =
            realloc(
               *qryFlagSCPtr,
               (*sizeFlagSL << 1)
            );
         if(! tmpStr)
         { /*If: had memory error*/
            fprintf(
               stderr,
               "memory error adding fastq file %s flag\n",
               argAryStr[siArg]
            ); 

            goto err_fun03_sec03;
         } /*If: had memory error*/

         *qryFlagSCPtr = tmpStr;
         tmpStr = 0;
         (*sizeFlagSL) <<= 1;
      } /*If: need more memory*/

      /**************************************************\
      * Fun03 Sec02 Sub02:
      *   - check for fastq files
      *   o fun03 sec02 sub02 cat01:
      *     - find query file type
      *   o fun03 sec02 sub02 cat02:
      *     - add query (fastq/fasta) to input array
      \**************************************************/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun03 Sec02 Sub02 Cat01:
      +   - find query file type
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      if(
         ! eqlNull_ulCp(
            (signed char *) "-fq",
            (signed char *) argAryStr[siArg]
         )
      ){ /*If: fastq file input*/
         (*qryFlagSCPtr)[qryFileSTPtr->lenSL] =
             def_fqQry_mainMapRead;
         goto addQry_fun03_sec02_sub02_cat02;
      }  /*If: fastq file input*/

      else if(
         ! eqlNull_ulCp(
            (signed char *) "-fq-gz",
            (signed char *) argAryStr[siArg]
         )
      ){ /*else If: fastq.gz file input*/
         (*qryFlagSCPtr)[qryFileSTPtr->lenSL] =
             def_fqQry_mainMapRead;
         (*qryFlagSCPtr)[qryFileSTPtr->lenSL] =
            def_gzQry_mainMapRead;
         goto addQry_fun03_sec02_sub02_cat02;
      }  /*Else If: fastq.gz file input*/

      else if(
         ! eqlNull_ulCp(
            (signed char *) "-fa",
            (signed char *) argAryStr[siArg]
         )
      ){ /*else If: fasta file input*/
         (*qryFlagSCPtr)[qryFileSTPtr->lenSL] =
             def_faQry_mainMapRead;
         goto addQry_fun03_sec02_sub02_cat02;
      }  /*Else If: fasta file input*/

      else if(
         ! eqlNull_ulCp(
            (signed char *) "-fa-gz",
            (signed char *) argAryStr[siArg]
         )
      ){ /*else If: fasta.gz file input*/
         (*qryFlagSCPtr)[qryFileSTPtr->lenSL] =
             def_faQry_mainMapRead;
         (*qryFlagSCPtr)[qryFileSTPtr->lenSL] =
            def_gzQry_mainMapRead;
         goto addQry_fun03_sec02_sub02_cat02;
      }  /*Else If: fasta.gz file input*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun03 Sec02 Sub02 Cat02:
      +   - add query (fastq/fasta) to input array
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      else if(1 == 0)
      { /*Else If: add input file (goto jump only)*/
         addQry_fun03_sec02_sub02_cat02:;

         ++siArg;

         if(
            add_str_ptrAry(
               (signed char *) argAryStr[siArg],
               qryFileSTPtr,
               qryFileSTPtr->lenSL
            )
         ){ /*If: had memory error*/
            fprintf(
               stderr,
               "memory error adding fastq file %s\n",
               argAryStr[siArg]
            ); 

            goto err_fun03_sec03;
         }  /*If: had memory error*/
      } /*Else If: add input file (goto jump only)*/

      /**************************************************\
      * Fun03 Sec02 Sub03:
      *   - reference, output file, and mapping type
      \**************************************************/

      else if(
         ! eqlNull_ulCp(
            (signed char *) "-ref",
            (signed char *) argAryStr[siArg]
         )
      ){ /*Else If: reference fasta input*/
         ++siArg;
         *refFileStrPtr = (signed char *) argAryStr[siArg];
      }  /*Else If: reference fasta input*/


      else if(
         ! eqlNull_ulCp(
            (signed char *) "-out",
            (signed char *) argAryStr[siArg]
         )
      ){ /*Else If: output file input*/
         ++siArg;
         *outFileStrPtr = (signed char *) argAryStr[siArg];
      }  /*Else If: output file input*/

      else if(
         ! eqlNull_ulCp(
            (signed char *) "-quick",
            (signed char *) argAryStr[siArg]
         )
      ) *qckMapBlPtr = 1;

      else if(
         ! eqlNull_ulCp(
            (signed char *) "-slow",
            (signed char *) argAryStr[siArg]
         )
      ) *qckMapBlPtr = 0;

      /**************************************************\
      * Fun03 Sec02 Sub04:
      *   - kmerScan variables
      *   o fun03 sec02 sub04 cat01:
      *     - extra nucleotides in window (as %)
      *   o fun03 sec02 sub04 cat02:
      *     - window percent query shift
      *   o fun03 sec02 sub04 cat03:
      *     -kmer-val query length, kmer size, and % kmer
      \**************************************************/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun03 Sec02 Sub04 Cat01:
      +   - extra nucleotides in window (as %)
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      else if(
         ! eqlNull_ulCp(
            (signed char *) "-kmer-extra-nt",
            (signed char *) argAryStr[siArg]
         )
      ){  /*Else If: window percent nucleotide size*/
         ++siArg;
         tmpStr = (signed char *) argAryStr[siArg];
         tmpStr +=
            strToF_base10str(
              tmpStr,
              &setSTPtr->percExtraNtF
            );

         if(*tmpStr != '\0')
         { /*If: error*/
            fprintf(
               stderr,
               "-kmer-extra-nt %s; non-numeric\n",
               argAryStr[siArg]
            );

            goto err_fun03_sec03;
         } /*If: error*/
      }  /*Else If: window percent nucleotide size*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun03 Sec02 Sub04 Cat02:
      +   - window percent query shift
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      else if(
         ! eqlNull_ulCp(
            (signed char *) "-kmer-shift",
            (signed char *) argAryStr[siArg]
         )
      ){ /*Else If: window shift size*/
         ++siArg;
         tmpStr = (signed char *) argAryStr[siArg];
         tmpStr +=
            strToF_base10str(
              tmpStr,
              &setSTPtr->percShiftF
            );

         if(*tmpStr != '\0')
         { /*If: error*/
            fprintf(
               stderr,
               "-kmer-shift %s; non-numeric/to large\n",
               argAryStr[siArg]
            );

            goto err_fun03_sec03;
         } /*If: error*/
      }  /*Else If: window shift size*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun03 Sec02 Sub04 Cat03:
      +   -kmer-val query length, kmer size, and % kmer
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      else if(
         ! eqlNull_ulCp(
            (signed char *) "-kmer-val",
            (signed char *) argAryStr[siArg]
         )
      ){  /*Else If: query length settings*/

         if(setSTPtr->lenKmersUC >= 16)
         { /*If: overlfow*/
            fprintf(
               stderr,
               "-kmer-val can only support 16 values\n"
            );

            goto err_fun03_sec03;
         } /*If: overlfow*/


         ++siArg;
         tmpStr = (signed char *) argAryStr[siArg];


         tmpStr +=
            strToSI_base10str(
              tmpStr,
              &setSTPtr->lenArySI[setSTPtr->lenKmersUC]
            );

         if(*tmpStr != ',')
         { /*If: error*/
            fprintf(
               stderr,
               "-kmer-val %s; length is not valid\n",
               argAryStr[siArg]
            );

            goto err_fun03_sec03;
         } /*If: error*/


         ++tmpStr;
         tmpStr +=
            strToUC_base10str(
              tmpStr,
              &setSTPtr->kmerAryUC[setSTPtr->lenKmersUC]
            );

         if(*tmpStr != ',')
         { /*If: error*/
            fprintf(
               stderr,
               "-kmer-val %s; kmer size is not valid\n",
               argAryStr[siArg]
            );

            goto err_fun03_sec03;
         } /*If: error*/

         ++tmpStr;
         tmpStr +=
           strToF_base10str(
            tmpStr,
            &setSTPtr->minKmersAryF[setSTPtr->lenKmersUC]
           );

         if(*tmpStr != ',')
         { /*If: error*/
            fprintf(
               stderr,
               "-kmer-val %s min %% kmers is not valid\n",
               argAryStr[siArg]
            );


            goto err_fun03_sec03;
         } /*If: error*/

         ++tmpStr;
         tmpStr +=
           strToSI_base10str(
            tmpStr,
            &setSTPtr->chainLenArySI[setSTPtr->lenKmersUC]
           );

         if(*tmpStr != '\0')
         { /*If: error*/
            fprintf(
               stderr,
               "-kmer-val %s; min chain len not valid\n",
               argAryStr[siArg]
            );

            goto err_fun03_sec03;
         } /*If: error*/

         ++setSTPtr->lenKmersUC;
      }  /*Else If: query length settings*/

      /**************************************************\
      * Fun03 Sec02 Sub05:
      *   - chaing and scoring variables
      *   o fun03 sec02 sub05 cat01:
      *     - sub alignment boolean
      *   o fun03 sec02 sub05 Cat02:
      *     - match score
      *   o fun03 sec02 sub05 Cat03:
      *     - snp score
      *   o fun03 sec02 sub05 Cat04:
      *     - gap opening penalty
      *   o fun03 sec02 sub05 Cat05:
      *     - gap extension penalty
      *   o fun03 sec02 sub05 Cat06:
      *     - two anonymous bases score
      *   o fun03 sec02 sub05 Cat07:
      *     - three anonymous bases score
      *   o fun03 sec02 sub05 Cat08:
      *     - four anonymous bases score
      *   o fun03 sec02 sub05 Cat09:
      *     - min percent score
      *   o fun03 sec02 sub05 Cat10:
      *     - min percent length
      \**************************************************/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun03 Sec02 Sub05 Cat01:
      +   - sub-alignment boolean
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      else if(
         ! eqlNull_ulCp(
            (signed char *) "-sub-aln",
            (signed char *) argAryStr[siArg]
         )
      ) setSTPtr->subBl = 1;

      else if(
         ! eqlNull_ulCp(
            (signed char *) "-no-sub-aln",
            (signed char *) argAryStr[siArg]
         )
      ) setSTPtr->subBl = 0;

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun03 Sec02 Sub05 Cat02:
      +   - match score
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      else if(
         ! eqlNull_ulCp(
            (signed char *) "-match",
            (signed char *) argAryStr[siArg]
         )
      ){ /*Else If: match score*/
         ++siArg;
         tmpStr = (signed char *) argAryStr[siArg];
         tmpStr +=
            strToSI_base10str(
              tmpStr,
              &tmpSI
            );

         if(*tmpStr != '\0')
         { /*If: error*/
            fprintf(
               stderr,
               "-match %s; non-numeric or to large\n",
               argAryStr[siArg]
            );

            goto err_fun03_sec03;
         } /*If: error*/

         for(
            refNtSI = 0;
            refNtSI < numNtSC;
            ++refNtSI
         ){ /*Loop: add matches*/
            setMatch_alnSet(
               ntArySC[refNtSI],
               ntArySC[refNtSI],
               tmpSI,
               setSTPtr->alnSetST
            );
         }  /*Loop: add matches*/
      }  /*Else If: match score*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun03 Sec02 Sub05 Cat03:
      +   - snp score
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      else if(
         ! eqlNull_ulCp(
            (signed char *) "-snp",
            (signed char *) argAryStr[siArg]
         )
      ){ /*Else If: snp score*/
         ++siArg;
         tmpStr = (signed char *) argAryStr[siArg];
         tmpStr +=
            strToSI_base10str(
              tmpStr,
              &tmpSI
            );

         if(*tmpStr != '\0')
         { /*If: error*/
            fprintf(
               stderr,
               "-snp %s; non-numeric or to large\n",
               argAryStr[siArg]
            );

            goto err_fun03_sec03;
         } /*If: error*/

         for(
            refNtSI = 0;
            refNtSI < numAllNtSC;
            ++refNtSI
         ){ /*Loop: all ref*/
            for(
               qryNtSI = 0;
               qryNtSI < numAllNtSC;
               ++qryNtSI
            ){ /*Loop: add snps*/
               if(qryNtSI != refNtSI)
                  setMatch_alnSet(
                     allNtArySC[refNtSI],
                     allNtArySC[qryNtSI],
                     tmpSI,
                     setSTPtr->alnSetST
                  );
            }  /*Loop: add snps*/
         }  /*Loop: all ref*/
      }  /*Else If: snp score*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun03 Sec02 Sub05 Cat04:
      +   - gap opening penalty
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      else if(
         ! eqlNull_ulCp(
            (signed char *) "-gap-open",
            (signed char *) argAryStr[siArg]
         )
      ){ /*Else If: gap opening penalty*/
         ++siArg;
         tmpStr = (signed char *) argAryStr[siArg];
         tmpStr +=
            strToSI_base10str(
              tmpStr,
              &tmpSI
            );

         if(*tmpStr != '\0')
         { /*If: error*/
            fprintf(
               stderr,
               "-gap-open %s; non-numeric or to large\n",
               argAryStr[siArg]
            );

            goto err_fun03_sec03;
         } /*If: error*/

         changeGap_alnSet(
            setSTPtr->alnSetST,
            tmpSI,
            setSTPtr->alnSetST->extendSS
         );
      }  /*Else If: gap opening penalty*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun03 Sec02 Sub05 Cat05:
      +   - gap extension penalty
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      else if(
         ! eqlNull_ulCp(
            (signed char *) "-gap-extend",
            (signed char *) argAryStr[siArg]
         )
      ){ /*Else If: gap extension penalty*/
         ++siArg;
         tmpStr = (signed char *) argAryStr[siArg];
         tmpStr +=
            strToSI_base10str(
              tmpStr,
              &tmpSI
            );

         if(*tmpStr != '\0')
         { /*If: error*/
            fprintf(
               stderr,
               "-gap-extend %s non-numeric or to large\n",
               argAryStr[siArg]
            );

            goto err_fun03_sec03;
         } /*If: error*/

         changeGap_alnSet(
            setSTPtr->alnSetST,
            tmpSI,
            setSTPtr->alnSetST->gapSS
         );
      }  /*Else If: gap extension penalty*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun03 Sec02 Sub05 Cat06:
      +   - two anonyous bases score
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      else if(
         ! eqlNull_ulCp(
            (signed char *) "-two-anon",
            (signed char *) argAryStr[siArg]
         )
      ){  /*Else If: score for 2 base anonymous base*/
         ++siArg;
         tmpStr = (signed char *) argAryStr[siArg];
         tmpStr +=
            strToSI_base10str(
              tmpStr,
              &tmpSI
            );

         if(*tmpStr != '\0')
         { /*If: error*/
            fprintf(
               stderr,
               "-match %s; non-numeric or to large\n",
               argAryStr[siArg]
            );

            goto err_fun03_sec03;
         } /*If: error*/

         for(
            refNtSI = 0;
            refNtSI < numTwoAnonSC;
            ++refNtSI
         ){ /*Loop: add matches*/
            setMatch_alnSet(
               twoAnonNtArySC[refNtSI],
               twoAnonNtArySC[refNtSI],
               tmpSI,
               setSTPtr->alnSetST
            );
         }  /*Loop: add matches*/
      }  /*Else If: score for 2 base anonymous base*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun03 Sec02 Sub05 Cat07:
      +   - three anonyous bases score
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      else if(
         ! eqlNull_ulCp(
            (signed char *) "-three-anon",
            (signed char *) argAryStr[siArg]
         )
      ){  /*Else If: score for 3 base anonymous base*/
         ++siArg;
         tmpStr = (signed char *) argAryStr[siArg];
         tmpStr +=
            strToSI_base10str(
              tmpStr,
              &tmpSI
            );

         if(*tmpStr != '\0')
         { /*If: error*/
            fprintf(
               stderr,
               "-match %s; non-numeric or to large\n",
               argAryStr[siArg]
            );

            goto err_fun03_sec03;
         } /*If: error*/

         for(
            refNtSI = 0;
            refNtSI < numThreeAnonNtSC;
            ++refNtSI
         ){ /*Loop: add matches*/
            setMatch_alnSet(
               threeAnonNtArySC[refNtSI],
               threeAnonNtArySC[refNtSI],
               tmpSI,
               setSTPtr->alnSetST
            );
         }  /*Loop: add matches*/
      }  /*Else If: score for 3 base anonymous base*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun03 Sec02 Sub05 Cat08:
      +   - four anonyous bases score
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      else if(
         ! eqlNull_ulCp(
            (signed char *) "-four-anon",
            (signed char *) argAryStr[siArg]
         )
      ){  /*Else If: score for 4 base anonymous base*/
         ++siArg;
         tmpStr = (signed char *) argAryStr[siArg];
         tmpStr +=
            strToSI_base10str(
              tmpStr,
              &tmpSI
            );

         if(*tmpStr != '\0')
         { /*If: error*/
            fprintf(
               stderr,
               "-match %s; non-numeric or to large\n",
               argAryStr[siArg]
            );

            goto err_fun03_sec03;
         } /*If: error*/

         for(
            refNtSI = 0;
            refNtSI < numMaskAnonNtSC;
            ++refNtSI
         ){ /*Loop: add matches*/
            setMatch_alnSet(
               maskAnonNtArySC[refNtSI],
               maskAnonNtArySC[refNtSI],
               tmpSI,
               setSTPtr->alnSetST
            );
         }  /*Loop: add matches*/
      }  /*Else If: score for 4 base anonymous base*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun03 Sec02 Sub05 Cat09:
      +   - min percent score
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      else if(
         ! eqlNull_ulCp(
            (signed char *) "-min-perc",
            (signed char *) argAryStr[siArg]
         )
      ){  /*Else If: min percent score*/
         ++siArg;
         tmpStr = (signed char *) argAryStr[siArg];
         tmpStr +=
            strToF_base10str(
              tmpStr,
              &setSTPtr->minScoreF
            );

         if(*tmpStr != '\0')
         { /*If: error*/
            fprintf(
               stderr,
               "-min-perc %s; non-numeric or to large\n",
               argAryStr[siArg]
            );

            goto err_fun03_sec03;
         } /*If: error*/

         if(
               setSTPtr->minScoreF < 0
            || setSTPtr->minScoreF > 1
         ){ /*If: overflow*/
            fprintf(
               stderr,
               "-min-perc %s; must be between 0 and 1\n",
               argAryStr[siArg]
            );

            goto err_fun03_sec03;
         } /*If: overflow*/
      }  /*Else If: min percent score*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun03 Sec02 Sub05 Cat10:
      +   - min percent length
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      else if(
         ! eqlNull_ulCp(
            (signed char *) "-min-len-perc",
            (signed char *) argAryStr[siArg]
         )
      ){  /*Else If: min percent lenth*/
         ++siArg;
         tmpStr = (signed char *) argAryStr[siArg];
         tmpStr +=
            strToF_base10str(
              tmpStr,
              &setSTPtr->minPercLenF
            );

         if(*tmpStr != '\0')
         { /*If: error*/
            fprintf(
               stderr,
               "-min-perc-len %s non-numeric/to large\n",
               argAryStr[siArg]
            );

            goto err_fun03_sec03;
         } /*If: error*/

         if(
               setSTPtr->minScoreF < 0
            || setSTPtr->minScoreF > 1
         ){ /*If: overflow*/
            fprintf(
               stderr,
               "-min-perc-len %s; not between 0 and 1\n",
               argAryStr[siArg]
            );

            goto err_fun03_sec03;
         } /*If: overflow*/
      }  /*Else If: min percent length*/

      /**************************************************\
      * Fun03 Sec02 Sub06:
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
      * Fun03 Sec02 Sub07:
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

      else if( ! eqlNull_ulCp(
            (signed char *) "version",
            (signed char *) argAryStr[siArg]
         )
      ) goto pversion_fun03_sec03;

      else if( ! eqlNull_ulCp(
            (signed char *) "-version",
            (signed char *) argAryStr[siArg]
         )
      ) goto pversion_fun03_sec03;

      else if( ! eqlNull_ulCp(
            (signed char *) "--version",
            (signed char *) argAryStr[siArg]
         )
      ) goto pversion_fun03_sec03;

      /**************************************************\
      * Fun03 Sec02 Sub08:
      *   - unkown input case
      \**************************************************/

      else
      { /*Else: unkown input*/
         fprintf(
            stderr,
            "%s is not recognized\n",
            argAryStr[siArg]
         );

         goto err_fun03_sec03;
      } /*Else: unkown input*/

      /**************************************************\
      * Fun03 Sec02 Sub09:
      *   - move to next arugment
      \**************************************************/

      ++siArg;
   } /*Loop: get user input*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec03:
   ^   - return result
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   errSC = 0;
   goto ret_fun03_sec03;

   phelp_fun03_sec03:;
      errSC = 1;
      phelp_mainMapRead(stdout);
      goto ret_fun03_sec03;
   pversion_fun03_sec03:;
      errSC = 1;
      pversion_mainMapRead(stdout);
      goto ret_fun03_sec03;
   err_fun03_sec03:;
      errSC = 2;
      goto ret_fun03_sec03;

   ret_fun03_sec03:;
      return errSC;
} /*input_mainMapRead*/

/*-------------------------------------------------------\
| Main:
|   - driver function for mapRead
| Input:
|   - numArgsSI:
|     o number arugments user input
|   - argAryStr:
|     o c-string array with user input
| Output:
|   - Prints:
|     o mapped reads to -out argument (or stdout)
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
   '   - driver function for mapRead
   '   o main sec01:
   '     - variable declarations
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   signed char errSC = 0;

   struct str_ptrAry qryFilesStackST; /*query files*/
   signed char *qryFlagHeapArySC = 0;/*query file types*/
   signed long sizeFlagSL = 0;       /*size of qry flags*/

   signed char *refFileStr = 0;
   signed char *outFileStr = 0;
   signed char qckMapBl = def_quick_mainMapRead;
   signed char lastSeqBl = 0;

   struct set_mapRead setStackST;

   signed long seqSL = 0;

   struct seqST qryStackST;
   struct seqST refStackST;

   struct samEntry samStackST;
   signed char *buffHeapStr = 0;
   signed long lenBuffSL = 0;

   FILE *seqFILE = 0;
   FILE *outFILE = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec02:
   ^   - initialize, get input, setup, and check input
   ^   o main sec02 sub01:
   ^     - initialize structs
   ^   o main sec02 sub02:
   ^     - get input
   ^   o main sec02 sub03:
   ^     - setup structs
   ^   o main sec02 sub04:
   ^     - open output file
	^   o main sec02 sub05:
   ^     - get reference sequence
   ^   o main sec02 sub06:
   ^     - open query file
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Main Sec02 Sub01:
   *   - initialize structs
   \*****************************************************/

   init_set_mapRead(&setStackST);

   init_seqST(&qryStackST);
   init_seqST(&refStackST);
   init_samEntry(&samStackST);

   init_str_ptrAry(&qryFilesStackST);

   /*this needs to be setup early to get alnSet struct*/
   if(
      setup_set_mapRead(
         &setStackST,
         7,           /*kmer size, updated later*/
         2000         /*reference length updated later*/
      )
   ){ /*If: memory error*/
      fprintf(
         stderr,
         "memory error with setup main sec02 sub02\n"
      );

      goto err_main_sec04;
   }  /*If: memory error*/

   /*****************************************************\
   * Main Sec02 Sub02:
   *   - get input
   \*****************************************************/

   errSC =
      input_mainMapRead(
         numArgsSI,
         argAryStr,
         &qryFilesStackST,   /*fastx files*/
         &qryFlagHeapArySC, /*flags for file types*/
         &sizeFlagSL,       /*flag array size*/
         &refFileStr,       /*reference file path*/
         &outFileStr,       /*output file path*/
         &qckMapBl,         /*quick or slow maping*/
         &setStackST        /*settings for mapping*/
      ); /*get user input*/

   if(errSC)
   { /*If: need to exit*/
      --errSC; /*convert help/version to no error*/
      goto ret_main_sec04; /*error value setup*/
   } /*If: need to exit*/

   /*****************************************************\
   * Main Sec02 Sub03:
   *   - setup structs
   \*****************************************************/

   if(setup_samEntry(&samStackST))
   { /*If: memory error*/
      fprintf(
         stderr,
         "memory error in setup step; main sec02 sub03\n"
      );

      goto err_main_sec04;
   } /*If: memory error*/

   /*****************************************************\
   * Main Sec02 Sub04:
   *   - open output file
   \*****************************************************/

   if(
         ! outFileStr
      || *outFileStr == '-'
   ){ /*If: user wanted stdout*/
       outFILE = stdout;
       outFileStr = (signed char *) '-';
   }  /*If: user wanted stdout*/

   else
   { /*Else: user input output file*/
      outFILE =
         fopen(
            (char *) outFileStr,
            "r"
         );

      if(! outFILE)
      { /*If: could not open output*/
         fprintf(
            stderr,
            "could not open -out %s\n",
            outFileStr
         );

         goto err_main_sec04;
      } /*If: could not open output*/
   } /*Else: user input output file*/

   /*****************************************************\
   * Main Sec02 Sub05:
   *   - get reference sequence
   *   o main sec02 sub05 cat01:
   *     - get reference sequence
   *   o main sec02 sub05 cat02:
   *     - check if had errors (reference sequence)
   \*****************************************************/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Main Sec02 Sub05 Cat01:
   +   - get reference sequence
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   seqFILE =
      fopen(
         (char *) refFileStr,
         "r"
      );

   if(! seqFILE)
   { /*If: could not open reference*/
      fprintf(
         stderr,
         "could not open -ref %s\n",
         refFileStr
      );

      goto err_main_sec04;
   } /*If: could not open reference*/

   /*currently setting this up for single reference,
   `  TODO: set up for multi-reference
   */

   errSC =
      getFaSeq_seqST(
         seqFILE,
         &refStackST
      ); /*get reference sequence*/ 

   fclose(seqFILE);
   seqFILE = 0;
   seqToIndex_alnSet(refStackST.seqStr);

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Main Sec02 Sub05 Cat02:
   +   - check if had errors (reference sequence)
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   if(errSC)
   { /*If: had error*/

      if(errSC == def_EOF_seqST)
      { /*If: end of file*/

         if(refStackST.seqStr[0] == '\0')
         { /*If: no reference sequence*/
            fprintf(
               stderr,
               "no sequence in -ref %s\n",
               refFileStr
            );

            goto err_main_sec04;
         } /*If: no reference sequence*/
      } /*If: end of file*/

      else if(errSC == def_memErr_seqST)
      { /*Else If: memory error*/
         fprintf(
            stderr,
            "memory error reading -ref %s\n",
            refFileStr
         );

         goto err_main_sec04;
      } /*Else If: memory error*/

      else
      { /*Else If: file error*/
         fprintf(
            stderr,
            "file error reading -ref %s\n",
            refFileStr
         );

         goto err_main_sec04;
      } /*Else If: file error*/
   } /*If: had error*/

   /*****************************************************\
   * Main Sec02 Sub06:
   *   - open query file
   \*****************************************************/

   /*TODO: this is for single query file support, add in
   `   mutli-file support later, also in fun03, add in
   `   multi-file input by one flag
   `  for testing, I am keep it simple
   */

   if(
         ! qryFilesStackST.lenSL
      || qryFilesStackST.strAry[0][0] == '-'
   ) seqFILE = stdin;

   else
   { /*Else: user input a file*/
      seqFILE =
         fopen(
            (char *) qryFilesStackST.strAry[0],
            "r"
         );

      if(! seqFILE)
      { /*If: could not open query file*/
         fprintf(
            stderr,
            "could not open %s\n",
            qryFilesStackST.strAry[0]
         );
 
         goto err_main_sec04;
      } /*If: could not open query file*/
   } /*Else: user input a file*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec03:
   ^   - map reads
   ^   o main sec03 sub01:
   ^     - get first entry
   ^   o main sec03 sub02:
   ^     - deal with EOF errors (others caught after loop)
   ^   o main sec03 sub03:
   ^     - map reads
   ^   o main sec03 sub04:
   ^     - handel errors before cleanup
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Main Sec03 Sub01:
   *   - get first entry
   \*****************************************************/

   if(qryFlagHeapArySC[0] == def_fqQry_mainMapRead)
      errSC =
         getFqSeq_seqST(
            seqFILE,
            &qryStackST
         );
   else if(qryFlagHeapArySC[0] == def_faQry_mainMapRead)
      errSC =
         getFaSeq_seqST(
            seqFILE,
            &qryStackST
         );
   else
   { /*Else: unsupported flag*/
      fprintf(
         stderr,
         ".gz currently not supported (future plans)\n"
      );
      goto err_main_sec04;
   } /*Else: unsupported flag*/

   /*****************************************************\
   * Main Sec03 Sub02:
   *   - deal with EOF errors (others caught after loop)
   \*****************************************************/

   if(errSC == def_EOF_seqST)
   { /*If: end of file*/
      if(qryStackST.seqStr[0] != '\0')
      { /*If: one sequence in file*/
         lastSeqBl = 1;
         errSC = 0;
         goto mapRead_main_sec03_sub02_cat01;
      } /*If: one sequence in file*/

      else
      { /*Else: nothing in file*/
         fprintf(
            stderr,
            "query %s has no seqeunce\n",
            qryFilesStackST.strAry[0]
         );

         goto err_main_sec04;
      } /*Else: nothing in file*/
   } /*If: end of file*/

   /*****************************************************\
   * Main Sec03 Sub03:
   *   - map reads
   *   o main sec03 sub03 cat01:
   *     - map read
	*   o main sec03 sub03 cat02:
   *     - print read
   \*****************************************************/
   
   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Main Sec03 Sub03 Cat01:
   +   - map read
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   while(! errSC)
   { /*Loop: map reads*/
      mapRead_main_sec03_sub02_cat01:;

      seqToIndex_alnSet(qryStackST.seqStr);
        /*for alignment and scoring steps*/ 

      ++seqSL;

      /*TODO add slower mapping step, want to debug
      `  faster first and modify return
      `  if(! qckMapBl)
      */

      testAln_mapRead(
         &samStackST, /*gets alignment*/
         &qryStackST, /*read to map*/
         &refStackST, /*reference*/
         &setStackST, /*settings*/
         &errSC       /*gets error value*/
      );

      if(errSC == def_memErr_mapRead)
      { /*If: memory error*/
         fprintf(
            stderr,
            "memory error on sequence %li in %s\n",
            seqSL,
            qryFilesStackST.strAry[0]
         );

         goto err_main_sec04;
      } /*If: memory error*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Main Sec03 Sub03 Cat02:
      +   - print read
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      else if(! errSC)
      { /*Else If: mapped read*/ 
         errSC =
            p_samEntry(
               &samStackST,
               &buffHeapStr,
               (unsigned long *) &lenBuffSL,
               0, /*want newline*/
               outFILE
            );

         if(errSC)
         { /*If: memory error*/
            fprintf(
               stderr,
               "memory error printing seq %li in %s\n",
               seqSL,
               qryFilesStackST.strAry[0]
            );

            goto err_main_sec04;
         } /*If: memory error*/
      } /*Else If: mapped read*/ 

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Main Sec03 Sub03 Cat03:
      +   - get next read
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      if(qryFlagHeapArySC[0] == def_fqQry_mainMapRead)
         errSC =
            getFqSeq_seqST(
               seqFILE,
               &qryStackST
            );

      else if(
         qryFlagHeapArySC[0] == def_faQry_mainMapRead
      ) errSC =
            getFaSeq_seqST(
               seqFILE,
               &qryStackST
            );
   } /*Loop: map reads*/


   /*****************************************************\
   * Main Sec03 Sub04:
   *   - handel errors before cleanup
   \*****************************************************/

   if(errSC == def_EOF_seqST)
   { /*If: end of file*/
      if(
            qryStackST.seqStr[0] != '\0'
         && ! lastSeqBl
      ){ /*If: on last sequence*/
         lastSeqBl = 1;
         goto mapRead_main_sec03_sub02_cat01;
      }  /*If: on last sequence*/
   } /*If: end of file*/

   else if(errSC == def_memErr_seqST)
   { /*Else If: memory error*/
      fprintf(
         stderr,
         "memory error on sequence %li in %s\n",
         seqSL,
         qryFilesStackST.strAry[0]
      );
   } /*Else If: memory error*/
  
   else if(errSC)
   { /*Else If: file error*/
      fprintf(
         stderr,
         "file error on sequence %li in %s\n",
         seqSL,
         qryFilesStackST.strAry[0]
      );
   } /*Else If: file error*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec04:
   ^   - clean up and return
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   errSC = 0;
   goto ret_main_sec04;

   err_main_sec04:;
      errSC = 1;
      goto ret_main_sec04;

   ret_main_sec04:;
      freeStack_set_mapRead(&setStackST);

      freeStack_seqST(&qryStackST);
      freeStack_seqST(&refStackST);
      freeStack_samEntry(&samStackST);

      freeStack_str_ptrAry(&qryFilesStackST);

      if(qryFlagHeapArySC)
         free(qryFlagHeapArySC);
      qryFlagHeapArySC = 0;

      if(buffHeapStr)
         free(buffHeapStr);
      buffHeapStr = 0;

      if(
            seqFILE
         && seqFILE != stdin
         && seqFILE != stdout
         && seqFILE != stderr
      ) fclose(seqFILE);
      seqFILE = 0;

      if(
            outFILE
         && outFILE != stdin
         && outFILE != stdout
         && outFILE != stderr
      ) fclose(outFILE);
      outFILE = 0;

      return errSC;
} /*main*/
