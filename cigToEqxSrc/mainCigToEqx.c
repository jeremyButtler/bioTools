/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' cigToEqx SOF: Start Of File
'   - converts an single reference sam file with regular
'     cigars into an sam file with eqx cigars
'   o header:
'     - included libraries
'   o fun02: phel_mainCigToEqx
'     - prints the help message
'   o fun01: pversion_mainCigToEqx
'     - prints the version
'   o fun-03: input_mainCigToEqx
'     - convert an sam entry cigar to an eqx cigar
'   o main:
'     - driver function to convert cigar to eqx cigar
'   o license:
'     - licensing for this code (CC0)
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

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

#include "../genLib/charCp.h"

#include "../genLib/ulCp.h"
#include "../genBio/seqST.h"
#include "../genBio/samEntry.h"
#include "../genBio/cigToEqx.h"

/*.h files only*/
#include "../genLib/endLine.h"
#include "../bioTools.h"

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\
! Hidden libraries:
!   o .c  #include "../genLib/base10StrToNum.h"
!   o .c  #include "../genLib/numToStr.h"
!   o .c  #include "../genLib/fileFun.h"
!   o .h  #include "../genBio/ntTo5Bit.h"
\%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

#define def_nAsSnp 0 /*1: treat anoymous bases as snps*/

/*-------------------------------------------------------\
| Fun01: pversion_mainCigToEqx
|   - prints the version
| Input:
|   - outFILE:
|     o FILE pointer to print version to
| Output:
|   - Prints:
|     o verision to outFILE
\-------------------------------------------------------*/
void
pversion_mainCigToEqx(
   FILE *outFILE
){
   fprintf(
      outFILE,
      "cigToEqx from bioTools version %i-%02i-%02i%s",
      def_year_bioTools,
      def_month_bioTools,
      def_day_bioTools,
      str_endLine
   );
} /*pversion_mainCigToEqx*/


/*-------------------------------------------------------\
| Fun02: phelp_mainCigToEqx
|   - prints the help message
| Input:
|   - outFILE:
|     o FILE pointer to print help message to
| Output:
|   - Prints:
|     o help message to outFILE
\-------------------------------------------------------*/
void
phelp_mainCigToEqx(
   FILE *outFILE
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun02 TOC:
   '   - print the help message
   '   o fun02 sec01:
   '     - print out usage
   '   o fun02 sec02:
   '     - print out input block
   '   o fun02 sec03:
   '     - print out output block
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun02 Sec01:
   ^   - print out usage
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   fprintf(
      outFILE,
      "cigToEqx -sam reads.sam -ref reference.fasta%s",
      str_endLine
   );

   fprintf(
      outFILE,
      "  - Converst cigars in sam files to eqx cigars%s",
      str_endLine
   );

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun02 Sec02:
   ^   - print out input block
   ^   o fun02 sec02 sub01:
   ^     - input block
   ^   o fun02 sec02 sub02:
   ^     - sam file entry
   ^   o fun02 sec02 sub03:
   ^     - reference file entry
   ^   o fun02 sec02 sub04:
   ^     - anonymous base entry
   ^   o fun02 sec02 sub05:
   ^     - help and version number entry
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun02 Sec02 Sub01:
   *   - input block
   \*****************************************************/

   fprintf(
      outFILE,
      "Input:%s",
      str_endLine
   );

   /*****************************************************\
   * Fun02 Sec02 Sub02:
   *   - sam file entry
   \*****************************************************/

   fprintf(
      outFILE,
      "  -sam reads.sam: [Required]%s",
      str_endLine
   );

   fprintf(
      outFILE,
      "    o Sam file to convert to cigar eqx entries%s",
      str_endLine
   );

   fprintf(
      outFILE,
      "    o Use `sam -` for stdin input%s",
      str_endLine
   );

   /*****************************************************\
   * Fun02 Sec02 Sub03:
   *   - reference file entry
   \*****************************************************/

   fprintf(
      outFILE,
      "  -ref reference.fastaq: [Required]%s",
      str_endLine
   );

   fprintf(
      outFILE,
      "    o Reference file used to map reads to%s",
      str_endLine
   );

   /*****************************************************\
   * Fun02 Sec02 Sub04:
   *   - anonymous base entry
   \*****************************************************/

   if(def_nAsSnp)
      fprintf(
         outFILE,
         "  -n-as-snp: [Yes]%s",
         str_endLine
      );
   else
      fprintf(
         outFILE,
         "  -n-as-snp: [No]%s",
         str_endLine
      );

   fprintf(
      outFILE,
      "    o Treat anonymous bases as SNPs%s",
      str_endLine
   );

   fprintf(
      outFILE,
      "    o Disable with `-n-as-match`%s",
      str_endLine
   );

   /*****************************************************\
   * Fun02 Sec02 Sub05:
   *   - help and version number entry
   \*****************************************************/

   fprintf(
      outFILE,
      "  -h: print out help message%s",
      str_endLine
   );

   fprintf(
      outFILE,
      "  -v: print out version number%s",
      str_endLine
   );

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun02 Sec03:
   ^   - print out output block
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   fprintf(
      outFILE,
      "Output:%s",
      str_endLine
   );

   fprintf(
      outFILE,
      "  - Prints converted sam file to stdout%s",
      str_endLine
   );
} /*phelp_mainCigToEqx*/

/*-------------------------------------------------------\
| Fun-03: input_mainCigToEqx
|   - extracts user input from an array for cigToEqx
| Input:
|   - argAryStr:
|     o array of c-stringsi with user input arguments
|   - numArgsSI:
|     o number of arguments the user input
|   - samFileStr:
|     o pointer to c-string to point to the samfiles name
|   - refFileStr:
|     o pointer to c-string to point to the reference
|      fasta file name
|   - refSeqSTPtr:
|     o pointer to seqST to hold the reference
|       sequence
|   - nAsSnpBl:
|     o point to char to hold if treating anonymous bases
|       as matches or mismathces
|       - 0: matches
|       - 0: mismatches (snps)
| Output:
|   - Modifies:
|     o input to hold user input values
|   - Prints:
|     o help message or version number to sdtout if asked
|       for
|     o For errors; prints the error
|   - Returns:
|     o 0 for no problems
|     o 1 for help message
|     o 2 for problem
\-------------------------------------------------------*/
signed char
input_mainCigToEqx(
   char *argAryStr[],
   signed int numArgsSI,
   signed char **samFileStr,
   signed char **refFileStr,
   signed char *nAsSnpBl
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun03 TOC:
   '   o fun03 sec01:
   '     - check user input
   '   o fun03 sec02:
   '     - variable declerations
   '   o fun03 sec03:
   '     - return any errors
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec01:
   ^   - variable declerations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   signed int siArg = 1;
   signed char errSC = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec02:
   ^   - check user input
   ^   o fun03 sec02 sub01:
   ^     - set the sam file pointer
   ^   o fun03 sec02 sub02:
   ^     - read in the reference sequence
   ^   o fun03 sec02 sub03:
   ^     - check how handleing anonymous bases
   ^   o fun03 sec02 sub04:
   ^     - Check for help messages
   ^   o fun03 sec02 sub05:
   ^     - Check for version number
   ^   o fun03 sec02 sub06:
   ^     - Check for invalid input
   ^   o fun03 sec02 sub07:
   ^     - move to the next argument
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(numArgsSI <= 1)
   { /*If: nothing input*/
      phelp_mainCigToEqx(stdout);
      goto phelp_fun03_sec03;
   } /*If: nothing input*/

   while(siArg < numArgsSI)
   { /*Loop: Read in user input*/

      /**************************************************\
      * Fun03 Sec02 Sub01:
      *   - set the sam file pointer
      \**************************************************/

      if(
         ! eql_charCp(
            (signed char *) "-sam",
            (signed char *) argAryStr[siArg],
            (signed char) '\0'
         )
      ){ /*If: An sam file was input*/
         ++siArg;
         *samFileStr = (signed char *) argAryStr[siArg];
      } /*If: An sam file was input*/

      /**************************************************\
      * Fun03 Sec02 Sub02:
      *   - read in the reference sequence
      \**************************************************/

      else if(
         ! eql_charCp(
            (signed char *) "-ref",
            (signed char *) argAryStr[siArg],
            (signed char) '\0'
         )
      ){ /*Else If: An reference file was input*/
         ++siArg;
         *refFileStr = (signed char *) argAryStr[siArg];
      } /*Else If: An reference file was input*/

      /**************************************************\
      * Fun03 Sec02 Sub03:
      *   - check how handleing anonymous bases
      \**************************************************/

      else if(
         ! eql_charCp(
            (signed char *) "-n-as-snp",
            (signed char *) argAryStr[siArg],
            (signed char) '\0'
      )) *nAsSnpBl = 1;

      else if(
         ! eql_charCp(
            (signed char *) "-n-as-match",
            (signed char *) argAryStr[siArg],
            (signed char) 0
      )) *nAsSnpBl = 0;

      /**************************************************\
      * Fun03 Sec02 Sub04:
      *   - Check for help messages
      \**************************************************/

      else if(
         ! eql_charCp(
            (signed char *) "-h",
            (signed char *) argAryStr[siArg],
            (signed char) '\0'
         )
      ){ /*Else If: user wanted the help message*/
         phelp_mainCigToEqx(stdout);
         goto phelp_fun03_sec03;
      } /*Else If: user wanted the help message*/

      else if(
         ! eql_charCp(
            (signed char *) "--h",
            (signed char *) argAryStr[siArg],
            (signed char) '\0'
         )
      ){ /*Else If: user wanted the help message*/
         phelp_mainCigToEqx(stdout);
         goto phelp_fun03_sec03;
      } /*Else If: user wanted the help message*/

      else if(
         ! eql_charCp(
            (signed char *) "help",
            (signed char *) argAryStr[siArg],
            (signed char) '\0'
         )
      ){ /*Else If: user wanted the help message*/
         phelp_mainCigToEqx(stdout);
         goto phelp_fun03_sec03;
      } /*Else If: user wanted the help message*/

      else if(
         ! eql_charCp(
            (signed char *) "-help",
            (signed char *) argAryStr[siArg],
            (signed char) '\0'
         )
      ){ /*Else If: user wanted the help message*/
         phelp_mainCigToEqx(stdout);
         goto phelp_fun03_sec03;
      } /*Else If: user wanted the help message*/

      else if(
         ! eql_charCp(
            (signed char *) "--help",
            (signed char *) argAryStr[siArg],
            (signed char) '\0'
         )
      ){ /*Else If: user wanted the help message*/
         phelp_mainCigToEqx(stdout);
         goto phelp_fun03_sec03;
      } /*Else If: user wanted the help message*/

      /**************************************************\
      * Fun03 Sec02 Sub05:
      *   - Check for version number
      \**************************************************/

      else if(
         ! eql_charCp(
            (signed char *) "-v",
            (signed char *) argAryStr[siArg],
            (signed char) '\0'
         )
      ){ /*Else If: user wanted the help message*/
         pversion_mainCigToEqx(stdout);
         goto pversion_fun03_sec03;
      } /*Else If: user wanted the help message*/

      else if(
         ! eql_charCp(
            (signed char *) "--v",
            (signed char *) argAryStr[siArg],
            (signed char) '\0'
         )
      ){ /*Else If: user wanted the help message*/
         pversion_mainCigToEqx(stdout);
         goto pversion_fun03_sec03;
      } /*Else If: user wanted the help message*/

      else if(
         ! eql_charCp(
            (signed char *) "version",
            (signed char *) argAryStr[siArg],
            (signed char) '\0'
         )
      ){ /*Else If: user wanted the help message*/
         pversion_mainCigToEqx(stdout);
         goto pversion_fun03_sec03;
      } /*Else If: user wanted the help message*/

      else if(
         ! eql_charCp(
            (signed char *) "-version",
            (signed char *) argAryStr[siArg],
            (signed char) '\0'
         )
      ){ /*Else If: user wanted the help message*/
         pversion_mainCigToEqx(stdout);
         goto pversion_fun03_sec03;
      } /*Else If: user wanted the help message*/

      else if(
         ! eql_charCp(
            (signed char *) "--version",
            (signed char *) argAryStr[siArg],
            (signed char) '\0'
         )
      ){ /*Else If: user wanted the help message*/
         pversion_mainCigToEqx(stdout);
         goto pversion_fun03_sec03;
      } /*Else If: user wanted the help message*/

      /**************************************************\
      * Fun03 Sec02 Sub06:
      *   - Check for invalid input
      \**************************************************/

      else
      { /*Else: input not recongnized*/
         fprintf(
            stderr,
            "%s is unkown%s",
            argAryStr[siArg],
            str_endLine
         );

         goto err_fun03_sec03;
      } /*Else: input not recongnized*/

      /**************************************************\
      * Fun03 Sec02 Sub07:
      *   - move to the next argument
      \**************************************************/

      ++siArg;
   } /*Loop: Read in user input*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec03:
   ^   - return any errors
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   errSC = 0;
   goto ret_fun03_sec03;

   pversion_fun03_sec03:;
   phelp_fun03_sec03:;
     errSC = 1;
     goto ret_fun03_sec03;

   err_fun03_sec03:;
     errSC = 2;
     goto ret_fun03_sec03;

   ret_fun03_sec03:;
      return errSC;
} /*input_mainCigToEqx*/


/*-------------------------------------------------------\
| Main:
|    - driver function to convert cigar to eqx cigar
| Input:
|    - numArgsSI:
|      o integer with user input
|    - argsAryStr:
|      o pointer to c-string array with user input
| Output:
|   - prints:
|     o sam file with eqx cigar entries to stdout
\-------------------------------------------------------*/
int
main(
  signed int numArgsSI,
  char *argsAryStr[]
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Main TOC:
   '   - driver function to convert cigar to eqx cigar
   '   o main Sec01:
   '     - variable declerations
   '   o main Sec02:
   '     - initialize varialbes and get input
   '   o main sec03:
   '     - get reference sequences and open sam file
   '   o main sec04:
   '     - convert cigar entris to eqx cigars
   '   o main sec05:
   '     - clean up
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec01:
   ^   - variable declerations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   signed char *tmpStr = 0;
   signed char errSC = 0;

   signed int *cigHeapArySI = 0;
   signed int lenCigSI = 0;

   signed char nAsSnpBl = def_nAsSnp;

   struct seqST *refHeapAryST = 0;
   signed long lenRefSL = 0;
   signed long sizeRefSL = 0;
   signed long indexSL = 0;   /*index of reference*/

   struct samEntry samStackST;
   signed char endOfHeadBl = 0;

   /*for converting cigar entries*/
   signed char *buffHeapStr = 0;
   unsigned long lenBuffUL = 0;

   /*for printing cigToEqx input to header*/
   signed char *samFileStr = 0;
   signed char *refFileStr = 0;

   FILE *samFILE = 0;
   FILE *outFILE = stdout;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec02:
   ^   - initialize varialbes and get input
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   init_samEntry(&samStackST);

   errSC =
      input_mainCigToEqx(
         argsAryStr,
         numArgsSI,
         &samFileStr,
         &refFileStr,
         &nAsSnpBl
      ); /*Get user input*/
      
   if(errSC)
   { /*If: I need to exit or had error*/
      --errSC; /*reduce help/version to 0 (no error)*/
      goto cleanUp_main_sec05_sub03;
   } /*If: I need to exit or had error*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec03:
   ^   - get reference sequences and open sam file
   ^   o main sec03 sub01:
   ^     - open reference file (reusing samFILE variable)
   ^   o main sec03 sub02:
   ^     - get sequences from reference file
   ^   o main sec03 sub03:
   ^     - sort reference sequences
   ^   o main sec03 sub04:
   ^     - open the sam file
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Main Sec03 Sub01:
   *   - open reference file (reusing samFILE variable)
   \*****************************************************/

   samFILE =
      fopen(
         (char *) refFileStr,
         "r"
      );

   if(! samFILE)
   { /*If: could not open reference file*/
      fprintf(
         stderr,
         "could not open -ref %s%s",
         refFileStr,
         str_endLine
      );

      goto err_main_sec05_sub02;
   } /*If: could not open reference file*/

   /*****************************************************\
   * Main Sec03 Sub02:
   *   - get sequences from reference file
   \*****************************************************/

   refHeapAryST =
      readFaFile_seqST(
         samFILE,
         &lenRefSL,
         &sizeRefSL,
         &errSC
      ); /*get reference sequences*/

   fclose(samFILE);
   samFILE = 0;


   if(errSC)
   { /*If: had error*/
      if(errSC == def_memErr_seqST)
         fprintf(
            stderr,
            "memory error getting references%s",
            str_endLine
         );
      else if(errSC & def_badLine_seqST)
         fprintf(
            stderr,
            "at least one line in -ref %s is invalid%s",
            refFileStr,
            str_endLine
         );
      else
         fprintf(
            stderr,
            "file error (likely no sequences) -ref %s%s",
            refFileStr,
            str_endLine
         );

      goto err_main_sec05_sub02;
   } /*If: had error*/

   /*****************************************************\
   * Main Sec03 Sub03:
   *   - sort reference sequences and remove whitespace
   \*****************************************************/

   for(
      indexSL = 0;
      indexSL < lenRefSL;
      ++indexSL
   ){ /*Loop: remove white space from names*/
      tmpStr = refHeapAryST[indexSL].idStr;
      tmpStr += endWhite_ulCp(tmpStr);
      *tmpStr = '\0';
   } /*Loop: remove white space from names*/

   sort_seqST(
      refHeapAryST,
      lenRefSL
   );

   /*****************************************************\
   * Main Sec03 Sub04:
   *   - open the sam file
   \*****************************************************/

   if(
         ! samFileStr
      || *samFileStr == '-'
   ) samFILE = stdin;

   else
   { /*Else: sam file input*/
      samFILE =
         fopen(
            (char *) samFileStr,
            "r"
         );

      if(! samFILE)
      { /*If: could not open file*/
         fprintf(
            stderr,
            "unable to open -sam %s%s",
            samFileStr,
            str_endLine
         );

         goto err_main_sec05_sub02;
      } /*If: could not open file*/
   } /*Else: sam file input*/
         
   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec04:
   ^   - convert cigar entris to eqx cigars
   ^   o main sec04 sub01:
   ^     - read in first sam file line and start loop
   ^   o main sec04 sub02:
   ^     - print headers
   ^   o main sec04 sub03:
   ^     - print this programs sam file header
   ^   o main sec04 sub04:
   ^     - check if can filter, then filter
   ^   o main sec04 sub05:
   ^     - find reference sequence used to map read
   ^   o main sec04 sub06:
   ^     - convert cigar to eqx cigar
   ^   o main sec04 sub07:
   ^     - print entry if keeping and get next entry
   ^   o main sec04 sub08:
   ^     - finished conversion, check fo errors
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Main Sec04 Sub01:
   *   - read in first sam file line and start loop
   \*****************************************************/

   errSC = setup_samEntry(&samStackST);

   if(errSC)
      goto err_main_sec05_sub02;

   errSC = get_samEntry(&samStackST, samFILE);

   while(! errSC)
   { /*Loop: convert cigar entries to eqx cigars*/

      /**************************************************\
      * Main Sec04 Sub02:
      *   - print headers
      \**************************************************/

      if(samStackST.extraLenUI <= 0)
         ;
      else if(samStackST.extraStr[0] == '@')
      { /*If: This is an comment*/
         p_samEntry(&samStackST, 0, outFILE);
         goto nextEntry_main_sec04_sub05;
      } /*If: This is an comment*/

      /**************************************************\
      * Main Sec04 Sub03:
      *   - print this programs sam file header
      \**************************************************/

      if(! endOfHeadBl)
      { /*If: I need to print out the program comment*/
         
         fprintf(
            outFILE,
            "@PG\tID:cigToEqx\tPN:cigToEqx"
         );

         fprintf(
            outFILE,
            "\tVN:bioTools_%i-%02i-%02i\tCL:cigToEqx",
            def_year_bioTools,
            def_month_bioTools,
            def_day_bioTools
         );

         fprintf(outFILE, " -ref %s", refFileStr);

         if(nAsSnpBl)
            fprintf(outFILE, " -n-as-snp");
         else
            fprintf(outFILE, " -n-as-match");

         if(samFileStr)
            fprintf(
               outFILE,
               " -sam %s%s",
               samFileStr,
               str_endLine
            );

         else
            fprintf(
               outFILE,
               " -sam -%s",
               str_endLine
            );

         
         endOfHeadBl = 1;
      } /*If: I need to print out the program comment*/

      /**************************************************\
      * Main Sec04 Sub04:
      *   - check if can filter, then filter
      \**************************************************/

      if(samStackST.readLenUI < 1)
         goto nextEntry_main_sec04_sub05; /*No sequence*/

      if(lenBuffUL > (unsigned long) lenCigSI)
      { /*If: I need to resize buffers*/
         lenCigSI = (signed int) lenBuffUL;

         if(cigHeapArySI)
            free(cigHeapArySI);

         cigHeapArySI = 0;

         cigHeapArySI =
            malloc((lenCigSI + 1) * sizeof(signed int));

         if(! cigHeapArySI)
         { /*If: I had an memory error*/
            fprintf(
               stderr,
               "memory error%s",
               str_endLine
            );

            goto err_main_sec05_sub02;
         } /*If: I had an memory error*/
      } /*If: I need to resize buffers*/

      /**************************************************\
      * Main Sec04 Sub05:
      *   - find reference sequence used to map read
      \**************************************************/

      indexSL =
         search_seqST(
            refHeapAryST,
            samStackST.refIdStr,
            lenRefSL
         ); /*see if can find reference*/

      if(indexSL < 0)
      { /*If: could not find reference*/
         fprintf(
            stderr,
            "-ref %s missing %s, skipping read %s%s",
            refFileStr,
            samStackST.refIdStr,
            samStackST.qryIdStr,
            str_endLine
         ); /*warn about missing reference*/

         goto nextEntry_main_sec04_sub05;
      } /*If: could not find reference*/

      /**************************************************\
      * Main Sec04 Sub06:
      *   - convert cigar to eqx cigar
      \**************************************************/

      errSC =
         cigToEqx(
            &samStackST,
            refHeapAryST[indexSL].seqStr,
            &buffHeapStr,
            &cigHeapArySI,
            &lenCigSI,
            nAsSnpBl
         );

      if(errSC == 1)
      { /*If: had memory error*/
         errSC = def_memErr_samEntry;
         break;
      } /*If: had memory error*/

      if(lenBuffUL < (unsigned long) lenCigSI)
         lenBuffUL = (unsigned long) lenCigSI;

      /**************************************************\
      * Main Sec04 Sub07:
      *   - print entry if keeping and get next entry
      \**************************************************/

      if(! errSC)
         p_samEntry(&samStackST, 0, outFILE);

      nextEntry_main_sec04_sub05:;
         errSC = get_samEntry(&samStackST, samFILE);
   } /*Loop: convert cigar entries to eqx cigars*/

   /*****************************************************\
   * Main Sec04 Sub08:
   *   - finished conversion, check fo errors
   \*****************************************************/

   if(errSC != def_EOF_samEntry)
   { /*If: had sam file error*/
      fprintf(
         stderr,
         "memory error reading -sam %s%s",
         samFileStr,
         str_endLine
      );

      goto err_main_sec05_sub02;
   } /*If: had sam file error*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec05:
   ^   - clean up
   ^   o main sec05 sub01:
   ^     - no error clean up
   ^   o main sec05 sub02:
   ^     - error clean up
   ^   o main sec05 sub03:
   ^     - general clean up (error/no error)
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
   errSC = 1;
   goto cleanUp_main_sec05_sub03;

   /*****************************************************\
   * Main Sec05 Sub03:
   *   - general clean up (error/no error)
   \*****************************************************/

   cleanUp_main_sec05_sub03:;

      if(buffHeapStr)
         free(buffHeapStr);
      buffHeapStr = 0;

      if(cigHeapArySI)
         free(cigHeapArySI);
      cigHeapArySI = 0;

      if(outFILE && outFILE != stdout)
         fclose(outFILE);
      outFILE = 0;

      if(samFILE && samFILE != stdin)
         fclose(samFILE);
      samFILE = 0;

      if(refHeapAryST)
         freeHeapAry_seqST(
            refHeapAryST,
            sizeRefSL
         );
      refHeapAryST = 0;

      freeStack_samEntry(&samStackST);

      return(errSC);
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
