/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' maskPrim SOF: Start Of File
'   - mask primers in an sequence using input coordinates
'   o header:
'     - included libraries and defined variables
'   o fun01: checkHelp_mainMaskPrim
'     - checks if input paramter is help message request
'   o fun02: checkVersion_mainMaskPrim
'     - checks if input paramter is an version request
'   o fun03: pversion_mainMaskPrim
'     - print version number for maskPrim
'   o fun04: phelp_mainMaskPrim
'     - print the help message for maskPrim
'   o fun05: input_mainMaskPrim
'     - processes user command line supplied input
'   o main:
'     - driver function to mask primers
'   o license:
'     - licensing for this code (CC0)
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*-------------------------------------------------------\
| Header:
|   - included libraries and defined variables
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

#include "../genBio/samEntry.h"
#include "../genBio/maskPrim.h"

/*No .c files (only .h)*/
#include "../genLib/endLine.h"
#include "../bioTools.h"

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\
! Hidden libraries
!   o .c  #include "../genLIb/numToStr.h"
!   o .c  #include "../genLib/ulCp.h"
!   o .c  #include "../genLib/shellSort.h"
!   o .c  #include "../genLib/fileFun.h"
!   o .h  #include "../genBio/ntTo5Bit.h"
\%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

/*Some default values*/
#define def_mask_mainMaskPrim 'N'  /*Char to mask with*/
#define def_fudge_mainMaskPrim -1  /*Fudge length*/
#define def_filter_mainMaskPrim 0  /*0 = do not filter*/
   /*-1 is skip/anyware*/

/*-------------------------------------------------------\
| Fun01: checkHelp_mainMaskPrim
|   - checks if input paramter is an help message request
| Input:
|   - parmStr:
|     o parameter to see if is an help message
| Output:
|   - Returns:
|     o 0 if not an help message request
|     o 1 if is an help message request
\-------------------------------------------------------*/
signed char
checkHelp_mainMaskPrim(
   signed char *parmStr
){
   signed char retSC = 0;

   if(
      ! eql_charCp(
         (signed char *) "-h",
         (parmStr),
         (signed char) '\0'
   )) retSC = 1;

   else if(
      ! eql_charCp(
         (signed char *) "--h",
         (parmStr),
         (signed char) '\0'
   )) retSC = 1;

   else if(
      ! eql_charCp(
         (signed char *) "help",
         (parmStr),
         (signed char) '\0'
   )) retSC = 1;

   else if(
      ! eql_charCp(
         (signed char *) "-help",
         (parmStr),
         (signed char) '\0'
   )) retSC = 1;

   else if(
      ! eql_charCp(
         (signed char *) "--help",
         (parmStr),
         (signed char) '\0'
   )) retSC = 1;

   return retSC;
} /*checkHelp_mainMaskPrim*/

/*-------------------------------------------------------\
| Fun02: checkVersion_mainMaskPrim
|   - checks if input paramter is an version request
| Input:
|   - parmStr:
|     o parameter to see if is an version requiest
| Output:
|   - Returns:
|     o 0 if not an verson request
|     o 1 if is an version request
\-------------------------------------------------------*/
signed char
checkVersion_mainMaskPrim(
   signed char *parmStr
){
   signed char retSC = 0;

   if(
      ! eql_charCp(
         (signed char *) "-v",
         (parmStr),
         (signed char) '\0'
   )) retSC = 1;

   else if(
      ! eql_charCp(
         (signed char *) "--v",
         (parmStr),
         (signed char) '\0'
   )) retSC = 1;

   if(
      ! eql_charCp(
         (signed char *) "-V",
         (parmStr),
         (signed char) '\0'
   )) retSC = 1;

   else if(
      ! eql_charCp(
         (signed char *) "--V",
         (parmStr),
         (signed char) '\0'
   )) retSC = 1;

   else if(
      ! eql_charCp(
         (signed char *) "version",
         (parmStr),
         (signed char) '\0'
   )) retSC = 1;

   else if(
      ! eql_charCp(
         (signed char *) "-version",
         (parmStr),
         (signed char) '\0'
   )) retSC = 1;

   else if(
      ! eql_charCp(
         (signed char *) "--version",
         (parmStr),
         (signed char) '\0'
   )) retSC = 1;

   return retSC; 
} /*checkVersion_mainMaskPrim*/

/*-------------------------------------------------------\
| Fun03: pversion_mainMaskPrim
|   - print version number for maskPrim
| Input:
|   - outFILE:
|     o file to print version number to
| Output:
|   - Prints:
|     o version number to outFILE
\-------------------------------------------------------*/
void
pversion_mainMaskPrim(
   void *outFILE
){
   fprintf(
      (FILE *) outFILE,
      "maskPrim from bioTools version: %i-%02i-%02i%s",
      def_year_bioTools,
      def_month_bioTools,
      def_day_bioTools,
      str_endLine
   );
} /*pversion_mainMaskPrim*/

/*-------------------------------------------------------\
| Fun04: phelp_mainMaskPrim
|   - print the help message for maskPrim
| Input:
|   - outFILE:
|     o file to print the help message to
| Output:
|   - Prints:
|     o the help messge to outFILE
\-------------------------------------------------------*/
void
phelp_mainMaskPrim(
   void *outFILE
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun04 TOC:
   '   - print the help message for maskPrim
   '   o fun04 sec01:
   '     - usage block
   '   o fun04 sec02:
   '     - input block
   '   o fun04 sec03:
   '     - output block
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun04 Sec01:
   ^   - usage block
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   fprintf(
     (FILE *) outFILE,
     "maskPrim -sam reads.sam -prim primer-coords.tsv%s",
     str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "  - Masks primer positions in reads%s",
      str_endLine
   );

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun04 Sec02:
   ^   - input block
   ^   o fun04 sec02 sub01:
   ^     - input block header
   ^   o fun04 sec02 sub02:
   ^     - sam file entry
   ^   o fun04 sec02 sub03:
   ^     - output entry
   ^   o fun04 sec02 sub04:
   ^     - primer coordinates entry
   ^   o fun04 sec02 sub05:
   ^     - mask character entry
   ^   o fun04 sec02 sub06:
   ^     - fudge length entry
   ^   o fun04 sec02 sub07:
   ^     - filtering
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun04 Sec02 Sub01:
   *   - input block header
   \*****************************************************/

   fprintf(
      (FILE *) outFILE,
      "Input:%s",
      str_endLine
   );

   /*****************************************************\
   * Fun04 Sec02 Sub02:
   *   - sam file entry
   \*****************************************************/

   fprintf(
      (FILE *) outFILE,
      "  -sam: [stdin]%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "    o Sam file with reads to mask primers in%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "    o use \"-\" for stdin input%s",
      str_endLine
   );

   /*****************************************************\
   * Fun04 Sec02 Sub03:
   *   - output entry
   \*****************************************************/

   fprintf(
      (FILE *) outFILE,
      "  -out: [stdout]%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "    o File to output masked sam file to%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "    o use \"-\" for stdout output%s",
      str_endLine
   );

   /*****************************************************\
   * Fun04 Sec02 Sub04:
   *   - primer coordinates entry
   \*****************************************************/

   fprintf(
      (FILE *) outFILE,
      "  -prim: [Required]%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "    o Tsv file with the primer coordinages%s",
      str_endLine
   );

   fprintf(
     (FILE *) outFILE,
     "      - First column is ingored (refernce id?)%s",
     str_endLine
   );

   fprintf(
     (FILE *) outFILE,
     "      - Second column is ingored (primer name?)%s",
     str_endLine
   );

   fprintf(
    (FILE *) outFILE,
    "      - Third column is 1 primers are paired or 0%s",
    str_endLine
   );

   fprintf(
     (FILE *) outFILE,
     "        o Paired = both primers must be present to"
   );

   fprintf((FILE *) outFILE,  " mask%s", str_endLine);

   fprintf(
    (FILE *) outFILE,
    "      - Fourth column is forward start coordinate%s",
    str_endLine
   );

   fprintf(
     (FILE *) outFILE,
     "        - use \"NA\" for no forward primer%s",
     str_endLine
   );

   fprintf(
     (FILE *) outFILE,
     "      - Fifth column is forward end coordinate%s",
     str_endLine
   );

   fprintf(
     (FILE *) outFILE,
     "      - Six column is reverse start coordinate%s",
     str_endLine
   );
   fprintf(
     (FILE *) outFILE,
     "        - use \"NA\" for no reverse primer%s",
     str_endLine
   );

   fprintf(
     (FILE *) outFILE,
     "      - Seventh column is reverse end coordinate%s",
     str_endLine
   );

   /*****************************************************\
   * Fun04 Sec02 Sub05:
   *   - mask character entry
   \*****************************************************/

   fprintf(
      (FILE *) outFILE,
      "  -mask: [%c]%s",
      def_mask_mainMaskPrim,
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "    o Chacter to mask with%s",
      str_endLine
   );

   /*****************************************************\
   * Fun04 Sec02 Sub06:
   *   - fudge length entry
   \*****************************************************/

   fprintf(
      (FILE *) outFILE,
      "  -fudge: [%i]%s",
      def_fudge_mainMaskPrim,
      str_endLine
   );

   fprintf(
     (FILE *) outFILE,
     "    o How much seqence can come before an primer%s",
     str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "    o -1 = mask regardless of position%s",
      str_endLine
   );

   /*****************************************************\
   * Fun04 Sec02 Sub07:
   *   - filtering
   \*****************************************************/

   if(def_filter_mainMaskPrim)
   { /*If: default is to filter*/
      fprintf(
         (FILE *) outFILE,
         "  -filter: [Yes]%s",
         str_endLine
      );
   } /*If: default is to filter*/

   else
   { /*If: default is to filter*/
      fprintf(
         (FILE *) outFILE,
         "  -filter: [No]%s",
         str_endLine
      );
   } /*If: default is to filter*/

   fprintf(
      (FILE *) outFILE,
      "    o Remove reads with no detected primers%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "    o Disable with -no-filter%s",
      str_endLine
   );

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun04 Sec03:
   ^   - output block
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/
    
   fprintf(
      (FILE *) outFILE,
      "Output:%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "  - Prints sam file with masked reads to -out%s",
      str_endLine
   );
} /*phelp_mainMaskPrim*/

/*-------------------------------------------------------\
| Fun05: input_mainMaskPrim
|   - processes user command line supplied input
| Input:
|   - numArgsSI:
|     o number of arguments the user input
|   - argAryStr:
|     o array of c-strings with the users input
|   - samFileStr:
|     o pointer to c-string to point to the input sam file
|       name
|   - primFileStr:
|     o pointer to c-string to point to the input primer
|       coorindates file name
|   - outFileStr:
|     o pointer to c-string to point to ouput file name
|   - maskSC:
|     o Pointer to char to hold charcter to mask with
|   - fudgeSI:
|     o int Pointer to hold the amount to fudge primer
|       start/end by
|   - filterBl:
|     o character set to 1 or 0 if doing filtering.
|       - 1 removing reads without primers
|       - 0 not removing reads without primers
| Output:
|   - Modifies:
|     o all input values except numArgsSI and argAryStr
|       to the user input.
|   - Returns:
|     o 0 for no errors
|     o def_help_maskPrim if help message requested
|     o def_version_maskPrim if version number requested
|     o def_nonNumeric | (arg_index << 8) for numeric
|       errors
|     o def_unkownInput | (arg_index << 8) if an argument
|       could not be recongnized
\-------------------------------------------------------*/
signed long
input_mainMaskPrim(
   signed int numArgsSI,
   char *argAryStr[],
   signed char **samFileStr,
   signed char **primFileStr,
   signed char **outFileStr,
   signed char *maskSC,
   signed int *fudgeSI,
   signed char *filterBl
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun05 TOC:
   '   - process user command line input
   '   o fun05 sec01:
   '     - variable declerations
   '   o fun05 sec02:
   '     - check if have input
   '   o fun05 sec03:
   '     - process user input
   '   o fun05 sec04:
   '     - return success or error
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun05 Sec01:
   ^   - variable declerations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   unsigned int uiArg = 1;
   signed char *tmpStr = 0;
   signed char errSC = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun05 Sec02:
   ^   - check if have input
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(numArgsSI <= 1)
   { /*If: nothing input*/
      phelp_mainMaskPrim(stdout);
      goto phelp_fun05_sec04;
   } /*If: nothing input*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun05 Sec03:
   ^   - process user input
   ^   o fun05 sec03 sub01:
   ^     - file io (input/output)
   ^   o fun05 sec03 sub02:
   ^     - masking and filter settings
   ^   o fun05 sec03 sub03:
   ^     - help message and version number
   ^   o fun05 sec03 sub04:
   ^     - invalid input
   ^   o fun05 sec03 sub05:
   ^     - move to next argument
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun05 Sec03 Sub01:
   *   - file io (input/output)
   \*****************************************************/

   while(uiArg < (unsigned int) numArgsSI)
   { /*Loop: Process each peice of user input*/
      if(
         ! eql_charCp(
            (signed char *) "-sam",
            (signed char *) argAryStr[uiArg],
            (signed char) '\0'
         )
      ){ /*If: sam file*/
         ++uiArg;
         *samFileStr = (signed char *) argAryStr[uiArg];
      } /*If: sam file*/

      else if(
         ! eql_charCp(
            (signed char *) "-prim",
            (signed char *) argAryStr[uiArg],
            (signed char) '\0'
         )
      ){ /*Else If: primer coordiantes file*/
         ++uiArg;
         *primFileStr = (signed char *) argAryStr[uiArg];
      } /*Else If: primer coordinates file*/

      else if(
         ! eql_charCp(
            (signed char *) "-out",
            (signed char *) argAryStr[uiArg],
            (signed char) '\0'
         )
      ){ /*Else If: output file*/
         ++uiArg;
         *outFileStr = (signed char *) argAryStr[uiArg];
      } /*Else If: output file*/

      /**************************************************\
      * Fun05 Sec03 Sub02:
      *   - masking and filter settings
      \**************************************************/

      else if(
         ! eql_charCp(
            (signed char *) "-mask",
            (signed char *) argAryStr[uiArg],
            (signed char) '\0'
         )
      ){ /*Else If: mask primers*/
         ++uiArg;
         *maskSC = (signed char) *argAryStr[uiArg];
      } /*Else If: mask primers*/

      else if(
         ! eql_charCp(
            (signed char *) "-fudge",
            (signed char *) argAryStr[uiArg],
            (signed char) '\0'
         )
      ){ /*Else If: fudge length*/
         ++uiArg;

         tmpStr = (signed char *) argAryStr[uiArg];

         tmpStr +=
            strToSI_base10str(
               (signed char *) argAryStr[uiArg],
               fudgeSI
            );

         if(*tmpStr)
         { /*If: non-numeric input*/
            fprintf(
               stderr,
               "-fudge %s is non-numeric or to large%s",
               argAryStr[uiArg],
               str_endLine
            );
 
            goto err_fun05_sec04;
         } /*If: non-numeric input*/
      } /*Else If: fudge length*/

      else if(
         ! eql_charCp(
            (signed char *) "-filter",
            (signed char *) argAryStr[uiArg],
            (signed char) '\0'
      )) *filterBl = 1;

      else if(
         ! eql_charCp(
            (signed char *) "-no-filter",
            (signed char *) argAryStr[uiArg],
            (signed char) '\0'
       )) *filterBl = 0;

      /**************************************************\
      * Fun05 Sec03 Sub03:
      *   - help message and version number
      \**************************************************/

      else if(
         checkHelp_mainMaskPrim(
            (signed char *) argAryStr[uiArg]
         )
      ){ /*Else If: help message requested*/
         phelp_mainMaskPrim(stdout);
         goto phelp_fun05_sec04;
      } /*Else If: help message requested*/

      else if(
         checkVersion_mainMaskPrim(
            (signed char *) argAryStr[uiArg])
         )
      { /*Else If: version number requested*/
         pversion_mainMaskPrim(stdout);
         goto pversion_fun05_sec04;
      } /*Else If: version number requested*/

      /**************************************************\
      * Fun05 Sec03 Sub04:
      *   - invalid input
      \**************************************************/

      else
      { /*Else: invalid input*/
         fprintf(
            stderr,
            "%s no recognized%s",
            argAryStr[uiArg],
            str_endLine
         );

         goto err_fun05_sec04;
      } /*Else: invalid input*/

      /**************************************************\
      * Fun05 Sec03 Sub05:
      *   - move to next argument
      \**************************************************/

      ++uiArg;
   } /*Loop: Process each peice of user input*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun05 Sec04:
   ^   - return success or error
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
} /*input_mainMaskPrim*/

/*-------------------------------------------------------\
| Main:
|   - driver function to mask primers
| Input:
|   - numArgsSI:
|     o number of arguments the user input
|   - argAryStr:
|     o pointer to an array of c-strings with user
|       arguments
| Output:
|   - Prints:
|     o sam file with masked sequenes to output file
\-------------------------------------------------------*/
int
main(
   signed int numArgsSI,
   char *argAryStr[]
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Main TOC:
   '   - driver function to mask primers
   '   o main sec01:
   '     - variable declerations
   '   o main sec02:
   '     - initialize, get input, and check input
   '   o main sec03:
   '     - read in the primer coordinates
   '   o main sec04:
   '     - print header
   '   o main sec05:
   '     - mask primers
   '   o main sec06:
   '     - clean up
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec01:
   ^   - variable declerations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   signed char *samFileStr = 0;
   signed char *primFileStr = 0;
   signed char *outFileStr = 0;
   signed char maskSC = def_mask_mainMaskPrim;
   signed int fudgeSI = def_fudge_mainMaskPrim;

   signed char filterBl = def_filter_mainMaskPrim;
     /*Remove reads with no primers*/

   unsigned int *startAryHeapUI = 0;
   unsigned int *endAryHeapUI = 0;
   unsigned int *flagAryHeapUI = 0;
   signed int numPrimSI = 0;

   struct samEntry samStackST;

   FILE *samFILE = 0;
   FILE *outFILE = 0;

   /*For error reporting*/
   signed long errSL = 0;
   signed char errSC = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec02:
   ^   - initialize, get input, and check input
   ^   o main sec02 sub01:
   ^     - initialzei and get (process) the user input
   ^   o main sec02 sub02:
   ^     - check if can open the primer file
   ^   o main sec02 sub03:
   ^     - check if can open output file
   ^   o main sec02 sub04:
   ^     - check if can open sam file
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Main Sec02 Sub01:
   *   - initialize and get (process) user input
   \*****************************************************/

   init_samEntry(&samStackST);

   errSC =
      input_mainMaskPrim(
         numArgsSI,
         argAryStr,
         &samFileStr,
         &primFileStr,
         &outFileStr,
         &maskSC,
         &fudgeSI,
         &filterBl
      );

   if(errSC)
   { /*If: error in the user input*/
      --errSC;
          /*convert help/version request to no error (0)*/

      goto cleanUp_main_sec05_sub03;
   } /*If: error in the user input*/

   /*****************************************************\
   * Main Sec02 Sub02:
   *   - check if can open primer file
   \*****************************************************/

   outFILE =
      fopen(
         (char *) primFileStr,
         "r"
      );
   
   if(! outFILE)
   { /*If: could not open primer coordinates file*/
      fprintf(
         stderr,
         "Unable to open -prim %s%s",
         primFileStr,
         str_endLine
      );

      goto err_main_sec05_sub02;
   } /*If: coudl not open primer coordinates file*/

   fclose(outFILE);
   outFILE = 0;

   /*****************************************************\
   * Main Sec02 Sub03:
   *   - open output file
   \*****************************************************/

   if(
         ! outFileStr 
      || *outFileStr == '-'
   ) outFILE = stdout;

   else
   { /*Else: output file provided*/
      outFILE =
         fopen(
            (char *) outFileStr,
            "w"
         );
      
      if(! outFILE)
      { /*If: I could not open primer coordinates file*/
         fprintf(
            stderr,
            "Unable to open -out %s%s",
            outFileStr,
            str_endLine
         );

         goto err_main_sec05_sub02;
      } /*If: I could not open primer coordinates file*/
   } /*Else: output file provided*/

   /*****************************************************\
   * Main Sec02 Sub04:
   *   - check if can open sam file
   \*****************************************************/

   if(
         ! samFileStr
      || *samFileStr == '-'
   ) samFILE = stdin;

   else
   { /*Else: sam file was provided*/
      samFILE =
         fopen(
            (char *) samFileStr,
            "r"
         );

      if(! samFILE)
      { /*If: could not open sam file*/
         fprintf(
            stderr,
            "Unable to open -sam %s%s",
            samFileStr,
            str_endLine
         );

         goto err_main_sec05_sub02;
      } /*If: could not open sam file*/
   } /*Else: sam file was provided*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec03:
   ^   - read in primer coordinates
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   numPrimSI =
      getCoords_maskPrim(
         primFileStr,
         &startAryHeapUI,
         &endAryHeapUI,
         &flagAryHeapUI,
         &errSL
      ); /*Get the primer list*/

   if(! numPrimSI)
   { /*If: I had an error*/
      if(errSL & def_emptyFileErr_maskPrim)
      { /*If: I had an empty file*/
         fprintf(
            stderr,
            "-prim %s is emtpy%s",
            primFileStr,
            str_endLine
         );
      } /*If: I had an empty file*/

      else if(errSL & def_emptyFileErr_maskPrim)
      { /*Else If: I had an invalid line*/
         fprintf(
            stderr,
            "line number %lu in -prim %s is invalid%s",
            (errSL >> 8),
            primFileStr,
            str_endLine
         );
      } /*Else If: I had an invalid line*/
      
      else
      { /*Else: I had an memory error*/
         fprintf(
            stderr,
            "Memory error reading %s%s",
            primFileStr,
            str_endLine
         );

      } /*Else: I had an memory error*/

      goto err_main_sec05_sub02;
   } /*If: I had an error*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec04:
   ^   - print header
   ^   o main sec04 sub01:
   ^     - get first sam file entry
   ^   o main sec04 sub02:
   ^     - print sam file header
   ^   o main sec04 sub03:
   ^     - make and print the maskPrim header
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Main Sec04 Sub01:
   *   - get first sam file entry
   \*****************************************************/

   errSC = setup_samEntry(&samStackST);

   if(errSC)
   { /*If: memory error*/
      fprintf(
         stderr,
         "memory error setting up samEntry struct%s",
         str_endLine
      );
       
      goto err_main_sec05_sub02;
   } /*If: memory error*/

   /*get first line in the sam file*/
   errSC = get_samEntry(&samStackST, samFILE);

   /*****************************************************\
   * Main Sec04 Sub02:
   *   - print sam file header
   \*****************************************************/

   while(! errSC)
   { /*Loop: print header*/
      if(*samStackST.extraStr != '@')
         break;
      p_samEntry(&samStackST, 0, outFILE);
      errSC = get_samEntry(&samStackST, samFILE);
   } /*Loop: print header*/

   /**************************************************\
   * Main Sec04 Sub03:
   *   - make and print the maskPrim header
   \**************************************************/

   fprintf(
      outFILE,
      "@PG\tID:maskPrim\tPN:maskPrim"
   );

   fprintf(
      outFILE,
      "\tVN:bioTools_%i-%02i-%02i\tCL:maskPrim -mask %c",
      def_year_bioTools,
      def_month_bioTools,
      def_day_bioTools,
      maskSC
   );

   if(filterBl)
      fprintf( outFILE, "\t-filter");
   else
      fprintf( outFILE, "\t-no-filter");

   fprintf(
      outFILE,
      "\t-fudge %i -prim %s",
      fudgeSI,
      primFileStr
   );

   if(samFileStr)
      fprintf(outFILE, "\t-sam %s", samFileStr);
   else
      fprintf(outFILE, "\t-sam -");

   if(outFileStr)
      fprintf(outFILE, "\t-out %s", outFileStr);
   else
      fprintf(outFILE, "\t-out -");

   fprintf(outFILE, "%s", str_endLine);

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec05:
   ^   - mask primers
   ^   o main sec04 sub01:
   ^     - mask primers and print entry + start loop
   ^   o main sec04 sub02:
   ^     - get the next entry
   ^   o main sec04 sub03:
   ^     - check if had an error
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Main Sec05 Sub04:
   *  - mask primers and print entry + start loop
   \*****************************************************/

   while(! errSC)
   { /*Loop: Read in all lines and mask primers*/
      if(*samStackST.seqStr == '*')
         goto nextLine_main_sec05; /*no sequence*/
      
      errSL =
         maskPrim(
            &samStackST,
            startAryHeapUI,
            endAryHeapUI,
            flagAryHeapUI,
            numPrimSI,
            fudgeSI,
            maskSC
         ); /*Mask primers in the sequence*/

      if(
           filterBl
         && errSL
      ) goto nextLine_main_sec05; /*rm non-primer seq*/

      p_samEntry(&samStackST, 0, outFILE);

      /**************************************************\
      * Main Sec05 Sub02:
      *  - get the next entry
      \**************************************************/

      nextLine_main_sec05:;
         errSC = get_samEntry(&samStackST, samFILE);
   } /*Loop: Read in all lines and mask primers*/

   /*****************************************************\
   * Main Sec05 Sub03:
   *  - check if had an error
   \*****************************************************/

   if(errSC == def_memErr_samEntry)
   { /*If: I had an memroy error*/
      fprintf(
         stderr,
         "Memory error reading -sam %s%s",
         samFileStr,
         str_endLine
      );

      goto err_main_sec05_sub02;
   } /*If: I had an memroy error*/

   else if(errSC != def_EOF_samEntry)
   { /*If: I had an memroy error*/
      fprintf(
         stderr,
         "file error -sam %s%s",
         samFileStr,
         str_endLine
      );

      goto err_main_sec05_sub02;
   } /*If: I had an memroy error*/

   /*****************************************************\
   * Main Sec05 Sub01:
   *   - Success clean up
   \*****************************************************/

   errSC = 0;
   goto cleanUp_main_sec05_sub03;

   /*****************************************************\
   * Main Sec05 Sub02:
   *   - Error clean up
   \*****************************************************/

   err_main_sec05_sub02:;
      errSC = -1;
      goto cleanUp_main_sec05_sub03;

   cleanUp_main_sec05_sub03:;
      if(startAryHeapUI)
         free(startAryHeapUI);
      startAryHeapUI = 0;

      if(endAryHeapUI)
         free(endAryHeapUI);
      endAryHeapUI = 0;

      if(flagAryHeapUI)
         free(flagAryHeapUI);
      flagAryHeapUI = 0;

      freeStack_samEntry(&samStackST);

      if(! samFILE) ;
      else if(samFILE == stdin) ;
      else if(samFILE == stdout) ;
      else if(samFILE == stderr) ;
      else
         fclose(samFILE);
      samFILE = 0;

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
