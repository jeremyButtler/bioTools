/*#######################################################\
\#######################################################*/

#ifdef PLAN9
   #include <u.h>
   #include <libc.h>
#else
   #include <stdlib.h>
#endif

#include <stdio.h>

#include "../genLib/samEntry.h"
#include "edDist.h"

/*.h files only*/
#include "../genLib/dataTypeShortHand.h"

#define def_minIndelLen_mainClust 20
#define def_minSnpQ_mainClust 10

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\
! Hidden libraries:
!   o .c  #include "../genLib/ulCp.h"
!   o .c   #include "../genLib/base10str.h"
!   o .c   #include "../genLib/numToStr.h"
!   o .h   #include "../genLib/ntTo5Bit.h"
!   o .h   #include "../genLib/genMath.h" not using .c
\%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

int
main(
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Main TOC:
   '   - runs clustering program
   '   o main sec01:
   '     - variable declerations
   '   o main sec02:
   '     - get user input and open filesj
   '   o main sec03:
   '     - ??
   '   o main sec0x:
   '     - clean up
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec01:
   ^   - variable declerations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   schar *samFileStr =
      (schar *)
      "del.sam";
      /*"/home/reason/files/testing/diflu/map-cal2009-filt.sam";*/

   /*edit distance settings*/
   uint minIndelLenUI = def_minIndelLen_mainClust;
   uchar minQUC = def_minSnpQ_mainClust;

   schar errSC = 0;
   slong distSL = 0;

   uint indelEventsUI = 0;
   uint lenOverlapUI = 0;

   FILE *samFILE = 0;

   struct samEntry refStackST;
   struct samEntry qryStackST;
   schar *buffHeapStr = 0;
   ulong lenBuffUL = 0;
   ulong entryUL = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec02:
   ^   - get user input and open files
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   init_samEntry(&refStackST);
   init_samEntry(&qryStackST);

   errSC = setup_samEntry(&refStackST);

   if(errSC)
   { /*If: had memory error*/
      fprintf(
         stderr,
         "MEMORY error\n"
      );

      goto err_main_sec0x_sub02;
   } /*If: had memory error*/
   
   errSC = setup_samEntry(&qryStackST);

   if(errSC)
   { /*If: had memory error*/
      fprintf(
         stderr,
         "MEMORY error\n"
      );

      goto err_main_sec0x_sub02;
   } /*If: had memory error*/

   samFILE = 
      fopen(
         (char *) samFileStr,
         "r"
      );
       
   if(! samFILE)
   { /*If: could not open sam file*/
      fprintf(
         stderr,
         "could not open -sam %s\n",
         samFileStr
      );

      goto err_main_sec0x_sub02;
   } /*If: could not open sam file*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec03:
   ^   - ??
   ^   o main sec03 sub01:
   ^     - get refernce sequence
   ^   o main sec03 sub03:
   ^     - find edit distances
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Main Sec03 Sub01:
   *   - get refernce sequence
   \*****************************************************/

   errSC =
      (schar)
      get_samEntry(
         &refStackST,
         &buffHeapStr,
         &lenBuffUL,
         samFILE
      );

   while(! errSC)
   { /*Loop: get first seqeunce*/
      if(*refStackST.extraStr != '@')
         break;

      errSC =
         (schar)
         get_samEntry(
            &refStackST,
            &buffHeapStr,
            &lenBuffUL,
            samFILE
         );
   } /*Loop: get first seqeunce*/

   if(errSC)
   { /*If: had an error*/
      if(errSC == 64)
         fprintf(
            stderr,
            "MEMORY ERROR for frist read in -sam %s\n",
            samFileStr
         );
      else
         fprintf(
            stderr,
            "one or fewer reads in file -sam %s\n",
            samFileStr
         );

      goto err_main_sec0x_sub02;
   } /*If: had an error*/

   /*****************************************************\
   * Main Sec03 Sub02:
   *   - find edit distances
   \*****************************************************/

   entryUL = 1;

   printf("ref\tqry\tdist\tlen\tnum_large_indel\n");

   errSC =
      (schar)
      get_samEntry(
         &qryStackST,
         &buffHeapStr,
         &lenBuffUL,
         samFILE
      );

   while(! errSC)
   { /*Loop: get edit distances*/
      ++entryUL;

      distSL =
         readCmpDist_edDist(
            &qryStackST,
            &refStackST,
            minIndelLenUI,
            minQUC,
            &indelEventsUI
         );

      if(distSL >= 0)
         printf(
            "1\t%lu\t%li\t%u\t%u\n",
            entryUL,
            distSL,
            lenOverlapUI,
            indelEventsUI
         );
         
      errSC =
         (schar)
         get_samEntry(
            &refStackST,
            &buffHeapStr,
            &lenBuffUL,
            samFILE
         );
   } /*Loop: get edit distances*/

   if(errSC == 64)
   { /*If: had an error*/
      fprintf(
         stderr,
         "MEMORY ERROR reading %lu in -sam %s\n",
         entryUL,
         samFileStr
      );

      goto err_main_sec0x_sub02;
   } /*If: had an error*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec0x:
   ^   - clean up
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   errSC = 0;
   goto cleanUp_main_sec0x_sub0y;

   err_main_sec0x_sub02:;
   errSC = -1;
   goto cleanUp_main_sec0x_sub0y;

   cleanUp_main_sec0x_sub0y:;

   if(
         samFILE
      && samFILE != stdin
      && samFILE != stdout
   ) fclose(samFILE);

   freeStack_samEntry(&refStackST);
   freeStack_samEntry(&qryStackST);

   free(buffHeapStr);
   buffHeapStr = 0;

   exit(0); 
}
