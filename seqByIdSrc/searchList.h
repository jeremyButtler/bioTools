/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' searchList SOF: Start Of File
'   - holds struct and functions for read id multi-file
'     handling
'   o header:
'     - guards and forward declarations
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
|   - guards and forward declarations
\-------------------------------------------------------*/

#ifndef SEQ_BY_ID_FILE_SEARCH_LIST_H
#define SEQ_BY_ID_FILE_SEARCH_LIST_H

struct searchST;
struct str_ptrAry;

#define def_memErr_searchList -1
#define def_missing_searchList -2
#define def_fileErr_searchList -4

/*-------------------------------------------------------\
| ST01: file_searchList
|   - holds file name to save to and ids to search for
\-------------------------------------------------------*/
typdef struct file_searchList
{
   struct str_ptrAry *fileAryStr; /*output file array*/
   struct searchST *idAryST;      /*array 

   signed int sizeArySI;          /*array sizes*/
   signed int lenArySI;           /*items in arrays*/
}file_searchList;

/*-------------------------------------------------------\
| Fun01: blank_file_searchList
|   - blanks a file_searchList structure (and all childs)
| Input:
|   - fileSTPtr:
|     o pointer to file_searchList struct to blank
| Output:
|   - Modifies:
|     o fileAryStr to be blanked ('\0') (if has values)
|     o idAryST (not blanked yet [function does nothing])
|     o lenArySI to be 0
\-------------------------------------------------------*/
void
blank_file_searchList(
   struct file_searchList *fileSTPtr
);

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
);

/*-------------------------------------------------------\
| Fun03: freeStack_file_searchList
|   - frees variables in a a file_searchList structure
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
);

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
);

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
|     o fileAryStr in file_searchList to have sizeSI
|       items
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
);

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
);

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
);

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
);

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
);

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
);

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
);

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
);

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
);

#endif
