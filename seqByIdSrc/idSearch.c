/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' idSearch SOF: Start Of File
'   - holds functions for searching throug a file by id
'   o header:
'     - included libraries
'   o fun01: getFqSeq_idSearch
'     - extract fastq sequences from file by read ids
'   o fun02: getSamSeq_idSearch
'     - extract sam sequences from sam file by read ids
'   o license:
'     - licensing for this code (public domain / mit)
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

#include "../genLib/ulCp.h"

#include "searchST.h"
#include "getSeq.h"

/*.h files only*/
#include "../genLib/dataTypeShortHand.h"

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\
! Hidden libraries:
!   - .h  #include "idLkTbl.h"
!   - .h  #include "genMath.h" max .h macro only
\%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

#define def_lenBuff_idSearch (1 << 17) /*around 65536*/

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

   schar buffStr[def_lenBuff_idSearch + 1];

   schar outBuffStr[def_lenBuff_idSearch + 1];
   ulong outPosUL = 0; /*byte to copy at*/

   uchar hashBl = !!(searchSTPtr->hashTblUL);

   ulong bytesUL = 0; /*number bytes in buffer*/
   slong entrySL = 0; /*for errors*/

   uchar errUC = 0;            /*holds errors*/

   ulong startUL = 0; /*start of read id*/
   ulong endUL = 0;   /*end of read id*/
   ulong_searchST idAryUL[17]; /*maximum libs allowed*/

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
         def_lenBuff_idSearch - 1,
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
               def_lenBuff_idSearch,
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
               def_lenBuff_idSearch,
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
|   - seqByIdHeaderStr:
|     o c-string with sam file header to print for seqById
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
   signed char *seqByIdHeaderStr,
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

   schar buffStr[def_lenBuff_idSearch + 1];

   schar outBuffStr[def_lenBuff_idSearch + 1];
   ulong outPosUL = 0; /*byte to copy at*/
   ulong tmpUL = 0;

   uchar hashBl = !!(searchSTPtr->hashTblUL);

   ulong bytesUL = 0; /*number bytes in buffer*/
   slong entrySL = 0; /*for errors*/

   uchar errUC = 0;            /*holds errors*/

   ulong startUL = 0; /*start of read id*/
   ulong endUL = 0;   /*end of read id*/
   ulong_searchST idAryUL[17]; /*maximum libs allowed*/

   slong indexSL = -1; /*array index of found read id*/

   schar pheadBl = 1;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun02 Sec02:
   ^   - read in first part of file & set print parameter
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   revSearchBl = !! revSearchBl; /*convert to 1 or 0*/

   bytesUL =
      fread(
         (char *) buffStr,
         sizeof(schar),
         def_lenBuff_idSearch - 1,
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

      else if(pheadBl)
      { /*Else If: printing header*/
         tmpUL = 0;

         pId_getSeq(
            seqByIdHeaderStr,
            lenStr_ulCp(
               seqByIdHeaderStr,
               0,
               '\0'
            ),
            &tmpUL,
            outBuffStr,
            &outPosUL,
            def_lenBuff_idSearch,
            outFILE
         );

         pheadBl = 0;
      } /*Else If: printing header*/
         

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
               def_lenBuff_idSearch,
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
               def_lenBuff_idSearch,
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

/*=======================================================\
: License:
: 
: This code is under the unlicense (public domain).
:   However, for cases were the public domain is not
:   suitable, such as countries that do not respect the
:   public domain or were working with the public domain
:   is inconvient / not possible, this code is under the
:   MIT license.
: 
: Public domain:
: 
: This is free and unencumbered software released into the
:   public domain.
: 
: Anyone is free to copy, modify, publish, use, compile,
:   sell, or distribute this software, either in source
:   code form or as a compiled binary, for any purpose,
:   commercial or non-commercial, and by any means.
: 
: In jurisdictions that recognize copyright laws, the
:   author or authors of this software dedicate any and
:   all copyright interest in the software to the public
:   domain. We make this dedication for the benefit of the
:   public at large and to the detriment of our heirs and
:   successors. We intend this dedication to be an overt
:   act of relinquishment in perpetuity of all present and
:   future rights to this software under copyright law.
: 
: THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF
:   ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT
:   LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
:   FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO
:   EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM,
:   DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
:   CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR
:   IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
:   DEALINGS IN THE SOFTWARE.
: 
: For more information, please refer to
:   <https://unlicense.org>
: 
: MIT License:
: 
: Copyright (c) 2024 jeremyButtler
: 
: Permission is hereby granted, free of charge, to any
:   person obtaining a copy of this software and
:   associated documentation files (the "Software"), to
:   deal in the Software without restriction, including
:   without limitation the rights to use, copy, modify,
:   merge, publish, distribute, sublicense, and/or sell
:   copies of the Software, and to permit persons to whom
:   the Software is furnished to do so, subject to the
:   following conditions:
: 
: The above copyright notice and this permission notice
:   shall be included in all copies or substantial
:   portions of the Software.
: 
: THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF
:   ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT
:   LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
:   FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO
:   EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
:   FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN
:   AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
:   FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE
:   USE OR OTHER DEALINGS IN THE SOFTWARE.
\=======================================================*/
