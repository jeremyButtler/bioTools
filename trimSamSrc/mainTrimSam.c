/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' mainTrimSam SOF: Start Of File
'   - driver for triming sam file entries 
'   o fun01: pversion_mainTrimSam
'     - prints version number
'   o fun02: phelp_mainTrimSam
'     - prints help message
'   o fun03: input_mainTrimSam
'     - gets user input from input arguments
'   o main:
'     -  main function to glue everything together
'   o license:
'     - licensing for this code (CC0)
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*I still will have issues with fprintf on plan9, so this
`  is just one part of the problem
*/
#ifdef PLAN9
   #include <u.h>
   #include <libc.h> /*think includes <string.h>*/
#else
   #include <stdlib.h>
   #include <string.h>
#endif

#include <stdio.h>

#include "../genBio/trimSam.h"
#include "../genBio/samEntry.h"

/*no .c files*/
#include "../genLib/endLine.h"
#include "../bioTools.h"

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\
! Hidden libraries:
!   - .c  #include "../genLib/ulCp.h"
!   - .c  #include "../genLib/numToStr.h"
!   - .c  #include "../genLib/base10str.h"
!   - .c  #include "../genLib/fileFun.h"
!   - .h  #include "../genBio/ntTo5Bit.h"
\%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

#define def_keepNoMap_mainTrimSam 0

/*-------------------------------------------------------\
| Fun01: pversion_mainTrimSam
|   - prints version number
| Input:
|   - outFILE:
|     o file to print version number to
| Output:
|   - Prints:
|     o version number to outFILE
\-------------------------------------------------------*/
void
pversion_mainTrimSam(
   void *outFILE
){
   fprintf(
      (FILE *) outFILE,
      "trimSam from bioTools version: %i-%02i-%02i%s",
      def_year_bioTools,
      def_month_bioTools,
      def_day_bioTools,
      str_endLine
   );
} /*pversion_mainTrimSam*/

/*-------------------------------------------------------\
| Fun02: phelp_mainTrimSam
|   - prints help message
| Input:
|   - outFILE:
|     o file to print help message to
| Output:
|   - Prints:
|     o help message to outFILE
\-------------------------------------------------------*/
void
phelp_mainTrimSam(
   void *outFILE
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun02 TOC:
   '   - prints help message
   '   o fun02 sec01:
   '      - usage block
   '   o fun02 sec02:
   '      - input block
   '   o fun02 sec03:
   '      - output block
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun02 Sec01:
   ^   - usage block
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   fprintf(
      (FILE *) outFILE,
      "trimSam -sam reads.sam [options ...] > out.sam%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "  - removes softmaskng from reads in a sam file%s",
      str_endLine
   );

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun02 Sec02:
   ^   - input block
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   fprintf(
      (FILE *) outFILE,
      "Input:%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "  -sam reads.sam: [Required; stdin]%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "    o sam file with reads to trim%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "  -out out.sam: [stdout]%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "    o sam file to print trimmed reads to%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "  -keep-no-map: [Optional; No]%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "    o keep unmapped reads%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "    o disable with -no-unmapped%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "  -h: prints this help message and exits%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "  -v: prints version number and exits%s",
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
      "   - prints trimmed reads to stdout or -out%s",
      str_endLine
   );
} /*phelp_mainTrimSam*/

/*-------------------------------------------------------\
| Fun03: input_mainTrimSam
|   - gets user input from input arguments
| Input:
|   - lenArgsSI:
|     o number of aguments input
|   - argAryStr:
|     o c-string array with user input
|   - samFileStr:
|     o pointer to c-string to point to sam file name in
|       argAryStr
|   - outFileStr:
|     o pointer to c-string to point to output file name
|       in argAryStr
|   - keepNoMapBl:
|     o set to 1 if keeping unmapped reads
|     o set to 0 if discarding unmapped reads
| Output:
|   - Modifies:
|     o input variables (except lenArgsSI and argAryStr)
|       to hold or point to user input
|   - Returns:
|     o 0: if valid input
|     o 1: if printed help message/version number
|     o 2: if invalid input
\-------------------------------------------------------*/
char input_mainTrimSam(
    int lenArgsSI,           /*number arguments input*/
    char *argAryStr[],       /*array with user arguments*/
    signed char **samFileStr,/*file name of input file*/
    signed char **outFileStr,/*file name of output file*/
    signed char *keepNoMapBl /*keeping unmapped reads?*/
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun03 TOC:
   '   - gets user input from input arguments
   '   o fun03 sec01:
   '     - variable declerations
   '   o fun03 sec02:
   '     - check if no input provided
   '   o fun03 sec03:
   '     - get user input
   '   o fun03 sec04:
   '     - clean up
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

    /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
    ^ Fun03 Sec01:
    ^   - variable declerations
    \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

    signed char errSC = 0;
    signed int siArg = 1;

    /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
    ^ Fun03 Sec02:
    ^   - check if no input provided
    \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

    if(lenArgsSI <= 1)
    { /*If: no user input; print help message*/
       phelp_mainTrimSam(stdout);
       goto phelp_fun03_sec04;
    } /*If: no user input; print help message*/

    /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
    ^ Fun03 Sec03:
    ^   - get user input
    ^   o fun03 sec03 sub01:
    ^     - start loop and check for normal input
    ^   o fun03 sec03 sub02:
    ^     - check for help message requests
    ^   o fun03 sec03 sub03:
    ^     - check for version number requests
    ^   o fun03 sec03 sub04:
    ^     - handel invalid input
    ^   o fun03 sec03 sub05:
    ^     - move to next argument
    <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

    /****************************************************\
    * Fun03 Sec03 Sub01:
    *   - start loop and check for normal input
    \****************************************************/

    while(siArg < lenArgsSI)
    { /*loop through all user input arguments*/
        if(! strcmp("-sam", argAryStr[siArg]))
        { /*If: This is the input file*/
           ++siArg;
           *samFileStr = (signed char *) argAryStr[siArg];
        } /*If: This is the input file*/

        else if(! strcmp("-out", argAryStr[siArg]))
        { /*Else If: This is the output file*/
           ++siArg;
           *outFileStr = (signed char *) argAryStr[siArg];
        } /*Else If: This is the output file*/

        else if(! strcmp("-keep-no-map",argAryStr[siArg]))
           *keepNoMapBl = 1;

        else if(! strcmp("-no-unmapped",argAryStr[siArg]))
           *keepNoMapBl = 0;

        /************************************************\
        * Fun03 Sec03 Sub02:
        *   - check for help message requests
        \************************************************/

        else if(! strcmp("-h", argAryStr[siArg]))
        { /*Else If: was help message*/
           phelp_mainTrimSam(stdout);
           goto phelp_fun03_sec04;
        } /*Else If: was help message*/

        else if(! strcmp("--h", argAryStr[siArg]))
        { /*Else If: was help message*/
           phelp_mainTrimSam(stdout);
           goto phelp_fun03_sec04;
        } /*Else If: was help message*/

        else if(! strcmp("help", argAryStr[siArg]))
        { /*Else If: was help message*/
           phelp_mainTrimSam(stdout);
           goto phelp_fun03_sec04;
        } /*Else If: was help message*/

        else if(! strcmp("-help", argAryStr[siArg]))
        { /*Else If: was help message*/
           phelp_mainTrimSam(stdout);
           goto phelp_fun03_sec04;
        } /*Else If: was help message*/

        else if(! strcmp("--help", argAryStr[siArg]))
        { /*Else If: was help message*/
           phelp_mainTrimSam(stdout);
           goto phelp_fun03_sec04;
        } /*Else If: was help message*/

        /************************************************\
        * Fun03 Sec03 Sub03:
        *   - check for version number requests
        \************************************************/

        else if(! strcmp("-v", argAryStr[siArg]))
        { /*Else If: was version number*/
           pversion_mainTrimSam(stdout);
           goto pversion_fun03_sec04;
        } /*Else If: was version number*/

        else if(! strcmp("--v", argAryStr[siArg]))
        { /*Else If: was version number*/
           pversion_mainTrimSam(stdout);
           goto pversion_fun03_sec04;
        } /*Else If: was version number*/

        else if(! strcmp("version", argAryStr[siArg]))
        { /*Else If: was version number*/
           pversion_mainTrimSam(stdout);
           goto pversion_fun03_sec04;
        } /*Else If: was version number*/

        else if(! strcmp("-version", argAryStr[siArg]))
        { /*Else If: was version number*/
           pversion_mainTrimSam(stdout);
           goto pversion_fun03_sec04;
        } /*Else If: was version number*/

        else if(! strcmp("--version", argAryStr[siArg]))
        { /*Else If: was version number*/
           pversion_mainTrimSam(stdout);
           goto pversion_fun03_sec04;
        } /*Else If: was version number*/

        /************************************************\
        * Fun03 Sec03 Sub04:
        *   - handel invalid input
        \************************************************/

        else
        { /*Else: invalid input*/
           fprintf(
              stderr,
              "%s is not recognized%s",
              argAryStr[siArg],
              str_endLine
           );

           goto err_fun03_sec04;
        } /*Else: invalid input*/

        /************************************************\
        * Fun03 Sec03 Sub05:
        *   - move to next argument
        \************************************************/

       ++siArg;
    } /*loop through all user input arguments*/

    /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
    ^ Fun03 Sec04:
    ^   - clean up
    <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

    errSC = 0;
    goto cleanUp_fun03_sec04;

    phelp_fun03_sec04:;
       errSC = 1;
       goto cleanUp_fun03_sec04;

    pversion_fun03_sec04:;
       errSC = 1;
       goto cleanUp_fun03_sec04;

    err_fun03_sec04:;
       errSC = 2;
       goto cleanUp_fun03_sec04;

    cleanUp_fun03_sec04:;
       return errSC;
} /*input_mainTrimSam*/

/*-------------------------------------------------------\
| Main:
|   - driver function for trimSam
| Input:
|   - lenArgsSI:
|     o number of aguments input
|   - argAryStr:
|     o c-string array with user input
| Output:
|   - prints trimmed reads to output file
\-------------------------------------------------------*/
int
main(
   int lenArgsSI,
   char *argsPtrCStr[]
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Main TOC: main
   '   o main sec01:
   '     - variable declarations
   '   o main sec02:
   '     - get input and open files
   '   o main sec03:
   '     - trim reads
   '   o main Sec04:
   '     - clean up
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

    /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
    ^ Main Sec01:
    ^   - variable declarations
    \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

    signed char *samFileStr = 0;     /*sam file to trim*/
    signed char *outFileStr = 0;     /*output sam file*/
    signed char keepNoMapBl = def_keepNoMap_mainTrimSam;
    signed char errSC = 0;

    signed long lineSL = 0;
    signed long seqSL = 0;

    struct samEntry samStackST;

    FILE *samFILE = 0;
    FILE *outFILE = 0;

    /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
    ^ Main Sec02:
    ^   - get input and open files
    ^   o main sec02 sub01:
    ^     - get input
    ^   o main sec02 sub02:
    ^     - open input sam file
    ^   o main sec02 sub03:
    ^     - open ouput file (sam)
    \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

    /****************************************************\
    * Main Sec02 Sub01:
    *   - get input
    \****************************************************/

    init_samEntry(&samStackST);

    errSC =
        input_mainTrimSam(
            lenArgsSI,     /*number argumnets input*/
            argsPtrCStr,   /*input arguments*/
            &samFileStr,   /*input sam file*/
            &outFileStr,   /*output file*/
            &keepNoMapBl   /*Keep unammped reads?*/
    ); /*Get user input*/

    if(errSC)
    { /*If: had error*/
       --errSC; /*reduce help/versoin numbers to 0*/
       goto cleanUp_main_sec04;
    } /*If: had error*/

    errSC = setup_samEntry(&samStackST);

    if(errSC)
    { /*If: memory error*/
       fprintf(
          stderr,
          "memory error setting up sam entry%s",
          str_endLine
       );

       goto memErr_main_sec04;
    } /*If: memory error*/
       
    /****************************************************\
    * Main Sec02 Sub02:
    *   - open input sam file
    \****************************************************/

    if(! samFileStr || samFileStr[0] == '-')
       samFILE = stdin;

    else
    { /*Else: using a file for input*/
        samFILE =
           fopen(
              (char *) samFileStr,
              "r"
           );
    
        if(samFILE == 0)
        { /*If: could not open sam file*/
            fprintf(
                stderr,
                "could not open -sam %s%s",
                samFileStr,
                str_endLine
            ); /*If file was not valid*/

            goto fileErr_main_sec04;
        } /*If: could not open sam file*/
    } /*Else: using a file for input*/

    /****************************************************\
    * Main Sec02 Sub03:
    *   - open ouput file (sam)
    \****************************************************/

    if(! outFILE || outFileStr[0] == '-')
       outFILE = stdout;

    else
    { /*Else: user provided output file*/
        outFILE =
           fopen(
              (char *) outFileStr,
              "w"
           );

        if(outFILE == 0)
        { /*If: could no open output file*/
            fprintf(
                stderr,
                "could not open -out (%s)%s",
                outFileStr,
                str_endLine
            );
                
            goto fileErr_main_sec04;
        } /*If: could no open output file*/
    } /*Else: user provided output file*/

    /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
    ^ Main Sec03:
    ^   - print headers
    ^   o main sec03 sub01:
    ^     - get first line in sam file
    ^   o main sec03 sub02:
    ^     - print headers in sam file
    ^   o main sec03 sub03:
    ^     - check for errors
    ^   o main sec03 sub04:
    ^     - print trimSam header
    \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

    /****************************************************\
    * Main Sec03 Sub01:
    *   - get first line in sam file
    \****************************************************/

    ++lineSL;
    errSC = get_samEntry(&samStackST, samFILE);

    if(errSC)
    { /*If: error*/
       if(errSC == def_memErr_samEntry)
       { /*If: memory error*/
          fprintf(
             stderr,
             "memory error first line in -sam %s%s",
             samFileStr,
             str_endLine
          );

          goto memErr_main_sec04;
       } /*If: memory error*/

       /*EOF*/
       fprintf(
          stderr,
          "nothing in -sam %s%s",
          samFileStr,
          str_endLine
       );

       goto fileErr_main_sec04;
    } /*If: error*/

    /****************************************************\
    * Main Sec03 Sub02:
    *   - print headers in sam file
    \****************************************************/

    while(! errSC)
    { /*Loop: print headers*/
       if(samStackST.extraStr[0] != '@')
          break;
       p_samEntry(&samStackST, 0, outFILE);
       errSC = get_samEntry(&samStackST, samFILE);
       ++lineSL;
    } /*Loop: print headers*/

    /****************************************************\
    * Main Sec03 Sub03:
    *   - check for errors
    \****************************************************/

    if(errSC)
    { /*If: error*/
       if(errSC == def_memErr_samEntry)
       { /*If: memory error*/
          fprintf(
             stderr,
             "memory error printing -sam %s headers%s",
             samFileStr,
             str_endLine
          );

          goto memErr_main_sec04;
       } /*If: memory error*/

       /*EOF*/
       fprintf(
          stderr,
          "no reads or bad line in -sam %s%s",
          samFileStr,
          str_endLine
       );

       goto fileErr_main_sec04;
    } /*If: error*/

    /****************************************************\
    * Main Sec03 Sub04:
    *   - print trimSam header
    \****************************************************/

    fprintf( 
       (FILE *) outFILE,
       "@PG\tID:trimSam\tPN:trimSam"
    );

    fprintf(
       (FILE *) outFILE,
       "\tVN:bioTools_%i-%02i-%02i\tCL:trimSam",
       def_year_bioTools,
       def_month_bioTools,
       def_day_bioTools
    );

    if(keepNoMapBl)
       fprintf((FILE *) outFILE, " -keep-no-map");

    if(samFILE != stdout)
       fprintf((FILE *) outFILE, " -sam %s", samFileStr);
    else
       fprintf((FILE *) outFILE, " -sam -");

    if(outFILE != stdout)
       fprintf((FILE *) outFILE, " -out %s", outFileStr);
    else
       fprintf((FILE *) outFILE, " -out -");

    fprintf((FILE *) outFILE, "%s", str_endLine);
    
    /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
    ^ Main Sec03:
    ^   - trim reads
    \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

    while(! errSC)
    { /*Loop: trim reads*/
        if(samStackST.extraStr[0] == '@')
           ; /*comment*/
        else if( samStackST.flagUS & (256 | 2048) )
           ; /*supplemental or secondary alignment*/
        else
           ++seqSL;

        /*Convert & print out sam file entry*/
        errSC = seq_trimSam(&samStackST);

        if(
              keepNoMapBl
           && errSC & def_noMap_trimSam
        ) errSC = 0;

        if(! errSC) 
           p_samEntry(&samStackST, 0, outFILE);
        
        errSC = get_samEntry(&samStackST, samFILE);
        ++lineSL;
    } /*Loop: trim reads*/

    if(errSC == def_memErr_samEntry)
    { /*If: had an error*/
       fprintf(
          stderr,
          "memory error on read %li, read %li%s",
          lineSL,
          seqSL,
          str_endLine
       );

       goto memErr_main_sec04;
    } /*If: had an error*/

    if(errSC != def_EOF_samEntry)
    { /*If: had an error*/
       fprintf(
          stderr,
          "file error on line %li and read %li%s",
          lineSL,
          seqSL,
          str_endLine
       );

       goto fileErr_main_sec04;
    } /*If: had an error*/

    /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
    ^ Main Sec04:
    ^   - clean up
    \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

    errSC = 0;
    goto cleanUp_main_sec04;

    memErr_main_sec04:;
       errSC = def_memErr_trimSam;
       goto cleanUp_main_sec04;

    fileErr_main_sec04:;
       errSC = def_fileErr_trimSam;
       goto cleanUp_main_sec04;

    cleanUp_main_sec04:;
       freeStack_samEntry(&samStackST);

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

       return errSC;
} /*main function*/

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
