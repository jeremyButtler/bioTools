/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' vectScan SOF: Start Of File
'   - holds functions for scaning files with vectors
'   o header:
'     - included libraries
'   o fun01: endLine_vectScan
'     - finds end of line using vectors
'   o license:
'     - licensing for this code (CC0)
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
   '     - variable declerations
   '   o fun01 var0a sec02:
   '     - find end of line
   '   o fun01 var0a sec03:
   '    - get coordinates for end of line
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun01 Var0A Sec01:
   ^  - variable declerations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   unsigned long posUL = 0;
   unsigned long maskUL = 0;

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

   unsigned long posUL = 0;
   unsigned long maskUL = 0;

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

   unsigned long posUL = 0;
   unsigned long maskUL = 0;

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

   unsigned long posUL = 0;
   unsigned long offsetUL = 0;
   unsigned long maskUL = 0;

   int8x16_t lineVect = vdupq_n_s8('\n');
   int8x16_t nullVect = vdupq_n_s8(0);

   int8x16_t buffVect = vld1q_s8((int8_t *) buffStr);

   /*get population counts*/
   uint16x8_t res16x8 = vreinterpretq_u16_s8((vectI8)); \
   uint8x8_t res8x8 = vshrn_n_u16(res16x8, 4); \
   uint64x1_t res64x1 = vreinterpret_u64_u8(res8x8); \

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun01 Var0D Sec02:
   ^  - find end of line
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

  res16x8 =
     vreinterpretq_u16_s8(
        vorrq_s8(
            vceqq_s8(buffVect, lineVect),
            vceqq_s8(buffVect, nullVect)
        ) /*merge comparisons*/
     );
  res8x8 = vshrn_n_u16(res16x8, 4);
  res64x1 = vreinterpret_u64_u8(res8x8);
  maskUL = vget_lane_u64(res64x1, 0);

   while(! maskUL)
   { /*Loop: find end of line/buffer*/
      posUL += 16;

      buffVect =
         vld1q_s8((int8_t *) &buffStr[posUL]);

      /*get poplution count*/
      res16x8 =
         vreinterpretq_u16_s8(
            vorrq_s8(
                vceqq_s8(buffVect, lineVect),
                vceqq_s8(buffVect, nullVect)
            ) /*merge comparisons*/
         );
      res8x8 = vshrn_n_u16(res16x8, 4);
      res64x1 = vreinterpret_u64_u8(res8x8);
      maskUL = vget_lane_u64(res64x1, 0);
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
   #ifdef NON_SAFE
      return endLineUnix_ulCp(buffStr);
      /*cacluations on this are bases on '\n', so need to
      `  move past carriage returns. This might pose a
      `  problem on MACs
      */
      /*this is actually slower, odd*/
   #else
      /*not as fast, but will work even if only a '\r'
      ` (ancient mac) is present
      */
      unsigned long retUL = endLine_ulCp(buffStr);

      return retUL;
   #endif
} /*endLine_vectScan*/

#endif /*NEON or scalr check*/
#endif /*AVX512 check*/
#endif /*AVX2 check*/
#endif /*SSE check*/

/*=======================================================\
: License:
: 
: Creative Commons Legal Code
: 
: CC0 1.0 Universal
: 
:     CREATIVE COMMONS CORPORATION IS NOT A LAW FIRM AND
:     DOES NOT PROVIDE LEGAL SERVICES. DISTRIBUTION OF
:     THIS DOCUMENT DOES NOT CREATE AN ATTORNEY-CLIENT
:     RELATIONSHIP. CREATIVE COMMONS PROVIDES THIS
:     INFORMATION ON AN "AS-IS" BASIS. CREATIVE COMMONS
:     MAKES NO WARRANTIES REGARDING THE USE OF THIS
:     DOCUMENT OR THE INFORMATION OR WORKS PROVIDED
:     HEREUNDER, AND DISCLAIMS LIABILITY FOR DAMAGES
:     RESULTING FROM THE USE OF THIS DOCUMENT OR THE
:     INFORMATION OR WORKS PROVIDED HEREUNDER.
: 
: Statement of Purpose
: 
: The laws of most jurisdictions throughout the world
: automatically confer exclusive Copyright and Related
: Rights (defined below) upon the creator and subsequent
: owner(s) (each and all, an "owner") of an original work
: of authorship and/or a database (each, a "Work").
: 
: Certain owners wish to permanently relinquish those
: rights to a Work for the purpose of contributing to a
: commons of creative, cultural and scientific works
: ("Commons") that the public can reliably and without
: fear of later claims of infringement build upon, modify,
: incorporate in other works, reuse and redistribute as
: freely as possible in any form whatsoever and for any
: purposes, including without limitation commercial
: purposes. These owners may contribute to the Commons to
: promote the ideal of a free culture and the further
: production of creative, cultural and scientific works,
: or to gain reputation or greater distribution for their
: Work in part through the use and efforts of others.
: 
: For these and/or other purposes and motivations, and
: without any expectation of additional consideration or
: compensation, the person associating CC0 with a Work
: (the "Affirmer"), to the extent that he or she is an
: owner of Copyright and Related Rights in the Work,
: voluntarily elects to apply CC0 to the Work and publicly
: distribute the Work under its terms, with knowledge of
: his or her Copyright and Related Rights in the Work and
: the meaning and intended legal effect of CC0 on those
: rights.
: 
: 1. Copyright and Related Rights. A Work made available
:    under CC0 may be protected by copyright and related
:    or neighboring rights ("Copyright and Related
:    Rights"). Copyright and Related Rights include, but
:    are not limited to, the following:
: 
:   i. the right to reproduce, adapt, distribute, perform,
:      display, communicate, and translate a Work;
:  ii. moral rights retained by the original author(s)
:      and/or performer(s);
: iii. publicity and privacy rights pertaining to a
:      person's image or likeness depicted in a Work;
:  iv. rights protecting against unfair competition in
:      regards to a Work, subject to the limitations in
:      paragraph 4(a), below;
:   v. rights protecting the extraction, dissemination,
:      use and reuse of data in a Work;
:  vi. database rights (such as those arising under
:      Directive 96/9/EC of the European Parliament and of
:      the Council of 11 March 1996 on the legal
:      protection of databases, and under any national
:      implementation thereof, including any amended or
:      successor version of such directive); and
: vii. other similar, equivalent or corresponding rights
:      throughout the world based on applicable law or
:      treaty, and any national implementations thereof.
: 
: 2. Waiver. To the greatest extent permitted by, but not
:    in contravention of, applicable law, Affirmer hereby
:    overtly, fully, permanently, irrevocably and
:    unconditionally waives, abandons, and surrenders all
:    of Affirmer's Copyright and Related Rights and
:    associated claims and causes of action, whether now
:    known or unknown (including existing as well as
:    future claims and causes of action), in the Work (i)
:    in all territories worldwide, (ii) for the maximum
:    duration provided by applicable law or treaty
:    (including future time extensions), (iii) in any
:    current or future medium and for any number of
:    copies, and (iv) for any purpose whatsoever,
:    including without limitation commercial, advertising
:    or promotional purposes (the "Waiver"). Affirmer
:    makes the Waiver for the benefit of each member of
:    the public at large and to the detriment of
:    Affirmer's heirs and successors, fully intending that
:    such Waiver shall not be subject to revocation,
:    rescission, cancellation, termination, or any other
:    legal or equitable action to disrupt the quiet
:    enjoyment of the Work by the public as contemplated
:    by Affirmer's express Statement of Purpose.
: 
: 3. Public License Fallback. Should any part of the
:    Waiver for any reason be judged legally invalid or
:    ineffective under applicable law, then the Waiver
:    shall be preserved to the maximum extent permitted
:    taking into account Affirmer's express Statement of
:    Purpose. In addition, to the extent the Waiver is so
:    judged Affirmer hereby grants to each affected person
:    a royalty-free, non transferable, non sublicensable,
:    non exclusive, irrevocable and unconditional license
:    to exercise Affirmer's Copyright and Related Rights
:    in the Work (i) in all territories worldwide, (ii)
:    for the maximum duration provided by applicable law
:    or treaty (including future time extensions), (iii)
:    in any current or future medium and for any number of
:    copies, and (iv) for any purpose whatsoever,
:    including without limitation commercial, advertising
:    or promotional purposes (the "License"). The License
:    shall be deemed effective as of the date CC0 was
:    applied by Affirmer to the Work. Should any part of
:    the License for any reason be judged legally invalid
:    or ineffective under applicable law, such partial
:    invalidity or ineffectiveness shall not invalidate
:    the remainder of the License, and in such case
:    Affirmer hereby affirms that he or she will not (i)
:    exercise any of his or her remaining Copyright and
:    Related Rights in the Work or (ii) assert any
:    associated claims and causes of action with respect
:    to the Work, in either case contrary to Affirmer's
:    express Statement of Purpose.
: 
: 4. Limitations and Disclaimers.
: 
:  a. No trademark or patent rights held by Affirmer are
:     waived, abandoned, surrendered, licensed or
:     otherwise affected by this document.
:  b. Affirmer offers the Work as-is and makes no
:     representations or warranties of any kind concerning
:     the Work, express, implied, statutory or otherwise,
:     including without limitation warranties of title,
:     merchantability, fitness for a particular purpose,
:     non infringement, or the absence of latent or other
:     defects, accuracy, or the present or absence of
:     errors, whether or not discoverable, all to the
:     greatest extent permissible under applicable law.
:  c. Affirmer disclaims responsibility for clearing
:     rights of other persons that may apply to the Work
:     or any use thereof, including without limitation any
:     person's Copyright and Related Rights in the Work.
:     Further, Affirmer disclaims responsibility for
:     obtaining any necessary consents, permissions or
:     other rights required for any use of the Work.
:  d. Affirmer understands and acknowledges that Creative
:     Commons is not a party to this document and has no
:     duty or obligation with respect to this CC0 or use
:     of the Work.
\=======================================================*/
