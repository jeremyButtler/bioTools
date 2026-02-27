/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' mapClust SOF: Start Of File
'   - clustering reads by using a read mapper to find
'     similar reads (denovo)
'   o header:
'     - included libraries
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*-------------------------------------------------------\
| Header:
|   - included libraries
\-------------------------------------------------------*/

#ifndef PLAN9
   #include <libc.h>
#else
   #include <stdlib.h>
#endif

#include <stdio.h>
#include "mapClust.h"
#include "../genAln/mapRead.h"

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\
! Hidden Libraries:
!   - .c  #include "../genLib/base10Str.h"
!   - .c  #include "../genLib/charCp.h"
!   - .c  #include "../genLib/strAry.h"
!   - .c  #include "../genLib/numToStr.h"
!   - .c  #include "../genLib/ulCp.h"
!   - .c  #include "../genLib/shellSort.h"
!   - .c  #include "../genLib/fileFun.h"
!   - .c  #include "../genBio/kmerFun.h"
!   - .c  #include "../genBio/seqST.h"
!   - .c  #include "../genBio/samEntry.h"
!   - .c  #include "../genAln/alnSet.h"
!   - .c  #include "../genAln/dirMatrix.h"
!   - .c  #include "../genAln/needle.h"
!   - .c  #include "../genAln/water.h"
!   - .c  #include "../genAln/indexToCoord.h"
!   - .c  #include "../genAln/memwater.h"
!   - .h  #include "../genBio/ntTo5Bit.h"
!   - .h  #include "../genLib/endLine.h"
!   - .h  #include "../genBio/kmerBit.h"
!   - .h  #include "../genAln/alnDefs.h"
!   - .h  #include "../genAln/defsMapRead.h"
!   - .h  #include "../genLib/genMath.h"
\%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/


/*-------------------------------------------------------\
| Fun0x: fqIndex_mapClust
|   - index a fastq file for extracting reads
| Input:
|    - fqFILE:
|      o FILE pointer to fastq file to index reads with
|    - errSLPtr:
|      o signed long pionter to get errors
| Output:
|   - Modifies:
|     o errSLPtr:
|       * 0 for no errors
|       * 1 for an empty file
|       * line + 1 for an file error
|       * -1 for memory error
|   - Returns:
|     o a pointer to an index_mapClust structure
|     o 0 for an error
\-------------------------------------------------------*/
struct index_mapClust
fqIndex_mapClust(
   void *fqFILE,
   signed long *errSLPtr
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun0x TOC:
   '   - index a fastq file for extracting reads
   '   o fun0x sec01:
   '     - variable declarations
   '   o fun0x sec0x:
   '     - clean up and return
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun0x Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   # define lenBuff_fun0x 4096
   signed char *buffHeapStr[lenBuff_fun0x + 8];
   signed int lenSI = 0;
   signed long lineSL = 0;
   struct index_mapClust *indexHeapSTPtr = 0;

   signed int posSI = 0;     /*position at in buffer*/
   unsigned long bytesUL = 0;/*number of bytes from file*/
   signed char fastqEntryUC = 0; /*marks fastq entry on*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   indexHeapSTPtr = malloc(sizeof(struct index_mapClust));
   if(! indexHeapSTPtr)
      goto memErr_fun0x_sec0x;
   init_index_mapClust(indexHeapSTPtr);

   /*this is a bit inefficent. However, I am also going
   `   to be repeatedly go through the same file again
   `   and again, so the inefficeny is minimal
   */
   lineSL = lineCnt_fileFun(fqFILE, &lineSL);
   if(! lineSL)
      goto emptyFile_fun0x_sec0x;

   if( setupMem_index_mapClust(indexHeapSTPtr, lineSL) )
      goto memErr_fun0x_sec0x;
   goto getLine_fun0x_sec0x_sub0x;

   bytesUL = 0;
   fastqEntryUC = 3;

   while(lenSI)
   { /*Loop: read the entire file*/
      posSI = endStr_ulCp(&buffStr[posSI]);

      if(! buffStr[posSI])
      { /*If: no new line (found null)*/
         bytesUL += posSI;
         goto getLine_fun0x_sec0x_sub0x;
      } /*If: no new line (found null)*/

      ++fastqEntryUC;
      if(fastqEntryUC == 3)
      { /*If: found a fastq header*/
      } /*If: found a fastq header*/

      getLine_fun0x_sec0x_sub0x:;
         lenSI =
            fread(
               buffStr,
               sizeof(signed char), 
               lenBuff_fun0x,
               (FILE *) fqFILE
            );
         buffStr[lenSI] = 0;
         posSI = 0;
   } /*Loop: read the entire file*/

   if(! lineSL)
    goto emptyFile_fun0x_sec0x:;
            

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun0x Sec0x:
   ^   - clean up and return
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   *errSLPtr = 0;
   goto ret_fun0x_sec0x;

   emptyFile_fun0x_sec0x:;
      *errSLPtr = 1;
      goto errClean_fun0x_sec0x;

   fileErr_fun0x_sec0x:;
      *errSLPtr = lineSL + 1;
      goto errClean_fun0x_sec0x;

   memErr_fun0x_sec0x:;
      *errSLPtr = -1;
      goto errClean_fun0x_sec0x;

   errClean_fun0x_sec0x:;
      if(indexHeapSTPtr)
         freeHeap_index_mapClust(indexHeapSTPtr);
      indexHeapSTPtr = 0;
      goto ret_fun0x_sec0x;

   ret_fun0x_sec0x:;
      return indexHeapSTPtr;
} /*fqIndex_mapClust*/
