/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' unGz SOF: Start Of File
'   - uncompresses gz files
'   o header:
'     - included libraries
'   o fun01: pversion_unGz
'     - prints version for unGz
'   o fun02: phelp_unGz
'     - prints help message for unGz
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
#endif

#include <stdio.h>

#include "../genLib/ulCp.h"
#include "../genLib/inflate.h"

/*using only the .h file*/
#include "../genLib/endLine.h"
#include "../bioTools.h"

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\
! Hidden Libraries:
!   - std #include <stdlib.h>
!   - .c  #include "../genLib/endin.h"
!   - .c  #include "../genLib/checksum.h"
!   - .c  #include "../genLib/genMath.h"
\%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

/*-------------------------------------------------------\
| Fun01: pversion_unGz
|   - prints version for unGz
| Input:
|   - outFILE:
|     o FILE pointer to print version number to
| Output:
|   - Prints:
|     o version number to outFILE
\-------------------------------------------------------*/
void
pversion_unGz(
   void *outFILE
){
   fprintf(
      (FILE *) outFILE,
      "unGz from bioTools version: %i-%02i-%02i%s",
      def_year_bioTools,
      def_month_bioTools,
      def_day_bioTools,
      str_endLine
   );
} /*pversion_unGz*/

/*-------------------------------------------------------\
| Fun02: phelp_unGz
|   - prints help message for unGz
| Input:
|   - outFILE:
|     o FILE pointer to print help message to
| Output:
|   - Prints:
|     o help message to outFILE
\-------------------------------------------------------*/
void
phelp_unGz(
   void *outFILE
){
   fprintf(
      (FILE *) outFILE,
      "unGz file1.gz file2.gz file3.gz ... filen.gz%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "  - prints uncompressed output to stdout%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "  - use `-` for stdin input%s",
      str_endLine
   );
} /*phelp_unGz*/

/*-------------------------------------------------------\
| Main:
|   - driver function for unGz
| Input:
|   - argAryStr:
|     o c-string array with files to uncompress
|   - numArgsSI:
|     o number of files input
| Output:
|   - Prints:
|     o file contents to stdout
|     o errors to stderr
\-------------------------------------------------------*/
int
main(
   int numArgsSI,
   char *argAryStr[]
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Main TOC:
   '   - driver function for unGz
   '   o main sec01:
   '     - variable declarations
   '   o main sec02:
   '     - initialize and check for version/help message
   '   o main sec03:
   '     - unzip files
   '   o main sec04:
   '     - print errors, clean up, and return
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec01:
   ^   - variable declarations and initialize
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   #define def_buffSize_main 4096

   signed char errSC = 0;

   struct file_inflate zipStackST;
   signed char outStr[def_buffSize_main];
   signed long bytesSL = 0;

   int argSI = 1;
   FILE *inFILE = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec02:
   ^   - initialize and check for version/help message
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   for(bytesSL =0; bytesSL < def_buffSize_main; ++bytesSL)
      outStr[bytesSL] = '\0';
   bytesSL = 0;
   init_file_inflate(&zipStackST);

   if(numArgsSI <= 1)
   { /*If: nothing input*/
      phelp_unGz(stdout);
      goto noInput_main_sec04;
   } /*If: nothing input*/

   if(
         argAryStr[1][0] == '-'
      && argAryStr[1][1] == 'h'
   ){ /*If: user wanted help message*/
      errSC = 0;
      phelp_unGz(stdout);
      goto ret_main_sec04;
   } /*If: user wanted help message*/

   if(
         argAryStr[1][0] == '-'
      && argAryStr[1][1] == '-'
      && argAryStr[1][2] == 'h'
   ){ /*If: user wanted help message*/
      errSC = 0;
      phelp_unGz(stdout);
      goto ret_main_sec04;
   } /*If: user wanted help message*/

   if(
         argAryStr[1][0] == 'h'
      && argAryStr[1][1] == 'e'
      && argAryStr[1][2] == 'l'
      && argAryStr[1][3] == 'p'
      && argAryStr[1][4] == '\0'
   ){ /*If: user wanted help message*/
      errSC = 0;
      phelp_unGz(stdout);
      goto ret_main_sec04;
   } /*If: user wanted help message*/

   if(
         argAryStr[1][0] == '-'
      && argAryStr[1][1] == 'v'
   ){ /*If: user wanted version number*/
      errSC = 0;
      pversion_unGz(stdout);
      goto ret_main_sec04;
   } /*If: user wanted version number*/

   if(
         argAryStr[1][0] == '-'
      && argAryStr[1][1] == '-'
      && argAryStr[1][2] == 'v'
   ){ /*If: user wanted version number*/
      errSC = 0;
      pversion_unGz(stdout);
      goto ret_main_sec04;
   } /*If: user wanted version number*/

   if(
         argAryStr[1][0] == 'v'
      && argAryStr[1][1] == 'e'
      && argAryStr[1][2] == 'r'
      && argAryStr[1][3] == 's'
      && argAryStr[1][4] == 'i'
      && argAryStr[1][5] == 'o'
      && argAryStr[1][6] == 'n'
      && argAryStr[1][7] == '\0'
   ){ /*If: user wanted version number*/
      errSC = 0;
      pversion_unGz(stdout);
      goto ret_main_sec04;
   } /*If: user wanted version number*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec03:
   ^   - unzip files
   ^   o main sec03 sub01:
   ^     - add gz file to zipStackST + start loop
   ^   o main sec03 sub02:
   ^     - inflate file (inflate loop)
   ^   o main sec03 sub03:
   ^     - move to next file
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Main Sec03 Sub01:
   *   - add gz file to zipStackST + start loop
   \*****************************************************/

   while(argSI < numArgsSI)
   { /*Loop: inflate gz files*/
      inFILE = fopen(argAryStr[argSI], "rb");
      if(! inFILE)
         goto openErr_main_sec04;

      errSC = add_file_inflate(inFILE, &zipStackST, 0);
      inFILE = 0;

      if(errSC == def_eof_inflate)
      { /*If: nothing in file (likely)*/
         fprintf(
            stderr,
            "nothing in %s; moving to next file%s",
            argAryStr[argSI],
            str_endLine
         );
      } /*If: nothing in file (likely)*/

      else if(errSC)
         goto fileErr_main_sec04;

      else if(zipStackST.typeSC != def_gz_inflate)
         goto notGz_main_sec04;

      /**************************************************\
      * Main Sec03 Sub02:
      *   - inflate file (inflate loop)
      \**************************************************/

      while(! errSC)
      { /*Loop: inflate file*/
         uncompress_main_sec03:;

         bytesSL =
            get_inflate(
               &zipStackST,
               def_buffSize_main,
               0,
               outStr,
               &errSC
            );

         if(bytesSL >= 0)
            ; /*no errors*/
         else if(errSC == def_memErr_inflate)
            goto memErr_main_sec04;
         else if(errSC != def_eof_inflate)
            goto fileErr_main_sec04;

         printf("%s", outStr);
      } /*Loop: inflate file*/

      /**************************************************\
      * Main Sec03 Sub03:
      *   - move to next file
      \**************************************************/

      errSC = next_file_inflate(&zipStackST, 0, 0);

      if(! errSC)
         goto uncompress_main_sec03; /*more in file*/
      else if(errSC == def_badCheck_inflate)
         goto badCheck_main_sec04;
      else if(errSC != def_eof_inflate)
         goto fileErr_main_sec04;
      else
         ++argSI;

      blank_file_inflate(&zipStackST, 1);
   } /*Loop: inflate gz files*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec04:
   ^   - print errors, clean up, and return
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   errSC = 0;
   goto ret_main_sec04;

   noInput_main_sec04:;
      fprintf(stderr, "nothing input%s", str_endLine);
      errSC = 1;
      goto ret_main_sec04;

   notGz_main_sec04:;
      fprintf(
         stderr,
         "%s is not a gz file%s",
         argAryStr[argSI],
         str_endLine
      );
      errSC = 2;
      goto ret_main_sec04;

   memErr_main_sec04:;
      fprintf(
         stderr,
         "memory error ucompressing %s%s",
         argAryStr[argSI],
         str_endLine
      );
      errSC = 3;
      goto ret_main_sec04;

   fileErr_main_sec04:;
      fprintf(
         stderr,
         "%s is not in gz format%s",
         argAryStr[argSI],
         str_endLine
      );
      errSC = 4;
      goto ret_main_sec04;

   openErr_main_sec04:;
      fprintf(
         stderr,
         "could not open %s%s",
         argAryStr[argSI],
         str_endLine
      );
      errSC = 5;
      goto ret_main_sec04;

   badCheck_main_sec04:;
      fprintf(
         stderr,
         "gz file %s is corupted%s",
         argAryStr[argSI],
         str_endLine
      );
      errSC = 6;
      goto ret_main_sec04;

   ret_main_sec04:;
      freeStack_file_inflate(&zipStackST);
      if(
            inFILE
         && inFILE != stdin
         && inFILE != stdout
         && inFILE != stderr
      ) fclose(inFILE);
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
