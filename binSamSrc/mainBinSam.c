/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' mainBinSam SOF:
'   - driver function to bin reads by reference in a sam
'     file
'   o header:
'     o included libraries and defaults
'   o fun01 pversion_mainBinSam
'     - prints the version number for binSam
'   o fun02 phelp_mainBinSam
'     - prints the help message for binSam
'   o fun03: input_mainBinSam
'     - get input from user input arrguments
'   o main:
'     - driver function for binSam
'   o license:
'     - licensing for this code (CC0)
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*-------------------------------------------------------\
| Header:
|   - included libraries and defaults
\-------------------------------------------------------*/

#ifdef PLAN9
   #include <u.h>
   #include <stdlib.h>
#else
   #include <stdlib.h>
#endif

#include <stdio.h>

#include "../genLib/ulCp.h"
#include "../genLib/numToStr.h"
#include "../genBio/samEntry.h"
#include "../genBio/samRef.h"

/*only .h files*/
#include "../genLib/endLine.h"
#include "../bioTools.h"

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\
! Hidden Libraries:
!   - 
!   - .c  #include "../genLib/numToStr.h"
!   - .c  #include "../genLib/base10str.h"
!   - .c  #include "../genLib/strAry.h"
!   - .c  #include "../genLib/fileFun.h"
!   - .h  #include "ntTo5Bit.h"
\%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

#define def_strict_mainBinSam 0
   /*0: if merge refseq assembly genomes 1: do not*/
#define def_noMap_mainBinSam 0
#define def_secondaryAln_mainBinSam 0
#define def_supplementalAln_mainBinSam 0
#define def_outputFormat_mainBinSam 0
   /*0 = sam file; 1 = fastq file; 2 = fasta file*/
signed char *glob_prefixStr = (signed char *) "out";

/*-------------------------------------------------------\
| Fun01 pversion_mainBinSam
|   - prints the version number for binSam
| Input:
|   - outFILE:
|     o FILE pionter to print the version number to
| Output:
|   - Prints:
|     o version number to outFILE
\-------------------------------------------------------*/
void
pversion_mainBinSam(
   void *outFILE
){
   fprintf(
      (FILE *) outFILE,
      "binSam from bioTools version: %i-%02i-%02i%s",
      def_year_bioTools,
      def_month_bioTools,
      def_day_bioTools,
      str_endLine
   );
} /*pversion_mainBinSam*/

/*-------------------------------------------------------\
| Fun02 phelp_mainBinSam
|   - prints the help message for binSam
| Input:
|   - outFILE:
|     o FILE pionter to print the help message to
| Output:
|   - Prints:
|     o help message to outFILE
\-------------------------------------------------------*/
void
phelp_mainBinSam(
   void *outFILE
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun02 TOC:
   '   - prints the help message for binSam
   '     - print usage message
   '   o fun02 sec02:
   '     - print input message
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun02 Sec01:
   ^   - print usage message
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   fprintf(
      (FILE *) outFILE,
      "binSam -sam reads.sam -prefix %s%s",
      glob_prefixStr,
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "  - bins reads by reference%s",
      str_endLine
   );

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun02 Sec02:
   ^   - print input message
   ^   o fun02 sec02 sub01:
   ^     - input entry and read input
   ^   o fun02 sec02 sub02:
   ^     - output options
   ^   o fun02 sec02 sub03:
   ^     - strict mode
   ^   o fun02 sec02 sub04:
   ^     - umapped reads
   ^   o fun02 sec02 sub05:
   ^     - secondary alignments
   ^   o fun02 sec02 sub06:
   ^     - supplemental alignments
   ^   o fun02 sec02 sub07:
   ^     - help message and version
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun02 Sec02 Sub01:
   *   - input entry and read input
   \*****************************************************/

   fprintf((FILE *) outFILE, "Input:%s", str_endLine);

   fprintf(
      (FILE *) outFILE,
      "  -sam reads.sam: [Optional]%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "    o sam file with reads to bin%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "    o use `-sam -` for stdin input%s",
      str_endLine
   );

   /*****************************************************\
   * Fun02 Sec02 Sub02:
   *   - output options
   *   o fun02 sec03 sub02 cat01:
   *     - prefix (output)
   *   o fun02 sec03 sub02 cat02:
   *     - sam file (output)
   *   o fun02 sec03 sub02 cat03:
   *     - fastq file (output)
   *   o fun02 sec03 sub02 cat04:
   *     - fasta file (output)
   \*****************************************************/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun02 Sec03 Sub02 Cat01:
   +   - prefix (output)
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "  -prefix %s: [Optional]%s",
      glob_prefixStr,
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "    o prefix for output file names%s",
      str_endLine
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun02 Sec03 Sub02 Cat02:
   +   - sam file (output)
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   if(def_outputFormat_mainBinSam == 0)
      fprintf(
         (FILE *) outFILE,
         "  -sam-out: [Optional; yes]%s",
         str_endLine
      );
   else
      fprintf(
         (FILE *) outFILE,
         "  -sam-out: [Optional; no]%s",
         str_endLine
      );

   fprintf(
      (FILE *) outFILE,
      "    o saved binned reads in a sam file%s",
      str_endLine
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun02 Sec03 Sub02 Cat03:
   +   - fastq file (output)
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   if(def_outputFormat_mainBinSam == 1)
      fprintf(
         (FILE *) outFILE,
         "  -fq-out: [Optional; yes]%s",
         str_endLine
      );
   else
      fprintf(
         (FILE *) outFILE,
         "  -fq-out: [Optional; no]%s",
         str_endLine
      );

   fprintf(
      (FILE *) outFILE,
      "    o saved binned reads in a fastq file%s",
      str_endLine
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun02 Sec03 Sub02 Cat04:
   +   - fasta file (output)
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   if(def_outputFormat_mainBinSam == 2)
      fprintf(
         (FILE *) outFILE,
         "  -fa-out: [Optional; yes]%s",
         str_endLine
      );
   else
      fprintf(
         (FILE *) outFILE,
         "  -fa-out: [Optional; no]%s",
         str_endLine
      );

   fprintf(
      (FILE *) outFILE,
      "    o saved binned reads in a fasta file%s",
      str_endLine
   );


   /*****************************************************\
   * Fun02 Sec02 Sub03:
   *   - strict mode
   \*****************************************************/

   if(def_strict_mainBinSam)
      fprintf(
         (FILE *) outFILE,
         "  -strict: [Optional; True]%s",
         str_endLine
      );
   else
      fprintf(
         (FILE *) outFILE,
         "  -strict: [Optional; False]%s",
         str_endLine
      );

   fprintf(
      (FILE *) outFILE,
      "    o do not merge assembly contigs from refseq%s",
      str_endLine
   );
   fprintf(
      (FILE *) outFILE,
      "    o use `-merge` to disable%s",
      str_endLine
   );

   /*****************************************************\
   * Fun02 Sec02 Sub04:
   *   - umapped reads
   \*****************************************************/

   if(def_noMap_mainBinSam)
      fprintf(
         (FILE *) outFILE,
         "  -unmap: [Optional; True]%s",
         str_endLine
      );
   else
      fprintf(
         (FILE *) outFILE,
         "  -unmap: [Optional; False]%s",
         str_endLine
      );

   fprintf(
      (FILE *) outFILE,
      "    o print unmapped reads%s",
      str_endLine
   );
   fprintf(
      (FILE *) outFILE,
      "    o use `-no-unmap` to disable%s",
      str_endLine
   );

   /*****************************************************\
   * Fun02 Sec02 Sub05:
   *   - secondary alignments
   \*****************************************************/

   if(def_secondaryAln_mainBinSam)
      fprintf(
         (FILE *) outFILE,
         "  -secondary: [Optional; True]%s",
         str_endLine
      );
   else
      fprintf(
         (FILE *) outFILE,
         "  -secondary: [Optional; False]%s",
         str_endLine
      );

   fprintf(
      (FILE *) outFILE,
      "    o print secondary aligned reads with sequences"
   );
   fprintf((FILE *) outFILE, "%s", str_endLine);
   fprintf(
      (FILE *) outFILE,
      "    o use `-no-secondary` to disable%s",
      str_endLine
   );

   /*****************************************************\
   * Fun02 Sec02 Sub06:
   *   - supplemental alignments
   \*****************************************************/

   if(def_supplementalAln_mainBinSam)
      fprintf(
         (FILE *) outFILE,
         "  -supplemental: [Optional; True]%s",
         str_endLine
      );
   else
      fprintf(
         (FILE *) outFILE,
         "  -supplemental: [Optional; False]%s",
         str_endLine
      );

   fprintf(
      (FILE *) outFILE,
      "    o print supplemental aligned reads with"
   );
   fprintf((FILE *) outFILE, " sequences%s", str_endLine);
   fprintf(
      (FILE *) outFILE,
      "    o use `-no-supplemental` to disable%s",
      str_endLine
   );

   /*****************************************************\
   * Fun02 Sec02 Sub07:
   *   - help message and version
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
   ^   - print output message
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   fprintf((FILE *) outFILE, "Output:%s", str_endLine);

   fprintf(
    (FILE *) outFILE,
     "  - reads to file name prefix-<reference_id>.sam%s",
     str_endLine
   );
} /*phelp_mainBinSam*/

/*-------------------------------------------------------\
| Fun03: input_mainBinSam
|   - get input from user input arrguments
| Input:
|   - argLenSI:
|     o number of arguments the user input
|   - argAryStr:
|     o c-string array with user input
|   - samFileStrPtr:
|     o c-string pionter to be set to the input sam file
|   - prefixStrPtr:
|     o c-string pionter to be set to the output prefix
|   - outTypeSCPtr:
|     o signed char pointer to get the output file type
|       * set to 0 for sam file
|       * set to 1 for fastq file
|       * set to 2 for fasta file
|   - pStrictBlPtr:
|     o signed char pointer to get if merging references
|       when possible (if assembly likely has multiple
|       contigs)
|       * 1: yes
|       * 0: no
|   - pUnmapBlPtr:
|     o signed char pointer to get if print unmapped reads
|       * 1: yes
|       * 0: no
|   - pSecondaryBlPtr:
|     o signed char pointer to get if print secondary
|       alignments
|       * 1: yes
|       * 0: no
|   - pSupplementalBlPtr:
|     o signed char pointer to get if print supplemental
|       alignments
|       * 1: yes
|       * 0: no
| Output:
|   - Modifies:
|     o all input (except argLenSI and argAryStr) to have
|       user input
|   - Prints:
|     o help message to stdout if user requested it
|     o version number to stdout if user requested it
|     o errors to stderr
|   - Returns:
|     - 0 for no errors
|     - 1 if printed help message or version number
|     - 2 for errors
\-------------------------------------------------------*/
signed char
input_mainBinSam(
   int argLenSI,                 /*number arguments*/
   char *argAryStr[],            /*user arugments*/
   signed char **samFileStrPtr,  /*input sam file*/
   signed char **prefixStrPtr,   /*prefix for file names*/
   signed char *outTypeSCPtr,    /*output file type*/
   signed char *pSctricttBlPtr,  /*no reference merging*/
   signed char *pUnmapBlPtr,     /*print unmapped reads*/
   signed char *pSecondaryBlPtr, /*print secondary alns*/
   signed char *pSupplementalBlPtr/*print supplemental*/
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun03 TOC:
   '   - get input from user input arrguments
   '   o fun03 sec01:
   '     - variable declarations and check if have input
   '   o fun03 sec03:
   '     - return result
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec01:
   ^   - variable declarations and check if have input
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   signed int siArg = 1;
   signed char *tmpStr = 0;

   if(argLenSI < 2)
      goto phelp_fun03_sec03;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec02:
   ^   - get user input
   ^   o fun03 sec02 sub01:
   ^     - start input loop + get file IO input
   ^   o fun03 sec02 sub02:
   ^     - output boolean settings
   ^   o fun03 sec02 sub03:
   ^     - help message
   ^   o fun03 sec02 sub04:
   ^     - version number
   ^   o fun03 sec02 sub05:
   ^     - unkown input
   ^   o fun03 sec02 sub06:
   ^     - move to the next argument
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun03 Sec02 Sub01:
   *   - start input loop + get file IO input
   \*****************************************************/

   while(siArg < argLenSI)
   { /*Loop: get user input*/
      tmpStr = (signed char *) argAryStr[siArg];

      if(! eqlNull_ulCp((signed char *) "-sam", tmpStr))
      { /*If: sam file input*/
         ++siArg;
         *samFileStrPtr=(signed char *) argAryStr[siArg];
      } /*If: sam file input*/

      else if(
        ! eqlNull_ulCp((signed char *) "-prefix", tmpStr)
      ){ /*Else If: prefix for output file*/
         ++siArg;
         *prefixStrPtr=(signed char *) argAryStr[siArg];
      }  /*Else If: prefix for output file*/

      else if(
        ! eqlNull_ulCp((signed char *) "-sam-out", tmpStr)
      ) *outTypeSCPtr = 0; /*outputing to a sam file*/

      else if(
        ! eqlNull_ulCp((signed char *) "-fq-out", tmpStr)
      ) *outTypeSCPtr = 1; /*outputing to a fastq file*/

      else if(
        ! eqlNull_ulCp((signed char *) "-fa-out", tmpStr)
      ) *outTypeSCPtr = 2; /*outputing to a fasta file*/

      /**************************************************\
      * Fun03 Sec02 Sub02:
      *   - output boolean settings
      \**************************************************/

      else if(
        ! eqlNull_ulCp((signed char *) "-strict", tmpStr)
      ) *pSctricttBlPtr = 1;

      else if(
        ! eqlNull_ulCp((signed char *) "-merge", tmpStr)
      ) *pSctricttBlPtr = 0;

      else if(
        ! eqlNull_ulCp((signed char *) "-unmap", tmpStr)
      ) *pUnmapBlPtr = 1;

      else if(
        ! eqlNull_ulCp((signed char *) "-no-unmap",tmpStr)
      ) *pUnmapBlPtr = 0;

      else if(
        ! eqlNull_ulCp(
           (signed char *) "-secondary",
           tmpStr
        )
      ) *pSecondaryBlPtr = 1;

      else if(
        ! eqlNull_ulCp(
           (signed char *) "-no-secondary",
           tmpStr
        )
      ) *pSecondaryBlPtr = 0;

      else if(
        ! eqlNull_ulCp(
           (signed char *) "-supplemental",
           tmpStr
        )
      ) *pSupplementalBlPtr = 1;

      else if(
        ! eqlNull_ulCp(
           (signed char *) "-no-supplemental",
           tmpStr
        )
      ) *pSupplementalBlPtr = 0;

      /**************************************************\
      * Fun03 Sec02 Sub03:
      *   - help message
      \**************************************************/

      else if(
        ! eqlNull_ulCp((signed char *) "-h", tmpStr)
      ) goto phelp_fun03_sec03;

      else if(
        ! eqlNull_ulCp((signed char *) "--h", tmpStr)
      ) goto phelp_fun03_sec03;

      else if(
        ! eqlNull_ulCp((signed char *) "help", tmpStr)
      ) goto phelp_fun03_sec03;

      else if(
        ! eqlNull_ulCp((signed char *) "-help", tmpStr)
      ) goto phelp_fun03_sec03;

      else if(
        ! eqlNull_ulCp((signed char *) "--help", tmpStr)
      ) goto phelp_fun03_sec03;

      /**************************************************\
      * Fun03 Sec02 Sub04:
      *   - version number
      \**************************************************/

      else if(
        ! eqlNull_ulCp((signed char *) "-v", tmpStr)
      ) goto pversion_fun03_sec03;

      else if(
        ! eqlNull_ulCp((signed char *) "--v", tmpStr)
      ) goto pversion_fun03_sec03;

      else if(
        ! eqlNull_ulCp((signed char *) "version", tmpStr)
      ) goto pversion_fun03_sec03;

      else if(
        ! eqlNull_ulCp((signed char *) "-version", tmpStr)
      ) goto pversion_fun03_sec03;

      else if(
        ! eqlNull_ulCp((signed char *) "--version",tmpStr)
      ) goto pversion_fun03_sec03;

      /**************************************************\
      * Fun03 Sec02 Sub05:
      *   - unkown input
      \**************************************************/

      else
      { /*Else: no idea what was input*/
         fprintf(
            stderr,
            "%s is not recognized%s",
            argAryStr[siArg],
            str_endLine
         );
         goto err_fun03_sec03;
      } /*Else: no idea what was input*/

      /**************************************************\
      * Fun03 Sec02 Sub06:
      *   - move to the next argument
      \**************************************************/

      ++siArg;
   } /*Loop: get user input*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec03:
   ^   - return result
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   siArg = 0;
   goto ret_fun03_sec03;

   phelp_fun03_sec03:;
      phelp_mainBinSam(stdout);
      siArg = 1;
      goto ret_fun03_sec03;
   pversion_fun03_sec03:;
      pversion_mainBinSam(stdout);
      siArg = 1;
      goto ret_fun03_sec03;
   err_fun03_sec03:;
      siArg = 2;
      goto ret_fun03_sec03;
   ret_fun03_sec03:;
      return (signed char) siArg;
} /*input_mainBinSam*/

/*-------------------------------------------------------\
| Main:
|   - driver function for binSam
| Input:
|   - argLenSI:
|     o number of arguments the user input
|   - argAryStr:
|     o c-string array with user input
| Output:
|   - Returns:
|     o 0 for no errors
|     o 1 for errors
\-------------------------------------------------------*/
signed int
main(
   int argLenSI,      /*number arguments user input*/
   char *argAryStr[]  /*user input*/
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Main TOC:
   '   - driver function for binSam
   '   o main sec01:
   '     - variable declarations
   '   o main sec02:
   '     - initialize, get user input, and open files
   '   o main sec03:
   '     - get reference headers
   '   o main sec04:
   '     - bin reads
   '   o main sec05:
   '     - clean up and return
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   signed char *samFileStr = 0;
   signed char *prefixStr = glob_prefixStr;
   signed char outTypeSC = def_outputFormat_mainBinSam;

   /*types of entries to print out*/
   signed char pStrictBl = 0;
   signed char pUnmapBl = 0;
   signed char pSecondaryBl = 0;
   signed char pSupplementalBl = 0;

   signed char errSC = 0;
   signed int *indexHeapArySI = 0;
   signed char *headerHeapStr = 0;
   unsigned long headSizeUL = 0;

   signed int tmpSI = 0;
   signed char *tmpStr = 0;

   struct samEntry readStackST;
   signed long seqSL = 0;
   struct refs_samRef refStackST;

   FILE *inFILE = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec02:
   ^   - initialize, get user input, and open files
   ^   o main sec02 sub01:
   ^     - initialize structures
   ^   o main sec02 sub02:
   ^     - get user input
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Main Sec02 Sub01:
   *   - initialize structures
   \*****************************************************/

   init_samEntry(&readStackST);
   init_refs_samRef(&refStackST);

   /*****************************************************\
   * Main Sec02 Sub02:
   *   - get user input
   \*****************************************************/

   errSC =
      input_mainBinSam(
         argLenSI,
         argAryStr,
         &samFileStr,
         &prefixStr,
         &outTypeSC,
         &pStrictBl,
         &pUnmapBl,
         &pSecondaryBl,
         &pSupplementalBl
      );

   if(errSC)
   { /*If: printed help/version or had an error*/
      --errSC; /*changes help/version return to 0*/
      goto ret_main_sec05;
   } /*If: printed help/version or had an error*/

   if( setup_samEntry(&readStackST) )
   { /*If: faild to add memory to the sam structure*/
      fprintf(
         stderr,
         "memory error setting up the samEntry struct%s",
         str_endLine
      );
      goto memErr_main_sec05;
   } /*If: faild to add memory to the sam structure*/

   /*****************************************************\
   * Main Sec02 Sub03:
   *   - open the input file
   \*****************************************************/

   if(! samFileStr || *samFileStr == '-')
   { /*If: input is from stdin*/
      samFileStr = (signed char *) "-";
      inFILE = stdin;
   } /*If: input is from stdin*/

   else
   { /*Else: input is from a file*/
      inFILE = fopen((char *) samFileStr, "r");

      if(! inFILE)
      { /*If: could not open the sam file*/
         fprintf(
            stderr,
            "could not open -sam %s%s",
            samFileStr,
            str_endLine
         );

         goto fileErr_main_sec05;
      } /*If: could not open the sam file*/
   } /*Else: input is from a file*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec03:
   ^   - get reference headers
   ^   o main sec03 sub01:
   ^     - get reference headers
   ^   o main sec03 sub02:
   ^     - check if need more memory for binSam header
   ^   o main sec03 sub03:
   ^     - build the binSam header
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Main Sec03 Sub01:
   *   - get reference headers
   \*****************************************************/

   errSC =
      getRefLen_samRef(
         &refStackST,
         &readStackST,
         inFILE,
         0,       /*not printing headers to toher files*/
         &headerHeapStr, /*gets header from sam file*/
         &headSizeUL   /*spare bytes in header*/
      );

   if(errSC)
   { /*If: had an error*/
      if(errSC == def_memErr_samEntry)
      { /*If: memory error*/
         fprintf(
            stderr,
            "memory error getting reference names%s",
            str_endLine
         );
         goto memErr_main_sec05;
      } /*If: memory error*/

      else if(errSC == 64)
      { /*Else: no references in header*/
         fprintf(
            stderr,
            "no references in header of -sam %s%s",
            samFileStr,
            str_endLine
         );
         goto fileErr_main_sec05;
      } /*Else: no references in header*/

      else
      { /*Else: file error*/
         fprintf(
            stderr,
            "file error reading header of -sam %s%s",
            samFileStr,
            str_endLine
         );
         goto fileErr_main_sec05;
      } /*Else: file error*/
   } /*If: had an error*/

   /*****************************************************\
   * Main Sec03 Sub02:
   *   - check if need more memory for binSam header
   \*****************************************************/

   tmpSI = endStr_ulCp(headerHeapStr);

   if(headSizeUL - tmpSI < 1024)
   { /*If: want to expand header before adding header*/
      headSizeUL += 1024;

      tmpStr =
         realloc(
            headerHeapStr,
            headSizeUL * sizeof(signed char)
         );
      if(! tmpStr)
      { /*If: head memory error*/
         fprintf(
           stderr,
           "memory error adding binSam entry to header%s",
           str_endLine
         );
         goto memErr_main_sec05;
      } /*If: head memory error*/

      headerHeapStr = tmpStr;
      tmpStr = 0;
   } /*If: want to expand header before adding header*/

   /*****************************************************\
   * Main Sec03 Sub03:
   *   - build the binSam header
   \*****************************************************/

   /*________________add_program_id_____________________*/
   tmpStr = &headerHeapStr[tmpSI];
   tmpStr +=
      cpStr_ulCp(
         &headerHeapStr[tmpSI],
         (signed char *) "@PG\tID:binSam\tPN:binSam\tVN:"
      );

   /*________________add_version_number_________________*/
   tmpStr += numToStr(tmpStr, def_year_bioTools);

   if(def_month_bioTools > 9)
      tmpStr += numToStr(tmpStr, def_month_bioTools);
   else
   { /*Else: add 0 before date (single digit)*/
      *tmpStr++ = '0';
      *tmpStr++ = def_month_bioTools + 48;
   } /*Else: add 0 before date (single digit)*/

   if(def_day_bioTools > 9)
      tmpStr += numToStr(tmpStr, def_day_bioTools);
   else
   { /*Else: add 0 before date (single digit)*/
      *tmpStr++ = '0';
      *tmpStr++ = def_day_bioTools + 48;
   } /*Else: add 0 before date (single digit)*/

   *tmpStr++ = '\t';

   /*________________add_the_input_commands_____________*/
   tmpStr +=
      cpStr_ulCp(tmpStr, (signed char *) "CL:binSam");

   if(pStrictBl)
      tmpStr +=
         cpStr_ulCp(tmpStr, (signed char *) " -strict");
   else
      tmpStr +=
         cpStr_ulCp(tmpStr, (signed char *) " -merge");

   if(pUnmapBl)
      tmpStr +=
         cpStr_ulCp(tmpStr, (signed char *) " -unmap");
   else
      tmpStr +=
         cpStr_ulCp(tmpStr, (signed char *) " -no-unmap");

   if(pSecondaryBl)
      tmpStr +=
         cpStr_ulCp(tmpStr,(signed char *) " -secondary");
   else
     tmpStr +=
      cpStr_ulCp(tmpStr,(signed char *) " -no-secondary");

   if(pSupplementalBl)
      tmpStr +=
         cpStr_ulCp(
            tmpStr,
            (signed char *) " -supplemental"
         );
   else
      tmpStr +=
         cpStr_ulCp(
            tmpStr,
            (signed char *) " -no-supplemental"
         );

   tmpStr+=cpStr_ulCp(tmpStr,(signed char *) " -prefix ");
   tmpStr += cpStr_ulCp(tmpStr, prefixStr);
   tmpStr += cpStr_ulCp(tmpStr, (signed char *) " -sam ");
   tmpStr += cpStr_ulCp(tmpStr, samFileStr);

   *tmpStr++ = str_endLine[0];
   if(str_endLine[1])
      *tmpStr++ = str_endLine[1];
   *tmpStr = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec04:
   ^   - bin reads
   ^   o main sec04 sub01:
   ^     - index references for merge +start loop + filter
   ^   o main sec04 sub02:
   ^     - bin references
   ^   o main sec04 sub03:
   ^     - get next read
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Main Sec04 Sub01:
   *   - index references for merge + start loop + filter
   \*****************************************************/

   if(! pStrictBl)
   { /*If: merging references*/
      indexHeapArySI =
         buildRefMergeIndex_samRef(&refStackST);

      if(! indexHeapArySI)
      { /*If: memory errors*/
         fprintf(
            stderr,
            "memory error indexing references%s",
            str_endLine
         );
         goto memErr_main_sec05;
      } /*If: memory errors*/
   } /*If: merging references*/

   seqSL = 1;
   while(! errSC)
   { /*Loop: bin reads*/
      if(! pUnmapBl && readStackST.flagUS & 4)
         goto nextRead_sec04_sub0x;
      if(! pSecondaryBl && readStackST.flagUS & 256)
         goto nextRead_sec04_sub0x;
      if(! pSupplementalBl && readStackST.flagUS & 2048)
         goto nextRead_sec04_sub0x;
 
      /**************************************************\
      * Main Sec04 Sub02:
      *   - bin references
      \**************************************************/

      errSC =
         pSamToRef_samRef(
            &readStackST,
            prefixStr,
            headerHeapStr,
            outTypeSC,
            pUnmapBl,
            &refStackST,
            indexHeapArySI
         ); /*bin reads*/

      if(errSC < 0)
      { /*If: had file error*/
         fprintf(
            stderr,
            "could not open file to print read %li",
            seqSL
         );
         fprintf(
            stderr,
            " in -sam %s%s",
            samFileStr,
            str_endLine
         );
         goto fileErr_main_sec05;
      } /*If: had file  error*/

      /**************************************************\
      * Main Sec04 Sub03:
      *   - get next read
      \**************************************************/

      nextRead_sec04_sub0x:;
         ++seqSL;
         errSC = get_samEntry(&readStackST, inFILE);
   } /*Loop: bin reads*/

   /*****************************************************\
   * Main Sec04 Sub04:
   *   - deal with errors
   \*****************************************************/

   if(inFILE != stdin)
      fclose(inFILE);
   inFILE = 0;

   free(indexHeapArySI);
   indexHeapArySI = 0;

   free(headerHeapStr);
   headerHeapStr = 0;

   freeStack_samEntry(&readStackST);
   freeStack_refs_samRef(&refStackST);

   if(errSC != def_EOF_samEntry)
   { /*If: had an error*/
      if(errSC == def_memErr_samEntry)
      { /*If: memory error*/
         fprintf(
            stderr,
            "memory error on read %li in -sam %s%s",
            seqSL,
            samFileStr,
            str_endLine
         );
         goto memErr_main_sec05;
      } /*If: memory error*/

      else
      { /*Else: file error*/
         fprintf(
            stderr,
            "file error on read %li in -sam %s%s",
            seqSL,
            samFileStr,
            str_endLine
         );
         goto fileErr_main_sec05;
      } /*Else: file error*/
   } /*If: had an error*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec05:
   ^   - clean up and return
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   errSC = 0;
   goto ret_main_sec05;

   fileErr_main_sec05:;
      errSC = 1;
      goto ret_main_sec05;
   memErr_main_sec05:;
      errSC = 2;
      goto ret_main_sec05;

   ret_main_sec05:;
      freeStack_samEntry(&readStackST);
      freeStack_refs_samRef(&refStackST);

      if(indexHeapArySI)
         free(indexHeapArySI);
      indexHeapArySI = 0;

      if(headerHeapStr)
         free(headerHeapStr);
      headerHeapStr = 0;

      if(! inFILE) ;
      else if(inFILE == stdin) ;
      else if(inFILE == stdout) ;
      else if(inFILE == stderr) ;
      else fclose(inFILE);
      inFILE = 0;

      return errSC;
} /*main*/

/*=======================================================\
: License:
: 
: Creative Commons Legal Code
: 
: CC0 1.0 Universal
: 
:     CREATIVE COMMONS CORPORATION IS NOT A LAW FIRM AND
:     DOES NOT PROVIDE LEGAL SERVICES. DISTRIBUTION OF
:     THIS DOCUMENT DOES NOT CREATE AN ATTORNEY-CLIENT
:     RELATIONSHIP. CREATIVE COMMONS PROVIDES THIS
:     INFORMATION ON AN "AS-IS" BASIS. CREATIVE COMMONS
:     MAKES NO WARRANTIES REGARDING THE USE OF THIS
:     DOCUMENT OR THE INFORMATION OR WORKS PROVIDED
:     HEREUNDER, AND DISCLAIMS LIABILITY FOR DAMAGES
:     RESULTING FROM THE USE OF THIS DOCUMENT OR THE
:     INFORMATION OR WORKS PROVIDED HEREUNDER.
: 
: Statement of Purpose
: 
: The laws of most jurisdictions throughout the world
: automatically confer exclusive Copyright and Related
: Rights (defined below) upon the creator and subsequent
: owner(s) (each and all, an "owner") of an original work
: of authorship and/or a database (each, a "Work").
: 
: Certain owners wish to permanently relinquish those
: rights to a Work for the purpose of contributing to a
: commons of creative, cultural and scientific works
: ("Commons") that the public can reliably and without
: fear of later claims of infringement build upon, modify,
: incorporate in other works, reuse and redistribute as
: freely as possible in any form whatsoever and for any
: purposes, including without limitation commercial
: purposes. These owners may contribute to the Commons to
: promote the ideal of a free culture and the further
: production of creative, cultural and scientific works,
: or to gain reputation or greater distribution for their
: Work in part through the use and efforts of others.
: 
: For these and/or other purposes and motivations, and
: without any expectation of additional consideration or
: compensation, the person associating CC0 with a Work
: (the "Affirmer"), to the extent that he or she is an
: owner of Copyright and Related Rights in the Work,
: voluntarily elects to apply CC0 to the Work and publicly
: distribute the Work under its terms, with knowledge of
: his or her Copyright and Related Rights in the Work and
: the meaning and intended legal effect of CC0 on those
: rights.
: 
: 1. Copyright and Related Rights. A Work made available
:    under CC0 may be protected by copyright and related
:    or neighboring rights ("Copyright and Related
:    Rights"). Copyright and Related Rights include, but
:    are not limited to, the following:
: 
:   i. the right to reproduce, adapt, distribute, perform,
:      display, communicate, and translate a Work;
:  ii. moral rights retained by the original author(s)
:      and/or performer(s);
: iii. publicity and privacy rights pertaining to a
:      person's image or likeness depicted in a Work;
:  iv. rights protecting against unfair competition in
:      regards to a Work, subject to the limitations in
:      paragraph 4(a), below;
:   v. rights protecting the extraction, dissemination,
:      use and reuse of data in a Work;
:  vi. database rights (such as those arising under
:      Directive 96/9/EC of the European Parliament and of
:      the Council of 11 March 1996 on the legal
:      protection of databases, and under any national
:      implementation thereof, including any amended or
:      successor version of such directive); and
: vii. other similar, equivalent or corresponding rights
:      throughout the world based on applicable law or
:      treaty, and any national implementations thereof.
: 
: 2. Waiver. To the greatest extent permitted by, but not
:    in contravention of, applicable law, Affirmer hereby
:    overtly, fully, permanently, irrevocably and
:    unconditionally waives, abandons, and surrenders all
:    of Affirmer's Copyright and Related Rights and
:    associated claims and causes of action, whether now
:    known or unknown (including existing as well as
:    future claims and causes of action), in the Work (i)
:    in all territories worldwide, (ii) for the maximum
:    duration provided by applicable law or treaty
:    (including future time extensions), (iii) in any
:    current or future medium and for any number of
:    copies, and (iv) for any purpose whatsoever,
:    including without limitation commercial, advertising
:    or promotional purposes (the "Waiver"). Affirmer
:    makes the Waiver for the benefit of each member of
:    the public at large and to the detriment of
:    Affirmer's heirs and successors, fully intending that
:    such Waiver shall not be subject to revocation,
:    rescission, cancellation, termination, or any other
:    legal or equitable action to disrupt the quiet
:    enjoyment of the Work by the public as contemplated
:    by Affirmer's express Statement of Purpose.
: 
: 3. Public License Fallback. Should any part of the
:    Waiver for any reason be judged legally invalid or
:    ineffective under applicable law, then the Waiver
:    shall be preserved to the maximum extent permitted
:    taking into account Affirmer's express Statement of
:    Purpose. In addition, to the extent the Waiver is so
:    judged Affirmer hereby grants to each affected person
:    a royalty-free, non transferable, non sublicensable,
:    non exclusive, irrevocable and unconditional license
:    to exercise Affirmer's Copyright and Related Rights
:    in the Work (i) in all territories worldwide, (ii)
:    for the maximum duration provided by applicable law
:    or treaty (including future time extensions), (iii)
:    in any current or future medium and for any number of
:    copies, and (iv) for any purpose whatsoever,
:    including without limitation commercial, advertising
:    or promotional purposes (the "License"). The License
:    shall be deemed effective as of the date CC0 was
:    applied by Affirmer to the Work. Should any part of
:    the License for any reason be judged legally invalid
:    or ineffective under applicable law, such partial
:    invalidity or ineffectiveness shall not invalidate
:    the remainder of the License, and in such case
:    Affirmer hereby affirms that he or she will not (i)
:    exercise any of his or her remaining Copyright and
:    Related Rights in the Work or (ii) assert any
:    associated claims and causes of action with respect
:    to the Work, in either case contrary to Affirmer's
:    express Statement of Purpose.
: 
: 4. Limitations and Disclaimers.
: 
:  a. No trademark or patent rights held by Affirmer are
:     waived, abandoned, surrendered, licensed or
:     otherwise affected by this document.
:  b. Affirmer offers the Work as-is and makes no
:     representations or warranties of any kind concerning
:     the Work, express, implied, statutory or otherwise,
:     including without limitation warranties of title,
:     merchantability, fitness for a particular purpose,
:     non infringement, or the absence of latent or other
:     defects, accuracy, or the present or absence of
:     errors, whether or not discoverable, all to the
:     greatest extent permissible under applicable law.
:  c. Affirmer disclaims responsibility for clearing
:     rights of other persons that may apply to the Work
:     or any use thereof, including without limitation any
:     person's Copyright and Related Rights in the Work.
:     Further, Affirmer disclaims responsibility for
:     obtaining any necessary consents, permissions or
:     other rights required for any use of the Work.
:  d. Affirmer understands and acknowledges that Creative
:     Commons is not a party to this document and has no
:     duty or obligation with respect to this CC0 or use
:     of the Work.
\=======================================================*/
