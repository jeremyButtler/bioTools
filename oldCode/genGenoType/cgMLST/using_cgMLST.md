# Goal:

Go though how to use cgMLST. Not very great, but hope
  is better than nothing.

# Overview

In cgMLST you have two structs you will be working with.
  The allele_cgMLST struct, which as the cgMLST database
  and the tables. And the samEntry struct, which has
  your mapped reads/consensus

One note to be aware of is cgMLST currentlty does not
  align matching insertions. Also, at some points
  the edit distances may be a bit off. This isq due to
  trying to compare a mapped read to a mapped
  reference. Sometimes, there will be subtle differences,
  like the read mapper choosing a deletion over an SNP.
  These minor differences that can throw off the edit
  distances.

I guess, take this as a way to get a close answer, but
  not as a final answer.

# samEntry struct

## samEntry overview

The samEntry struct has a lot of information, so I am not
  going to got through the variables here. The breif
  summary is that it stores an entry in a sam file.

For comments, it will store the comment in the
  extraStr c-string in the samEntry struct. Otherwise,
  it stores everything else in its list of variables.

## Steps in using a samEntry struct:

1. declare `struct samEntry samStack;`
2. initialize `init_samEntry(&samStack);`
3. setup `if(setup_samEntry(&samStack)) handel error;`

After the setup step you can read lines from a sam file
  using get_samEntry (fun12 ../genBio/samEntry.c/h).

You will need to input your samEntry sturct, a buffer
  (memory is auto allocated), the current length of
  the buffer (auto resized), and the sam FILE.

The return value for get_samEntry is either 0 for no
  errors, def_EOF_samEntry for end of file, or
  def_memErr_samEntry for memory errors. The sam file
  line will be stored in the samEntry structure.

After you are finished, you will need to clean up you
  samEntry struct with freeStack_samEntry (fun04 
  ../genLib/samEntry.c/h) and also free your buffer with
  free().

## samEntry example

```
#include <stdlib.h>
#include <stdio.h>
#include "../genLib/samEntry.h"

signed char errorSC = 0;
signed char *bufferHeapStr = 0;
unsigned long bufferLengthUL = 0;
FILE *samFILE = 0;
struct samEntry samStack;

samFILE = fopen("/path/to/file.sam", "r");
if(! samFILE)
   /*HANDEL FILE ERRROR*/

init_samEntry(&samStack);

if( setup_samEntry(&samStack) )
   /*HANDEL MEMORY ERROR*/

do{
{ /*Loop: read in rest of sam file*/
   errorSC =
      get_samEntry(
        &samStack,  /*samEntry structure to hold read*/
        &bufferHeapStr,/*buffer to get file line with*/
        &bufferLengthUL,
        samFILE
      ); /*get next line of sam file*/

   if(errorSC)
      break;

   if(samStack.extraStr[0] == '@')
      continue; /*comment in sam file, not read*/

   /*do something with samStack*/
} while(! errorSC) /*Loop: read in rest of sam file*/

freeStack_samEntry(&samStack);
free(buffHeapStr);
fclose(samFILE);
```

# allele_cgMLST

## overview allele_cgMLST

The allele_cgMLST struct stores your mapped cgMLST
  database and the tables that hold the number of hits
  to each cgMLST allele.

## Variables in allele_cgMLST:

- cntAryUI has the number of hits for each allele
- offAryUI has the number of hits that did not mapp to
  any allele (count is for closest allele)
- numAlleleSL is the number of alleles in your struct
- lociIndexArySI is the index of the frist allele for
  each loci
- lociStartAryUI is the lowest refernce starting position
  for all alleles in this loci
- lociIdsST is a str_ptrAry (see using this code in
   ../genLib) with the read loci ids (names)
- numLociSL is the number of stored loci
- mapSamST is an array of samEntry structs that have
  each mapped allele (for all loci)

## Using allele_cgMLST:

The best way is to call getLoci_allele_cgMLST (fun08
  cgMLST.c/h). This will return a heap allocated
  allele_cgMLST struct that has all alleles in a file.

The input for this is a sam file with mapped loci and a
  signed character to hold any errors.

The return values is the setup allele_cgMLST array or 0
  if there was an error. For errors, the input signed char
  is set to def_memErr_cgMLST for memory errors and
  def_fileErr_cgMLST for file errors; no reads/bad format.
 
When finished with the allele_cgMLST struct you can free
  it with freeHeap_allele_cgMLST() (fun04 cgMLST.c/h).

## Example allele_cgMLST

```
#include <stdlib.h>
#include <stdio.h>
#include "cgMLST.h"

signed char errorSC = 0;
struct allele_cgMLST *cgMLST = 0;
FILE *lociFILE = 0;

lociFILE = fopen("/path/to/mapped-loci.sam", "r");

if(! lociFILE)
   /*HANDEL FILE ERROR*/

cgMLST =
   getLoci_allele_cgMLST(
      lociFILE,
      &errorSC
   );

if(! cgMLST)
{ /*If: had error*/
   if(errorSC == def_memErr_cgMLST)
      /*HANDEL MEMORY ERROR*/
   else
     /*HANDLE FILE ERROR*/
} /*If: had error*/



/*YOU CODE GOES HERE*/


/*clean up*/
freeHeap_allele_cgMLST(cgMLST);
fclose(lociFILE);
```

# Functions:

There are three functions of note, beyond the setup and
  free functions mentioned in the allele_cgMLST structure
  overview.

The are seqToLoci_allele_cgMLST (fun10 cgMLST.c/h),
  plin_allele_cgMLST (fun11 cgMLST.c/h), and
  blank_allele_cgMLST (fun01 cgMLST.c/h).

seqToLoci_allele_cgMLST scans a sequence for all loci and
  updates the read count tables (cntAryUI and offAryUI).

plin_allele_cgMLST will print the most supported allele
  for each loci in the cntAryUI and offAryUI tables.

blank_allele_cgMLST will reset the cntAryUI and offAryUI
  tables to 0 (no reads).

## seqToLoci_allele_cgMLST function

Builds the loci allele count table for the input read.
  Warining, this does not blank the table, so adding
  multipe reads will increase allele counts.

Input:

- the allele_cgMLST structure returned from
  getLoci_allele_cgMLST
- a samEntry struct with a read to find loci in
- the maximum edit distance allowed for a loci to be a
  match (not assigned to offAryUI)
- the minimum indel size to count an idnel in the edit
  distance (reduces read errors)
- the min quality score to count a SNP
- what percentage of the allele should be covered by the
  read to count as mapped
  - Note: this is only for the aligned parts, softmasked
    parts of allele will be ignored

It will return 0 for no errors and def_noLoci_cgMLST if
  no compelete loci were found in the read.

It also adds the detected loci (alleles) to the cntAryUI
  table for good mappigns and offAryUI for unitified
  alleles.

## plin_allele_cgMLST function

Prints out lineage for each loci with the most support.

Inptut:

- allele_cgMLST stucture modified by
  seqToLoci_allele_cgMLST
- a c-string with the id to print for the entry, if
  `0`, then `NA` is printed
- minimum read depth for loci to print allele, otherwise
  is NA
- minimum percent support for allele to treat as a
  confident result
  - (allele support) / (total loci mapped reads)
  - if to low, prints a `*` first
- 1 or 0 to tell if printign the header
  - not 0, ie 1 prints header
  - 0 does not print header
- the file to print the results to

Output is the most supported alleled for each loci to
  the output file you provided.

Possible output lines

| output   | Meaning                                  |
|:---------|:-----------------------------------------|
| number   | detected allele (had support)            |
| \*number | allele had a low percent of support      |
| M        | enough support, but no allele id         |
| UX       | allele is unique and closest to allele X |
| \*M      | low support and no allele id             |
| \*UX     | low support for unique allele            |
| NA       | loci had to low read depth               |

Table: list of possible output values for each loci that
  plin_allele_cgMLST can output.

## blank_allele_cgMLST function

Blanks (sets all values to 0) the tables (cntAryUI and
  offAryUI) in an allele_cgMLST struct.

The input is an allele_cgMLST struct to blank. There is
  no return.


## example code

Here is an example showing how this system might work. It
  is not ment to be function (there is no main), but to
  give you an idea of the flow.

```
#include <stdlib.h>
#include <stdio.h>
#include "../genLib/samEntry.h"
#include "cgMLST.h"

signed char errorSC = 0;
signed char *bufferHeapStr = 0;
unsigned long bufferLengthUL = 0;
signed long readNumSL = 0;
FILE *samFILE = 0;

struct samEntry samStack;
struct allele_cgMLST *cgMLST = 0;

/*intializie values*/
init_samEntry(&samStack);
if( setup_samEntry(&samStack) )
   /*HANDEL MEMORY ERROR*/


/*get cgMLST database*/
samFILE = fopen("/path/to/mapped-loci.sam", "r");

if(! samFILE)
   /*HANDEL FILE ERRROR*/

cgMLST =
   getLoci_allele_cgMLST(
      samFILE,
      &errorSC
   );

if(! cgMLST)
{ /*If: had error*/
   if(errorSC == def_memErr_cgMLST)
      /*HANDEL MEMORY ERROR*/
   else
     /*HANDLE FILE ERROR*/
} /*If: had error*/


fclose(samFILE);
samFILE = 0;


/*get sam entries*/
samFILE = fopen("/path/to/file.sam", "r");


/*get mapped loci and alleles from reads*/
do{
{ /*Loop: read in rest of sam file*/
   errorSC =
      get_samEntry(
        &samStack,  /*samEntry structure to hold read*/
        &bufferHeapStr,/*buffer to get file line with*/
        &bufferLengthUL,
        samFILE
      ); /*get next line in sam file*/


   if(errorSC == def_memErr_samEntry)
      /*HANDEL MEMORY ERRORS*/

   else if(errorSC == def_EOF_samEntry)
      break; /*finished (end of sam file)*/

 
   if(samStack.extraStr[0] == '@')
      continue; /*comment in sam file, not read*/


   errorSC =
      seqToLoci_allele_cgMLST(
         cgMLST,
         &samStack,
         3,        /*maximum of three differences*/
         5,        /*indels must be 5 bases long*/
         0,        /*ingore SNP q-scores*/
         0.95      /*at least 95% of allele is covered*/
      ); /*add any alleles in read to tables*/

   if(! errorSC)
      ++readNumSL; /*number reads with loci*/
} while(! errorSC) /*Loop: read in rest of sam file*/


if(! readNumSL)
   /*HANDEL NO READS WITH LOCI ERROR*/

plin_allele_cgMLST(
   cgMLST,
   (signed char *) "id",/*up to you, just no spaces/tabs*/
   10,                /*at least 10 reads mapped to loci*/
   0.4,         /*at least 40% of reads supported allele*/
   1,           /*print header*/
   stdout       /*output to terminal*/
); /*print detected lineage*/


/*clean up*/
freeHeap_allele_cgMLST(cgMLST);
freeStack_samEntry(&samStack);
free(buffHeapStr);
fclose(samFILE);
```
