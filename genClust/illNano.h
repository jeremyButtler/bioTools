/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' illNano SOF: Start Of File
'   - extracs ONT reads with Illumina variants
'   o header:
'     - forward declarations, defined variabals, guards
'   o fun01: getIllSnp_illNano
'     - gets snps from tbCon tsv file for Illumina data
'   o fun02: getVarNano_illNano
'     - identifies variants in a nanopore read using an
'       Illumina profile
'   o fun03: getNanoReads_illNano
'     - gets nanopore reads using Illumina profile
'   o fun04: run_illNano
'     - runs the illNano algorithim
'   o license:
'     - licensing for this code (public domain / mit)
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*-------------------------------------------------------\
| Header:
|   - forward declarations, defined variabals, guards
\-------------------------------------------------------*/

#ifndef ILLUMINA_NANOPORE_READ_EXTRACT_H
#define ILLUMINA_NANOPORE_READ_EXTRACT_H

typedef struct samEntry samEntry;

#define def_memErr_illNano 1
#define def_fileErr_illNano 2

#define def_A_illNano 1
#define def_C_illNano 2
#define def_G_illNano 4
#define def_T_illNano 8
#define def_del_illNano 16
#define def_noBase_illNano 64

#define def_delChar_illNano 'D'
#define def_unkown_illNano 'X'

/*-------------------------------------------------------\
| Fun01: getIllSnp_illNano
|   - gets snps from tbCon tsv file for Illumina data
| Input:
|   - minDepthUI:
|     o min read depth to keep varaint
|   - minPercDepthF:
|     o min percent read depth to keep varaint
|   - minPercDelF:
|     o min percent read depth to keep deletion; you
|       should use at least 50%, so deletion needs to be
|       major choice
|   - lenProfUI:
|     o pointer to unsigned int to hold length (final
|       reference position) of profile
|   - tsvFILE:
|     o FILE pointer to tsv file from tbCon
|   - errSCPtr:
|     o pointer to signed char to hold returned error
| Output:
|   - Modifies:
|     o lenProfUI to have reference position of last
|       variant
|     o tsvFILE to point to end of file (EOF)
|     o errSCPtr:
|       - 0 for no errors
|       - def_memErr_illNano for memory errors
|       - def_fileErr_illNano for file errors
|   - Returns:
|     o signed char array with variants
|       o def_A_illNano if A supported
|       o def_C_illNano if C supported
|       o def_G_illNano if G supported
|       o def_T_illNano if T supported
|       o def_del_illNano if del supported
|       o def_noBase_illNano if no variant for position
|       o '\0' for end of variants (no more variants)
|     o 0 for memory error
\-------------------------------------------------------*/
signed char *
getIllSnp_illNano(
   unsigned int minDepthUI, /*min depth to keep snp*/
   float minPercDepthF,     /*min % depth to keep snp*/
   float minPercDelF,       /*min % depth to keep del*/
   unsigned int *lenProfUI, /*length of final profile*/
   void *tsvFILE,           /*tsv with snps*/
   signed char *errSCPtr
);

/*-------------------------------------------------------\
| Fun02: getVarNano_illNano
|   - identifies variants in a nanopore read using an
|     Illumina profile
| Input:
|   - samSTPtr:
|     o pointer to samEntry stuct with read to scan
|   - profileStr:
|     o c-strint with variants (profile) to extract
|   - mapPosAryUI:
|     o unsigned int array to store variant positions in
|       (size needs to be number variants in profileStr)
|   - mapNtArySC:
|     o signed char array to store variant nucleotides in
|       (size needs to be number variants in profileStr)
|   - tranBl:
|     o 1: for unkown entries; assigned variant to a
|          transversion
|     o 0: all unidentifed entries are def_unkown_illNano
| Output:
|   - Modifies:
|     o mapPosAryUI to have variant positions
|     o mapNtArySC to have variant nucleotdies
|   - Returns:
|     o number of detected variant positions
\-------------------------------------------------------*/
unsigned int
getVarNano_illNano(  
   struct samEntry *samSTPtr,/*read to scan*/
   signed char *profileStr,  /*illumina profile*/
   signed char tranBl,       /*1: is uknown transition*/
   unsigned int *mapPosAryUI,/*will have variant pos*/
   unsigned int *mapNtAryUI  /*will have variant bases*/
);

/*-------------------------------------------------------\
| Fun03: getNanoReads_illNano
|   - gets nanopore reads using Illumina profile
| Input:
|   - profileStr:
|     o c-strint with variants (profile) to extract
|   - lenProfileUI:
|     o position of last reference base in profile
|   - tranBl:
|     o 1: for unkown entries; assigned variant to a
|          transversion
|     o 0: all unidentifed entries are def_unkown_illNano
|   - buffStrPtr:
|     o pointer to c-string to use in reading sam file
|   - lenBuffULPtr:
|     o length of buffStrPtr
|   - samFILE:
|     o FILE pointer to sam file with reads to get
|   - outFILE:
|     o FILE pointer to file to print reads to
| Output:
|   - Modifies:
|     o samSTPtr to have last sam file entry
|     o buffStrPtr to have last line in sam flie
|     o lenBuffULPtr to have buffStrPtr size (if resized)
|     o samFILE to point to end of file (EOF)
|   - Prints:
|     o read ids + profile to outFILE as tsv
|   - Returns:
|     o 0 for no errors
|     o def_memErr_illNano for memory errors
|     o def_fileErr_illNano for file errors
\-------------------------------------------------------*/
signed char
getNanoReads_illNano(  
   signed char *profileStr,  /*illumina profile*/
   unsigned int lenProfileUI,/*last ref position in prof*/
   signed char tranBl,       /*1: is uknown transition*/
   struct samEntry *samSTPtr,/*sam file reading*/
   signed char **buffStrPtr, /*buffer for file reading*/
   unsigned long *lenBuffULPtr,/*length of buffStrPtr*/
   void *samFILE,            /*sam file with ONT reads*/
   void *outFILE             /*file to save reads to*/
);

/*-------------------------------------------------------\
| Fun04: run_illNano
|   - runs the illNano algorithim
| Input:
|   - minDepthUI:
|     o minimum depth to keep illumina variant
|   - minPercDepthF:
|     o minimum percent read depth to keep Illumina SNP
|   - minPercDelF:
|     o min percent read depth to keep Illumina deletion
|   - tranBl:
|     o 1: for unkown entries; assigned variant to a
|          transversion
|     o 0: all unidentifed entries are def_unkown_illNano
|   - illTsvFILE:
|     o FILE pointer to tbCon tsv file with Illumina vars
|   - samFILE:
|     o FILE pointer to sam file with reads to get
|   - outFILE:
|     o FILE pointer to file to print reads to
| Output:
|   - Modifies:
|     o illTsvFILE to point to end of file (EOF)
|     o samFILE to point to end of file (EOF)
|   - Prints:
|     o read ids + profile to outFILE as tsv
|   - Returns:
|     o 0 for no errors
|     o def_memErr_illNano for memory errors
|     o def_fileErr_illNano for file errors
\-------------------------------------------------------*/
signed char
run_illNano(  
   unsigned int minDepthUI,  /*min depth for illumina*/
   float minPercDepthF,      /*min % depth for ill snp*/
   float minPercDelF,        /*min % depth for ill del*/
   signed char tranBl,       /*1: is uknown transition*/
   void *illTsvFILE,         /*tbCon tsv file for ill*/
   void *samFILE,            /*sam file with ONT reads*/
   void *outFILE             /*file to save reads to*/
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
