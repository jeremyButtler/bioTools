/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' getSeq SOF: Start Of File
'   - holds functions used to process sequence files,
'     such as fastq, fasta, and sam files
'   o .c fun01: pId_getSeq
'     - prints or adds ouput to buffer
'   o fun02 ulfq_getSeq:
'     - gets next fastq entry and prints out old fastq
'       entry if file provided (using longs)
'   o fun03 ulsam_getSeq:
'     - gets next sam entry and prints out old sam entry
'       if file provided using bytes
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#ifndef GET_SEQUENCE_H
#define GET_SEQUENCE_H

/*--------------------------------------------------------\
| Fun02 ulfq_getSeq:
|   - gets next fastq entry and prints out old fastq
|     entry if file provided using bytes
| Input:
|   - buffStr:
|     o c-string with fastq file buffer to find next entry
|       in or update
|   - lenBuffUI:
|     o length of buffStr
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
|    o incurments pointInBufferCStr to start of next read
|  - Returns:
|   o 0 if nothing went wrong
|   o 1 If the end of the file
|   o 4 If ran out of file
\--------------------------------------------------------*/
unsigned char
ulfq_getSeq(
    signed char *buffStr,   /*buffer with input to scan*/
    unsigned long *posUL,   /*Position in buffer*/
    signed char *outBuffStr,/*holds reads to print*/
    unsigned long *outPosUL,/*position at in outBuffStr*/
    unsigned int lenBuffUI, /*Size of buffer*/
    unsigned long *bytesUL, /*Number chars in buffStr*/
    FILE *fqFILE,           /*Fastq file with input*/
    FILE *outFILE           /*file to output reads to*/
);

/*--------------------------------------------------------\
| Fun03 ulsam_getSeq:
|   - gets next sam entry and prints out old sam entry if
|     file provided using bytes
| Input:
|   - buffStr:
|     o c-string with fastq file buffer to find next entry
|       in or update
|   - lenBuffUI:
|     o length of buffStr
|   - bytesUL:
|     o pointer to a unsigned long with the number of
|       bytes in buffer (updated as needed)
|   - samFILE:
|     o sam FILE with reads to extract
|   - outFILE:
|     o FILE to print fastq entry to
| Output:
|  - Prints:
|    o Reads to outFILE if outFILE points to a file
|  - Modifies:
|    o buffStr to have the next buffer if empty
|    o incurments pointInBufferCStr to start of next read
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
    FILE *samFILE,           /*Fastq file with input*/
    FILE *outFILE           /*file to output reads to*/
);

#endif
