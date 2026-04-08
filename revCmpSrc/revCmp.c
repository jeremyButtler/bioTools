/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' revCmp SOF: Start Of File
'   - reverse complements all sequences in a fasta file
'   o header:
'     - included libraries
'   o fun01: pversion_revCmp
'     - Prints version number
'   o fun02 pHelpMsg:
'     - Prints out the help message
'   o main main:
'     - Reverse complements all sequences in a fasta file
'   o license:
'     - licensing for this code (CC0)
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*--------------------------------------------------------\
| Header:
|  - Includes
\--------------------------------------------------------*/

#ifdef PLAN9
   #include <u.h>
   #include <libc.h>
#else
   #include <stdlib.h>
#endif

#include <stdio.h>

#include "../genLib/ulCp.h"
#include "../genBio/seqST.h"

/*.h files only*/
#include "../genLib/endLine.h"
#include "../bioTools.h"

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\
! Hidden Libraries:
!   - .c  #include "../genLib/fileFun.h"
!   - .c  #include "../genLib/ulCp.h"
\%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

/*--------------------------------------------------------\
| Fun01: pversion_revCmp
|  - Prints version number
| Input:
|  - outFILE:
|    o Pointer to FILE to print to
| Output:
|  - Prints:
|    o version number to outFILE
\--------------------------------------------------------*/
void
pversion_revCmp(
   void *outFILE
){
   fprintf(
     (FILE *) outFILE,
     "revCmp from bioTools version: %i-%02i-%02i%s",
     def_year_bioTools,
     def_month_bioTools,
     def_day_bioTools,
     str_endLine
   );
} /*pversion_revCmp*/


/*--------------------------------------------------------\
| Fun02: phelp_revCmp
|  - Prints out the help message
| Input:
|  - outFILE:
|    o Pointer to FILE to print to
| Output:
|  - Prints:
|    o help message to outFILE
\--------------------------------------------------------*/
void
phelp_revCmp(
   void *outFILE
){
   fprintf(
       (FILE *) outFILE,
       "revCmpSeq -out out.fa file_1.fa file_2.fa ...%s",
       str_endLine
   );

   fprintf(
     (FILE *) outFILE,
     "  - reverse complement sequences in fasta a file%s",
     str_endLine
   );


   fprintf((FILE *) outFILE, "Input:%s", str_endLine);
   fprintf(
     (FILE *) outFILE,
     "  file.fa: [Required]%s",
     str_endLine
   );

   fprintf(
     (FILE *) outFILE,
     "    o Fasta file with sequences to reverse%s",
     str_endLine
   );

   fprintf(
     (FILE *) outFILE,
     "      complement%s",
     str_endLine
   );


   fprintf(
     (FILE *) outFILE,
     "  -out out.fa: [Optional: stdout]%s",
     str_endLine
   );
   fprintf(
     (FILE *) outFILE,
     "    o File to save reverse complement seqs to%s",
     str_endLine
   );
   fprintf(
     (FILE *) outFILE,
     "    o Use `-` for stdout%s",
     str_endLine
   );
   fprintf(
     (FILE *) outFILE,
     "    o This must be the first arugment%s",
     str_endLine
   );


   fprintf((FILE *) outFILE, "Output:%s", str_endLine);
   fprintf(
     (FILE *) outFILE,
     "  - Reverse complement sequences to stdout%s",
     str_endLine
   );
} /*phelp_revCmp*/

/*--------------------------------------------------------\
| Main: main
|  - Reverse complements the input sequence
| Input:
|  - numArgsSI:
|    o Number of arguments the user input
|  - argAryStr:
|    o Arguments and parameters the user input
| Output:
|  - Prints:
|    o reverse complement sequence to stdout
\--------------------------------------------------------*/
int main(
   int numArgsSI,   /*Number of arguments the user input*/
   char *argAryStr[] /*Arguments/parametersminput by user*/
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Main: main
   '  - Reverse complements sequences in argument 1
   '  o main sec01:
   '    - Variable declerations
   '  o main sec02:
   '    - Check user input and initialize variables
   '  o main sec03:
   '    - Get and reverse complement fasta entries
   '  o main sec04:
   '    - Clean up and exit
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec01:
   ^  - Variable declerations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   signed char errSC = 0;
   signed int numSeqSI = 0;
   signed int siArg = 1;
   signed char stdinBl = 0;

   FILE *inFILE = 0;
   FILE *outFILE = stdout;
   struct seqST seqStackST;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec02:
   ^  - Check user input and initialize variables
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   init_seqST(&seqStackST);

   if(numArgsSI < 2)
   { /*If: printing help message*/
      phelp_revCmp(stdout);
      goto clean_main_sec04;
   } /*If: printing help message*/


   if(argAryStr[1][0] == '-')
   { /*If: help message, output file or version number*/
      if(argAryStr[1][1] == '-')
      { /*If: used two dashes*/
         if(argAryStr[1][2] == 'h')
         { /*Else If: likely help message*/
            phelp_revCmp(stdout);
            goto clean_main_sec04;
         } /*Else If: likely help message*/

         else if(argAryStr[1][2] == 'v')
         { /*Else If: likely version number*/
            pversion_revCmp(stdout);
            goto clean_main_sec04;
         } /*Else If: likely version number*/

         else if(argAryStr[1][2] == 'o')
         { /*Else If: likley output file*/
            if(argAryStr[2][0] != '-')
            { /*If: file was input*/
               outFILE = 0;
               outFILE = fopen(argAryStr[2], "w");
               if(! outFILE)
                  goto outFileErr_main_sec04;
            } /*If: file was input*/
 
            siArg = 3;
         } /*Else If: likely output file*/

         else
            goto unkownInput_main_sec04; /*no idea*/
      } /*If: used two dashes*/

      else if(! argAryStr[1][1])
         ; /*uer wants stdin input*/

      else if(argAryStr[1][1] == 'h')
      { /*Else If: user likely requested help message*/
         phelp_revCmp(stdout);
         goto clean_main_sec04;
      } /*Else If: user likely requested help message*/

      else if(argAryStr[1][1] == 'v')
      { /*Else If: user likely requested version number*/
         pversion_revCmp(stdout);
         goto clean_main_sec04;
      } /*Else If: user likely requested version number*/

      else if(argAryStr[1][1] == 'o')
      { /*Else If: user input output file*/
         if(argAryStr[2][0] != '-')
         { /*If: file was input*/
            outFILE = 0;
            outFILE = fopen(argAryStr[2], "w");
            if(! outFILE)
               goto outFileErr_main_sec04;
         } /*If: file was input*/
 
         siArg = 3;
      } /*Else If: user input output file*/

      else
         goto unkownInput_main_sec04; /*no idea what is*/
   } /*If: help message, output file or version number*/


   if(
      ! eqlNull_ulCp(
         (signed char *) "help",
         (signed char *) argAryStr[1]
      )
   ){ /*If: the help message was requested*/
      phelp_revCmp(stdout);
      goto clean_main_sec04;
   } /*If: the help message was requested*/

   else if(
      ! eqlNull_ulCp(
         (signed char *) "version",
         (signed char *) argAryStr[1]
      )
   ){ /*Else If: version number requested*/
      pversion_revCmp(stdout);
      goto clean_main_sec04;
   } /*Else If: version number requested*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec03:
   ^  - Get and reverse complement fasta entries
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   while(siArg < numArgsSI)
   { /*Loop: got through fasta files*/
      if(! inFILE) ;
      else if(inFILE == stdin) ;
      else
         fclose(inFILE);
      inFILE = 0;

      if(argAryStr[siArg][0] == '-')
      { /*If: stdin input*/
         if(argAryStr[siArg][1])
            goto unkownInput_main_sec04;

         if(stdinBl)
         { /*If: mulitple stdin inputs requested*/
            ++siArg;
            continue;
         } /*If: mulitple stdin inputs requested*/

         inFILE = stdin;
         stdinBl = 1;
      } /*If: stdin input*/

      else
      { /*Else: need to open the file*/
         inFILE = fopen(argAryStr[siArg], "r");
         if(! inFILE)
            goto faOpenErr_main_sec04;
      } /*Else: need to open the file*/

      blank_seqST(&seqStackST);
      errSC = getFa_seqST(inFILE, &seqStackST);
      numSeqSI = 0;

      if(errSC == def_memErr_seqST)
         goto memErr_main_sec04;
      else if(errSC)
         goto faFileErr_main_sec04;

      while(! errSC)
      { /*Loop: though all sequences in the fasta file*/
         ++numSeqSI;

         revComp_seqST(&seqStackST);

         fprintf(
            outFILE,
            ">%s%s%s%s",
            seqStackST.idStr,
            str_endLine,
            seqStackST.seqStr,
            str_endLine
         ); /*Print out the sequence*/

         blank_seqST(&seqStackST);
         errSC = getFa_seqST(inFILE, &seqStackST);
      } /*LoopCheck user input*/

      if(errSC == def_memErr_seqST)
         goto memErr_main_sec04;
      else if(errSC != def_EOF_seqST)
         goto faFileErr_main_sec04;
      else
         errSC = 0;

      ++siArg;
   } /*Loop: got through fasta files*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec04:
   ^  - Clean up and exit
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   errSC = 0;
   goto clean_main_sec04;

   memErr_main_sec04:;
      fprintf(stderr, "memory error%s", str_endLine);
      errSC = 1;
      goto clean_main_sec04;

   outFileErr_main_sec04:;
      fprintf(
         stderr,
         "failed to open -out %s%s",
         argAryStr[2], /*-out is 1st value, file is 3rd*/
         str_endLine

      );
      errSC = 2;
      goto clean_main_sec04;

   faOpenErr_main_sec04:;
      fprintf(
         stderr,
         "failed to open %s%s",
         argAryStr[siArg],
         str_endLine

      );
      errSC = 3;
      goto clean_main_sec04;

   faFileErr_main_sec04:;
      fprintf(
         stderr,
         "line %i in %s is not a fasta entry%s",
         numSeqSI,
         argAryStr[siArg],
         str_endLine

      );
      errSC = 4;
      goto clean_main_sec04;

   unkownInput_main_sec04:;
      fprintf(
         stderr,
         "%s is not -out out.fa%s",
         argAryStr[siArg],
         str_endLine

      );
      errSC = 4;
      goto clean_main_sec04;

   clean_main_sec04:;
      freeStack_seqST(&seqStackST);

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
