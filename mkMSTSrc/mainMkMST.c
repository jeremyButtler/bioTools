/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' mainMkMST.c SOF: Start Of File
'   - makes a minimum spanning tree for input lineages
'   o header:
'     - included libraries
'   o fun01: pversion_mainMkMST
'     - prints the version number of mkMST
'   o fun02: phelp_mainMkMST
'     - prints the help message for mkMST
'   o fun03: input_mainMkMST
'     - get user input from the args array
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

#include "../genLib/ulCp.h"
#include "../genLib/base10str.h"
#include "../genTree/runPrim.h"

/*.h files only*/
#include "../bioTools.h"
#include "../genLib/endLine.h"

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\
! Hidden Libraries:
!   - .c  #include "../genTree/prim.h"
!   - .c  #include "../genSort/siBinSearch.h"
!   - .c  #include "../genLib/genMath.h"
!   - .c  #include "../genLib/fileFun.h"
\%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

/*-------------------------------------------------------\
| Fun01: pversion_mainMkMST
|   - prints the version number of mkMST
| Input:
|   - inFILE:
|     o FILE pointer to file to print version to
| Output:
|   - Prints:
|     o prints the version number to inFILE
\-------------------------------------------------------*/
void
pversion_mainMkMST(
   void *inFILE
){
   fprintf(
      (FILE *) inFILE,
      "mkMST from bioTools version: %i-%02i-%02i%s",
      def_year_bioTools,
      def_month_bioTools,
      def_day_bioTools,
      str_endLine
   );
} /*pversion_mainMkMST*/

/*-------------------------------------------------------\
| Fun02: phelp_mainMkMST
|   - prints the help message for mkMST
| Input:
|   - inFILE:
|     o FILE pointer to file to print the help message to
| Output:
|   - Prints:
|     o help message to inFILE
\-------------------------------------------------------*/
void
phelp_mainMkMST(
   void *inFILE
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun02 TOC:
   '   - prints the help message for mkMST
   '   o fun02 sec01:
   '     - usage entry
   '   o fun02 sec01:
   '     - usage entry
   '   o fun02 sec02:
   '     - user input
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun02 Sec01:
   ^   - usage entry
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   fprintf(
      (FILE *) inFILE,
      "mkMST -features <features>.tsv%s",
      str_endLine
   );

   fprintf(
      (FILE *) inFILE,
      "  - builds a minimum spanning tree from the%s",
      str_endLine
   );

   fprintf(
      (FILE *) inFILE,
      "    input feature table and saves tree to a%s",
      str_endLine
   );

   fprintf(
      (FILE *) inFILE,
      "    newick file %s",
      str_endLine
   );

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun02 Sec02:
   ^   - user input
   ^   o fun02 sec02 sub01:
   ^     - feature table and input header
   ^   o fun02 sec02 sub02:
   ^     - node to start building the tree with
   ^   o fun02 sec02 sub03:
   ^     - output file
   ^   o fun02 sec02 sub04:
   ^     - help and version number
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun02 Sec02 Sub01:
   *   - feature table and input header
   \*****************************************************/

   fprintf((FILE *) inFILE, "Input:%s", str_endLine);


   fprintf(
      (FILE *) inFILE,
      "  -features <featurrs>.tsv: [Required]%s",
      str_endLine
   );

   fprintf(
      (FILE *) inFILE,
      "    o feature table with lineages to build the %s",
      str_endLine
   );

   fprintf(
      (FILE *) inFILE,
      "      minimum spanning tree from%s",
      str_endLine
   );

   fprintf(
      (FILE *) inFILE,
      "    o format: %s",
      str_endLine
   );
   fprintf(
      (FILE *) inFILE,
      "      - first row is the header%s",
      str_endLine
   );

   fprintf(
      (FILE *) inFILE,
      "      - 2nd row to last are the sequences%s",
      str_endLine
   );

   fprintf(
      (FILE *) inFILE,
      "      - 1st column is the sequence name%s",
      str_endLine
   );

   fprintf(
      (FILE *) inFILE,
      "      - 2nd to last columns are the lineages%s",
      str_endLine
   );

   fprintf(
    (FILE *) inFILE,
    "        * mark lineage as unkown with `*` or `na`%s",
    str_endLine
   );

   /*****************************************************\
   * Fun02 Sec02 Sub02:
   *   - node to start building the tree with
   \*****************************************************/

   fprintf(
      (FILE *) inFILE,
      "  -node 1: [Default; 1]%s",
      str_endLine
   );

   fprintf(
      (FILE *) inFILE,
      "    o entry to start building the MST with%s",
      str_endLine
   );

   fprintf(
      (FILE *) inFILE,
      "    o index 1 (one is first row in the file)%s",
      str_endLine
   );

   /*****************************************************\
   * Fun02 Sec02 Sub03:
   *   - output file
   \*****************************************************/

   fprintf(
      (FILE *) inFILE,
      "  -out <file.newick>: [Default; stdout]%s",
      str_endLine
   );

   fprintf(
      (FILE *) inFILE,
      "    o file to save the output tree to%s",
      str_endLine
   );

   fprintf(
      (FILE *) inFILE,
      "    o use `-out -` for stdout%s",
      str_endLine
   );

   /*****************************************************\
   * Fun02 Sec02 Sub04:
   *   - help and version number
   \*****************************************************/

   fprintf(
      (FILE *) inFILE,
      "  -h: print this help message and exit%s",
      str_endLine
   );

   fprintf(
      (FILE *) inFILE,
      "  -v: print the version number and exit%s",
      str_endLine
   );

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun02 Sec03:
   ^   - output
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   fprintf((FILE *) inFILE, "Output:%s", str_endLine);

   fprintf(
      (FILE *) inFILE,
      "  - prints the MST to a newick file (-out)%s",
      str_endLine
   );
} /*phelp_mainMkMST*/

/*-------------------------------------------------------\
| Fun03: input_mainMkMST
|   - get user input from the args array
| Input:
|   - argLenSI:
|     o number arguments the user input
|   - argAryStr:
|     o cstring array with the user input
|   - inFileStrPtr:
|     o gets input (feature table) file path
|   - nodeSIPtr:
|     o gets the node to start on (set to index 0)
|   - outFileStrPtr:
|     o gets output file path
| Output:
|   - Modifies:
|     o inFileStrPtr, nodeSIPtr, and outFileStrPtr to have
|       the user input
|   - Returns:
|     o 0 for no errors
|     o 1 if printed the help message or version number
|     o 2 for an error
\-------------------------------------------------------*/
signed char
input_mainMkMST(
   int argLenSI,               /*number of input items*/
   char *argAryStr[],          /*user input*/
   signed char **inFileStrPtr, /*gets input file path*/
   signed int *nodeSIPtr,      /*gets node to start at*/
   signed char **outFileStrPtr /*gets output file path*/
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun03 TOC:
   '   - get user input from the args array
   '   o fun03 sec01:
   '     - variable declarations and quick checks
   '   o fun03 sec02:
   '     - get user input
   '   o fun03 sec03:
   '     - return
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec01:
   ^   - variable declarations and quick checks
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   signed int siArg = 1;
   signed char *tmpStr = 0;

   if(argLenSI <= 1)
      goto phelp_fun03_sec03;/*nothing input, print help*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec02:
   ^   - get user input
   ^   o fun03 sec02 sub01:
   ^     - feature table check + start loop
   ^   o fun03 sec02 sub02:
   ^     - output file check
   ^   o fun03 sec02 sub03:
   ^     - starting node check
   ^   o fun03 sec02 sub04:
   ^     - help message check
   ^   o fun03 sec02 sub05:
   ^     - version number check
   ^   o fun03 sec02 sub06:
   ^     - deal with invalid input
   ^   o fun03 sec02 sub07:
   ^     - move to the next argument
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun03 Sec02 Sub01:
   *   - feature table check + start loop
   \*****************************************************/

   while(siArg < argLenSI)
   { /*Loop: get user input*/
      if(
         ! eqlNull_ulCp(
            (signed char *) "-features",
            (signed char *) argAryStr[siArg]
         )
      ){ /*If: input feature table*/
         ++siArg;
         *inFileStrPtr = (signed char *) argAryStr[siArg];
      }  /*If: input feature table*/

      /**************************************************\
      * Fun03 Sec02 Sub02:
      *   - output file check
      \**************************************************/

      else if(
         ! eqlNull_ulCp(
            (signed char *) "-out",
            (signed char *) argAryStr[siArg]
         )
      ){ /*Else If: output file*/
         ++siArg;
         *outFileStrPtr=(signed char *) argAryStr[siArg];
      }  /*Else If: output file*/

      /**************************************************\
      * Fun03 Sec02 Sub03:
      *   - starting node check
      \**************************************************/

      else if(
         ! eqlNull_ulCp(
            (signed char *) "-node",
            (signed char *) argAryStr[siArg]
         )
      ){ /*Else If: node to start at*/
         ++siArg;
         tmpStr = (signed char *) argAryStr[siArg];
         tmpStr += strToSI_base10str(tmpStr, nodeSIPtr);

         if(*tmpStr)
         { /*If: non-numeric or to large*/
            fprintf(
               stderr,
               "-node %s is non-numeric or to large%s",
               argAryStr[siArg],
               str_endLine
            );
            goto err_fun03_sec03;
         } /*If: non-numeric or to large*/

         else if(*nodeSIPtr < 0)
         { /*Else If: invalid node value*/
            fprintf(
               stderr,
               "-node %s must be 1 or greater%s",
               argAryStr[siArg],
               str_endLine
            );
            goto err_fun03_sec03;
         } /*Else If: invalid node value*/

         else if(*nodeSIPtr)
            --(*nodeSIPtr); /*convert to index 0*/
      }  /*Else If: node to start at*/

      /**************************************************\
      * Fun03 Sec02 Sub04:
      *   - help message check
      \**************************************************/

      else if(
         ! eqlNull_ulCp(
            (signed char *) "-h",
            (signed char *) argAryStr[siArg]
         )
      ) goto phelp_fun03_sec03;

      else if(
         ! eqlNull_ulCp(
            (signed char *) "--h",
            (signed char *) argAryStr[siArg]
         )
      ) goto phelp_fun03_sec03;

      else if(
         ! eqlNull_ulCp(
            (signed char *) "help",
            (signed char *) argAryStr[siArg]
         )
      ) goto phelp_fun03_sec03;

      else if(
         ! eqlNull_ulCp(
            (signed char *) "-help",
            (signed char *) argAryStr[siArg]
         )
      ) goto phelp_fun03_sec03;

      else if(
         ! eqlNull_ulCp(
            (signed char *) "--help",
            (signed char *) argAryStr[siArg]
         )
      ) goto phelp_fun03_sec03;

      /**************************************************\
      * Fun03 Sec02 Sub05:
      *   - version number check
      \**************************************************/

      else if(
         ! eqlNull_ulCp(
            (signed char *) "-v",
            (signed char *) argAryStr[siArg]
         )
      ) goto pversion_fun03_sec03;

      else if(
         ! eqlNull_ulCp(
            (signed char *) "--v",
            (signed char *) argAryStr[siArg]
         )
      ) goto pversion_fun03_sec03;

      else if(
         ! eqlNull_ulCp(
            (signed char *) "version",
            (signed char *) argAryStr[siArg]
         )
      ) goto pversion_fun03_sec03;

      else if(
         ! eqlNull_ulCp(
            (signed char *) "-version",
            (signed char *) argAryStr[siArg]
         )
      ) goto pversion_fun03_sec03;

      else if(
         ! eqlNull_ulCp(
            (signed char *) "-version",
            (signed char *) argAryStr[siArg]
         )
      ) goto pversion_fun03_sec03;

      /**************************************************\
      * Fun03 Sec02 Sub06:
      *   - deal with invalid input
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
      * Fun03 Sec02 Sub07:
      *   - move to the next argument
      \**************************************************/

       ++siArg;
   } /*Loop: get user input*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec03:
   ^   - return
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   siArg = 0;
   goto ret_fun03_sec03;

   phelp_fun03_sec03:;
      phelp_mainMkMST(stdout);
      siArg = 1;
      goto ret_fun03_sec03;

   pversion_fun03_sec03:;
      pversion_mainMkMST(stdout);
      siArg = 1;
      goto ret_fun03_sec03;

   err_fun03_sec03:;
      siArg = 2;
      goto ret_fun03_sec03;

   ret_fun03_sec03:;
      return (signed char) siArg;
} /*input_mainMkMST*/

/*-------------------------------------------------------\
| Main:
|   - driver function for mainMkMST (makes a mst tree)
| Input:
|   - argLenSI:
|     o number arguments the user input
|   - argAryStr:
|     o cstring array with user input
| Output
|   - Prints:
|     o mst tree to stdout or -out <file>
|   - Returns:
|     o 0 for no errors
|     o 1 for an error
\-------------------------------------------------------*/
int
main(
   int argLenSI,
   char *argAryStr[]
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Main TOC:
   '   - driver function for mainMkMST (makes a mst tree)
   '   o main sec01:
   '     - variable declarations
   '   o main sec0x:
   '     - return and clean up
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   signed char *inFileStr = 0;  /*feature table for MST*/
   signed char *outFileStr = 0; /*newick file to save to*/
   signed int nodeSI = 0;       /*node to start on*/

   signed char errSC = 0;
   signed long errSL = 0;

   struct mst_runPrim *mstHeapST = 0; /*gets MST*/

   FILE *inFILE = 0;
   FILE *outFILE = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec02:
   ^   - get and check user input
   ^   o main sec02 sub01:
   ^     - get user input
   ^   o main sec02 sub02:
   ^     - open user input files
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Main Sec02 Sub01:
   *   - get user input
   \*****************************************************/

   errSC =
      input_mainMkMST(
         argLenSI,
         argAryStr,
         &inFileStr,
         &nodeSI,
         &outFileStr
      );

   if(errSC)
   { /*If: printed help/version or had an error*/
      --errSC; /*help/version number goes to 0; no error*/
      goto ret_main_sec0x;
   } /*If: printed help/version or had an error*/

   /*****************************************************\
   * Main Sec02 Sub02:
   *   - open user input files
   *   o main sec02 sub02 cat01:
   *     - open the feature table
   *   o main sec02 sub02 cat02:
   *     - open output file
   \*****************************************************/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Main Sec02 Sub02 Cat01:
   +   - open the feature table
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   inFILE = fopen((char *) inFileStr, "r");

   if(! inFILE)
   { /*If: could not open the feature table*/
      fprintf(
         stderr,
         "could not open -features %s%s",
         inFileStr,
         str_endLine
      );
      goto err_main_sec0x;
   } /*If: could not open the feature table*/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Main Sec02 Sub02 Cat02:
   +   - open output file
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   if(! outFileStr || outFileStr[0] == '-')
      outFILE = stdout;
   else
   { /*Else: user provided and output file*/
      outFILE = fopen((char *) outFileStr, "w");

      if(! outFILE)
      { /*If: could not open the output file*/
         fprintf(
            stderr,
            "could not open -out %s%s",
            outFileStr,
            str_endLine
         );
         goto err_main_sec0x;
      } /*If: could not open the feature table*/
   } /*Else: user provided and output file*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec03:
   ^   - build the minimum spanning tree
   ^   o main sec03 sub01:
   ^     - index the feature table
   ^   o main sec03 sub02:
   ^     - build and print the MST
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Main Sec03 Sub01:
   *   - index the feature table
   \*****************************************************/

   mstHeapST = indexFeatureFile_runPrim(inFILE, &errSC);

   if(! errSC)
      ;

   else if(errSC < 0)
   { /*Else If: memory error*/
      fprintf(
         stderr,
         "memory error indexing -features %s%s",
         inFileStr,
         str_endLine
      );
      goto err_main_sec0x;
   } /*Else If: memory error*/

   else
   { /*Else: empty file*/
      fprintf(
         stderr,
         "nothing in -features %s%s",
         inFileStr,
         str_endLine
      );
      goto err_main_sec0x;
   } /*Else: empty file*/

   /*****************************************************\
   * Main Sec03 Sub02:
   *   - build and print the MST
   \*****************************************************/

   errSL = build_mst_runPrim(inFILE, nodeSI, mstHeapST);

   if(! errSL)
      ;
   else if(errSL == 1)
   { /*Else If: invalid node*/
      fprintf(
         stderr,
         "tree in file %s does not have node number %i%s",
         inFileStr,
         nodeSI + 1,
         str_endLine
      );
      goto err_main_sec0x;
   } /*Else If: invalid node*/

   else if(errSL > 1)
   { /*Else If: memory error*/
      fprintf(
         stderr,
         "memory error buildng MST for -features %s%s",
         inFileStr,
         str_endLine
      );
      goto err_main_sec0x;
   } /*Else If: memory error*/

   else
   { /*Else: empty file*/
      fprintf(
         stderr,
         "file error on line %li in -features %s%s",
         errSL * -1,
         inFileStr,
         str_endLine
      );
      goto err_main_sec0x;
   } /*Else: empty file*/

   mstToNewick_mst_runPrim(mstHeapST, outFILE);

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec0x:
   ^   - return and clean up
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   errSC = 0;
   goto ret_main_sec0x;

   err_main_sec0x:;
      errSC = 1;
      goto ret_main_sec0x;

   ret_main_sec0x:;
      if(! inFILE)
         ;
      else if(inFILE == stdin)
         ;
      else if(inFILE == stderr)
         ;
      else if(inFILE != stdout)
         fclose(inFILE);
      inFILE = 0;


      if(! outFILE)
         ;
      else if(outFILE == stdin)
         ;
      else if(outFILE == stderr)
         ;
      else if(outFILE != stdout)
         fclose(outFILE);
      outFILE = 0;


      if(mstHeapST)
         freeHeap_mst_runPrim(mstHeapST);


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
