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
'   o main:
'     - driver function for mapRead
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
#include "../genLib/inflate.h"

#include "../genBio/seqST.h"
#include "../genBio/gzSeqST.h"
#include "../genBio/samEntry.h"
#include "../genBio/rmHomo.h"

#include "../genAln/alnSet.h"
#include "../genAln/mapRead.h"

/*only .h files*/
#include "../bioTools.h"
#include "../genLib/endLine.h"
#include "../genAln/defsMapRead.h"
#include "../genAln/alnDefs.h"

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\
! Hidden libraries
!   - .c  #include "../genLib/base10Str.h"
!   - .c  #include "../genLib/numToStr.h"
!   - .c  #include "../genLib/charCp.h"
!   - .c  #include "../genLib/genMath.h"
!   - .c  #include "../genLib/shellSort.h"
!   - .c  #include "../genLib/endin.h"
!   - .c  #include "../genLib/checkSum.h"
!   - .c  #include "../genLib/fileFun.h"
!   - .c  #include "../genBio/kmerFun.h"
!   - .c  #include "../genAln/indexToCoord.h"
!   - .c  #include "../genAln/dirMatrix.h"
!   - .c  #include "../genAln/needle.h"
!   - .h  #include "../genBio/ntTo5Bit.h"
!   - .h  #include "../genBio/kmerBit.h"
\%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

#define def_fqQry_mainMapRead 0
#define def_gzQry_mainMapRead 1
#define def_faQry_mainMapRead 2

#define def_refInIndex_mainMapRead 0
#define def_refInFa_mainMapRead 1
#define def_refOutIndex_mainMapRead 2

#define def_quick_mainMapRead 1 /*1: quick method*/

#define def_leftAln_mainMapRead 1
   /*do left alignments on reads*/

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
      "mapRead from bioTools version: %i-%02i-%02i%s",
      def_year_bioTools,
      def_month_bioTools,
      def_day_bioTools,
      str_endLine
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
      "mapRead -ref ref.fa read_1.fq read_2.fq ... > out.sam%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "  - maps reads to reference%s",
      str_endLine
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
   ^     - variable chaining variables (length,size,%)
   ^   o fun02 sec02 sub05:
   ^     - chaining and scoring variables
   ^   o fun02 sec02 sub06:
   ^     - help message and version number
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun02 Sec02 Sub01:
   *   - input block header
   \*****************************************************/

   fprintf(
      (FILE *) outFILE,
      "Input:%s",
      str_endLine
   );

   /*****************************************************\
   * Fun02 Sec02 Sub02:
   *   - query input
   \*****************************************************/

   fprintf(
      (FILE *) outFILE,
      "  - read_1.fq read_2.fq ...: [one Required]%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "    o fasta, fastq, fasta.gz or fastq.gz files%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "      with reads to map to reference%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "    o must come at end of command%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "    o use `-` for stdin input%s",
      str_endLine
   );

   /*****************************************************\
   * Fun02 Sec02 Sub03:
   *   - reference input and output file
   *   o fun02 sec02 sub03 cat01:
   *     - reference fasta input file
   *   o fun02 sec02 sub03 cat02:
   *     - index reference input file
   *   o fun02 sec02 sub03 cat03:
   *     - index reference
   *   o fun02 sec02 sub03 cat04:
   *     - output file
   \*****************************************************/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun02 Sec02 Sub03 Cat01:
   +   - reference fasta input file
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "  File IO (non-query):%s",
      str_endLine
   );


   fprintf(
      (FILE *) outFILE,
      "    -ref reference.fa: [Required]%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "      o fasta file with reference(s) to map%s",
      str_endLine
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun02 Sec02 Sub03 Cat02:
   +   - index reference input file
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "    -ref-index reference.index: [Replaces -ref]%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "      o reference file output by -index%s",
      str_endLine
   );


   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun02 Sec02 Sub02 Cat03:
   +   - index reference
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "    -index reference.fasta: [Disabled]%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "      o print reference.fasta index to -out%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "      o no alignment is done, so -fq is ignored%s",
      str_endLine
   );


   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun02 Sec02 Sub03 Cat04:
   +   - output file
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "    -out file.sam: [Default: stdout]%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "      o file to save mapped reads to (sam file)%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "      o use \"-out -\" for stdin input%s",
      str_endLine
   );

   /*****************************************************\
   * Fun02 Sec02 Sub04:
   *   - variable chaining variables (length,size,%)
   \*****************************************************/

   fprintf(
      (FILE *) outFILE,
      "  kmer/chaining variables:%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "    -kmer-val length,size,chain: [Optional]%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "      o kmer scan values to use based on query%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "        length (up to 16 values supported)%s",
      str_endLine
   );


   fprintf(
      (FILE *) outFILE,
      "        * length: min query length to use%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "        * size: kmer size (ex 7mer) to use%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "                needed to do an alignment%s",
      str_endLine
   );


   fprintf(
      (FILE *) outFILE,
      "        * chain: minimum kmers in chain needed%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "                 to keep a chain%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "      o Defaults:%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "        * -kmer-val %i,%i,%i%s",
      def_minLen_defsMapRead,
      def_minKmer_defsMapRead,
      def_minChain_defsMapRead,
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "        * -kmer-val %i,%i,%i%s",
      def_secLen_defsMapRead,
      def_secKmer_defsMapRead,
      def_secChain_defsMapRead,
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "        * -kmer-val %i,%i,%i%s",
      def_thirdLen_defsMapRead,
      def_thirdKmer_defsMapRead,
      def_thirdChain_defsMapRead,
      str_endLine
   );

   /*****************************************************\
   * Fun02 Sec02 Sub05:
   *   - chaining and scoring variables
   *   o fun02 sec02 sub05 cat01:
   *     - chain and score header
   *   o fun02 sec02 sub05 cat02:
   *     - align masked ends
   *   o fun02 sec02 sub05 cat03:
   *     - sub-alignment
   *   o fun02 sec02 sub05 cat04:
   *     - max percent length to merge chains
   *   o fun02 sec02 sub05 cat05:
   *     - match score
   *   o fun02 sec02 sub05 cat06:
   *     - snp score
   *   o fun02 sec02 sub05 cat07:
   *     - gap open score
   *   o fun02 sec02 sub05 cat08:
   *     - gap extend score
   *   o fun02 sec02 sub05 cat09:
   *     - two anonymous base score
   *   o fun02 sec02 sub05 cat10:
   *     - three anonymous base score
   *   o fun02 sec02 sub05 cat11:
   *     - four anonymous base score
   *   o fun02 sec02 sub05 cat12:
   *     - min percent score
   *   o fun02 sec02 sub05 cat13:
   *     - min percent score
   *   o fun02 sec02 sub05 cat14:
   *     - min percent length
   *   o fun02 sec02 sub05 cat15:
   *     - chain mininum percent query bases
   *   o fun02 sec02 sub05 cat16:
   *     - left alingment for homopolymer indels
   \*****************************************************/
   
   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun02 Sec02 Sub05 Cat01:
   +   - chain and score header
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "  Chain and score:%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "    - alignment step (chaining & scoring step)%s",
      str_endLine
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun02 Sec02 Sub05 Cat02:
   +   - align masked ends
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   if(def_alnEnd_defsMapRead)
      fprintf(
         (FILE *) outFILE,
         "    -aln-ends: [Optional; Yes]%s",
         str_endLine
      );
   else
      fprintf(
         (FILE *) outFILE,
         "    -aln-ends: [Optional; No]%s",
         str_endLine
      );

   fprintf(
      (FILE *) outFILE,
      "      o align 1.5 * (min bases in chain) soft%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "        masked bases at alignment ends%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "      o disable with \"-no-aln-ends\"%s",
      str_endLine
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun02 Sec02 Sub05 Cat03:
   +   - sub-alignment
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   if(def_subAln_defsMapRead)
      fprintf(
         (FILE *) outFILE,
         "    -sub-aln: [Optional; Yes]%s",
         str_endLine
      );
   else
      fprintf(
         (FILE *) outFILE,
         "    -sub-aln: [Optional; No]%s",
         str_endLine
      );

   fprintf(
      (FILE *) outFILE,
      "      o during scoring step find and return the%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "        best sub-alignment%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "      o disable with \"-no-sub-aln\"%s",
      str_endLine
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun02 Sec02 Sub05 Cat04:
   +   - max percent length to merge chains
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "    -max-perc-len %0.2f: [Optional; %0.2f]%s",
      def_maxPercLen_defsMapRead,
      def_maxPercLen_defsMapRead,
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "      o maximun gap (as percent query lenth)%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "        allowed between chains; for chain merge%s",
      str_endLine
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun02 Sec02 Sub05 Cat05:
   +   - match score
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "    -match 5: [Optional; 5]%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "      o score for a single match%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "      o also match score for chain merging step%s",
      str_endLine
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun02 Sec02 Sub05 Cat06:
   +   - snp score
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "    -snp -4: [Optional; -4]%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "      o score for a single snp%s",
      str_endLine
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun02 Sec02 Sub05 Cat07:
   +   - gap open score
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "    -gap-open %i: [Optional; %i]%s",
      def_gapOpen_alnDefs,
      def_gapOpen_alnDefs,
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "      o score for starting a gap (indel)%s",
      str_endLine
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun02 Sec02 Sub05 Cat08:
   +   - gap extension score
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "    -gap-extend %i: [Optional; %i]%s",
      def_gapExtend_alnDefs,
      def_gapExtend_alnDefs,
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "      o score for extending a gap (indel)%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "    -chain-gap %i: [Optional; %i]%s",
      def_gapExtend_alnDefs,
      def_gapExtend_alnDefs,
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "      o score for extending chain past a gap%s",
      str_endLine
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun02 Sec02 Sub05 Cat09:
   +   - two anonymous base score
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "    -two-anon 1: [Optional; 1]%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "      o score for A,T,C,G matched to a W,S,M,K,%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "        R, or Y%s",
      str_endLine
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun02 Sec02 Sub05 Cat10:
   +   - three anonymous base score
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "    -three-anon -1: [Optional; -1]%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "      o score for A,T,C,G matched to a B,D,H,V%s",
      str_endLine
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun02 Sec02 Sub05 Cat11:
   +   - four anonymous base score
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "    -four-anon -2: [Optional; -2]%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "      o score for matching a base to a N%s",
      str_endLine
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun02 Sec02 Sub05 Cat13:
   +   - min percent score
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "    -min-perc-score %0.2f: [Optional; %0.2f]%s",
      def_minScore_defsMapRead,
      def_minScore_defsMapRead,
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "      o mininum percent score to keep aligment%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "    -min-perc-match %0.2f: [Optional; %0.2f]%s",
      def_minMatch_defsMapRead,
      def_minMatch_defsMapRead,
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "      o mininum percent matche to keep aligment%s",
      str_endLine
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun02 Sec02 Sub05 Cat14:
   +   - min percent length
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "    -min-len-perc %0.2f: [Optional; %0.2f]%s",
      def_minPercLen_defsMapRead,
      def_minPercLen_defsMapRead,
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "      o mininum percent or query needed to map%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "        reference%s",
      str_endLine
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun02 Sec02 Sub05 Cat15:
   +   - chain mininum percent query bases
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "    -min-chain-nt %0.2f: [Optional; %0.2f]%s",
      def_chainMinLen_defsMapRead,
      def_chainMinLen_defsMapRead,
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "      o mininum percent for query bases needed%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "        in merged chains to alignmen%s",
      str_endLine
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun02 Sec02 Sub05 Cat16:
   +   - left alingment for homopolymer indels
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   if(def_leftAln_mainMapRead)
      fprintf(
         (FILE *) outFILE,
         "    -left-aln: [Optional; Yes]%s",
         str_endLine
      );
   else
      fprintf(
         (FILE *) outFILE,
         "    -left-aln: [Optional; No]%s",
         str_endLine
      );

   fprintf(
      (FILE *) outFILE,
      "      o left align indels in homopolymers%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "      o disable with `-no-left-aln`%s",
      str_endLine
   );

   /*****************************************************\
   * Fun02 Sec02 Sub06:
   *   - help message and version number
   \*****************************************************/

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
   ^   - output entry
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   fprintf(
      (FILE *) outFILE,
      "Output:%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "  - prints aligned (mapped) reads to -out%s",
      str_endLine
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
|   - refFileStrPtr:
|     o c-string pointer to get ref file (do not free)
|   - refFlagSCPtr:
|     o marks if printing reference index, or if reading
|       index reference or fasta reference
|   - outFileStrPtr:
|     o c-string pointer to get output file (do not free)
|   - qckMapBlPtr:
|     o if doing quick or slower mapping
|       * 1: quicker mapping
|       * 0: slower mapping
|   - fxPosSIPtr:
|     o signed int pointer to get index of first fastx
|       file in arguments
|   - leftAlnBlPtr:
|     o signed char pointer to be set to 1 if doing left
|       homopolymer indel alignment or 0 if not
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
   signed char **refFileStrPtr, /*gets reference file*/
   signed char *refFlagSCPtr,   /*reference file type*/
   signed char **outFileStrPtr, /*gets output file*/
   signed int *fxPosSIPtr,      /*fastx file position*/
   signed char *leftAlnBlPtr,   /*1: left alignment*/
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
  
   FILE *testFILE = 0;

   if(numArgsSI <= 1)
      goto phelp_fun03_sec03; /*nothing input*/
   *fxPosSIPtr = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec02:
   ^   - get input
   ^   o fun03 sec02 sub01:
   ^     - file IO by flags (reference and output)
   ^   o fun03 sec02 sub02:
   ^     - kmerScan variables
   ^   o fun03 sec02 sub03:
   ^     - chaing and scoring variables
   ^   o fun03 sec02 sub04:
   ^     - help message
   ^   o fun03 sec02 sub05:
   ^     - version number
   ^   o fun03 sec02 sub06:
   ^     - check if query fastx files
   ^   o fun03 sec02 sub07:
   ^     - unkown input case
   ^   o fun03 sec02 sub08:
   ^     - move to next arugment
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun03 Sec02 Sub01:
   *   - file IO by flags (reference and output)
   \*****************************************************/

   while(siArg < numArgsSI)
   { /*Loop: get user input*/

      if(
         ! eqlNull_ulCp(
            (signed char *) "-ref",
            (signed char *) argAryStr[siArg]
         )
      ){ /*Else If: reference fasta input*/
         if(*fxPosSIPtr)
            goto qryNotEndErr_fun03_sec03;

         ++siArg;
         *refFileStrPtr = (signed char *) argAryStr[siArg];
         *refFlagSCPtr = def_refInFa_mainMapRead;
      }  /*Else If: reference fasta input*/

      else if(
         ! eqlNull_ulCp(
            (signed char *) "-ref-index",
            (signed char *) argAryStr[siArg]
         )
      ){ /*Else If: reference index input*/
         if(*fxPosSIPtr)
            goto qryNotEndErr_fun03_sec03;

         ++siArg;
         *refFileStrPtr = (signed char *) argAryStr[siArg];
         *refFlagSCPtr = def_refInIndex_mainMapRead;
      }  /*Else If: reference index input*/

      else if(
         ! eqlNull_ulCp(
            (signed char *) "-index",
            (signed char *) argAryStr[siArg]
         )
      ){ /*Else If: want indexed referencde ouput*/
         if(*fxPosSIPtr)
            goto qryNotEndErr_fun03_sec03;

         ++siArg;
         *refFileStrPtr = (signed char *) argAryStr[siArg];
         *refFlagSCPtr = def_refOutIndex_mainMapRead;
         *refFlagSCPtr |= def_refInFa_mainMapRead;
      }  /*Else If: want indexed referencde ouput*/

      else if(
         ! eqlNull_ulCp(
            (signed char *) "-out",
            (signed char *) argAryStr[siArg]
         )
      ){ /*Else If: output file input*/
         if(*fxPosSIPtr)
            goto qryNotEndErr_fun03_sec03;

        ++siArg;
        *outFileStrPtr = (signed char *) argAryStr[siArg];
      }  /*Else If: output file input*/

      /**************************************************\
      * Fun03 Sec02 Sub02:
      *   - kmer-val query length, kmer size, and % kmer
      \**************************************************/

      else if(
         ! eqlNull_ulCp(
            (signed char *) "-kmer-val",
            (signed char *) argAryStr[siArg]
         )
      ){  /*Else If: query length settings*/
         if(*fxPosSIPtr)
            goto qryNotEndErr_fun03_sec03;

         if(setSTPtr->lenKmersUC >= 16)
         { /*If: overlfow*/
            fprintf(
               stderr,
               "-kmer-val can only support 16 values%s",
               str_endLine
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
               "-kmer-val %s; length is not valid%s",
               argAryStr[siArg],
               str_endLine
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
               "-kmer-val %s; kmer size is not valid%s",
               argAryStr[siArg],
               str_endLine
            );

            goto err_fun03_sec03;
         } /*If: error*/

         if(
            setSTPtr->kmerAryUC[setSTPtr->lenKmersUC] > 15
         ){ /*If: kmer length to large*/
            fprintf(
               stderr,
               "-kmer-val %s; kmer size > 15; to large%s",
               argAryStr[siArg],
               str_endLine
            );

            fprintf(
               stderr,
               "  use -kmer-val %i,15,%i%s",
               setSTPtr->lenArySI[setSTPtr->lenKmersUC],
               setSTPtr->kmerAryUC[setSTPtr->lenKmersUC]
                  - 15,
               str_endLine
            );
            goto err_fun03_sec03;
         }  /*If: kmer length to large*/
 

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
               "-kmer-val %s; min chain len not valid%s",
               argAryStr[siArg],
               str_endLine
            );

            goto err_fun03_sec03;
         } /*If: error*/

         if(
             setSTPtr->chainLenArySI[setSTPtr->lenKmersUC]
           <= 0
         ){ /*If: kmer length to large*/
            fprintf(
               stderr,
               "-kmer-val %s; min chain length < 1%s",
               argAryStr[siArg],
               str_endLine
            );
            goto err_fun03_sec03;
         }  /*If: kmer length to large*/

         ++setSTPtr->lenKmersUC;
      }  /*Else If: query length settings*/

      /**************************************************\
      * Fun03 Sec02 Sub03:
      *   - chaing and scoring variables
      *   o fun03 sec02 sub03 cat01:
      *     - align ends boolean
      *   o fun03 sec02 sub03 cat02:
      *     - sub alignment boolean
      *   o fun03 sec02 sub03 cat03:
      *     - max percent length to merge chains
      *   o fun03 sec02 sub03 cat04:
      *     - match score
      *   o fun03 sec02 sub03 cat05:
      *     - snp score
      *   o fun03 sec02 sub03 cat06:
      *     - gap opening penalty
      *   o fun03 sec02 sub03 cat07:
      *     - gap extension penalty
      *   o fun03 sec02 sub03 cat08:
      *     - two anonymous bases score
      *   o fun03 sec02 sub03 cat09:
      *     - three anonymous bases score
      *   o fun03 sec02 sub03 cat10:
      *     - four anonymous bases score
      *   o fun03 sec02 sub03 cat11:
      *     - min percent score
      *   o fun03 sec02 sub03 cat12:
      *     - min percent length
      \**************************************************/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun03 Sec02 Sub03 Cat01:
      +   - align ends boolean
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      else if(
         ! eqlNull_ulCp(
            (signed char *) "-aln-ends",
            (signed char *) argAryStr[siArg]
         )
      ){ /*Else If: alining ends*/
         if(*fxPosSIPtr)
            goto qryNotEndErr_fun03_sec03;
         setSTPtr->alnEndsBl = 1;
      }  /*Else If: alining ends*/

      else if(
         ! eqlNull_ulCp(
            (signed char *) "-no-aln-ends",
            (signed char *) argAryStr[siArg]
         )
      ){ /*Else If: not aligining ends*/
         if(*fxPosSIPtr)
            goto qryNotEndErr_fun03_sec03;
         setSTPtr->alnEndsBl = 0;
      }  /*Else If: not aligining ends*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun03 Sec02 Sub03 Cat02:
      +   - sub-alignment boolean
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      else if(
         ! eqlNull_ulCp(
            (signed char *) "-sub-aln",
            (signed char *) argAryStr[siArg]
         )
      ){ /*Else If: getting best sub-alignment*/
         if(*fxPosSIPtr)
            goto qryNotEndErr_fun03_sec03;
         setSTPtr->subBl = 1;
      }  /*Else If: getting best sub-alignment*/

      else if(
         ! eqlNull_ulCp(
            (signed char *) "-no-sub-aln",
            (signed char *) argAryStr[siArg]
         )
      ){ /*Else If: not getting best sub-alignment*/
         if(*fxPosSIPtr)
            goto qryNotEndErr_fun03_sec03;
         setSTPtr->subBl = 0;
      }  /*Else If: not getting best sub-alignment*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun03 Sec02 Sub03 Cat03:
      +   - max percent length to merge chains
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      else if(
         ! eqlNull_ulCp(
            (signed char *) "-max-perc-len",
            (signed char *) argAryStr[siArg]
         )
      ){ /*Else If: maximum percent length*/
         if(*fxPosSIPtr)
            goto qryNotEndErr_fun03_sec03;

         ++siArg;
         tmpStr = (signed char *) argAryStr[siArg];
         tmpStr +=
            strToF_base10str(
              tmpStr,
              &setSTPtr->maxLenPercF
            );

         if(*tmpStr != '\0')
         { /*If: error*/
            fprintf(
               stderr,
               "-max-per-len %s; non-numeric/to large%s",
               argAryStr[siArg],
               str_endLine
            );

            goto err_fun03_sec03;
         } /*If: error*/

         if(setSTPtr->maxLenPercF < 0)
         { /*If: out of bounds*/
            fprintf(
               stderr,
               "-max-per-len %s must be positive%s",
               argAryStr[siArg],
               str_endLine
            );

            goto err_fun03_sec03;
         }  /*If: out of bounds*/
 
      }  /*Else If: maximum percent length*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun03 Sec02 Sub03 Cat04:
      +   - match score
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      else if(
         ! eqlNull_ulCp(
            (signed char *) "-match",
            (signed char *) argAryStr[siArg]
         )
      ){ /*Else If: match score*/
         if(*fxPosSIPtr)
            goto qryNotEndErr_fun03_sec03;

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
               "-match %s; non-numeric or to large%s",
               argAryStr[siArg],
               str_endLine
            );

            goto err_fun03_sec03;
         } /*If: error*/

         setSTPtr->matchSI = tmpSI;

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
      + Fun03 Sec02 Sub03 Cat05:
      +   - snp score
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      else if(
         ! eqlNull_ulCp(
            (signed char *) "-snp",
            (signed char *) argAryStr[siArg]
         )
      ){ /*Else If: snp score*/
         if(*fxPosSIPtr)
            goto qryNotEndErr_fun03_sec03;

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
               "-snp %s; non-numeric or to large%s",
               argAryStr[siArg],
               str_endLine
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
      + Fun03 Sec02 Sub03 Cat06:
      +   - gap opening penalty
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      else if(
         ! eqlNull_ulCp(
            (signed char *) "-gap-open",
            (signed char *) argAryStr[siArg]
         )
      ){ /*Else If: gap opening penalty*/
         if(*fxPosSIPtr)
            goto qryNotEndErr_fun03_sec03;

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
               "-gap-open %s; non-numeric or to large%s",
               argAryStr[siArg],
               str_endLine
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
      + Fun03 Sec02 Sub03 Cat07:
      +   - gap extension penalty
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      else if(
         ! eqlNull_ulCp(
            (signed char *) "-gap-extend",
            (signed char *) argAryStr[siArg]
         )
      ){ /*Else If: gap extension penalty*/
         if(*fxPosSIPtr)
            goto qryNotEndErr_fun03_sec03;

         ++siArg;
         tmpStr = (signed char *) argAryStr[siArg];
         tmpStr += strToSI_base10str(tmpStr, &tmpSI);

         if(*tmpStr != '\0')
         { /*If: error*/
            fprintf(
               stderr,
               "-gap-extend %s non-numeric or to large%s",
               argAryStr[siArg],
               str_endLine
            );

            goto err_fun03_sec03;
         } /*If: error*/

         changeGap_alnSet(
            setSTPtr->alnSetST,
            tmpSI,
            setSTPtr->alnSetST->gapSS
         );
      }  /*Else If: gap extension penalty*/

      else if(
         ! eqlNull_ulCp(
            (signed char *) "-chain-gap",
            (signed char *) argAryStr[siArg]
         )
      ){ /*Else If: gap extension penalty*/
         if(*fxPosSIPtr)
            goto qryNotEndErr_fun03_sec03;

         ++siArg;
         tmpStr = (signed char *) argAryStr[siArg];
         tmpStr += strToSI_base10str(tmpStr, &tmpSI);

         if(*tmpStr != '\0')
         { /*If: error*/
            fprintf(
               stderr,
               "-gap-extend %s non-numeric or to large%s",
               argAryStr[siArg],
               str_endLine
            );

            goto err_fun03_sec03;
         } /*If: error*/

         setSTPtr->gapSI = tmpSI;
      }  /*Else If: gap extension penalty*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun03 Sec02 Sub03 Cat08:
      +   - two anonyous bases score
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      else if(
         ! eqlNull_ulCp(
            (signed char *) "-two-anon",
            (signed char *) argAryStr[siArg]
         )
      ){  /*Else If: score for 2 base anonymous base*/
         if(*fxPosSIPtr)
            goto qryNotEndErr_fun03_sec03;

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
               "-match %s; non-numeric or to large%s",
               argAryStr[siArg],
               str_endLine
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
      + Fun03 Sec02 Sub03 Cat09:
      +   - three anonyous bases score
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      else if(
         ! eqlNull_ulCp(
            (signed char *) "-three-anon",
            (signed char *) argAryStr[siArg]
         )
      ){  /*Else If: score for 3 base anonymous base*/
         if(*fxPosSIPtr)
            goto qryNotEndErr_fun03_sec03;

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
               "-match %s; non-numeric or to large%s",
               argAryStr[siArg],
               str_endLine
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
      + Fun03 Sec02 Sub03 Cat10:
      +   - four anonyous bases score
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      else if(
         ! eqlNull_ulCp(
            (signed char *) "-four-anon",
            (signed char *) argAryStr[siArg]
         )
      ){  /*Else If: score for 4 base anonymous base*/
         if(*fxPosSIPtr)
            goto qryNotEndErr_fun03_sec03;

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
               "-match %s; non-numeric or to large%s",
               argAryStr[siArg],
               str_endLine
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
      + Fun03 Sec02 Sub03 Cat11:
      +   - min percent score
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      else if(
         ! eqlNull_ulCp(
            (signed char *) "-min-perc-score",
            (signed char *) argAryStr[siArg]
         )
      ){  /*Else If: min percent score*/
         if(*fxPosSIPtr)
            goto qryNotEndErr_fun03_sec03;

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
               "-min-perc-score %s; non-num/to large%s",
               argAryStr[siArg],
               str_endLine
            );

            goto err_fun03_sec03;
         } /*If: error*/

         if(setSTPtr->minScoreF > 1)
         { /*If: overflow*/
            fprintf(
               stderr,
               "-min-perc-score %s; must be 1 or less%s",
               argAryStr[siArg],
               str_endLine
            );

            goto err_fun03_sec03;
         } /*If: overflow*/
      }  /*Else If: min percent score*/

      else if(
         ! eqlNull_ulCp(
            (signed char *) "-min-perc-match",
            (signed char *) argAryStr[siArg]
         )
      ){  /*Else If: min percent score*/
         ++siArg;
         tmpStr = (signed char *) argAryStr[siArg];
         tmpStr +=
            strToF_base10str(
              tmpStr,
              &setSTPtr->minMatchF
            );

         if(*tmpStr != '\0')
         { /*If: error*/
            fprintf(
               stderr,
               "-min-perc-match %s non-numeric/toLarge%s",
               argAryStr[siArg],
               str_endLine
            );

            goto err_fun03_sec03;
         } /*If: error*/

         if(
               setSTPtr->minMatchF < 0
            || setSTPtr->minMatchF > 1
         ){ /*If: overflow*/
            fprintf(
               stderr,
               "-min-perc-match %s; is < 0 or > 1%s",
               argAryStr[siArg],
               str_endLine
            );

            goto err_fun03_sec03;
         } /*If: overflow*/
      }  /*Else If: min percent score*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun03 Sec02 Sub03 Cat12:
      +   - min percent length
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      else if(
         ! eqlNull_ulCp(
            (signed char *) "-min-len-perc",
            (signed char *) argAryStr[siArg]
         )
      ){  /*Else If: min percent lenth*/
         if(*fxPosSIPtr)
            goto qryNotEndErr_fun03_sec03;

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
               "-min-perc-len %s non-numeric/to large%s",
               argAryStr[siArg],
               str_endLine
            );

            goto err_fun03_sec03;
         } /*If: error*/

         if(
               setSTPtr->minScoreF < 0
            || setSTPtr->minScoreF > 1
         ){ /*If: overflow*/
            fprintf(
               stderr,
               "-min-perc-len %s; not between 0 and 1%s",
               argAryStr[siArg],
               str_endLine
            );

            goto err_fun03_sec03;
         } /*If: overflow*/
      }  /*Else If: min percent length*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun03 Sec02 Sub03 Cat13:
      +   - chain minimum percent query bases
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      else if(
         ! eqlNull_ulCp(
            (signed char *) "-min-chain-nt",
            (signed char *) argAryStr[siArg]
         )
      ){  /*Else If: min percent lenth*/
         if(*fxPosSIPtr)
            goto qryNotEndErr_fun03_sec03;

         ++siArg;
         tmpStr = (signed char *) argAryStr[siArg];
         tmpStr +=
            strToF_base10str(
              tmpStr,
              &setSTPtr->chainMinNtF
            );

         if(*tmpStr != '\0')
         { /*If: error*/
            fprintf(
               stderr,
               "-min-chain-nt %s non-numeric/to large%s",
               argAryStr[siArg],
               str_endLine
            );

            goto err_fun03_sec03;
         } /*If: error*/

         if(
               setSTPtr->minScoreF < 0
            || setSTPtr->minScoreF > 1
         ){ /*If: overflow*/
            fprintf(
               stderr,
               "-min-chain-nt %s; not between 0 and 1%s",
               argAryStr[siArg],
               str_endLine
            );

            goto err_fun03_sec03;
         } /*If: overflow*/
      }  /*Else If: min percent length*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun03 Sec02 Sub03 Cat14:
      +   - left alignment for homopolymer indels
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      else if(
         ! eqlNull_ulCp(
            (signed char *) "-left-aln",
            (signed char *) argAryStr[siArg]
         )
      ) *leftAlnBlPtr = 1;

      else if(
         ! eqlNull_ulCp(
            (signed char *) "-no-left-aln",
            (signed char *) argAryStr[siArg]
         )
      ) *leftAlnBlPtr = 0;

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
      * Fun03 Sec02 Sub06:
      *   - check if query fastx files
      \**************************************************/

      else if(argAryStr[siArg][0] != '-')
      { /*Else If: likely query fastx files*/
         if(! *fxPosSIPtr)
            *fxPosSIPtr = siArg;

         if(argAryStr[siArg][0] == '>')
            goto done_fun03_sec03;
            /*at stdout redirect to file*/

         testFILE = fopen(argAryStr[siArg], "rb");

         if(! testFILE)
            goto invalidFile_fun03_sec03;
         fclose(testFILE);
         testFILE = 0;
      } /*Else If: likely query fastx files*/

      else if(! argAryStr[siArg][1])
      { /*Else If: standard input*/
         if(! *fxPosSIPtr)
            *fxPosSIPtr = siArg; /*stdin input*/
      } /*Else If: standard input*/

      /**************************************************\
      * Fun03 Sec02 Sub07:
      *   - unkown input case
      \**************************************************/

      else
      { /*Else: unkown input*/
         fprintf(
            stderr,
            "%s is not recognized%s",
            argAryStr[siArg],
            str_endLine
         );

         goto err_fun03_sec03;
      } /*Else: unkown input*/

      /**************************************************\
      * Fun03 Sec02 Sub08:
      *   - move to next arugment
      \**************************************************/

      ++siArg;
   } /*Loop: get user input*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec03:
   ^   - return result
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   done_fun03_sec03:;
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

   qryNotEndErr_fun03_sec03:;
      fprintf(
         stderr,
         "query fastx files must be at end of input%s",
         str_endLine
      );
      goto err_fun03_sec03;

   invalidFile_fun03_sec03:;
      fprintf(
         stderr,
         "could not open %s%s",
         argAryStr[siArg],
         str_endLine
      );
      goto err_fun03_sec03;

   err_fun03_sec03:;
      errSC = 2;
      goto ret_fun03_sec03;

   ret_fun03_sec03:;
      if(! testFILE) ;
      else if(testFILE == stdin) ;
      else if(testFILE == stdout) ;
      else if(testFILE == stderr) ;
      else
         fclose(testFILE);
      testFILE = 0;

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
   '   o main sec02:
   '     - initialize, get input, setup, and check input
   '   o main sec03:
   '     - print out user input settings
   '   o main sec04:
   '     - map reads to reference
   '   o main sec05:
   '     - clean up and return
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   signed char *tmpStr = 0;
   signed char errSC = 0;
   signed char forErrSC = 0;
   signed char revErrSC = 0;
   signed char leftAlnBl = def_leftAln_mainMapRead;

   signed int qryFileIndexSI = 0;    /*first query file*/

   signed char *refFileStr = 0;
   signed char refFlagSC = def_refInFa_mainMapRead;
   signed char *outFileStr = 0;

   signed long seqSL = 0;

   signed int chainSI = 0;
       /*chain used to get alignment*/
   struct seqST qryStackST;
      /*has query sequence*/
   struct ref_mapRead refStackST;
      /*reference sequence and reference kmer arrays*/
   struct set_mapRead setStackST;
      /*alignment settings*/
   struct aln_mapRead alnStackST;
      /*has arrays needed for alignment*/

   struct samEntry forSamStackST;
   struct samEntry revSamStackST;

   signed long forScoreSL = 0;
   signed long revScoreSL = 0;

   struct file_inflate inFileStackST;
   signed char fileTypeSC = 0;
   signed char stdinBl = 0;
   
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

   init_samEntry(&forSamStackST);
   init_samEntry(&revSamStackST);

   init_seqST(&qryStackST);
   init_ref_mapRead(&refStackST);
   init_aln_mapRead(&alnStackST);

   init_file_inflate(&inFileStackST);

   /*this needs to be setup early to get alnSet struct*/
   if(setup_set_mapRead(&setStackST) )
   { /*If: memory error*/
      fprintf(
         stderr,
         "memory error with setup main sec02 sub02%s",
         str_endLine
      );

      goto err_main_sec05;
   }  /*If: memory error*/

   /*****************************************************\
   * Main Sec02 Sub02:
   *   - get input
   \*****************************************************/

   errSC =
      input_mainMapRead(
         numArgsSI,
         argAryStr,
         &refFileStr,       /*reference file path*/
         &refFlagSC,        /*tells if indexed reference*/
         &outFileStr,       /*output file path*/
         &qryFileIndexSI,
         &leftAlnBl,
         &setStackST        /*settings for mapping*/
      ); /*get user input*/

   if(errSC)
   { /*If: need to exit*/
      --errSC; /*convert help/version to no error*/
      goto ret_main_sec05; /*error value setup*/
   } /*If: need to exit*/

   /*****************************************************\
   * Main Sec02 Sub03:
   *   - setup structs
   \*****************************************************/

   if( setup_aln_mapRead(&alnStackST) )
   { /*If: memory error setting up alignment strucutre*/
      fprintf(
         stderr,
         "memory error in setup step; main sec02 sub03%s",
         str_endLine
      );

      goto err_main_sec05;
   } /*If: memory error setting up alignment strucutre*/


   if( setup_samEntry(&forSamStackST) )
   { /*If: memory error setting up alignment strucutre*/
      fprintf(
         stderr,
         "memory error in setup step; main sec02 sub03%s",
         str_endLine
      );

      goto err_main_sec05;
   } /*If: memory error setting up alignment strucutre*/


   if( setup_samEntry(&revSamStackST) )
   { /*If: memory error setting up alignment strucutre*/
      fprintf(
         stderr,
         "memory error in setup step; main sec02 sub03%s",
         str_endLine
      );

      goto err_main_sec05;
   } /*If: memory error setting up alignment strucutre*/

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
      outFILE = fopen((char *) outFileStr, "w");

      if(! outFILE)
      { /*If: could not open output*/
         fprintf(
            stderr,
            "could not open -out %s%s",
            outFileStr,
            str_endLine
         );

         goto err_main_sec05;
      } /*If: could not open output*/
   } /*Else: user input output file*/

   /*****************************************************\
   * Main Sec02 Sub05:
   *   - get reference sequence
   *   o main sec02 sub05 cat01:
   *     - open reference file
   *   o main sec02 sub05 cat02:
   *     - get fasta reference sequence
   *   o main sec02 sub05 cat03:
   *     - get index reference sequence
   *   o main sec02 sub05 cat04:
   *     - if requested print reference index
   \*****************************************************/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Main Sec02 Sub05 Cat01:
   +   - open reference file
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   seqFILE = fopen((char *) refFileStr, "rb");

   if(! seqFILE)
   { /*If: could not open reference*/
      fprintf(
         stderr,
         "could not open -ref %s%s",
         refFileStr,
         str_endLine
      );

      goto err_main_sec05;
   } /*If: could not open reference*/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Main Sec02 Sub05 Cat02:
   +   - get fasta reference sequence
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   if(refFlagSC & def_refInFa_mainMapRead)
   { /*If: reading reference from fasta file*/
      errSC =
         getRef_ref_mapRead(
            &refStackST,
            &setStackST,
            0, /*1: is fastq file; 0: is fasta file*/
            0, /*1: is gzipped file; 0: is not-zipped*/
            seqFILE
         ); /*get reference sequence*/ 

      if(errSC)
      { /*If: had error*/

         if(errSC == def_EOF_mapRead)
         { /*If: end of file*/

            if(refStackST.seqSTPtr->seqStr[0] == '\0')
            { /*If: no reference sequence*/
               fprintf(
                  stderr,
                  "no sequence in -ref %s%s",
                  refFileStr,
                  str_endLine
               );

               goto err_main_sec05;
            } /*If: no reference sequence*/
         } /*If: end of file*/

         else if(errSC == def_memErr_mapRead)
         { /*Else If: memory error*/
            fprintf(
               stderr,
               "memory error reading -ref %s%s",
               refFileStr,
               str_endLine
            );

            goto err_main_sec05;
         } /*Else If: memory error*/

         /*I can ingore can def_noStruct_mapRead error
         `  because I know these structs are not null
         */

         else
         { /*Else: file error*/
            fprintf(
               stderr,
               "file error reading -ref %s%s",
               refFileStr,
               str_endLine
            );

            goto err_main_sec05;
         } /*Else: file error*/
      } /*If: had error*/
   } /*If: reading reference from fasta file*/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Main Sec02 Sub05 Cat03:
   +   - get index reference sequence
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   else
   { /*Else: getting reference from index file*/
      errSC =
         getRefBin_ref_mapRead(
            &refStackST,
            seqFILE
         );

      if(errSC == def_memErr_mapRead)
      { /*If: had memory error*/
         fprintf(
            stderr,
            "memory error reading -ref-index %s%s",
            refFileStr,
            str_endLine
         );

         goto err_main_sec05;
      } /*If: had memory error*/

      else if(errSC == def_badFile_mapRead)
      { /*If: had memory error*/
         fprintf(
            stderr,
            "file error reading -ref-index %s%s",
            refFileStr,
            str_endLine
         );

         goto err_main_sec05;
      } /*If: had memory error*/
   } /*Else: getting reference from index file*/

   fclose(seqFILE);
   seqFILE = 0;

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Main Sec02 Sub05 Cat04:
   +   - if requested print reference index
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   if(refFlagSC & def_refOutIndex_mainMapRead)
   { /*If: printing reference index to file*/
      writeRefBin_ref_mapRead(
        &refStackST,
        outFILE
      );
      goto noErr_main_sec05;
   } /*If: printing reference index to file*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec03:
   ^   - print out user input settings
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   fprintf(outFILE, "@HD\tVN:1.6\tSO:unsorted\tGO:query");
   fprintf(outFILE, "%s", str_endLine);

   tmpStr = refStackST.seqSTPtr->idStr;
   while(*tmpStr > 32)
      ++tmpStr;
   errSC = *tmpStr;
   *tmpStr = 0;

   fprintf(
      outFILE,
      "@SQ\tSN:%s\tLN:%li%s",
      refStackST.seqSTPtr->idStr,
      refStackST.seqSTPtr->seqLenSL + 1,
      str_endLine
   );
   *tmpStr = errSC;
   tmpStr = 0;

   fprintf(
      outFILE,
      "@PG\tID:mapRead\tVN:%i-%02i-%02i\tCL:mapRead",
      def_year_bioTools,
      def_month_bioTools,
      def_day_bioTools
   );

   for(seqSL = 1; seqSL < numArgsSI; ++seqSL)
      fprintf(outFILE, " %s", argAryStr[seqSL]);
   seqSL = 0;
   fprintf(outFILE, "%s", str_endLine);

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec04:
   ^   - map reads to reference
   ^   o main sec04 sub01:
   ^     - open query files
   ^   o main sec04 sub02:
   ^     - map reads in one file
   ^   o main sec04 sub04:
   ^     - handel errors before cleanup
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Main Sec04 Sub01:
   *   - open query files
   \*****************************************************/

   for( ; qryFileIndexSI < numArgsSI; ++qryFileIndexSI)
   { /*Loop: for each file; map reads*/
      if(argAryStr[qryFileIndexSI][0] == '-')
      { /*If: reading stdin input*/
         if(stdinBl)
            continue; /*already read stdin input*/
         else
            seqFILE = stdin;
      } /*If: reading stdin input*/

      else
      { /*Else: getting input from file*/
         seqFILE = fopen(argAryStr[qryFileIndexSI], "rb");

         if(! seqFILE)
            goto noFile_main_sec05;
      } /*Else: getting input from file*/
         

      errSC =
         get_gzSeqST(
            &inFileStackST,
            &fileTypeSC,
            &qryStackST,
            seqFILE
         );

      seqSL = 0;
      seqFILE = 0; /*this will get cleaned up by inflate*/

      if(errSC == def_EOF_seqST)
      { /*If: nothing in file*/
         fprintf(
            stderr,
            "no reads in %s%s",
            argAryStr[qryFileIndexSI],
            str_endLine
         );
         fflush(stderr);
         continue; /*nothing in file*/
      } /*If: nothing in file*/

      else if(errSC == def_memErr_seqST)
         goto memErr_main_sec05;

      else if(errSC)
         goto fileErr_main_sec05;

      else
      { /*Else: valid file*/
         fprintf(
            stderr,
            "mapping reads in %s%s",
            argAryStr[qryFileIndexSI],
            str_endLine
         );
         fflush(stderr);
      } /*Else: valid file*/

      /**************************************************\
      * Main Sec04 Sub02:
      *   - map reads in one file
      *   o main sec04 sub02 cat01:
      *     - map forward read + start loop
      *   o main sec04 sub02 cat02:
      *     - map reverse read
	   *   o main sec04 sub02 cat03:
      *     - print read
	   *   o main sec04 sub02 cat04:
      *     - get next read
      \**************************************************/
      
      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Main Sec04 Sub02 Cat01:
      +   - map forward read + start loop
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      while(! errSC)
      { /*Loop: map reads in one file*/
         ++seqSL;
         forScoreSL = 0;
         revScoreSL = 0;

         forScoreSL =
            align_mapRead(
               &qryStackST,   /*has read to map*/
               &refStackST,   /*has reference sequence*/
               &chainSI,      /*index of mapped chain*/
               &forSamStackST,/*gets alignments*/
               &alnStackST,   /*buffers for alignment*/
               &setStackST,   /*has alignment settings*/
               &forErrSC      /*gets error value*/
            );

         if(forErrSC == def_memErr_mapRead)
            goto memErr_main_sec05;

         /*++++++++++++++++++++++++++++++++++++++++++++++\
         + Main Sec04 Sub03 Cat02:
         +   - map reverse read
         \++++++++++++++++++++++++++++++++++++++++++++++*/

         revCmpIndex_alnSet(
            qryStackST.seqStr,
            qryStackST.qStr,
            qryStackST.seqLenSL
         );

         revScoreSL =
            align_mapRead(
               &qryStackST,   /*read to map*/
               &refStackST,   /*reference sequence*/
               &chainSI,      /*index of mapped chain*/
               &revSamStackST,/*gets alignments*/
               &alnStackST,   /*buffers for alignment*/
               &setStackST,   /*has alignment settings*/
               &revErrSC      /*gets error value*/
            );

         revSamStackST.flagUS = 16;

         if(revErrSC == def_memErr_mapRead)
            goto memErr_main_sec05;

         /*++++++++++++++++++++++++++++++++++++++++++++++\
         + Main Sec04 Sub03 Cat03:
         +   - print read
         \++++++++++++++++++++++++++++++++++++++++++++++*/

         errSC = 0;

         /*trying to avoid secondary alignments*/
         if(forScoreSL >= revScoreSL)
            revSamStackST.flagUS |= 256;
         else
            forSamStackST.flagUS |= 256;

         if(! forErrSC && forScoreSL > revScoreSL)
         { /*If: printing forward alignment*/
            if(leftAlnBl)
            { /*If: doing left alignment*/
               /*is an ineffecent way of doing this, but
               `  avoids this step in the interal read
               `  mapper
               */
               seqToIndex_alnSet(forSamStackST.seqStr);
               leftAlnIndel_rmHomo(
                  &forSamStackST,
                  refStackST.seqSTPtr->seqStr
               );
               indexToSeq_alnSet(forSamStackST.seqStr);
            } /*If: doing left alignment*/

            p_samEntry(&forSamStackST, 0, outFILE);
         } /*If: printing forward alignment*/

         else if(! revErrSC && revScoreSL > forScoreSL)
         { /*Else If: printing reverse alignment*/
            if(leftAlnBl)
            { /*If: doing left alignment*/
               /*is an ineffecent way of doing this, but
               `  avoids this step in the interal read
               `  mapper
               */
               seqToIndex_alnSet(revSamStackST.seqStr);
               leftAlnIndel_rmHomo(
                  &revSamStackST,
                  refStackST.seqSTPtr->seqStr
               );
               indexToSeq_alnSet(revSamStackST.seqStr);
            } /*If: doing left alignment*/

            p_samEntry(&revSamStackST, 0, outFILE);
         } /*Else If: printing reverse alignment*/

         /*++++++++++++++++++++++++++++++++++++++++++++++\
         + Main Sec04 Sub03 Cat04:
         +   - get next read
         \++++++++++++++++++++++++++++++++++++++++++++++*/

         errSC =
            get_gzSeqST(
               &inFileStackST,
               &fileTypeSC,
               &qryStackST,
               seqFILE
            );
      } /*Loop: map reads in one file*/

      /**************************************************\
      * Main Sec04 Sub04:
      *   - handel errors before cleanup
      \**************************************************/

      if(errSC == def_memErr_seqST)
         goto memErr_main_sec05;
      else if(errSC != def_EOF_seqST)
         goto fileErr_main_sec05;
   } /*Loop: for each file; map reads*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec05:
   ^   - clean up and return
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   goto noErr_main_sec05;

   noErr_main_sec05:;
      errSC = 0;
      goto ret_main_sec05;

   err_main_sec05:;
      errSC = 1;
      goto ret_main_sec05;

   memErr_main_sec05:;
      errSC = 2;
      if(seqSL)
         fprintf(
            stderr,
            "memory error while mapping read %li in %s%s",
            seqSL,
            argAryStr[qryFileIndexSI],
            str_endLine
         );
      else
         fprintf(
            stderr,
            "memory error setting up to map reads%s",
            str_endLine
         );
      goto ret_main_sec05;

   noFile_main_sec05:;
      errSC = 3;
      fprintf(
         stderr,
         "could not open file %s%s",
         argAryStr[qryFileIndexSI],
         str_endLine
      );
      goto ret_main_sec05;

   fileErr_main_sec05:;
      errSC = 4;
      fprintf(
         stderr,
         "%s is likely not a fasta or fastq file%s",
         argAryStr[qryFileIndexSI],
         str_endLine
      );
      fprintf(
         stderr,
         "problem is at line %li%s",
         seqSL,
         str_endLine
      );
      goto ret_main_sec05;

   ret_main_sec05:;
      freeStack_samEntry(&forSamStackST);
      freeStack_samEntry(&revSamStackST);
      freeStack_seqST(&qryStackST);
      freeStack_ref_mapRead(&refStackST);
      freeStack_set_mapRead(&setStackST);
      freeStack_aln_mapRead(&alnStackST);
      freeStack_file_inflate(&inFileStackST);

      if(! seqFILE) ;
      else if(seqFILE == stdin) ;
      else if(seqFILE == stdout) ;
      else if(seqFILE == stderr) ;
      else
         fclose(seqFILE);
      seqFILE = 0;

      if(! outFILE) ;
      else if(outFILE == stdin) ;
      else if(outFILE == stdout) ;
      else if(outFILE == stderr) ;
      else
         fclose(outFILE);
      outFILE = 0;

      return errSC;
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
: Copyright (c) 2025 jeremyButtler
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
