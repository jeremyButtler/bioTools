/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' searchST SOF: Start Of File
'   - has functions to build the search table structure
'   o variants:
'     - -DADD_ID_LEN: adds read id length to id long.
'       o slower for ONT. (may be faster for Illuminia??)
'     - -DTHIRTY_TWO_BIT:
'       o for 32 bit cpus (only changes sum part of fun01)
'   o header:
'     - included libraries and defined variables
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
'   o fun11: getReadIds_searchST
'     - get read ids from a file and make a hash table
'   o fun12: searchId_searchST
'     - search for a read id in a searchST structure not
'       set up as a hash table
'   o fun13: searchHash_searchST
'     - search for a read id in a searchST hash table
'   o fun14: blank_searchST
'     - here in case need in future. Does nothing
'   o fun15: init_searchST
'     - initializes a searchST structure
'   o fun16: freeStack_searchST
'     - frees all variables in a searchST structure
'   o fun17: freeHeap_searchST
'     - frees a searchST structure
'   o fun18: idToHexAry_maxLimb_searchST
'     - converts an read id to a series of hex numbers and
'       does not go past a max limb count
'     - this is here for idSearch
'   o license:
'     - licensing for this code (public domain / mit)
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*-------------------------------------------------------\
| Header:
|   - included libraries
\-------------------------------------------------------*/

#ifdef PLAN9
   #include <u.h>
   #include <libc.c>
#else
   #include<stdlib.h>
#endif

#include "searchST.h"
#include <stdio.h>

#include "../genLib/ulCp.h"


/*.h files only*/
#include "../genLib/dataTypeShortHand.h"
#include "idLkTbl.h"
#include "../genLib/genMath.h" /*max .h macro only*/

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\
! Hidden libraries:
\%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

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
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun01: cnvtIdToHexAry_searchST
   '   o fun01 sec01:
   '     - variable declerations
   '   o fun01 sec02:
   '     - convert the read id
   '   o fun01 sec03:
   '     - add in the id length and number limbs used
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun01 Sec01:
   ^   - variable declerations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   ulong endLimbUL = *posInUL + def_maxLimbs_idLkTbl;
   ulong sumPosUL = *posInUL + 1; /*sum limb*/
   ulong indexPosUL = *posInUL; /*limb with the index*/
   uchar numLimbUC = 0;

   uchar lenIdUC = 0;
   uchar endLenIdUC = 0;
   uchar hexUC = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun01 Sec02:
   ^   - convert the read id
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   idStr += (*idStr == '\n');
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
         hexUC = hexTblUC_idLkTbl[(uchar) *idStr++];

         if(hexUC == def_invsChar_idLkTbl)
            break;

         ++lenIdUC;

         idAryUL[(*posInUL)] <<= def_elmSizeUL_idLkTbl;
         idAryUL[(*posInUL)] |= hexUC;
      } /*Loop: add each id character to the list*/

      #ifdef THIRTY_TWO_BIT
         idAryUL[sumPosUL] = 
            ((ushort *) &idAryUL[*posInUL])[0] +
            ((ushort *) &idAryUL[*posInUL])[1];
      #else
         idAryUL[sumPosUL] = 
            ((uint *) &idAryUL[*posInUL])[0] +
            ((uint *) &idAryUL[*posInUL])[1];
      #endif
   } /*Loop: convert the read id*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun01 Sec03:
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
   idAryUL[indexPosUL] = (ulong) numLimbUC + 2;
       /*+2 accounts for index and total (sum) limbs*/

   ++(*posInUL); /*move to next empty long in array*/
   return lenIdUC;
} /*cnvtIdToHexAry_searchST*/

/*-------------------------------------------------------\
| Fun02: swapIds_searchST
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
   uchar ucLimbMac = 0;

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
){
   unsigned char ucLimb = 0;

   signed long retSL =
        (slong) refIdAryUL[1] - (slong) qryIdAryUL[1];

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
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun05 TOC:
   '  - Sorts the input array by hashid
   '  - Shell sort taken from:
   '    - Adam Drozdek. 2013. Data Structures and
   '      Algorithims in c++. Cengage Leraning. fourth
   '      edition. pages 505-508
   '    - I made some minor changes, but is mostly the
   '      same
   '  o fun05 sec01:
   '    - Variable declerations
   '  o fun05 sec02:
   '    - Find the number of rounds to sort for
   '  o fun05 sec03:
   '    - Sort the array
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun05 Sec01:
   ^  - Variable declerations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*Number of elements to sort*/
   ulong numElmUL =
       hashSTPtr->numIdsUL * hashSTPtr->maxLimbsUC;

   /*Number of sorting rounds*/
   ulong subUL = 0;
   ulong nextElmUL = 0;
   ulong lastElmUL = 0;
   ulong elmOnUL = 0;

   /*Variables to incurment loops*/
   ulong ulIndex = 0;
   ulong ulElm = 0;

   slong idEqlSL = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun05 Sec02:
   ^  - Find the max search value (number rounds to sort)
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*Recursion formula: h[0] = 1, h[n] = 3 * h[n - 1] +1*/
   subUL = 1; /*Initialzie first array*/

   while(subUL < numElmUL - 1)
      subUL = (3 * subUL) + 1;

   /*accont for having multiple longs (limbs)*/
   subUL *= hashSTPtr->maxLimbsUC;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun05 Sec03:
   ^  - Sort the arrays in genIndiceST
   ^  o fun05 sec03 sub01:
   ^    - start the sorting loops
   ^  o fun05 sec03 sub02:
   ^    - check if I need to swap ids
   ^  o fun05 sec03 sub03:
   ^    - check if need to move id further back
   ^  o fun05 sec03 sub04:
   ^    - move to the next subset/round
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun05 Sec05 Sub01:
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
           * Fun05 Sec05 Sub02:
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
              * Fun05 Sec05 Sub03:
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
      * Fun05 Sec05 Sub04:
      *   - move to the next subset/round
      \**************************************************/

     subUL /= hashSTPtr->maxLimbsUC;
     subUL = (subUL - 1) / 3;
     subUL *= hashSTPtr->maxLimbsUC;
   } /*loop through all sub arrays to sort the subarrays*/
} /*sortIds_searchST*/

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
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun06 TOC:
   '  - Sorts the input array by hashid
   '  - Shell sort taken from:
   '    - Adam Drozdek. 2013. Data Structures and
   '      Algorithims in c++. Cengage Leraning. fourth
   '      edition. pages 505-508
   '    - I made some minor changes, but is mostly the
   '      same
   '  o fun06 sec01:
   '    - Variable declerations
   '  o fun06 sec02:
   '    - Find the number of rounds to sort for
   '  o fun06 sec03:
   '    - Sort the array
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun06 Sec01:
   ^  - Variable declerations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*Number of elements to sort*/
   ulong numElmUL =
       (hashSTPtr->numIdsUL - 1) * hashSTPtr->maxLimbsUC;

   /*Number of sorting rounds*/
   ulong subUL = 0;
   ulong nextElmUL = 0;
   ulong lastElmUL = 0;
   ulong elmOnUL = 0;

   /*Variables to incurment loops*/
   ulong ulIndex = 0;
   ulong ulElm = 0;

   slong hashIdSL = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun06 Sec02:
   ^  - Find the max search value (number rounds to sort)
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*Recursion formula: h[0] = 1, h[n] = 3 * h[n - 1] +1*/
   subUL = 1; /*Initialzie first array*/

   while(subUL < numElmUL - 1)
      subUL = (3 * subUL) + 1;

   /*accont for having multiple longs (limbs)*/
   subUL *= hashSTPtr->maxLimbsUC;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun06 Sec03:
   ^  - Sort the arrays in genIndiceST
   ^  o fun06 sec03 sub01:
   ^    - start the sorting loops
   ^  o fun06 sec03 sub02:
   ^    - check if I need to swap ids
   ^  o fun06 sec03 sub03:
   ^    - check if need to move id further back
   ^  o fun06 sec03 sub04:
   ^    - move to the next subset/round
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun06 Sec05 Sub01:
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
           * Fun06 Sec05 Sub02:
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
              * Fun06 Sec05 Sub03:
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
      * Fun06 Sec05 Sub04:
      *   - move to the next subset/round
      \**************************************************/

     subUL /= hashSTPtr->maxLimbsUC;
     subUL = (subUL - 1) / 3;
     subUL *= hashSTPtr->maxLimbsUC;
   } /*loop through all sub arrays to sort the subarrays*/
} /*hashSortIds_searchST*/

/*-------------------------------------------------------\
| Fun07: evenLimbs_seachST
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
   ' Fun07 TOC:
   '   - takes a read id array with unequal number of
   '     limbs (longs) per limb and makes all ids have the
   '     same number of limbs
   '   o fun07 sec01:
   '     - variable declarations
   '   o fun07 sec02:
   '     - add first few ids to buffer array
   '   o fun07 sec03:
   '     - make all ids have same number of limbs
   '   o fun07 sec04:
   '     - add first few ids in buffer to the end of array
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun07 Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   ulong numLimbsUL = 0;
   ulong endAryUL = 0; /*list long in the id array*/
   ulong cpPosUL = 0;
   ulong dupPosUL = 0;
   ulong ulId = 0;

   ulong ulLimb = 0;   /*copy index for shifting*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun07 Sec02:
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
   ^ Fun07 Sec03:
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
| Fun08: mkSkip_searchST
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
   slong midSL = (numIdsSL + firstIdSL) >> 1;
   uint *posAryUI = (uint *) &idAryUL[midSL * limbsUC];

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
| Fun09: majicNum_searchST
|   - finds the majic number for an hash table
|   - this is system independent
| Input:
| Output:
|   - Returns:
|     o the majic number
\-------------------------------------------------------*/
unsigned long
majicNum_searchST(
){
   unsigned char ucDig = 0;
   unsigned long majicNumUL = 0;
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
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun10 TOC:
   '   - takes in a searchST structure with ids and makes
   '     a hash table for it
   '   o fun10 sec01:
   '     - variable declarations
   '   o fun10 sec02:
   '     - find key values needed for hashing
   '   o fun10 sec03:
   '     - make sure id array has enough room for hash
   '       table
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   ulong posUL = 0;
   ulong hashUL = 0;          /*hash value of current id*/

   ulong lastHashUL = 0;      /*hash value looking at*/
   ulong oldPosUL = 0;        /*index of lastHashUL*/

   ulong *tmpULPtr = 0; /*for reallocs or iterating*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun10 Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun10 Sec02:
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
   ^ Fun10 Sec03:
   ^   - make sure id array has enough room for hash table
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   posUL = searchSTPtr->numLimbsUL;
   posUL += searchSTPtr->hashSizeUL + 1;

   if(searchSTPtr->lenIdAryUL < posUL)
   { /*If: not enough array to hold the has values*/
      tmpULPtr =
         realloc(
            searchSTPtr->idAryUL,
            posUL * sizeof(ulong)
         );

      if(! tmpULPtr)
         goto memErr_fun10_sec05;

      searchSTPtr->idAryUL = tmpULPtr;
   } /*If: not enough array to hold the has values*/
   
   /*set up hash table pointer*/
   posUL = searchSTPtr->numLimbsUL + 1;
     /*+ 1 avoids array out of bounds error*/

   searchSTPtr->hashTblUL = searchSTPtr->idAryUL + posUL;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun10 Sec04:
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
   ^ Fun10 Sec05:
   ^   - return
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   return 0;

   memErr_fun10_sec05:;

   return 1;
} /*mkhash_searchST*/

/*-------------------------------------------------------\
| Fun11: getReadIds_searchST
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
   ' Fun11 TOC:
   '   - get the read ids and build the hash table
   '   o fun11 sec01:
   '     - variable declerations
   '   o fun11 sec02:
   '     - open files and initialize variables
   '   o fun11 sec03:
   '     - read in the read ids
   '   o fun11 sec04:
   '     - even limbs, sort arrays, and make hash tables
   '   o fun11 sec05:
   '     - clean up and return
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun11 Sec01:
   ^   - variable declerations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   sshort lenBuffUS = 4096;  /*maximum size of buffer*/
   schar buffStr[lenBuffUS]; /*buffer for file input*/
   ulong ulId = 0;

   ulong newBytesUL = 0;     /*bytes read by fread*/
   ulong bytesUL = 0;        /*number bytes in buffer*/
   ulong posUL = 0;     /*position in id array*/
   ulong oldPosUL = 0;     /*old position in id array*/

   ulong idLenUL = 0;
   ulong lastIdLenUL = 0;

   ulong *tmpULPtr;          /*for reallocs*/
   struct searchST *hashHeapST = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun11 Sec02:
   ^   - open files and initialize variables
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   hashHeapST = malloc(sizeof(struct searchST));

   if(! hashHeapST)
      goto memErr_fun11_sec05_sub02;

   init_searchST(hashHeapST);
     /*will allocate some memroy for read id array*/

   /*add memory to the id array*/
   hashHeapST->idAryUL = malloc(1024 * 5 * sizeof(ulong));

   if(! hashHeapST->idAryUL)
      goto memErr_fun11_sec05_sub02;
   
   hashHeapST->lenIdAryUL = 1024 * 5;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun11 Sec03:
   ^   - read in the read ids
   ^   o fun11 sec03 sub01:
   ^     - read in first part of file and start loop
   ^   o fun11 sec03 sub02:
   ^     - check if need to resize read id array
   ^   o fun11 sec03 sub03:
   ^     - check if need to read more file
   ^   o fun11 sec03 sub04:
   ^     - convert read id
   ^   o fun11 sec03 sub05:
   ^     - find end of read id line
   ^   o fun11 sec03 sub06:
   ^     - check if have a few bytes remaing to process
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun11 Sec03 Sub01:
   *   - read in first part of file and start loop
   \*****************************************************/

   newBytesUL =
      fread(
         (char *) buffStr,
         sizeof(uchar),
         lenBuffUS - 1,
         idFILE
      ); /*read in the file*/

   bytesUL = newBytesUL;
   buffStr[bytesUL] = '\0';
   hashHeapST->maxLimbsUC = 0;

   while(newBytesUL)
   { /*Loop: read in each read id*/

      /**************************************************\
      * Fun11 Sec03 Sub02:
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
               hashHeapST->lenIdAryUL * sizeof(ulong)
            );

         if(! tmpULPtr)
            goto memErr_fun11_sec05_sub02;

         hashHeapST->idAryUL = tmpULPtr;
      } /*If: I need to make an larger array*/
      
      /**************************************************\
      * Fun11 Sec03 Sub03:
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
               sizeof(schar),
               lenBuffUS - (1 + bytesUL),
               idFILE
            ); /*read in the file*/

         bytesUL += newBytesUL;
         ulId = 0;
         buffStr[bytesUL] = '\0';
      } /*If: I need to get more file*/

      /**************************************************\
      * Fun11 Sec03 Sub04:
      *   - convert read id
      \**************************************************/

      /*on the last few characters*/
      finishId_fun11_sec03_sub04:;

      oldPosUL = posUL;

      ulId +=
         cnvtIdToHexAry_searchST(
            &buffStr[ulId],
            hashHeapST->idAryUL,
            &posUL
         ); /*convert the read id*/

      ++ulId; /*get off white space*/

      /*make sure final limb count is always at end*/
      idLenUL = (uchar) hashHeapST->idAryUL[oldPosUL];
      hashHeapST->idAryUL[posUL] = idLenUL;

      /*add the previos read ids length in*/
      hashHeapST->idAryUL[oldPosUL] = lastIdLenUL;
      lastIdLenUL = idLenUL;

      hashHeapST->maxLimbsUC =
         max_genMath(
            hashHeapST->maxLimbsUC,
            (uchar) idLenUL
         ); /*find the most limbs used for a single id*/

      hashHeapST->numLimbsUL = posUL;
      ++hashHeapST->numIdsUL;

      /**************************************************\
      * Fun11 Sec03 Sub05:
      *   - find end of read id line
      \**************************************************/

      ulId += endLine_ulCp(&buffStr[ulId]);

      while(buffStr[ulId] != '\n')
      { /*Loop: till on the next id (next line)*/
         newBytesUL =
            fread(
               (char *) buffStr,
               sizeof(schar),
               lenBuffUS - 1,
               idFILE
            ); /*read in the file*/

         bytesUL = newBytesUL;
         ulId = 0;
         buffStr[bytesUL] = '\0';

         if(newBytesUL == 0)
            break;

         ulId += endLine_ulCp(&buffStr[ulId]);
      } /*Loop: till on the next id (next line)*/
   } /*Loop: read in each read id*/

   /*****************************************************\
   * Fun11 Sec03 Sub06:
   *   - check if have a few bytes remaing to process
   \*****************************************************/

   if(bytesUL - ulId == 1) /*false most of the time*/
      ++ulId;

   /*checked if I finished the last line*/
   if(ulId < bytesUL)
      goto finishId_fun11_sec03_sub04;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun11 Sec04:
   ^   - even limbs, sort arrays, and make hash tables
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*make sure all ids have same number of limbs*/
   evenLimbs_searchST(hashHeapST);

   if(hashBl)
      mkhash_searchST(hashHeapST); /*does sorting*/
   else
      sortIds_searchST(hashHeapST);

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun11 Sec05:
   ^   - clean up and return
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   return hashHeapST;

   /*deal with errors (memory only)*/

   memErr_fun11_sec05_sub02:;

   *errSC = def_memErr_searchST;
   freeHeap_searchST(hashHeapST);
   hashHeapST = 0;

   return 0;
} /*getReadIds_searchST*/

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
){
   signed long midSL = 0;
   signed long leftSL = 0;
   slong rightSL = 0;
   slong indexSL = 0;
   signed long eqlSL = 0;

   indexSL =
      getHash_searchST(
         searchSTPtr,
         qryIdAryUL
      ); /*find the hash value*/

   indexSL = (slong) searchSTPtr->hashTblUL[indexSL];

   if(indexSL < 0)
      return def_negULBit_searchST; /*not in hash table*/

   leftSL = 0;
   rightSL = (slong) searchSTPtr->idAryUL[indexSL];

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
){
   searchSTPtr = searchSTPtr;
} /*blank_searchST*/
   
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
){
   freeStack_searchST(searchSTPtr);
   free(searchSTPtr);
} /*freeHeap_searchST*/

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
){
   ulong ulPos = 0;     /*first limb to add char to*/
   uchar lenIdUC = 0;

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
