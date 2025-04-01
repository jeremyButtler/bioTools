# Goal:

How to use mapRead's code in your own program. This is
  not a very great guide, but should give an idea.

**This is out of date and I need to update**

# Overview:

mapRead maps a read to a reference genome.

The mapRead algorithim consists of four steps, which are
  divided into four functions. In the frist step
  neighboring kmers are merged to build a chain of kmers
  (chain building). In the second step the chains that
  overlap or separated by gaps are merged (merging
  chains). In the third step gaps in the merged chains are
  aligned to complete the alignment (gap filling). In the
  fourth step, the most optimal sub-alignment and its
  score are found.

For each of the four steps I am assuming you already know
  the alignment direction. For more information on each
  step see the how works [how works](#how_works) section
  at the end.

These four steps and a scanning step are all grouped into
   the aln_mapRead function. The scanning step using the
   kmerScan method to divy the reference into query length
   chunks. 2.5 of these chunks are then merged into a
   window and the number of matching kmers in query and
   the window is found. If enough kmers pass, then the
   building chain step is called for the window. To avoid
   repeated alignments, on a hit I move a window twice
   more and recored best hit before the chain system
   is called.

There are up to six structures you have to be familar
   with to use mapRead. They are seqST, samEntry,
   refST_kmerFind, tblST_kmerFind, set_mapRead, and
   finally chains_mapRead (mapRead's structure).

If you are using the aln_mapRead function, then you only
  need to know the seqST, samEntry, and set_mapRead
  structs.

 
# quick integration overview:

These steps are for a quick intgration step. They focus
  on using the aln_mapRead and qckAln_mapRead functions.
  These functions merge the scan, chaining, chain
  merging, gap filling, and sub-alignment scoring steps.
  If you want to use each individual steps, then see
  slow integration.
 

# quick integration Structures:

There are three structures you need to be familiar with
  for the quick integration steps. They are seqST
  (holds sequence), samEntry (has returned sam entry),
  and set_mapRead (settings + alignment variables).

## seqST struct

The seqST sruct stores a read/sequence. Its variables
  are idStr (read/sequence id), seqStr (sequence),
  and qStr (q-score entry; qStr[0] == '\0' if none).

The read length is stored in the lenSeqUL variable.

### Steps in using a samEntry struct:

The use a seqST struct you must first initialize it with
  init_seqST (fun06 ../genBio/seqST.c).

After initialization, you can blank a seqST struct using
  blank_seqST (fun05 ../genBio/seqST.c). You need to blank
  before grabbing new sequences, but not after
  initialization.

You can read in a fasta sequence with getFaSeq_seqST
  (fun03 ../genBio/seqST.c) or a fastq sequence wtih
  getFqSeq_seqST (fun04 ../genBio/seqST.c). Both
  getFa and getFq take in a file pointer and a seqST
  struct. On success the file is moved to the next
  sequence and the sequence is stored in seqST. On failure
  an error value is returned.

| error value       | meaning      | returned          |
|:------------------|:-------------|:------------------|
| 0                 | no error     | success           |
| def_EOF_seqST     | end of file  | success or EOF    |
| def_fileErr_seqST | file error   | file error        |
| def_badLine_seqST | invalid line | file error        |
| def_memErr_seqST  | memory error | ran out of memory |

Table: errors form getFxSeq_seqST

The def_badLine_seqST error also includes the file error
  (actual return def_badLine_seqST | def_fileErr_seqST).


When finished free the seqST struct with freeStack_seqST
  (fun07 ../genBio/seqST.c) to free the variables in your
  seqST struct or freeHeap_seqST (fun08 ../genBio/seqST.c)
  to free your seqST struct.

### seqST example

```
#ifdef PLAN9 /*for portablility*/
   #include <u.h>
   #include <libc.h>
#else
  #include <stdlib.h>
#endif 

#indluce <stdio.h>

#include "../genBio/seqST.h"

int
main(
){
   signed char errorSC = 0;
   struct seqST seqStackST;
   FILE *faFILE = fopen(/path/to/file.fasta, "r");
   
   if(! faFILE)
      /*deal with file error*/
   
   errorSC =
      getFaSeq(
         faFILE,      /*fasta file*/
         &seqStackST  /*will get sequence*/
      );
   
   if(errorSC)
   { /*If: had error*/
   
      if(errorSC == def_EOF_seqST)
      { /*If: at end of file*/
   
         if(seqStackST.seqStr[0] == '\0')
            /*deal with end of file (no sequence)*/
   
         /*you will get EOF with last sequence*/
   
      } /*If: at end of file*/
   
      else
         /*deal with file/memory error*/
   } /*If: had error*/
   
   
   /*do something with you read in sequence*/
   
   /*in this case reverse complementing sequence*/
   revComp_seqST(&seqStackST);
   
   fprintf(
      stdout,
      "%s\n%s\n",
      seqStackST.idStr,
      seqStackST.seqStr
   );
   
   freeStack_seqST(&seqStackST);
   return 0;
}
```

## samEntry struct

The samEntry struct has a lot of information, so I am not
  going to got through the variables here. The breif
  summary is that it stores an entry in a sam file. This
  is what you will store the alignment for the mapped read
  in and also use to print out.

For comments, it will store the comment in the
  extraStr c-string in the samEntry struct. Otherwise,
  it stores everything else in its list of variables.

### Steps in using a samEntry struct:

1. declare `struct samEntry samStack;`
2. initialize `init_samEntry(&samStack);`
3. setup `if(setup_samEntry(&samStack)) handel error;`

After the setup step you can read lines from a sam file
  using get_samEntry (fun12 ../genBio/samEntry.c/h).

You will need to input your samEntry sturct, a buffer
  (memory is auto allocated), the current length of
  the buffer (auto resized), and the sam FILE.

You can print out the mapped read with p_samEntry. This
  takes in a samEntry struct, a buffer (signed char \*\*),
  the buffer length, a 1 or 0 to mark if want new line
  after, and the file to print the read to.

  - the buffer is modified and resized as needed
  - the buffer length is a unsigned long pointer, and is
    changed when the buffer is resized
  - 1: does not add a new line, use if you plan to add
       more extra entries
  - 0: adds new line at end; use if finished with entry

After you are finished, you will need to clean up you
  samEntry struct with freeStack_samEntry (fun04 
  ../genLib/samEntry.c/h) and also free your buffer with
  free().

### samEntry example

```
#ifdef PLAN9 /*for portablility*/
   #include <u.h>
   #include <libc.h>
#else
  #include <stdlib.h>
#endif 

#indluce <stdio.h>

#include "../genLib/samEntry.h"

int
main(
){
   signed char errorSC = 0;
   signed char *bufferHeapStr = 0;
   unsigned long bufferLengthUL = 0;
   FILE *samFILE = 0;
   struct samEntry samStack;
   
   
   samFILE = fopen("/path/to/out-file.sam", "w");
   if(! samFILE)
      /*HANDEL FILE ERRROR*/
   
   init_samEntry(&samStack);
   
   if( setup_samEntry(&samStack) )
      /*HANDEL MEMORY ERROR*/
   
   
   /*do alingment*/
   
   p_samEntry(
      &samStackST,
      &bufferHeapStr,
      &bufferLengthUL
      0, /*want new line; use 1 to not have newline*/
      samFILE
   );
   
   
   freeStack_samEntry(&samStack);
   free(buffHeapStr);
   fclose(samFILE);
   return 0;
}
```

## set_mapRead struct

The set_mapRead structure has the settings and buffers
   you will use to map reads. It is only used for the
   aln_mapRead and qckAln_mapRead functions.

### Steps in using a set_mapRead struct:

The set_mapRead struct has 

- lenArySI:
  - array of lengths, that controll which settings are
    used during the kmer scaning and chaining step
  - maximum limit of 16 (0-15) items
- kmerAryUC:
  - the kmer size to use with each length in lenArySI
  - maximum limit of 16 (0-15) items
- minKmersAryF:
  - minimum percentage of kmers in window needed to
    do alignment on a window
  - determined by closest length in lenArySI
  - maximum limit of 16 (0-15) items
- chainLenArySI:
  - minimum number of kmers a chain needs to keep a chain
  - determined by closest length in lenArySI
  - maximum limit of 16 (0-15) items
- percExtraNtF:
  - during a kmer scan, this is the number of extra bases
    to inclued in the window
  - query length + (query length * percExtraNtF)
- percShiftF:
  - percentage of query length to shift window by in
    kmer scan step
- numWinSC:
  - number of windows to scan before doing alignment
  - not used in qckAln_mapRead
  - window is always moved to window after numWinSC, so
    there is potential to miss neighboring duplications 
- alnSetST:
  - settings for the alignment and scoring steps
  - see alnSet struct in slow integration for more details
- subBl:
  - boolean (0 or 1) that tells if trying to find best
    sub-alignment in the final alignment.
  - 1: find best sub-alignment
  - 0: just find score for alignment
- minPercScoreF:
  - minimum percent score of final alignment to keep
    alignment
- minPercLenF:
  - minimum percent of query length final alignment must
    be (how many bases were kept)

The remaining (unmentioned) variables are used to pass
  input to the individual alignment steps, so you do not
  need to know these.

### using set_mapRead struct

The steps in usinga set_mapRead struct follows 

#### initilazation (set_mapRead)

To use a set_mapRead structure you must first initialize
  it, then setup the memory. Because alnSet is needed for
  the alignment settings, it is best if you do this before
  getting user input.

You can initialize a set_mapRead struct using
  init_set_mapRead (fun17 ../genAln/mapRead.c). It takes
  in a set_mapRead pointer and sets pointers to 0 (null).
  It also sets up the default values. These default values
  are in ../genAln/defsMapRead.h.

#### setup (set_mapRead)

After intialization, you can then setup (allocate memory)
   for a set_mapRead struct using setup_set_mapRead
   (fun20 ../genAln/mapRead.c). This takes in a
   set_mapRead pointer and returns 0 for success. For
   memory errors it returns def_memErr_mapRead.

The setup step will also initialize the alnSet (alnSetST)
  structure in the set_mapRead structure. Its defaults
  are located in the ../genAln/alnDefs.h file. The input
  is a set_mapRead structure pointer, the kmer size,
  and the maximum reference length.

For the kmer size and reference length, aln_mapRead and
  qckAln_mapRead will modify as needed. So, provided a
  reasnobale value.

#### freeing (set_mapRead)

After you are done with a set_mapRead structure it can
  be freeded using freeStack_set_mapRead
  (fun18 ../genAln/mapRead.c) for a set_mapRead structure
  on the stack. This will free all variables in the
  set_mapRead struct and then call init_set_mapRead
  function.

 For set_mapRead structures on the heap use
   freeHeap_set_mapRead (fun19 ../genAln/mapRead.c). This
   will call freeStack_set_mapRead and then free the input
   set_mapRead structure. You will have to set the input
   pointer to 0/null.

Both freeStack_set_mapRead and freHeap_set_mapRead only
  take a set_mapRead pointer.

### set_mapRead example

Here is an example of a set_mapRead workflow.

```
#ifdef PLAN9  /*plan9 has different libraries*/
   #include <u.h>
   #include <libc.h>
#else
   #include <stdlib.h>
#endif

#include <stdio.h>

#include "../genAln/mapRead.h"

int
main(
){
   struct set_mapRead settingsStackST;
   init_set_mapRead(&settingsStackST);

   if(
      setup_set_mapRead(
         &settingsStackST,
         7,     /*kmer length; aln_mapRead changes*/
         2000   /*reference length, aln_mapRead changed*/
      )
   ){ /*If; memory error*/
      /*deal with memory error*/
   }  /*If; memory error*/

   /*do something with structure here*/

   freeStack_set_mapRead(&settingsStackST);
   return 0;
}
```

# quick integration functions

This is the guide on how to use aln_mapRead and
  qckAln_mapRead functions.

## work before calling alignment functions

The first step is to read in your reference sequence and
  then convert the nucleotides to lookup index's with
  seqToIndex_alnSet (fun09 ../genAln/alnSet.c). You
  can convert the sequence back with indexToSeq_alnSet
  (fun10 ../genAln/alnSet.c). Both functions take in
  a c-string with your sequence.

To read in a sequence you should use the getFqSeq_seqST
  (fun03 ../genBio/seqST.c) and getFqSeq_seqST
  (fun02 ../genBio/seqST.c) functions. For an example
  see the seqST structure section.

## quick alignment function

After reading in the reference and query sequence you
  can then use the aln_mapRead
 (fun22 ../genAln/mapRead.c) or qckAln_mapRead 
 (fun23 ../genAln/mapRead.c) to do your alingment.

At the time of this writing, I am  focusing on getting
  qckAln_mapRead up and working, so aln_mapRead is not
  functional yet. Also, I am thinking of modifiyign the
  return type for aln_mapRead to an array of samEntry
  structures.

To use qckAln_mapRead, you povide the samEntry structure
  to hold the aligment, the query sequence (in a seqST
  structure), the reference sequence (in a seqST
  structure), the settings (set_mapRead structure), and
  a signed char variable to hold the error message. The
  score for the found alignment is returned. The error
  variable is set to def_memErr_mapRead for memory errors,
  def_noChains_mapRead if chaing step failed,
  and def_noAln_mapRead if no alignment could be found.

# quick integration example

```
#ifdef PLAN9
   #include <u.h>
   #include <libc.h>
#else
   #include <stdlib.h>
#endif

#include <stdio.h>

#include "../genBio/seqST.h"
#include "../genBio/samEntry.h"
#include "../genAln/mapRead.h"

int
main(
){
   signed char errorSC = 0;

   signed char *buffHeapStr = 0; /*for printing*/
   unsigned long lenBuffUL = 0;  /*for printing*/

   struct seqST refStackST;
   struct seqST queryStackST;
   struct samEntry samStackST;
   struct set_mapRead settingsStackST;

   FILE *refFILE = fopen("/path/to/reference.fasta", "r");
   FILE *queryFILE = fopen("/path/to/query.fastq", "r");

   if(! refFILE)
      /*handel no reference file errors*/
   if(! queryFILE)
      /*handel no query file errors*/


   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ initialization and setup*/
   \>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/

   init_seqST(&refStackST);
   init_seqST(&queryStackST);
   init_samEntry(&samStackST);
   init_set_mapRead(&settingsStackST);

   if( setup_samEntry(&samStackST) )
      /*handel memory error*/

   if(
       setup_set_mapRead(
          &settingsStackST,
          7,   /*kmer size (just put something)*/
          refStackST.lenSeqUL
       )
    ){ /*If: memory error*/
      /*handel memory error*/
    }  /*If: memory error*/


   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ read in sequences
   /<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

    errorSC =
       getFaSeq_seqST(
          refFILE,
          &refStackST
       );

    fclose(refFILE);
    refFILE = 0;
    if(errorSC)
       /*handle errors*/

    seqToIndex_alnSet(refStackST.seqStr);
    /*convert sequence to look up index's (needed)*/


    errorSC =
       getFqSeq_seqST(
          queryFILE,
          &queryStackST
       );

    flcose(queryFILE);
    queryFILE = 0;
    if(errorSC)
       /*handle errors*/

    seqToIndex_alnSet(qryStackST.seqStr);
    /*convert sequence to look up index's (needed)*/


   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ do alignment
   /<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

    qckAln_mapRead(
       &samStackST,
       &queryStackST,
       &refStackST,
       &settingsStackST,
       &errorSC
   );

   if(errorSC == def_memErr_mapRead)
      /*handle memory errors*/
   else if(! errorSC)
   { /*Else If: have alignment*/
      errorSC =
         p_samEntry(
            &samStackST,
            &buffHeapStr,
            &lenBuffUL,
            0, /*want newline at end*/
            stdout
         );

      if(errorSC)
        /*handel memory error*/
   } /*Else If: have alignment*/


   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ clean up and return
   /<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/
   
   freeStack_seqST(&queryStackST);
   freeStack_seqST(&refStackST);
   freeStack_samEntry(&samStackST);
   freeStack_set_mapRead(&settingsStackST);

   return 0;
}
```


# slow integration

This section is to cover how to use each individual step
  in aln_mapRead. You should be familar with the samEntry
  and seqST structures covered in the quick integration
  steps. You also will need to learn about the alnSet,
  tblST_kmerFind, refST_kmerFind, and the chains_mapRead
  structures.

I would recomend using the set_mapRead structure. It
  provides many of these structures and auto frees
  them. That being said, it is not needed.

# slow integraion structures

In addition to seqST and samEntry structures, you will
  also need to be familiar with the chains_mapRead
  (chainng building and merging), refST_kmerFind
  (for kmer scan), tblST_kmerFind (for kmer scan), and
  alnSet (alignment settings) structures.

## alnSet

### alnSet overview:

The alnSet is a structure that has the aligment settings
  for the scoring and alingment steps. This is one of
  the few structures that I would recomend using the
  default functions for.

It has a scoring matrix, the gap opening penalty, and
  the gap extension penalty. Additionaly, it includes
  a deletion and insertion matrix to speed up gap opening
  and gap extension calcuations. For mapReads, you do not
  need to mess with these, just their wrapper functions/
  macros.

If you plan to use this for more than mapRead, then
  remember to convert your sequence to index's with
  seqToIndex_alnSet(sequenceStr). You can deconvert
  with indexToSeq_alnSEt(sequenceStr). I cover this
  for map read in the function section.

### alnSet functions

The first function to call is init_alnSet
  (fun14 ../genAln/alnSet.c). This will initialize all
  values to defaults (see ../genAln/alnDefs.h). The
  defaults are for the eDNA matrix.

To change a score for two bases, use setScore_alnSet
  (fun01 ../genAln/alnSet.h). The input is the query
  base, reference base, the new score, and a pointer
  to the alnSet structure to modify. The bases can be
  in atgc format or index format
  (base & def_ntToCode_alnSet).

If you want to read in a set of scores from a file, then
  use readScoreFile_alnSet (fun07 ../genAln/alnSet.c).
  This takes in a alnSet structure pointer and a FILE
  pointer to the file to get socres from. Each line in the
  file must be `base1 base2 score` (ex: `A T -4`). Use
  negative numbers for penalties. The return value is
  0 for no errors, or line number (index 1) of error.

To change the gap penalties use changeGap_alnSet (fun12
  ../genAln/alnSet.c). The input is a pointer to the alSet
  structure to change, the new gap opening score, and the
  new (or old) gap extension score.

When finished you can free the variables in an alnSet
  structure using freeStack_alnSet (fun03
   ../genAln/alnSet.c). Currently alnSet has no heap
   allocation, so this just calls init_alnSet. Still,
   use this function to avoid future issues.

If you have a heap allocated alnSet struct you can use
  freeHeap_alnSet (fun04 ../genAln/alnSet.c). Remember to
  set your alnSet structure pointer to 0/null afterwards.

### alnSet example

```
#ifdef PLAN9
   #include <u.h>
   #include <libc.h>
#else
   #include <stdlib.h>
#endif

#include <stdio.h>

int
main(
){
   unsigned long errorLineUL = 0;
   struct alnSet alnStackST;
   FILE *scoreMatrixFILE = fopen("/path/to/file.txt","r");

   if(! scoreMatrixFILE)
      /*deal with file error*/

   init_alnSet(&alnStackST);

   errorLineUL =
      readScoreFile_alnSet(
         &alnStackST,
         scoreMatrixFILE
      ); /*get scoring matrix*/

   if(errorLineUL)
      /*deal with file error on line errorLineUL*/


   /*this is to show how to use, in this case this is the
   `  default setting
   */
   changeGap_alnSet(
      &alnStackST,
      -10, /*new gap opening score*/
      -1   /*new gap extension score*/
   ); /*add new gap scores*/


   /*this is to change a single score at a time*/
   setScore_alnSet(
      'A', /*query base*/
      'T', /*refernce base*/
      -4,  /*new score*/
      &alnStackST
   );


   /*use alnSet with mapReads*/
   freeStack_alnSet(&alnStackST);
   return 0;
} /*main*/

```

## chains_mapRead

## refST_kmerFind

## tblST_kmerFind


# How works

## 1. chain buildng step

In the building chains step kmers are merged into chains
  of similar kmers.

1. the first query kmer is selected
2. find all reference positions that have the same kmer as
   the reference kmer
   - for this step I have three arrays, the first is an
     - (array 1) reference kmer sequence
     - (array 2) sorted reference kmers (negatives at end)
     - (array 3) index's to converted sorted index
       (array 2) to sequence index (array 1)
3. extend query and reference kmer sequence until mismatch
4. keep chain if the chain (merged kmers) are longer
   then the min chain length
   - else if to short discard chain
5. move to the next query kmer
6. find positions of all matching reference kmers
7. remove query/reference hits that are in a previous
   chain
   - when both query and reference kmer index's are in a
     previous chain, I will just replicate a previous
     chain
8. repeat steps three to five until no more query kmers

Here is a more visual example. One difference in the
   visual is that I am merging all steps. In reallity
   each step is done one reference kmer at a time.

```ditta

+-----------------------------------------------------------+
| Legend                                                    |
| o qry is query                                            |
| o ref is reference                                        |
| o ... is rest of sequence; no atg, ccc, gcg, or taa kmers |
| o minimum chain length is 2 (really 20; but makes easier) |
| o kmer length is 3 (really 7 to 10; makes example eaiser) |
+-----------------------------------------------------------+

2. find matching kmers:

               1st   2nd   3rd   4th   nth
             +-----+-----+-----+-----+-----+
   qry kmer: | atg | ccc | gcg | ... | taa |
             +--+--+--+--+-----+-----+--+--+
                |     |                 |
                |     +-----+           |
                |     |     |           |
             +--+--+--+--+--+--+-----+--+--+
   ref kmer: | atg | ccc | ccc | ... | taa |
             +-----+-----+-----+-----+-----+
               1st   2nd   3rd   4th   nth

3. extend reference kmers (make chains)

                        +-----+-----+----------------+
   (1st qry, 1st ref):  | atg | ccc | gcg (mismatch) |
      length 2          +-----+-----+----------------+
   
                        +-----+----------------+
   (2nd qry, 2nd ref):  | ccc | gcg (mismatch) | length 1
     sub-chain of (1,1) +-----+----------------+

                        +-----+----------------+
   (2nd qry, 3rd ref):  | ccc | ... (mismatch) |
      length 1          +-----+----------------+


   (3rd qry, 3rd ref): no match (discarded)


                        +-----+-----+
   (4th qry, 4th ref):  | ... | taa |
      length n - 3      +-----+-----+

                        +-----+
   (nth qry, nth ref):  | taa | length 1
     sub-chain of (4,4) +-----+

7. remove sub-chains (chains in other chains)
                         
                           +-----+-----+----------------+
   1st qry, 1st ref kmer:  | atg | ccc | gcg (mismatch) |
     length 2              +-----+-----+----------------+
   
                           +-----+----------------+
   3rd qry, 3rd ref kmer:  | ccc | ... (mismatch) |
     length 1              +-----+----------------+

                           +-----+-----+
   4th qry, 4th ref kmer:  | ... | taa |
     length n - 3          +-----+-----+

5. remove chains beneath min length (3)

                           +-----+-----+
   4th qry, 4th ref kmer:  | ... | taa |
     length n - 3          +-----+-----+
```

This is a simplisitic example, in reallity, there will be
   more than one chain.

To reduce the number of chains, I use a large kmer space
   (7mer = 16000 kmers to 11mer = 4 million kemrs) to
   reduce the possible number of chains. It also reduces
   the computational intensity needed to build chains.

Kmer chain size should change based on shortest sequence
   length.

## 2. chain merging step

In the merging step chains are merged to form a single
  chain filled with gaps. The idea is to find the maximum
  number of nuclueotides a single alignment can hold.

Chains from step one are sorted by query position, then by
  reference position.

1. assign chain length as score for last chains (at end
   of query)
   - recored chain with most bases (index + score)
   - recored chain with second most bases (score only)
   - if find multiple best scores, record the last best
     score is kept (first reference hit)
     - logic here is that for duplications, this will
       prefere first event
2. move back one query kmer and score chains
   - if overlap with best scoring chain, subtract overlap
     to get total bases and add best score
     - if no overlap; see check for matches
     - if better than 2nd best score, count as final score
       - add best score
       - add index of best score chain
       - check if new best score
       - check if new second best score
     - else search for chain to merge with
       - add score of merged chain (highest score in scan)
       - add index of merged chain (highest score in scan)
       - check if new best score
       - check if new second best score
   - else if query is before best chain (no overlap)
     - add best score
     - add index of best score chain
     - check if new best score
     - check if new second best score
   - else can not merge, scan for best chain to merge with
     - add score of merged chain (highest score in scan)
     - add index of merged chain (highest score in scan)
     - check if new best score
     - check if new second best score
3. repeat step two until at end of chain
4. return index of best scoring chain (merged)

## 3. gap filling step

During the gap filling steps the gaps in the chain are
  aligned using Needleman and Waterman alingments.

1. find gap
2. align gap
   - if only reference has gap: deletion
   - else if only query has gap: insertion
   - else if reference and query have 1 gap: snp
   - else align with Needleman
3. align gaps at ends
   - grab (min chain length) + (0.5 * min chain length)
     bases for reference and query
   - align with Waterman
   - if no softmasking at merge end, then merge
     - starting gap: at end
     - ending gap: at start
   - else soft mask end
4. complete fill out samEntry struct variables

## 4. optimal sub-alignmetn step

In this step the alignment is scored and if wanted, the
   optimal sub-aligment is found. If the the optimal
   sub-alignment was wanted, then the sam entry cigar
   is modified to have the optimal sub-alignment.

1. score cigar; sub-score and total score
   - insertion: gap open + (length - 1) * extension
   - deletion: gap open + (length - 1) * extension
   - snp: snp penalty * length
   - match: match score * length
2. if subscore 0 or less
   - restet sub-alignment start
   - restet sub-alignment end
   - subscore becomes 0
3. else if subscore increases
   - save sub-alignment start
   - save sub-alignment end
   - save sub-alignment score as best score
4. repeat steps 1-3 until entire cigar scanned
5. soft mask all cigar entries not part of best
   sub-alignment (before start or after end)
   - deletions removed
   - insertions masked
   - snps masked
   - matches masked

