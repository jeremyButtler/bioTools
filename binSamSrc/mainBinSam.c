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
'     - licensing for this code (public domain / mit)
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
   ^     - output prefix
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
   *   - output prefix
   \*****************************************************/

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
