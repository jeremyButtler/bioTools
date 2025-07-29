/*-------------------------------------------------------\
' leftAln SOF: Start Of File
'   - left aligns indels in homopolymers
'   o header:
'     - included libraries
'   o fun01: pversion_leftAln
'     -  prints bioTools version leftAln was compiled with
'   o fun02: phelp_leftAln
'     -  prints help message for leftAln
'   o fun03: input_leftAln
'     - gets input for leftAln
'   o main:
'     - driver function to shift homopolymer indels
\-------------------------------------------------------*/

/*-------------------------------------------------------\
| Header:
|   - included libraries
\-------------------------------------------------------*/

#ifdef PLAN9
   #include <u.h>
   #include <libc.h>
#else
   #include<stdlib.h>
#endif

#include <stdio.h>

#include "../genLib/ulCp.h"
#include "../genBio/seqST.h"
#include "../genBio/samEntry.h"
#include "../genBio/rmHomo.h"

/*.h files only*/
#include "../bioTools.h"
#include "../genLib/endLine.h"

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\
! Hidden Libraries:
!   - std #include <stdio.h>
!   - .c  #include "../genLib/base10str.h"
!   - .c  #include "../genLib/numToStr.h"
!   - .c  #include "../genLib/fileFun.h"
!   - .h  #include "../genBio/ntTo5Bit.h"
\%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

/*-------------------------------------------------------\
| Fun01: pversion_leftAln
|   -  prints bioTools version leftAln was compiled with
| Input:
|   - outFILE:
|     o FILE pointer to file to print version number to
| Output:
|   - Prints:
|     o version number to outFILE
\-------------------------------------------------------*/
void
pversion_leftAln(
   void *outFILE
){
   fprintf(
      (FILE *) outFILE,
      "leftAln from bioTools version: %i-%02i-%02i%s",
      def_year_bioTools,
      def_month_bioTools,
      def_day_bioTools,
      str_endLine
   );
} /*pversion_leftAln*/

/*-------------------------------------------------------\
| Fun02: phelp_leftAln
|   -  prints help message for leftAln
| Input:
|   - outFILE:
|     o FILE pointer to file to print help message to
| Output:
|   - Prints:
|     o help message to outFILE
\-------------------------------------------------------*/
void
phelp_leftAln(
   void *outFILE
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun02 TOC:
   '   -  prints help message for leftAln
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
      "leftAln -ref ref.fa -sam file.sam > leftAln.sam%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "  - Shifts homopolymer insertions and deletions%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "    to the left (start) of the homopolymer%s",
      str_endLine
   );

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun02 Sec02:
   ^   - input entry
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   fprintf(
      (FILE *) outFILE,
      "Input:%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "  -ref reference.fasta [Required]:%s",
      str_endLine
   );
   fprintf(
      (FILE *) outFILE,
      "    o reference sequence reads were mapped to%s",
      str_endLine
   );
   fprintf(
      (FILE *) outFILE,
      "    o use `-ref -` for stdin%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "  -sam file.sam [Required; stdin]:%s",
      str_endLine
   );
   fprintf(
      (FILE *) outFILE,
      "    o sam file to left align indels in%s",
      str_endLine
   );
   fprintf(
      (FILE *) outFILE,
      "    o use `-sam -` for stdin%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "  -out output-file.sam [Optional]:%s",
      str_endLine
   );
   fprintf(
      (FILE *) outFILE,
      "    o file to save output to%s",
      str_endLine
   );
   fprintf(
      (FILE *) outFILE,
      "    o use `-out -` for stdout%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "  -h  print this help message and exit:%s",
      str_endLine
   );
   fprintf(
      (FILE *) outFILE,
      "  -v  print version number and exit:%s",
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
      "  - prints left aligned reads to stdout%s",
      str_endLine
   );
} /*phelp_leftAln*/

/*-------------------------------------------------------\
| Fun03: input_leftAln
|   - gets input for leftAln
| Input:
|   - argLenSI:
|     o number of arguments the user input
|   - argStrAry:
|     o c-string array with user input
|   - refStrPtr:
|     o pointer to asign reference file name to
|     o DO NOT FREE THIS
|   - samStrPtr:
|     o pointer to asign sam file name to
|     o DO NOT FREE THIS
|   - outStrPtr:
|     o pointer to asign output file name to
|     o DO NOT FREE THIS
| Output:
|   - Modifies:
|     o refStrPtr to point to reference file
|     o samStrPtr to point to sam file
|     o outStrPtr to point to output file
|   - Prints:
|     o help message or version number (if requested) to
|       stdout
|     o error message to stderr
|   - Returns:
|     o 0 for no errors
|     o 1 if printed help message or version number
|     o 2 if had error
\-------------------------------------------------------*/
signed char
input_leftAln(
   signed int argLenSI,
   char *argAryStr[],
   signed char **refStrPtr,
   signed char **samStrPtr,
   signed char **outStrPtr
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun03 TOC:
   '   - gets input for leftAln
   '   o fun03 sec01:
   '     - variable declarations and check if have input
   '   o fun03 sec02:
   '     - get input
   '   o fun03 sec03:
   '     - return
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec01:
   ^   - variable declarations and check if have input
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   signed int siArg = 1;

   if(argLenSI <= 1)
      goto pHelp_fun03_sec03;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec02:
   ^   - get input
   ^   o fun03 sec02 sub01:
   ^     - get file input + start loop
   ^   o fun03 sec02 sub02:
   ^     - help  message checks
   ^   o fun03 sec02 sub03:
   ^     - version number checks
   ^   o fun03 sec02 sub04:
   ^     - invalid input
   ^   o fun03 sec02 sub05:
   ^     - move to next item in input
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun03 Sec02 Sub01:
   *   - get file input + start loop
   \*****************************************************/

   while(siArg < argLenSI)
   { /*Loop: get user input*/

      if(
         ! eqlNull_ulCp(
            (signed char *) "-ref",
            (signed char *) argAryStr[siArg]
         )
      ){ /*If: reference file input*/
         ++siArg;
         *refStrPtr = (signed char *) argAryStr[siArg];
      }  /*If: reference file input*/

      else if(
         ! eqlNull_ulCp(
            (signed char *) "-sam",
            (signed char *) argAryStr[siArg]
         )
      ){ /*Else If: sam file input*/
         ++siArg;
         *samStrPtr = (signed char *) argAryStr[siArg];
      }  /*Else If: same file input*/

      else if(
         ! eqlNull_ulCp(
            (signed char *) "-out",
            (signed char *) argAryStr[siArg]
         )
      ){ /*Else If: sam file input*/
         ++siArg;
         *outStrPtr = (signed char *) argAryStr[siArg];
      }  /*Else If: same file input*/

      /**************************************************\
      * Fun03 Sec02 Sub02:
      *   - help  message checks
      \**************************************************/

      else if(
         ! eqlNull_ulCp(
            (signed char *) "-h",
            (signed char *) argAryStr[siArg]
         )
      ) goto pHelp_fun03_sec03;

      else if(
         ! eqlNull_ulCp(
            (signed char *) "--h",
            (signed char *) argAryStr[siArg]
         )
      ) goto pHelp_fun03_sec03;

      else if(
         ! eqlNull_ulCp(
            (signed char *) "help",
            (signed char *) argAryStr[siArg]
         )
      ) goto pHelp_fun03_sec03;

      else if(
         ! eqlNull_ulCp(
            (signed char *) "-help",
            (signed char *) argAryStr[siArg]
         )
      ) goto pHelp_fun03_sec03;

      else if(
         ! eqlNull_ulCp(
            (signed char *) "--help",
            (signed char *) argAryStr[siArg]
         )
      ) goto pHelp_fun03_sec03;

      /**************************************************\
      * Fun03 Sec02 Sub03:
      *   - version number checks
      \**************************************************/

      else if(
         ! eqlNull_ulCp(
            (signed char *) "-v",
            (signed char *) argAryStr[siArg]
         )
      ) goto pVersion_fun03_sec03;

      else if(
         ! eqlNull_ulCp(
            (signed char *) "--v",
            (signed char *) argAryStr[siArg]
         )
      ) goto pVersion_fun03_sec03;

      else if(
         ! eqlNull_ulCp(
            (signed char *) "version",
            (signed char *) argAryStr[siArg]
         )
      ) goto pVersion_fun03_sec03;

      else if(
         ! eqlNull_ulCp(
            (signed char *) "-version",
            (signed char *) argAryStr[siArg]
         )
      ) goto pVersion_fun03_sec03;

      else if(
         ! eqlNull_ulCp(
            (signed char *) "--version",
            (signed char *) argAryStr[siArg]
         )
      ) goto pVersion_fun03_sec03;

      /**************************************************\
      * Fun03 Sec02 Sub04:
      *   - invalid input
      \**************************************************/

      else
      { /*Else: input is not recongnized*/
         fprintf(
            stderr,
            "unkown input (%s)%s",
            argAryStr[siArg],
            str_endLine
         );

         goto err_fun03_sec03;
      } /*Else: input is not recongnized*/

      /**************************************************\
      * Fun03 Sec02 Sub05:
      *   - move to next item in input
      \**************************************************/

      ++siArg;
   } /*Loop: get user input*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec03:
   ^   - return
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   siArg = 0;
   goto ret_fun03_sec03;

   pHelp_fun03_sec03:;
      phelp_leftAln(stdout);
      siArg = 1;
      goto ret_fun03_sec03;

   pVersion_fun03_sec03:;
      pversion_leftAln(stdout);
      siArg = 1;
      goto ret_fun03_sec03;

   err_fun03_sec03:;
      siArg = 2;
      goto ret_fun03_sec03;

   ret_fun03_sec03:;
      return (signed char) siArg;
} /*input_leftAln*/

/*-------------------------------------------------------\
| Main:
|   - driver function to shift homopolymer indels
| Input:
|   - argLenSI:
|     o number arguments user input
|   - argAryStr:
|     o c-string array with user input
| Output:
|   - Prints:
|     o left shifted reads to stdout
|     o errors to stdout
|   - Returns:
|     o 0 for no errors
|     o 1 for error of some kind
\-------------------------------------------------------*/
signed int
main(
   signed int argLenSI,
   char *argAryStr[]
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Main TOC:
   '   - driver function to shift homopolymer indels
   '   o main sec01:
   '     - variable declarations
   '   o main sec02:
   '     - initalize and get input
   '   o main sec03:
   '     - left align reads
   '   o main sec04:
   '     - return
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   signed char *refStr = 0;
   signed char *samStr = 0;
   signed char *outStr = 0;

   signed char errSC = 0;

   struct samEntry samStackST;
   struct seqST refStackST;
   signed long readSL = 0;

   FILE *inFILE = 0;
   FILE *outFILE = stdout;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec02:
   ^   - initalize and get input
   ^   o main sec02 sub01:
   ^     - initialize
   ^   o main sec02 sub02:
   ^     - get input
   ^   o main sec02 sub03:
   ^     - get reference sequence
   ^   o main sec02 sub04:
   ^     - open output file
   ^   o main sec02 sub05:
   ^     - open sam file
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Main Sec02 Sub01:
   *   - initialize
   \*****************************************************/

   init_samEntry(&samStackST);
   init_seqST(&refStackST);

   if( setup_samEntry(&samStackST) )
      goto memErr_main_sec04;

   /*****************************************************\
   * Main Sec02 Sub02:
   *   - get input
   \*****************************************************/

   errSC =
      input_leftAln(
         argLenSI,
         argAryStr,
         &refStr,
         &samStr,
         &outStr
      );

   if(errSC)
   { /*If: had error*/
      --errSC;/*convert help/version (1) to no error (0)*/
      goto ret_main_sec04;
   } /*If: had error*/

   /*****************************************************\
   * Main Sec02 Sub03:
   *   - get reference sequence
   *   o main sec02 sub03 cat01:
   *     - open reference file
   *   o main sec02 sub03 cat02:
   *     - get reference sequence
   \*****************************************************/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Main Sec02 Sub03 Cat01:
   +   - open reference file
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   if(! refStr || *refStr == '-')
   { /*If: stdin input*/
      if(! samStr || *samStr == '-')
      { /*If: double stdin input*/
         fprintf(
            stderr,
            "both -ref and -sam are from stdin '-'%s",
            str_endLine
         );

         goto err_main_sec04;
      } /*If: double stdin input*/

      inFILE = stdin;
      refStr = (signed char *) "-";
   } /*If: stdin input*/

   else
   { /*Else: reference is from file*/
      inFILE = fopen((char *) refStr, "r");

      if(! inFILE)
      { /*If: could not open reference file*/
         fprintf(
            stderr,
            "unable to open -ref %s%s",
            refStr,
            str_endLine
         );

         goto err_main_sec04;
      } /*If: could not open reference file*/
   } /*Else: reference is from file*/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Main Sec02 Sub03 Cat02:
   +   - get reference sequence
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   errSC = getFa_seqST(inFILE, &refStackST);

   if(! errSC)
       ;
   else if(errSC != def_EOF_seqST)
   { /*If: could not read reference*/
      fprintf(
         stderr,
         "could not read reference from -ref %s%s",
         refStr,
         str_endLine
      );

      goto err_main_sec04;
   } /*If: could not read reference*/

   else
   { /*Else: hit EOF*/
      if(! refStackST.seqStr || ! *refStackST.seqStr)
      { /*If: no reference sequence*/
         fprintf(
            stderr,
            "no sequence or format error in -ref %s%s",
            refStr,
            str_endLine
         );

         goto err_main_sec04;
      } /*If: no reference sequence*/
   } /*Else: hit EOF*/

   if(inFILE != stdin)
      fclose(inFILE);
   inFILE = 0;

   /*****************************************************\
   * Main Sec02 Sub04:
   *   - open output file
   \*****************************************************/
   
   if(! outStr || *outStr == '-')
   { /*If: input is from stdin*/
      outFILE = stdout;
      outStr = (signed char *) "-";
   } /*If: input is from stdin*/

   else
   { /*Else: printing output to file*/
      outFILE = fopen((char *) samStr, "w");

      if(! outFILE)
      { /*If: could not open output file*/
         fprintf(
            stderr,
            "unable to open -out %s%s",
            outStr,
            str_endLine
         );

         goto err_main_sec04;
      } /*If: could not open output file*/
   } /*Else: printing output to file*/

   /*****************************************************\
   * Main Sec02 Sub05:
   *   - open sam file
   \*****************************************************/
   
   if(! samStr || *samStr == '-')
   { /*If: input is from stdin*/
      inFILE = stdin;
      samStr = (signed char *) "-";
   } /*If: input is from stdin*/

   else
   { /*Else: sam file is from file*/
      inFILE = fopen((char *) samStr, "r");

      if(! inFILE)
      { /*If: could not open sam file*/
         fprintf(
            stderr,
            "unable to open -sam %s%s",
            samStr,
            str_endLine
         );

         goto err_main_sec04;
      } /*If: could not open sam file*/
   } /*Else: sam file is from file*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec03:
   ^   - left align reads
   ^   o main sec03 sub01:
   ^     - read first line in the sam file
   ^   o main sec03 sub02:
   ^     - print header in sam file
   ^   o main sec03 sub03:
   ^     - print leftAln header
   ^   o main sec03 sub04:
   ^     - left align reads
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Main Sec03 Sub01:
   *   - read first line in the sam file
   \*****************************************************/

   errSC = get_samEntry(&samStackST, inFILE);

   if(! errSC)
      ;

   else if(errSC == def_EOF_samEntry)
   { /*Else If: empty file*/
      fprintf(
         stderr,
         "nothing in -sam %s%s",
         samStr,
         str_endLine
      );

      goto err_main_sec04;
   } /*Else If: empty file*/

   else if(errSC == def_memErr_samEntry)
   { /*Else If: memory error*/
      fprintf(
         stderr,
         "ran out of memory reading -sam %s%s",
         samStr,
         str_endLine
      );

      goto err_main_sec04;
   } /*Else If: memory error*/

   else
   { /*Else: file error*/
      fprintf(
         stderr,
         "-sam %s is not a sam file%s",
         samStr,
         str_endLine
      );

      goto err_main_sec04;
   } /*Else: file error*/

   /*****************************************************\
   * Main Sec03 Sub02:
   *   - print header in sam file
   \*****************************************************/

   while(! errSC && *samStackST.extraStr == '@')
   { /*Loop: get past the header*/
      p_samEntry(&samStackST, 0, outFILE);
      errSC = get_samEntry(&samStackST, inFILE);
   } /*Loop: get past the header*/

   if(! errSC)
      ;

   else if(errSC == def_EOF_samEntry)
   { /*Else If: file only had a header*/
      fprintf(
         stderr,
         "-sam %s has only header (no reads)%s",
         samStr,
         str_endLine
      );

      goto err_main_sec04;
   } /*Else If: file only had a header*/

   else if(errSC == def_memErr_samEntry)
   { /*Else If: memory error*/
      fprintf(
         stderr,
         "ran out of memory reading -sam %s%s",
         samStr,
         str_endLine
      );

      goto err_main_sec04;
   } /*Else If: memory error*/

   else
   { /*Else: file error*/
      fprintf(
         stderr,
         "-sam %s is not a sam file%s",
         samStr,
         str_endLine
      );

      goto err_main_sec04;
   } /*Else: file error*/

   /*****************************************************\
   * Main Sec03 Sub03:
   *   - print leftAln header
   \*****************************************************/

   fprintf(
      outFILE,
      "@PG\tID:leftAln\tPN:leftAln\tVN:%i-%02i-%02i",
      def_year_bioTools,
      def_month_bioTools,
      def_day_bioTools
   );

   fprintf(
      outFILE,
      "\tCL:leftAln -ref %s -sam %s -out %s%s",
      refStr,
      samStr,
      outStr,
      str_endLine
   );

   /*****************************************************\
   * Main Sec03 Sub04:
   *   - left align reads
   \*****************************************************/

   readSL = 0;

   while(! errSC)
   { /*Loop: left align reads*/
      ++readSL;

      if(
         leftAlnIndel_rmHomo(
            &samStackST,
            refStackST.seqStr
         )
      ){ /*If: memory error*/
         fprintf(
            stderr,
            "memory error aliging read %li%s",
            readSL,
            str_endLine
         );

         goto err_main_sec04;
      }  /*If: memory error*/

      p_samEntry(&samStackST, 0, outFILE);
      errSC = get_samEntry(&samStackST, inFILE);
   } /*Loop: left align reads*/


   if(errSC == def_EOF_samEntry)
      ;

   else if(! errSC)
      ;

   else if(errSC == def_memErr_samEntry)
   { /*Else If: memory error*/
      fprintf(
         stderr,
         "ran out of memory in -sam %s on read %li%s",
         samStr,
         readSL,
         str_endLine
      );

      goto err_main_sec04;
   } /*Else If: memory error*/

   else
   { /*Else: file error*/
      fprintf(
         stderr,
         "read %li in -sam %s is not a valid entry%s",
         readSL,
         samStr,
         str_endLine
      );

      goto err_main_sec04;
   } /*Else: file error*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec04:
   ^   - return
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   errSC = 0;
   goto ret_main_sec04;

   memErr_main_sec04:;
      fprintf(outFILE, "memory error%s", str_endLine);
      errSC = 1;
      goto ret_main_sec04;

   err_main_sec04:;
      errSC = 1;
      goto ret_main_sec04;

   ret_main_sec04:;
      freeStack_samEntry(&samStackST);
      freeStack_seqST(&refStackST);

      if(! inFILE) ;
      else if(inFILE == stdin) ;
      else if(inFILE == stdout) ;
      else if(inFILE == stderr) ;
      else
         fclose(inFILE);
      inFILE = 0;

      if(! outFILE) ;
      else if(outFILE == stdout) ;
      else if(outFILE == stdout) ;
      else if(outFILE == stderr) ;
      else
         fclose(outFILE);
      outFILE = 0;

      return errSC;
} /*main*/
