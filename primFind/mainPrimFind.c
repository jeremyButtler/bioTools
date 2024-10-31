/*-------------------------------------------------------\
' mainPrimFind SOF: Start Of File
'   - finds sequences with input primers
'   o header:
'     - included libraries
'   o main:
'     - driver function
'   o license:
'     - Licensing for this code (public domain / mit)
\-------------------------------------------------------*/

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
#include "../genLib/charCp.h"

#include "../genBio/seqST.h"

#include "../genAln/alnSet.h"
#include "../genAln/alnDefs.h"
#include "../genAln/kmerFind.h"

/*.h files only*/
#include "../genLib/dataTypeShortHand.h"
#include "../bioTools.h"

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\
! Hidden libraries:
!   - .c  #include "../genLib/ulCp.h"
!   - .c  #include "../genLib/shellSort.h"
!   - .c  #include "../genAln/indexToCoord.h"
!   - .c  #include "../genAln/alnDefs.h"
!   - .c  #include "../genAln/memwater.h"
!   - .h  #include "../genLib/genMath.h"
\%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

#define def_faPrimFile_mainPrimFind 1
#define def_tsvPrimFile_mainPrimFind 2

#define def_minLen_mainPrimFind 50
#define def_maxLen_mainPrimFind 0

#define def_fastSearch_mainPrimFind 1 /*1 = fast search*/

/*-------------------------------------------------------\
| Fun01: pversion_mainPrimFind
|   - prints the version for mainPrimFind to the input file
| Input:
|   - outFILE:
|     o file to print version number to
| Output:
|   - Prints;
|     o version number to outFILE
\-------------------------------------------------------*/
void
pversion_mainPrimFind(
   void *outFILE
){
   fprintf(
      (FILE *) outFILE,
      "primFind from bioTools verssion: %i-%02i-%02i\n",
       def_year_bioTools,
       def_month_bioTools,
       def_day_bioTools
   );
} /*pverson_mainPrimFind*/

/*-------------------------------------------------------\
| Fun02: phelp_mainPrimFind
|   - prints the help message for mainPrimFind to the input
|     file
| Input:
|   - outFILE:
|     o file to print the help message to
| Output:
|   - Prints;
|     o help message to outFILE
\-------------------------------------------------------*/
void
phelp_mainPrimFind(
   void *outFILE
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun02 TOC:
   '   - prints help message
   '   o fun02 sec01:
   '     - print out usage
   '   o fun02 sec02:
   '     - print out input
   '   o fun02 sec03:
   '     - print output block
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun02 Sec01:
   ^   - print out usage
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   fprintf(
      (FILE *) outFILE,
      "mainPrimFind -prim-tsv primers.tsv -fq reads.fasta\n"
   );

   fprintf(
      (FILE *) outFILE,
      "  - Finds reads with input primers\n"
   );

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun02 Sec02:
   ^   - print out input
   ^   o fun02 sec02 sub01:
   ^      - input block header
   ^   o fun02 sec02 sub02:
   ^     - file input
   ^   o fun02 sec02 sub03:
   ^     - filtering input
   ^   o fun02 sec02 sub04:
   ^     - kmer search options
   ^   o fun02 sec02 sub05:
   ^     - help message and version number options
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
   *   - file input
   *   o fun02 sec02 sub02 cat01:
   *     - primer tsv file input
   *   o fun02 sec02 sub02 cat02:
   *     - primer fasta file input
   *   o fun02 sec02 sub02 cat03:
   *     - read input
   \*****************************************************/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun02 Sec02 Sub02 Cat01:
   +   - primer tsv file input
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

    fprintf(
      (FILE *) outFILE,
       "  -prim-tsv primers.tsv: [Required]\n"
    );

    fprintf(
      (FILE *) outFILE,
       "    o tsv file with primers to search for\n"
    );

    fprintf(
      (FILE *) outFILE,
       "      - id\tpaired\tfor_seq\trev_seq\n"
    );

    fprintf(
      (FILE *) outFILE,
       "      - column 1: primer name\n"
    );

    fprintf(
      (FILE *) outFILE,
       "      - column 2: True = primers paired\n"
    );

    fprintf(
      (FILE *) outFILE,
       "        - both present in opposite directions\n"
    );

    fprintf(
      (FILE *) outFILE,
       "      - column 3: forward primer sequence or NA\n"
    );

    fprintf(
      (FILE *) outFILE,
       "      - column 4: reverse primer sequence or NA\n"
    );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun02 Sec02 Sub02 Cat02:
   +   - primer fasta file input
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

    fprintf(
      (FILE *) outFILE,
       "  -prim-fa primers.fasta: [Replaces -prim-tsv]\n"
    );

    fprintf(
      (FILE *) outFILE,
       "    o fasta file with primers to search for\n"
    );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun02 Sec02 Sub02 Cat03:
   +   - read input
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

    fprintf(
      (FILE *) outFILE,
       "  -fq reads.fastq: [Required]\n"
    );

    fprintf(
      (FILE *) outFILE,
       "    o fastq file with reads to search\n"
    );

    fprintf(
      (FILE *) outFILE,
       "  -fa reads.fasta: [Replaces -fq]\n"
    );

    fprintf(
      (FILE *) outFILE,
       "    o fasta file with reads to search\n"
    );

   /*****************************************************\
   * Fun02 Sec02 Sub03:
   *   - filtering input
   \*****************************************************/

    fprintf(
      (FILE *) outFILE,
       "  -min-len %i: [Optional]\n",
       def_minLen_mainPrimFind
    );

    fprintf(
      (FILE *) outFILE,
       "    o minimum read length to keep a read\n"
    );

    fprintf(
      (FILE *) outFILE,
       "  -max-len %i: [Optional]\n",
       def_maxLen_mainPrimFind
    );

    fprintf(
      (FILE *) outFILE,
       "    o maximum read length to keep a read\n"
    );

    fprintf(
      (FILE *) outFILE,
       "  -min-perc-score %0.2f: [Optional]\n",
       def_minPercScore_kmerFind
    );

    fprintf(
      (FILE *) outFILE,
       "    o minimum percent score (alignment) to\n"
    );

    fprintf(
      (FILE *) outFILE,
       "      count a primer as mapped\n"
    );

   /*****************************************************\
   * Fun02 Sec02 Sub04:
   *   - kmer search options
   *   o fun02 sec02 sub04 cat01:
   *     - fast/slow options
   *   o fun02 sec02 sub04 cat02:
   *     - kmer length options
   *   o fun02 sec02 sub04 cat03:
   *     - min percent kmers setting
   \*****************************************************/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun02 Sec02 Sub04 Cat01:
   +   - fast/slow options
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   if(def_fastSearch_mainPrimFind)
      fprintf(
        (FILE *) outFILE,
         "  -fast: [True]\n"
      );

   else
      fprintf(
        (FILE *) outFILE,
         "  -fast: [False]\n"
      );

    fprintf(
      (FILE *) outFILE,
       "    o do faster, but less percise kmer search\n"
    );

   fprintf(
     (FILE *) outFILE,
      "    o use \"-slow\" to search by Waterman\n"
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun02 Sec02 Sub04 Cat02:
   +   - kmer length options
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
     (FILE *) outFILE,
      "  -len-kmer %i: [Optional]\n",
      def_lenKmer_kmerFind
   );

   fprintf(
     (FILE *) outFILE,
      "    o kmer size for -fast; bigger is faster, but\n"
   );

   fprintf(
     (FILE *) outFILE,
      "      also less sensitive (misses more)\n"
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun02 Sec02 Sub04 Cat03:
   +   - min percent kmers setting
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
     (FILE *) outFILE,
      "  -min-perc-kmer %0.2f: [Optional]\n",
      def_minKmerPerc_kmerFind
   );

   fprintf(
     (FILE *) outFILE,
      "    o minimum percent of total kmers needed to\n"
   );

   fprintf(
     (FILE *) outFILE,
      "      do a Waterman on a window\n"
   );

   fprintf(
     (FILE *) outFILE,
      "    o higher is faster, but also less sensitive\n"
   );

   /*****************************************************\
   * Fun02 Sec02 Sub05:
   *   - help message and version number options
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
      "  - prints reads with detected primers to stdout\n"
   );

   fprintf(
      (FILE *) outFILE,
      "    o column 1 is read ID; see output for rest\n"
   );
} /*phelp_mainPrimFind*/

/*-------------------------------------------------------\
| Fun03: getInput_mainPrimFind
|   - gets user input for mainPrimFind
| Input:
|   - numArgsSI:
|     o number of arguments/parameters in argAryStr
|   - argAryStr:
|     o array of c-strings with user input
|   - primFileStr:
|     o pointer to c-string to point to the primer file
|       argument
|   - primTypeSC:
|     o pointer to char to hold the primer file type
|       (tsv or fasta)
|   - readFileStr:
|     o pointer to c-string to point to file with reads
|   - fqBl:
|     o ponter to char to be set to
|       - 1 = readFileStr is a fastq file
|       - 0 = readFileStr is a fasta file
|   - minLenUI:
|     o pointer to unsigned long to hold minimum primer
|       length
|   - maxlenUI:
|     o pointer to unsigned long to hold maximum primer
|       length
|   - minPerScoreF:
|     o pointer to float to hold the minimum percent score
|       to count a primer as mapped (found by waterman)
|   - fastBl:
|     o ponter to char to be set to
|       - 1 = use faster, but less senistive kmer method
|       - 0 = use slower, but more percise waterman method
|   - lenKmerUC:
|     o pointer to unsigned char to hold kmer length for
|       the faster kmer search
|   - minPercKmerF:
|     o pointer to float to hold min percentage of kmers
|       needed to do waterman alignment (kmer search only)
| Output:
|   - Modifies:
|     o all input, except numArgsSI and argAryStr to hold
|       the users input
|   - Prints:
|     o help message and version number to stdout if
|       requested
|     o prints any errors to stderr
|   - Returns:
|     o 0 for no errors
|     o 1 for printed help message or version number
|     o 2 for an error
\-------------------------------------------------------*/
signed char
getInput_mainPrimFind(
   int numArgsSI,
   char *argAryStr[],
   signed char **primFileStr,
   signed char *primTypeSC,
   signed char **readFileStr,
   signed char *fqBl,
   unsigned int *minLenUI,
   unsigned int *maxLenUI,
   float *minPercScoreF,
   signed char *fastBl,
   unsigned char *lenKmerUC,
   float *minPercKmerF
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun03 TOC:
   '   - gets user input for mainPrimFind
   '   o fun03 sec01:
   '     - variable declerations
   '   o fun03 sec01:
   '     - get input
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec01:
   ^   - variable declerations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   schar *tmpStr = 0;
   sint siArg = 1;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec02:
   ^   - get input
   ^   o fun03 sec02 sub01:
   ^     - check if have input and start loop
   ^   o fun03 sec02 sub02:
   ^     - file input (primer sequences and reads)
   ^   o fun03 sec02 sub03:
   ^     - filtering input (length/score)
   ^   o fun03 sec02 sub04:
   ^     - fast alignment input
   ^   o fun03 sec02 sub05:
   ^     - help messages
   ^   o fun03 sec02 sub06:
   ^     - check invalid input and move on
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun03 Sec02 Sub01:
   *   - check if have input and start loop
   \*****************************************************/

   if(numArgsSI < 2)
   { /*If: nothing was input; assume help message wanted*/
      phelp_mainPrimFind(stdout);
      return 1;
   } /*If: nothing was input; assume help message wanted*/

   while(siArg < numArgsSI)
   { /*Loop: get user input*/

      /**************************************************\
      * Fun03 Sec02 Sub02:
      *   - file input (primer sequences and reads)
      \**************************************************/

      if(
        ! eql_charCp(
            (schar *) "-prim-tsv",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*If: a tsv primer file was input*/
         ++siArg; /*move to argument*/
         *primTypeSC = def_tsvPrimFile_mainPrimFind;
         *primFileStr = (schar *) argAryStr[siArg];
      } /*If: a tsv primer file was input*/

      else if(
        ! eql_charCp(
            (schar *) "-prim-fa",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else If: a fasta primer file was input*/
         ++siArg; /*move to argument*/
         *primTypeSC = def_faPrimFile_mainPrimFind;
         *primFileStr = (schar *) argAryStr[siArg];
      } /*Else If: a fasta primer file was input*/

      else if(
        ! eql_charCp(
            (schar *) "-fa",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else If: a fasta sequence file was input*/
         ++siArg; /*move to argument*/
         *fqBl = 0;
         *readFileStr = (schar *) argAryStr[siArg];
      } /*Else If: a fasta sequence file was input*/

      else if(
        ! eql_charCp(
            (schar *) "-fq",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else If: a fastq sequence file was input*/
         ++siArg; /*move to argument*/
         *fqBl = 1;
         *readFileStr = (schar *) argAryStr[siArg];
      } /*Else If: a fastq sequence file was input*/

      /**************************************************\
      * Fun03 Sec02 Sub03:
      *   - filtering input (length/score)
      \**************************************************/

      else if(
        ! eql_charCp(
            (schar *) "-min-len",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else If: the min length was input*/
         ++siArg; /*move to argument*/
         tmpStr = (schar *) argAryStr[siArg];

         tmpStr +=
            strToUI_base10str(
               tmpStr,
               minLenUI
            );

         if(*tmpStr != '\0')
         { /*If: this is non-numeric*/
            fprintf(
               stderr,
               "-min-len %s is non-numeric\n",
               argAryStr[siArg]
            );

            return 2;
         } /*If: this is non-numeric*/
      } /*Else If: the min length was input*/

      else if(
        ! eql_charCp(
            (schar *) "-max-len",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else If: the max length was input*/
         ++siArg; /*move to argument*/
         tmpStr = (schar *) argAryStr[siArg];

         tmpStr +=
            strToUI_base10str(
               tmpStr,
               maxLenUI
            );

         if(*tmpStr != '\0')
         { /*If: this is non-numeric*/
            fprintf(
               stderr,
               "-max-len %s is non-numeric\n",
               argAryStr[siArg]
            );

            return 2;
         } /*If: this is non-numeric*/
      } /*Else If: the max length was input*/

      else if(
          ! eql_charCp(
               (schar *) "-min-perc-score",
               (schar *) argAryStr[siArg],
               (schar) '\0'
            )
      ){ /*Else If: the min percent score was input*/
         ++siArg; /*move to argument*/
         *minPercScoreF = atof(argAryStr[siArg]);

         if(*minPercScoreF == 0)
         { /*If: invalid input*/
            fprintf(
               stderr,
               "-min-perc-score %s is non-numeric or 0\n",
               argAryStr[siArg]
            );

            return 2;
         } /*If: invalid input*/
      } /*Else If: the min percent score was input*/

      /**************************************************\
      * Fun03 Sec02 Sub04:
      *   - fast alignment input
      \**************************************************/

      /*check if using a fast (kmer) or slow alignment*/
      else if(
        ! eql_charCp(
            (schar *)  "-fast",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ) *fastBl = 1;

      else if(
        ! eql_charCp(
            (schar *)  "-slow",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ) *fastBl = 0;

      else if(
         ! eql_charCp(
            (schar *) "-len-kmer",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else If: kmer length was input*/
         ++siArg; /*move to argument*/
         tmpStr = (schar *) argAryStr[siArg];

         tmpStr +=
            strToUC_base10str(
               tmpStr,
               lenKmerUC
            );

         if(*tmpStr != '\0')
         { /*If: this is non-numeric*/
            fprintf(
               stderr,
               "-len-kmer %s is non-numeric\n",
               argAryStr[siArg]
            );

            return 2;
         } /*If: this is non-numeric*/
      } /*Else If: kmer length was input*/

      else if(
          ! eql_charCp(
               (schar *) "-min-perc-kmer",
               (schar *) argAryStr[siArg],
               (schar) '\0'
            )
      ){ /*Else If: min percent kmers was input*/
         ++siArg; /*move to argument*/
         *minPercKmerF = atof(argAryStr[siArg]);

         if(*minPercScoreF == 0)
         { /*If: invalid input*/
            fprintf(
               stderr,
               "-min-perc-kmer %s is non-numeric or 0\n",
               argAryStr[siArg]
            );

            return 2;
         } /*If: invalid input*/
      } /*Else If: min percent kmers was input*/

      /**************************************************\
      * Fun03 Sec02 Sub05:
      *   - help and version messages
      \**************************************************/

      else if(
            ! eql_charCp(
               (schar *) "-h",
               (schar *) argAryStr[siArg],
               (schar) '\0'
            )
         || ! eql_charCp(
               (schar *) "--h",
               (schar *) argAryStr[siArg],
               (schar) '\0'
            )
         || ! eql_charCp(
               (schar *) "help",
               (schar *) argAryStr[siArg],
               (schar) '\0'
            )
         || ! eql_charCp(
               (schar *) "-help",
               (schar *) argAryStr[siArg],
               (schar) '\0'
            )
         || ! eql_charCp(
               (schar *) "--help",
               (schar *) argAryStr[siArg],
               (schar) '\0'
            )
      ){ /*Else If: help message*/
         phelp_mainPrimFind(stdout);
         return 1;
      } /*Else If: help message*/

      else if(
            ! eql_charCp(
               (schar *) "-v",
               (schar *) argAryStr[siArg],
               (schar) '\0'
            )
         || ! eql_charCp(
               (schar *) "--v",
               (schar *) argAryStr[siArg],
               (schar) '\0'
            )
         || ! eql_charCp(
               (schar *) "version",
               (schar *) argAryStr[siArg],
               (schar) '\0'
            )
         || ! eql_charCp(
               (schar *) "-version",
               (schar *) argAryStr[siArg],
               (schar) '\0'
            )
         || ! eql_charCp(
               (schar *) "--version",
               (schar *) argAryStr[siArg],
               (schar) '\0'
            )
      ){ /*Else If: help message*/
         pversion_mainPrimFind(stdout);
         return 1;
      } /*Else If: help message*/

      /**************************************************\
      * Fun03 Sec02 Sub06:
      *   - check invalid input and move on
      \**************************************************/

      else
      { /*Else: input not recognized*/
         fprintf(
            stderr,
            "%s is not recognized\n",
            argAryStr[siArg]
         );

         return 2;
      } /*Else: input not recognized*/

      ++siArg;
   } /*Loop: get user input*/

   return 0;
} /*getInput_mainPrimFind*/


/*-------------------------------------------------------\
| Main:
|   - main driver function for mainPrimFind
| Input:
|   - numArgsSI:
|     o number of arguments user input in argAryStr
|   - argAryStr:
|     o array of c-strings with user arguments
| Output:
|   - Prints:
|     o
\-------------------------------------------------------*/
int
main(
   int numArgsSI,
   char *argAryStr[]
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Main TOC:
   '   o main sec01:
   '     - variable declerations
   '   o main sec02:
   '     - get user input and initialize structures
   '   o main sec03:
   '     - read in references and allocate memory
   '   o main sec04:
   '     - find and print primer positions
   '   o main sec05:
   '     - clean up
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec01:
   ^   - variable declerations
   ^   o main sec01 sub01:
   ^     - output/input variables (one tempory)
   ^   o main sec01 sub02:
   ^     - variables for filtering or storing results
   ^   o main sec01 sub03:
   ^     - variables unique to kmer search
   ^   o main sec01 sub04:
   ^     - structure and file variables
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Main Sec01 Sub01:
   *   - output/input variables (one tempory)
   \*****************************************************/

   schar errSC = 0;      /*returned error*/

   schar *seqFileStr = 0; /*fasta/q (x) with sequences*/
   schar fqBl = 1;       /*1 is a fastq file; 0 is fasta*/

   schar *primFileStr = 0; /*file with primer sequences*/
   schar primFileTypeSC = def_tsvPrimFile_mainPrimFind;
      /*file type with primer sequences*/

   schar *outFileStr = 0; /*output file*/

   ulong entryOnUL = 0;
   schar *tmpStr = 0;

   /*****************************************************\
   * Main Sec01 Sub02:
   *   - variables for filtering or storing results
   \*****************************************************/

   /*variables for filtering*/
   uint minLenUI = def_minLen_mainPrimFind;
   uint maxLenUI = def_maxLen_mainPrimFind;

   /*variables for holding search output*/
   uint *codeHeapAryUI = 0; /*matchs/primer*/

   slong *scoreHeapArySL = 0;
   schar *dirHeapArySC = 0;

   ulong *seqStartHeapAryUL = 0;
   ulong *seqEndHeapAryUL = 0;

   ulong *primStartHeapAryUL = 0;
   ulong *primEndHeapAryUL = 0;

   /*****************************************************\
   * Main Sec01 Sub03:
   *   - variables unique to kmer search
   \*****************************************************/

   schar fastBl = def_fastSearch_mainPrimFind;
      /*1 = kmer search, 0 = waterman*/

   float minPercScoreF = def_minPercScore_kmerFind;
      /*min score to keep mapping*/

   uchar lenKmerUC = def_lenKmer_kmerFind; /*kmer length*/

   float frameShiftF = def_percShift_kmerFind;
      /*percentage of bases to move when shifting a win*/

   float minPercKmerF = def_minKmerPerc_kmerFind;
      /*minimum percentage of kmers to do a waterman*/

   float extraNtInWinF = def_extraNtInWin_kmerFind;
     /*number of extra (beyond primer) bases in window*/

   /*****************************************************\
   * Main Sec01 Sub04:
   *   - structure and file variables
   \*****************************************************/

   /*structures for primer search*/
   struct refST_kmerFind *refHeapAryST = 0;
   sint numRefsSI = 0; /*references in refHeapAryST*/

   struct tblST_kmerFind kmerTblStackST;
   struct seqST seqStackST;
   struct alnSet alnSetStackST;

   FILE *seqFILE = 0;
   FILE *outFILE = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec02:
   ^   - get user input and initialize structures
   ^   o main sec02 sub01:
   ^     - initialize structures
   ^   o main sec02 sub02:
   ^     - get input
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Main Sec02 Sub01:
   *   - initialize structures
   \*****************************************************/

   init_seqST(&seqStackST);
   init_alnSet(&alnSetStackST);
   init_tblST_kmerFind(&kmerTblStackST);

   alnSetStackST.gapSS = -4 * def_scoreAdj_alnDefs;
   alnSetStackST.extendSS = -1 * def_scoreAdj_alnDefs;

   /*****************************************************\
   * Main Sec02 Sub02:
   *   - get input
   \*****************************************************/

   errSC =
      getInput_mainPrimFind(
         numArgsSI,
         argAryStr,
         &primFileStr,
         &primFileTypeSC,
         &seqFileStr,
         &fqBl,
         &minLenUI,
         &maxLenUI,
         &minPercScoreF,
         &fastBl,
         &lenKmerUC,
         &minPercKmerF
      );

   if(errSC)
   { /*If: had error*/
      --errSC; /*remove help message error*/
      errSC <<= 5; /*avoid conflicts with other errors*/
      goto cleanUp_main_sec05_sub04;
   } /*If: had error*/


   errSC =
      setup_tblST_kmerFind(
         &kmerTblStackST,
         lenKmerUC
      );

   if(errSC)
   { /*If: memory error*/
      fprintf(
         stderr,
         "memory error setting up kmer find table\n"
      );

      goto memErr_main_sec05_sub02;  
   } /*If: memory error*/

   /*****************************************************\
   * Main Sec02 Sub03:
   *   - open files
   *   o main sec02 sub03 cat01:
   *     - open output file
   *   o main sec02 sub03 cat02:
   *     - open read file
   \*****************************************************/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Main Sec02 Sub03 Cat01:
   +   - open output file
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   if(!outFileStr || *outFileStr == '-')
      outFILE = stdout;

   else
   { /*Else: user supplied an output file*/
      outFILE =
         fopen(
            (char *) outFileStr,
            "w"
         );

      if(! outFILE)
      { /*If: could not open output file*/
         fprintf(
            stderr,
            "could not open -out %s\n",
            outFileStr
         );
      } /*If: could not open output file*/

      goto fileErr_main_sec05_sub03;
   } /*Else: user supplied an output file*/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Main Sec02 Sub03 Cat02:
   +   - open read file
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   if(! seqFileStr || *seqFileStr == '-')
      seqFILE = stdin;

   else
   { /*Else: user supplied an read file*/
      seqFILE =
         fopen(
            (char *) seqFileStr,
            "r"
         );

      if(! seqFILE)
      { /*If: could not open readFxput file*/
         if(fqBl)
            tmpStr = (schar *) "-fq";
         else
            tmpStr = (schar *) "-fa";

         fprintf(
            stderr,
            "could not open %s %s\n",
            tmpStr,
            seqFileStr
         );

         goto fileErr_main_sec05_sub03;
      } /*If: could not open readFxput file*/
   } /*Else: user supplied an read file*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec03:
   ^   - read in references and allocate memory
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Main Sec03 Sub02:
   *   - read in references
   \*****************************************************/

   if(primFileTypeSC == def_tsvPrimFile_mainPrimFind)
   { /*If: input was from tsv file*/
      refHeapAryST =
         tsvToAry_refST_kmerFind(
            primFileStr,
            lenKmerUC,
            &numRefsSI,
            minPercKmerF,
            &kmerTblStackST,
            extraNtInWinF,
            frameShiftF,
            &alnSetStackST,
            &errSC
         );
   } /*If: input was from tsv file*/

   else
   { /*Else: input was from fasta file*/
      refHeapAryST =
         faToAry_refST_kmerFind(
            primFileStr,
            lenKmerUC,
            &numRefsSI,
            minPercKmerF,
            &kmerTblStackST,
            extraNtInWinF,
            frameShiftF,
            &alnSetStackST,
            &errSC
         );
   } /*Else: input was from fasta file*/

   if(errSC)
   { /*If: I had an error*/
      if(errSC == def_memErr_kmerFind)
      { /*If: I had a memory error*/
         fprintf(
            stderr,
            "Memory error when reading primer sequences\n"
         );

         goto memErr_main_sec05_sub02;
      } /*If: I had a memory error*/

      if(errSC == def_fileErr_kmerFind)
      { /*If: had a file error*/
         fprintf(
            stderr,
            "Problem reading -prim %s\n",
            primFileStr
         );
      } /*If: had a file error*/

      goto fileErr_main_sec05_sub03;
   } /*If: I had an error*/

   /*****************************************************\
   * Main Sec03 Sub02:
   *   - allocate arrays for reference results
   \*****************************************************/

   dirHeapArySC = malloc(numRefsSI * sizeof(schar));

   if(!dirHeapArySC)
      goto memErr_main_sec05_sub02;
  
   codeHeapAryUI = malloc(numRefsSI * sizeof(uint));

   if(! codeHeapAryUI)
      goto memErr_main_sec05_sub02;

   scoreHeapArySL = malloc(numRefsSI * sizeof(slong));

   if(! scoreHeapArySL)
      goto memErr_main_sec05_sub02;

   seqStartHeapAryUL = malloc(numRefsSI * sizeof(ulong));

   if(! seqStartHeapAryUL)
      goto memErr_main_sec05_sub02;

   seqEndHeapAryUL = malloc(numRefsSI * sizeof(ulong));

   if(! seqEndHeapAryUL)
      goto memErr_main_sec05_sub02;

   primStartHeapAryUL = malloc(numRefsSI * sizeof(ulong));

   if(! primStartHeapAryUL)
      goto memErr_main_sec05_sub02;

   primEndHeapAryUL = malloc(numRefsSI * sizeof(ulong));

   if(! primEndHeapAryUL)
      goto memErr_main_sec05_sub02;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec04:
   ^   - find and print primer positions
   ^   o main sec04 sub01:
   ^     - read in first sequence and start loop
   ^   o main sec04 sub02:
   ^     - check if sequence passes filters
   ^   o main sec04 sub03:
   ^     - find and print primer positions
   ^   o main sec04 sub04:
   ^     - get the next sequence
   ^   o main sec04 sub05:
   ^     - check for errors
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Main Sec04 Sub01:
   *   - read in first sequence and start loop
   \*****************************************************/

   pHeaderHit_kmerFind(outFILE);

   if(fqBl)
   { /*If: i have a fastq file*/
      errSC =
         (schar)
         getFqSeq_seqST(
            seqFILE,
            &seqStackST
         );
   } /*If: i have a fastq file*/

   else
   { /*Else: i have a fasta file*/
      errSC =
         (schar)
         getFaSeq_seqST(
            seqFILE,
            &seqStackST
         );
   } /*Else: i have a fasta file*/

   while(! errSC)
   { /*Loop: find primers in sequences*/

      /**************************************************\
      * Main Sec04 Sub02:
      *   - check if sequence passes filters
      \**************************************************/

      ++entryOnUL;

      if(seqStackST.lenSeqUL < minLenUI)
         goto nextSeq_main_sec04_sub04;

      if(seqStackST.lenSeqUL > maxLenUI)
      { /*If: read is longer then max length*/
         if(maxLenUI > 0)
            goto nextSeq_main_sec04_sub04;
      } /*If: read is longer then max length*/

      /**************************************************\
      * Main Sec04 Sub03:
      *   - find and print primer positions
      \**************************************************/

      if(fastBl)
      { /*If: i am searching with kmers*/
         errSC =
            fxFindPrims_kmerFind(
               &kmerTblStackST,    /*table structure*/
               refHeapAryST,       /*primers looking for*/
               numRefsSI,          /*number of primers*/
               &seqStackST,        /*sequence to look at*/
               minPercScoreF,      /*min % for waterman*/
               codeHeapAryUI,      /*# times found prim*/
               dirHeapArySC,       /*direction of primer*/
               scoreHeapArySL,     /*scores for prims*/
               seqStartHeapAryUL,  /*1st align seq base*/
               seqEndHeapAryUL,    /*last align seq base*/
               primStartHeapAryUL, /*1st align prim base*/
               primEndHeapAryUL,   /*last align prim bas*/
               &alnSetStackST      /*settings*/
            ); 
      } /*If: i am searching with kmers*/

      else
      { /*Else: slower waterman search*/
         errSC =
            waterFindPrims_kmerFind(
               refHeapAryST,       /*primers looking for*/
               numRefsSI,          /*number of primers*/
               &seqStackST,        /*sequence to look at*/
               minPercScoreF,      /*min % for waterman*/
               codeHeapAryUI,      /*# times found prim*/
               dirHeapArySC,       /*direction of primer*/
               scoreHeapArySL,     /*scores for prims*/
               seqStartHeapAryUL,  /*1st align seq base*/
               seqEndHeapAryUL,    /*last align seq base*/
               primStartHeapAryUL, /*1st align prim base*/
               primEndHeapAryUL,   /*last align prim bas*/
               &alnSetStackST      /*settings*/
            ); 
      } /*Else: slower waterman search*/

      if(! errSC)
      { /*If: I found at least on primer*/
         phit_kmerFind(
            refHeapAryST,       /*primers looking for*/
            numRefsSI,          /*number of primers*/
            &seqStackST,        /*sequence to look at*/
            codeHeapAryUI,      /*# times found a primer*/
            dirHeapArySC,       /*direction of primer*/
            scoreHeapArySL,     /*scores for each primer*/
            seqStartHeapAryUL,  /*first aligned seq base*/
            seqEndHeapAryUL,    /*last aligned seq base*/
            primStartHeapAryUL, /*1st aligned prim base*/
            primEndHeapAryUL,   /*last alinged prim base*/
            outFILE
         );
      } /*If: I found at least on primer*/

      /**************************************************\
      * Main Sec04 Sub04:
      *   - get the next sequence
      \**************************************************/

      nextSeq_main_sec04_sub04:;

      if(fqBl)
      { /*If: i have a fastq file*/
         errSC =
            (schar)
            getFqSeq_seqST(
               seqFILE,
               &seqStackST
            );
      } /*If: i have a fastq file*/

      else
      { /*Else: i have a fasta file*/
         errSC =
            (schar)
            getFaSeq_seqST(
               seqFILE,
               &seqStackST
            );
      } /*Else: i have a fasta file*/
   } /*Loop: find primers in sequences*/

   /*****************************************************\
   * Main Sec04 Sub05:
   *   - check for errors
   \*****************************************************/

   if(errSC != def_EOF_seqST)
   { /*If: i had an error of some kind*/
      if(fqBl)
         tmpStr = (schar *) "-fq";
      else
         tmpStr = (schar *) "-fa";

      if(errSC & def_memErr_seqST)
      { /*If: i had a memory error*/
         fprintf(
            stderr,
            "Memory error when %s %s\n",
            tmpStr,
            seqFileStr
         );

         goto memErr_main_sec05_sub02;
      } /*If: i had a memory error*/

      fprintf(
         stderr,
         "Error on entry %lu of %s %s\n",
         entryOnUL,
         tmpStr,
         seqFileStr
      );

      goto fileErr_main_sec05_sub03;
   } /*If: i had an error of some kind*/

   else
      errSC = 0; /*no errors*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec05:
   ^   - clean up
   ^   o main sec05 sub01:
   ^     - clean up after no errors
   ^   o main sec05 sub02:
   ^     - deal with memory errors
   ^   o main sec05 sub03:
   ^     - deal with file errors
   ^   o main sec05 sub04:
   ^     - clean up and exit
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Main Sec05 Sub01:
   *   - clean up after no errors
   \*****************************************************/

   goto cleanUp_main_sec05_sub04;

   /*****************************************************\
   * Main Sec05 Sub02:
   *   - deal with memory errors
   \*****************************************************/

   memErr_main_sec05_sub02:;

   errSC = def_memErr_kmerFind;
   goto cleanUp_main_sec05_sub04;

   /*****************************************************\
   * Main Sec05 Sub03:
   *   - deal with file errors
   \*****************************************************/

   fileErr_main_sec05_sub03:;

   errSC = def_fileErr_kmerFind;
   goto cleanUp_main_sec05_sub04;

   /*****************************************************\
   * Main Sec05 Sub04:
   *   - clean up and exit
   \*****************************************************/

   cleanUp_main_sec05_sub04:;

   /*waterman smith alignment arrays*/
   free(dirHeapArySC);
   dirHeapArySC = 0;

   free(codeHeapAryUI);
   codeHeapAryUI = 0;

   free(scoreHeapArySL);
   scoreHeapArySL = 0;

   free(seqStartHeapAryUL);
   seqStartHeapAryUL = 0;

   free(seqEndHeapAryUL);
   seqEndHeapAryUL = 0;

   free(primStartHeapAryUL);
   primStartHeapAryUL = 0;

   free(primEndHeapAryUL);
   primEndHeapAryUL = 0;

   /*alignment settings*/
   freeStack_alnSet(&alnSetStackST);

   /*kmer find structures*/
   freeStack_seqST(&seqStackST);
   freeStack_tblST_kmerFind(&kmerTblStackST);

   freeHeapAry_refST_kmerFind(
      refHeapAryST,
      numRefsSI
   );

   refHeapAryST = 0;

   if(outFILE && outFILE != stdout)
      fclose(outFILE);

   outFILE = 0;

   if(seqFILE && seqFILE != stdin)
      fclose(seqFILE);

   seqFILE = 0;

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
