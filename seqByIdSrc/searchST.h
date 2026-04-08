/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' searchST SOF: Start Of File
'   - has functions to build the search table structure
'   o variants:
'     - -DADD_ID_LEN: adds read id length to id long.
'       o slower for ONT. (may be faster for Illuminia??)
'     - -DTHIRTY_TWO_BIT:
'       o for 32 bit cpus (only changes sum part of fun03)
'   o header:
'     - defined variables and guards
'   o .h st01: searchST
'     - holds the hash table and read ids
'   o .c tbl01: hexTblUC_searchST
'     - conversion table of read id character to hex
'   o .h fun01: upperUL_searchST
'     - gets upper half of unsigned long
'   o .h fun02: lowerUL_searchST
'     - gets lower half of unsigned long
'   o fun03: cnvtIdToHexAry_searchST
'     - converts an read id to a series of hex numbers
'   o .c fun04: swapIds_searchST
'     - swaps two ids in a searchST structure
'   o .h fun05: getHash_searchST
'     - get the hash for a read id
'   o fun06: cmpIds_searchST
'     - compares two read ids
'   o fun07: sortIds_searchST
'     - sorts ids in a searchST numerically
'   o fun08: hashSortIds_searchST
'     - sorts ids in a searchST by hash value
'   o fun09: evenLimbs_seachST
'     - takes a read id array with unequal number of limbs
'       (longs) per limb and makes all ids hae the same
'       number of limbs
'   o .c fun10: mkSkip_searchST
'     - converts a sorted list of read ids to a skip list
'   o .c fun11: majicNum_searchST
'     - finds the majic number for an hash table
'   o fun12: mkhash_searchST
'     - takes in a searchST structure with ids and makes a
'       hash table for it
'   o fun12: getReadIds_searchST
'     - get read ids from a file and make a hash table
'   o fun14: searchId_searchST
'     - search for a read id in a searchST structure not
'       set up as a hash table
'   o fun15: searchHash_searchST
'     - search for a read id in a searchST hash table
'   o fun16: blank_searchST
'     - here in case need in future. Does nothing
'   o fun17: init_searchST
'     - initializes a searchST structure
'   o fun18: freeStack_searchST
'     - frees all variables in a searchST structure
'   o fun19: freeHeap_searchST
'     - frees a searchST structure
'   o fun20: idToHexAry_maxLimb_searchST
'     - converts an read id to a series of hex numbers and
'       does not go past a max limb count
'     - this is here for idSearch
'   o fun21: swap_searchST
'     - swaps valus in two searchST structures
'   o license:
'     - licensing for this code (CC0)
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*-------------------------------------------------------\
| Header:
|   - defined variables and guards
\-------------------------------------------------------*/

#ifndef SEARCH_STRUCTURE_H
#define SEARCH_STRUCTURE_H

/*this is here to avoid longs being treated as 32 bit
`   in plan9 (yes it actually happens)
*/
#ifdef PLAN9_64
   typedef unsigned long long ulong_searchST;   
#else
   typedef unsigned long ulong_searchST;   
#endif


#define def_ulBits_searchST (sizeof(ulong_searchST) << 3)

#define def_negULBit_searchST ( (signed long) ((ulong_searchST) 1 << (def_ulBits_searchST - 1)) )

#define def_fileErr_searchST 2
#define def_memErr_searchST 4

/*The frist limb in the ulong_searchST hex id array holds
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

#define def_getNumLimb_searchST (((ulong_searchST) 0xff) << def_adjLimbCnt_searchST)

#define def_adjIdLen_searchST 0xff /*nothing to do*/
#define def_adjLimbCnt_searchST def_ulBits_searchST - 8

#define def_clearNumLimb_searchST (((ulong_searchST) -1) >> 8)

#define def_clearLenId_searchST 0xff

/*-------------------------------------------------------\
| ST01: searchST
|   - holds the hash table and read ids
\-------------------------------------------------------*/
typedef struct searchST
{
   ulong_searchST *idAryUL;   /*array of read ids*/
   ulong_searchST numIdsUL;   /*number read ids in hash*/
   ulong_searchST numLimbsUL; /*number limbs in id array*/
   ulong_searchST lenIdAryUL; /*length of id array*/
   unsigned char maxLimbsUC; /*max limbs in an id*/

   ulong_searchST *hashTblUL; /*hash table to search*/
   ulong_searchST hashSizeUL; /*size of hash table*/
   unsigned char hashPowUC;  /*2^hashPowUC = hashSizeUL*/
   ulong_searchST majicUL;    /*majic for kunths hash*/
}searchST;

/*-------------------------------------------------------\
| Fun01: upperUL_searchST
|   - gets upper half of unsigned long
| Input:
|   - number:
|     o number to convert to half the size of a long
| Output:
|   - Returns
|     o upper half of the long (number)
\-------------------------------------------------------*/
#define upperUL_searchST(number) ( (number) >> (sizeof(ulong_searchST) << 2) )
   /*Logic: convets upper half of long to 1/2 of data type
   `   - shiftBy: sizeof((unsigned long)) << 2):
   `     o number of bits to shift by to remove 1/2 of
   `       a long
   `   - number >> shiftBy:
   `     o shits upper half of long to lower half
   */

/*-------------------------------------------------------\
| Fun02: lowerUL_searchST
|   - gets lower half of unsigned long
| Input:
|   - number:
|     o number to convert to half the size of a long
| Output:
|   - Returns
|     o lower half of the long (number)
\-------------------------------------------------------*/
#define lowerUL_searchST(number) ( (number) & ((ulong_searchST) -1 >> (sizeof(ulong_searchST) << 2)) )
   /*Logic: removes upper half (converts to 1/2 datatype
   `   - shiftBy: sizeof((unsigned long)) << 2):
   `     o number of bits to shift by to remove 1/2 of
   `       a long
   `   - mask: ((unsigned long) -1) >> shiftBy:
   `     o creats a mask that clears the upper bits
   `   - number & mask:
   `     o clears upper half of long
   */


/*-------------------------------------------------------\
| Fun03: cnvtIdToHexAry_searchST
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
   ulong_searchST *idAryUL,
   ulong_searchST *posInUL
);

/*-------------------------------------------------------\
| Fun05: getHash_searchST
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
| Fun06: cmpIds_searchST
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
   ulong_searchST *refIdAryUL,
   ulong_searchST *qryIdAryUL
);

/*-------------------------------------------------------\
| Fun07: sortIds_searchST
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
| Fun08: hashSortIds_searchST
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
| Fun09: evenLimbs_seachST
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
| Fun10: mkSkip_searchST
|   - converts a sorted list of read ids to a skip list
| Input:
|   - idAryUL:
|     - read id array to convert to a skip list
|   - nuIdsUL:
|     - number of ids (index 0) in idAryUL
|   - limbsUC:
|     - number of limbs per read id
|   - firstSkipSC:
|     - always put 1 (for recursive calls)
| Output:
|   - Modifies:
|     o first long for every id in idAryUL to have the
|       number of ids to jump forward or back by
|       - first id is always a negative number
\-------------------------------------------------------*/
void
mkSkip_searchST(
   unsigned long *idAryUL,   /*read id array*/
   signed long firstIdSL,    /*index of first id*/
   signed long numIdsSL,     /*index of last id*/
   unsigned char limbsUC     /*# of limbs in 1 id*/
);

/*-------------------------------------------------------\
| Fun11: majicNum_searchST
|   - finds the majic number for an hash table
|   - this is system independent
| Input:
| Output:
|   - Returns:
|     o the majic number
\-------------------------------------------------------*/
ulong_searchST
majicNum_searchST(
   void
);

/*-------------------------------------------------------\
| Fun12: mkhash_searchST
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
| Fun12: getReadIds_searchST
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
| Fun14: searchId_searchST
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
| Fun15: searchHash_searchST
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
| Fun16: blank_searchST
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
| Fun17: init_searchST
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
| Fun18: freeStack_searchST
|   - frees all variables in a searchST structure
| Input:
|   - searchSTPtr:
|     o pointer to a searchST structure to free variables
| Output:
|   - Frees:
|     o all variables & calls init_searchST (fun15)
\-------------------------------------------------------*/
void
freeStack_searchST(
   struct searchST *searchSTPtr
);

/*-------------------------------------------------------\
| Fun19: freeHeap_searchST
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
| Fun20: idToHexAry_maxLimb_searchST
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
   ulong_searchST *idAryUL,
   unsigned char maxLimbsUC
);

/*-------------------------------------------------------\
| Fun21: swap_searchST
|   - swaps valus in two searchST structures
| Input:
|   - firstST:
|     o pointer to frist searchST struct to swap values in
|   - secST:
|     o pointer to second searchST struct to swap values
| Output:
|   - Modifies:
|     o firstST to have secST values
|     o secST to have firstST values
\-------------------------------------------------------*/
void
swap_searchST(
   struct searchST *firstST,
   struct searchST *secST
);

#endif

/*=======================================================\
: License:
: 
: Creative Commons Legal Code
: 
: CC0 1.0 Universal
: 
:     CREATIVE COMMONS CORPORATION IS NOT A LAW FIRM AND
:     DOES NOT PROVIDE LEGAL SERVICES. DISTRIBUTION OF
:     THIS DOCUMENT DOES NOT CREATE AN ATTORNEY-CLIENT
:     RELATIONSHIP. CREATIVE COMMONS PROVIDES THIS
:     INFORMATION ON AN "AS-IS" BASIS. CREATIVE COMMONS
:     MAKES NO WARRANTIES REGARDING THE USE OF THIS
:     DOCUMENT OR THE INFORMATION OR WORKS PROVIDED
:     HEREUNDER, AND DISCLAIMS LIABILITY FOR DAMAGES
:     RESULTING FROM THE USE OF THIS DOCUMENT OR THE
:     INFORMATION OR WORKS PROVIDED HEREUNDER.
: 
: Statement of Purpose
: 
: The laws of most jurisdictions throughout the world
: automatically confer exclusive Copyright and Related
: Rights (defined below) upon the creator and subsequent
: owner(s) (each and all, an "owner") of an original work
: of authorship and/or a database (each, a "Work").
: 
: Certain owners wish to permanently relinquish those
: rights to a Work for the purpose of contributing to a
: commons of creative, cultural and scientific works
: ("Commons") that the public can reliably and without
: fear of later claims of infringement build upon, modify,
: incorporate in other works, reuse and redistribute as
: freely as possible in any form whatsoever and for any
: purposes, including without limitation commercial
: purposes. These owners may contribute to the Commons to
: promote the ideal of a free culture and the further
: production of creative, cultural and scientific works,
: or to gain reputation or greater distribution for their
: Work in part through the use and efforts of others.
: 
: For these and/or other purposes and motivations, and
: without any expectation of additional consideration or
: compensation, the person associating CC0 with a Work
: (the "Affirmer"), to the extent that he or she is an
: owner of Copyright and Related Rights in the Work,
: voluntarily elects to apply CC0 to the Work and publicly
: distribute the Work under its terms, with knowledge of
: his or her Copyright and Related Rights in the Work and
: the meaning and intended legal effect of CC0 on those
: rights.
: 
: 1. Copyright and Related Rights. A Work made available
:    under CC0 may be protected by copyright and related
:    or neighboring rights ("Copyright and Related
:    Rights"). Copyright and Related Rights include, but
:    are not limited to, the following:
: 
:   i. the right to reproduce, adapt, distribute, perform,
:      display, communicate, and translate a Work;
:  ii. moral rights retained by the original author(s)
:      and/or performer(s);
: iii. publicity and privacy rights pertaining to a
:      person's image or likeness depicted in a Work;
:  iv. rights protecting against unfair competition in
:      regards to a Work, subject to the limitations in
:      paragraph 4(a), below;
:   v. rights protecting the extraction, dissemination,
:      use and reuse of data in a Work;
:  vi. database rights (such as those arising under
:      Directive 96/9/EC of the European Parliament and of
:      the Council of 11 March 1996 on the legal
:      protection of databases, and under any national
:      implementation thereof, including any amended or
:      successor version of such directive); and
: vii. other similar, equivalent or corresponding rights
:      throughout the world based on applicable law or
:      treaty, and any national implementations thereof.
: 
: 2. Waiver. To the greatest extent permitted by, but not
:    in contravention of, applicable law, Affirmer hereby
:    overtly, fully, permanently, irrevocably and
:    unconditionally waives, abandons, and surrenders all
:    of Affirmer's Copyright and Related Rights and
:    associated claims and causes of action, whether now
:    known or unknown (including existing as well as
:    future claims and causes of action), in the Work (i)
:    in all territories worldwide, (ii) for the maximum
:    duration provided by applicable law or treaty
:    (including future time extensions), (iii) in any
:    current or future medium and for any number of
:    copies, and (iv) for any purpose whatsoever,
:    including without limitation commercial, advertising
:    or promotional purposes (the "Waiver"). Affirmer
:    makes the Waiver for the benefit of each member of
:    the public at large and to the detriment of
:    Affirmer's heirs and successors, fully intending that
:    such Waiver shall not be subject to revocation,
:    rescission, cancellation, termination, or any other
:    legal or equitable action to disrupt the quiet
:    enjoyment of the Work by the public as contemplated
:    by Affirmer's express Statement of Purpose.
: 
: 3. Public License Fallback. Should any part of the
:    Waiver for any reason be judged legally invalid or
:    ineffective under applicable law, then the Waiver
:    shall be preserved to the maximum extent permitted
:    taking into account Affirmer's express Statement of
:    Purpose. In addition, to the extent the Waiver is so
:    judged Affirmer hereby grants to each affected person
:    a royalty-free, non transferable, non sublicensable,
:    non exclusive, irrevocable and unconditional license
:    to exercise Affirmer's Copyright and Related Rights
:    in the Work (i) in all territories worldwide, (ii)
:    for the maximum duration provided by applicable law
:    or treaty (including future time extensions), (iii)
:    in any current or future medium and for any number of
:    copies, and (iv) for any purpose whatsoever,
:    including without limitation commercial, advertising
:    or promotional purposes (the "License"). The License
:    shall be deemed effective as of the date CC0 was
:    applied by Affirmer to the Work. Should any part of
:    the License for any reason be judged legally invalid
:    or ineffective under applicable law, such partial
:    invalidity or ineffectiveness shall not invalidate
:    the remainder of the License, and in such case
:    Affirmer hereby affirms that he or she will not (i)
:    exercise any of his or her remaining Copyright and
:    Related Rights in the Work or (ii) assert any
:    associated claims and causes of action with respect
:    to the Work, in either case contrary to Affirmer's
:    express Statement of Purpose.
: 
: 4. Limitations and Disclaimers.
: 
:  a. No trademark or patent rights held by Affirmer are
:     waived, abandoned, surrendered, licensed or
:     otherwise affected by this document.
:  b. Affirmer offers the Work as-is and makes no
:     representations or warranties of any kind concerning
:     the Work, express, implied, statutory or otherwise,
:     including without limitation warranties of title,
:     merchantability, fitness for a particular purpose,
:     non infringement, or the absence of latent or other
:     defects, accuracy, or the present or absence of
:     errors, whether or not discoverable, all to the
:     greatest extent permissible under applicable law.
:  c. Affirmer disclaims responsibility for clearing
:     rights of other persons that may apply to the Work
:     or any use thereof, including without limitation any
:     person's Copyright and Related Rights in the Work.
:     Further, Affirmer disclaims responsibility for
:     obtaining any necessary consents, permissions or
:     other rights required for any use of the Work.
:  d. Affirmer understands and acknowledges that Creative
:     Commons is not a party to this document and has no
:     duty or obligation with respect to this CC0 or use
:     of the Work.
\=======================================================*/
