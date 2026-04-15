/*
for testIdatCrc32
cc \
    -o tmp.o \
    ../../genLib/checkSum.c \
    ../../genLib/endin.c \
    tmp.c
*/

#include <stdlib.h>
#include <stdio.h>

#include "../../genLib/checkSum.h"
#include "../../genLib/endin.h"

int
main(
   void
){

   /*the correct answer is 6cef2cbf*/
   unsigned int crc32UI = 0xffffffff;
   crc32UI = crc32Byte_checkSum(0x49, crc32UI);
   crc32UI = crc32Byte_checkSum(0x44, crc32UI);
   crc32UI = crc32Byte_checkSum(0x41, crc32UI);
   crc32UI = crc32Byte_checkSum(0x54, crc32UI);
   crc32UI = crc32Byte_checkSum(0x78, crc32UI);
   crc32UI = crc32Byte_checkSum(0xda, crc32UI);
   crc32UI = crc32Byte_checkSum(0x01, crc32UI);
   crc32UI = crc32Byte_checkSum(0x02, crc32UI);
   crc32UI = crc32Byte_checkSum(0x00, crc32UI);
   crc32UI = crc32Byte_checkSum(0xfd, crc32UI);
   crc32UI = crc32Byte_checkSum(0xff, crc32UI);
   crc32UI = crc32Byte_checkSum(0x00, crc32UI);
   crc32UI = crc32Byte_checkSum(0x50, crc32UI);

   /*is the adler32*/
   crc32UI = crc32Byte_checkSum(0x00, crc32UI);
   crc32UI = crc32Byte_checkSum(0x52, crc32UI);
   crc32UI = crc32Byte_checkSum(0x00, crc32UI);
   crc32UI = crc32Byte_checkSum(0x51, crc32UI);

   crc32UI = crc32Finish_checkSum(crc32UI);
   printf("%x\n", crc32UI);
} /*main*/
