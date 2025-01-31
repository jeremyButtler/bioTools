/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' idSearch SOF: Start Of File
'   - holds functions for searching throug a file by id
'   o header:
'     - forward declerations and guards
'   o fun01: getFqSeq_idSearch
'     - extract fastq sequences from file by read ids
'   o fun02: getSamSeq_idSearch
'     - extract sam sequences from sam file by read ids
'   o license:
'     - licensing for this code (public domain / mit)
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*-------------------------------------------------------\
| Header:
|   - forward declerations and guards
\-------------------------------------------------------*/

#ifndef ID_SEARCH_H
#define ID_SEARCH_H

struct searchST;

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
|   - seqByIdHeaderStr:
|     o c-string with sam file header to print for seqById
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
   signed char *seqByIdHeaderStr,
   void *samFILE,
   void *outFILE
);

#endif

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
