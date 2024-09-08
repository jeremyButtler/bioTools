/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' SOF: Start Of File
'   o header:
'     - defined variables and guards
'   o .h st01: searchST
'     - holds the hash table and read ids
'   o .c tbl01: hexTblUC_searchST
'     - conversion table of read id character to hex
'   o fun01: cnvtIdToHexAry_searchST
'     - converts an read id to a series of hex numbers
'   o .c fun02: swapIds_searchST
'     - swaps two ids in a searchST structure
'   o .h fun03: getHash_searchST
'     - get the hash for a read id
'   o fun04: cmpIds_searchST
'     - compares two read ids
'   o fun05: sortIds_searchST
'     - sorts ids in a searchST numerically
'   o fun06: hashSortIds_searchST
'     - sorts ids in a searchST by hash value
'   o fun07: evenLimbs_seachST
'     - takes a read id array with unequal number of limbs
'       (longs) per limb and makes all ids hae the same
'       number of limbs
'   o .c fun08: mkSkip_searchST
'     - converts a sorted list of read ids to a skip list
'   o .c fun09: majicNum_searchST
'     - finds the majic number for an hash table
'   o fun10: mkhash_searchST
'     - takes in a searchST structure with ids and makes a
'       hash table for it
'   o fun10: getReadIds_searchST
'     - get read ids from a file and make a hash table
'   o fun12: searchId_searchST
'     - search for a read id in a searchST structure not
'       set up as a hash table
'   o fun13: searchHash_searchST
'     - search for a read id in a searchST hash table
'   o fun13: blank_searchST
'     - here in case need in future. Does nothing
'   o fun13: init_searchST
'     - initializes a searchST structure
'   o fun16: freeStack_searchST
'     - frees all variables in a searchST structure
'   o fun17: freeHeap_searchST
'     - frees a searchST structure
'   o fun18: idToHexAry_maxLimb_searchST
'     - converts an read id to a series of hex numbers and
'       does not go past a max limb count
'     - this is here for idSearch
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*-------------------------------------------------------\
| Header:
|   - defined variables and guards
\-------------------------------------------------------*/

#ifndef SEARCH_STRUCTURE_H
#define SEARCH_STRUCTURE_H

#define def_ulBits_searchST (sizeof(unsigned long) << 3)

#define def_negULBit_searchST ( (signed long) ((unsigned long) 1 << (def_ulBits_searchST - 1)) )

#define def_fileErr_searchST 2
#define def_memErr_searchST 4

/*The frist limb in the unsigned long hex id array holds
'   the sum, number of characters in the id, and the
`   number of limbs used.
` These varaibles help add, clear, or get the number of
`   characters and limbs from the first limb
*/

#define def_addLenId_searchST (def_ulBits_searchST - 16)

#define def_addLenLimb_searchST (def_ulBits_searchST - 8)

#define def_maxIdLenBits_searchST 8
   /*maximum of 8 bits for id length*/

#define def_getIdLen_searchST 0xff
   /*being in first 8 bits alllows a better hash*/

#define def_getNumLimb_searchST (((unsigned long) 0xff) << def_adjLimbCnt_searchST)

#define def_adjIdLen_searchST 0xff /*nothing to do*/
#define def_adjLimbCnt_searchST def_ulBits_searchST - 8

#define def_clearNumLimb_searchST (((unsigned long) -1) >> 8)

#define def_clearLenId_searchST 0xff

/*-------------------------------------------------------\
| ST01: searchST
|   - holds the hash table and read ids
\-------------------------------------------------------*/
typedef struct searchST
{
   unsigned long *idAryUL;   /*array of read ids*/
   unsigned long numIdsUL;   /*number read ids in hash*/
   unsigned long numLimbsUL; /*number limbs in id array*/
   unsigned long lenIdAryUL; /*length of id array*/
   unsigned char maxLimbsUC; /*max limbs in an id*/

   unsigned long *hashTblUL; /*hash table to search*/
   unsigned long hashSizeUL; /*size of hash table*/
   unsigned char hashPowUC;  /*2^hashPowUC = hashSizeUL*/
   unsigned long majicUL;    /*majic for kunths hash*/
}searchST;

/*-------------------------------------------------------\
| Fun01: cnvtIdToHexAry_searchST
|   - converts an read id to a series of hex numbers
| Input:
|   - idStr:
|     o c-string with the id to convert
|   - idAryUL:
|     o array of unsigned longs to hold the id
|     o make sure there are at least seventeen empty longs
|       in the array. Allows for ids of length
|       def_maxCharInId_idLkTbl + a extra limb for
|       storing the sum
|   - posInUL:
|     o pointer to a unsigned long with the current
|       position in the array
| Output:
|   - Modfies:
|     o idAryUL to hold the converted id
|       - first element & def_getNumLimb_searchST will
|         give the number of limbs used
|         o clear with def_clearNumLimb_searchST
|     o posInUL to hold the next blank array position
|   - Returns
|     o the number of characters converted in the id
\-------------------------------------------------------*/
unsigned char
cnvtIdToHexAry_searchST(
   signed char *idStr,
   unsigned long *idAryUL,
   unsigned long *posInUL
);

/*-------------------------------------------------------\
| Fun03: getHash_searchST
|   - get the hash for a read id
| Input:
|   - hashSTPtr:
|     o pointer to a searchST structure with id
|       to get hash from
|   - idUL:
|     o long to get id for (should be index + 1)
| Output:
|   - Returns:
|     o 0 if ids are equal
|     o >0 if first id is greater than second
|     o <0 if first id is lesser than second
\-------------------------------------------------------*/
#define getHash_searchST(hashSTPtr, idUL) ( ((idUL)[1] * (hashSTPtr)->majicUL) >> (def_ulBits_searchST - (hashSTPtr)->hashPowUC) )


/*-------------------------------------------------------\
| Fun04: cmpIds_searchST
|   - compares two read ids
| Input:
|   - hashSTPtr:
|     o pointer to a searchST structure with ids
|       to compare
|   - refIdAryUL:
|     o unsigned long pointer to the first id to compare
|   - qryIdAryUL:
|     o unsigned long pointer to the second id to compare
| Output:
|   - Returns:
|     o 0 if ids are equal
|     o >0 if first id is greater than second
|     o <0 if first id is lesser than second
\-------------------------------------------------------*/
signed long
cmpIds_searchST(
   struct searchST *hashSTPtr,
   unsigned long *refIdAryUL,
   unsigned long *qryIdAryUL
);

/*-------------------------------------------------------\
| Fun05: sortIds_searchST
|   - sorts ids in a searchST numerically
| Input:
|   - hashSTPtr:
|     o pointer to a searchST structure with read
|       ids to sort
| Output:
|   - Modifies:
|     o idAryUL in hashSTPtr to be sorted by read id
\-------------------------------------------------------*/
void
sortIds_searchST(
   struct searchST *hashSTPtr
);

/*-------------------------------------------------------\
| Fun06: hashSortIds_searchST
|   - sorts ids in a searchST by hash value
| Input:
|   - hashSTPtr:
|     o pointer to a searchST structure with read
|       ids to sort
| Output:
|   - Modifies:
|     o idAryUL in hashSTPtr to be sorted by read id hash
\-------------------------------------------------------*/
void
hashSortIds_searchST(
   struct searchST *hashSTPtr
);

/*-------------------------------------------------------\
| Fun07: evenLimbs_seachST
|   - takes a read id array with unequal number of limbs
|     (longs) per limb and makes all ids hae the same
|     number of limbs
| Input:
|   - searchSTPtr:
|     o pointer to a searchST structure with unven ids and
|       evens out the ids
|   - hashBl:
|     o 1: doing hashing
|     o 0: not doing hashing
| Output:
|   - Modifies:
|     o idAryUL in searchST structure to have equal limb
|       lengths
|     o maxLimbsUC to be +1 for not hashing or +2 for
|       hashing
| Note:
|   - always sort ids after this function. The first few
|     ids are always added to the end
|   - this assumes you have plenty of memory in the array
\-------------------------------------------------------*/
void
evenLimbs_searchST(
   struct searchST *searchSTPtr
);

/*-------------------------------------------------------\
| Fun10: mkhash_searchST
|   - takes in a searchST structure with ids and makes a
|     hash table for it
| Input:
|   - searchSTPtr:
|     o pionter to an searchST structure to get the hash
|       for
| Output:
|   - Modifies:
|     o idAryUL to be sorted by hash value and to be
|       resized to fit the hash table (if needed)
|     o lenIdAryUL to reflect new idAryUL size
|       (if idArrUL is resized)
|     o hashTblUL to point to extra array at end of
|       idAryUL (this is hashing array)
|     o hashSizeUL to have number of longs in the hash
|       table
|     o hashPowUC to have the power of two hashSizeUL is
|     o majicUL to have the majic number of hashing
|   - Returns:
|     o 0 for no errors
|     o 1 for memory error
\-------------------------------------------------------*/
signed char
mkhash_searchST(
   struct searchST *searchSTPtr
);

/*-------------------------------------------------------\
| Fun10: getReadIds_searchST
|   - get read ids from a file and make a hash table
| Input:
|   - idFILE:
|     o FILE pionter with read ids to extract
|   - hashBl:
|     o 1: make a hash table
|     o 0: do not make a hash table (just sort ids)
|   - errSC:
|     o signed char with error tpype
| Output:
|   - Modifies:
|     o errSC to hold the error message
|   - Returns:
|     o pointer to a searchST structure for no errors
|     o 0 for errors
\-------------------------------------------------------*/
searchST *
getReadIds_searchST(
   void *idFILE,
   signed char hashBl,
   signed char *errSC
);

/*-------------------------------------------------------\
| Fun12: searchId_searchST
|   - search for a read id in a searchST structure not
|     set up as a hash table
| Input:
|   - searchSTPtr:
|     o pointer to searchSTPtr structure to search for
|       read id in
|   - qryIdAryUL:
|     o unsigned long pointer to read id to search for
| Ouput:
|   - Returns:
|     o index of read id if found    
|     o def_negULBit_searchST if id not found
|       this has only the negative bit set
\-------------------------------------------------------*/
signed long
searchId_searchST(
   struct searchST *searchSTPtr,
   unsigned long *qryIdAryUL
);

/*-------------------------------------------------------\
| Fun13: searchHash_searchST
|   - search for a read id in a searchST hash table
| Input:
|   - searchSTPtr:
|     o pointer to searchSTPtr structure to search for
|       read id in
|   - qryIdAryUL:
|     o unsigned long pointer to read id to search for
| Ouput:
|   - Returns:
|     o index of read id if found    
|     o def_negULBit_searchST if id not found
|       this has only the negative bit set
\-------------------------------------------------------*/
signed long
searchHash_searchST(
   struct searchST *searchSTPtr,
   unsigned long *qryIdAryUL
);

/*-------------------------------------------------------\
| Fun13: blank_searchST
|   - here in case need in future. Currently does nothing
| Input:
|   - searchSTPtr:
|     o pointer to a searchST structure to blank
| Output:
|   - Modifies:
|     o currently nothing
\-------------------------------------------------------*/
void
blank_searchST(
   struct searchST *searchSTPtr
);

/*-------------------------------------------------------\
| Fun13: init_searchST
|   - initializes a searchST structure
| Input:
|   - searchSTPtr:
|     o pointer to a searchST structure to initialize
| Output:
|   - Modifies:
|     o sets pointers and all values in searchST to zero
\-------------------------------------------------------*/
void
init_searchST(
   struct searchST *searchSTPtr
);
   
/*-------------------------------------------------------\
| Fun16: freeStack_searchST
|   - frees all variables in a searchST structure
| Input:
|   - searchSTPtr:
|     o pointer to a searchST structure to free variables
| Output:
|   - Frees:
|     o all variables & calls init_searchST (fun13)
\-------------------------------------------------------*/
void
freeStack_searchST(
   struct searchST *searchSTPtr
);

/*-------------------------------------------------------\
| Fun17: freeHeap_searchST
|   - frees a searchST structure
| Input:
|   - searchSTPtr:
|     o pointer to a searchST structure to free
| Output:
|   - Frees:
|     o searchSTPtr
\-------------------------------------------------------*/
void
freeHeap_searchST(
   struct searchST *searchSTPtr
);

/*-------------------------------------------------------\
| Fun18: idToHexAry_maxLimb_searchST
|   - converts an read id to a series of hex numbers and
|     does not go past a max limb count
| Input:
|   - idStr:
|     o c-string with the id to convert
|   - idAryUL:
|     o array of unsigned longs to hold the id
|     o make sure there are at least seventeen empty longs
|       in the array. Allows for ids of length
|       def_maxCharInId_idLkTbl + a extra limb for
|       storing the sum
|   - maxLimbsUC:
|     o number of limbs one read id takes (extras are
|       blanked)
| Output:
|   - Modfies:
|     o idAryUL to hold the converted id
|   - Returns
|     o the number of characters converted in the id
\-------------------------------------------------------*/
unsigned char
idToHexAry_maxLimb_searchST(
   signed char *idStr,
   unsigned long *idAryUL,
   unsigned char maxLimbsUC
);

#endif
