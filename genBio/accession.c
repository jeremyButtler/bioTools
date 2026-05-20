/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' accession SOF: Start Of File
'   - functions for dealing with database accession
'     names (currently genbank only)
'   o header:
'     - included libraries
'   o fun01: genbankAccExtract_accession
'     - splits a genbank accession number into its
'       components of accession, assembly version, contig
'       number, and sequence version
'     - note: stops at first white space (if have an
'       accession)
'   o license:
'     - licensing for this code (CC0)
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*-------------------------------------------------------\
| Header:
|   - included libraries
\-------------------------------------------------------*/

#ifdef PLAN9
   #include <u.h>
#endif

/*-------------------------------------------------------\
| Fun01: genbankAccExtract_accession
|   - splits a genbank accession number into its
|     components of accession, assembly version, contig
|     number, and sequence version
|   - note: stops at first white space (if have an
|     accession)
| Input:
|   - accStr:
|     o c-string with accession
|   - outStr:
|     o c-string to get the accession bin
|   - refseqStr:
|     o 4 character c-string array to get the refseq id
|       * bytes 0 and 1 get refseq id
|       * bytes 2 gets null/0 or for genome accesions the
|         third letter (GCA or GCF)
|       * byte 3 gets null/0 if genome accession
|   - accVersionSIPtr:
|     o signed int pointer to get accession version or -1
|   - asmVersionSIPtr:
|     o signed int pointer to get assembely version or -1
|   - contigNumSIPtr:
|     o signed int pointer to get the contig number of
|       the accession
| Output:
|   - Modifies:
|     o outStr to have the name of the bin to assign the
|       assembly to
|     o refseqStr to have the two character refseq id or
|       null/0
|     o accVersionSIPtr to have the version number of the
|       assembly or -1 if does not end in `.<version>`
|     o asmVersionSIPtr to have the assembly version
|       number or -1 if not an assembly id
|     o contigNumSIPtr to have the contig number or -1
|   - Returns:
|     o length of bin name
|     o 0 if no accession input
|     o < 0 if not a known genbank accession format
\-------------------------------------------------------*/
signed int
genbankAccExtract_accession(
   signed char *accStr,
   signed char *outStr,
   signed char *refseqStr,
   signed int *accVersionSIPtr,
   signed int *asmVersionSIPtr,
   signed int *contigNumSIPtr
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun01 TOC:
   '   - finds bin accession number to assign accesion to
   '   o fun01 sec01:
   '     - variable declarations
   '   o fun01 sec02:
   '     - initial setup and quick checks
   '   o fun01 Sec03:
   '     - genome assembly accessions check and parser
   '   o fun01 sec04:
   '     - refseq accession check and get first letters
   '   o fun01 sec05:
   '     - non-assembly accessions
   '   o fun01 sec06:
   '     - likely an assembly accession
   '   o fun01 sec07:
   '     - return and clean up
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun01 Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   signed int retLenSI = 0;
   signed int posSI = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun01 Sec02:
   ^   - initial setup and quick checks
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   *accVersionSIPtr = -1;
   *asmVersionSIPtr = -1;
   *contigNumSIPtr = -1;
   refseqStr[0] = 0;
   outStr[0] = 0;

   if(! accStr)
      goto noInput_fun01_sec07;
   else if(! accStr[0])
      goto noInput_fun01_sec07;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun01 Sec03:
   ^   - genome assembly accessions check and parser
   ^   -https://support.nlm.nih.gov/kbArticle/?pn=KA-03451
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if((accStr[0] | 32) != 'g')
      ;
   else if((accStr[1] | 32) != 'c')
      ;
   else if(
         (accStr[2] | 32) != 'a'
      && (accStr[2] | 32) != 'f'
   ) ;

   else if(accStr[3] == '_')
   { /*Else: genome assembly id*/
      /*________get_genome_assembly_tag_________________*/
      refseqStr[0] = accStr[0];
      refseqStr[1] = accStr[1];
      refseqStr[2] = accStr[2];
      refseqStr[3] = 0;

      /*________get_accession_number____________________*/
      /*the format should have 9 numbers, but I want ot
      `  future proof a bit
      */
      retLenSI = 0;
      for(posSI = 4; accStr[posSI]; ++posSI)
      { /*Loop: copy the accession number*/
         if(accStr[posSI] == '.')
            break;
         else if(accStr[posSI] < 32)
            break;
         else if(accStr[posSI] < '0')
            goto notAcc_fun01_sec07;
         else if(accStr[posSI] > '9')
            goto notAcc_fun01_sec07;

         outStr[retLenSI++] = accStr[posSI];
      } /*Loop: copy the accession number*/

      outStr[retLenSI] = 0;

      /*________get_version_number______________________*/
      if(accStr[posSI] == '.')
      { /*If: have a version number*/
         *accVersionSIPtr = 0;
         for(posSI = posSI + 1; accStr[posSI]; ++posSI)
         { /*Loop: get the version number*/
            if(accStr[posSI] <= 32)
               break;
            else if(accStr[posSI] < '0')
               goto notAcc_fun01_sec07;
            else if(accStr[posSI] > '9')
               goto notAcc_fun01_sec07;

            *accVersionSIPtr *= 10;
            *accVersionSIPtr += (accStr[posSI] - 48);
         } /*Loop: get the version number*/
      } /*If: have a version number*/

      goto ret_fun01_sec07;
   } /*Else If: genome assembly id*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun01 Sec04:
   ^   - refseq accession check and get first letters
   ^   -https://support.nlm.nih.gov/kbArticle/?pn=KA-03437
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*______________get_two_letter_refseq_id_____________*/
   if((accStr[0] | 32) < 'a' || (accStr[0] | 32) > 'z')
      goto notAcc_fun01_sec07;
   else if((accStr[1] | 32) < 'a')
      ;
   else if((accStr[1] | 32) > 'z')
      ;
   else if(accStr[2] == '_')
   { /*If: refseq accession number*/
      posSI = 3;
      refseqStr[0] = accStr[0];
      refseqStr[1] = accStr[1];
      refseqStr[2] = 0;
   } /*If: refseq accession number*/

   /*______________get_starting_letters_if_any__________*/
   retLenSI = 0;
   for( ; accStr[posSI]; ++posSI)
   { /*Loop: copy the first part of the assembly id*/
      if((accStr[posSI] | 32) < 'a')
         break;
      else if((accStr[posSI] | 32) > 'z')
         break;
      outStr[retLenSI++] = accStr[posSI];

      if(retLenSI > 16)
         goto notAcc_fun01_sec07;
         /*to many letters to be a valid id*/
   } /*Loop: copy the first part of the assembly id*/

   outStr[retLenSI] = 0;

   /*make sure have the alphabetical part of the ids*/
   if((accStr[posSI] | 32) < '0')
      goto notAcc_fun01_sec07;
   else if((accStr[posSI] | 32) > '9')
      goto notAcc_fun01_sec07;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun01 Sec05:
   ^   - non-assembly accessions
   ^   -https://support.nlm.nih.gov/kbArticle/?pn=KA-03437
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(retLenSI <= 3)
   { /*If: not an assembly id*/
      /*________get_assembly_numeric_part_______________*/
      for( ; accStr[posSI]; ++posSI)
      { /*Loop: copy the accession number*/
         if(accStr[posSI] == '.')
            break;
         else if(accStr[posSI] < 32)
            break;
         else if(accStr[posSI] < '0')
            goto notAcc_fun01_sec07;
         else if(accStr[posSI] > '9')
            goto notAcc_fun01_sec07;

         outStr[retLenSI++] = accStr[posSI];
      } /*Loop: copy the accession number*/

      outStr[retLenSI] = 0;

      /*________get_version_number______________________*/
      if(accStr[posSI] == '.')
      { /*If: have a version number*/
         *accVersionSIPtr = 0;
         for(posSI = posSI + 1; accStr[posSI]; ++posSI)
         { /*Loop: get the version number*/
            if(accStr[posSI] <= 32)
               break;
            else if(accStr[posSI] < '0')
               goto notAcc_fun01_sec07;
            else if(accStr[posSI] > '9')
               goto notAcc_fun01_sec07;

            *accVersionSIPtr *= 10;
            *accVersionSIPtr += (accStr[posSI] - 48);
         } /*Loop: get the version number*/
      } /*If: have a version number*/

      goto ret_fun01_sec07;
   } /*If: not an assembly id*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun01 Sec06:
   ^   - assembly accessions
   ^   -https://support.nlm.nih.gov/kbArticle/?pn=KA-03437
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   else
   { /*Else: likely an assembly accession id*/
      /*_________get_assembly_version_number____________*/
      if(accStr[posSI + 1] < '0')
          goto notAcc_fun01_sec07;
      else if (accStr[posSI + 1] > '9')
          goto notAcc_fun01_sec07;

      *asmVersionSIPtr = ( (accStr[posSI] - 48) * 10 );
      ++posSI;
      *asmVersionSIPtr += (accStr[posSI] - 48);
      ++posSI;

      /*________get_contig_number_______________________*/
      *contigNumSIPtr = 0;
      for(; accStr[posSI]; ++posSI)
      { /*Loop: get contig number*/
         if(accStr[posSI] == '.')
            break; /*version number*/
         else if(accStr[posSI] < 32)
            break;
         else if(accStr[posSI] < '0')
            goto notAcc_fun01_sec07;
         else if(accStr[posSI] > '9')
            goto notAcc_fun01_sec07;

         *contigNumSIPtr *= 10;
         *contigNumSIPtr += (accStr[posSI] - 48);
      } /*Loop: get contig number*/

      /*________get_version_number______________________*/
      if(accStr[posSI] == '.')
      { /*If: have a version number*/
         *accVersionSIPtr = 0;
         for(posSI = posSI + 1; accStr[posSI]; ++posSI)
         { /*Loop: get the version number*/
            if(accStr[posSI] <= 32)
               break;
            else if(accStr[posSI] < '0')
               goto notAcc_fun01_sec07;
            else if(accStr[posSI] > '9')
               goto notAcc_fun01_sec07;

            *accVersionSIPtr *= 10;
            *accVersionSIPtr += (accStr[posSI] - 48);
         } /*Loop: get the version number*/
      } /*If: have a version number*/

      goto ret_fun01_sec07;
   } /*Else: likely an assembly accession id*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun01 Sec07:
   ^   - return and clean up
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   goto ret_fun01_sec07;

   noInput_fun01_sec07:;
      retLenSI = 0;
      goto errClean_fun01_sec07;
   notAcc_fun01_sec07:;
      retLenSI = -1;
      goto errClean_fun01_sec07;
   errClean_fun01_sec07:;
      outStr[0] = 0;
      goto ret_fun01_sec07;

   ret_fun01_sec07:;
      return retLenSI;

   /*NCBI accession format:
   `   - https://gist.github.com/clintval/38afe5fc33e179902f09290a186c3e36
   `   - all formats can end in .<version_number>
   `   - nucleotide:
   `     o 1 letter + 5 numbers
   `     o 2 letters + 6 numbers
   `   - protien:
   `     o 3 letters + 5 numbers
   `   - WGS (assembly)
   `     o 4 letters + 2 numbers + 6 to 8 numbers
   `       - 4 letters for the id
   `       - 2 numbers for assembly version
   `       - 6 to 8 numbers for number of contigs
   ` - https://support.nlm.nih.gov/kbArticle/?pn=KA-03436
   `   o genbank accesion number format
   `   o [alphabetical prefix] [series of digits] [.] [version number]
   ` - https://support.nlm.nih.gov/kbArticle/?pn=KA-03437
   `   o more accession version for refseq
   `   o [A-Z][A-Z]_[0-9,A-Z].[0-9]
   `     - two letters fore the refseq index
   `     - an `_`
   `     - numbers or alphabetical characters
   ` - https://www.ncbi.nlm.nih.gov/books/NBK21091/table/ch18.T.refseq_accession_numbers_and_mole/?report=objectonly
   `   o refseq prefixes
   ` - https://support.nlm.nih.gov/kbArticle/?pn=KA-03436
   `   o [A-Z]*[0-9]*.[0-9]*
   `     - alphabet prefix
   `     - numbers
   `     - .version
   ` - https://support.nlm.nih.gov/kbArticle/?pn=KA-03451
   `   o genome assembly accession (for entire assembly)
   `   - [GCA] [ _  ] [nine digits] [.] [version number]
   `     * genbank
   `   - [GCF] [ _  ] [nine digits] [.] [version number]
   `     * refseq
   */
} /*genbankAccExtract_accession*/

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
