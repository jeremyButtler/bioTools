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
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*-------------------------------------------------------\
| Header:
|   - guards and forward declarations
\-------------------------------------------------------*/

#ifndef SEQ_BY_ID_FILE_SEARCH_LIST_H
#define SEQ_BY_ID_FILE_SEARCH_LIST_H

struct searchST;
struct str_ptrAry;

#define def_memErr_searchList 1

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

#endif
