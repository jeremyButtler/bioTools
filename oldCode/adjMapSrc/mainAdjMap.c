/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' mainAdjMap SOF: Start Of File
'   - driver function for adjMap
'   o header:
'     - included libraries and definitions
'   o fun01: pversion_mainAdjMap
'     - prints the version number for adjMap
'   o fun02: phelp_mainAdjMap
'     - prints help message for adjMap
'   o fun03: input_mainAdjMap
'     - gets input for adjMap
'   o main:
'     - driver function for adjMap
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*-------------------------------------------------------\
| Header:
|   - included libraries and definitions
\-------------------------------------------------------*/

#ifdef PLAN9
   #include <u.h>
   #include <libc.h>
#else
   #include <stdlib.h>
#endif

#include <stdio.h>

#include "../genLib/ulCp.h"
#include "../genLib/charCp.h"

#include "../genBio/samEntry.h"

#include "../genAln/alnSet.h"

#include "adjMap.h"

/*no .h files*/
#include "../genLib/dataTypeShortHand.h"

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\
! Hidden libraries:
!   o std #include <stdio.h>
!   o .c  #include "../genLib/base10str.h"
!   o .c  #include "../genLib/numToStr.h"
!   o .c  #include "../genLib/strAry.h"
!   o .c  #include "../genBio/seqST.h"
!   o .c  #include "../genAln/indexToCoord.h"
!   o .c  #include "../genAln/dirMatrix.h"
!   o .c  #include "../genAln/needle.h"
!   o .c  #include "../genAln/water.h"
!   o .h  #include "../genBio/ntTo5Bit.h"
!   o .h  #include "../genLib/genMath.h" max/min .h macro
!   o .h  #include "../genAln/alnDefs.h"
\%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

#define def_year_mainAdjMap 2024
#define def_month_mainAdjMap 9
#define def_day_mainAdjMap 4

/*-------------------------------------------------------\
| Fun01: pversion_mainAdjMap
|   - prints the version number for adjMap
| Input:
|   - outFILE:
|     o file to print version number to
| Output:
|   - Prints:
|     o version number to outFILE
\-------------------------------------------------------*/
void
pversion_mainAdjMap(
   void *outFILE 
){
   fprintf(
      (FILE *) outFILE,
      "adjMap version: %i-%02i-%02i\n",
      def_year_mainAdjMap,
      def_month_mainAdjMap,
      def_day_mainAdjMap
   );
} /*pversion_mainAdjMap*/

/*-------------------------------------------------------\
| Fun02: phelp_mainAdjMap
|   - prints help message for adjMap
| Input:
|   - outFILE:
|     o file to print help message to
| Output:
|   - Prints:
|     o help message to outFILE
\-------------------------------------------------------*/
void
phelp_mainAdjMap(
   void *outFILE
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun02 TOC:
   '   - prints help message
   '   o fun02 sec01:
   '     - usage block
   '   o fun02 sec02:
   '     - input block
   '   o fun02 sec03:
   '     - output block
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun02 Sec01:
   ^   - usage block
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   fprintf(
      (FILE *) outFILE,
      "adjMap -sam reads.sam -ref ref.sam > re-map.sam\n"
   );

   fprintf(
      (FILE *) outFILE,
      "  - adjusts read mappings to an input reference\n"
   );

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun02 Sec01:
   ^   - input block
   ^   o fun02 sec01 sub01:
   ^     - input block header
   ^   o fun02 sec01 sub02:
   ^     - sam file input
   ^   o fun02 sec01 sub03:
   ^     - reference input
   ^   o fun02 sec01 sub04:
   ^     - output
   ^   o fun02 sec01 sub05:
   ^     - minimum percent score for softmasked regions
   ^   o fun02 sec01 sub06:
   ^     - help message and version
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun02 Sec01 Sub01:
   *   - input block header
   \*****************************************************/

   fprintf(
      (FILE *) outFILE,
      "Input:\n"
   );

   /*****************************************************\
   * Fun02 Sec01 Sub02:
   *   - sam file input
   \*****************************************************/

   fprintf(
      (FILE *) outFILE,
      "  -sam reads.sam: [Required]\n"
   );

   fprintf(
      (FILE *) outFILE,
      "    o sam file with reads to swap reference for\n"
   );

   fprintf(
      (FILE *) outFILE,
      "    o use \"-sam -\" for stdin\n"
   );

   /*****************************************************\
   * Fun02 Sec01 Sub03:
   *   - reference input
   \*****************************************************/

   fprintf(
      (FILE *) outFILE,
      "  -ref ref.sam: [Required]\n"
   );

   fprintf(
      (FILE *) outFILE,
      "    o sam file with reference to swap to\n"
   );

   fprintf(
      (FILE *) outFILE,
      "    o shold be mapped to same reference as reads\n"
   );

   fprintf(
      (FILE *) outFILE,
      "    o use \"-ref -\" for stdin\n"
   );

   /*****************************************************\
   * Fun02 Sec01 Sub04:
   *   - output
   \*****************************************************/

   fprintf(
      (FILE *) outFILE,
      "  -out re-amp.sam: [Optional; stdout]\n"
   );

   fprintf(
      (FILE *) outFILE,
      "    o file to save re-mapped reads to\n"
   );

   fprintf(
      (FILE *) outFILE,
      "    o use \"-out -\" for stdout\n"
   );

   /*****************************************************\
   * Fun02 Sec01 Sub05:
   *   - minimum percent score for softmasked regions
   \*****************************************************/


   fprintf(
      (FILE *) outFILE,
      "  -min-overlap %0.2f: [Optional; %0.2f]\n",
      def_minOverlapPerc_adjMap,
      def_minOverlapPerc_adjMap
   );

   fprintf(
      (FILE *) outFILE,
      "    o minimum percent overlap needed to adjust\n"
   );

   fprintf(
     (FILE *) outFILE,
     "    o (overlap end - overlap start) / aligned len\n"
   );

   /*****************************************************\
   * Fun02 Sec01 Sub06:
   *   - help message and version
   \*****************************************************/

   fprintf(
      (FILE *) outFILE,
      "  -h: print this help messge and exit\n"
   );

   fprintf(
      (FILE *) outFILE,
      "  -v: print version number and exit\n"
   );

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun02 Sec03:
   ^   - output block
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   fprintf(
      (FILE *) outFILE,
      "Output:\n"
   );

   fprintf(
      (FILE *) outFILE,
      "  - prints reads mapped to -ref to -out\n"
   );
} /*phelp_mainAdjMap*/

/*-------------------------------------------------------\
| Fun03: input_mainAdjMap
|   - gets input for adjMap
| Input:
|   - numArgsSI:
|     o number of arguments input
|   - argAryStr:
|     o c-string array of user input arguments
|   - readFileStr:
|     o c-string to point to input sam file with reads
|       (do not free)
|   - refFileStr:
|     o c-string to point to input sam file with new
|       reference (do not free)
|   - outFileStr:
|     o c-string to point to output file
|   - minPercF:
|     o pointer to float to hold min percent score
|   - minOverlapPercF:
|     o pointer to float to hold min percent overlap
| Output:
|   - Modifies:
|     o all input except numArgsSI and argAryStr to point
|       to or hold user input
|   - Prints:
|     o if requested, prints help message or version
|       number to stdout
|     o input errors to stderr
|   - Returns:
|     o 0 for no errors
|     o 1 if printed help message or version number
|     o 2 if had input error
\-------------------------------------------------------*/
signed char
input_mainAdjMap(
   int numArgsSI,
   char *argAryStr[],
   signed char **readFileStr,
   signed char **refFileStr,
   signed char **outFileStr,
   float *minOverlapPercF
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun03 TOC:
   '   - gets input for adjMap
   '   o fun03 sec01:
   '     - variable declarations
   '   o fun03 sec02:
   '     - check if have input
   '   o fun03 sec03:
   '     - get input
   '   o fun03 sec04:
   '     - return result
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   schar errSC = 0;
   sint siArg = 1;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec02:
   ^   - check if have input
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(numArgsSI <= 1)
   { /*If: have not input*/
      phelp_mainAdjMap(stdout);
      goto phelp_fun03_sec04;
   } /*If: have not input*/
   
   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec03:
   ^   - get input
   ^   o fun03 sec03 sub01:
   ^     - file IO + start loop
   ^   o fun03 sec03 sub02:
   ^     - min percent score input
   ^   o fun03 sec03 sub03:
   ^     - help message
   ^   o fun03 sec03 sub04:
   ^     - version number
   ^   o fun03 sec03 sub05:
   ^     - invalid input
   ^   o fun03 sec03 sub06:
   ^     - move to next argument
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun03 Sec03 Sub01:
   *   - file IO + start loop
   \*****************************************************/

   while(siArg < numArgsSI)
   { /*Loop: get user input*/
      if(
         ! eql_charCp(
            (schar *) "-sam",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*If: reads input*/
         ++siArg;
         *readFileStr = (schar *) argAryStr[siArg];
      } /*If: reads input*/


      else if(
         ! eql_charCp(
            (schar *) "-ref",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else If: reference input*/
         ++siArg;
         *refFileStr = (schar *) argAryStr[siArg];
      } /*Else If: reference input*/


      else if(
         ! eql_charCp(
            (schar *) "-out",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else If: output file input*/
         ++siArg;
         *outFileStr = (schar *) argAryStr[siArg];
      } /*Else If: output file input*/

      /**************************************************\
      * Fun03 Sec03 Sub02:
      *   - min percent score input
      \**************************************************/

      else if(
         ! eql_charCp(
            (schar *) "-min-overlap",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else If: min percent score was input*/
         ++siArg;
         *minOverlapPercF = atof(argAryStr[siArg]);
      } /*Else If: min percent score was input*/

      /**************************************************\
      * Fun03 Sec03 Sub03:
      *   - help message
      \**************************************************/

      else if(
         ! eql_charCp(
            (schar *) "-h",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else If: help message requested*/
         phelp_mainAdjMap(stdout);
         goto phelp_fun03_sec04;
      } /*Else If: help message requested*/

      else if(
         ! eql_charCp(
            (schar *) "--h",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else If: help message requested*/
         phelp_mainAdjMap(stdout);
         goto phelp_fun03_sec04;
      } /*Else If: help message requested*/

      else if(
         ! eql_charCp(
            (schar *) "help",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else If: help message requested*/
         phelp_mainAdjMap(stdout);
         goto phelp_fun03_sec04;
      } /*Else If: help message requested*/

      else if(
         ! eql_charCp(
            (schar *) "-help",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else If: help message requested*/
         phelp_mainAdjMap(stdout);
         goto phelp_fun03_sec04;
      } /*Else If: help message requested*/

      else if(
         ! eql_charCp(
            (schar *) "--help",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else If: help message requested*/
         phelp_mainAdjMap(stdout);
         goto phelp_fun03_sec04;
      } /*Else If: help message requested*/

      /**************************************************\
      * Fun03 Sec03 Sub04:
      *   - version number
      \**************************************************/

      else if(
         ! eql_charCp(
            (schar *) "-v",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else If: version number requested*/
         pversion_mainAdjMap(stdout);
         goto pversion_fun03_sec04;
      } /*Else If: version number requested*/

      else if(
         ! eql_charCp(
            (schar *) "--v",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else If: version number requested*/
         pversion_mainAdjMap(stdout);
         goto pversion_fun03_sec04;
      } /*Else If: version number requested*/

      else if(
         ! eql_charCp(
            (schar *) "version",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else If: version number requested*/
         pversion_mainAdjMap(stdout);
         goto pversion_fun03_sec04;
      } /*Else If: version number requested*/

      else if(
         ! eql_charCp(
            (schar *) "-version",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else If: version number requested*/
         pversion_mainAdjMap(stdout);
         goto pversion_fun03_sec04;
      } /*Else If: version number requested*/

      else if(
         ! eql_charCp(
            (schar *) "--version",
            (schar *) argAryStr[siArg],
            (schar) '\0'
         )
      ){ /*Else If: version number requested*/
         pversion_mainAdjMap(stdout);
         goto pversion_fun03_sec04;
      } /*Else If: version number requested*/

      /**************************************************\
      * Fun03 Sec03 Sub05:
      *   - invalid input
      \**************************************************/

      else
      { /*Else: invalid input*/
         fprintf(
            stderr,
            "%s is not recongnized\n",
            argAryStr[siArg]
         );

         goto err_fun03_sec04;
      } /*Else: invalid input*/

      /**************************************************\
      * Fun03 Sec03 Sub06:
      *   - move to next argument
      \**************************************************/

      ++siArg;
   } /*Loop: get user input*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec04:
   ^   - return result
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   errSC = 0;
   goto ret_fun03_sec04;

   phelp_fun03_sec04:;
   pversion_fun03_sec04:;
   errSC = 1;
   goto ret_fun03_sec04;

   err_fun03_sec04:;
   errSC = 2;
   goto ret_fun03_sec04;

   ret_fun03_sec04:;
   return errSC;
} /*input_mainAdjMap*/

/*-------------------------------------------------------\
| Main:
|   - driver function for adjMap
| Input:
|   - main function auto supplied
| Output:
|   - Prints:
|     o sam file with swapped reference
|     o any errors to stderr
\-------------------------------------------------------*/
int
main(
   int numArgsSI,
   char *argAryStr[]
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Main TOC:
   '   o main sec01:
   '     - variable declarations
   '   o main sec02:
   '     - initialize/setup, get input, and check input
   '   o main sec03:
   '     - print sam file header
   '   o main sec04:
   '     - adjust mapping coordinates
   '   o main sec05:
   '     - clean up
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   schar errSC = 0;        /*errors and return value*/

   schar *readFileStr = 0;
   schar *refFileStr = 0;
   schar *outFileStr = 0;
   float minOverlapPercF = def_minOverlapPerc_adjMap;

   schar *tmpStr = 0;
   ulong entryUL = 0;  /*read on*/

   schar *buffHeapStr = 0;
   ulong lenBuffUL = 0;
   struct samEntry readStackST;
   struct samEntry refStackST;

   schar refCmdStr[1 << 13]; /*around 8192*/
      /*stores all stored commands in sam file; I am
      `   thinking a sam file should not have more then
      `   2000 characters in program ids/commands
      */
   uint lenRefCmdUI = 0;

   struct alnSet alnSetStackST;

   FILE *samFILE = 0;
   FILE *outFILE = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec02:
   ^   - initialize/setup, get input, and check input
   ^   o main sec02 sub01:
   ^     - initialize varaibles
   ^   o main sec02 sub02:
   ^     - get input
   ^   o main sec02 sub03:
   ^     - setup variables (samEntry only)
   ^   o main sec02 sub04:
   ^     - get/check reference
   ^   o main sec02 sub05:
   ^     - open read sam file
   ^   o main sec02 sub06:
   ^     - open output file
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Main Sec02 Sub01:
   *   - initialize varaibles
   \*****************************************************/

   init_samEntry(&readStackST);
   init_samEntry(&refStackST);
   init_alnSet(&alnSetStackST);

   refCmdStr[0] = '\0';

   /*****************************************************\
   * Main Sec02 Sub02:
   *   - get input
   \*****************************************************/

   errSC =
      input_mainAdjMap(
         numArgsSI,
         argAryStr,
         &readFileStr,
         &refFileStr,
         &outFileStr,
         &minOverlapPercF
      );

   if(errSC)
   { /*If: error; help message, version, or real error*/
      --errSC; /*help/version goes to 0 (no error)*/
      goto cleanUp_main_sec05_sub03;
   } /*If: error; help message, version, or real error*/

   /*****************************************************\
   * Main Sec02 Sub03:
   *   - setup variables (samEntry only)
   \*****************************************************/

   errSC = setup_samEntry(&readStackST);

   if(errSC)
   { /*If: memory error*/
      fprintf(
         stderr,
         "memory error allocating read samEntry\n"
      );

      goto err_main_sec05_sub02;
   } /*If: memory error*/

   errSC = setup_samEntry(&refStackST);

   if(errSC)
   { /*If: memory error*/
      fprintf(
         stderr,
         "memory error allocating reference samEntry\n"
      );

      goto err_main_sec05_sub02;
   } /*If: memory error*/

   /*****************************************************\
   * Main Sec02 Sub04:
   *   - get/check reference
   *   o main sec02 sub04 cat01:
   *     - open reference file
   *   o main sec02 sub04 cat02:
   *     - get first line of reference file
   *   o main sec02 sub04 cat03:
   *     - get reference sequence (frist sequence in file)
   *   o main sec02 sub04 cat04:
   *     - check for errors and close file
   \*****************************************************/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Main Sec02 Sub04 Cat01:
   +   - open reference file
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   if(
         ! refFileStr
      || *refFileStr == '-'
   ){ /*If: reference is from stdin input*/
      if(
            ! readFileStr
         || *readFileStr == '-'
      ){ /*If: read file also from stdin*/
         fprintf(
            stderr,
            "both -ref and -sam from stdin\n"
         );

         goto err_main_sec05_sub02;
      } /*If: read file also from stdin*/

      samFILE = stdin;
   } /*If: reference is from stdin input*/

   else
   { /*Else: user provided a reference sam file*/
      samFILE =
         fopen(
            (char *) refFileStr,
            "r"
         );

      if(! samFILE)
      { /*If: could not open file*/
         fprintf(
            stderr,
            "could not open -ref %s\n",
            refFileStr
         );

         goto err_main_sec05_sub02;
      } /*If: could not open file*/
   } /*Else: user provided a reference sam file*/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Main Sec02 Sub04 Cat02:
   +   - get first line of reference file
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   errSC =
      get_samEntry(
         &refStackST,
         &buffHeapStr,
         &lenBuffUL,
         samFILE
      );

   if(errSC == def_EOF_samEntry)
   { /*If: nothing in file*/
      fprintf(
         stderr,
         "nothing in -ref %s\n",
         refFileStr
      );

      goto err_main_sec05_sub02;
   } /*If: nothing in file*/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Main Sec02 Sub04 Cat03:
   +   - get reference sequence (frist sequence in file)
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   while(! errSC)
   { /*Loop: get reference*/
      if(*refStackST.extraStr != '@')
         break;

      if(
            refStackST.extraStr[1] == 'P'
         && refStackST.extraStr[2] == 'G'
         && refStackST.extraStr[3] == '\t'
      ){ /*If: is program entry*/
         cpLen_ulCp(
            &refCmdStr[lenRefCmdUI],
            refStackST.extraStr,
            refStackST.lenExtraUI
         );

         lenRefCmdUI += refStackST.lenExtraUI;
         refCmdStr[lenRefCmdUI] = '\n';
         ++lenRefCmdUI;
         refCmdStr[lenRefCmdUI] = '\0';
      } /*If: is program entry*/

      errSC =
         get_samEntry(
            &refStackST,
            &buffHeapStr,
            &lenBuffUL,
            samFILE
         );

      if(errSC == def_EOF_samEntry)
      { /*If: no sequence in file*/
         fprintf(
            stderr,
            "-ref %s has no sequences\n",
            refFileStr
         );

         goto err_main_sec05_sub02;
      } /*If: no sequence in file*/
   } /*Loop: get reference*/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Main Sec02 Sub04 Cat04:
   +   - check for errors and close file
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   if(errSC == def_memErr_samEntry)
   { /*If: memory error*/
      fprintf(
         stderr,
         "could not open -ref %s\n",
         refFileStr
      );

      goto err_main_sec05_sub02;
   } /*If: memory error*/

   if(samFILE != stdin)
      fclose(samFILE);

   samFILE = 0;

   /*****************************************************\
   * Main Sec02 Sub05:
   *   - open read sam file
   \*****************************************************/

   if(
         ! readFileStr
      || *readFileStr == '-'
   ) samFILE = stdin;

   else
   { /*Else: user provided read sam file*/
      samFILE =
         fopen(
            (char *) readFileStr,
            "r"
         );

      if(! samFILE)
      { /*If: could not open file*/
         fprintf(
            stderr,
            "could not open -sam %s\n",
            readFileStr
         );

         goto err_main_sec05_sub02;
      } /*If: could not open file*/
   } /*Else: user provided read sam file*/

   /*****************************************************\
   * Main Sec02 Sub06:
   *   - open output file
   \*****************************************************/

   if(
         ! outFileStr
      || *outFileStr == '-'
   ) outFILE = stdout;

   else
   { /*Else: user provided output file*/
      outFILE =
         fopen(
            (char *) outFileStr,
            "w"
         );

      if(! samFILE)
      { /*If: could not open file*/
         fprintf(
            stderr,
            "could not open -out %s\n",
            outFileStr
         );

         goto err_main_sec05_sub02;
      } /*If: could not open file*/
   } /*Else: user provided output file*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec03:
   ^   - print sam file header
   ^   o main sec03 sub01:
   ^     - get first line in sam file
   ^   o main sec03 sub02:
   ^     - print headers (comments) in sam file
   ^   o main sec03 sub03:
   ^     - check for errors
   ^   o main sec03 sub04:
   ^     - print adjMap header
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Main Sec03 Sub01:
   *   - get first line in sam file
   \*****************************************************/

   errSC =
      get_samEntry(
         &readStackST,
         &buffHeapStr,
         &lenBuffUL,
         samFILE
      );

   if(errSC == def_EOF_samEntry)
   { /*If: nothing in file*/
      fprintf(
         stderr,
         "nothing in -sam %s\n",
         readFileStr
      );

      goto err_main_sec05_sub02;
   } /*If: nothing in file*/

   /*****************************************************\
   * Main Sec03 Sub02:
   *   - print headers (comments) in sam file
   \*****************************************************/

   while(! errSC)
   { /*Loop: print header in sam file*/
      if(readStackST.extraStr[0] != '@')
         break; /*no more header*/

      /*making sure this is the input reference entry*/
      if(
         ! eql_charCp(
           (schar *) "@SQ\t",
           readStackST.extraStr,
           (schar) '\t'
         )
      ){ /*If: on reference entry*/
         tmpStr = &readStackST.extraStr[7];

         while(*tmpStr++ > 32) ;

         *(tmpStr - 1) = '\0';

         if(
            ! eql_charCp(
               refStackST.refIdStr,
               &readStackST.extraStr[7],
               (schar) '\0'
            )
         ){ /*If: references match*/
            *tmpStr = '\t';

            p_samEntry(
               &readStackST,
               &buffHeapStr,
               &lenBuffUL,
               0, /*want new line at end*/
               outFILE
            ); /*print out the comment*/
         } /*If: references match*/
      } /*If: on reference entry*/

      else
      { /*Else: not reference entry; print*/
         p_samEntry(
            &readStackST,
            &buffHeapStr,
            &lenBuffUL,
            0, /*want new line at end*/
            outFILE
         ); /*print out the comment*/
      } /*Else: not reference entry; print*/

      errSC =
         get_samEntry(
            &readStackST,
            &buffHeapStr,
            &lenBuffUL,
            samFILE
         );
   } /*Loop: print header in sam file*/

   /*****************************************************\
   * Main Sec03 Sub03:
   *   - check for errors
   \*****************************************************/

   if(errSC)
   { /*If: error*/
      if(errSC == def_EOF_samEntry)
         fprintf(
            stderr,
            "no reads in -sam %s\n",
            readFileStr
         );
      else
         fprintf(
            stderr,
            "memory error reading header of -sam %s\n",
            readFileStr
         );

      goto err_main_sec05_sub02;
   } /*If: error*/

   /*****************************************************\
   * Main Sec03 Sub04:
   *   - print adjMap header
   *   o main sec03 sub04 cat01:
   *     - printer program version and name
   *   o main sec03 sub04 cat02:
   *     - file input (reads/reference) part of header
   *   o main sec03 sub04 cat03:
   *     - file output part of header
   \*****************************************************/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Main Sec03 Sub04 Cat01:
   +   - printer program version and name
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "%s",
      refCmdStr
   ); /*keep the reference command(s)*/

   fprintf(
      (FILE *) outFILE,
      "@PG\tID:adjMap\tPN:adjMap\tVN:%i-%02i-%02i",
      def_year_mainAdjMap,
      def_month_mainAdjMap,
      def_day_mainAdjMap
   );

   fprintf(
      (FILE *) outFILE,
      "\tCL:adjMap"
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Main Sec03 Sub04 Cat02:
   +   - file input (reads/reference) part of header
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   if(
         ! readFileStr
      || *readFileStr == '-'
   ){ /*If: reads were from stdin*/
      fprintf(
         (FILE *) outFILE,
         " -sam - -ref %s",
         refFileStr
      );
   } /*If: reads were from stdin*/

   else if(
         ! refFileStr
      || *refFileStr == '-'
   ){ /*Else If: reference was from stdin*/
      fprintf(
         (FILE *) outFILE,
         " -sam %s -ref -",
         readFileStr
      );
   } /*Else If: reference was from stdin*/

   else
   { /*Else: no stdin input*/
      fprintf(
         (FILE *) outFILE,
         " -sam %s -ref %s",
         readFileStr,
         refFileStr
      );
   } /*Else: no stdin input*/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Main Sec03 Sub04 Cat03:
   +   - file output part of header
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   if(
         ! outFileStr
      || *outFileStr == '-'
   ){ /*If: output is to stdout*/
      fprintf(
         (FILE *) outFILE,
         " -out -"
      );
   } /*If: output is to stdout*/

   else
   { /*Else: output file input*/
      fprintf(
         (FILE *) outFILE,
         " -out %s",
         outFileStr
      );
   } /*Else: output file input*/

   fprintf(
      (FILE *) outFILE,
      "\n"
   );

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec04:
   ^   - adjust mapping coordinates
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   while(! errSC)
   { /*Loop: adjust mapping coordinates*/
      ++entryUL;

      errSC =
         adjMap(
            &readStackST,
            &refStackST,
            minOverlapPercF,
            &alnSetStackST
         );

      if(! errSC)
      { /*If: no errors*/
         p_samEntry(
            &readStackST,
            &buffHeapStr,
            &lenBuffUL,
            0,
            outFILE
         ); /*print adjusted read*/
      } /*If: no errors*/

      else
      { /*Else: had an error*/
         if(errSC == def_memErr_adjMap)
         { /*If: memory error (real error)*/
            fprintf(
               stderr,
               "memory error on read %lu in -sam %s\n",
               entryUL,
               readFileStr
            );

            goto err_main_sec05_sub02;
         } /*If: memory error (real error)*/

         /*else: read could not be mapped
         `       various reasons, not worth checking
         */
      } /*Else: had an error*/

      errSC =
         get_samEntry(
            &readStackST,
            &buffHeapStr,
            &lenBuffUL,
            samFILE
         ); /*get next read*/
   } /*Loop: adjust mapping coordinates*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec05:
   ^   - clean up
   ^   o main sec05 sub01:
   ^     - no error clean up
   ^   o main sec05 sub02:
   ^     - error clean up
   ^   o main sec05 sub03:
   ^     - general clean up (everything calls)
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Main Sec05 Sub01:
   *   - no error clean up
   \*****************************************************/

   errSC = 0;
   goto cleanUp_main_sec05_sub03;

   /*****************************************************\
   * Main Sec05 Sub02:
   *   - error clean up
   \*****************************************************/

   err_main_sec05_sub02:;
   errSC = -1;
   goto cleanUp_main_sec05_sub03;

   /*****************************************************\
   * Main Sec05 Sub03:
   *   - general clean up (everything calls)
   \*****************************************************/

   cleanUp_main_sec05_sub03:;

   if(buffHeapStr)
      free(buffHeapStr);

   buffHeapStr = 0;
   lenBuffUL = 0;

   freeStack_samEntry(&refStackST);
   freeStack_samEntry(&readStackST);
   freeStack_alnSet(&alnSetStackST);

   if(
         samFILE
      && samFILE != stdin
      && samFILE != stdout
   ) fclose(samFILE);

   samFILE = 0;

   if(
         outFILE
      && outFILE != stdin
      && outFILE != stdout
   ) fclose(outFILE);

   samFILE = 0;

   return errSC;
} /*main*/
