/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' runPrim.c SOF: Start Of File
'   - holds the functions to run prim.c
'   o header:
'     - guards for people how include .h files in .h files
'   o fun01: blank_mst_runPrim
'     - lazely blanks a mst_runPrim structure
'   o .c fun02: init_mst_runPrim
'     - initialize values in a mst_runPrim struct
'   o fun03: freeStack_mst_runPrim
'     - frees the variables in a mst_runPrim struct
'   o fun04: freeHeap_mst_runPrim
'     - free a mst_runPrim struct that is on the heap
'   o .c fun05: setupMem_mst_runPrim
'     - adds the initial memory to a runPrim structure
'   o .c fun06: mk_mst_runPrim
'     - make a mst_runPrim structure and allocate memory
'   o .c fun07: addName_mst_runPrim
'     - adds name to name array in a mst_runPrim struct
'   o fun08: indexFeatureFile_runPrim
'     - index a feature file for prims to get each nodes
'       location, also get the feature names
'   o fun09: build_mst_runPrim
'     - run prim using a file without keeping a distance
'       matrix (less memory, but one tree build)
'   o fun10: mstToNewick_mst_runPrim
'     - print the tree in a mst_runPrim struct as a newick
'       file
'   o license:
'     - licensing for this code (CC0)
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*-------------------------------------------------------\
| Header:
|   - guards for people how include .h files in .h files
\-------------------------------------------------------*/

#ifndef RUN_PRIMS_ALGORITHM_FROM_PRIM_C_H
#define RUN_PRIMS_ALGORITHM_FROM_PRIM_C_H

struct heap_prim;

/*-------------------------------------------------------\
| ST01: mst_runPrim
|   - holds the mst tree built by the run functions
\-------------------------------------------------------*/
typedef struct
mst_runPrim
{
   struct heap_prim *mstST;   /*has mst tree*/
   signed char **nameAryStr;  /*node names for newick*/
   signed int *distArySI;  /*distance row or spare array*/
   signed long *indexArySL;   /*index of all lines*/

   signed long nodeLenSL;    /*number nodex in tree*/
   signed long nodeSizeSL;   /*nodes in structure*/
   signed long maxLineLenSL; /*max line length in file*/
}mst_runPrim;

/*-------------------------------------------------------\
| Fun01: blank_mst_runPrim
|   - lazely blanks a mst_runPrim structure
| Input:
|   - mstSTPtr:
|     o mst_runPrim struct pointer to blank
| Output:
|   - Modifies:
|     o mstST in mstSTPtr to be blanked
\-------------------------------------------------------*/
void
blank_mst_runPrim(
   struct mst_runPrim *mstSTPtr
);

/*-------------------------------------------------------\
| Fun03: freeStack_mst_runPrim
|   - frees the variables in a mst_runPrim struct
| Input:
|   - mstSTPtr:
|     o mst_runPrim struct pionter with variables to free
| Output:
|   - Frees:
|     o all variables in mst_runPrim and sets to 0/null
\-------------------------------------------------------*/
void
freeStack_mst_runPrim(
   struct mst_runPrim *mstSTPtr
);

/*-------------------------------------------------------\
| Fun04: freeHeap_mst_runPrim
|   - free a mst_runPrim struct that is on the heap
| Input:
|   - mstSTPtr:
|     o struct mst_runPrim pointer to free
| Output:
|   - Frees:
|     o frees the mst_runPrim structure, but you must set
|       to null
\-------------------------------------------------------*/
void
freeHeap_mst_runPrim(
   struct mst_runPrim *mstSTPtr
);

/*-------------------------------------------------------\
| Fun08: indexFeatureFile_runPrim
|   - index a feature file for prims to get each nodes
|     location, also get the feature names
| Input:
|   - featureFILE:
|     o FILE pointer to feature file to index
|   - errSCPtr:
|     o signed char pointer to get errors
| Output:
|   - Modifies:
|     o featureFILE to move to end and then be set to
|       index 0
|     o errSCPtr to have the number of lines or errors
|       * 0 no errors
|       * -1: memory error
|       * 1: empty file or to few features
|   - Returns:
|     o mst_runPrim struct pointer with file index's and
|       the names for each entry
|       * nameAryStr skips the header
|       * nodeLenSL includes the header
|     o 0 for an error
\-------------------------------------------------------*/
struct mst_runPrim *
indexFeatureFile_runPrim(
   void *featureFILE,
   signed char *errSCPtr
);

/*-------------------------------------------------------\
| Fun09: build_mst_runPrim
|   - run prim using a file without keeping a distance
|     matrix (less memory, but one tree build)
| Input:
|   - featureFILE:
|     o FILE pointer to file with features
|   - firstNodeSI:
|     o node number to start building the tree with
|       * value 0 is the first node in the tree
|       * max value is mstSTPtr->nodeLenSL - 2
|   - mstSTPtr
|     o mst_runPrim struct pointer with the index for
|       the file (returned from indexFeatureFile_runPrim)
| Output:
|   - Modifies;
|     o mstST in mstSTPtr to have minimum spanning tree
|     o distArySI in mstSTPtr to have distances from the
|       last read row
|   - Returns:
|     o 0 for no errors
|     o 1 if firstNodeSI is not in the tree
|     o 2 for memory errors
|     o (1 + lineSL) * -1 for file errors
\-------------------------------------------------------*/
signed long
build_mst_runPrim(
   void *featureFILE,
   signed int firstNodeSI,      /*entry to build tree*/
   struct mst_runPrim *mstSTPtr
);

/*-------------------------------------------------------\
| Fun10: mstToNewick_mst_runPrim
|   - print the tree in a mst_runPrim struct as a newick
|     file
| Input:
|   - mstSTPtr:
|     o mst_runPrim struct with the tree to print
|   - outFILE:
|     o FILE pointer to file to print to
| Output:
|   - Prints:
|     o minimum spanning tree in mstSTPtr to outFILE
|   - Modifies:
|     o distArySI in mstSTPtr to have values 0 to number
|       of nodes (this array is a temporary array, so not
|       a big change)
\-------------------------------------------------------*/
void
mstToNewick_mst_runPrim(
   struct mst_runPrim *mstSTPtr,
   void *outFILE
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
