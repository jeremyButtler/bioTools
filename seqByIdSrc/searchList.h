/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' searchList SOF: Start Of File
'   - holds struct and functions for read id multi-file
'     handling
'   o header:
'     - guards and forward declarations
'   o .h st01: file_searchList
'     - holds file name to save to and ids to search for
'   o fun01: blank_file_searchList
'     - blanks a file_searchList structure and all childs
'   o fun02: init_file_searchList
'     - initializes a file_searchList structure
'   o fun03: freeStack_file_searchList
'     - frees variables in a a file_searchList structure
'   o fun04: freeHeap_file_searchList
'     - frees a file_searchList structure
'   o fun05: setup_file_searchList
'     - sets up variables in a file_searchList structure
'   o fun06: realloc_file_searchList
'     - allocates more memory for a file_searchList struct
'   o fun07: swap_file_searchList
'     - swaps two items in a file_searchList arrays
'   o fun08: sort_file_searchList
'     - sort arrays in file_searchList struct by file name
'   o fun09: search_file_searchList
'     - searchs arrays in a file_searchList for file name
'   o fun10: addFile_file_searchList
'     - add file name to a file_searchList struct
'   o fun11: addId_file_searchList
'     - add read id to a file_searchList struct
'   o fun12: hashSetup_file_searchLIst
'     - setsups all hash tables (after added all ids) in a
'       file_searchST struct
'   o fun13: findId_file_searchList
'     - find all files that are assigned the input id in
'       a file_searchList struct
'   o license:
'     - licensing for this code (CC0)
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*-------------------------------------------------------\
| Header:
|   - guards and forward declarations
\-------------------------------------------------------*/

#ifndef SEQ_BY_ID_FILE_SEARCH_LIST_H
#define SEQ_BY_ID_FILE_SEARCH_LIST_H

struct searchST;
struct str_ptrAry;

#define def_memErr_searchList -1
#define def_missing_searchList -2
#define def_fileErr_searchList -4

/*-------------------------------------------------------\
| ST01: file_searchList
|   - holds file name to save to and ids to search for
\-------------------------------------------------------*/
typdef struct file_searchList
{
   struct str_ptrAry *fileAryStr; /*output file array*/
   struct searchST *idAryST;      /*array 

   signed int sizeArySI;          /*array sizes*/
   signed int lenArySI;           /*items in arrays*/
}file_searchList;

/*-------------------------------------------------------\
| Fun01: blank_file_searchList
|   - blanks a file_searchList structure (and all childs)
| Input:
|   - fileSTPtr:
|     o pointer to file_searchList struct to blank
| Output:
|   - Modifies:
|     o fileAryStr to be blanked ('\0') (if has values)
|     o idAryST (not blanked yet [function does nothing])
|     o lenArySI to be 0
\-------------------------------------------------------*/
void
blank_file_searchList(
   struct file_searchList *fileSTPtr
);

/*-------------------------------------------------------\
| Fun02: init_file_searchList
|   - initializes a file_searchList structure
| Input:
|   - fileSTPtr:
|     o pointer to file_searchList struct to initialize
| Output:
|   - Modifies:
|     o fileAryStr in fileSTPtr to be 0
|     o idAryST in fileSTPtr to be 0
|     o lenArySI in fileSTPtr to be 0
|     o sizeArySI in fileSTPTr to be 0
\-------------------------------------------------------*/
void
init_file_searchList(
   struct file_searchList *fileSTPtr
);

/*-------------------------------------------------------\
| Fun03: freeStack_file_searchList
|   - frees variables in a a file_searchList structure
| Input:
|   - fileSTPtr:
|     o pointer to file_searchList with vars to free
| Output:
|   - Modifies:
|     o frees fileAryStr and initializes
|     o frees idAryST and initializes
|     o sets lenArySI to 0
|     o sets sizeArySI to 0
\-------------------------------------------------------*/
void
freeStack_file_searchList(
   struct file_searchList *fileSTPtr
);

/*-------------------------------------------------------\
| Fun04: freeHeap_file_searchList
|   - frees a file_searchList structure
| Input:
|   - fileSTPtr:
|     o pointer to file_searchList with vars to free
| Output:
|   - Modifies:
|     o frees fileSTPtr (you must set to 0/null)
\-------------------------------------------------------*/
void
freeHeap_file_searchList(
   struct file_searchList *fileSTPtr
);

/*-------------------------------------------------------\
| Fun05: setup_file_searchList
|   - sets up variables in a file_searchList structure
| Input:
|   - fileSTPtr:
|     o pointer to file_searchList to set up
|   - sizeSI:
|     o starting array length
|     o changed to 8 if sizeSI is < 1
| Output:
|   - Modifies:
|     o fileAryStr in file_searchList to have sizeSI
|       items
|       - is freeded if already exists (not 0/null)
|     o idAryST in file_searchList to have sizeSI items
|       - is freeded if already exists (not 0/null)
|     o sizeArySI in file_searchList to be sizeSI
|     o lenArySI in file_searchList to be 0
|   - Returns:
|     o 0 for no errors
|     o def_memErr_searchList for memory errors
\-------------------------------------------------------*/
signed char
setup_file_searchList(
   struct file_searchList *fileSTPtr,
   signed int sizeSI
);

/*-------------------------------------------------------\
| Fun06: realloc_file_searchList
|   - allocates more memory for a file_searchList struct
| Input:
|   - fileSTPtr:
|     o pointer to file_searchList to increase array size
|   - sizeSI:
|     o new array length
| Output:
|   - Modifies:
|     o fileAryStr in file_searchList to have sizeSI items
|     o idAryST in file_searchList to have sizeSI items
|     o sizeArySI in file_searchList to be sizeSI
|   - Returns:
|     o 0 for no errors
|     o def_memErr_searchList for memory errors
\-------------------------------------------------------*/
signed char
realloc_file_searchList(
   struct file_searchList *fileSTPtr,
   signed int sizeSI
);

/*-------------------------------------------------------\
| Fun07: swap_file_searchList
|   - swaps two items in a file_searchList arrays
| Input:
|   - fileSTPtr:
|     o pointer to file_searchList with items to swap
|   - firstIndexSI:
|     o index of frist item to swap
|   - secIndexSI:
|     o index of second item to swap
| Output:
|   - Modifies:
|     o swaps fileAryStr and idAryST values at
|       firstIndexSI and secIndexSI
\-------------------------------------------------------*/
void
swap_file_searchList(
   struct file_searchList *fileSTPtr,
   signed int firstIndexSI,
   signed int secIndexSI
);

/*-------------------------------------------------------\
| Fun08: sort_file_searchList
|   - sorts arrays in file_searchList struct by file name
| Input:
|   - fileSTPtr:
|     o pointer to file_searchList to sort
| Output:
|   - Modifies:
|     o fileAryStr in fileSTPtr to be sorted by file name
|     o idAryST in fileSTPtr to be sorted by file name
\-------------------------------------------------------*/
void
sort_file_searchList(
   struct file_searchList *fileSTPtr
);

/*-------------------------------------------------------\
| Fun09: search_file_searchList
|   - searchs arrays in a file_searchList for file name
| Input:
|   - fileStr:
|     o file to find
|   - fileSTPtr:
|     o pointer to file_searchList to search
| Output:
|   - Returns:
|     o index of target file name
| Note:
|   - this assumes things are sorted, which add file
|     always does
\-------------------------------------------------------*/
signed long
search_file_searchList(
   signed char *fileStr,
   struct file_searchList *fileSTPtr
);

/*-------------------------------------------------------\
| Fun10: addFile_file_searchList
|   - add file name to a file_searchList struct
| Input:
|   - fileStr:
|     o c-string with file to append to list
|   - fileSTPtr:
|     o pointer to file_searchList struct to append file
| Output:
|   - Modifies:
|     o fileAryStr to have file appened; resized if needed
|     o idAryST to be resized if fileAryStr is resized
|     o sizeArySI to be resized value if needed to resize
|     o lenArySI to be incurmented
|   - Returns:
|     o 0 for no errors
|     o def_memErr_searchList for memory errors
\-------------------------------------------------------*/
signed char
addFile_file_searchList(
   signed char *fileStr,
   struct file_searchList *fileSTPtr
);

/*-------------------------------------------------------\
| Fun11: addId_file_searchList
|   - add read id to a file_searchList struct
| Input:
|   - idStr:
|     o c-string with read id to add
|   - fileStr:
|     o c-string with file to add read id to
|   - fileSTPtr:
|     o pointer to file_searchList struct to add id to
| Output:
|   - Modifies:
|     o idAryST in fileSTPtr to have read id
|       * idAryUL in idAryST[index] gets new read
|       * lenIdAryUL in idAryST[index] if need more memory
|       * numIdsUL in idAryST[index] is incurmented
|       * numLimbsUL in idAryST[index] to have new end
|       * maxLimbsUC in idAryST[index] if id has more
|         limbs than previous ids
|   - Returns:
|     o 0 for no errors
|     o def_memErr_searchList for memory errors
|     o def_missing_searchList; fileStr not in fileAryStr
| Note:
|   - assumes the file is in fileSTPtr and the file names
|     are already sorted
\-------------------------------------------------------*/
signed char
addId_file_searchList(
   signed char *idStr,               /*read id to add*/
   signed char *fileStr,             /*file with read id*/
   struct file_searchList *fileSTPtr /*add read id to*/
);

/*-------------------------------------------------------\
| Fun12: hashSetup_file_searchLIst
|   - setsups all hash tables (after added all ids) in a
|     file_searchST struct
| Input:
|   - fileSTPtr:
|     o pointer to file_searchList struct to setup hash
| Output:
|   - Modifies:
|     o all elements of idAryST in fileSTPtr to have their
|       hash table setup
|       * all ids in idAryUL to have the same number of
|         limbs (maxLimbsUC)
|       * hashTblUL to have index's of idAryUL (hash tbl)
|       * hashSizeUL to have size of hashTblUL
|       * hashPowUC to have power (2^x) of hash table size
|       * majicUL to have majic number for kunths hash
|   - Returns:
|     o 0 for no errors
|     o def_memErr_searhLIst for memory errors
\-------------------------------------------------------*/
signed char
hashSetup_file_searchList(
   struct file_searchList *fileSTPtr
);

/*-------------------------------------------------------\
| Fun13: findId_file_searchList
|   - find all files that are assigned the input id in
|     a file_searchList struct
| Input:
|   - idAryUL:
|     o pointer to unsigned long with read id
|       - use idToHexAry_maxLimb_searchST to convert
|         it. For limb size use first item in fileSTPtr
|         (fileSTPtr->idAryST[0].maxLimbsUC). If used
|         functions to add files and ids, then all files
|         have same maxLimbsUC.
|   - fileSTPtr:
|     o pointer to file_searchList struct with files to
|       search for id in (as hash table)
|   - errSCPtr:
|     o pointer to signed char to hold error value
| Output:
|   - Modifies:
|     o errSCPtr:
|       * 0 for no errors (no ids or found ids)
|       * def_memErr_searchList for memory errors
|   - Returns:
|     o signed int array with index of found files (you
|       must free) [end of file index's is -1)
|     o 0 if id not found or memory error error
\-------------------------------------------------------*/
signed int *
findId_file_searchList(
   struct searchST *idSearchSTPtr,    /*id to find*/
   struct file_searchList *fileSTPtr, /*has ids + files*/
   signed char *errSCPtr              /*assigned errors*/
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
