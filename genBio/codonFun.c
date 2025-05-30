/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' codonTbl SOF: Start Of File
'   - has codon table and functions to support the
'     codon table
'   o header:
'     - included libraries
'   o fun01: codonToAA_codonFun
'     - converts codon into an amino acid
'   o fun02: revCodonToAA_codonFun
'     - reverses complements input codon & gets amino acid
'   o fun03: bacStartCode_codonFun
'     - checks to see if input bases is a bacterial start
'       codon (convert nt with ntTo2Bit.h)
'   o fun04: bacStart_codonFun
'     - checks if codon (no conversion) is a start codon
'   o fun05: bactRevStart_codonFun
'     - checks if codon (no conversion) is a start codon
'       for a reverse complement sequence
'   o fun06: aaTripToChar_codonFun
'     - converts a three letter amino acid idenity to its
'       single letter amino acid identity
'   o fun07: seqToAA_codonFun
'     - converts nucleotide sequence to amino acids
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
   #include <stdlib.h>
#endif

#include "codonFun.h"

/*no .c files*/
#include "ntTo2Bit.h"
#include "revNtTo2Bit.h"
#include "codonTbl.h"

/*-------------------------------------------------------\
| Fun01: codonToAA_codonFun
|   - converts codon into an amino acid
| Input:
|   - firstNtSC:
|     o first base in codon
|   - secNtSC:
|     o second base in codon
|   - thirdNtSC:
|     o last base in codon
| Output:
|   - Returns:
|     o amino acid of the input codon
\-------------------------------------------------------*/
signed char
codonToAA_codonFun(
   signed char firstNtSC,
   signed char secNtSC,
   signed char thirdNtSC
){
   return
      codonTbl[
            ntTo2Bit[(unsigned char) firstNtSC]
         ][
            ntTo2Bit[(unsigned char) secNtSC]
         ][
            ntTo2Bit[(unsigned char) thirdNtSC]
         ];
} /*codonToAA_codonFun*/

/*-------------------------------------------------------\
| Fun02: revCodonToAA_codonFun
|   - reverses complements input codon & finds amino acid
| Input:
|   - firstNtSC:
|     o first base in the foward codon, but third (last)
|       base in the reverse complement codon
|   - secNtSC:
|     o second base in forward codon, but second (middle)
|       base in the reverse complement codon
|   - thirdNtSC:
|     o last base in forward codon, but first base in the
|       reverse complement codon
| Output:
|   - Returns:
|     o reverse complement amino acid of the input
|       codon
\-------------------------------------------------------*/
signed char
revCodonToAA_codonFun(
   signed char firstNtSC,
   signed char secNtSC,
   signed char thirdNtSC
){
   return
      codonTbl[
            revNtTo2Bit[(unsigned char) thirdNtSC]
         ][
            revNtTo2Bit[(unsigned char) secNtSC]
         ][
            revNtTo2Bit[(unsigned char) firstNtSC]
         ];
} /*revCodonToAA_codonFun*/

/*-------------------------------------------------------\
| Fun03: bactStartCode_codonFun
|   - checks to see if input bases is a bacterial start
|     codon (convert nt with ntTo2Bit.h)
| Input:
|   - firstNtSC:
|     o first base to check; it needs to be the output of
|       an base to code look up table
|   - secBaseC:
|     o second base to check; it needs to be the output
|       of an base to code look up table
|   - thirdBaseC:
|     o third base to check; it needs to be the output
|       of an base to code look up table
| Output:
|   - Returns:
|     o 1 if this is an bacterial start codon
|     o 0 if this is not an bacterial start codon
\-------------------------------------------------------*/
signed char
bactStartCode_codonFun(
   unsigned char firstNtSC,
   unsigned char secNtSC,
   unsigned char thirdNtSC
){
   firstNtSC = 
          (firstNtSC == def_a_ntTo2Bit)
        | (firstNtSC == def_g_ntTo2Bit)
        | (firstNtSC == def_t_ntTo2Bit);

   firstNtSC &= (secNtSC == def_t_ntTo2Bit);
   return firstNtSC & ((thirdNtSC) == def_g_ntTo2Bit);
} /*bacStartCode_codonFun*/
     /*Logic:
     `   - x = firstBaseC == def_g_codonFun:
     `     o 1 if the first base is a G
     `     o 0 if not a g
     `   - x |= (firstBaseC == def_t_codonFun):
     `     o 1 if the first base is an "G" or "T"
     `     o 0 if not an "G" or "T"
     `   - x |= (firstBaseC == def_a_codonFun):
     `     o 1 if the first base is an "G", "T", or "A"
     `     o 0 if not an "G", "T", or "A"
     `     o This covers the first codon for all posible
     `       start codons (ATG, GTG, and TTG)
     `   - The second and thrid base comparisions clear
     `     the bit (set to 0) if I do not have an
     `     TTG, GTG, or ATG codon
     */

/*-------------------------------------------------------\
| Fun04: bactStart_codonFun
|   - checks if codon (no conversion) is a start codon
| Input:
|   - firstNtSC:
|     o first base to check
|   - secBaseC:
|     o second base to check
|   - thirdBaseC:
|     o third base to check
| Output:
|   - Returns:
|     o 1 if this is a bacterial start codon
|     o 0 if this is not a bacterial start codon
\-------------------------------------------------------*/
signed char
bactStart_codonFun(
   signed char firstNtSC,
   signed char secNtSC,
   signed char thirdNtSC
){
   firstNtSC = 
          ((firstNtSC & ~32) == 'A')
        | ((firstNtSC & ~32) == 'G')
        | ((firstNtSC & ~32) == 'T');

   firstNtSC &= ((secNtSC & ~32) == 'T');
   return firstNtSC & ((thirdNtSC & ~32) == 'G');
} /*bacStart_codonFun*/

/*-------------------------------------------------------\
| Fun05: bactRevStart_codonFun
|   - checks if codon (no conversion) is a start codon
|     for a reverse complement sequence
| Input:
|   - firstNtSC:
|     o first base to check (last in codon)
|   - secBaseC:
|     o second base to check (middle in codon)
|   - thirdBaseC:
|     o third base to check (first in codon)
| Output:
|   - Returns:
|     o 1 if this is a reverse bacterial start codon
|     o 0 if this is not a reverse bacterial start codon
\-------------------------------------------------------*/
signed char
bactRevStart_codonFun(
   signed char firstNtSC,
   signed char secNtSC,
   signed char thirdNtSC
){
   thirdNtSC = 
          ((thirdNtSC & ~32) == 'A')
        | ((thirdNtSC & ~32) == 'C')
        | ((thirdNtSC & ~32) == 'T');

   thirdNtSC &= ((secNtSC & ~32) == 'A');
   return thirdNtSC & ((firstNtSC & ~32) == 'C');
} /*bacRevStart_codonFun*/

/*-------------------------------------------------------\
| Fun06: aaTripToChar_codonFun
|   - converts a three letter amino acid idenity to its
|     single letter amino acid identity
| Input:
|   - codonStr:
|     o c-string with the three letter amino acid to
|       convert
| Output:
|   - Returns:
|     o one letter code for the amino acid
|     o 0 for an invalid code
\-------------------------------------------------------*/
signed char
aaTripToChar_codonFun(
   signed char *codonStr
){
   signed char retSC = 0;

   switch(codonStr[0] & ~32)
   { /*Switch: Check the first letter*/
      case 'A':
      /*Case: The first letter is an A*/
         switch(codonStr[2] & ~32)
         { /*Switch: Check the A's third letter*/
            case 'A': retSC = 'a'; break; /*Ala*/
            case 'G': retSC = 'r'; break; /*Arg*/
            case 'N': retSC = 'n'; break; /*Asn*/
            case 'P': retSC = 'd'; break; /*Asp*/
         } /*Switch: Check the A's third letter*/

         break;
      /*Case: The first letter is an A*/

      case 'C': retSC = 'c'; break; /*Cys*/

      case 'G':
      /*Case: The first letter is a G*/
         switch(codonStr[2] & ~32)
         { /*Switch: Check the third letter of G*/
            case 'N': retSC = 'q'; break; /*Gln*/
            case 'U': retSC = 'e'; break; /*Glu*/
            case 'Y': retSC = 'g'; break; /*Gly*/
         } /*Switch: Check the third letter of G*/

         break;
      /*Case: The first letter is a G*/

      case 'H': retSC = 'h'; break; /*His*/
      case 'I': retSC = 'i'; break; /*Ile*/

      case 'L':
      /*Case: The first letter is a L*/
         switch(codonStr[2] & ~32)
         { /*Switch: Check the third letter of L*/
            case 'S': retSC = 'k'; break; /*Lys*/
            case 'U': retSC = 'l'; break; /*Leu*/
         } /*Switch: Check the third letter of L*/

         break;
      /*Case: The first letter is a L*/

      case 'M': retSC = 'm'; break; /*Met*/

      case 'P':
      /*Case: The first letter is a P*/
         switch(codonStr[2] & ~32)
         { /*Switch: Check the third letter of P*/
            case 'E': retSC = 'f'; break; /*Phe*/
            case 'O': retSC = 'p'; break; /*Pro*/
         } /*Switch: Check the third letter of P*/

         break;
      /*Case: The first letter is a P*/

      case 'S': retSC = 's'; break; /*Ser*/

      case 'T':
      /*Case: The first letter is a T*/
         switch(codonStr[2] & ~32)
         { /*Switch: Check the third letter of T*/
            case 'R': retSC = 't'; break; /*Thr*/
            case 'P': retSC = 'w'; break; /*Trp*/
            case 'Y': retSC = 'y'; break; /*Try*/
         } /*Switch: Check the third letter of T*/

         break;
      /*Case: The first letter is a T*/

      case 'V': retSC = 'v'; break; /*Val*/

      /*This case is for stop and it will look like a
      ` new line. So there is some risk
      */
      case ('*' & ~32): retSC = '*'; break; /*stop*/
   } /*Switch: Check the first letter*/

   return retSC;
} /*aaTripToChar_codonFun*/

/*--------------------------------------------------------\
| Fun07: seqToAA_codonFun
|  - converts a nucleotide sequence to amino acid sequence
| Input:
|  - seqStr: 
|    o c-string with the sequence to convert
|  - aaStr:
|    o c-string to hold the converted sequence, must be
|      at least sequence / 3 bases long
|  - startSL:
|    o position to start translation at
|    o use 0 for all sequence
|  - endSL:
|    o position to end translation at
|    o use 0 for all sequence
| Output:
|  - Modifies:
|    o aaStr to hold the amino acid sequence
|      * on nucleotide errors, a '\0' is added after last
|        correct call
|  - Returns:
|    o length (> 0) of returned ammino acid sequence
|    o def_unkownNt_codonFun (< 0) for sequence errors
|    o def_incomplete_codonFun (< 0) if had partial end
\--------------------------------------------------------*/
signed long
seqToAA_codonFun(
   signed char *seqStr,
   signed char *aaStr,
   signed long startSL,
   signed long stopSL
){
   signed long aaLenSL = 0;

   unsigned char firstNtUC = 0;
   unsigned char secNtUC = 0;
   unsigned char thirdNtUC = 0;

   if(! stopSL)
      stopSL = -1;
      /*using -1 so that I can use (unsigned long) -1 to
      `  get the maximum long value
      */

   while((unsigned long) startSL < (unsigned long) stopSL)
   { /*Loop: Translate sequence*/
      if(seqStr[startSL] == '\0')
         break;

      firstNtUC =
         ntTo2Bit[(unsigned char) seqStr[startSL++]];

      if(seqStr[startSL] == '\0')
         goto incomplete_fun07;

      secNtUC =
         ntTo2Bit[(unsigned char) seqStr[startSL++]];

      if(seqStr[startSL] == '\0')
         goto incomplete_fun07;

      thirdNtUC =
         ntTo2Bit[(unsigned char) seqStr[startSL++]];

      if(
         (   firstNtUC
           | secNtUC
           | thirdNtUC
         ) & 8
      ) goto unkownNt_fun07;
         /*unkown base*/

      aaStr[aaLenSL++] =
         codonTbl[firstNtUC][secNtUC][thirdNtUC];
   } /*Loop: Translate sequence*/\

   aaStr[aaLenSL] = '\0';
   return aaLenSL;

   unkownNt_fun07:;
      aaStr[aaLenSL] = '\0';
      return def_unkownNt_codonFun;
   incomplete_fun07:;
      aaStr[aaLenSL] = '\0';
      return def_incomplete_codonFun;
} /*seqToAA*/

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
