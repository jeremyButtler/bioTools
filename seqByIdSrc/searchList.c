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

   blank_file_searchST(fileSTPtr);
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
| Fun08: search_file_searchList
|   - searchs arrays in a file_searchList for file name
| Input:
|   - fileSTPtr:
|     o pointer to file_searchList to search
| Output:
|   - Returns:
|     o index of target file name
| Note:
|   - this assumes things are sorted, which add file
|     always does
\-------------------------------------------------------*/
void
search_file_searchList(
   struct file_searchList *fileSTPtr
){
} /*search_file_searchList*/
