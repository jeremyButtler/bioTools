/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' idSearch SOF: Start Of File
'   - holds functions for searching throug a file by id
'   o header:
'     - included libraries
'   o fun01: getFqSeq_idSearch
'     - extract fastq sequences from file by read ids
'   o fun02: getSamSeq_idSearch
'     - extract sam sequences from sam file by read ids
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

#include "idSearch.h"
#include <stdio.h>

#include "searchST.h"
#include "getSeq.h"

/*.h files only*/
#include "../genLib/dataTypeShortHand.h"

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\
! Hidden libraries:
!   - .c  #include "../genLib/ulCp.h"
!   - .h  #include "idLkTbl.h"
!   - .h  #include "genMath.h" max .h macro only
\%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

/*-------------------------------------------------------\
| Fun01: getFqSeq_idSearch
|   - extract fastq sequences from file by read ids
| Input:
|   - searchSTPtr:
|     o pointer to a searchST structure with read id
|       hash table or array to search
|   - revSearchBl:
|     o 1: print read ids not in seachSTPtr
|     o 0: print read ids in seachSTPtr
|   - fqFile:
|     o fastq file with reads to search
|   - outFile:
|     o output file to print extracted reads to
| Output:
|   - Prints:
|     o extracted reads to outFILE
|   - Returns:
|     o 0 for no errors
|     o fastq entry errored out on
|     o -1 for memory error
\-------------------------------------------------------*/
signed long
getFqSeq_idSearch(
   struct searchST *searchSTPtr,
   signed char revSearchBl,
   void *fqFILE,
   void *outFILE
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun01 TOC:
   '   o fun01 sec01:
   '     - variable declerations
   '   o fun01 sec02:
   '     - read first part of file & set print parameter
   '   o fun01 sec03:
   '     - read first part of file & set print parameter
   '   o fun01 sec04:
   '     - find reads matching ids (or if requested not)
   '   o fun01 sec05:
   '     - return if invalid file or fastq file
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun01 Sec01:
   ^   - variable declerations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   uint lenBuffUI = 1 << 17; /*around 65536*/
   schar buffStr[lenBuffUI + 1];

   schar outBuffStr[lenBuffUI + 1];
   ulong outPosUL = 0; /*byte to copy at*/

   uchar hashBl = !!(searchSTPtr->hashTblUL);

   ulong bytesUL = 0; /*number bytes in buffer*/
   slong entrySL = 0; /*for errors*/

   uchar errUC = 0;            /*holds errors*/

   ulong startUL = 0; /*start of read id*/
   ulong endUL = 0;   /*end of read id*/
   ulong idAryUL[17]; /*maximum libs allowed*/

   slong indexSL = -1; /*array index of found read id*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun01 Sec02:
   ^   - read in first part of file & set print parameter
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   revSearchBl = !! revSearchBl; /*convert to 1 or 0*/

   bytesUL =
      fread(
         (char *) buffStr,
         sizeof(schar),
         lenBuffUI - 1,
         (FILE *) fqFILE
      ); /*read in first chunk from file*/

   buffStr[bytesUL] = '\0';

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun01 Sec03:
   ^   - find reads matching ids (or if requested not)
   ^   o fun01 sec02 sub01:
   ^     0 start loop and convert read id
   ^   o fun01 sec02 sub02:
   ^     - find read id and print or skip to next entry
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun01 Sec02 Sub01:
   *   - start loop and convert read id
   \*****************************************************/

   while(! errUC)
   { /*Loop: till have search file for ids*/
      ++entrySL;

      endUL = startUL;

      endUL += 
         idToHexAry_maxLimb_searchST(
            &buffStr[startUL],
            idAryUL,
            searchSTPtr->maxLimbsUC
         );

      /**************************************************\
      * Fun01 Sec02 Sub02:
      *   - find read id and print or skip to next entry
      \**************************************************/

      if(hashBl)
      { /*If: searching the hash table*/
         indexSL =
            searchHash_searchST(
               searchSTPtr,
               idAryUL
            );
      } /*If: searching the hash table*/

      else
      { /*Else: binary search*/
         indexSL =
            searchId_searchST(
               searchSTPtr,
               idAryUL
            );
      } /*Else: binary search*/

      if((indexSL >= 0) ^ revSearchBl)
      { /*If: read id is in list*/
         errUC =
            ulfq_getSeq(
               buffStr,
               &startUL,
               outBuffStr,
               &outPosUL,
               lenBuffUI,
               &bytesUL,
               fqFILE,
               outFILE
            );
      } /*If: read id is in list*/

      else
      { /*Else: read id is not in list*/
         errUC =
            ulfq_getSeq(
               buffStr,
               &startUL,
               outBuffStr,
               &outPosUL,
               lenBuffUI,
               &bytesUL,
               fqFILE,
               0
            );
      } /*Else: read id is not in list*/
   } /*Loop: till have search file for ids*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun01 Sec04:
   ^   - return if invalid file or fastq file
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(outPosUL > 0)
      fprintf(
         (FILE *) outFILE,
         "%s",
         outBuffStr
      );

   if(errUC < 2)
      entrySL = 0; /*no errors*/

   return entrySL;
} /*getIds_idSearch*/

/*-------------------------------------------------------\
| Fun02: getSamSeq_idSearch
|   - extract sam sequences from sam file by read ids
| Input:
|   - searchSTPtr:
|     o pointer to a searchST structure with read id
|       hash table or array to search
|   - revSearchBl:
|     o 1: print read ids not in seachSTPtr
|     o 0: print read ids in seachSTPtr
|   - samFile:
|     o sam file with reads to search
|   - outFile:
|     o output file to print extracted reads to
| Output:
|   - Prints:
|     o extracted reads to outFILE
|   - Returns:
|     o 0 for no errors
|     o sam entry errored out on
|     o -1 for memory error
\-------------------------------------------------------*/
signed long
getSamSeq_idSearch(
   struct searchST *searchSTPtr,
   signed char revSearchBl,
   void *samFILE,
   void *outFILE
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun02 TOC:
   '   o fun02 sec01:
   '     - variable declerations
   '   o fun02 sec02:
   '     - read first part of file & set print parameter
   '   o fun02 sec03:
   '     - read first part of file & set print parameter
   '   o fun02 sec04:
   '     - find reads matching ids (or if requested not)
   '   o fun02 sec05:
   '     - return if invalid file or sam file
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun02 Sec01:
   ^   - variable declerations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   uint lenBuffUI = 1 << 17; /*around 65536*/
   schar buffStr[lenBuffUI + 1];

   schar outBuffStr[lenBuffUI + 1];
   ulong outPosUL = 0; /*byte to copy at*/

   uchar hashBl = !!(searchSTPtr->hashTblUL);

   ulong bytesUL = 0; /*number bytes in buffer*/
   slong entrySL = 0; /*for errors*/

   uchar errUC = 0;            /*holds errors*/

   ulong startUL = 0; /*start of read id*/
   ulong endUL = 0;   /*end of read id*/
   ulong idAryUL[17]; /*maximum libs allowed*/

   slong indexSL = -1; /*array index of found read id*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun02 Sec02:
   ^   - read in first part of file & set print parameter
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   revSearchBl = !! revSearchBl; /*convert to 1 or 0*/

   bytesUL =
      fread(
         (char *) buffStr,
         sizeof(schar),
         lenBuffUI - 1,
         (FILE *) samFILE
      ); /*read in first chunk from file*/

   buffStr[bytesUL] = '\0';

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun02 Sec03:
   ^   - find reads matching ids (or if requested not)
   ^   o fun02 sec02 sub01:
   ^     0 start loop and convert read id
   ^   o fun02 sec02 sub02:
   ^     - find read id and print or skip to next entry
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun02 Sec02 Sub01:
   *   - start loop and convert read id
   \*****************************************************/

   while(! errUC)
   { /*Loop: till have search file for ids*/
      ++entrySL;

      endUL = startUL;

      if(buffStr[startUL] == '@')
         goto printEntry_fun02_sec02_sub02;

      endUL += 
         idToHexAry_maxLimb_searchST(
            &buffStr[startUL],
            idAryUL,
            searchSTPtr->maxLimbsUC
         );

      /**************************************************\
      * Fun02 Sec02 Sub02:
      *   - find read id and print or skip to next entry
      \**************************************************/

      if(hashBl)
      { /*If: searching the hash table*/
         indexSL =
            searchHash_searchST(
               searchSTPtr,
               idAryUL
            );
      } /*If: searching the hash table*/

      else
      { /*Else: binary search*/
         indexSL =
            searchId_searchST(
               searchSTPtr,
               idAryUL
            );
      } /*Else: binary search*/

      if((indexSL >= 0) ^ revSearchBl)
      { /*If: read id is in list*/
         printEntry_fun02_sec02_sub02:;

         errUC =
            ulsam_getSeq(
               buffStr,
               &startUL,
               outBuffStr,
               &outPosUL,
               lenBuffUI,
               &bytesUL,
               samFILE,
               outFILE
            );
      } /*If: read id is in list*/

      else
      { /*Else: read id is not in list*/
         errUC =
            ulsam_getSeq(
               buffStr,
               &startUL,
               outBuffStr,
               &outPosUL,
               lenBuffUI,
               &bytesUL,
               samFILE,
               0
            );
      } /*Else: read id is not in list*/
   } /*Loop: till have search file for ids*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun02 Sec04:
   ^   - return if invalid file or sam file
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(outPosUL > 0)
      fprintf(
         (FILE *) outFILE,
         "%s",
         outBuffStr
      );

   if(errUC < 2)
      entrySL = 0; /*no errors*/

   return entrySL;
} /*getIds_idSearch*/
