/*#######################################################\
# Name: vectScan
#   - holds functions for scaning files with vectors
\#######################################################*/

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' SOF: Start Of File
'   o header:
'     - guards
'   o fun01: endLine_vectScan
'     - finds end of line using vectors
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*-------------------------------------------------------\
| Header:
|   - guards
\-------------------------------------------------------*/

#ifndef VECTOR_SCAN_H
#define VECTOR_SCAN_H

/*-------------------------------------------------------\
| Fun01: endLine_vectScan
|   - finds end of line using vectors
| Variants:
|   o fun01 var0a:
|     - SSE find end of line
|   o fun01 var0b:
|     - AVX2 find end of line
|   o fun01 var0C:
|     - AVX512 find end of line
|   o fun01 var0D: endLine_vectScan
|     - NEON find end of line
|   o fun01 var0e:
|     - scalar find end of line
| Input:
|   o buffStr:
|     - buffer to scan for new line or null
| Output:
|  - Returns:
|    o number of bytes to first end of line or null
\-------------------------------------------------------*/
unsigned long
endLine_vectScan(
   signed char *buffStr
);

#endif
