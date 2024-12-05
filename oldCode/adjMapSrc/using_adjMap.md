# Goal:

Go through how to use adjMap in your own code.

# Note:

For softmasking at the ends adjMap only maps regions of
  the same (minmum) length. So, the answers may not always
  be the best.

Also, with soft masking this may not be faster than
  minimap2. However, when insertion/insertion changes are
  rare, and soft masked bases are removed, this goes
  fast. I need to work on my algorithim a bit.

# adjMap:

Adjusts the alignments on a mapped read (sam file) to a
  new reference (mapped to same reference as read).

The input for adjMap are two samEntry structures
  (genLib/samEntry.h st01), two floats, and an alnSet
  structure (genAln/alnSet.h st01). See supporting
  structures section for a description of the needed
  structures.

The input samEntry structure with the query is modified to
  be aligned to the input reference samEntry structure.
  Both the query and reference must be aligned to the same
  reference.

The first float controls how well the softmasked ends must
  align. If the score is to low, then the ends are treated
  as soft masked.

The second float controls the minum percentag of overlap
  needed to try adjusting the alignment.

The alnSet structure has the alignment settings for the
  needleman (isertion regions) and waterman (softmasking)
  pairwise aligners.

The return value is 0 for no errors, def_noOverlap_adjMap
  if there is no shared sequence between the query and
  reference, and def_memErr_adjMap for memory errors.

## Example code for using adjMap:

```
#include <stdio.h>

#include "../genLib/samEntry.h"
#include "../genAln/alnSet.h"
#include "adjMap.h"

/*these files are hidden dependencies that will be needed
  in your make command
#include "../genLib/base10str.h"
#include "../genLib/numToStr.h"
#include "../genLib/ulCp.h"
#include "../genLib/seqST.h"

#include "../genAln/indexToCoord.h"
#include "../genAln/dirMatrix.h"
#include "../genAln/needle.h"
#include "../genAln/water.h"

These are dependencies that do not have/use a .c file

#include "../genLib/dataTypeShortHand.h"
#include "../genLib/genMath.h"
#include "../genLib/ntTo5Bit.h"
#include "../genAln/alnDefs.h"
*/

int
main(){
   /*****************************************************\
   * variable declarations
   \*****************************************************/

   signed char errSC = 0;
   signed char *buffStr = 0;
   unsigned long lenBuffUL = 0;
   
   FILE *refFILE = fopen("ref.sam", "r");
   FILE *readFILE = fopen("ref.sam", "r");
   FILE *outFILE = fopen("out.fastq", "r");

   struct samEntry refST;
   struct samEntry readST;
   struct alnSet settings;
   
   /*****************************************************\
   * initialize and set up
   \*****************************************************/

   init_samEntry(&refST);
   init_samEntry(&readST);
   init_alnSet(&settings);

   if(refFILE == 0) {/*handle file error*/}
   if(readFILE == 0) {/*handle file error*/}
   if(outFILE == 0) {/*handle file error*/}
   
   errSC = setup_samEntry(&refST);
   if(errSC) {/*handle memory error*/}

   errSC = setup_samEntry(&readST);
   if(errSC) {/*handle memory error*/}

   /*****************************************************\
   * get reference sequence
   \*****************************************************/

   do{
      errSC =
         get_samEntry(&refST,&buffStr,&lenBuffUL,refFILE);

      if(refST.extraStr[0] != '@')
         break; /*found reference*/
   } while(errSC != 0);

   if(errSC == def_memErr_samEntry)
   {/*handle memory error*/}

   /*****************************************************\
   * convert read mappings to new reference
   \*****************************************************/

   errSC =
      get_samEntry(&readST,&buffStr,&lenBuffUL,readFILE);
   
   while(errSC != 0)
   { /*Loop: change reference*/
     if(readST.extraStr[0] == '@')
     {/*do something about comment entry*/}
   
     if(readST.flagUS & 4)
     {/*do something about umapped read*/}
   
     /*adjust the mapping (alingment)*/
     errSC = adjMap(&readST, &refST, 0.9, 0.5, &settings);
        /*0.9 is min % score; 0.5 is min % overlap*/

     if(errSC == def_memErr_adjMap)
     {/*handle memory error*/}

     if(errSC)
     {/*handle reads could not be mapped*/}

     /*print new alingment*/
     errSC =
        p_samEntry(&readST,&buffStr,&lenBuffUL,0,outFILE);
   
     if(errSC == def_memErr_samEntry)
     {/*handle memory error*/}

     /*get the next line*/
     errSC =
        get_samEntry(&readST,&buffStr,&lenBuffUL,samFILE);
   } /*Loop: change reference*/

   if(errSC == def_memErr_samEntry)
   {/*handle memory error*/}

   /*****************************************************\
   * clean up
   \*****************************************************/
   
   if(errSC) {/*handle memory error*/}
   
   free(buffStr);
   freeStack_samEntry(&readST);
   freeStack_samEntry(&refST);
   freeStack_alnSet(&settings);
   fclose(samFILE);
   fclose(outFILE);

   return 0;
```

##

# Supporing structures

## samEntry struct

Holds one read or comment from a sam file.

### samEntry introduction

The samEntry structure holds a single entry from a sam
  file. Before setting up the samEntry struct you need to
  do two steps, initialization and setup. During the
  initialization step you are settings all pointers to
  zero. During the setup step you are allocating memory.

### intializing and setting up a samEntry struct

To initalize use init_samEntry (../genLib/samEntry.c/h
  fun02). The input is a pointer to a samEntry structure
  to initialze. There is no output (no error is possible).

To setup a samEntry struct use setup_samEntry
  (../genLib/samEntry.c/h fun03). The input is a pointer
  to a samEntry structure. The reurn value is
  def_memErr_samEntry (../genLib/samEntry.h) for memory
  errors and 0 for success.

You can skip these steps and use mk_samEntry
  (../genLib/samEntry.c fun06) to make a heap allocated
  samEntry structure. There is no input. The return value
  is a pointer to the new samEntry structure or zero for
  a memory error.

### reading a sam file

After initialization and setup you can read in entries
  from a sam file into a samEntry struct using
  get_samEntry (../genLib/samEntry.c/h fun10).

This function takes in a samEntry struct pointer (holds
  entry), a buffer (signed char **), the length of the
  buffer (unsigned long *), and the FILE.

The return values are 0 for no error,
  def_EOF_samEntry (../genLib/samEntry.h) if hit end of
  file, and def_memErr_samEntry if had a memory error.
  Comment entries are stored in samEntry->extraStr. You
  can check if an entry is a comment
  with `if(*samEntry->extrStr == '@')`.

### samEntry printing

#### printing introcution
You can print a samEntry structure using any of the
  print functions (p_samEntry [fun12], pfq_samEntr[fun13],
  pfa_samEntry [fun14], and pstats_samEntry [fun15]).

#### p_samEntry

The p_samEntry prints out the sam file entry to a sam
  file. It takes in the samEntry structure to print,
  a buffer (signed char **), the length of the buffer
  (unsigned long *), a bolean (signed char), and the file
  to print to (FILE *).

If the bolean is set to 1 (instead of 0), then no new
  line is printed. Only do this if you plan to add extra
  entries.

The return value for p_samEntry is 0 for no errors and
  def_memErr_samEntry for memory errors.

#### pfq_samEntry and pfa_samEntry

The pfq_samEntry (fastq) and pfa_samEntry (fasta) print
  out the sam entry as a fastq or fasta file (alignments
  not saved). They both take in a samEntry structure and
  file to print to. Their is no return avlue.

#### pstats_samEntry

The pstats_samEntry function prints out a report for the
  sam entry (number matches, number snps, number ins,
  number dels, lengths, and mean/median qualty scores).

The input is the samEntry structure to print, a pointer to
  a bolean (signed char *) to print the header (1 yes,
  0 no), a bolean to count anonymous bases (1 yes, 0 no),
  and the file to print the line to.

There is no return value, however, the header bolean is
  set to 0 if it is 1.
  
### samEntry; freeing memory

You can free the memory in a samEntry struct using
  freeStack_samEntry (../genLib/samEntry.c/h fun04) for
  stack allocated samEntry structures or for freeing
  internal memory. For heap allocated samEntry structs
  (mk_samEntry) use freeHeap_samEntry
  (../genLib/samEntry.c/h fun04).

Both freeStack_samEntry and freeHeap_samEntry take in a
  pointer and have no return value.

Be aware freeHeap_samEntry does not set the pointer to
  zero. You have to do that yourself.

### example code of using a samEntry structure

```
#include <stdio.h>
#include "../genLib/samEntry.h"
   /*look at ../genLib/samEntry.c for other needed files*/

/*these files are hidden dependencies that will be needed
  in  your make command
#include "../genLib/base10str.h"
#include "../genLib/numToStr.h"
#include "../genLib/ulCp.h"

These are dependencies that do not have/use a .c file

#include "../genLib/dataTypeShortHand.h"
#include "../genLib/ntTo5Bit.h"
*/

int
main(){
   signed char errSC = 0;
   signed char *buffStr = 0;
   unsigned long lenBuffUL = 0;
   
   FILE *samFILE = fopen("in.sam", "r");
   FILE *outFILE = fopen("out.fastq", "r");
   struct samEntry readST;
   
   init_samEntry(&readST);

   if(samFILE == 0) {/*handle file error*/}
   if(outFILE == 0) {/*handle file error*/}
   
   errSC = setup_samEntry(&readST);
   if(errSC) {/*handle memory error*/}
   
   /*read first line from sam file*/
   errSC =
      get_samEntry(&readST,&buffStr,&lenBuffUL,samFILE);
   
   while(errSC != 0)
   { /*Loop: convert sam file to fastq file*/
     if(readST.extraStr[0] == '@')
     {/*do something about comment entry*/}
   
     if(readST.flagUS & 4)
     {/*do something about umapped read*/}
   
     pfq_samEntry(&readST, outFILE); /*readST to fastq*/
   
     /*get the next line*/
     errSC =
        get_samEntry(&readST,&buffStr,&lenBuffUL,samFILE);
   } /*Loop: convert sam file to fastq file*/
   
   if(errSC) {/*handle memory error*/}
   
   free(buffStr);
   freeStack_samEntry(&readST);
   fclose(samFILE);
   fclose(outFILE);

   return 0;
}
```


## alnSet struct

Holds needleman or waterman alignment settings

### alnSet introduction/initialization

The alnSet structure (../genAln/alnSet.h st01) has the
  settings and score matrix for doing traditional
  pairwise alignments. Before use this structure needs
  to be initialized with init_alnSet (../genAln/alnSet.c/h
  fun13).

### alnSet scoring matrix

You can change the scoring matrix with individual scores
  using setScore_alnSet (../genAln/alnSet.h fun02) or 
  with a file readScoreFile_alnSet (../genAln/alnSet.c/h
  fun07). The input should be in non-index format (so 'A'
  or 'a' for 'A').

To change the gap penalties use changeGap_alnSet
  (../genAln/alnSet.c/h fun11). It takes in the alnSet
  structure to modify, the new gap score, and the new
  gap extension score.

To get a score you first need the sequence to be in index
  format ('A' goes to index). You can do this with
  `base & def_ntToCode_alnSet` (../genAln/alnSet.h) for
  individual bases or with seqToIndex_alnSet 
  (../genAln/alnSet.c/h fun09) for sequences. You can
  convert index's back to sequences using
  indexToSeq_alnSet (../genAln/alnSet.c/h fun10).

Both seqToIndex_alnSet and indexToSeq_alnSet only take
  in a c-string to convert. There is no return value.

You can get a score using getScore_alnSet
  (../genAln/alnSet.h fun05). Input is the query base,
  the reference base, and the alnSet struct.

### alnSet freeing

You can free an alnSet structure using freeStack_alnSet
  (../genAln/alnSet.c/h fun03) for stack allocated alnSet
  structs. For heap allocation use freeHeap_alnSet
  (../genAln/alnSet.c/h fun04).

Be aware freeHeap_alnSet does not set the pointer to
  zero. You have to do that yourself.

### example code of using an alnSet structure


```
#include "../genAln/alnSet.h"
    /*see ../genAln/alnSet.c for needed .c file list*/

/*This is a hidden dependency needed by your make command
#include "../genAln/base10str.h"

These are dependencies that do not have/use a .c file

#include "../genLib/dataTypeShortHand.h"
#include "../genAln/alnDefs.h"
*/

int
main(){
  signed char *referenceSeqStr = (signed char *) "atgatt";
  signed char *querySeqStr = (signed char *) "aggatt';
  struct alnSet settings;

  init_alnSet(&settings);
  
  seqToIndex(referenceSeqStr);
  seqToIndex(querySeqStr);
  
  /*call functions using alnSet*/
  
  freeStack_alnSet(&settings);
  return 0;
}
```
