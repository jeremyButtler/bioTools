/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' mainDICoords SOF: Start Of File
'   - driver function to get di coordinates
'   o header:
'     - included libraries and definitions
'   o fun01: pversion_mainDICoords
'     - prints version number
'   o fun02: phelp_mainDICoords
'     - prints help message
'   o fun03: input_mainDICoords
'     - gets user input from an array of c-strings
'   o main:
'     - main driver function to get DI coordinates
'   o license:
'     - licensing for this code (CC0)
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

#include "../genLib/base10str.h"
#include "../genLib/ulCp.h"
#include "../genBio/samEntry.h"
#include "../genBio/diCoords.h"

/*.h files only*/
#include "../bioTools.h"
#include "../genLib/endLine.h"

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\
! Hidden libraries:
!   o .c  #include "../genLib/numToStr.h"
!   o .c  #include "../genLib/fileFun.h"
!   o .h  #include "../genBio/ntTo5Bit.h"
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

#define def_minDIDelLen_mainDICoords 20
   /*minimum deletion size to count as a DI event*/

#define def_minPadNt_mainDICoords 12
   /*minimum bases at end/start needed to count a DI
   `   this is here to avoid large DI and then 4 matches
   */

#define def_pNonDI_mainDICoords 0
   /*print non-DI entries*/

/*what type of indels to target*/
#define def_pDel_mainDICoords 1
#define def_pIns_mainDICoords 0

/*-------------------------------------------------------\
| Fun01: pversion_mainDICoords
|   - prints version number
| Input:
|   - outFILE:
|     o file to print version number to
| Output:
|   - Prints:
|     o version number to outFILE
\-------------------------------------------------------*/
void
pversion_mainDICoords(
   void *outFILE
){
   fprintf(
      (FILE *) outFILE,
      "diCoords from bioTools version: %i-%02i-%02i%s",
      def_year_bioTools,
      def_month_bioTools,
      def_day_bioTools,
      str_endLine
   );
} /*pversion_mainDICoords*/

/*-------------------------------------------------------\
| Fun02: phelp_mainDICoords
|   - prints help message
| Input:
|   - outFILE:
|     o file to print help message to
| Output:
|   - Prints:
|     o help message to outFILE
\-------------------------------------------------------*/
void
phelp_mainDICoords(
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
      "mainDICoords -sam reads.sam > out.tsv"
   );

   fprintf(
      (FILE *) outFILE,
      "  - find coordinates of flu DIs in mapped reads%s",
      str_endLine
   );

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun02 Sec02:
   ^   - input block
   ^   o fun02 sec02 sub01:
   ^     - input header
   ^   o fun02 sec02 sub02:
   ^     - file io input
   ^   o fun02 sec02 sub03:
   ^     - DI filter settings
   ^   o fun02 sec02 sub04:
   ^     - print settings
   ^   o fun02 sec02 sub05:
   ^     - help and version number
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun02 Sec02 Sub01:
   *   - input header
   \*****************************************************/

   fprintf((FILE *) outFILE, "Input:%s", str_endLine);

   /*****************************************************\
   * Fun02 Sec02 Sub02:
   *   - file io input
   \*****************************************************/

   /*input*/
   fprintf(
      (FILE *) outFILE,
      "  -sam reads.sam: [Required]%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "    o sam file with mapped reads to search%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "    o use \"-sam -\" for stdin%s",
      str_endLine
   );

   /*output*/
   fprintf(
      (FILE *) outFILE,
      "  -out out.tsv: [Optinal: stdout]%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "    o file to output to%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "    o use \"-out -\" for stdout%s",
      str_endLine
   );

   /*****************************************************\
   * Fun02 Sec02 Sub03:
   *   - DI filter settings
   \*****************************************************/

    /*print min del size*/
   fprintf(
      (FILE *) outFILE,
      "  -min-del %u: [Optinal; %u]%s",
      def_minDIDelLen_mainDICoords,
      def_minDIDelLen_mainDICoords,
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "    o minimum number of deleted bases to flag a%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "      deletion as a DI event%s",
      str_endLine
   );

   /*min nucleotides at start/end of sequence*/
   fprintf(
      (FILE *) outFILE,
      "  -min-nt-pad %u: [Optinal; %u]%s",
      def_minPadNt_mainDICoords,
      def_minPadNt_mainDICoords,
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "    o minimum number of matches, SNPs, and%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "      insertions needed at start/end of read%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "      before counting large deltions as DI%s",
      str_endLine
   );

   /*****************************************************\
   * Fun02 Sec02 Sub04:
   *   - print settings
   \*****************************************************/

   if(def_pDel_mainDICoords)
      fprintf(
         (FILE *) outFILE,
         "  -pdel: [Optional yes]%s",
      str_endLine
      );
   else
      fprintf(
         (FILE *) outFILE,
         "  -pdel: [Optional no]%s",
      str_endLine
      );


   fprintf(
      (FILE *) outFILE,
      "    o search and print large deletions%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "    o disable with \"-no-pdel\"%s",
      str_endLine
   );

   if(def_pIns_mainDICoords)
      fprintf(
         (FILE *) outFILE,
         "  -pins: [Optional yes]%s",
      str_endLine
      );
   else
      fprintf(
         (FILE *) outFILE,
         "  -pins: [Optional no]%s",
      str_endLine
      );


   fprintf(
      (FILE *) outFILE,
      "    o search and print large insertions%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "    o disable with \"-no-pins\"%s",
      str_endLine
   );


   /*print non-DI reads*/
   if(def_pNonDI_mainDICoords)
      fprintf(
         (FILE *) outFILE,
         "  -non-di: [Optinal; Yes]%s",
      str_endLine
      );
   else
      fprintf(
         (FILE *) outFILE,
         "  -non-di: [Optinal; No]%s",
      str_endLine
      );

   fprintf(
      (FILE *) outFILE,
      "    o print entries with no DI events%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "    o disable with \"-di-only\"%s",
      str_endLine
   );

   /*****************************************************\
   * Fun02 Sec02 Sub05:
   *   - help and version number
   \*****************************************************/

   fprintf(
      (FILE *) outFILE,
      "  -h: print this help message%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "  -v: print the version number%s",
      str_endLine
   );

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun02 Sec03:
   ^   - output block
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   fprintf(
      (FILE *) outFILE,
      "Output:%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "  - tsv with read id, reference name,%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "    DI start coordinate, and DI end coordinate%s",
      str_endLine
   );
} /*phelp_mainDICoords*/

/*-------------------------------------------------------\
| Fun03: input_mainDICoords
|   - gets user input from an array of c-strings
| Input:
|   - numArgsSI:
|     o number of arguments input
|   - argAryStr:
|     o array of c-strings with user input
|   - samFileStrPtr:
|     o pointer to c-sting to point to sam file name
|   - outFileStrPtr:
|     o pointer to c-sting to point to output file name
|   - minDILenUIPtr:
|     o pointer to unsigned int to hold the minimum
|       deletion length to be a DI event
|   - minPadNtUIPtr:
|     o pointer to unsigned int to hold the minimum
|       number of nucleotides before/after a large DI
|       event
|   - pNoDIBlPtr:
|     o pointer to signed char (1: print non-DI read ids)
|   - indelFlagSCPtr:
|     o set to indel type to look for and print
|       - 1: to keep deletions
|       - 2: to keep insertions
|       - 3: to keep both detetions and insertions
| Output:
|   - Modifies:
|     o all variables except numArgsSI and argAryStr to
|       hold/point to user input
|   - Returns:
|     o 0 for no errors
|     o 1 for printed version number or help message
|     o 2 for invalid input
\-------------------------------------------------------*/
signed char
input_mainDICoords(
   int numArgsSI,                /*number of arguments*/
   char *argAryStr[],            /*has input arguments*/
   signed char **samFileStrPtr,  /*will have sam file*/
   signed char **outFileStrPtr,  /*will have out file*/
   unsigned int *minDILenUIPtr,  /*will have min DI len*/
   unsigned int *minPadNtUIPtr,  /*ends non-del length*/
   signed char *pNoDIBlPtr,      /*1 if keeping non-DI*/
   signed char *indelFlagSCPtr/*keep:1 del; 2 ins; 3 all*/
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   '   o fun03 sec01:
   '     - variable declarations
   '   o fun03 sec02:
   '     - check if user input something
   '   o fun03 sec03:
   '     - get user input
   '   o fun03 sec04:
   '     - clean up and return
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   signed char errSC = 0;
   signed int siArg = 1;
   signed char *tmpStr = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec02:
   ^   - check if user input something
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(numArgsSI <= 1)
   { /*If: nothing input*/
      phelp_mainDICoords(stdout);
      goto phelp_fun03_sec04;
   } /*If: nothing input*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec03:
   ^   - get user input
   ^   o fun03 sec03 sub01:
   ^     - start loop and general input
   ^   o fun03 sec03 sub02:
   ^     - check for help messages
   ^   o fun03 sec03 sub03:
   ^     - check for version number requests
   ^   o fun03 sec03 sub04:
   ^     - invalid entry and move to next entry
   \\<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun03 Sec03 Sub01:
   *   - start loop and general input
   \*****************************************************/

   while(siArg < numArgsSI)
   { /*Loop: get user input*/
      if(
         ! eqlNull_ulCp(
            (signed char *) "-sam",
            (signed char *) argAryStr[siArg]
         )
      ){ /*If: is the sam file name*/
         ++siArg;
         *samFileStrPtr =
            (signed char *) argAryStr[siArg];
      } /*If: is the sam file name*/

      else if(
         ! eqlNull_ulCp(
            (signed char *) "-out",
            (signed char *) argAryStr[siArg]
         )
      ){ /*Else If: is the output file name*/
         ++siArg;
         *outFileStrPtr =
            (signed char *) argAryStr[siArg];
      } /*Else If: is the output file name*/

      else if(
         ! eqlNull_ulCp(
            (signed char *) "-pdel",
            (signed char *) argAryStr[siArg]
         )
      ) *indelFlagSCPtr |= 1;

      else if(
         ! eqlNull_ulCp(
            (signed char *) "-no-pdel",
            (signed char *) argAryStr[siArg]
         )
      ) *indelFlagSCPtr &= ~1;

      else if(
         ! eqlNull_ulCp(
            (signed char *) "-pins",
            (signed char *) argAryStr[siArg]
         )
      ) *indelFlagSCPtr |= 2;

      else if(
         ! eqlNull_ulCp(
            (signed char *) "-no-pins",
            (signed char *) argAryStr[siArg]
         )
      ) *indelFlagSCPtr &= ~2;

      else if(
         ! eqlNull_ulCp(
            (signed char *) "-non-di",
            (signed char *) argAryStr[siArg]
         )
      ) *pNoDIBlPtr = 1;

      else if(
         ! eqlNull_ulCp(
            (signed char *) "-di-only",
            (signed char *) argAryStr[siArg]
         )
      ) *pNoDIBlPtr = 0;

      else if(
         ! eqlNull_ulCp(
            (signed char *) "-min-del",
            (signed char *) argAryStr[siArg]
         )
      ){ /*Else If: is the minimum DI deletion lenght*/
         ++siArg;

         tmpStr = (signed char *) argAryStr[siArg];

         tmpStr +=
            strToUI_base10str(
               (signed char *) argAryStr[siArg],
               minDILenUIPtr
            );

         if(*tmpStr != '\0')
         { /*If: had non-numeric entry*/
            fprintf(
               stderr,
               "-min-del %s is non-numeric or to large%s",
               argAryStr[siArg],
               str_endLine
            );
 
            goto invalidInput_fun03_sec04;
         } /*If: had non-numeric entry*/
      } /*Else If: is the minimum DI deletion lenght*/

      else if(
         
         ! eqlNull_ulCp(
            (signed char *) "-min-nt-pad",
            (signed char *) argAryStr[siArg]
         )
      ){ /*Else If: is the minimum DI deletion length*/
         ++siArg;

         tmpStr = (signed char *) argAryStr[siArg];

         tmpStr +=
            strToUI_base10str(
               (signed char *) argAryStr[siArg],
               minPadNtUIPtr
            );

         if(*tmpStr != '\0')
         { /*If: had non-numeric entry*/
            fprintf(
               stderr,
               "-min-nt-pad %s; non-numeric or to big%s",
               argAryStr[siArg],
               str_endLine
            );
 
            goto invalidInput_fun03_sec04;
         } /*If: had non-numeric entry*/
      } /*Else If: is the minimum DI deletion length*/

      /**************************************************\
      * Fun03 Sec03 Sub02:
      *   - check for help messages
      \**************************************************/

      else if(
         ! eqlNull_ulCp(
            (signed char *) "-h",
            (signed char *) argAryStr[siArg]
         )
      ) goto phelp_fun03_sec04;

      else if(
         ! eqlNull_ulCp(
            (signed char *) "--h",
            (signed char *) argAryStr[siArg]
         )
      ) goto phelp_fun03_sec04;

      else if(
         ! eqlNull_ulCp(
            (signed char *) "help",
            (signed char *) argAryStr[siArg]
         )
      ) goto phelp_fun03_sec04;

      else if(
         ! eqlNull_ulCp(
            (signed char *) "-help",
            (signed char *) argAryStr[siArg]
         )
      ) goto phelp_fun03_sec04;

      else if(
         ! eqlNull_ulCp(
            (signed char *) "--help",
            (signed char *) argAryStr[siArg]
         )
      ) goto phelp_fun03_sec04;

      /**************************************************\
      * Fun03 Sec03 Sub03:
      *   - check for version number requests
      \**************************************************/
      
      else if(
         ! eqlNull_ulCp(
            (signed char *) "-v",
            (signed char *) argAryStr[siArg]
         )
      ) goto pversion_fun03_sec04;

      else if(
         ! eqlNull_ulCp(
            (signed char *) "--v",
            (signed char *) argAryStr[siArg]
         )
      ) goto pversion_fun03_sec04;

      else if(
         ! eqlNull_ulCp(
            (signed char *) "version",
            (signed char *) argAryStr[siArg]
         )
      ) goto pversion_fun03_sec04;

      else if(
         ! eqlNull_ulCp(
            (signed char *) "-version",
            (signed char *) argAryStr[siArg]
         )
      ) goto pversion_fun03_sec04;

      else if(
         ! eqlNull_ulCp(
            (signed char *) "--version",
            (signed char *) argAryStr[siArg]
         )
      ) goto pversion_fun03_sec04;

      /**************************************************\
      * Fun03 Sec03 Sub04:
      *   - invalid entry and move to next entry
      \**************************************************/

      else
      { /*Else: uknown input*/
         fprintf(
            stderr,
            "%s is not recongized%s",
            argAryStr[siArg],
            str_endLine
         );
 
         goto invalidInput_fun03_sec04;
      } /*Else: uknown input*/

      ++siArg;
   } /*Loop: get user input*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec04:
   ^   - clean up and return
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   errSC = 0;
   goto cleanUp_fun03_sec04;

   phelp_fun03_sec04:;
      phelp_mainDICoords(stdout);
      errSC = 1;
      goto cleanUp_fun03_sec04;
   pversion_fun03_sec04:;
      pversion_mainDICoords(stdout);
      errSC = 1;
      goto cleanUp_fun03_sec04;

   invalidInput_fun03_sec04:;
      errSC = 2;
      goto cleanUp_fun03_sec04;

   cleanUp_fun03_sec04:;
   return(errSC);
} /*input_mainDICoords*/

/*-------------------------------------------------------\
| Main:
|   - main driver function to get DI coordinates
| Input:
|   - numArgsSI:
|     o number of arguments the user input
|   - argAryStr:
|     o array of c-strings with user input
| Output:
|   - Prints:
|     o tsv with ids, number DI events & coordiantes
\-------------------------------------------------------*/
int
main(
   int numArgsSI,
   char *argAryStr[]
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Main TOC:
   '   - main driver function to get DI coordinates
   '   o main sec01:
   '     - variable declerations
   '   o main sec02:
   '     - initialize structs, get input, setup memory
   '   o main sec03:
   '     - get coordinates
   '   o main sec04:
   '     - clean up
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec01:
   ^   - variable declerations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   signed char *samFileStr = 0;
   signed char *outFileStr = 0;

   unsigned int minDILenUI = def_minDIDelLen_mainDICoords;
   unsigned int minPadNtUI = def_minPadNt_mainDICoords;

   signed char pNoDIBl = def_pNonDI_mainDICoords;

   signed char indelFlagSC =
      def_pDel_mainDICoords | def_pIns_mainDICoords;
      /*if printing deletions/indels*/

   signed char errSC = 0;

   unsigned int *startHeapAryUI = 0;
   unsigned int *endHeapAryUI = 0;
   unsigned int *delSizeHeapAryUI = 0;
   signed char *indelHeapArySC = 0;
   unsigned int lenArysUI = 0;
   signed int numDIsSI = 0;

   struct samEntry samStackST;
   signed char *buffHeapStr = 0; /*buffer for sam file*/
   unsigned long numEntryUL = 0;
      /*number reads in sam file*/

   FILE *samFILE = 0;
   FILE *outFILE = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec02:
   ^   - initialize structs, get input, setup memory
   ^   o main sec02 sub01:
   ^     - initialize structures
   ^   o main sec02 sub02:
   ^     - get input
   ^   o main sec02 sub03:
   ^     - allocate memory
   ^   o main sec02 sub04:
   ^     - open sam file
   ^   o main sec02 sub05:
   ^     - open output file
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Main Sec02 Sub01:
   *   - initialize structures
   \*****************************************************/

   init_samEntry(&samStackST);

   /*****************************************************\
   * Main Sec02 Sub02:
   *   - get input
   \*****************************************************/

   errSC =
      input_mainDICoords(
         numArgsSI,     /*number of arguments*/
         argAryStr,     /*has input arguments*/
         &samFileStr,   /*will have sam file*/
         &outFileStr,   /*will have out file*/
         &minDILenUI,   /*will have min DI len*/
         &minPadNtUI,   /*min pad length to count DI*/
         &pNoDIBl,      /*1 if keeping non-DI*/
         &indelFlagSC
     ); /*get user input*/

   if(errSC)
   { /*If: had error*/
      --errSC; /*help message/version error to no error*/
      goto cleanUp_sec04_sub04;
   } /*If: had error*/

   /*****************************************************\
   * Main Sec02 Sub03:
   *   - allocate memory
   \*****************************************************/

   setup_samEntry(&samStackST);

   /*****************************************************\
   * Main Sec02 Sub04:
   *   - open sam file
   \*****************************************************/

   if(
         ! samFileStr
      || *samFileStr == '-'
   ) samFILE = stdin;

   else
   { /*Else: user provided a sam file*/
      samFILE =
         fopen(
            (char *) samFileStr,
            "r"
         );

      if(! samFILE)
      { /*If: no sam file*/
         fprintf(
            stderr,
            "could not open -sam %s%s",
            samFileStr,
            str_endLine
         );

         goto fileErr_main_sec04_sub03;
      } /*If: no sam file*/
   } /*Else: user provided a sam file*/

   /*****************************************************\
   * Main Sec02 Sub05:
   *   - open output file
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
            "r"
         );

      if(! outFILE)
      { /*If: no output file*/
         fprintf(
            stderr,
            "could not open -sam %s%s",
            outFileStr,
            str_endLine
         );

         goto fileErr_main_sec04_sub03;
      } /*If: no output file*/
   } /*Else: user provided an output file*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec03:
   ^   - get coordinates
   ^   o main sec03 sub01:
   ^     - get first sam file entry and start loop
   ^   o main sec03 sub02:
   ^     - remove comments, and non-primary aligned reads
   ^   o main sec03 sub04:
   ^     - move to the next entry in the sam file
   ^   o main sec03 sub05:
   ^     - after loop; check for errors
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Main Sec03 Sub01:
   *   - get first sam file entry and start loop
   \*****************************************************/

   pDIHead_diCoords(outFILE);
   errSC = get_samEntry(&samStackST, samFILE);

   while(! errSC)
   { /*Loop: find DI events and coordinates*/

      /**************************************************\
      * Main Sec03 Sub02:
      *   - remove comments, and non-primary aligned reads
      \**************************************************/

      ++numEntryUL;

      if(samStackST.extraStr[0] == '@')
         goto nextEntry_main_sec03_sub04;

      if(samStackST.flagUS & (4 | 256 | 2048))
         goto nextEntry_main_sec03_sub04;
         /* Explenation:
         `   - 4 = unmapped read
         `   - 256 = secondary alignment (alternative),
         `   - 2048 = supplemental alignment (diff ref)
         */
          
      /**************************************************\
      * Main Sec03 Sub03:
      *   - scan for DI entries
      \**************************************************/

      numDIsSI =
         get_diCoords(
            &samStackST,
            minDILenUI,
            minPadNtUI,
            indelFlagSC,
            &startHeapAryUI,
            &endHeapAryUI,
            &delSizeHeapAryUI,
            &indelHeapArySC,
            &lenArysUI
        ); /*find DI events and their coordinates*/
   
      if(numDIsSI < 0)
      { /*If: memory error*/
         fprintf(
            stderr,
            "MEMORY error on -sam %s line number %lu%s",
            samFileStr,
            numEntryUL,
            str_endLine
         );

         goto memErr_main_sec04_sub02;
      } /*If: memory error*/

      if(numDIsSI > 0 || pNoDIBl)
      { /*If: printing out read*/
         pDI_diCoords(
            samStackST.qryIdStr,
            samStackST.refIdStr,
            startHeapAryUI,
            endHeapAryUI,
            delSizeHeapAryUI,
            indelHeapArySC,
            numDIsSI,
            samStackST.refStartUI,
            samStackST.refEndUI,
            outFILE
        );
      } /*If: printing out read*/

      /**************************************************\
      * Main Sec03 Sub04:
      *   - move to the next entry in the sam file
      \**************************************************/
      nextEntry_main_sec03_sub04:;
         errSC = get_samEntry(&samStackST, samFILE);
   } /*Loop: find DI events and coordinates*/

   /*****************************************************\
   * Main Sec03 Sub05:
   *   - after loop; check for errors
   \*****************************************************/

   if(errSC != 1)
   { /*If: had memory error*/
      fprintf(
         stderr,
         "MEMORY ERROR on line %lu of -sam %s%s",
         numEntryUL,
         samFileStr,
         str_endLine
      );

      goto memErr_main_sec04_sub02;
   } /*If: had memory error*/

   if(! numEntryUL)
   { /*If: had no sam file entrys*/
      fprintf(
         stderr,
         "-sam %s has no mapped reads%s",
         samFileStr,
         str_endLine
      );

      goto memErr_main_sec04_sub02;
   } /*If: had no sam file entrys*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec04:
   ^   - clean up
   ^   o main sec04 sub01:
   ^     - no error clean up
   ^   o main sec04 sub02:
   ^     - memory error clean up
   ^   o main sec04 sub03:
   ^     - file error clean up
   ^   o main sec04 sub04:
   ^     - variable clean up (all errors and no errors)
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Main Sec04 Sub01:
   *   - no error clean up
   \*****************************************************/

   errSC = 0;
   goto cleanUp_sec04_sub04;

   /*****************************************************\
   * Main Sec04 Sub02:
   *   - memory error clean up
   \*****************************************************/

   memErr_main_sec04_sub02:;
      errSC = -1;
      goto cleanUp_sec04_sub04;

   /*****************************************************\
   * Main Sec04 Sub03:
   *   - file error clean up
   \*****************************************************/

   fileErr_main_sec04_sub03:;
      errSC = 2;
      goto cleanUp_sec04_sub04;

   /*****************************************************\
   * Main Sec04 Sub04:
   *   - variable clean up (all errors and no errors)
   \*****************************************************/

   cleanUp_sec04_sub04:;
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

      outFILE = 0;

      if(startHeapAryUI)
         free(startHeapAryUI);
      startHeapAryUI = 0;

      if(endHeapAryUI)
         free(endHeapAryUI);
      endHeapAryUI = 0;

      if(delSizeHeapAryUI)
         free(delSizeHeapAryUI);
      delSizeHeapAryUI = 0;

      if(indelHeapArySC)
         free(indelHeapArySC);
      indelHeapArySC = 0;

      if(buffHeapStr)
         free(buffHeapStr);
      buffHeapStr = 0;

      freeStack_samEntry(&samStackST);

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
