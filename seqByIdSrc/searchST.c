/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' searchST SOF: Start Of File
'   - has functions to build the search table structure
'   o variants:
'     - -DADD_ID_LEN: adds read id length to id long.
'       o slower for ONT. (may be faster for Illuminia??)
'     - -DTHIRTY_TWO_BIT:
'       o for 32 bit cpus (only changes sum part of fun03)
'   o header:
'     - included libraries and defined variables
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
'   o fun13: getReadIds_searchST
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
   #include<stdlib.h>
#endif

#include "searchST.h"
#include <stdio.h>

#include "../genLib/ulCp.h"


/*.h files only*/
#include "idLkTbl.h"
#include "../genLib/genMath.h" /*max .h macro only*/

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\
! Hidden libraries:
\%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

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
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun03: cnvtIdToHexAry_searchST
   '   o fun03 sec01:
   '     - variable declerations
   '   o fun03 sec02:
   '     - convert the read id
   '   o fun03 sec03:
   '     - add in the id length and number limbs used
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec01:
   ^   - variable declerations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   ulong_searchST
      endLimbUL = *posInUL + def_maxLimbs_idLkTbl;

   ulong_searchST sumPosUL = *posInUL + 1; /*sum limb*/
   ulong_searchST indexPosUL = *posInUL;
      /*limb with index*/

   unsigned char numLimbUC = 0;
   unsigned char lenIdUC = 0;
   unsigned char endLenIdUC = 0;
   unsigned char hexUC = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec02:
   ^   - convert the read id
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(*idStr == '\r')
      ++idStr;
   if(*idStr == '\n')
      ++idStr;
   if(*idStr == '\r')
      ++idStr;

   idStr += ( (*idStr == '@') | (*idStr == '>') );

   idAryUL[sumPosUL] = 0; /*blank total limb*/
   ++(*posInUL);

   while(hexUC != def_invsChar_idLkTbl)
   { /*Loop: convert the read id*/
      ++(*posInUL);
      ++numLimbUC;

      if(*posInUL >= endLimbUL)
         break; /*more characters in id than allowed*/

      idAryUL[(*posInUL)] = def_blankUL_idLkTbl;
      endLenIdUC += def_numElmUL_idLkTbl;

      while(lenIdUC < endLenIdUC)
      { /*Loop: add each id character to the list*/
         hexUC =
            hexTblUC_idLkTbl[(unsigned char) *idStr++];

         if(hexUC == def_invsChar_idLkTbl)
            break;

         ++lenIdUC;

         idAryUL[(*posInUL)] <<= def_elmSizeUL_idLkTbl;
         idAryUL[(*posInUL)] |= hexUC;
      } /*Loop: add each id character to the list*/

      idAryUL[sumPosUL] = 
           upperUL_searchST(idAryUL[*posInUL])
         + lowerUL_searchST(idAryUL[*posInUL]);
   } /*Loop: convert the read id*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec03:
   ^   - add in the id length and number limbs used
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*adds read length to hash. Slower for ONT, not sure
   `  for SRA or Illumina
   */
   #ifdef ADD_ID_LEN
      /*add the number of longs used to the id*/
      idAryUL[sumPosUL] <<= def_maxIdLenBits_searchST;
      idAryUL[sumPosUL] |= lenIdUC;
   #endif

   /*add the number of limbs used. this will be removed
   `  later when I get the final limb count
   */
   idAryUL[indexPosUL] = (ulong_searchST) numLimbUC + 2;
       /*+2 accounts for index and total (sum) limbs*/

   ++(*posInUL); /*move to next empty long in array*/
   return lenIdUC;
} /*cnvtIdToHexAry_searchST*/

/*-------------------------------------------------------\
| Fun04: swapIds_searchST
|   - swaps two ids in a searchST structure
| Input:
|   - hashSTPtr:
|     o pointer to a searchST structure with ids
|       to swap
|   - firstIdUL:
|     o index of first long in first id to swap
|   - secIdUL:
|     o index of first long second id to swap
| Output:
|   - Modifies:
|     o idAryUL in hashSTPtr to have the two ids swaped
\-------------------------------------------------------*/
void
swapIds_searchST( 
   struct searchST *hashSTPtr,
   unsigned long firstIdUL,
   unsigned long secIdUL
){
   unsigned char ucLimbMac = 0;

   for(
      ucLimbMac = 0;
      ucLimbMac < hashSTPtr->maxLimbsUC;
      ++ucLimbMac
   ){ /*Loop: swap ids*/
      hashSTPtr->idAryUL[firstIdUL + ucLimbMac] ^=
         hashSTPtr->idAryUL[secIdUL + ucLimbMac];

      hashSTPtr->idAryUL[secIdUL + ucLimbMac] ^=
         (hashSTPtr)->idAryUL[firstIdUL + ucLimbMac];

      hashSTPtr->idAryUL[firstIdUL + ucLimbMac] ^=
         hashSTPtr->idAryUL[secIdUL + ucLimbMac];
   } /*Loop: swap ids*/
} /*swapIds_searchST*/

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
){
   unsigned char ucLimb = 0;

   signed long retSL =
        (signed long) refIdAryUL[1]
      - (signed long) qryIdAryUL[1];

   if(! retSL)
   { /*If: the sums agree*/
      for(
         ucLimb = 2;
         ucLimb < hashSTPtr->maxLimbsUC;
         ++ucLimb
      ){ /*Loop: compare ids*/
         retSL = refIdAryUL[ucLimb] != qryIdAryUL[ucLimb];

         if(retSL)
         { /*If: values not equal*/
            if(refIdAryUL[ucLimb] >  qryIdAryUL[ucLimb])
               retSL = 1;
            else
               retSL = -1;

            /*branchless
            retSL =
               refIdAryUL[ucLimb] >  qryIdAryUL[ucLimb];

            retSL |= 
              -(refIdAryUL[ucLimb] <  qryIdAryUL[ucLimb]);
            */

            break;
         } /*If: values not equal*/
      } /*Loop: compare ids*/
   } /*If: the sums agree*/

   return retSL;
} /*getHash_searchST*/

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
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun07 TOC:
   '  - Sorts the input array by hashid
   '  - Shell sort taken from:
   '    - Adam Drozdek. 2013. Data Structures and
   '      Algorithims in c++. Cengage Leraning. fourth
   '      edition. pages 505-508
   '    - I made some minor changes, but is mostly the
   '      same
   '  o fun07 sec01:
   '    - Variable declerations
   '  o fun07 sec02:
   '    - Find the number of rounds to sort for
   '  o fun07 sec03:
   '    - Sort the array
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun07 Sec01:
   ^  - Variable declerations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*Number of elements to sort*/
   ulong_searchST numElmUL =
       hashSTPtr->numIdsUL * hashSTPtr->maxLimbsUC;

   /*Number of sorting rounds*/
   ulong_searchST subUL = 0;
   ulong_searchST nextElmUL = 0;
   ulong_searchST lastElmUL = 0;
   ulong_searchST elmOnUL = 0;

   /*Variables to incurment loops*/
   ulong_searchST ulIndex = 0;
   ulong_searchST ulElm = 0;

   signed long idEqlSL = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun07 Sec02:
   ^  - Find the max search value (number rounds to sort)
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*Recursion formula: h[0] = 1, h[n] = 3 * h[n - 1] +1*/
   subUL = 1; /*Initialzie first array*/

   if(numElmUL < 1)
      return; /*nothing (at most 1 item) to sort*/

   while(subUL < numElmUL - 1)
      subUL = (3 * subUL) + 1;

   /*accont for having multiple longs (limbs)*/
   subUL *= hashSTPtr->maxLimbsUC;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun07 Sec03:
   ^  - Sort the arrays in genIndiceST
   ^  o fun07 sec03 sub01:
   ^    - start the sorting loops
   ^  o fun07 sec03 sub02:
   ^    - check if I need to swap ids
   ^  o fun07 sec03 sub03:
   ^    - check if need to move id further back
   ^  o fun07 sec03 sub04:
   ^    - move to the next subset/round
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun07 Sec05 Sub01:
   *   - start the sorting loops
   \*****************************************************/

   while(subUL > 0)
   { /*loop trhough all sub arrays sort the subarrays*/
      for(
        ulIndex = 0;
        ulIndex <= subUL;
        ulIndex += hashSTPtr->maxLimbsUC
      ){ /*For each element in the subarray*/

        for(
            ulElm = ulIndex;
            ulElm + subUL < numElmUL;
            ulElm += subUL
         ){ /*Loop; swap each nth element of subarray*/
           nextElmUL = ulElm + subUL;

           /*********************************************\
           * Fun07 Sec05 Sub02:
           *   - check if I need to swap ids
           \*********************************************/

           idEqlSL =
              cmpIds_searchST(
                 hashSTPtr,
                 &(hashSTPtr->idAryUL[ulElm]),
                 &(hashSTPtr->idAryUL[nextElmUL])
              ); /*> 0 means ulElm id is greater*/

           if(idEqlSL > 0)
           { /*If I need to swap an element*/
               swapIds_searchST(
                  hashSTPtr,
                  ulElm,
                  nextElmUL
               );

              /******************************************\
              * Fun07 Sec05 Sub03:
              *   - check if need to move id further back
              \******************************************/

              lastElmUL = ulElm;
              elmOnUL = ulElm;

              while(lastElmUL >= subUL)
              { /*loop; move swapped element back*/
                 lastElmUL -= subUL;

                  idEqlSL =
                     cmpIds_searchST(
                        hashSTPtr,
                        &(hashSTPtr->idAryUL[elmOnUL]),
                        &(hashSTPtr->idAryUL[lastElmUL])
                     ); /*> 0 means ulElm id is >*/

                 if(idEqlSL > 0)
                    break; /*Positioned the element*/

                swapIds_searchST(
                   hashSTPtr,
                   elmOnUL,
                   lastElmUL
                );

                 elmOnUL = lastElmUL;
              } /*loop; move swapped element back*/
           } /*If I need to swap elements*/
         } /*Loop; swap each nth element of subarray*/
      } /*For each element in the subarray*/

      /**************************************************\
      * Fun07 Sec05 Sub04:
      *   - move to the next subset/round
      \**************************************************/

     subUL /= hashSTPtr->maxLimbsUC;
     subUL = (subUL - 1) / 3;
     subUL *= hashSTPtr->maxLimbsUC;
   } /*loop through all sub arrays to sort the subarrays*/
} /*sortIds_searchST*/

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
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun08 TOC:
   '  - Sorts the input array by hashid
   '  - Shell sort taken from:
   '    - Adam Drozdek. 2013. Data Structures and
   '      Algorithims in c++. Cengage Leraning. fourth
   '      edition. pages 505-508
   '    - I made some minor changes, but is mostly the
   '      same
   '  o fun08 sec01:
   '    - Variable declerations
   '  o fun08 sec02:
   '    - Find the number of rounds to sort for
   '  o fun08 sec03:
   '    - Sort the array
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun08 Sec01:
   ^  - Variable declerations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*Number of elements to sort*/
   ulong_searchST numElmUL =
       (hashSTPtr->numIdsUL - 1) * hashSTPtr->maxLimbsUC;

   /*Number of sorting rounds*/
   ulong_searchST subUL = 0;
   ulong_searchST nextElmUL = 0;
   ulong_searchST lastElmUL = 0;
   ulong_searchST elmOnUL = 0;

   /*Variables to incurment loops*/
   ulong_searchST ulIndex = 0;
   ulong_searchST ulElm = 0;

   signed long hashIdSL = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun08 Sec02:
   ^  - Find the max search value (number rounds to sort)
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*Recursion formula: h[0] = 1, h[n] = 3 * h[n - 1] +1*/
   subUL = 1; /*Initialzie first array*/

   if(numElmUL < 1)
      return; /*nothing (at most 1 item) to sort*/

   while(subUL < numElmUL - 1)
      subUL = (3 * subUL) + 1;

   /*accont for having multiple longs (limbs)*/
   subUL *= hashSTPtr->maxLimbsUC;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun08 Sec03:
   ^  - Sort the arrays in genIndiceST
   ^  o fun08 sec03 sub01:
   ^    - start the sorting loops
   ^  o fun08 sec03 sub02:
   ^    - check if I need to swap ids
   ^  o fun08 sec03 sub03:
   ^    - check if need to move id further back
   ^  o fun08 sec03 sub04:
   ^    - move to the next subset/round
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun08 Sec05 Sub01:
   *   - start the sorting loops
   \*****************************************************/

   while(subUL > 0)
   { /*loop trhough all sub arrays sort the subarrays*/
      for(
        ulIndex = 0;
        ulIndex <= subUL;
        ulIndex += hashSTPtr->maxLimbsUC
      ){ /*For each element in the subarray*/

        for(
            ulElm = ulIndex;
            ulElm + subUL <= numElmUL;
            ulElm += subUL
         ){ /*Loop; swap each nth element of subarray*/
           nextElmUL = ulElm + subUL;

           /*********************************************\
           * Fun08 Sec05 Sub02:
           *   - check if I need to swap ids
           \*********************************************/

           hashIdSL =
              getHash_searchST(
                 hashSTPtr,
                 &(hashSTPtr->idAryUL[ulElm])
              );

           hashIdSL -=
              getHash_searchST(
                 hashSTPtr,
                 &(hashSTPtr->idAryUL[nextElmUL])
              ); /*> 0 if ulElm is > nextElmUL*/

           if(hashIdSL == 0)
           { /*If: hashes are equal*/
              hashIdSL =
                 cmpIds_searchST(
                    hashSTPtr,
                    &(hashSTPtr->idAryUL[ulElm]),
                    &(hashSTPtr->idAryUL[nextElmUL])
                 ); /*> 0 means ulElm id is greater*/
           } /*If: hashes are equal*/

           if(hashIdSL > 0)
           { /*If I need to swap an element*/
               swapIds_searchST(
                  hashSTPtr,
                  ulElm,
                  nextElmUL
               );

              /******************************************\
              * Fun08 Sec05 Sub03:
              *   - check if need to move id further back
              \******************************************/

              lastElmUL = ulElm;
              elmOnUL = ulElm;

              while(lastElmUL >= subUL)
              { /*loop; move swapped element back*/
                 lastElmUL -= subUL;

                 hashIdSL =
                    getHash_searchST(
                       hashSTPtr,
                       &(hashSTPtr->idAryUL[elmOnUL])
                    );

                 hashIdSL -=
                    getHash_searchST(
                       hashSTPtr,
                       &(hashSTPtr->idAryUL[lastElmUL])
                    ); /*> 0 if elmOnUL is > lastElmUL*/

                 if(hashIdSL == 0)
                 { /*If: hashes are equal*/
                    hashIdSL =
                       cmpIds_searchST(
                          hashSTPtr,
                          &(hashSTPtr->idAryUL[elmOnUL]),
                          &(hashSTPtr->idAryUL[lastElmUL])
                       ); /*> 0 means elmOnUL is greater*/
                 } /*If: hashes are equal*/

                 if(hashIdSL > 0)
                    break; /*Positioned the element*/

                 swapIds_searchST(
                    hashSTPtr,
                    elmOnUL,
                    lastElmUL
                 );

                 elmOnUL = lastElmUL;
              } /*loop; move swapped element back*/
           } /*If I need to swap elements*/
         } /*Loop; swap each nth element of subarray*/
      } /*For each element in the subarray*/

      /**************************************************\
      * Fun08 Sec05 Sub04:
      *   - move to the next subset/round
      \**************************************************/

     subUL /= hashSTPtr->maxLimbsUC;
     subUL = (subUL - 1) / 3;
     subUL *= hashSTPtr->maxLimbsUC;
   } /*loop through all sub arrays to sort the subarrays*/
} /*hashSortIds_searchST*/

/*-------------------------------------------------------\
| Fun09: evenLimbs_seachST
|   - takes a read id array with unequal number of limbs
|     (longs) per limb and makes all ids hae the same
|     number of limbs
| Input:
|   - searchSTPtr:
|     o pointer to a searchST structure with unven ids and
|       evens out the ids
| Output:
|   - Modifies:
|     o idAryUL in searchST structure to have equal limb
|       lengths
|     o maxLimbsUC to +2 for the sum and index longs
| Note:
|   - always sort ids after this function. The first few
|     ids are always added to the end
|   - this assumes you have plenty of memory in the array
\-------------------------------------------------------*/
void
evenLimbs_searchST(
   struct searchST *searchSTPtr
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun09 TOC:
   '   - takes a read id array with unequal number of
   '     limbs (longs) per limb and makes all ids have the
   '     same number of limbs
   '   o fun09 sec01:
   '     - variable declarations
   '   o fun09 sec02:
   '     - add first few ids to buffer array
   '   o fun09 sec03:
   '     - make all ids have same number of limbs
   '   o fun09 sec04:
   '     - add first few ids in buffer to the end of array
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun09 Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   ulong_searchST numLimbsUL = 0;
   ulong_searchST endAryUL = 0; /*list long in id array*/
   ulong_searchST cpPosUL = 0;
   ulong_searchST dupPosUL = 0;
   ulong_searchST ulId = 0;

   ulong_searchST ulLimb = 0;  /*copy index for shifting*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun09 Sec02:
   ^   - blank id list and set up for loop
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   endAryUL =
      searchSTPtr->maxLimbsUC * searchSTPtr->numIdsUL;

   if(endAryUL == searchSTPtr->numLimbsUL)
   { /*If: only need to blank limb counts*/
      for(
         cpPosUL = 0;
         cpPosUL < endAryUL;
         cpPosUL += searchSTPtr->maxLimbsUC
      ) searchSTPtr->idAryUL[cpPosUL] = 0;

      return; /*everything is evened out already*/
   } /*If: only need to blank limb counts*/

   --endAryUL; /*convert to index 0*/

   /*number of limbs used in last id*/
   numLimbsUL = searchSTPtr->numLimbsUL;
   cpPosUL += numLimbsUL - 1; /*get on last limb*/
   numLimbsUL = searchSTPtr->idAryUL[numLimbsUL];

   dupPosUL = endAryUL;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun09 Sec03:
   ^   - start loop and even ids
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   while(ulId < searchSTPtr->numIdsUL)
   { /*Loop: even out read ids*/
      ulLimb = 0;

      while(numLimbsUL < searchSTPtr->maxLimbsUC)
      { /*Loop: add in the blank limbs*/
         searchSTPtr->idAryUL[dupPosUL] =
             def_blankUL_idLkTbl;

         ++numLimbsUL;
         ++ulLimb;
         --dupPosUL;
      } /*Loop: add in the blank limbs*/

      while(ulLimb < numLimbsUL)
      { /*Loop: copy over limbs*/
         searchSTPtr->idAryUL[dupPosUL] =
            searchSTPtr->idAryUL[cpPosUL];

         ++ulLimb;
         --cpPosUL;
         --dupPosUL;
      } /*Loop: copy over limbs*/

      numLimbsUL = searchSTPtr->idAryUL[cpPosUL + 1];
      searchSTPtr->idAryUL[dupPosUL + 1] = 0;

      ++ulId;
   } /*Loop: even out read ids*/

   searchSTPtr->numLimbsUL = endAryUL;
} /*evenLimbs_searchST*/

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
){
   signed long midSL = (numIdsSL + firstIdSL) >> 1;
   unsigned int *posAryUI =
      (unsigned int *) &idAryUL[midSL * limbsUC];

   if(midSL <= 0)
      return;

   if(firstIdSL < midSL)
   { /*If: have left ids*/
      posAryUI[0] = ((midSL + firstIdSL) >> 1) * limbsUC;

      mkSkip_searchST(
         idAryUL,
         firstIdSL,
         midSL - 1,
         limbsUC
      );
   } /*If: have left ids*/

   if(midSL < numIdsSL)
   { /*If: have ids on right side*/
      posAryUI[1] = ((numIdsSL + midSL) >> 1) * limbsUC;

      mkSkip_searchST(
         idAryUL,
         midSL,
         numIdsSL + 1,
         limbsUC
      );
   } /*If: have ids on right side*/

   return;
} /*mkSkip_searchST*/

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
){
   unsigned char ucDig = 0;
   ulong_searchST majicNumUL = 0;
   unsigned char goldNumStr[]="618033988749894848204";
      /*First 21 digits of the golden ratio/number*/

   unsigned char numDigUC =
      (def_ulBits_searchST / 3.3333333333333333333);
      /* Logic
      `   the idea here is 2^(x * 3.333...) ~ 10^x.
      `   this gives me an rough idea, but not fully 
      `     accurate number of base 10 digits per limb.
      `   one problem is 64 bit numbers, becuase
      `     64 / 3.333... ~ 19. 
      `   However, the max for 64 bits is actually 20
      `     digits 18,446,744,073,709,551,615.
      */

   /*Grab the first digits in the golden ratio*/
   for(
      ucDig = 0;
      ucDig < numDigUC;
      ++ucDig
   ){ /*Loop: find the majic number*/
      majicNumUL =
          (10 * majicNumUL) + (goldNumStr[ucDig] & ~48);
   } /*Loop: find the majic number*/

   return majicNumUL;
} /*majicNum_searchST*/

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
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun12 TOC:
   '   - takes in a searchST structure with ids and makes
   '     a hash table for it
   '   o fun12 sec01:
   '     - variable declarations
   '   o fun12 sec02:
   '     - find key values needed for hashing
   '   o fun12 sec03:
   '     - make sure id array has enough room for hash
   '       table
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   ulong_searchST posUL = 0;
   ulong_searchST hashUL = 0; /*hash value of current id*/

   ulong_searchST lastHashUL = 0;/*hash value looking at*/
   ulong_searchST oldPosUL = 0;  /*index of lastHashUL*/

   ulong_searchST *tmpULPtr = 0;
      /*for reallocs or iterating*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun12 Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun12 Sec02:
   ^   - find key values needed for hashing
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   searchSTPtr->majicUL = majicNum_searchST();

   /*find the hash size*/
   searchSTPtr->hashPowUC = 0;
   searchSTPtr->hashSizeUL = 1;

   while(searchSTPtr->hashSizeUL < searchSTPtr->numIdsUL)
   { /*Loop: find closest power of two to has values*/
      ++searchSTPtr->hashPowUC;
      searchSTPtr->hashSizeUL <<= 1;
   } /*Loop: find closest power of two to has values*/

   ++searchSTPtr->hashPowUC;
   searchSTPtr->hashSizeUL <<= 1; /*double hash array*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun12 Sec03:
   ^   - make sure id array has enough room for hash table
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   posUL = searchSTPtr->numLimbsUL;
   posUL += searchSTPtr->hashSizeUL + 1;

   if(searchSTPtr->lenIdAryUL < posUL)
   { /*If: not enough array to hold the has values*/
      tmpULPtr =
         realloc(
            searchSTPtr->idAryUL,
            posUL * sizeof(ulong_searchST)
         );

      if(! tmpULPtr)
         goto memErr_fun12_sec05;

      searchSTPtr->idAryUL = tmpULPtr;
   } /*If: not enough array to hold the has values*/
   
   /*set up hash table pointer*/
   posUL = searchSTPtr->numLimbsUL + 1;
     /*+ 1 avoids array out of bounds error*/

   searchSTPtr->hashTblUL = searchSTPtr->idAryUL + posUL;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun12 Sec04:
   ^   - sort ids by hash and set hash values and indexs
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   hashSortIds_searchST(searchSTPtr);

   oldPosUL = 0;
   lastHashUL = -1;

   /*initialize the hash table*/
   for(
      posUL = 0;
      posUL < searchSTPtr->hashSizeUL;
      ++posUL
   ) searchSTPtr->hashTblUL[posUL] = -1;

   for(
      posUL = 0;
      posUL < searchSTPtr->numLimbsUL;
      posUL += searchSTPtr->maxLimbsUC
   ){ /*Loop: find number of ids in hash table*/
      hashUL =
         getHash_searchST(
            searchSTPtr,
            &searchSTPtr->idAryUL[posUL]
         );

      if(hashUL == lastHashUL)
         ++(searchSTPtr->idAryUL[oldPosUL]);
         /*count number of read ids with same hash*/

      else
      { /*Else: this is a new hash value*/
         lastHashUL = hashUL;
         oldPosUL = posUL;
         searchSTPtr->idAryUL[oldPosUL] = 0;
         searchSTPtr->hashTblUL[lastHashUL] = oldPosUL;
      } /*Else: this is a new hash value*/
   } /*Loop: find number of ids in hash table*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun12 Sec05:
   ^   - return
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   return 0;

   memErr_fun12_sec05:;
      return 1;
} /*mkhash_searchST*/

/*-------------------------------------------------------\
| Fun13: getReadIds_searchST
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
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun13 TOC:
   '   - get the read ids and build the hash table
   '   o fun13 sec01:
   '     - variable declerations
   '   o fun13 sec02:
   '     - open files and initialize variables
   '   o fun13 sec03:
   '     - read in the read ids
   '   o fun13 sec04:
   '     - even limbs, sort arrays, and make hash tables
   '   o fun13 sec05:
   '     - clean up and return
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun13 Sec01:
   ^   - variable declerations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   #define lenBuffUS 4096  /*maximum size of buffer*/
   signed char buffStr[lenBuffUS];
      /*buffer for file input*/
   ulong_searchST ulId = 0;

   ulong_searchST newBytesUL = 0;/*bytes read by fread*/
   ulong_searchST bytesUL = 0;   /*number bytes in buffer*/
   ulong_searchST posUL = 0;     /*position in id array*/
   ulong_searchST oldPosUL = 0;  /*old id array position*/

   ulong_searchST idLenUL = 0;
   ulong_searchST lastIdLenUL = 0;

   ulong_searchST *tmpULPtr;          /*for reallocs*/
   struct searchST *hashHeapST = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun13 Sec02:
   ^   - open files and initialize variables
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   hashHeapST = malloc(sizeof(struct searchST));

   if(! hashHeapST)
      goto memErr_fun13_sec05_sub02;

   init_searchST(hashHeapST);
     /*will allocate some memroy for read id array*/

   /*add memory to the id array*/
   hashHeapST->idAryUL =
      malloc(1024 * 5 * sizeof(ulong_searchST));

   if(! hashHeapST->idAryUL)
      goto memErr_fun13_sec05_sub02;
   
   hashHeapST->lenIdAryUL = 1024 * 5;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun13 Sec03:
   ^   - read in the read ids
   ^   o fun13 sec03 sub01:
   ^     - read in first part of file and start loop
   ^   o fun13 sec03 sub02:
   ^     - check if need to resize read id array
   ^   o fun13 sec03 sub03:
   ^     - check if need to read more file
   ^   o fun13 sec03 sub04:
   ^     - convert read id
   ^   o fun13 sec03 sub05:
   ^     - find end of read id line
   ^   o fun13 sec03 sub06:
   ^     - check if have a few bytes remaing to process
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun13 Sec03 Sub01:
   *   - read in first part of file and start loop
   \*****************************************************/

   newBytesUL =
      fread(
         (char *) buffStr,
         sizeof(unsigned char),
         lenBuffUS - 1,
         idFILE
      ); /*read in the file*/

   bytesUL = newBytesUL;
   buffStr[bytesUL] = '\0';
   hashHeapST->maxLimbsUC = 0;

   while(newBytesUL)
   { /*Loop: read in each read id*/

      /**************************************************\
      * Fun13 Sec03 Sub02:
      *   - check if I need to resize read id array
      \**************************************************/

      if(
           hashHeapST->lenIdAryUL - posUL
         < def_maxLimbs_idLkTbl
      ){ /*If: I need to make an larger array*/
         hashHeapST->lenIdAryUL = hashHeapST->numIdsUL;

         hashHeapST->lenIdAryUL *= 
            (hashHeapST->maxLimbsUC + 2);
            /*1 long for sum, another for index limb*/

         /*increas array size by 50%*/
         hashHeapST->lenIdAryUL +=
            (hashHeapST->lenIdAryUL >> 1);

         tmpULPtr =
            realloc(
               hashHeapST->idAryUL,
                 hashHeapST->lenIdAryUL
               * sizeof(ulong_searchST)
            );

         if(! tmpULPtr)
            goto memErr_fun13_sec05_sub02;

         hashHeapST->idAryUL = tmpULPtr;
      } /*If: I need to make an larger array*/
      
      /**************************************************\
      * Fun13 Sec03 Sub03:
      *   - check if I need to read more file
      \**************************************************/

      if(ulId + def_maxCharInId_idLkTbl >= bytesUL)
      { /*If: I need to get more file*/
         bytesUL -= ulId;

         cpLen_ulCp(
            buffStr,
            &buffStr[ulId],
            bytesUL
         );

         newBytesUL =
            fread(
               (char *) &buffStr[bytesUL],
               sizeof(signed char),
               lenBuffUS - (1 + bytesUL),
               idFILE
            ); /*read in the file*/

         bytesUL += newBytesUL;
         ulId = 0;
         buffStr[bytesUL] = '\0';
      } /*If: I need to get more file*/

      /**************************************************\
      * Fun13 Sec03 Sub04:
      *   - convert read id
      \**************************************************/

      /*on the last few characters*/
      finishId_fun13_sec03_sub04:;

      oldPosUL = posUL;

      ulId +=
         cnvtIdToHexAry_searchST(
            &buffStr[ulId],
            hashHeapST->idAryUL,
            &posUL
         ); /*convert the read id*/

      ++ulId; /*get off white space*/

      /*make sure final limb count is always at end*/
      idLenUL =
         (unsigned char) hashHeapST->idAryUL[oldPosUL];
      hashHeapST->idAryUL[posUL] = idLenUL;

      /*add the previos read ids length in*/
      hashHeapST->idAryUL[oldPosUL] = lastIdLenUL;
      lastIdLenUL = idLenUL;

      hashHeapST->maxLimbsUC =
         max_genMath(
            hashHeapST->maxLimbsUC,
            (unsigned char) idLenUL
         ); /*find the most limbs used for a single id*/

      hashHeapST->numLimbsUL = posUL;
      ++hashHeapST->numIdsUL;

      /**************************************************\
      * Fun13 Sec03 Sub05:
      *   - find end of read id line
      \**************************************************/

      /*do not want carraige returns here*/
      ulId += endLineUnix_ulCp(&buffStr[ulId]);

      while( buffStr[ulId] == '\0')
      { /*Loop: till on the next id (next line)*/
         newBytesUL =
            fread(
               (char *) buffStr,
               sizeof(signed char),
               lenBuffUS - 1,
               idFILE
            ); /*read in the file*/

         bytesUL = newBytesUL;
         ulId = 0;
         buffStr[bytesUL] = '\0';

         if(newBytesUL == 0)
            break;

         ulId += endLineUnix_ulCp(&buffStr[ulId]);
      } /*Loop: till on the next id (next line)*/
   } /*Loop: read in each read id*/

   /*****************************************************\
   * Fun13 Sec03 Sub06:
   *   - check if have a few bytes remaing to process
   \*****************************************************/

   if(bytesUL - ulId == 1) /*false most of the time*/
      ++ulId;

   /*checked if I finished the last line*/
   if(ulId < bytesUL)
      goto finishId_fun13_sec03_sub04;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun13 Sec04:
   ^   - even limbs, sort arrays, and make hash tables
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*make sure all ids have same number of limbs*/
   evenLimbs_searchST(hashHeapST);

   if(hashBl)
      mkhash_searchST(hashHeapST); /*does sorting*/
   else
      sortIds_searchST(hashHeapST);

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun13 Sec05:
   ^   - clean up and return
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   return hashHeapST;

   /*deal with errors (memory only)*/

   memErr_fun13_sec05_sub02:;

   *errSC = def_memErr_searchST;
   freeHeap_searchST(hashHeapST);
   hashHeapST = 0;

   return 0;
} /*getReadIds_searchST*/

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
){
   signed long midSL = 0;
   signed long rightSL = 0;
   signed long leftSL = 0;
   signed long eqlSL = 0;

   rightSL = searchSTPtr->numIdsUL - 1;

   while(leftSL <= rightSL)
   { /*Loop: Search for the querys index*/
     midSL = (leftSL + rightSL) >> 1;

     eqlSL =
        cmpIds_searchST(
           (searchSTPtr),
           (qryIdAryUL),
           &(searchSTPtr)->idAryUL[
                 midSL * searchSTPtr->maxLimbsUC
           ]
        );

     if(eqlSL > 0)
         leftSL = midSL + 1;

     else if(eqlSL < 0)
         rightSL = midSL - 1;

     else
        return midSL; /*found query*/
   } /*Loop: Search for the querys index*/
   return def_negULBit_searchST;
} /*searchForId_searchST*/

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
){
   signed long midSL = 0;
   signed long leftSL = 0;
   signed long rightSL = 0;
   signed long indexSL = 0;
   signed long eqlSL = 0;

   indexSL =
      getHash_searchST(
         searchSTPtr,
         qryIdAryUL
      ); /*find the hash value*/

   indexSL =
      (signed long) searchSTPtr->hashTblUL[indexSL];

   if(indexSL < 0)
      return def_negULBit_searchST; /*not in hash table*/

   leftSL = 0;
   rightSL = (signed long) searchSTPtr->idAryUL[indexSL];

   while(leftSL <= rightSL)
   { /*Loop: Search for the querys index*/
       midSL = ((rightSL + leftSL) >> 1);

      eqlSL =
         cmpIds_searchST(
            searchSTPtr,
            qryIdAryUL,
            &searchSTPtr->idAryUL[
              (midSL * searchSTPtr->maxLimbsUC) + indexSL
           ]
         );

      if(eqlSL > 0)
          leftSL = midSL + 1;

      else if(eqlSL < 0)
          rightSL = midSL - 1;

      else
         return midSL + indexSL;
   } /*Loop: Search for the querys index*/

   return def_negULBit_searchST;
} /*searchHash_searchST*/

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
){
   searchSTPtr = searchSTPtr;
} /*blank_searchST*/
   
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
){
   searchSTPtr->idAryUL = 0;
   searchSTPtr->numIdsUL = 0;
   searchSTPtr->numLimbsUL = 0;
   searchSTPtr->lenIdAryUL = 0;
   searchSTPtr->maxLimbsUC = 0;

   searchSTPtr->hashTblUL = 0;
   searchSTPtr->hashSizeUL = 0;
   searchSTPtr->hashPowUC = 0;
   searchSTPtr->majicUL = 0;

   blank_searchST(searchSTPtr);
} /*init_searchST*/

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
){
   if(! searchSTPtr)
      return; /*nothing to do*/

   if(searchSTPtr->idAryUL < searchSTPtr->hashTblUL)
   { /*If: hash table could be part of idAry block*/
      if(
           searchSTPtr->idAryUL + searchSTPtr->lenIdAryUL
         <= searchSTPtr->hashTblUL
      ) free(searchSTPtr->hashTblUL); /*separate mallocs*/
   } /*If: hash table could be part of idAry block*/

   else
      free(searchSTPtr->hashTblUL);

   searchSTPtr->hashTblUL = 0;

   free(searchSTPtr->idAryUL);
   searchSTPtr->idAryUL = 0;

   init_searchST(searchSTPtr);
} /*freeStack_searchST*/

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
){
   freeStack_searchST(searchSTPtr);
   free(searchSTPtr);
} /*freeHeap_searchST*/

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
){
   ulong_searchST ulPos = 0; /*first limb to add char to*/
   unsigned char lenIdUC = 0;

   lenIdUC = 
      cnvtIdToHexAry_searchST(
         idStr,
         idAryUL,
         &ulPos
      );

   while(ulPos < maxLimbsUC)
      idAryUL[ulPos++] = def_blankUL_idLkTbl;

   return lenIdUC;
} /*idToHexAry_maxLimb_searchST*/

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
){
   ulong_searchST *swapUL;

   swapUL = firstST->idAryUL;
   firstST->idAryUL = secST->idAryUL;
   secST->idAryUL = swapUL;
   
   firstST->numIdsUL ^= secST->numIdsUL;
   secST->numIdsUL ^= firstST->numIdsUL;
   firstST->numIdsUL ^= secST->numIdsUL;

   firstST->numLimbsUL ^= secST->numLimbsUL;
   secST->numLimbsUL ^= firstST->numLimbsUL;
   firstST->numLimbsUL ^= secST->numLimbsUL;

   firstST->lenIdAryUL ^= secST->lenIdAryUL;
   secST->lenIdAryUL ^= firstST->lenIdAryUL;
   firstST->lenIdAryUL ^= secST->lenIdAryUL;

   firstST->maxLimbsUC ^= secST->maxLimbsUC;
   secST->maxLimbsUC ^= firstST->maxLimbsUC;
   firstST->maxLimbsUC ^= secST->maxLimbsUC;


   swapUL = firstST->hashTblUL;
   firstST->hashTblUL = secST->hashTblUL;
   secST->hashTblUL = swapUL;

   firstST->hashSizeUL ^= secST->hashSizeUL;
   secST->hashSizeUL ^= firstST->hashSizeUL;
   firstST->hashSizeUL ^= secST->hashSizeUL;

   firstST->hashPowUC ^= secST->hashPowUC;
   secST->hashPowUC ^= firstST->hashPowUC;
   firstST->hashPowUC ^= secST->hashPowUC;

   firstST->majicUL ^= secST->majicUL;
   secST->majicUL ^= firstST->majicUL;
   firstST->majicUL ^= secST->majicUL;
} /*swap_searchST*/

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
