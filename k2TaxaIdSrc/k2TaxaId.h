/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' k2TaxaId SOF: Start Of File
'   - has functions and structs to get read ids by kraken
'     organisms
'   o header:
'     - included libraries
'   o .h st01: taxa_k2TaxaId
'     - has taxa names and ids to extract by
'   o fun01: blank_taxa_k2TaxaId
'     - blanks a taxa_k2TaxaId structure
'   o fun02: init_taxa_k2TaxaId
'     - initializes a taxa_k2TaxaId structure
'   o fun04: freeHeap_taxa_k2TaxaId
'     - frees a taxa_k2TaxaId structure
'   o fun05: setup_taxa_k2TaxaId
'     - allocates memory for a taxa_k2TaxaId struc
'   o fun06: realloc_taxa_k2TaxaId
'     - reallocates memory for a taxa_k2TaxaId struc
'   o fun07: sortCodes_taxa_k2TaxaId
'     - sorts the sortCodeArySL array while keeping the
'       indexArySL in sync a taxa_k2TaxaId
'   o fun08: findCode_taxa_k2TaxaId
'     - returns index of taxa code, searches sorted list
'       and returns unsorted list index
'   o fun09: getLevel_k2TaxaId
'     - get the level in tree of a kraken entry; col 4
'   o fun10: readReport_k2TaxaId
'     - gets list of organism codes
'   o .c fun11: mkTaxaIdFile_k2TaxaIds
'     - prints a single taxa id (here to avoid clutter)
'   o fun12: pIds_k2TaxaId
'     - prints out read ids by taxa for kraken2
'   o license:
'     - licensing for this code (CC0)
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*-------------------------------------------------------\
| Header:
|   - defined variables and gaurds
\-------------------------------------------------------*/

#ifndef KRAKEN_2_GET_TAXA_IDS_H
#define KRAKEN_2_GET_TAXA_IDS_H

struct str_ptrAry;

#define def_maxLevels_k2TaxaId 64 /*deepest level*/
#define def_maxDepth_k2TaxaId 32 /*maximum tree depth*/

#define def_unkown_k2TaxaId ((signed short) -1)

/*the unclassified bit deterimines the max number of
` levels one id can have. 16 means that I can have S1-S15
*/
#define def_unclassifed_k2TaxaId (1 << 4) /*16*/
#define def_root_k2TaxaId    (1 << 5)     /*32*/
#define def_domain_k2TaxaId  (1 << 6)     /*64*/
#define def_phylum_k2TaxaId  (1 << 7)     /*128*/

#define def_class_k2TaxaId   (1 << 8)     /*256*/
#define def_order_k2TaxaId   (1 << 9)     /*512*/
#define def_family_k2TaxaId  (1 << 10)    /*1024*/
#define def_genus_k2TaxaId   (1 << 11)    /*2048*/
#define def_species_k2TaxaId (1 << 12)    /*4096*/

#define def_memErr_k2TaxaId 1
#define def_fileErr_k2TaxaId 2
#define def_noIds_k2TaxaId 4

#define def_mergeUpBl_k2TaxaId ((signed char) 1)
#define def_mergeDownBl_k2TaxaId ((signed char) 2)
#define def_skip_k2TaxaId ((signed char) 4)
#define def_lowDepth_k2TaxaId ((signed char) 8)
    /*mark as low read depth, for print lowest level*/

/*-------------------------------------------------------\
| ST01: taxa_k2TaxaId
|   - has taxa names and ids to extract by
\-------------------------------------------------------*/
typedef struct taxa_k2TaxaId
{
   signed long *codeArySL;    /*taxa codes for oragnisms*/
   struct str_ptrAry *idAryST; /*has taxa names*/

   signed long *backTrackArySL;
      /*has previous taxanomic rank for backtracking*/
   signed char *mergeAryBl; /*1: is merge taxa*/

   signed short *levelArySS;/*position of taxa*/

   /*these are for quick look ups*/
   signed long *indexArySL; /*index of codeArySL in sort*/

   signed long numTaxaSL;  /*number detected taxa*/
   signed long sizeTaxaSL; /*size of all arrays*/
}taxa_k2TaxaId;

/*-------------------------------------------------------\
| Fun01: blank_taxa_k2TaxaId
|   - blanks a taxa_k2TaxaId structure
| Input:
|   - taxaSTPtr:
|     o pointer to taxa_k2TaxaId struct to blank
| Output:
|   - Modifies:
|     o nothing; just here for future poofing
\-------------------------------------------------------*/
void
blank_taxa_k2TaxaId(
   struct taxa_k2TaxaId *taxaSTPtr
);

/*-------------------------------------------------------\
| Fun02: init_taxa_k2TaxaId
|   - initializes a taxa_k2TaxaId structure
| Input:
|   - taxaSTPtr:
|     o pointer to taxa_k2TaxaId to initiazlize
| Output:
|   - Modifies:
|     o sets everything in taxaSTPtr to 0
\-------------------------------------------------------*/
void
init_taxa_k2TaxaId(
   struct taxa_k2TaxaId *taxaSTPtr
);

/*-------------------------------------------------------\
| Fun03: freeStack_taxa_k2TaxaId
|   - frees variables in a taxa_k2TaxaId structure
| Input:
|   - taxaSTPtr:
|     o pointer to taxa_k2TaxaId with vars to free
| Output:
|   - Frees:
|     o all variables in taxaSTPtr (sets vars to 0)
\-------------------------------------------------------*/
void
freeStack_taxa_k2TaxaId(
   struct taxa_k2TaxaId *taxaSTPtr
);

/*-------------------------------------------------------\
| Fun04: freeHeap_taxa_k2TaxaId
|   - frees a taxa_k2TaxaId structure
| Input:
|   - taxaSTPtr:
|     o pointer to taxa_k2TaxaId to free
| Output:
|   - Frees:
|     o taxaSTPtr (you must set to null (0))
\-------------------------------------------------------*/
void
freeHeap_taxa_k2TaxaId(
   struct taxa_k2TaxaId *taxaSTPtr
);

/*-------------------------------------------------------\
| Fun05: setup_taxa_k2TaxaId
|   - allocates memory for a taxa_k2TaxaId struc
| Input:
|   - taxaSTPtr:
|     o pointer to taxa_k2TaxaId to setup memory
|   - numElmSL:
|     o number of elements to allocate memory for
| Output:
|   - Modifies:
|     o all arrays in taxaSTPtr to have memory
|   - Returns:
|     o 0 for no errors
|     o def_memErr_k2TaxaId for memory errors
\-------------------------------------------------------*/
signed char
setup_taxa_k2TaxaId(
   struct taxa_k2TaxaId *taxaSTPtr,
   signed long numElmSL
);

/*-------------------------------------------------------\
| Fun06: realloc_taxa_k2TaxaId
|   - reallocates memory for a taxa_k2TaxaId struc
| Input:
|   - taxaSTPtr:
|     o pointer to taxa_k2TaxaId to resize
|   - numElmSL:
|     o number of elements to resize to
| Output:
|   - Modifies:
|     o all arrays in taxaSTPtr to be realloced
|   - Returns:
|     o 0 for no errors
|     o def_memErr_k2TaxaId for memory errors
\-------------------------------------------------------*/
signed char
realloc_taxa_k2TaxaId(
   struct taxa_k2TaxaId *taxaSTPtr,
   signed long numElmSL
);

/*-------------------------------------------------------\
| Fun07: sortCodes_taxa_k2TaxaId
|   - sorts the sortCodeArySL array while keeping the
|     indexArySL in sync a taxa_k2TaxaId
| Input:
|   - taxaSTPtr:
|     o pointer to taxa_k2TaxaId to sort
| Output:
|   - Modifies:
|     o sortCodeArySL to be sorted lowest to greatest
|     o indexArySL to be in sync with sortCodeArySL
|       - idea: index is index to unsorted arrays
\-------------------------------------------------------*/
void
sortCodes_taxa_k2TaxaId(
   struct taxa_k2TaxaId *taxaSTPtr
);

/*-------------------------------------------------------\
| Fun08: findCode_taxa_k2TaxaId
|  - returns index of taxa code, searches sorted list
|    and returns unsorted list index
| Input:
|   - codeSL:
|     o taxa code to search for
|   - taxaSTPtr:
|     o pointer to taxa_k2TaxaId to search for code
| Output:
|  - Returns:
|    o index of taxa code
|    o -1 if taxa code not in taxaSTPtr
\-------------------------------------------------------*/
signed long
findCode_taxa_k2TaxaId(
   signed long codeSL,
   struct taxa_k2TaxaId *taxaSTPtr
);

/*-------------------------------------------------------\
| Fun09: getLevel_k2TaxaId
|   - get the level in heirchyy of a kraken entry (col 4)
| Input:
|   - taxaStr:
|     o string with taxa assignment to get number for
| Output:
|   - Returns:
|     - taxa numuber + sub-taxa level (ex: for D1; D + 1)
|       o unclassified def_unclassifed_k2TaxaId; no level
|       o root def_root_k2TaxaId + level
|       o domain def_domain_k2TaxaId + level
|       o phylum def_phylum_k2TaxaId + level
|       o class def_class_k2TaxaId + level
|       o order def_order_k2TaxaId + level
|       o family def_family_k2TaxaId + level
|       o genus def_genus_k2TaxaId + level
|       o species def_species_k2TaxaId + level
\-------------------------------------------------------*/
signed short
getLevel_k2TaxaId(
   signed char *taxaStr
);

/*-------------------------------------------------------\
| Fun10: readReport_k2TaxaId
|   - gets codes of organisims for read ids print otu
| Input:
|   - startLevSS:
|     o lowest level (nearest root) to start printing for
|     o use getLevel_k2TaxaId to get this
|   - endLevSS:
|     o highest level (nearest tip) to start printing for
|     o use getLevel_k2TaxaId to get this
|   - minDepthUL:
|     o min read depth to keep an id (taxa)
|   - minPercDepthF:
|     o min percent read depth (0 to 100) to keep an id
|   - miniRepBl:
|     o 1: report is kraken2 minizer report (unique)
|     o 0: report is normal kraken2  report
|   - mergeRootBl:
|     o 1: merge lower tree (root) levels with their
|          upper (tip) levels
|     o 0: only keep ids mapping to its level
|   - mergeTipBl:
|     o 1: merge closer to tip not-targted taxa or low
|       depth taxa
|     o 0: do not merge
|   - errSCPtr:
|     o pointer to signed char to have errors
|   - inFILE:
|     o FILE pointer to kraken2 tsv report with taxons
|       to extract
| Output:
|   - Modifies:
|     o errSCPtr:
|       - 0 for no errors
|       - def_memErr_k2TaxaId for memory errors
|       - def_fileErr_k2TaxaId for file errors
|       - def_noIds_k2TaxaId if no id's in file
|   - Returns:
|     o taxa_k2TaxaId with list of taxon ids and names
|       to extract
|       - negative numbers in codeArySL denote merged
|         tips (! pScrictBl) or for pScrictBl, taxa to
|         ignore
|     o 0 for no ids or error
\-------------------------------------------------------*/
struct taxa_k2TaxaId *
readReport_k2TaxaId(
   signed short startLevSS,     /*lowest level to print*/
   signed short endLevSS,       /*highest level to print*/
   unsigned long minDepthUL,    /*min depth to keep id*/
   float minPercDepthF,         /*min % depth to keep id*/
   signed char miniRepBl,       /*1: k2 minimizer report*/
   signed char mergeRootBl,     /*1: do not merge levels*/
   signed char mergeTipBl,      /*1: merge lower reads*/
   signed char *errSCPtr,
   void *inFILE                 /*kraken2 report*/
);

/*-------------------------------------------------------\
| Fun12: pIds_k2TaxaId
|   - prints out read ids by taxa for kraken2
| Input:
|   - taxaSTPtr:
|     o pointer to taxa_k2TaxaId struct with taxa codes
|       to extract/merge
|   - prefixStr:
|     o c-string with prefix to name files with
|   - pUnclassBl:
|     o 1: print unclassifed reads
|     o 0: do not print unclassifed reads
|   - inFILE:
|     o FILE pointer to kraken2 tsv output with read ids
|       and their taxnomic assignments
| Output:
|   - Prints:
|     o reads to files named "prefixStr-code-taxa.ids"
|       were code is taxa code of file and taxa is taxa
|       name for code
|   - Returns:
|     o 0 for no errors
|     o 1 + line on for file errors
\-------------------------------------------------------*/
signed long
pIds_k2TaxaId(
   struct taxa_k2TaxaId *taxaSTPtr,
   signed char *prefixStr,
   signed char pUnclassBl,
   void *inFILE
);

#endif

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
