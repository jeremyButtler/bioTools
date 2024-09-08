/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' idSearch SOF: Start Of File
'   - holds functions for searching throug a file by id
'   o header:
'     - forward declerations and guards
'   o fun01: getFqSeq_idSearch
'     - extract fastq sequences from file by read ids
'   o fun02: getSamSeq_idSearch
'     - extract sam sequences from sam file by read ids
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*-------------------------------------------------------\
| Header:
|   - forward declerations and guards
\-------------------------------------------------------*/

#ifndef ID_SEARCH_H
#define ID_SEARCH_H

typedef struct searchST searchST;

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
);

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
);

#endif
