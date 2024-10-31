/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' seqById SOF: Start Of Files
'   - extra sequences by read id from fastq, ... files
'   o variants:
'     - -DADD_ID_LEN: adds read id length to id long.
'       o slower for ONT. (may be faster for Illuminia??)
'     - -DTHIRTY_TWO_BIT:
'       o for 32 bit cpus (searchST.c fun01 sum step)
'   o header:
'     - included libraries and defined variables
'   o fun01: pversion_seqById
'     - prints the version number for seqById
'   o fun02: phelp_seqById
'     - prints the help message for seqById
'   o fun03: getInput_seqById
'     - gets user input
'   o main:
'     - driver function to extract sequences by read id
'   o license:
'     - licensing for this code (public domain / mit)
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*-------------------------------------------------------\
| Header:
|   - included libraries and defined variables
|   o header sec01:
|     - included libraries
|   o header sec02:
|     - defined variables
\-------------------------------------------------------*/

/*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
^ Header Sec01:
^   - included libraries
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
#include "searchST.h"
#include "idSearch.h"

/*.h files only*/
#include "../genLib/dataTypeShortHand.h"
#include "../bioTools.h"

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\
! Hidden libraries:
|   - .c  #include "getSeq.h"
!   - .h  #include "../genLib/genMath.h" .h max macro only
!   - .h  #include "idLkTbl.h"
\%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

/*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
^ Header Sec02:
^   - defined variables
\<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

#define def_ignoreIds_seqById 0
    /*1: extract sequences not matching read ids*/
    /*0: extract sequences matching read ids*/

#define def_fqFile_seqById 0
#define def_faFile_seqById 1
#define def_samFile_seqById 2

/*-------------------------------------------------------\
| Fun01: pversion_seqById
|   - prints the version number for seqById
| Input:
|   - outFILE:
|     o file to print version number to
| Output:
|   - Prints:
|     o version number to outFILE
\-------------------------------------------------------*/
void
pversion_seqById(
   void * outFILE
){
   fprintf(
      (FILE *) outFILE,
      "seqById from bioTools version: %i-%02i-%02i\n",
      def_year_bioTools,
      def_month_bioTools,
      def_day_bioTools
   );
} /*pversion_seqById*/

/*-------------------------------------------------------\
| Fun02: phelp_seqById
|   - prints the help message for seqById
| Input:
|   - outFILE:
|     o file to print help message to
| Output:
|   - Prints:
|     o help message to outFILE
\-------------------------------------------------------*/
void
phelp_seqById(
   void * outFILE
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun02 TOC:
   '   - prints the help message for seqById
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
      "seqById -fq reads.fq -id target.ids > out.fq\n"
   ); /*print usage entry*/
      
   fprintf(
      (FILE *) outFILE,
      "  - extracts sequences by read id\n"
   ); /*print description*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun02 Sec02:
   ^   - print input block
   ^   o fun02 sec02 sub01:
   ^     - print input block header
   ^   o fun02 sec02 sub02:
   ^     - file options
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun02 Sec02 Sub01:
   *   - input block header
   \*****************************************************/

   fprintf(
      (FILE *) outFILE,
      "Input:\n"
   ); /*print description*/

   /*****************************************************\
   * Fun02 Sec02 Sub02:
   *   - file options
   *   o fun02 sec02 sub02 cat01:
   *     - output file
   *   o fun02 sec02 sub02 cat02:
   *     - read id file input
   *   o fun02 sec02 sub02 cat03:
   *     - fastq file input
   *   o fun02 sec02 sub02 cat04:
   *     - sam file input (todo)
   *   o fun02 sec02 sub02 cat05:
   *     - fasta file input (todo)
   \*****************************************************/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun02 Sec02 Sub02 Cat01:
   +   - output file
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "  -out out.fq: [Optional; stdout]\n"
   );

   fprintf(
      (FILE *) outFILE,
      "    o file to save read ids to\n"
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun02 Sec02 Sub02 Cat02:
   +   - read id file input
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "  -id target.ids: [Required]\n"
   );

   fprintf(
      (FILE *) outFILE,
      "    o read ids of sequences to extract\n"
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun02 Sec02 Sub02 Cat03:
   +   - fastq file input
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "  -fq reads.fq: [Required]\n"
   );

   fprintf(
      (FILE *) outFILE,
      "    o fastq file with sequences to extract\n"
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun02 Sec02 Sub02 Cat04:
   +   - sam file input
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "  -sam reads.sam: [Replaces -fq]\n"
   );

   fprintf(
      (FILE *) outFILE,
      "    o sam file with sequences to extract\n"
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun02 Sec02 Sub02 Cat05:
   +   - fasta file input (TODO)
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   /*
   fprintf(
      (FILE *) outFILE,
      "  -fq sequences.fa: [Replaces -fq]\n"
   );

   fprintf(
      (FILE *) outFILE,
      "    o fasta file with sequences to extract\n"
   );
   */

   /*****************************************************\
   * Fun02 Sec02 Sub03:
   *   - other options
   \*****************************************************/

   if(def_ignoreIds_seqById)
      fprintf(
         (FILE *) outFILE,
         "  -v: [Optional; Yes]\n"
      );

   else
      fprintf(
         (FILE *) outFILE,
         "  -v: [Optional; No]\n"
      );

   fprintf(
      (FILE *) outFILE,
      "    o extract sequences not matching read ids\n"
   );

   fprintf(
      (FILE *) outFILE,
      "    o disable with \"-no-v\"\n"
   );

   /*****************************************************\
   * Fun02 Sec02 Sub04:
   *   - help message and version numbers
   \*****************************************************/

   fprintf(
      (FILE *) outFILE,
      "  -h: print this help message\n"
   );

   fprintf(
      (FILE *) outFILE,
      "  -V: print version number\n"
   );

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun02 Sec03:
   ^   - print output block
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   fprintf(
      (FILE *) outFILE,
      "Output:\n"
   ); /*print description*/

   fprintf(
      (FILE *) outFILE,
      "  - prints extracted sequences to output file\n"
   ); /*print description*/
} /*phelp_seqById*/

/*-------------------------------------------------------\
| Fun03: getInput_seqById
|   - gets user input
| Input:
|   - numArgsSI:
|     o number of arguments user input
|   - argAryStr:
|     o array of c-stirngs with user input
|   - idFileStrPtr:
|     o c-string pointer to point to file name with ids to
|       extract
|   - seqFileStrPtr:
|     o pointer to c-string to point to file name to
|       extract sequences from
|   - fileTypeSC:
|     o pointer to char to hold seqFieStrPtr file type
|   - outFileStrPtr:
|     o c-string pointer to point to file name to output
|       to
|   - compBlPtr:
|     o char pointer to hold if extracting (1) or ignoring
|       (0) read ids
| Output:
|   - Modifies:
|     o all input variables except numArgsSI and argAryStr
|       to hold user input
|   - Returns:
|     o 0 for no errors
|     o 1 if print help message or version numbers
|     o 2 if had input error
\-------------------------------------------------------*/
signed char
getInput_seqById(
   signed int numArgsSI,       /*number arguments input*/
   char *argAryStr[],          /*input arguments*/
   signed char **idFileStrPtr, /*read ids to extract*/
   signed char **seqFileStrPtr,/*sequences to extract*/
   signed char *fileTypeSC,    /*file type seqFileStrPtr*/
   signed char **outFileStrPtr,/*file to output to*/
   signed char *compBlPtr      /*1: ignore read ids*/
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun03 TOC:
   '   - gets user input
   '   o fun03 sec01:
   '     - variable declerations
   '   o fun03 sec02:
   '     - check if have input
   '   o fun03 sec03:
   '     - get user input
   '   o fun03 sec04:
   '     - return result (error/no error)
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec01:
   ^   - variable declerations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   sint siArg = 1;
   schar resBl = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec02:
   ^   - check if have input
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(numArgsSI <= 1)
   { /*If: no arguments were input*/
      phelp_seqById(stdout);
      goto phelp_fun03_sec04;
   } /*If: no arguments were input*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec03:
   ^   - get user input
   ^   o fun03 sec03 sub01:
   ^     - start loop and check file input
   ^   o fun03 sec03 sub02:
   ^     - -v option (ingore read ids)
   ^   o fun03 sec03 sub03:
   ^     - help message
   ^   o fun03 sec03 sub04:
   ^     - version number
   ^   o fun03 sec03 sub05:
   ^     - unkown input
   ^   o fun03 sec03 sub06:
   ^     - move to next argument in input
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun03 Sec03 Sub01:
   *   - start loop and check file input
   *   o fun03 sec03 sub01 cat01:
   *     - id file (filter file)
   *   o fun03 sec03 sub01 cat02:
   *     - output file
   *   o fun03 sec03 sub01 cat03:
   *     - fastq file
   *   o fun03 sec03 sub01 cat04:
   *     - fasta file
   *   o fun03 sec03 sub01 cat05:
   *     - sam file
   \*****************************************************/

   while(siArg < numArgsSI)
   { /*Loop: get user input*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun03 Sec03 Sub01 Cat01:
      +   - id file (filter file)
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      resBl =
         eql_charCp(
            (schar *) "-id",
            (schar *) argAryStr[siArg],
            0
         );

      if(! resBl)
      { /*If: an read id file was input*/
         ++siArg;
         *idFileStrPtr = (schar *) argAryStr[siArg];
         goto nextArg_fun03_sec03_sub0x;
      } /*If: an read id file was input*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun03 Sec03 Sub01 Cat02:
      +   - output file
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      resBl =
         eql_charCp(
            (schar *) "-out",
            (schar *) argAryStr[siArg],
            0
         );

      if(! resBl)
      { /*If: an read id file was input*/
         ++siArg;
         *outFileStrPtr = (schar *) argAryStr[siArg];
         goto nextArg_fun03_sec03_sub0x;
      } /*If: an read id file was input*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun03 Sec03 Sub01 Cat03:
      +   - fastq file
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      resBl =
         eql_charCp(
            (schar *) "-fq",
            (schar *) argAryStr[siArg],
            0
         );

      if(! resBl)
      { /*If: a fastq file was input*/
         ++siArg;
         *seqFileStrPtr = (schar *) argAryStr[siArg];
         *fileTypeSC = def_fqFile_seqById;
         goto nextArg_fun03_sec03_sub0x;
      } /*If: a fastq file was input*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun03 Sec03 Sub01 Cat04:
      +   - fasta file
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      resBl =
         eql_charCp(
            (schar *) "-fa",
            (schar *) argAryStr[siArg],
            0
         ); /*check if read id file*/

      if(! resBl)
      { /*If: a fasta file was input*/
         ++siArg;
         *seqFileStrPtr = (schar *) argAryStr[siArg];
         *fileTypeSC = def_faFile_seqById;
         goto nextArg_fun03_sec03_sub0x;
      } /*If: a fasta file was input*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun03 Sec03 Sub01 Cat05:
      +   - sam file
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      resBl =
         eql_charCp(
            (schar *) "-sam",
            (schar *) argAryStr[siArg],
            0
         );

      if(! resBl)
      { /*If: a sam file was input*/
         ++siArg;
         *seqFileStrPtr = (schar *) argAryStr[siArg];
         *fileTypeSC = def_samFile_seqById;
         goto nextArg_fun03_sec03_sub0x;
      } /*If: a sam file was input*/

      /**************************************************\
      * Fun03 Sec03 Sub02:
      *   - -v option (ingore read ids)
      \**************************************************/

      resBl =
         eql_charCp(
            (schar *) "-v",
            (schar *) argAryStr[siArg],
            0
         );

      if(! resBl)
      { /*If: ignoring read ids*/
         *compBlPtr = 1;
         goto nextArg_fun03_sec03_sub0x;
      } /*If: ignoring read ids*/

      resBl =
         eql_charCp(
            (schar *) "-no-v",
            (schar *) argAryStr[siArg],
            0
         ); /*check if read id file*/

      if(! resBl)
      { /*If: keeping read ids*/
         *compBlPtr = 0;
         goto nextArg_fun03_sec03_sub0x;
      } /*If: keeping read ids*/

      /**************************************************\
      * Fun03 Sec03 Sub03:
      *   - help message
      \**************************************************/

      resBl =
         eql_charCp(
            (schar *) "-h",
            (schar *) argAryStr[siArg],
            0
         );

      if(! resBl)
      { /*If: printing help message*/
         phelp_seqById(stdout);
         goto phelp_fun03_sec04;
      } /*If: printing help message*/

      resBl =
         eql_charCp(
            (schar *) "--h",
            (schar *) argAryStr[siArg],
            0
         );

      if(! resBl)
      { /*If: printing help message*/
         phelp_seqById(stdout);
         goto phelp_fun03_sec04;
      } /*If: printing help message*/

      resBl =
         eql_charCp(
            (schar *) "help",
            (schar *) argAryStr[siArg],
            0
         );

      if(! resBl)
      { /*If: printing help message*/
         phelp_seqById(stdout);
         goto phelp_fun03_sec04;
      } /*If: printing help message*/

      resBl =
         eql_charCp(
            (schar *) "-help",
            (schar *) argAryStr[siArg],
            0
         ); /*check if read id file*/

      if(! resBl)
      { /*If: printing help message*/
         phelp_seqById(stdout);
         goto phelp_fun03_sec04;
      } /*If: printing help message*/

      resBl =
         eql_charCp(
            (schar *) "--help",
            (schar *) argAryStr[siArg],
            0
         );

      if(! resBl)
      { /*If: printing help message*/
         phelp_seqById(stdout);
         goto phelp_fun03_sec04;
      } /*If: printing help message*/

      /**************************************************\
      * Fun03 Sec03 Sub04:
      *   - version number
      \**************************************************/

      resBl =
         eql_charCp(
            (schar *) "-V",
            (schar *) argAryStr[siArg],
            0
         );

      if(! resBl)
      { /*If: printing help message*/
         pversion_seqById(stdout);
         goto pversion_fun03_sec04;
      } /*If: printing help message*/

      resBl =
         eql_charCp(
            (schar *) "--V",
            (schar *) argAryStr[siArg],
            0
         );

      if(! resBl)
      { /*If: printing help message*/
         pversion_seqById(stdout);
         goto pversion_fun03_sec04;
      } /*If: printing help message*/

      resBl =
         eql_charCp(
            (schar *) "version",
            (schar *) argAryStr[siArg],
            0
         );

      if(! resBl)
      { /*If: printing help message*/
         pversion_seqById(stdout);
         goto pversion_fun03_sec04;
      } /*If: printing help message*/

      resBl =
         eql_charCp(
            (schar *) "-version",
            (schar *) argAryStr[siArg],
            0
         );

      if(! resBl)
      { /*If: printing help message*/
         pversion_seqById(stdout);
         goto pversion_fun03_sec04;
      } /*If: printing help message*/

      resBl =
         eql_charCp(
            (schar *) "--version",
            (schar *) argAryStr[siArg],
            0
         );

      if(! resBl)
      { /*If: printing help message*/
         pversion_seqById(stdout);
         goto pversion_fun03_sec04;
      } /*If: printing help message*/

      /**************************************************\
      * Fun03 Sec03 Sub05:
      *   - unkown input
      \**************************************************/

      fprintf(
         stderr,
         "%s is not recongnized\n",
         argAryStr[siArg]
      );
         
      goto err_fun03_sec04;

      /**************************************************\
      * Fun03 Sec03 Sub06:
      *   - move to next input
      \**************************************************/

      nextArg_fun03_sec03_sub0x:;
      ++siArg;
   } /*Loop: get user input*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec04:
   ^   - return result (error/no error)
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   return 0;

   phelp_fun03_sec04:;
   return 1;

   pversion_fun03_sec04:;
   return 1;

   err_fun03_sec04:;
   return 2;
} /*getInput_seqById*/

/*-------------------------------------------------------\
| Main:
|   - driver function to extract sequences by read id
| Input:
|   - numArgsSI:
|     o number of arguments input
|   - argAryStr:
|     o array of c-stings with user input
| Output:
|   - Prints:
|     o selected sequences to stdout or specified out file
\-------------------------------------------------------*/
int
main(
   int numArgsSI,
   char *argAryStr[]
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Main TOC:
   '   - driver function to extract sequences by read id
   '   o main sec01:
   '     - variable declerations
   '   o main sec02:
   '     - get and check user input
   '   o main sec03:
   '     - build read id hash table
   '   o main sec04:
   '     - build sam file header
   '   o main sec05:
   '     - get sequences
   '   o main sec06:
   '     - clean up
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec01:
   ^   - variable declerations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   schar *idFileStr = 0;    /*has read ids to extract*/
   schar *seqFileStr = 0;   /*file to get sequences from*/
   schar fileTypeSC = 0;    /*file type of seqFileStr*/
   schar *outFileStr = 0;   /*file to output to*/

   sint tmpSI = 0;

   schar headerStr[1024];   /*header for sam file*/

   schar compBl = def_ignoreIds_seqById;
      /*1: keep reads not matching read ids*/

   signed char errSC = 0;
   slong errSL = 0;

   struct searchST *hashHeapST = 0;
   FILE *inFILE = 0;
   FILE *outFILE = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec02:
   ^   - get and check user input
   ^   o main sec02 sub01:
   ^     - get user input
   ^   o main sec02 sub02:
   ^     - check if have two stdin files
   ^   o main sec02 sub03:
   ^     - check if sequence file can be opened
   ^   o main sec02 sub04:
   ^     - check if id file can be opened
   ^   o main sec02 sub05:
   ^     - check if output file can be opened
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Main Sec02 Sub01:
   *   - get user input
   \*****************************************************/

   errSC =
      getInput_seqById(
         numArgsSI,      /*number arguments input*/
         argAryStr,      /*input arguments*/
         &idFileStr,     /*read ids to extract*/
         &seqFileStr,    /*sequences to extract*/
         &fileTypeSC,    /*fastq, fasta, or sam*/
         &outFileStr,    /*file to output to*/
         &compBl         /*1: ignore read ids*/
   ); /*get user input*/

   if(errSC)
   { /*If: had an error*/
      errSL = (ulong) errSC - 1;
         /*convert help/version error to 0*/

      goto cleanUp_main_sec06_sub0x;
   } /*If: had an error*/

   /*****************************************************\
   * Main Sec02 Sub02:
   *   - check if have two stdin files
   \*****************************************************/
 
   errSC = 0;

   if(
         ! idFileStr 
      || *idFileStr == '-'
   ) errSC += 1; /*id file is from stdin*/

   if(
         ! seqFileStr 
      || *seqFileStr == '-'
   ) errSC += 1; /*sequence file is from stdin*/

   if(errSC > 1)
   { /*If: both files from stdin*/
      fprintf(
         stderr,
         "ERROR: id file (-id) and sequence file\n"
      );

      fprintf(
         stderr,
         "  (-fq, -fa, or -sam) are from stdin\n"
      );

      goto fileErr_main_sec06_sub03;
   } /*If: both files from stdin*/

   /*****************************************************\
   * Main Sec02 Sub03:
   *   - check if sequence file can be opened
   \*****************************************************/

   if(
         ! seqFileStr
      || *seqFileStr == '-'
   ) ; /*from stdin; handle later*/

   else
   { /*Else: user provided a sequence file*/
      inFILE =
        fopen(
           (char *) seqFileStr,
           "r"
        );

      if(! inFILE)
      { /*If: no sequenc file provided*/
         if(fileTypeSC == def_fqFile_seqById)
            fprintf(
               stderr,
               "could not open -fq %s\n",
               seqFileStr
            );

         else if(fileTypeSC == def_faFile_seqById)
            fprintf(
               stderr,
               "could not open -fa %s\n",
               seqFileStr
            );

         else
            fprintf(
               stderr,
               "could not open -sam %s\n",
               seqFileStr
            );

        goto fileErr_main_sec06_sub03;
      } /*If: no sequenc file provided*/

      fclose(inFILE);
   } /*Else: user provided a sequence file*/

   inFILE = 0;

   /*****************************************************\
   * Main Sec02 Sub04:
   *   - check if id file can be opened
   \*****************************************************/

   if(
         ! idFileStr
      || *idFileStr == '-'
   ) inFILE = stdin;

   else
   { /*Else: user provided a read id file*/
      inFILE =
        fopen(
           (char *) idFileStr,
           "r"
        );

      if(! inFILE)
      { /*If: no id file provided*/
         if(fileTypeSC == def_fqFile_seqById)
            fprintf(
               stderr,
               "could not open -id %s\n",
               idFileStr
            );

        goto fileErr_main_sec06_sub03;
      } /*If: no id file provided*/
   } /*Else: user provided a read id file*/

   /*****************************************************\
   * Main Sec02 Sub05:
   *   - check if output file can be opened
   \*****************************************************/

   if(
         ! outFileStr
      || *outFileStr == '-'
   ) outFILE = stdout;

   else
   { /*Else: user provided an output file*/
      outFILE =
        fopen(
           (char *) outFileStr,
           "w+"
        );

      if(! inFILE)
      { /*If: no id file provided*/
         if(fileTypeSC == def_fqFile_seqById)
            fprintf(
               stderr,
               "could not open -out %s\n",
               outFileStr
            );

        goto fileErr_main_sec06_sub03;
      } /*If: no id file provided*/
   } /*Else: user provided an output file*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec03:
   ^   - build read id hash table
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   hashHeapST =
      getReadIds_searchST(
          inFILE,
          1,
          &errSC
      );

   if(
         inFILE
      && inFILE != stdin
      && inFILE != stdout
   ) fclose(inFILE);

   inFILE = 0;

   if(! hashHeapST)
   { /*If: had memory error*/
      fprintf(
         stderr,
         "MEMORY ERROR: getting read ids from -id %s\n",
         idFileStr
      );

      goto memErr_main_sec06_sub02;
   } /*If: had memory error*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec04:
   ^   - build sam file header
   ^   o main sec04 sub01:
   ^     - program id / version number
   ^   o main sec04 sub02:
   ^     - if complementing search
   ^   o main sec04 sub03:
   ^     - copy id file
   ^   o main sec04 sub04:
   ^     - copy input sequence file
   ^   o main sec04 sub05:
   ^     - copy output file
   ^   o main sec04 sub06:
   ^     - end the c-string
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(fileTypeSC == def_samFile_seqById)
   { /*If: input is samfile (output needs header)*/
      /**************************************************\
      * Main Sec04 Sub01:
      *   - program id / version number
      \**************************************************/

      tmpSI = 0;

      tmpSI +=
         cpDelim_ulCp(
            &headerStr[tmpSI],
            (schar *)
              "@PG\tID:seqById\tPN:seqById\tVN:bioTools_",
            0,
            '\0'
         );

      errSL = def_year_bioTools;
      
      headerStr[tmpSI++] = (errSL / 1000) + 48;
      errSL %= 1000;
      headerStr[tmpSI++] = (errSL / 100) + 48;
      errSL %= 100;
      headerStr[tmpSI++] = (errSL / 10) + 48;
      errSL %= 10;
      headerStr[tmpSI++] = (errSL) + 48;

      headerStr[tmpSI++] = '-';

      if(def_month_bioTools > 9)
      { /*If: month has two digits*/
        headerStr[tmpSI++] = (def_month_bioTools / 10)+48;
        headerStr[tmpSI++] = (def_month_bioTools % 10)+48;
      } /*If: month has two digits*/

      else
      { /*Else: month needs a add zero in front*/
         headerStr[tmpSI++] = '0';
         headerStr[tmpSI++] = def_month_bioTools + 48;
      } /*Else: month needs a add zero in front*/

      headerStr[tmpSI++] = '-';

      if(def_day_bioTools > 9)
      { /*If: day has two digits*/
        headerStr[tmpSI++] = (def_day_bioTools / 10) + 48;
        headerStr[tmpSI++] = (def_day_bioTools % 10) + 48;
      } /*If: day has two digits*/

      else
      { /*Else: day needs a add zero in front*/
         headerStr[tmpSI++] = '0';
         headerStr[tmpSI++] = def_day_bioTools + 48;
      } /*Else: day needs a add zero in front*/

      /**************************************************\
      * main sec04 sub02:
      *   - if complementing search
      \**************************************************/

      if(compBl)
      { /*If: getting complement reads*/
         headerStr[tmpSI++] = ' ';
         headerStr[tmpSI++] = '-';
         headerStr[tmpSI++] = 'v';
      } /*If: getting complement reads*/

      else
      { /*If: keeping target reads*/
         headerStr[tmpSI++] = ' ';
         headerStr[tmpSI++] = '-';
         headerStr[tmpSI++] = 'n';
         headerStr[tmpSI++] = 'o';
         headerStr[tmpSI++] = '-';
         headerStr[tmpSI++] = 'v';
      } /*If: keeping target reads*/

      /**************************************************\
      * main sec04 sub03:
      *   - copy id file
      \**************************************************/

      headerStr[tmpSI++] = ' ';
      headerStr[tmpSI++] = '-';
      headerStr[tmpSI++] = 'i';
      headerStr[tmpSI++] = 'd';

      if(
            ! idFileStr
         || *idFileStr == '-'
      ){ /*If: ids were from stdin*/
         headerStr[tmpSI++] = ' ';
         headerStr[tmpSI++] = '-';
      } /*If: ids were from stdin*/

      else
      { /*Else: id file input*/
         headerStr[tmpSI++] = ' ';

         tmpSI +=
            cpDelim_ulCp(
               &headerStr[tmpSI],
               idFileStr,
               0,
               '\0'
            );
      } /*Else: id file input*/

      /**************************************************\
      * main sec04 sub04:
      *   - copy input sequence file
      \**************************************************/

      headerStr[tmpSI++] = ' ';
      headerStr[tmpSI++] = '-';
      headerStr[tmpSI++] = 's';
      headerStr[tmpSI++] = 'a';
      headerStr[tmpSI++] = 'm';

      if(
            ! seqFileStr
         || *seqFileStr == '-'
      ){ /*If: sequences from stdin*/
         headerStr[tmpSI++] = ' ';
         headerStr[tmpSI++] = '-';
      } /*If: sequences from stdin*/

      else
      { /*Else: sequences file input*/
         headerStr[tmpSI++] = ' ';

         tmpSI +=
            cpDelim_ulCp(
               &headerStr[tmpSI],
               seqFileStr,
               0,
               '\0'
            );
      } /*Else: sequences file input*/

      /**************************************************\
      * main sec04 sub05:
      *   - copy output file
      \**************************************************/

      headerStr[tmpSI++] = ' ';
      headerStr[tmpSI++] = '-';
      headerStr[tmpSI++] = 'o';
      headerStr[tmpSI++] = 'u';
      headerStr[tmpSI++] = 't';

      if(
            ! outFileStr
         || *outFileStr == '-'
      ){ /*If: outputing to stdout*/
         headerStr[tmpSI++] = ' ';
         headerStr[tmpSI++] = '-';
      } /*If: outputing to stdout*/

      else
      { /*Else: output file input*/
         headerStr[tmpSI++] = ' ';

         tmpSI +=
            cpDelim_ulCp(
               &headerStr[tmpSI],
               outFileStr,
               0,
               '\0'
            );
      } /*Else: output file input*/

      /**************************************************\
      * main sec04 sub06:
      *   - end the c-string
      \**************************************************/

      headerStr[tmpSI++] = '\n';
      headerStr[tmpSI++] = '\0';
   } /*If: input is samfile (output needs header)*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec05:
   ^   - get sequences
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(
         ! seqFileStr
      || *seqFileStr == '-'
   ) inFILE = stdin;

   else
      inFILE =
        fopen(
           (char *) seqFileStr,
           "r"
        ); /*already checked if could open file*/

   if(fileTypeSC == def_samFile_seqById)
      errSL =
         getSamSeq_idSearch(
            hashHeapST,
            compBl,
            headerStr,
            inFILE,
            outFILE
         );

   else
      errSL =
         getFqSeq_idSearch(
            hashHeapST,
            compBl,
            inFILE,
            outFILE
         );

   if(errSL)
   { /*If: invalid file*/
      if(errSL < 0)
         fprintf(
            stderr,
            "MEMORY ERROR creating buffers\n"
         );
      
      else if(fileTypeSC == def_fqFile_seqById)
         fprintf(
            stderr,
            "entry %lu in -fq %s is not a fastq entry\n",
            errSL,
            seqFileStr
         );

      else if(fileTypeSC == def_faFile_seqById)
         fprintf(
            stderr,
            "entry %lu in -fa %s is not a fasta entry\n",
            errSL,
            seqFileStr
         );

      else
         fprintf(
            stderr,
            "entry %lu in -sam %s is not a sam entry\n",
            errSL,
            seqFileStr
         );

      goto fileErr_main_sec06_sub03;
   } /*If: invalid file*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec06:
   ^   - clean up
   ^   o main sec06 sub01:
   ^     - no error clean up
   ^   o main sec06 sub02:
   ^     - report memory errors
   ^   o main sec06 sub03:
   ^     - report file errors
   ^   o main sec06 sub04:
   ^     - clean up (error/no error)
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Main Sec06 Sub01:
   *   - no error clean up
   \*****************************************************/

   errSL = 0;
   goto cleanUp_main_sec06_sub0x;

   /*****************************************************\
   * Main Sec06 Sub02:
   *   - report memory errors
   \*****************************************************/

   memErr_main_sec06_sub02:;
   errSL = 1;
   goto cleanUp_main_sec06_sub0x;

   /*****************************************************\
   * Main Sec06 Sub03:
   *   - report file errors
   \*****************************************************/

   fileErr_main_sec06_sub03:;
   errSL = 2;
   goto cleanUp_main_sec06_sub0x;

   /*****************************************************\
   * Main Sec06 Sub04:
   *   - clean up (error/no error)
   \*****************************************************/

   cleanUp_main_sec06_sub0x:;

   if(
         inFILE
      && inFILE != stdin
      && inFILE != stdout
   ) fclose(inFILE);

   inFILE = 0;

   if(
         outFILE
      && outFILE != stdin
      && outFILE != stdout
   ) fclose(outFILE);

   outFILE = 0;

   freeHeap_searchST(hashHeapST);

   return errSL;
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
