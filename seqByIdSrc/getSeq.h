/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' getSeq SOF: Start Of File
'   - holds functions used to process sequence files,
'     such as fastq, fasta, and sam files
'   o fun01: pId_getSeq
'     - prints or adds ouput to buffer
'   o fun02 ulfq_getSeq:
'     - gets next fastq entry and adds current to buffer
'   o fun03 ulsam_getSeq:
'     - gets next sam entry and adds current to out buffer
'   o fun04 ulfqNoBuff_getSeq:
'     - gets next fastq entry and prints to output file
'   o fun05 ulsamNoBuff_getSeq:
'     - gets next sam entry and prints out old sam entry
'       if file provided
'   o license:
'     - licensing for this code (public domain / mit)
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#ifndef GET_SEQUENCE_H
#define GET_SEQUENCE_H

/*-------------------------------------------------------\
| Fun01: pId_getSeq
|   - prints or adds ouput to buffer
| Input:
|   - buffStr:
|     o c-string with buffer to copy contents from
|   - posUL:
|     o end position of copy for buffStr
|   - startPosUL:
|     o start position to copy for buffStr
|   - outBuffStr:
|     o buffer to copy output to
|   - outPosUL:
|     o position to start copy to the output buffer
|   - lenBuffUI:
|     o length of input (buffStr) & output (outBuffStr)
|       buffers
|   - outFILE:
|     o file to print to if outBuffStr is full
| Output:
|   - Modifies:
|     o outPosUL to be at end of copy
|     o startPosUL to be at start of array
|     o outBuffStr to have new contents
|   - Prints:
|     o outBuffStr to outFILE if outBuffStr is full
\-------------------------------------------------------*/
void
pId_getSeq(
   signed char *buffStr,
   unsigned long posUL,
   unsigned long *startPosUL,
   signed char *outBuffStr,
   unsigned long *outPosUL, 
   unsigned int lenBuffUI,
   void *outFILE
);

/*-------------------------------------------------------\
| Fun02 ulfq_getSeq:
|   - gets next fastq entry and adds current to buffer
| Input:
|   - buffStr:
|     o c-string with fastq file buffer to find next entry
|       in or update
|   - posUL:
|     o unsigned long pointer to position at in buffer
|   - outBuffStr:
|     o c-string with sequences to print out
|   - outPosUL:
|     o unsigned long pointer to position at in output
|       buffer
|   - lenBuffUI:
|     o length of input (buffStr) & output (outBuffStr)
|       buffers
|   - bytesUL:
|     o pointer to a unsigned long with the number of
|       bytes in buffer (updated as needed)
|   - fqFILE:
|     o fastq FILE with reads to extract
|   - outFILE:
|     o FILE to print fastq entry to
| Output:
|  - Prints:
|    o Reads to outFILE if outFILE points to a file
|  - Modifies:
|    o buffStr to have the next buffer if empty
|    o posUL to have position of next sequence in buffer
|    o bytesUL to have new number of bytes if need to
|      read in more from fastq file
|    o outBuffStr to have current sequence if outFILE
|      is input
|    o outposUL to have new position at in output buffer
|    o fqFILE to be at next point in file if more
|      file read into buffer
|    o outFILE to have contents of outBuffStr if
|      outBuffStr is full
|  - Returns:
|   o 0 if nothing went wrong
|   o 1 If the end of the file
|   o 4 If ran out of file
\-------------------------------------------------------*/
unsigned char
ulfq_getSeq(
    signed char *buffStr,   /*buffer with input to scan*/
    unsigned long *posUL,   /*Position in buffer*/
    signed char *outBuffStr,/*holds reads to print*/
    unsigned long *outPosUL,/*position at in outBuffStr*/
    unsigned int lenBuffUI, /*Size of buffer*/
    unsigned long *bytesUL, /*Number chars in buffStr*/
    void *fqFILE,           /*Fastq file with input*/
    void *outFILE           /*file to output reads to*/
);

/*--------------------------------------------------------\
| Fun03 ulsam_getSeq:
|   - gets next sam entry and adds current to out buffer
| Input:
|   - buffStr:
|     o c-string with fastq file buffer to find next entry
|       in or update
|   - posUL:
|     o unsigned long pointer to position at in buffer
|   - outBuffStr:
|     o c-string with sequences to print out
|   - outPosUL:
|     o unsigned long pointer to position at in output
|       buffer
|   - lenBuffUI:
|     o length of input (buffStr) & output (outBuffStr)
|       buffers
|   - bytesUL:
|     o pointer to a unsigned long with the number of
|       bytes in buffer (updated as needed)
|   - fqFILE:
|     o fastq FILE with reads to extract
|   - outFILE:
|     o FILE to print fastq entry to
| Output:
|  - Prints:
|    o Reads to outFILE if outFILE points to a file
|  - Modifies:
|    o buffStr to have the next buffer if empty
|    o posUL to have position of next sequence in buffer
|    o bytesUL to have new number of bytes if need to
|      read in more from fastq file
|    o outBuffStr to have current sequence if outFILE
|      is input
|    o outposUL to have new position at in output buffer
|    o fqFILE to be at next point in file if more
|      file read into buffer
|    o outFILE to have contents of outBuffStr if
|      outBuffStr is full
|  - Returns:
|   o 0 if nothing went wrong
|   o 1 If the end of the file
|   o 4 If ran out of file
\--------------------------------------------------------*/
unsigned char
ulsam_getSeq(
    signed char *buffStr,   /*buffer with input to scan*/
    unsigned long *posUL,   /*Position in buffer*/
    signed char *outBuffStr,/*holds reads to print*/
    unsigned long *outPosUL,/*position at in outBuffStr*/
    unsigned int lenBuffUI, /*Size of buffer*/
    unsigned long *bytesUL, /*Number chars in buffStr*/
    void *samFILE,           /*Fastq file with input*/
    void *outFILE           /*file to output reads to*/
);

/*-------------------------------------------------------\
| Fun04 ulfqNoBuff_getSeq:
|   - gets next fastq entry and prints to output file
| Input:
|   - buffStr:
|     o c-string with fastq file buffer to find next entry
|       in or update
|   - posUL:
|     o unsigned long pointer to position at in buffer
|   - lenBuffUI:
|     o length of input (buffStr) & output (outBuffStr)
|       buffers
|   - bytesUL:
|     o pointer to a unsigned long with the number of
|       bytes in buffer (updated as needed)
|   - fqFILE:
|     o fastq FILE with reads to extract
|   - outFILE:
|     o FILE to print fastq entry to (null to not print)
| Output:
|  - Prints:
|    o Reads to outFILE if outFILE points to a file
|  - Modifies:
|    o buffStr to have the next buffer if empty
|    o posUL to have position of next sequence in buffer
|    o bytesUL to have new number of bytes if need to
|      read in more from fastq file
|    o fqFILE to be at next point in file if more
|      file read into buffer
|    o outFILE to have new sequence (if provided)
|  - Returns:
|   o 0 if nothing went wrong
|   o 1 If the end of the file
|   o 4 If ran out of file
\-------------------------------------------------------*/
unsigned char
ulfqNoBuff_getSeq(
    signed char *buffStr,   /*buffer with input to scan*/
    unsigned long *posUL,   /*Position in buffer*/
    unsigned int lenBuffUI, /*Size of buffer*/
    unsigned long *bytesUL, /*Number chars in buffStr*/
    void *fqFILE,           /*Fastq file with input*/
    void *outFILE           /*file to output reads to*/
);

/*--------------------------------------------------------\
| Fun05 ulsamNoBuff_getSeq:
|   - gets next sam entry and prints out old sam entry if
|     file provided
| Input:
|   - buffStr:
|     o c-string with fastq file buffer to find next entry
|       in or update
|   - posUL:
|     o unsigned long pointer to position at in buffer
|   - lenBuffUI:
|     o length of input (buffStr) & output (outBuffStr)
|       buffers
|   - bytesUL:
|     o pointer to a unsigned long with the number of
|       bytes in buffer (updated as needed)
|   - fqFILE:
|     o fastq FILE with reads to extract
|   - outFILE:
|     o FILE to print fastq entry to
| Output:
|  - Prints:
|    o Reads to outFILE if outFILE points to a file
|  - Modifies:
|    o buffStr to have the next buffer if empty
|    o posUL to have position of next sequence in buffer
|    o bytesUL to have new number of bytes if need to
|      read in more from fastq file
|    o fqFILE to be at next point in file if more
|      file read into buffer
|    o outFILE to have contents of outBuffStr if
|      outBuffStr is full
|  - Returns:
|   o 0 if nothing went wrong
|   o 1 If the end of the file
|   o 4 If ran out of file
\--------------------------------------------------------*/
unsigned char
ulsamNoBuff_getSeq(
    signed char *buffStr,   /*buffer with input to scan*/
    unsigned long *posUL,   /*Position in buffer*/
    unsigned int lenBuffUI, /*Size of buffer*/
    unsigned long *bytesUL, /*Number chars in buffStr*/
    void *samFILE,           /*Fastq file with input*/
    void *outFILE           /*file to output reads to*/
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
