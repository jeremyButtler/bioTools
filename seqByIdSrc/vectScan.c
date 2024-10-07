/*#######################################################\
# Name: vectScan
#   - holds functions for scaning files with vectors
\#######################################################*/

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' SOF: Start Of File
'   o header:
'     - included libraries
'   o fun01: endLine_vectScan
'     - finds end of line using vectors
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*-------------------------------------------------------\
| Header:
|   - included libraries
\-------------------------------------------------------*/

#ifdef PLAN9
   #include <u.h>
   #include <libc.h>
#endif

#include "../genLib/ulCp.h" /*scalar fallback*/

/*.h file only (no .c)*/
#include "../genLib/dataTypeShortHand.h"

#ifdef SSE
   #include <immintrin.h>
#else
   #ifdef AVX2
      #include <immintrin.h>
   #else
      #ifdef AVX512
         #include <immintrin.h>
      #else
         #ifdef NEON
            #include <arm_neon.h>
         #endif /*neon*/
      #endif /*avx512 check*/
   #endif /*avx2 check*/
#endif /*sse check*/

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

#ifdef SSE

/*_______________________________________________________\
@ Fun01 Var0A: endLine_vectScan
@   - SSE find end of line
\_______________________________________________________*/

unsigned long
endLine_vectScan(
   signed char *buffStr
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun01 Var0A TOC:
   '   - finds end of line using SSE vectors
   '   o fun01 var0a sec01:
   '     - var0aible declerations
   '   o fun01 var0a sec02:
   '     - find end of line
   '   o fun01 var0a sec03:
   '    - get coordinates for end of line
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun01 Var0A Sec01:
   ^  - var0aible declerations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   ulong posUL = 0;
   ulong maskUL = 0;

   __m128i lineVect = _mm_set1_epi8('\n');
   __m128i nullVect = _mm_setzero_si128();

   __m128i buffVect =
      _mm_loadu_si128((__m128i *) buffStr);

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun01 Var0A Sec02:
   ^  - find end of line
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   maskUL =
      _mm_movemask_epi8(
         _mm_or_si128(
            _mm_cmpeq_epi8(buffVect, lineVect),
            _mm_cmpeq_epi8(buffVect, nullVect)
         ) /*merge comparisions*/
      ); /*get mask for comparision*/

   while(! maskUL)
   { /*Loop: find end of line/buffer*/
      posUL += 16;

      buffVect =
         _mm_loadu_si128((__m128i *) &buffStr[posUL]);

      maskUL =
         _mm_movemask_epi8(
            _mm_or_si128(
               _mm_cmpeq_epi8(buffVect, lineVect),
               _mm_cmpeq_epi8(buffVect, nullVect)
            ) /*merge comparisions*/
         ); /*get mask for comparision*/
   } /*Loop: find end of line/buffer*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun01 Var0A Sec03:
   ^  - get coordinates for end of line
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(maskUL & 0x00FF)
      maskUL &= 0x00FF;

   else
   { /*Else: the first bit is in upper position*/
     posUL += 8;
     maskUL = maskUL >> 8;
   } /*Else: the first bit is in upper position*/

   if(maskUL & 0x0F)
      maskUL &= 0x0F;

   else
   { /*Else: the first bit is in upper position*/
     posUL += 4;
     maskUL = maskUL >> 4;
   } /*Else: the first bit is in upper position*/

   if(maskUL & 3)
      maskUL &= 3;

   else
   { /*Else: the first bit is in upper position*/
     posUL += 2;
     maskUL = maskUL >> 2;
   } /*Else: the first bit is in upper position*/

   if(!(maskUL & 1))
      ++posUL;

   return posUL;
} /*endLine_vectScan*/

#else
#ifdef AVX2

/*_______________________________________________________\
@ Fun01 Var0B: endLine_vectScan
@   - AVX2 find end of line
\_______________________________________________________*/

unsigned long
endLine_vectScan(
   signed char *buffStr
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun01 Var0B TOC:
   '   - finds end of line using SSE vectors
   '   o fun01 var0b sec01:
   '     - var0aible declerations
   '   o fun01 var0b sec02:
   '     - find end of line
   '   o fun01 var0b sec03:
   '    - get coordinates for end of line
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun01 Var0B Sec01:
   ^  - var0aible declerations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   ulong posUL = 0;
   ulong maskUL = 0;

   __m256i lineVect = _mm256_set1_epi8('\n');
   __m256i nullVect = _mm256_setzero_si256();

   __m256i buffVect =
      _mm256_loadu_si256((__m256i *) buffStr);

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun01 Var0B Sec02:
   ^  - find end of line
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   maskUL =
      _mm256_movemask_epi8(
         _mm256_cmpeq_epi8(
            buffVect,
            lineVect
         ) /*check if have newline*/
      ); /*get mask for comparision*/

   if(! maskUL)
   { /*If: no new line*/
      maskUL =
         _mm256_movemask_epi8(
            _mm256_cmpeq_epi8(
               buffVect,
               nullVect
            ) /*check if have null*/
         ); /*get mask for comparision*/
   } /*If: no new line*/

   while(! maskUL)
   { /*Loop: find end of line/buffer*/
      posUL += 32;

      buffVect =
         _mm256_loadu_si256((__m256i *) &buffStr[posUL]);

      maskUL =
         _mm256_movemask_epi8(
            _mm256_cmpeq_epi8(
               buffVect,
               lineVect
            ) /*check if have newline*/
         ); /*get mask for comparision*/

      if(! maskUL)
      { /*If: no new line*/
         maskUL =
            _mm256_movemask_epi8(
               _mm256_cmpeq_epi8(
                  buffVect,
                  nullVect
               ) /*check if have null*/
            ); /*get mask for comparision*/
      } /*If: no new line*/
   } /*Loop: find end of line/buffer*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun01 Var0B Sec03:
   ^  - get coordinates for end of line
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(maskUL & 0x0000FFFF)
      maskUL &= 0x0000FFFF;

   else
   { /*Else: the first bit is in upper position*/
     posUL += 16;
     maskUL = maskUL >> 16;
   } /*Else: the first bit is in upper position*/

   if(maskUL & 0x00FF)
      maskUL &= 0x00FF;

   else
   { /*Else: the first bit is in upper position*/
     posUL += 8;
     maskUL = maskUL >> 8;
   } /*Else: the first bit is in upper position*/

   if(maskUL & 0x0F)
      maskUL &= 0x0F;

   else
   { /*Else: the first bit is in upper position*/
     posUL += 4;
     maskUL = maskUL >> 4;
   } /*Else: the first bit is in upper position*/

   if(maskUL & 3)
      maskUL &= 3;

   else
   { /*Else: the first bit is in upper position*/
     posUL += 2;
     maskUL = maskUL >> 2;
   } /*Else: the first bit is in upper position*/

   if(!(maskUL & 1))
      ++posUL;

   return posUL;
} /*endLine_vectScan*/

#else
#ifdef AVX512

/*_______________________________________________________\
@ Fun01 Var0C: endLine_vectScan
@   - AVX512 find end of line
\_______________________________________________________*/

unsigned long
endLine_vectScan(
   signed char *buffStr
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun01 Var0C TOC:
   '   - finds end of line using avx512 vectors
   '   o fun01 var0c sec01:
   '     - var0aible declerations
   '   o fun01 var0c sec02:
   '     - find end of line
   '   o fun01 var0c sec03:
   '    - get coordinates for end of line
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun01 Var0C Sec01:
   ^  - var0aible declerations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   ulong posUL = 0;
   ulong maskUL = 0;

   __m512i lineVect = _mm512_set1_epi8('\n');
   __m512i nullVect = _mm512_setzero_si512();

   __m512i buffVect =
      _mm512_loadu_si512((__m512i *) buffStr);

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun01 Var0C Sec02:
   ^  - find end of line
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   maskUL =
      _cvtmask64_u64(
         _mm512_cmpeq_epi8_mask(
            buffVect,
            lineVect
         ) /*check if have newline*/
      ); /*get mask for comparision*/

   if(! maskUL)
   { /*If: no new line*/
      maskUL =
         _cvtmask64_u64(
            _mm512_cmpeq_epi8_mask(
               buffVect,
               nullVect
            ) /*check if have null*/
         ); /*get mask for comparision*/
   } /*If: no new line*/

   while(! maskUL)
   { /*Loop: find end of line/buffer*/
      posUL += 64;

      buffVect =
         _mm512_loadu_si512((__m512i *) &buffStr[posUL]);

      maskUL =
         _cvtmask64_u64(
            _mm512_cmpeq_epi8_mask(
               buffVect,
               lineVect
            ) /*check if have newline*/
         ); /*get mask for comparision*/

      if(! maskUL)
      { /*If: no new line*/
         maskUL =
            _cvtmask64_u64(
               _mm512_cmpeq_epi8_mask(
                  buffVect,
                  nullVect
               ) /*check if have null*/
            ); /*get mask for comparision*/
      } /*If: no new line*/
   } /*Loop: find end of line/buffer*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun01 Var0C Sec03:
   ^  - get coordinates for end of line
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

    if(maskUL & 0x00000000FFFFFFFF)
      maskUL &= 0x00000000FFFFFFFF;

    else
    { /*Else: first bit is in upper position*/
      posUL += 32;
      maskUL = maskUL >> 32;
    } /*Else: first bit is in upper position*/

   if(maskUL & 0x0000FFFF)
      maskUL &= 0x0000FFFF;

   else
   { /*Else: the first bit is in upper position*/
     posUL += 16;
     maskUL = maskUL >> 16;
   } /*Else: the first bit is in upper position*/

   if(maskUL & 0x00FF)
      maskUL &= 0x00FF;

   else
   { /*Else: the first bit is in upper position*/
     posUL += 8;
     maskUL = maskUL >> 8;
   } /*Else: the first bit is in upper position*/

   if(maskUL & 0x0F)
      maskUL &= 0x0F;

   else
   { /*Else: the first bit is in upper position*/
     posUL += 4;
     maskUL = maskUL >> 4;
   } /*Else: the first bit is in upper position*/

   if(maskUL & 3)
      maskUL &= 3;

   else
   { /*Else: the first bit is in upper position*/
     posUL += 2;
     maskUL = maskUL >> 2;
   } /*Else: the first bit is in upper position*/

   if(!(maskUL & 1))
      ++posUL;

   return posUL;
} /*endLine_vectScan*/

#else
#ifdef NEON
/*_______________________________________________________\
@ Fun01 Var0D: endLine_vectScan
@   - NEON find end of line
\_______________________________________________________*/

#define \
storeNeon_vectScan( \
   vectI8 \
)({ \
  uint16x8_t res16x8 = vreinterpretq_u16_s8((vectI8)); \
  uint8x8_t res8x8 = vshrn_n_u16(res16x8, 4); \
  uint64x1_t res64x1 = vreinterpret_u64_u8(res8x8); \
  vget_lane_u64(res64x1, 0); \
}) /*storeNeon_vectScan*/

unsigned long
endLine_vectScan(
   signed char *buffStr
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun01 Var0D TOC:
   '   - finds end of line using neon vectors
   '   o fun01 var0d sec01:
   '     - var0aible declerations
   '   o fun01 var0d sec02:
   '     - find end of line
   '   o fun01 var0d sec03:
   '    - get coordinates for end of line
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun01 Var0D Sec01:
   ^  - var0aible declerations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   ulong posUL = 0;
   ulong offsetUL = 0;
   ulong maskUL = 0;

   int8x16_t lineVect = vdupq_n_s8('\n');
   int8x16_t nullVect = vdupq_n_s8(0);

   int8x16_t buffVect = vld1q_s8((int8_t *) buffStr);

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun01 Var0D Sec02:
   ^  - find end of line
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   maskUL =
      storeNeon_vectScan(
         vorrq_s8(
            vceqq_s8(buffVect, lineVect),
            vceqq_s8(buffVect, nullVect)
         ) /*merge comparisons*/
      ); /*get mask from comparisons*/

   while(! maskUL)
   { /*Loop: find end of line/buffer*/
      posUL += 16;

      buffVect =
         vld1q_s8((int8_t *) &buffStr[posUL]);

      maskUL =
         storeNeon_vectScan(
            vorrq_s8(
               vceqq_s8(buffVect, lineVect),
               vceqq_s8(buffVect, nullVect)
            ) /*merge comparisons*/
         ); /*get mask from comparisons*/
   } /*Loop: find end of line/buffer*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun01 Var0D Sec03:
   ^  - get coordinates for end of line
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

    /*NEON loads oddlly, so I need to treat like AVX512*/
    if(maskUL & 0x00000000FFFFFFFF)
      maskUL &= 0x00000000FFFFFFFF;

    else
    { /*Else: first bit is in upper position*/
      offsetUL += 32;
      maskUL = maskUL >> 32;
    } /*Else: first bit is in upper position*/

   if(maskUL & 0x0000FFFF)
      maskUL &= 0x0000FFFF;

   else
   { /*Else: the first bit is in upper position*/
     offsetUL += 16;
     maskUL = maskUL >> 16;
   } /*Else: the first bit is in upper position*/

   if(maskUL & 0x00FF)
      maskUL &= 0x00FF;

   else
   { /*Else: the first bit is in upper position*/
     offsetUL += 8;
     maskUL = maskUL >> 8;
   } /*Else: the first bit is in upper position*/

   if(maskUL & 0x0F)
      maskUL &= 0x0F;

   else
   { /*Else: the first bit is in upper position*/
     offsetUL += 4;
     maskUL = maskUL >> 4;
   } /*Else: the first bit is in upper position*/

   if(maskUL & 3)
      maskUL &= 3;

   else
   { /*Else: the first bit is in upper position*/
     offsetUL += 2;
     maskUL = maskUL >> 2;
   } /*Else: the first bit is in upper position*/

   if(!(maskUL & 1))
      ++offsetUL;

   return posUL + (offsetUL >> 2); /*fix count errors*/
} /*endLine_vectScan*/

#else
/*_______________________________________________________\
@ Fun01 Var0E: endLine_vectScan
@   - scalar find end of line
\_______________________________________________________*/

unsigned long
endLine_vectScan(
   signed char *buffStr
){
   return endLine_ulCp(buffStr);
} /*endLine_vectScan*/

#endif /*NEON or scalr check*/
#endif /*AVX512 check*/
#endif /*AVX2 check*/
#endif /*SSE check*/
