# Goal:

Describe how to use the accession library to parse genbank
  accession numbers.

Files: accession.c accession.h

No dependencies

# Overview

There is currently only one function. It parses Genbank
  accessions numbers into the refseq or special tag,
  the accession, and the version. For assembly accessions
  it also extracts the contig number and assembly version.

# parse genbank accessions

The funciton name is `genbankAccExtract_accession()` is
  used to break genbank functions into their individual
  components (parse).

- Input:
  1. accession to parse (c-string)
  2. c-string to get the accession part of the id
  3. c-string to get the refseq or genome tag
     - needs to be four bytes
     - set to 0/null if no refseq tag
  4. signed int pointer to get the accession version
     number
     - set to -1 if no version number
  5. signed int pionter to get the assembly version number
     - set to -1 if no version number
  6. signed int pionter to get the number assigned to the
     contig
     - set to -1 if not an assembly accession number
- Output:
  - Modifies input 2 to 6 to have the required input
  - Returns:
    - length of the accession part (stored in input 2)
    - 0 if input 1 is an empty string
    - < 0 (-1) if no a genbank accession

## genbankAccExtract working example:

Output from the working example:

```
WYAK01000010.1:
  accession: WYAK
  sequence version: 1
  assembly version: 1
  contig number: 10
NZ_CASIGT010000077.1:
  accession: CASIGT
  refseq database: NZ
  sequence version: 1
  assembly version: 1
  contig number: 77
GCF_028878055.3:
  accession: 028878055
  refseq database: GCF
  sequence version: 3
MK526900:
  accession: MK526900
```

The working example:

```
#ifdef PLAN9
   #include <u.h>
   #include <libc.h>
#endif

#include <stdio.h>
#include "accession.h"

void
main(
   void
){
   signed char *inStr[] =
   {
      (signed char *) "WYAK01000010.1",
      (signed char *) "NZ_CASIGT010000077.1",
      (signed char *) "GCF_028878055.3",
      (signed char *) "MK526900"
   };

   signed char accStr[32];
   signed char refseqTagStr[4];

   signed int accVersionSI = 0;
   signed int asmVersionSI = 0;
   signed int contigNumSI = 0;
   signed int lenSI = 0;
   signed int siIndex = 0;

   for(siIndex = 0; siIndex < 4; ++siIndex)
   { /*Loop: print accession numbers and there parts*/
      lenSI =
         genbankAccExtract_accession(
            inStr[siIndex],
            accStr,
            refseqTagStr,
            &accVersionSI,
            &asmVersionSI,
            &contigNumSI
         );

      if(lenSI > 0)
      { /*If: ncbi accession*/
         printf("%s:\n", inStr[siIndex]);
         printf("  accession: %s\n", accStr);

         if(refseqTagStr[0])
            printf(
               "  refseq database: %s\n",
               refseqTagStr
            );
         if(accVersionSI >= 0)
            printf(
               "  sequence version: %i\n",
               accVersionSI
            );
         if(asmVersionSI >= 0)
            printf(
               "  assembly version: %i\n",
               asmVersionSI
            );
         if(contigNumSI >= 0)
            printf("  contig number: %i\n", contigNumSI);
      } /*If: ncbi accession*/
   } /*Loop: print accession numbers and there parts*/
} /*main*/
```
