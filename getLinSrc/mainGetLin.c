/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' mainGetLin SOF: Start Of File
'   - driver script to find lineages in mapped reads
'   o header:
'     - included libraries and defaults
'   o fun01: pversion_mainGetLin
'     - prints the version number for getLin
'   o fun02: phelp_mainGetLin
'     - prints the help message for getLin
'   o fun03: input_mainGetLin
'     - get input for getLin
'   o main:
'     - driver function for getLin
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*-------------------------------------------------------\
| Header:
|   - inlucded libraries and defaults
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
#include "../genAln/kmerFind.h"
#include "../genGeneoType/getLin.h"
#include "../genGeneoType/linST.h"

/*.h files*/
#include "../genLib/endLine.h"
#include "../bioTools.h" /*version number*/

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\
! Hidden Libraries:
!   - .c  #include "../genLib/base10str.h"
!   - .c  #include "../genLib/numToStr.h"
!   - .c  #include "../genLib/shellSort.h"
!   - .c  #include "../genLib/fileFun.h"
!   - .c  #include "../genBio/seqST.h"
!   - .c  #include "../genBio/codonFun.h"
!   - .c  #include "../genAln/indexToCoord.h"
!   - .c  #include "../genAln/alnSet.h"
!   - .c  #include "../genAln/memwater.h"
!   - .c  #include "../genAln/memwaterScan.h"
!   - .c  #include "../genAln/kmerFind.h"
!   - .h  #include "../genLib/genMath.h"
!   - .h  #include "../genBio/ntTo5Bit.h"
!   - .h  #include "../genBio/codonTbl.h"
!   - .h  #include "../genBio/ntTo2Bit.h"
!   - .h  #include "../genBio/revNtTo2Bit.h"
!   - .h  #include "../genBio/kmerBit.h"
!   - .h  #include "../genAln/alnDefs.h"
\%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

#define def_minDepth_mainGetLin 10
#define def_maxMixedInfectSup_mainGetLin 0.4f
#define def_readOutput_mainGetLin 1

signed char *glob_outIdStr = (signed char *) "out";

/*-------------------------------------------------------\
| Fun01: pversion_mainGetLin
|   - prints the version number for getLin
| Input:
|   - outFILE:
|     o FILE pointer to print the version number to
| Output:
|   - Prints:
|     o version number to outFILE
\-------------------------------------------------------*/
void
pversion_mainGetLin(
   void *outFILE
){
   fprintf(
      (FILE *) outFILE,
      "getLin from bioTools version: %i-%02i-%02i%s",
      def_year_bioTools,
      def_month_bioTools,
      def_day_bioTools,
      str_endLine
   );
} /*pversion_mainGetLin*/

/*-------------------------------------------------------\
| Fun02: phelp_mainGetLin
|   - prints the help message for getLin
| Input:
|   - outFILE:
|     o file to print the help message to
| Output:
|   - Prints:
|     o help message to outFILE
\-------------------------------------------------------*/
void
phelp_mainGetLin(
   void *outFILE
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun02 TOC:
   '   - prints the help message for getLin
   '   o fun02 sec01:
   '     - usage entry
   '   o fun02 sec02:
   '     - input
   '   o fun02 sec03:
   '     - output
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun02 Sec01:
   ^   - usage entry
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   fprintf(
    (FILE *) outFILE,
    "getLin -simple simple-lineages.tsv -sam reads.sam%s",
    str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "  - detects lineages in mapped reads%s",
      str_endLine
   );

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun02 Sec02:
   ^   - input
   ^   o fun02 sec02 sub01:
   ^     - input header and simple database
   ^   o fun02 sec02 sub02:
   ^     - input header and simple database
   ^   o fun02 sec02 sub02:
   ^     - read input
   ^   o fun02 sec02 sub03:
   ^     - complex database
   ^   o fun02 sec02 sub04:
   ^     - output file
   ^   o fun02 sec02 sub05:
   ^     - output mode
   ^   o fun02 sec02 sub06:
   ^     - output id for entry
   ^   o fun02 sec02 sub07:
   ^     - minimum read depth
   ^   o fun02 sec02 sub08:
   ^     - maxiumum mixed infection print
   ^   o fun02 sec02 sub09:
   ^     - help message and version number
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun02 Sec02 Sub01:
   *   - input header and simple database
   \*****************************************************/

   fprintf((FILE *) outFILE, "Input:%s", str_endLine);

   fprintf(
      (FILE *) outFILE,
      "  -simple simple-lineages.tsv: [Required]%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "    o database (tsv file) with simple lineages%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "      (one variant per lineage) to look for%s",
      str_endLine
   );

   /*****************************************************\
   * Fun02 Sec02 Sub02:
   *   - read input
   \*****************************************************/

   fprintf(
      (FILE *) outFILE,
      "  -sam reads.sam: [Required]%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "    o sam file with reads mapped to the%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "      reference genome used to make the%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "      database provided with `-simple`%s",
      str_endLine
   );

   /*****************************************************\
   * Fun02 Sec02 Sub03:
   *   - complex database
   \*****************************************************/

   fprintf(
      (FILE *) outFILE,
      "  -complex complex-database.tsv: [Optional]%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "    o database with lineages made of multiple%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "      lineages from the database input by%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "      `-simple`%s",
      str_endLine
   );

   /*****************************************************\
   * Fun02 Sec02 Sub04:
   *   - output file
   \*****************************************************/

   fprintf(
      (FILE *) outFILE,
      "  -out file.tsv: [Optional; stdout]%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "    o file to print results to%s",
      str_endLine
   );
   fprintf(
      (FILE *) outFILE,
      "    o use `-out -` for stdout%s",
      str_endLine
   );

   /*****************************************************\
   * Fun02 Sec02 Sub05:
   *   - output mode
   \*****************************************************/

   if(def_readOutput_mainGetLin)
      fprintf(
         (FILE *) outFILE,
         "  -pmode-read: [Yes]%s",
         str_endLine
      );
   else
      fprintf(
         (FILE *) outFILE,
         "  -pmode-read: [No]%s",
         str_endLine
      );

   fprintf(
      (FILE *) outFILE,
      "    o print output in read mode%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "      * read mode prints read support for each%s",
      str_endLine
   );
   fprintf(
      (FILE *) outFILE,
      "        lineage and the consensus lineage%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "      * genome mode prints the lineage for each%s",
      str_endLine
   );
   fprintf(
      (FILE *) outFILE,
      "        mapped read%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "    o disable with `pmode-genome`%s",
      str_endLine
   );

   /*****************************************************\
   * Fun02 Sec02 Sub06:
   *   - output id for entry
   \*****************************************************/

   fprintf(
      (FILE *) outFILE,
      "  -id %s: [Optional]%s",
      glob_outIdStr,
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "    o id for `-pmode-read` (column 1 in output)%s",
      str_endLine
   );

   /*****************************************************\
   * Fun02 Sec02 Sub07:
   *   - minimum read depth
   \*****************************************************/

   fprintf(
      (FILE *) outFILE,
      "  -depth %i: [Optional]%s",
      def_minDepth_mainGetLin,
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "    o minimum read depth to print a lineage for%s",
      str_endLine
   );

   /*****************************************************\
   * Fun02 Sec02 Sub08:
   *   - maxiumum mixed infection print
   \*****************************************************/

   fprintf(
      (FILE *) outFILE,
      "  -mixed-sup %f: [Optional]%s",
      def_maxMixedInfectSup_mainGetLin,
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "    o maximum percentage to print a lineage for%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "      the consensus when a group as reads split%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "      between two lineages%s",
      str_endLine
   );

   /*****************************************************\
   * Fun02 Sec02 Sub09:
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
   ^   - output
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   fprintf((FILE *) outFILE, "Output:%s", str_endLine);

   fprintf(
      (FILE *) outFILE,
      "  - prints the lineages to stdout%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "    o warning: headers are not consisitent%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "    o for `pmode-genome` the header row and:%s",
      str_endLine
   );
   fprintf(
      (FILE *) outFILE,
      "    o lineage row is printed for each sequence%s",
      str_endLine
   );

   fprintf(
     (FILE *) outFILE,
     "    o for `pmode-read` the header row, depth row%s",
     str_endLine
   );
   fprintf(
      (FILE *) outFILE,
      "      and the consensus rows are printed%s",
      str_endLine
   );

   fprintf(
     (FILE *) outFILE,
     "    o formate of lineage row(s)%s",
     str_endLine
   );
   fprintf(
      (FILE *) outFILE,
      "      * column 1: genome/out id%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "      * column 2: `genome` for `-pmode-genome`,%s",
      str_endLine
   );
   fprintf(
      (FILE *) outFILE,
      "        `count` for `-pmode-read` counts row,%s",
      str_endLine
   );
   fprintf(
      (FILE *) outFILE,
      "        and `consnesus` for `-pmode-read`%s",
      str_endLine
   );
   fprintf(
      (FILE *) outFILE,
      "        consensus row%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "      * column 3 to n - 1: lineage(s) or depths%s",
      str_endLine
   );

   fprintf(
     (FILE *) outFILE,
     "      * column n ends in a `*` (header is `end`)%s",
     str_endLine
   );
} /*phelp_mainGetLin*/

/*-------------------------------------------------------\
| Fun03: input_mainGetLin
|   - get input for getLin
| Input:
|   - argLenSI:
|     o number of input arguments
|   - argAryStr:
|     o c-string array with user input
|   - simpleDbStrPtr:
|     o c-string pionter to get the simple database
|   - complexDbStrPtr:
|     o c-string pionter to get the complex database
|   - samStrPtr:
|     o c-string pionter to get the sam file with reads
|   - outFileStrPtr:
|     o c-string pionter to get the output file name
|   - idStrPtr:
|     o c-string pionter to get the output id
|   - readModeBlPtr:
|     o signed char pointer to be set to
|       * 1: if user is doing read mode
|       * 0: if user is doing sequence mode
|   - minDepthSIPtr:
|     o signed int pointer to get minimum read depth
|   - maxMixedFPtr:
|     o float ponter to get maximum mixed infection
|       support
| Output:
|   - Modifies:
|     o all input except argAryStr and argLenSI to have
|       user input
|   - Prints:
|     o help message or version number to stdout if
|       requested
|     o errors to stderr
|   - Returns:
|     o 0 for no errors
|     o 1 if printed the help message or version number
|     o 2 for input errors
\-------------------------------------------------------*/
signed char
input_mainGetLin(
   signed int argLenSI,      /*number of arguments input*/
   char *argAryStr[],        /*arguments input*/
   signed char **simpleDbStrPtr, /*simple lineages*/
   signed char **complexDbStrPtr,/*complex lineages*/
   signed char **samStrPtr,      /*sam file with reads*/
   signed char **outFileStrPtr,      /*gets output file*/
   signed char **idStrPtr,       /*output read mode id*/
   signed char *readModeBlPtr,   /*output mode to use*/
   signed int *minDepthSIPtr,    /*min read depth*/
   float *maxMixedFPtr           /*max mixed support*/
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun03 TOC:
   '   - gets input for getLin
   '   o fun03 sec01:
   '     - variable declarations and check if have input
   '   o fun03 sec02:
   '     - get input
   '   o fun03 sec03:
   '     - return results
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
   ^   - get input
   ^   o fun03 sec02 sub01:
   ^     - start loop + check for input/output files
   ^   o fun03 sec02 sub02:
   ^     - print mode settings (read/genome)
   ^   o fun03 sec02 sub03:
   ^     - minimum read depth
   ^   o fun03 sec02 sub04:
   ^     - max mixed infection support before not print
   ^   o fun03 sec02 sub05:
   ^     - help message
   ^   o fun03 sec02 sub06:
   ^     - version number
   ^   o fun03 sec02 sub07:
   ^     - deal with unkown input
   ^   o fun03 sec02 sub08:
   ^     - move to the next argument
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun03 Sec02 Sub01:
   *   - start loop + check for input/output files
   \*****************************************************/

   while(siArg < argLenSI)
   { /*Loop: get input*/
      tmpStr = (signed char *) argAryStr[siArg];

      if(! eqlNull_ulCp((signed char *) "-simple",tmpStr))
      { /*If: simple lineage database*/
         ++siArg;
         *simpleDbStrPtr=(signed char *) argAryStr[siArg];
      } /*If: simple lineage database*/

      else if(
         ! eqlNull_ulCp((signed char *) "-complex",tmpStr)
      ){ /*Else If: complex lineage database*/
        ++siArg;
        *complexDbStrPtr=(signed char *) argAryStr[siArg];
      } /*Else If: complex lineage database*/

      else if(
        ! eqlNull_ulCp((signed char *) "-sam", tmpStr)
      ){ /*Else If: sam file with reads*/
         ++siArg;
         *samStrPtr = (signed char *) argAryStr[siArg];
      } /*Else If: sam file with reads*/

      else if(
        ! eqlNull_ulCp((signed char *) "-out", tmpStr)
      ){ /*Else If: output file*/
         ++siArg;
         *outFileStrPtr = (signed char *) argAryStr[siArg];
      } /*Else If: output file*/

      else if(
        ! eqlNull_ulCp((signed char *) "-id", tmpStr)
      ){ /*Else If: id for read mode column*/
         ++siArg;
         *idStrPtr = (signed char *) argAryStr[siArg];
      }  /*Else If: id for read mode column*/

      /**************************************************\
      * Fun03 Sec02 Sub02:
      *   - check settings
      \**************************************************/

      else if(
        ! eqlNull_ulCp(
           (signed char *) "-pmode-read",
           tmpStr
        )
      ) *readModeBlPtr = 1;

      else if(
        ! eqlNull_ulCp(
           (signed char *) "-pmode-genome",
           tmpStr
        )
      ) *readModeBlPtr = 0;

      /**************************************************\
      * Fun03 Sec02 Sub03:
      *   - minimum read depth
      \**************************************************/

      else if(
        ! eqlNull_ulCp((signed char *) "-depth", tmpStr)
      ){ /*Else If: minimum read depth*/
         ++siArg;
         tmpStr = (signed char *) argAryStr[siArg];
         tmpStr+=strToSI_base10str(tmpStr, minDepthSIPtr);

         if(*tmpStr)
         { /*If: invalid input*/
            fprintf(
               stderr,
               "-depth %s; non-numeric or to large%s",
               argAryStr[siArg],
               str_endLine
            );

            goto err_fun03_sec03;
         } /*If: invalid input*/
      }  /*Else If: minimum read depth*/

      /**************************************************\
      * Fun03 Sec02 Sub04:
      *   - max mixed infection support before not print
      \**************************************************/

      else if(
       ! eqlNull_ulCp((signed char *) "-mixed-sup",tmpStr)
      ){ /*Else If: mixed support percent*/
         ++siArg;
         tmpStr = (signed char *) argAryStr[siArg];
         tmpStr += strToF_base10str(tmpStr, maxMixedFPtr);

         if(*tmpStr)
         { /*If: invalid input*/
            fprintf(
               stderr,
               "-mixed-sup %s; non-numeric or to large%s",
               argAryStr[siArg],
               str_endLine
            );

            goto err_fun03_sec03;
         } /*If: invalid input*/

         else if(*maxMixedFPtr > 100)
         { /*Else If: to large for percentage*/
            fprintf(
               stderr,
               "-mixed-sup %s; must be <= to 50%%%s",
               argAryStr[siArg],
               str_endLine
            );

            goto err_fun03_sec03;
         } /*Else If: to large for percentage*/

         else if(*maxMixedFPtr > 1)
            *maxMixedFPtr /= 100;/*convert to percentage*/

         if(*maxMixedFPtr > 0.5)
         { /*If: above 50% threshold*/
            fprintf(
               stderr,
               "-mixed-sup %s; must be <= to 50%%%s",
               argAryStr[siArg],
               str_endLine
            );

            goto err_fun03_sec03;
         } /*If: above 50% threshold*/
      }  /*Else If: mixed support percent*/

      /**************************************************\
      * Fun03 Sec02 Sub05:
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
      * Fun03 Sec02 Sub06:
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
      * Fun03 Sec02 Sub07:
      *   - deal with unkown input
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
      *   - move to the next argument
      \**************************************************/

      ++siArg;
   } /*Loop: get input*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec03:
   ^   - return results
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   siArg = 0;
   goto ret_fun03_sec03;

   phelp_fun03_sec03:;
      phelp_mainGetLin(stdout);
      siArg = 1;
      goto ret_fun03_sec03;
   pversion_fun03_sec03:;
      pversion_mainGetLin(stdout);
      siArg = 1;
      goto ret_fun03_sec03;
   err_fun03_sec03:;
      siArg = 2;
      goto ret_fun03_sec03;

   ret_fun03_sec03:;
      return (signed char) siArg;
} /*input_mainGetLin*/

/*-------------------------------------------------------\
| Main:
|   - driver function for getLin
| Input:
|   - argLenSI:
|     o number of arguments the user input
|   - argAryStr:
|     o c-string array with user input
| Output:
|   - Prints:
|     o lineages to `-out <file>` or stdout (`-out -`)
|     o errors to stderr
|   - Returns:
|     o 0 for no errors
|     o 1 for errors
\-------------------------------------------------------*/
int
main(
   int argLenSI,
   char *argAryStr[]
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Main TOC:
   '   - driver function for getLin
   '   o main sec01:
   '     - variable declarations
   '   o main sec02:
   '     - initialize, get input, and check and open input
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   signed char errSC = 0;
   signed char stdinSC = 0;   /*marks # open stdin files*/
   signed char *simpleDbStr = 0; /*simple database path*/
   signed char *complexDbStr = 0;/*complex database path*/
   signed char *samFileStr = 0;  /*sam file with reads*/
   signed char *outFileStr = 0;      /*output file*/
   signed char *idStr = glob_outIdStr; /*id for readmode*/

   signed char pReadModeBl = def_readOutput_mainGetLin;
   signed int minDepthSI = def_minDepth_mainGetLin;
   float mixedInfectSupF =
      def_maxMixedInfectSup_mainGetLin;

   signed char noFastBl = 0; /*add in as input later*/

   signed long seqSL = 0;
   signed long lineSL = 0;

   /*output variables for lineages*/
   signed int *simpleLinHeapArySI = 0;
   signed int *trsHeapArySI = 0;
   signed int simpleLenSI = 0;

   signed int *complexLinHeapArySI = 0;
   signed int complexLenSI = 0;

   /*structures need to work with*/
   struct samEntry readStackST;
   struct cnt_getLin cntStackST;
   struct tblST_kmerFind tblStackST;

   struct simple_linST *simpleHeapST = 0;
   struct complex_linST *complexHeapST = 0;

   /*file pointers*/
   FILE *outFILE = 0;
   FILE *inFILE = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec02:
   ^   - initialize, get input, and check and open input
   ^   o main sec02 sub01:
   ^     - initialize
   ^   o main sec02 sub02:
   ^     - get user input and setup
   ^   o main sec02 sub03:
   ^     - open the simple database
   ^   o main sec02 sub04:
   ^     - open the complex database (if provided)
   ^   o main sec02 sub05:
   ^     - open the input file
   ^   o main sec02 sub06:
   ^     - open the output file
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Main Sec02 Sub01:
   *   - initialize
   \*****************************************************/

   init_samEntry(&readStackST);
   init_cnt_getLin(&cntStackST);
   init_tblST_kmerFind(&tblStackST);

   /*****************************************************\
   * Main Sec02 Sub02:
   *   - get user input and setup
   \*****************************************************/

   errSC =
      input_mainGetLin(
         argLenSI,
         argAryStr,
         &simpleDbStr,
         &complexDbStr,
         &samFileStr,
         &outFileStr,
         &idStr,
         &pReadModeBl,
         &minDepthSI,
         &mixedInfectSupF
      );

   if(errSC)
   { /*If: printed help/version or had error*/
      --errSC;
         /*convert help/version print to 0 (no error)*/
      goto ret_main_sec0x;
   } /*If: printed help/version or had error*/

   if(
      setup_tblST_kmerFind(
        &tblStackST,
        def_lenKmer_kmerFind
      )
   ) goto memErr_main_sec0x;

   /*****************************************************\
   * Main Sec02 Sub03:
   *   - open the simple database
   *   o main sec02 sub03 cat01:
   *     - open the simple database file
   *   o main sec02 sub03 cat02:
   *     - read in the simple lineages
   \*****************************************************/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Main Sec02 Sub03 Cat01:
   +   - open the simple database file
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   if(! simpleDbStr)
   { /*If: no simple database input*/
      fprintf(
         stderr,
         "-simple has no input%s",
         str_endLine
      );
      goto fileErr_main_sec0x;
   } /*If: no simple database input*/

   else if(*simpleDbStr == '-')
   { /*Else If: stdin input*/
      ++stdinSC; /*mark I can no longer take input*/
      inFILE = stdin;
   } /*Else If: stdin input*/

   else
   { /*Else: file input*/
      inFILE = fopen((char *) simpleDbStr, "r");

      if(! inFILE)
      { /*If: no simple database*/
         fprintf(
            stderr,
            "could not open -simple%s%s",
            simpleDbStr,
            str_endLine
         );
         goto fileErr_main_sec0x;
         
      } /*If: no simple database*/
   } /*Else: file input*/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Main Sec02 Sub03 Cat02:
   +   - read in the simple lineages
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   simpleHeapST =
      getSimpleLineages_linST(
         noFastBl,
         &tblStackST,
         &lineSL,
         inFILE
      );
   if(inFILE != stdin)
      fclose(inFILE);
   inFILE = 0;

   if(! simpleHeapST)
   { /*If: failed to read in the simple database*/
      if(lineSL < 0)
      { /*If: file error for fast entry*/
         lineSL *= -1;
         fprintf(
            stderr,
            "for -simple %s%s",
            simpleDbStr,
            str_endLine
         );
         fprintf(
            stderr,
            "  trsFast, insFast, or countFast on line"
         );
         fprintf(
            stderr,
            " %li is amino acid sequence or to short%s",
            lineSL,
            str_endLine
         );
      } /*If: file error for fast entry*/

      else
      { /*Else: file or memory error*/
         fprintf(
            stderr,
            "error on line %li in -simple %s%s",
            lineSL,
            simpleDbStr,
            str_endLine
         );
      } /*Else: file or memory error*/

      goto fileErr_main_sec0x;
   } /*If: failed to read in the simple database*/

   /*****************************************************\
   * Main Sec02 Sub04:
   *   - open the complex database (if provided)
   *   o main sec02 sub04 cat01:
   *     - open the complex database file
   *   o main sec02 sub04 cat02:
   *     - read in the complex lineages
   \*****************************************************/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Main Sec02 Sub04 Cat01:
   +   - open the complex database file
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   if(! complexDbStr)
      goto openSamFile_main_sec02_sub05;
      /*not using the complex lineage database*/

   else if(*complexDbStr == '-' && stdinSC)
   { /*Else If: already have stdin input*/
      fprintf(
         stderr,
         "can not do `-simple -` and `-complex -`%s",
         str_endLine
      );
      fprintf(
         stderr,
         "  only one input can come from stdin (`-`)%s",
         str_endLine
      );
      goto fileErr_main_sec0x;
   } /*Else If: already have stdin input*/

   else if(*complexDbStr == '-')
   { /*Else If: stdin input*/
      inFILE = stdin;
      ++stdinSC;
   } /*Else If: stdin input*/

   else
   { /*Else: user input a file*/
      inFILE = fopen((char *) complexDbStr, "r");
      if(! inFILE)
      { /*If: could not open the file*/
         fprintf(
            stderr,
            "could not open `-complex %s`%s",
            complexDbStr,
            str_endLine
         );
         goto fileErr_main_sec0x;
      } /*If: could not open the file*/
   } /*Else: user input a file*/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Main Sec02 Sub04 Cat02:
   +   - read in the complex lineages
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   complexHeapST =
      getComplexLineages_linST(
         simpleHeapST,
         &lineSL,
         inFILE
      );
   if(inFILE != stdin)
      fclose(inFILE);
   inFILE = 0;

   if(! complexHeapST)
   { /*If: failed to read in the simple database*/
      if(lineSL < 0)
      { /*If: file error for fast entry*/
         lineSL *= -1;
         fprintf(
            stderr,
            "linege on line %li in `-complex %s`%s",
            lineSL,
            complexDbStr,
            str_endLine
         );
         fprintf(
            stderr,
            "  uses a lineage that does not exist%s",
            str_endLine
         );
      } /*If: file error for fast entry*/

      else
      { /*Else: file or memory error*/
         fprintf(
            stderr,
            "error on line %li in -complex %s%s",
            lineSL,
            complexDbStr,
            str_endLine
         );
      } /*Else: file or memory error*/

      goto fileErr_main_sec0x;
   } /*If: failed to read in the simple database*/

   /*****************************************************\
   * Main Sec02 Sub05:
   *   - open the input file
   \*****************************************************/

   openSamFile_main_sec02_sub05:;

   if((! samFileStr || *samFileStr == '-') && stdinSC)
   { /*If: reading two files from stdin*/
      if(*simpleDbStr == '-')
         fprintf(
            stderr,
            "can not do `-simple -` and `-sam -`%s",
            str_endLine
         );
      else
         fprintf(
            stderr,
            "can not do `-complex -` and `-sam -`%s",
            str_endLine
         );
      fprintf(
         stderr,
         "  only one input can come from stdin (`-`)%s",
         str_endLine
      );
      goto fileErr_main_sec0x;
   } /*If: reading two files from stdin*/

   else if(! samFileStr || *samFileStr == '-')
   { /*Else If: stdin input*/
      inFILE = stdin;
      ++stdinSC;
   } /*Else If: stdin input*/

   else
   { /*Else: user provided a file*/
      inFILE = fopen((char *) samFileStr, "r");

      if(! inFILE)
      { /*If: no input file*/
         fprintf(
            stderr,
            "could not open `-sam %s`%s",
            samFileStr,
            str_endLine
         );
         goto fileErr_main_sec0x;
      } /*If: no input file*/
   } /*Else: user provided a file*/

   /*****************************************************\
   * Main Sec02 Sub06:
   *   - open the output file
   \*****************************************************/

   if(! samFileStr || *samFileStr == '-')
      outFILE = stdin;

   else
   { /*Else: user provided a file*/
      inFILE = fopen((char *) outFileStr, "w");

      if(! inFILE)
      { /*If: no input file*/
         fprintf(
            stderr,
            "could not open `-out %s`%s",
            outFileStr,
            str_endLine
         );
         goto fileErr_main_sec0x;
      } /*If: no input file*/
   } /*Else: user provided a file*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec03:
   ^   - get lineages
   ^   o main sec03 sub01:
   ^     - get first read in the sam file
   ^   o main sec03 sub02:
   ^     - start loop + filter reads
   ^   o main sec03 sub03:
   ^     - get simple lineages
   ^   o main sec03 sub04:
   ^     - get complex lineages
   ^   o main sec03 sub05:
   ^     - add read lineage to count/print genome lineage
   ^   o main sec03 sub06:
   ^     - move to the next read
   ^   o main sec03 sub07:
   ^     - clean up and check for errors
   ^   o main sec03 sub08:
   ^     - for read mode, print read lineage + consensus
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Main Sec03 Sub01:
   *   - get first read in the sam file
   \*****************************************************/

   lineSL = 1;
   seqSL = 0;

   errSC = get_samEntry(&readStackST, inFILE);

   if(errSC)
   { /*If: had an error*/
      if(errSC == def_EOF_samEntry)
         fprintf(
            stderr,
            "nothing in `-sam %s`%s",
            samFileStr,
            str_endLine
         );
      else if(errSC == def_fileErr_samEntry)
         fprintf(
            stderr,
            "file error on line 1 of  `-sam %s`%s",
            samFileStr,
            str_endLine
         );
      else
      { /*Else: had memor error*/
         fprintf(
            stderr,
            "memory error on line 1 of  `-sam %s`%s",
            samFileStr,
            str_endLine
         );
         goto memErr_main_sec0x;
      } /*Else: had memor error*/

      goto fileErr_main_sec0x;
   } /*If: had an error*/

   /*****************************************************\
   * Main Sec03 Sub02:
   *   - start loop + filter reads
   \*****************************************************/

   while(! errSC)
   { /*Loop: get lineages*/
      if(readStackST.extraStr[0] == '@')
         goto nextRead_main_sec03_sub06; /*header*/

      if(readStackST.flagUS & (4 | 256 | 2048))
         goto nextRead_main_sec03_sub06;
           /*unmapped read (4), secondary alignment (256)
           `  or supplemental alignment (2048)
           */

      /**************************************************\
      * Main Sec03 Sub03:
      *   - get simple lineages
      \**************************************************/

      simpleLinHeapArySI =
         simpleLineage_getLin(
            &readStackST,
            simpleHeapST,
            &tblStackST,
            &trsHeapArySI,
            &simpleLenSI
         );

      if(! simpleLinHeapArySI)
      { /*If: error or no lineages*/
         if(simpleLenSI < 0)
         { /*If: had a memory error*/
            fprintf(
               stderr,
               "memory error getting simple lineages%s",
               str_endLine
            );
            goto memErr_main_sec0x;
         } /*If: had a memory error*/

         if(! pReadModeBl)
         { /*If: printing genomes*/
            fprintf(
               outFILE,
               "id\type\tend%s%s\tgenome\t*%s",
               readStackST.qryIdStr,
               str_endLine,
               str_endLine
            );
         } /*If: printing genomes*/

         goto nextRead_main_sec03_sub06; /*no lineages*/
      } /*If: error or no lineages*/

      /**************************************************\
      * Main Sec03 Sub04:
      *   - get complex lineages
      \**************************************************/

      if(complexHeapST)
      { /*If: have comlex lineages*/
         complexLinHeapArySI =
            complexLineage_getLin(
               complexHeapST,
               simpleLinHeapArySI,
               trsHeapArySI,
               &simpleLenSI,
               &complexLenSI
            );

         if(! complexLenSI && complexLenSI == -2)
         { /*If: memory error*/
            fprintf(
               stderr,
               "memory error getting complex lineage%s",
               str_endLine
            );
            goto memErr_main_sec0x;
         } /*If: memory error*/
      } /*If: have comlex lineages*/

      /**************************************************\
      * Main Sec03 Sub05:
      *   - add read lineage to count/print genome lineage
      \**************************************************/

      if(pReadModeBl)
      { /*If: finding lineages for mapped reads*/
         if(
            addReadLineages_cnt_getLin(
               &cntStackST,
               simpleLinHeapArySI,
               trsHeapArySI,
               simpleLenSI,
               complexLinHeapArySI,
               complexLenSI,
               simpleHeapST,
               complexHeapST
            )
         ){ /*If: had memory error*/
            fprintf(
               stderr,
               "memory error adding read lineages%s",
               str_endLine
            );
            goto memErr_main_sec0x;
         }  /*If: had memory error*/
      } /*If: finding lineages for mapped reads*/
 
      else
      { /*Else: working with genomes*/
         if(
            plineages_getLin(
               readStackST.qryIdStr,
               simpleLinHeapArySI,
               trsHeapArySI,
               simpleLenSI,
               complexLinHeapArySI,
               complexLenSI,
               simpleHeapST,
               complexHeapST,
               outFILE
            )
         ){ /*If: had memory error*/
            fprintf(
               stderr,
               "memory error printing genome lineages%s",
               str_endLine
            );
            goto memErr_main_sec0x;
         }  /*If: had memory error*/
      } /*Else: working with genomes*/

      /**************************************************\
      * Main Sec03 Sub06:
      *   - move to the next read
      \**************************************************/

      nextRead_main_sec03_sub06:;
         if(simpleLinHeapArySI)
            free(simpleLinHeapArySI);
         simpleLinHeapArySI = 0;

         if(trsHeapArySI)
            free(trsHeapArySI);
         trsHeapArySI = 0;

         if(complexLinHeapArySI)
            free(complexLinHeapArySI);
         complexLinHeapArySI = 0;

         errSC = get_samEntry(&readStackST, inFILE);
         ++lineSL;
         if(readStackST.extraStr[0] != '@')
            ++seqSL;
   } /*Loop: get lineages*/

   /*****************************************************\
   * Main Sec03 Sub07:
   *   - clean up and check for errors
   \*****************************************************/

   /*do some clean up*/
   if(inFILE != stdin)
      fclose(inFILE);
   inFILE = 0;

   freeStack_samEntry(&readStackST);
   freeStack_tblST_kmerFind(&tblStackST);

   /*deal with errors*/
   if(errSC != def_EOF_samEntry)
   { /*If: had error reading the sam file*/
      if(errSC == def_fileErr_samEntry)
      { /*If: file error*/
         fprintf(
            stderr,
            "file error on line %li (read %li) in",
            lineSL,
            seqSL
         );
         fprintf(
            stderr,
            " `-sam %s`%s",
            samFileStr,
            str_endLine
         );
         goto fileErr_main_sec0x;
      } /*If: file error*/

      else
      { /*Else:  memory error*/
         fprintf(
            stderr,
            "memory error on line %li (read %li) in",
            lineSL,
            seqSL
         );
         fprintf(
            stderr,
            " `-sam %s`%s",
            samFileStr,
            str_endLine
         );
         goto memErr_main_sec0x;
      } /*Else:  memory error*/
   } /*If: had error reading the sam file*/

   /*****************************************************\
   * Main Sec03 Sub08:
   *   - for read mode, print read lineage + consensus
   \*****************************************************/

   if(pReadModeBl)
   { /*If: printing read lineage*/
      if(
         pReadLineages_getLin(
            idStr,
            &cntStackST,
            minDepthSI,
            mixedInfectSupF,
            outFILE
         )
      ){ /*If: had memory error*/
         fprintf(
            stderr,
            "memory error printing read lineages%s",
            str_endLine
         );
         goto memErr_main_sec0x;
      }  /*If: had memory error*/
   } /*If: printing read lineage*/

   if(outFILE != stdout)
      fclose(outFILE);
   outFILE = 0;

   freeStack_cnt_getLin(&cntStackST);

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec0x:
   ^   - clean up and return
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   errSC = 0;
   goto ret_main_sec0x;

   fileErr_main_sec0x:;
      errSC = 1;
      goto ret_main_sec0x;
   memErr_main_sec0x:;
      errSC = 2;
      goto ret_main_sec0x;

   ret_main_sec0x:;
      freeStack_samEntry(&readStackST);
      freeStack_cnt_getLin(&cntStackST);
      freeStack_tblST_kmerFind(&tblStackST);

      if(simpleHeapST)
         freeHeap_simple_linST(simpleHeapST);
      simpleHeapST = 0;

      if(complexHeapST)
         freeHeap_complex_linST(complexHeapST);
      complexHeapST = 0;

      if(simpleLinHeapArySI)
         free(simpleLinHeapArySI);
      simpleLinHeapArySI = 0;

      if(trsHeapArySI)
         free(trsHeapArySI);
      trsHeapArySI = 0;

      if(complexLinHeapArySI)
         free(complexLinHeapArySI);
      complexLinHeapArySI = 0;

      if(! inFILE) ;
      else if(inFILE == stdin) ;
      else if(inFILE == stdout) ;
      else if(inFILE == stderr) ;
      else fclose(inFILE);
      inFILE = 0;

      if(! outFILE) ;
      else if(outFILE == stdin) ;
      else if(outFILE == stdout) ;
      else if(outFILE == stderr) ;
      else fclose(outFILE);
      outFILE = 0;

      return errSC;
} /*main*/
