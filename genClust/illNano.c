/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' illNano SOF: Start Of File
'   - extracs ONT reads with Illumina variants
'   o header:
'     - included libraries
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
|   - included libraries
\-------------------------------------------------------*/

#ifdef PLAN9
   #include <u.h>
   #include <libc.h>
#else
   #include <stdlib.h>
#endif

#include "illNano.h"

#include <stdio.h>

#include "../genLib/base10str.h"
#include "../genLib/numToStr.h"
#include "../genBio/samEntry.h"

/*.h files only*/
#include "../genLib/dataTypeShortHand.h"

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\
! Hidden libraries
!   o .c   #include "../genLib/numToStr.h"
!   o .c   #include "../genLib/strAry.h"
!   o .h   #include "ntTo5Bit.h"
\%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

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
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun01 TOC:
   '   - gets snps from tbCon tsv file for Illumina data
   '   o fun01 sec01:
   '     - variable declarations
   '   o fun01 sec02:
   '     - allocate profile memory (initial)
   '   o fun01 sec03:
   '     - read in variants
   '   o fun01 sec04:
   '     - remove positons with no varaints (one base)
   '   o fun01 sec05:
   '     - clean up and return result
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun01 Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   #define lenBuff_fun01 4096
   schar buffStr[lenBuff_fun01];
   uint uiPos = 0;

   uint refPosUI = 0;
   schar baseSC = 0;
   uint depthUI = 0;
   float percDepthF = 0;
   schar *profileStr = 0;
   uint sizeProfUI = 0;
   uint tmpUI = 0;

   schar *tmpStr = 0;
   schar delBl = 0;   /*1: deletion entry*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun01 Sec02:
   ^   - allocate profile memory (initial)
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   *lenProfUI = 0;

   profileStr = malloc((4096 + 9) * sizeof(schar));
   sizeProfUI = 4095;

   if(! profileStr)
      goto memErr_fun01_sec05;

   for(
      tmpUI = 0;
      tmpUI <= sizeProfUI;
      ++tmpUI
   ) profileStr[tmpUI] = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun01 Sec03:
   ^   - read in variants
   ^   o fun01 sec03 sub01:
   ^     - get past reference id/name + start loop
   ^   o fun01 sec03 sub02:
   ^     - get reference position
   ^   o fun01 sec03 sub03:
   ^     - check mutation type
   ^   o fun01 sec03 sub04:
   ^     - get nucleotide (if snp)
   ^   o fun01 sec03 sub05:
   ^     - get read depth (do minimum depth checks)
   ^   o fun01 sec03 sub06:
   ^     - get percent read depth (do min/max check)
   ^   o fun01 sec03 sub07:
   ^     - check if profile needs more memory
   ^   o fun01 sec03 sub08:
   ^     - assign variant to profile
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun01 Sec03 Sub01:
   *   - get past reference id/name + start loop
   \*****************************************************/

   tmpStr =
      (schar *)
      fgets(
         (char *) buffStr,
         lenBuff_fun01,
         (FILE *) tsvFILE
      ); /*get past header*/

   while(
      fgets(
         (char *) buffStr,
         lenBuff_fun01,
         (FILE *) tsvFILE
      )
   ){ /*Loop: get varaints*/
      uiPos = 0;

      while(buffStr[uiPos++] > 32) ;

      if(
            buffStr[uiPos - 1] != '\t'
         && buffStr[uiPos - 1] != ' '
      ) goto fileErr_fun01_sec05;

      while(buffStr[uiPos] < 33)
      { /*Loop: get off white space*/
         if(buffStr[uiPos] == '\0')
            goto fileErr_fun01_sec05; /*end of line*/
         ++uiPos;
      } /*Loop: get off white space*/

      /**************************************************\
      * Fun01 Sec03 Sub02:
      *   - get reference position
      \**************************************************/

      uiPos +=
         strToUI_base10str(
            &buffStr[uiPos],
            &refPosUI
         ); /*get position*/

      --refPosUI; /*convert to index 0*/

      if(
            buffStr[uiPos] != '\t'
         && buffStr[uiPos] != ' '
      ) goto fileErr_fun01_sec05;

      /*move to next entry*/

      while(buffStr[uiPos] < 33)
      { /*Loop: get off white space*/
         if(buffStr[uiPos] == '\0')
            goto fileErr_fun01_sec05; /*end of line*/
         ++uiPos;
      } /*Loop: get off white space*/

      /**************************************************\
      * Fun01 Sec03 Sub03:
      *   - check mutation type
      \**************************************************/

      /*get past type entry*/
      if(
            buffStr[uiPos] == 'i'
         && buffStr[uiPos + 1] == 'n'
         && buffStr[uiPos + 2] == 's'
      ) continue; /*is insertion entry*/

      if(
            buffStr[uiPos] == 'd'
         && buffStr[uiPos + 1] == 'e'
         && buffStr[uiPos + 2] == 'l'
      ) delBl = 1; /*deletion entry*/

      else
         delBl = 0;

      while(buffStr[uiPos++] > 32) ;

      if(
            buffStr[uiPos - 1] != '\t'
         && buffStr[uiPos - 1] != ' '
      ) goto fileErr_fun01_sec05;

      while(buffStr[uiPos] < 33)
      { /*Loop: get off white space*/
         if(buffStr[uiPos] == '\0')
            goto fileErr_fun01_sec05; /*end of line*/
         ++uiPos;
      } /*Loop: get off white space*/

      /**************************************************\
      * Fun01 Sec03 Sub04:
      *   - get nucleotide (if snp)
      \**************************************************/

      if(! delBl)
         baseSC = buffStr[uiPos]; /*nucleotide*/
      else
         baseSC = 'D';

      /*move to next entry*/

      while(buffStr[uiPos++] > 32) ;

      if(
            buffStr[uiPos - 1] != '\t'
         && buffStr[uiPos - 1] != ' '
      ) goto fileErr_fun01_sec05;

      while(buffStr[uiPos] < 33)
      { /*Loop: get off white space*/
         if(buffStr[uiPos] == '\0')
            goto fileErr_fun01_sec05; /*end of line*/
         ++uiPos;
      } /*Loop: get off white space*/

      /**************************************************\
      * Fun01 Sec03 Sub05:
      *   - get read depth (do minimum depth checks)
      \**************************************************/

      uiPos +=
         strToUI_base10str(
            &buffStr[uiPos],
            &depthUI
         ); /*get read depth*/

      if(
            buffStr[uiPos] != '\t'
         && buffStr[uiPos] != ' '
      ) goto fileErr_fun01_sec05;

      if(buffStr[uiPos] > 32)
         goto fileErr_fun01_sec05; /*end of line*/

      if(depthUI < minDepthUI)
         continue; /*variant not worth keeping*/

      /*move to next entry*/

      while(buffStr[uiPos] < 33)
      { /*Loop: get off white space*/
         if(buffStr[uiPos] == '\0')
            goto fileErr_fun01_sec05; /*end of line*/
         ++uiPos;
      } /*Loop: get off white space*/

      /**************************************************\
      * Fun01 Sec03 Sub06:
      *   - get percent read depth (do min/max check)
      \**************************************************/

      tmpUI = uiPos;

      while(buffStr[tmpUI++] > 32) ;
      --tmpUI;
      buffStr[tmpUI] = '\0';
      tmpUI = 0;

      percDepthF = atof((char *) &buffStr[uiPos]);

      if(
            ! delBl
         && percDepthF < minPercDepthF
      ) continue; /*variant not worth keeping*/

      else if(
            delBl
         && percDepthF < minPercDelF
      ) continue; /*deletion has to low support*/

      /**************************************************\
      * Fun01 Sec03 Sub07:
      *   - check if profile needs more memory
      \**************************************************/

      if(sizeProfUI < refPosUI)
      { /*If: need to resize array*/
         tmpStr =
            realloc(
               profileStr,
               ((refPosUI << 1) + 9) * sizeof(schar)
            );

         if(! tmpStr)
            goto memErr_fun01_sec05;

         profileStr = tmpStr;
         tmpStr = 0;

         for(
            tmpUI = sizeProfUI;
            tmpUI <= sizeProfUI << 1;
            ++tmpUI
         ) profileStr[tmpUI] = 0;

         sizeProfUI <<= 1;
      } /*If: need to resize array*/

      /**************************************************\
      * Fun01 Sec03 Sub08:
      *   - assign variant to profile
      \**************************************************/

      *lenProfUI = refPosUI;

      if(delBl)
      { /*If: was deletion*/
         profileStr[refPosUI] |= def_del_illNano;
         continue;
      } /*If: was deletion*/

      switch(baseSC & ~32)
      { /*Switch: find base*/
         case 'A':
            profileStr[refPosUI] |= def_A_illNano;
            break;
         case 'C':
            profileStr[refPosUI] |= def_C_illNano;
            break;
         case 'G':
            profileStr[refPosUI] |= def_G_illNano;
            break;
         case 'T':
            profileStr[refPosUI] |= def_T_illNano;
            break;
      } /*Switch: find base*/
   } /*Loop: get varaints*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun01 Sec04:
   ^   - remove positons with no varaints (one base)
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   ++(*lenProfUI);

   for(
      refPosUI = 0;
      refPosUI < *lenProfUI;
      ++refPosUI
   ){ /*Loop: remove positons with one varaint*/
      baseSC = profileStr[refPosUI];

      /*from stanford bit twiddeling guie; Brain Kernihn's
      `  method for counting bits
      */
      for(
         depthUI = 0;
         baseSC;
         ++depthUI
      ) baseSC &= (baseSC - 1);
        /*baseSC &= (baseSC - 1) removes last set bit
           baseSC &= 0x1111 - 0x1
             goes to 0x1110
           baseSC &= 0x1110 - 0x1
             goes to 0x1101 - 0x1
             goes to 0x1100
        */ 

      if(depthUI < 2)
         profileStr[refPosUI] = def_noBase_illNano;
         /*this case is true most of the time, so
         `   branch prediction is good here
         */
   } /*Loop: remove positons with one varaint*/

   profileStr[*lenProfUI] = '\0';

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun01 Sec05:
   ^   - clean up and return result
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   *errSCPtr = 0;
   goto ret_fun01_sec05;

   memErr_fun01_sec05:;
     *errSCPtr = def_memErr_illNano;
     goto errCleanUp_fun01_sec05;

   fileErr_fun01_sec05:;
      *errSCPtr = def_fileErr_illNano;
      goto errCleanUp_fun01_sec05;

   errCleanUp_fun01_sec05:;
      if(profileStr)
         free(profileStr);
      profileStr=0;
      goto ret_fun01_sec05;

   ret_fun01_sec05:;
      return profileStr;
} /*getIllSnp_illNano*/

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
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun02 TOC: getVarNano_illNano
   '   - identifies variants in a nanopore read using an
   '     Illumina profile
   '   o fun02 sec01:
   '     - allocate memory and get first line of sam file
   '   o fun02 sec02:
   '     - get first line of sam file
   '   o fun02 sec03:
   '     - scan read for variants
   '   o fun02 sec04:
   '     - return number detected variants
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun02 Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   uint refPosUI = 0;
   uint readPosUI = 0;
   uint cigPosUI = 0;
   uint cigNtUI = 0;

   uint numVarUI = 0;     /*number of variants*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun02 Sec03:
   ^   - scan read for variants
   ^   o fun02 sec03 sub01:
   ^     - prepare for scan + start loop + cigar switch
   ^   o fun02 sec03 sub02:
   ^     - scan snp/matches for variants
   ^   o fun02 sec03 sub03:
   ^     - check if have deletion variants
   ^   o fun02 sec03 sub04:
   ^     - ignore indel variants
   ^   o fun02 sec03 sub05:
   ^     - move to next cigar entry
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun02 Sec03 Sub01:
   *   - prepare for scan + start loop + cigar switch
   \*****************************************************/

   refPosUI = samSTPtr->refStartUI;

   if(samSTPtr->cigTypeStr[0] == 'S')
   { /*If: first bases soft masked*/
      cigPosUI = 1;
      readPosUI = samSTPtr->cigArySI[0];
   } /*If: first bases soft masked*/

   if(samSTPtr->cigTypeStr[0] == 'H')
      cigPosUI = 1;

   while(cigPosUI < samSTPtr->lenCigUI)
   { /*Loop: go through cigar*/

      if(profileStr[refPosUI] == '\0')
         goto ret_fun02_sec04;

      switch(samSTPtr->cigTypeStr[cigPosUI])
      { /*Switch: find mutation type*/

         /***********************************************\
         * Fun02 Sec03 Sub02:
         *   - scan snp/matches for variants
         *   o fun02 sec03 sub02 cat01:
         *     - found match/snp; loop though bases
         *   o fun02 sec03 sub02 cat02:
         *     - A variant cases
         *   o fun02 sec03 sub02 cat03:
         *     - C variant cases
         *   o fun02 sec03 sub02 cat04:
         *     - G variant cases
         *   o fun02 sec03 sub02 cat05:
         *     - T variant cases
         *   o fun02 sec03 sub02 cat06:
         *     - move to next base
         \***********************************************/

         /*++++++++++++++++++++++++++++++++++++++++++++++\
         + Fun02 Sec03 Sub02 Cat01:
         +   - found match/snp; loop though bases
         \++++++++++++++++++++++++++++++++++++++++++++++*/

         case 'M':
         case '=':
         case 'X':
         /*Case: match or snp*/
            cigNtUI = samSTPtr->cigArySI[cigPosUI];

            while(cigNtUI > 0)
            { /*Loop: see if have variant*/
               if(
                    profileStr[refPosUI]
                  & def_noBase_illNano
               ) goto nextNt_fun02_sec03_sub02_cat06;
                  /*no variants kept for position*/

               if(profileStr[refPosUI] == '\0')
                  goto ret_fun02_sec04;
               
               mapPosAryUI[numVarUI] = refPosUI;

               switch(
                  samSTPtr->seqStr[readPosUI] & ~32
               ){ /*Switch: find sequence base*/

                  /*+++++++++++++++++++++++++++++++++++++\
                  + Fun02 Sec03 Sub02 Cat02:
                  +  - A variant cases
                  \+++++++++++++++++++++++++++++++++++++*/

                  case 'A':
                  /*Case: read has A*/
                     if(
                          profileStr[refPosUI]
                        & def_A_illNano
                     ) mapNtAryUI[numVarUI] = 'A';
                       /*if variant dected*/

                     else if(tranBl)
                     { /*Else If: unkown; check transit*/
                        if(
                             profileStr[refPosUI]
                           & def_G_illNano
                        ) mapNtAryUI[numVarUI] = 'A';
                          /*is transition of varaint*/

                        else
                           mapNtAryUI[numVarUI] =
                              def_unkown_illNano;
                     } /*Else If: unkown; check transit*/

                     else
                        mapNtAryUI[numVarUI] =
                           def_unkown_illNano;
                           /*no idea what is*/

                     break;
                  /*Case: read has A*/

                  /*+++++++++++++++++++++++++++++++++++++\
                  + Fun02 Sec03 Sub02 Cat03:
                  +  - C variant cases
                  \+++++++++++++++++++++++++++++++++++++*/

                  case 'C':
                  /*Case: read has C*/
                     if(
                          profileStr[refPosUI]
                        & def_C_illNano
                     ) mapNtAryUI[numVarUI] = 'C';
                       /*variant detected*/

                     else if(tranBl)
                     { /*Else If: unkown; check transit*/
                        if(
                             profileStr[refPosUI]
                           & def_T_illNano
                        ) mapNtAryUI[numVarUI] = 'C';
                          /*is transition of varaint*/

                        else
                           mapNtAryUI[numVarUI] =
                              def_unkown_illNano;
                     } /*Else If: unkown; check transit*/

                     else
                        mapNtAryUI[numVarUI] =
                           def_unkown_illNano;
                           /*no idea what is*/

                     break;
                  /*Case: read has C*/

                  /*+++++++++++++++++++++++++++++++++++++\
                  + Fun02 Sec03 Sub02 Cat04:
                  +  - G variant cases
                  \+++++++++++++++++++++++++++++++++++++*/

                  case 'G':
                  /*Case: read has G*/
                     if(
                          profileStr[refPosUI]
                        & def_G_illNano
                     ) mapNtAryUI[numVarUI] = 'G';
                       /*matches varaint*/

                     else if(tranBl)
                     { /*Else If: unkown; check transit*/
                        if(
                             profileStr[refPosUI]
                           & def_A_illNano
                        ) mapNtAryUI[numVarUI] = 'G';
                          /*is transition of varaint*/

                        else
                           mapNtAryUI[numVarUI] =
                              def_unkown_illNano;
                     } /*Else If: unkown; check transit*/

                     else
                        mapNtAryUI[numVarUI] =
                           def_unkown_illNano;
                           /*no idea what is*/
                     break;
                  /*Case: read has G*/

                  /*+++++++++++++++++++++++++++++++++++++\
                  + Fun02 Sec03 Sub02 Cat05:
                  +  - T variant cases
                  \+++++++++++++++++++++++++++++++++++++*/

                  case 'T':
                  /*Case: read has T*/
                     if(
                          profileStr[refPosUI]
                        & def_T_illNano
                     ){ /*If: have variant*/
                        mapNtAryUI[numVarUI] = 'T';
                     } /*If: have variant*/

                     else if(tranBl)
                     { /*Else If: unkown; check transit*/
                        if(
                             profileStr[refPosUI]
                           & def_C_illNano
                        ) mapNtAryUI[numVarUI] = 'T';
                          /*is transition of varaint*/

                        else
                           mapNtAryUI[numVarUI] =
                              def_unkown_illNano;
                     } /*Else If: unkown; check transit*/

                     else
                        mapNtAryUI[numVarUI] =
                           def_unkown_illNano;
                           /*no idea what is*/

                     break;
                  /*Case: read has T*/
               } /*Switch: find sequence base*/

               /*++++++++++++++++++++++++++++++++++++++++\
               + Fun02 Sec03 Sub02 Cat06:
               +   - move to next base
               \++++++++++++++++++++++++++++++++++++++++*/

               ++numVarUI; /*only fires if profile pos*/

               nextNt_fun02_sec03_sub02_cat06:;

               --cigNtUI;
               ++readPosUI;
               ++refPosUI;
            } /*Loop: see if have variant*/

            break;
         /*Case: match or snp*/

         /***********************************************\
         * Fun02 Sec03 Sub03:
         *   - check if have deletion variants
         \***********************************************/

         case 'D':
         /*Case: deletion*/
            cigNtUI = samSTPtr->cigArySI[cigPosUI];

            while(cigNtUI > 0)
            { /*Loop: see if have variant*/
               if(profileStr[refPosUI] == '\0')
                  goto ret_fun02_sec04;
                  /*no more variants in profile*/;

               if(
                     profileStr[refPosUI]
                  != def_noBase_illNano
               ){ /*If: have variant at position*/
                  mapPosAryUI[numVarUI] = refPosUI;

                  if(
                     profileStr[refPosUI] &def_del_illNano
                  ) mapNtAryUI[numVarUI] =
                     def_delChar_illNano;
                     /*deletion variant*/
                  else
                     mapNtAryUI[numVarUI] =
                        def_unkown_illNano;
                        /*no idea what is*/

                  ++numVarUI;
               } /*If: have variant at position*/

               ++refPosUI;
               --cigNtUI;
            } /*Loop: see if have variant*/

            break;
         /*Case: deletion*/

         /***********************************************\
         * Fun02 Sec03 Sub04:
         *   - ignore indel variants
         \***********************************************/

         case 'I':
         /*Case: insertion*/
            readPosUI += samSTPtr->cigArySI[cigPosUI];
            break;
         /*Case: insertion*/
      } /*Switch: find mutation type*/

      /**************************************************\
      * Fun02 Sec03 Sub05:
      *   - move to next cigar entry
      \**************************************************/

      ++cigPosUI;
   } /*Loop: go through cigar*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun02 Sec04:
   ^   - return number detected variants
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   mapPosAryUI[numVarUI] = 0;
   mapNtAryUI[numVarUI] = '\0';
   
   ret_fun02_sec04:;
   return numVarUI;
} /*getVarNano_illNano*/

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
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun03 TOC: getNanoReads_illNano
   '   - gets nanopore reads using Illumina profile
   '   o fun03 sec01:
   '     - allocate memory and get first line of sam file
   '   o fun03 sec02:
   '     - get first line of sam file
   '   o fun03 sec03:
   '     - get reads from sam file
   '   o fun03 sec04:
   '     - clean up and return result
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   schar errSC = 0;

   uint *mapPosAryUI = 0; /*holds position of mapped var*/
   uint *mapNtArySC = 0;  /*nucleotide of mapped var*/
   uint numVarUI = 0;     /*number of variants*/
   uint uiVar = 0;        /*for printing variants*/
   uint uiPos = 0;        /*position in output buffer*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec02:
   ^   - allocate memory and get first line of sam file
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   mapPosAryUI = malloc(lenProfileUI * sizeof(uint));

   if(! mapPosAryUI)
      goto memErr_fun03_sec04;

   mapNtArySC = malloc(lenProfileUI * sizeof(schar));

   if(! mapNtArySC)
      goto memErr_fun03_sec04;

   errSC =
      get_samEntry(
         samSTPtr,
         buffStrPtr,
         lenBuffULPtr,
         samFILE
       );

   if(errSC)
   { /*If: had error*/
      if(errSC == def_memErr_samEntry)
        goto memErr_fun03_sec04;
      else
        goto fileErr_fun03_sec04;
   } /*If: had error*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec03:
   ^   - get reads from sam file
   ^   o fun03 sec03 sub01:
   ^     - filter reads + start loop
   ^   o fun03 sec03 sub02:
   ^     - find Illumina variants in read
   ^   o fun03 sec03 sub03:
   ^     - print kept read (cluster is cigar like format)
   ^   o fun03 sec03 sub04:
   ^     - move to next read
   ^   o fun03 sec03 sub05:
   ^     - final error check
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun03 Sec03 Sub01:
   *   - filter reads + start loop
   \*****************************************************/

   fprintf(
      (FILE *) outFILE,
      "id\tref\tnum_var\tprofile\n"
   );

   while(! errSC)
   { /*Loop: get reads*/
      if(samSTPtr->extraStr[0] == '@')
         goto nextRead_fun03_sec03_sub04;

      if(samSTPtr->flagUS & (4 | 256 | 2048))
         goto nextRead_fun03_sec03_sub04;

      /**************************************************\
      * Fun03 Sec03 Sub02:
      *   - find Illumina variants in read
      \**************************************************/

      numVarUI =
         getVarNano_illNano(
            samSTPtr,
            profileStr,
            tranBl,
            mapPosAryUI,
            mapNtArySC
         );

      /**************************************************\
      * Fun03 Sec03 Sub03:
      *   - print kept read (cluster is cigar like format)
      \**************************************************/

      if(numVarUI == 0)
         goto nextRead_fun03_sec03_sub04;
         /*no variant positions in read; ignore*/

      fprintf(
         (FILE *) outFILE,
         "%s\t%s\t%u\t",
         samSTPtr->qryIdStr,
         samSTPtr->refIdStr,
         numVarUI
      );

      uiPos = 0;

      for(
         uiVar = 0;
         uiVar < numVarUI;
         ++uiVar
      ){ /*Loop: print out each variant position*/
         if(uiVar > 0)
            (*buffStrPtr)[uiPos++] = '_'; /*for humans*/
         
         uiPos +=
            numToStr(
               &(*buffStrPtr)[uiPos],
               mapPosAryUI[uiVar] + 1  /*to index 1*/
            );
               
         (*buffStrPtr)[uiPos++] = mapNtArySC[uiVar];
      } /*Loop: print out each variant position*/

      (*buffStrPtr)[uiPos++] = '\n';
      (*buffStrPtr)[uiPos] = '\0';

      fprintf(
         (FILE *) outFILE,
         "%s",
         *buffStrPtr
      ); /*add new line ot end of line*/

      /**************************************************\
      * Fun03 Sec03 Sub04:
      *   - move to next read
      \**************************************************/

      nextRead_fun03_sec03_sub04:;

      errSC =
         get_samEntry(
            samSTPtr,
            buffStrPtr,
            lenBuffULPtr,
            samFILE
          );
   } /*Loop: get varaints*/

   /*****************************************************\
   * Fun03 Sec03 Sub05:
   *   - move to next read
   \*****************************************************/

   if(errSC == def_memErr_samEntry)
      goto memErr_fun03_sec04;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec04:
   ^   - clean up and return result
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   errSC = 0;
   goto ret_fun03_sec04;

   memErr_fun03_sec04:;
      errSC = def_memErr_illNano;
      goto ret_fun03_sec04;

   fileErr_fun03_sec04:;
      errSC = def_fileErr_illNano;
      goto ret_fun03_sec04;

   ret_fun03_sec04:;
      if(mapPosAryUI)
         free(mapPosAryUI);
      mapPosAryUI = 0;

      if(mapNtArySC)
         free(mapNtArySC);
      mapNtArySC = 0;

      return errSC;
} /*getNanoReads_illNano*/


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
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun04 TOC:
   '   - runs the illNano algorithim
   '   o fun04 sec01:
   '     - variable declarations
   '   o fun04 sec02:
   '     - set up (initialize and memory allocation)
   '   o fun04 sec03:
   '     - get illumina profile
   '   o fun04 sec04:
   '     - find reads with variant positions in profile
   '   o fun04 sec05:
   '     - clean up and return
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun04 Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   schar errSC = 0;

   schar *profHeapStr = 0;
   uint lenProfUI = 0;

   struct samEntry samStackST;
   schar *buffHeapStr = 0;
   ulong lenBuffUL = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun04 Sec02:
   ^   - set up (initialize and memory allocation)
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   init_samEntry(&samStackST);
   errSC = setup_samEntry(&samStackST);

   if(errSC)
      goto memErr_fun04_sec05;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun04 Sec03:
   ^   - get illumina profile
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   profHeapStr =
      getIllSnp_illNano(
         minDepthUI,
         minPercDepthF,
         minPercDelF,
         &lenProfUI,
         illTsvFILE,
         &errSC
      ); /*get profile for Illumina varaints*/

   if(errSC)
   { /*If: have error*/
      if(errSC == def_memErr_illNano)
         goto memErr_fun04_sec05;
      else
         goto fileErr_fun04_sec05;
   } /*If: have error*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun04 Sec04:
   ^   - find reads with variant positions in profile
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   errSC =
      getNanoReads_illNano(
         profHeapStr,
         lenProfUI,
         tranBl,
         &samStackST,
         &buffHeapStr,
         &lenBuffUL,
         samFILE,
         outFILE
      ); /*extract reads with variant positions*/

   if(errSC == def_memErr_illNano)
      goto memErr_fun04_sec05;

   if(errSC == def_fileErr_illNano)
      goto fileErr_fun04_sec05;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun04 Sec05:
   ^   - clean up and return
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   errSC = 0;
   goto ret_fun04_sec05;

   memErr_fun04_sec05:;
      errSC = def_memErr_illNano;
      goto ret_fun04_sec05;

   fileErr_fun04_sec05:;
      errSC = def_fileErr_illNano;
      goto ret_fun04_sec05;

   ret_fun04_sec05:;
      if(profHeapStr)
         free(profHeapStr);
      profHeapStr = 0;
      lenProfUI = 0;

      if(buffHeapStr)
         free(buffHeapStr);
      buffHeapStr = 0;
      lenBuffUL = 0;

      freeStack_samEntry(&samStackST);

      return errSC;
} /*run_illNano*/

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
