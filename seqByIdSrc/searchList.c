/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' searchList SOF: Start Of File
'   - holds struct and functions for read id multi-file
'     handling
'   o header:
'     - included libraries
'   o .h st01: file_searchList
'     - holds file name to save to and ids to search for
'   o fun01: blank_file_searchList
'     - blanks a file_searchList structure and all childs
'   o fun02: init_file_searchList
'     - initializes a file_searchList structure
'   o fun03: freeStack_file_searchList
'     - frees variables in a a file_searchList structure
'   o fun04: freeHeap_file_searchList
'     - frees a file_searchList structure
'   o fun05: setup_file_searchList
'     - sets up variables in a file_searchList structure
'   o fun06: realloc_file_searchList
'     - allocates more memory for a file_searchList struct
'   o fun07: swap_file_searchList
'     - swaps two items in a file_searchList arrays
'   o fun08: sort_file_searchList
'     - sort arrays in file_searchList struct by file name
'   o fun09: search_file_searchList
'     - searchs arrays in a file_searchList for file name
'   o fun10: addFile_file_searchList
'     - add file name to a file_searchList struct
'   o fun11: addId_file_searchList
'     - add read id to a file_searchList struct
'   o fun12: hashSetup_file_searchLIst
'     - setsups all hash tables (after added all ids) in a
'       file_searchST struct
'   o fun13: findId_file_searchList
'     - find all files that are assigned the input id in
'       a file_searchList struct
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

/*buffer size for scanning fastq file*/
#define def_lenBuff_searchList (1 << 17) /*~ 65536*/

/*-------------------------------------------------------\
| Fun01: blank_file_searchList
|   - blanks a file_searchList structure (and all childs)
| Input:
|   - fileSTPtr:
|     o pointer to file_searchList struct to blank
| Output:
|   - Modifies:
|     o fileAryStr in fileSTPtr to be blanked ('\0')
|     o idAryST in fileSTPtr; called function does nothing
|     o lenArySI in fileSTPtr to be 0
\-------------------------------------------------------*/
void
blank_file_searchList(
   struct file_searchList *fileSTPtr
){
   signed int siCnt = 0;

   if(fileSTPtr->fileAryStr)
      blank_str_ptrAry(fileSTPtr->fileAryStr);

   if(fileSTPtr->idAryST)
   { /*If: need to blank the id array*/
      for(
         siCnt = 0;
         siCnt < fileSTPtr->lenArySI;
         ++siCnt
      ) blank_searchST(fileSTPtr->idAryST[siCnt]);
   } /*If: need to blank the id array*/

   fileSTPtr->lenArySI = 0;
} /*blank_file_searchList*/

/*-------------------------------------------------------\
| Fun02: init_file_searchList
|   - initializes a file_searchList structure
| Input:
|   - fileSTPtr:
|     o pointer to file_searchList struct to initialize
| Output:
|   - Modifies:
|     o fileAryStr in fileSTPtr to be 0
|     o idAryST in fileSTPtr to be 0
|     o lenArySI in fileSTPtr to be 0
|     o sizeArySI in fileSTPTr to be 0
\-------------------------------------------------------*/
void
init_file_searchList(
   struct file_searchList *fileSTPtr
){
   fileSTPtr->fileAryStr = 0;
   fileSTPtr->idAryST = 0;
   fileSTPtr->sizeArySI = 0;
   fileSTPtr->lenArySI = 0;

   blank_file_searchList(fileSTPtr);
} /*blank_file_searchList*/

/*-------------------------------------------------------\
| Fun03: freeStack_file_searchList
|   - frees variables in a file_searchList structure
| Input:
|   - fileSTPtr:
|     o pointer to file_searchList with vars to free
| Output:
|   - Modifies:
|     o frees fileAryStr and initializes
|     o frees idAryST and initializes
|     o sets lenArySI to 0
|     o sets sizeArySI to 0
\-------------------------------------------------------*/
void
freeStack_file_searchList(
   struct file_searchList *fileSTPtr
){
   signed int siCnt = 0;

   if(! fileSTPtr)
      return; /*nothing to free*/

   if(fileSTPtr->fileAryStr)
      freeHeap_str_ptrAry(fileSTPtr->fileAryStr);

   if(fileSTPtr->idAryST)
   { /*If: need to free id structs*/
      for(
         siCnt = 0;
         siCnt < fileSTPtr->lenArySI;
         ++siCnt
      ) freeHeap_searchST(&fileSTPtr->idAryST[siCnt]);
   } /*If: need to free id structs*/

   init_file_searchList(fileSTPtr);
} /*freeStack_file_searchList*/

/*-------------------------------------------------------\
| Fun04: freeHeap_file_searchList
|   - frees a file_searchList structure
| Input:
|   - fileSTPtr:
|     o pointer to file_searchList with vars to free
| Output:
|   - Modifies:
|     o frees fileSTPtr (you must set to 0/null)
\-------------------------------------------------------*/
void
freeHeap_file_searchList(
   struct file_searchList *fileSTPtr
){
   if(! fileSTPtr)
      return;

   freeStack_file_searchList(fileSTPtr);
   free(fileSTPtr);
} /*freeHeap_file_searchList*/

/*-------------------------------------------------------\
| Fun05: setup_file_searchList
|   - sets up variables in a file_searchList structure
| Input:
|   - fileSTPtr:
|     o pointer to file_searchList to set up
|   - sizeSI:
|     o starting array length
|     o changed to 8 if sizeSI is < 1
| Output:
|   - Modifies:
|     o fileAryStr in file_searchList to have sizeSI items
|       - is freeded if already exists (not 0/null)
|     o idAryST in file_searchList to have sizeSI items
|       - is freeded if already exists (not 0/null)
|     o sizeArySI in file_searchList to be sizeSI
|     o lenArySI in file_searchList to be 0
|   - Returns:
|     o 0 for no errors
|     o def_memErr_searchList for memory errors
\-------------------------------------------------------*/
signed char
setup_file_searchList(
   struct file_searchList *fileSTPtr,
   signed int sizeSI
){
   signed int siCnt = 0;

   if(sizeSI <= 0)
      sizeSI = 8; /*make sure have some memory*/


   freeStack_file_searchList(fileSTPtr);
      /*free any allocated memory*/

   fileSTPtr->fileAryStr = mk_str_ptrAry(sizeSI);

   if(! fileSTPtr->fileAryStr)
      goto memErr_fun05;


   fileSTPtr->idAryST =
      malloc(sizeSI * sizeof(struct searchST));

   if(! fileSTPtr->idAryST)
      goto memErr_fun05;

   for(
      siCnt = 0;
      siCnt < fileSTPtr->sizeArySI;
      ++siCnt
   ) init_searchST(fileSTPtr->idAryST[siCnt]);
     /*initializing read id structures*/


   fileSTPtr->sizeArySI = 8;
   return 0;

   memErr_fun05:;
      return def_memErr_searchList;
} /*setup_file_searchList*/

/*-------------------------------------------------------\
| Fun06: realloc_file_searchList
|   - allocates more memory for a file_searchList struct
| Input:
|   - fileSTPtr:
|     o pointer to file_searchList to increase array size
|   - sizeSI:
|     o new array length
| Output:
|   - Modifies:
|     o fileAryStr in file_searchList to have sizeSI items
|     o idAryST in file_searchList to have sizeSI items
|     o sizeArySI in file_searchList to be sizeSI
|   - Returns:
|     o 0 for no errors
|     o def_memErr_searchList for memory errors
\-------------------------------------------------------*/
signed char
realloc_file_searchList(
   struct file_searchList *fileSTPtr,
   signed int sizeSI
){
   signed int siCnt = 0;
   struct searchST *ptrST = 0;


   siCnt =
      resize_str_ptrAry(
         fileSTPtr->fileAryStr,
         sizeSI
      ); /*get more memory for string array*/

   if(siCnt == def_memErr_ptrAry)
      goto memErr_fun06;


   ptrST =
      realloc(
         fileSTPtr->idAryST,
         sizeSI
      );

   if(! ptrST)
       goto memErr_fun06;

   fileSTPtr->idAryST = ptrST;


   for(
      siCnt = fileSTPtr->sizeArySI;
      siCnt < sizeSI;
      ++siCnt
   ) init_stackST(&fileSTPtr->idAryST[siCnt]);
     /*initialize stackST structs in array*/
   

   fileSTPtr->sizeArySI = sizeSI;
   return 0;

   memErr_fun06:;
      return def_memErr_searchList;
} /*realloc_file_searchList*/

/*-------------------------------------------------------\
| Fun07: swap_file_searchList
|   - swaps two items in a file_searchList arrays
| Input:
|   - fileSTPtr:
|     o pointer to file_searchList with items to swap
|   - firstIndexSI:
|     o index of frist item to swap
|   - secIndexSI:
|     o index of second item to swap
| Output:
|   - Modifies:
|     o swaps fileAryStr and idAryST values at
|       firstIndexSI and secIndexSI
\-------------------------------------------------------*/
void
swap_file_searchList(
   struct file_searchList *fileSTPtr,
   signed int firstIndexSI,
   signed int secIndexSI
){
   swap_str_ptrAry(
      fileSTPtr->fileAryStr,
      firstIdnexSI,
      secIndexSI
   ); /*swap file names*/

   swap_searchST(
      &fileSTPtr->idAryST[firstIndexSI],
      &fileSTPtr->idAryST[secIndexSI]
   );
} /*swap_file_searchList*/

/*-------------------------------------------------------\
| Fun08: sort_file_searchList
|   - sorts arrays in file_searchList struct by file name
| Input:
|   - fileSTPtr:
|     o pointer to file_searchList to sort
| Output:
|   - Modifies:
|     o fileAryStr in fileSTPtr to be sorted by file name
|     o idAryST in fileSTPtr to be sorted by file name
\-------------------------------------------------------*/
void
sort_file_searchList(
   struct file_searchList *fileSTPtr
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun08 TOC:
   '   - sorts signed int array from least to greatest
   '   o fun08 sec01:
   '     - variable declerations
   '   o fun08 sec02:
   '     - find the number of rounds to sort for
   '   o fun08 sec03:
   '     - sort the arrays
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun08 Sec01:
   ^   - variable declerations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*Number of elements to sort*/
   signed long cmpSL = 0;
   unsigned long numElmUL = fileSTPtr->lenArySI;

   /*Number of sorting rounds*/
   unsigned long subUL = 0;
   unsigned long nextUL = 0;
   unsigned long lastUL = 0;
   unsigned long onUL = 0;

   /*Variables to incurment loops*/
   unsigned long ulIndex = 0;
   unsigned long ulElm = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun08 Sec02:
   ^   - find the max search value (number rounds to sort)
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(! numElmUL)
      return;

   /*Recursion formula: h[0] = 1, h[n] = 3 * h[n - 1] +1*/
   subUL = 1; /*Initialzie first array*/

   while(subUL < numElmUL - 1)
      subUL = (3 * subUL) + 1;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun08 Sec03:
   ^   - sort arrays
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   while(subUL > 0)
   { /*Loop: all rounds*/
      for(
         ulIndex = 0;
         ulIndex <= subUL;
         ++ulIndex
      ){ /*Loop: though sub array*/
         ulElm = ulIndex;

         for(
            ulElm = ulIndex;
            ulElm + subUL <= endUL;
            ulElm += subUL
         ){ /*Loop: swap elements in subarray*/
            nextUL = ulElm + subUL;

            cmpSL =
               cmp_str_ptrAry(
                  (signed long) ulElm,
                  (signed long) nextUL
               ); /*see if file names are equal*/

            if(cmpSL > 0)
            { /*If I need to swap an element*/
               swap_file_searchList(
                  fileSTPtr,
                  (signed int) ulElm,
                  (signed int) nextUL
               ); /*swap file entries*/

               lastUL = ulElm;
               onUL = ulElm;

               while(lastUL >= subUL)
               { /*loop: move swapped element back*/
                  lastUL -= subUL;

                  cmpSL =
                     cmp_str_ptrAry(
                        (signed long) onUL,
                        (signed long) lastUL
                     );

                  if(ulAry[onUL] > ulAry[lastUL])
                     break; /*Positioned the element*/
   
                  swap_file_searchList(
                     fileSTPtr,
                     (signed int) onUL,
                     (signed int) lastUL
                  ); /*swap file entries*/

                  onUL = lastUL;
               } /*Loop: move swapped element back*/
            } /*If I need to swap elements*/
         } /*Loop: swap elements in subarray*/
      } /*Loop: though sub array*/

      subUL = (subUL - 1) / 3; /*Move to next round*/
   } /*Loop: all rounds*/
} /*sort_file_searchList*/

/*-------------------------------------------------------\
| Fun09: search_file_searchList
|   - searchs arrays in a file_searchList for file name
| Input:
|   - fileStr:
|     o file to find
|   - fileSTPtr:
|     o pointer to file_searchList to search
| Output:
|   - Returns:
|     o index of target file name
| Note:
|   - this assumes things are sorted, which add file
|     always does
\-------------------------------------------------------*/
signed long
search_file_searchList(
   signed char *fileStr,
   struct file_searchList *fileSTPtr
){
   return
      find_strPtrAry(
         fileSTPtr->fileAryStr,
         fileStr
      );
} /*search_file_searchList*/

/*-------------------------------------------------------\
| Fun10: addFile_file_searchList
|   - add file name to a file_searchList struct
| Input:
|   - fileStr:
|     o c-string with file to append to list
|   - fileSTPtr:
|     o pointer to file_searchList struct to append file
| Output:
|   - Modifies:
|     o fileAryStr to have file appened; resized if needed
|     o idAryST to be resized if fileAryStr is resized
|     o sizeArySI to be resized value if needed to resize
|     o lenArySI to be incurmented
|   - Returns:
|     o 0 for no errors
|     o def_memErr_searchList for memory errors
\-------------------------------------------------------*/
signed char
addFile_file_searchList(
   signed char *fileStr,
   struct file_searchList *fileSTPtr
){
   signed long indexSL = 0;
   signed long oldUL = 0;

   if(lenArySI >= sizeArySI)
   { /*If: need to resize arrays*/
      if(
         realloc_file_searchList(
            fileSTPtr,
            (fileSTPtr->sizeArySI << 1)
      ) goto memErr_fun10;
   } /*If: need to resize arrays*/

   indexSL =
      add_str_ptrAry(
         fileStr,
         fileSTPtr->fileAryStr,
         fileSTPtr->lenArySI
      ); /*already resized, so am safe*/


   if(fileSTPtr->lenArySI > 0)
   { /*If: need to get max limb size*/
      if(! indexSL)
         oldUL = 1; /*file was put at start of list*/
      else
         oldUL = 0;

      fileSTPtr->idAryST[indexSL].maxLimbsUC =
         fileSTPtr->idAryST[oldUL].maxLimbsUC;
   } /*If: need to get max limb size*/


   ++fileSTPtr->lenArySI;
   return 0;

   memErr_fun10:;
      return def_memErr_searchList;
} /*addFile_file_searchList*/

/*-------------------------------------------------------\
| Fun11: addId_file_searchList
|   - add read id to a file_searchList struct
| Input:
|   - idStr:
|     o c-string with read id to add
|   - fileStr:
|     o c-string with file to add read id to
|   - fileSTPtr:
|     o pointer to file_searchList struct to add id to
| Output:
|   - Modifies:
|     o idAryST in fileSTPtr to have read id
|       * idAryUL in idAryST[index] gets new read
|       * lenIdAryUL in idAryST[index] if need more memory
|       * numIdsUL in idAryST[index] is incurmented
|       * numLimbsUL in idAryST[index] to have new end
|       * maxLimbsUC in idAryST[index] if id has more
|         limbs than previous ids
|   - Returns:
|     o 0 for no errors
|     o def_memErr_searchList for memory errors
|     o def_missing_searchList; fileStr not in fileAryStr
| Note:
|   - assumes the file is in fileSTPtr and the file names
|     are already sorted
\-------------------------------------------------------*/
signed char
addId_file_searchList(
   signed char *idStr,               /*read id to add*/
   signed char *fileStr,             /*file with read id*/
   struct file_searchList *fileSTPtr /*add read id to*/
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun11 TOC:
   '   - add read id to a file_searchList struct
   '   o fun11 sec01:
   '     - variable declarations
   '   o fun11 sec02:
   '     - find file to add read id to
   '   o fun11 sec03:
   '     - add read id to file entry
   '   o fun11 sec04:
   '     - return result
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun11 Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   unsigned long posUL = 0;
   unsigned long oldPosUL = 0;
   unsigned long numLimbUL = 0; /*for finding limb size*/
   unsigned long oldLimbUL = 0; /*old id's limb size*/

   signed long indexSL = 0;
   signed long *swapULPtr = 0; /*reallocs*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun11 Sec02:
   ^   - find file to add read id to
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   indexSL =
      search_file_searchList(
         fileStr,
         fileSTPtr
      ); /*find file to assign read id to*/

   if(indexSL < 0)
      goto noFile_fun11_sec04;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun11 Sec03:
   ^   - add read id to file entry
   ^   o fun11 sec03 sub01:
   ^     - check if need more memory
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun11 Sec03 Sub01:
   *   - check if need more memory
   \*****************************************************/

   /*get number of remaing limbs*/
   posUL = fileSTPtr->idAryST[indexSL]->lenIdAryUL;
   posUL -= fileSTPtr->idAryST[indexSL]->numLimbsUL;

   if(posUL < def_maxLimbs_idLkTbl)
   { /*If: need more memory*/
      fileSTPtr->idAryST[indexSL].lenIdAryUL =
         fileSTPtr->idAryST[indexSL].numLimbsUL;

      posUL = fileSTPtr->idAryST[indexSL].lenIdAryUL;

      posUL *=
         (fileSTPtr->idAryST[indexSL].lenIdAryUL + 2);
         /*+1 long for sum long, +1 long for index limb*/

      posUL +=
         (fileSTPtr->idAryST[indexSL].lenIdAryUL >> 1);
         /*increase size by 50%*/

      swapULPtr =
         realloc(
            fileSTPtr->idAryST[indexSL].idAryUL,
            posUL * sizeof(ulong_searchST)
         );

      if(! swapULPtr)
         goto memErr_fun11_sec04;

      fileSTPtr->idAryST[indexSL].idAryUL = swapULPtr;
      fileSTPtr->idAryST[indexSL].lenIdAryUL = posUL;
   } /*If: need more memory*/

   /*****************************************************\
   * Fun11 Sec03 Sub02:
   *   - add read id
   \*****************************************************/

   oldPosUL = posUL; /*so can find limb size*/


   /*I am putting last id's length at start (end of last
   *  id), so it gets wiped every round.
   */
   oldLimbUL =
      fileSTPtr->idAryST[indexSL].idAryUL[oldPosUL];


   cnvtIdToHexAry_searchST(
      idStr,
      &fileSTPtr->idAryST[indexSL]
      &posUL
   ); /*add read id to file [also converts to hex]*/

   /*add in number of limbs to end*/
   numLimbUL =
      (unsigned char)
      fileSTPtr->idAryST[indexSL].idAryUL[oldPosUL];

   fileSTPtr->idAryST[indexSL].idAryUL[posUL] = numLimbUL;

   
   /*adding in last id length (no longer wiped)*/
   fileSTPtr->idAryST[indexSL].idAryUL[oldPosUL] =
      oldLimbUL;


   if(
        fileSTPtr->idAryST[indexSL].maxLimbsUC
      < (unsigned char) numLimbsUL
   ){ /*If: have larger limbs*/
      for(
         oldPosSL = 0;
         oldPosSL < fileSTPtr->lenArySI;
         ++oldPosSL
      ){ /*Loop: increase all file limb sizes*/
         fileSTPtr->idAryST[oldPosSL].maxLimbsUC =
            (unsigned char) numLimbsUL;
      }  /*Loop: increase all file limb sizes*/
   } /*If: have larger limbs*/

   fileSTPtr->idAryST[indexSL].maxLimbsUC =
      max_genMath(
         fileSTPtr->idAryST[indexSL].maxLimbsUC,
         (unsigned char) numLimbsUL
      ); /*make sure I have the maximum found limb size*/

   /*update position of last id limb in array*/
   fileSTPtr->idAryST[indexSL].numLimbsUL = posUL;

   /*update number of ids in array*/
   ++fileSTPtr->idAryST[indexSL].numIdsUL;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun11 Sec04:
   ^   - return result
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   return 0;

   memErr_fun11_sec04:;
      return def_memErr_searchList;

   noFile_fun11_sec04:;
      return def_missing_searchList;
} /*addId_file_searchList*/

/*-------------------------------------------------------\
| Fun12: hashSetup_file_searchLIst
|   - setsups all hash tables (after added all ids) in a
|     file_searchST struct
| Input:
|   - fileSTPtr:
|     o pointer to file_searchList struct to setup hash
| Output:
|   - Modifies:
|     o all elements of idAryST in fileSTPtr to have their
|       hash table setup
|       * all ids in idAryUL to have the same number of
|         limbs (maxLimbsUC)
|       * hashTblUL to have index's of idAryUL (hash tbl)
|       * hashSizeUL to have size of hashTblUL
|       * hashPowUC to have power (2^x) of hash table size
|       * majicUL to have majic number for kunths hash
|   - Returns:
|     o 0 for no errors
|     o def_memErr_searhLIst for memory errors
\-------------------------------------------------------*/
signed char
hashSetup_file_searchList(
   struct file_searchList *fileSTPtr
){
   signed long indexSL = 0;

   for(
      indexSL = 0;
      indexSL < lenArySI;
      ++indexSL
   ){ /*Loop: setup hash tables*/
      evenLimbs_searchST(&fileSTPtr[indexSL]);

      if(mkhash_searchST(&fileSTPtr[indexSL]))
         goto memErr_fun12;
   } /*Loop: setup hash tables*/

   return 0;

   memErr_fun12:;
      return def_memErr_searchList;
} /*hashSetup_file_searchList*/

/*-------------------------------------------------------\
| Fun13: findId_file_searchList
|   - find all files that are assigned the input id in
|     a file_searchList struct
| Input:
|   - idAryUL:
|     o pointer to unsigned long with read id
|       - use idToHexAry_maxLimb_searchST to convert
|         it. For limb size use first item in fileSTPtr
|         (fileSTPtr->idAryST[0].maxLimbsUC). If used
|         functions to add files and ids, then all files
|         have same maxLimbsUC.
|   - fileSTPtr:
|     o pointer to file_searchList struct with files to
|       search for id in (as hash table)
|   - errSCPtr:
|     o pointer to signed char to hold error value
| Output:
|   - Modifies:
|     o errSCPtr:
|       * 0 for no errors (no ids or found ids)
|       * def_memErr_searchList for memory errors
|   - Returns:
|     o signed int array with index of found files (you
|       must free) [end of file index's is -1)
|     o 0 if id not found or memory error error
\-------------------------------------------------------*/
signed int *
findId_file_searchList(
   struct searchST *idSearchSTPtr,    /*id to find*/
   struct file_searchList *fileSTPtr, /*has ids + files*/
   signed char *errSCPtr              /*assigned errors*/
){
   signed int *retHeapArySI = 0;
   signed long indexSL = 0;
   signed long foundSL = 0;


   retHeapArySI =
     malloc((fileSTPtr->lenArySI+1) * sizeof(signed int));

   if(! retHeapArySI)
      goto memErr_fun13;


   for(
      indexSL = 0;
      indexSL < fileSTPtr->lenArySI;
      ++indexSL
   ){ /*Loop: find files with matching ids*/
      foundSL =
         searchHash_searchST(
            &fileSTPtr->idAryST[indexSL],
            idSearchSTPtr->idAryUL
         ); /*See if read id was assigned to file*/

      if(foundSL)
         retHeapArySI[indexSL] = (signed int) indexSL;
   } /*Loop: find files with matching ids*/


   retHeapArySI[indexSL] = -1;


   *errSCPtr = 0;
   goto ret_fun13;

   noMatch_fun13:;
      *errSC = 0;             /*no matchs, not error*/
      goto errCleanUp_fun13;  /*need to clean up array*/

   memErr_fun13:;
      *errSC = def_memErr_searchList;
      goto errCleanUp_fun13;

   errCleanUp_fun13:;
      if(retHeapArySI)
         free(retHeapArySI);
      retHeapArySI = 0;
      goto ret_fun13;

   ret_fun13:;
      return retHeapArySI;
} /*findId_file_searchList*/

/*-------------------------------------------------------\
| Fun14: getFqSeq_searchList
|   - gets fastq sequences having read ids in a
|     file_searchList struct
| Input:
|   - fileSTPtr:
|     o pointer to file_searchList struct with ids to get
|   - revSearchBl:
|     o 1: keeping reads that are not in list
|     o 0: keeping reads in list
|   - fqFILE:
|     o file pointer to fastq file with reads to get
| Output:
|   - Prints:
|     o found sequences to their assigned output file(s)
|   - Returns
|     o 0 for no errors
|     o def_memErr_searchList for memory errors
|     o > 0 if line in fastq entry was invalid
|     o def_fileErr_searchList if had file error
\-------------------------------------------------------*/
signed long
getFqSeq_searchList(
   struct file_searchList *fileSTPtr,
   signed char revSearchBl,
   void *fqFILE
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun14 TOC:
   '   - gets and print fastq sequences in a
   '     file_searchList struct
   '   o fun14 sec01:
   '     - variable declarations
   '   o fun14 sec02:
   '     - set print parm + read first part of fastq file
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun14 Sec01:
   ^   - variable decartionst
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   signed char buffStr[def_lenBuff_searchList + 9];
   unsigned long bytesUL = 0; /*bytes in buffer*/
   signed long errSL = 0;   /*error reporting*/
   unsigned char errUC = 0; /*more error reporting*/

   unsigned long startUL = 0; /*start of read id*/
   unsigned long endUL = 0;   /*end of read id*/
   ulong_searchST idAryUL[17]; /*holds id*/

   signed int *fileHeapArySI = 0; /*files with read*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun14 Sec02:
   ^   - set print parm + read first part of fastq file
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   revSearchBl = !! revSearchBl; /*make sure 0 or 1*/

   bytesUL =
      fread(
         (char *) buffStr,
         sizeof(char),
         def_lenBuff_searchList - 1,
         (FILE *) fqFILE
      ); /*get first fastq line/read*/

   buffStr[bytesUL] = '\0';

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun14 Sec03:
   ^   - find reads matching read ids
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/
} /*getFqSeq_searchList*/

