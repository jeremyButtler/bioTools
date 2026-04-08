/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' getSeq SOF: Start Of File
'   - holds functions used to process sequence files,
'     such as fastq, fasta, and sam files
'   o fun01: pId_getSeq
'     - prints or adds ouput to buffer
'   o fun02 ulfq_getSeq:
'     - gets next fastq entry and adds current to buffer
'   o fun03 ulsam_getSeq:
'     - gets next sam entry and adds current to out buffer
'   o fun04 ulfqNoBuff_getSeq:
'     - gets next fastq entry and prints to output file
'   o fun05 ulsamNoBuff_getSeq:
'     - gets next sam entry and prints out old sam entry
'       if file provided
'   o license:
'     - licensing for this code (CC0)
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#ifndef GET_SEQUENCE_H
#define GET_SEQUENCE_H

/*-------------------------------------------------------\
| Fun01: pId_getSeq
|   - prints or adds ouput to buffer
| Input:
|   - buffStr:
|     o c-string with buffer to copy contents from
|   - posUL:
|     o end position of copy for buffStr
|   - startPosUL:
|     o start position to copy for buffStr
|   - outBuffStr:
|     o buffer to copy output to
|   - outPosUL:
|     o position to start copy to the output buffer
|   - lenBuffUI:
|     o length of input (buffStr) & output (outBuffStr)
|       buffers
|   - outFILE:
|     o file to print to if outBuffStr is full
| Output:
|   - Modifies:
|     o outPosUL to be at end of copy
|     o startPosUL to be at start of array
|     o outBuffStr to have new contents
|   - Prints:
|     o outBuffStr to outFILE if outBuffStr is full
\-------------------------------------------------------*/
void
pId_getSeq(
   signed char *buffStr,
   unsigned long posUL,
   unsigned long *startPosUL,
   signed char *outBuffStr,
   unsigned long *outPosUL, 
   unsigned int lenBuffUI,
   void *outFILE
);

/*-------------------------------------------------------\
| Fun02 ulfq_getSeq:
|   - gets next fastq entry and adds current to buffer
| Input:
|   - buffStr:
|     o c-string with fastq file buffer to find next entry
|       in or update
|   - posUL:
|     o unsigned long pointer to position at in buffer
|   - outBuffStr:
|     o c-string with sequences to print out
|   - outPosUL:
|     o unsigned long pointer to position at in output
|       buffer
|   - lenBuffUI:
|     o length of input (buffStr) & output (outBuffStr)
|       buffers
|   - bytesUL:
|     o pointer to a unsigned long with the number of
|       bytes in buffer (updated as needed)
|   - fqFILE:
|     o fastq FILE with reads to extract
|   - outFILE:
|     o FILE to print fastq entry to
| Output:
|  - Prints:
|    o Reads to outFILE if outFILE points to a file
|  - Modifies:
|    o buffStr to have the next buffer if empty
|    o posUL to have position of next sequence in buffer
|    o bytesUL to have new number of bytes if need to
|      read in more from fastq file
|    o outBuffStr to have current sequence if outFILE
|      is input
|    o outposUL to have new position at in output buffer
|    o fqFILE to be at next point in file if more
|      file read into buffer
|    o outFILE to have contents of outBuffStr if
|      outBuffStr is full
|  - Returns:
|   o 0 if nothing went wrong
|   o 1 If the end of the file
|   o 4 If ran out of file
\-------------------------------------------------------*/
unsigned char
ulfq_getSeq(
    signed char *buffStr,   /*buffer with input to scan*/
    unsigned long *posUL,   /*Position in buffer*/
    signed char *outBuffStr,/*holds reads to print*/
    unsigned long *outPosUL,/*position at in outBuffStr*/
    unsigned int lenBuffUI, /*Size of buffer*/
    unsigned long *bytesUL, /*Number chars in buffStr*/
    void *fqFILE,           /*Fastq file with input*/
    void *outFILE           /*file to output reads to*/
);

/*--------------------------------------------------------\
| Fun03 ulsam_getSeq:
|   - gets next sam entry and adds current to out buffer
| Input:
|   - buffStr:
|     o c-string with fastq file buffer to find next entry
|       in or update
|   - posUL:
|     o unsigned long pointer to position at in buffer
|   - outBuffStr:
|     o c-string with sequences to print out
|   - outPosUL:
|     o unsigned long pointer to position at in output
|       buffer
|   - lenBuffUI:
|     o length of input (buffStr) & output (outBuffStr)
|       buffers
|   - bytesUL:
|     o pointer to a unsigned long with the number of
|       bytes in buffer (updated as needed)
|   - fqFILE:
|     o fastq FILE with reads to extract
|   - outFILE:
|     o FILE to print fastq entry to
| Output:
|  - Prints:
|    o Reads to outFILE if outFILE points to a file
|  - Modifies:
|    o buffStr to have the next buffer if empty
|    o posUL to have position of next sequence in buffer
|    o bytesUL to have new number of bytes if need to
|      read in more from fastq file
|    o outBuffStr to have current sequence if outFILE
|      is input
|    o outposUL to have new position at in output buffer
|    o fqFILE to be at next point in file if more
|      file read into buffer
|    o outFILE to have contents of outBuffStr if
|      outBuffStr is full
|  - Returns:
|   o 0 if nothing went wrong
|   o 1 If the end of the file
|   o 4 If ran out of file
\--------------------------------------------------------*/
unsigned char
ulsam_getSeq(
    signed char *buffStr,   /*buffer with input to scan*/
    unsigned long *posUL,   /*Position in buffer*/
    signed char *outBuffStr,/*holds reads to print*/
    unsigned long *outPosUL,/*position at in outBuffStr*/
    unsigned int lenBuffUI, /*Size of buffer*/
    unsigned long *bytesUL, /*Number chars in buffStr*/
    void *samFILE,           /*Fastq file with input*/
    void *outFILE           /*file to output reads to*/
);

/*-------------------------------------------------------\
| Fun04 ulfqNoBuff_getSeq:
|   - gets next fastq entry and prints to output file
| Input:
|   - buffStr:
|     o c-string with fastq file buffer to find next entry
|       in or update
|   - posUL:
|     o unsigned long pointer to position at in buffer
|   - lenBuffUI:
|     o length of input (buffStr) & output (outBuffStr)
|       buffers
|   - bytesUL:
|     o pointer to a unsigned long with the number of
|       bytes in buffer (updated as needed)
|   - fqFILE:
|     o fastq FILE with reads to extract
|   - outFILE:
|     o FILE to print fastq entry to (null to not print)
| Output:
|  - Prints:
|    o Reads to outFILE if outFILE points to a file
|  - Modifies:
|    o buffStr to have the next buffer if empty
|    o posUL to have position of next sequence in buffer
|    o bytesUL to have new number of bytes if need to
|      read in more from fastq file
|    o fqFILE to be at next point in file if more
|      file read into buffer
|    o outFILE to have new sequence (if provided)
|  - Returns:
|   o 0 if nothing went wrong
|   o 1 If the end of the file
|   o 4 If ran out of file
\-------------------------------------------------------*/
unsigned char
ulfqNoBuff_getSeq(
    signed char *buffStr,   /*buffer with input to scan*/
    unsigned long *posUL,   /*Position in buffer*/
    unsigned int lenBuffUI, /*Size of buffer*/
    unsigned long *bytesUL, /*Number chars in buffStr*/
    void *fqFILE,           /*Fastq file with input*/
    void *outFILE           /*file to output reads to*/
);

/*--------------------------------------------------------\
| Fun05 ulsamNoBuff_getSeq:
|   - gets next sam entry and prints out old sam entry if
|     file provided
| Input:
|   - buffStr:
|     o c-string with fastq file buffer to find next entry
|       in or update
|   - posUL:
|     o unsigned long pointer to position at in buffer
|   - lenBuffUI:
|     o length of input (buffStr) & output (outBuffStr)
|       buffers
|   - bytesUL:
|     o pointer to a unsigned long with the number of
|       bytes in buffer (updated as needed)
|   - fqFILE:
|     o fastq FILE with reads to extract
|   - outFILE:
|     o FILE to print fastq entry to
| Output:
|  - Prints:
|    o Reads to outFILE if outFILE points to a file
|  - Modifies:
|    o buffStr to have the next buffer if empty
|    o posUL to have position of next sequence in buffer
|    o bytesUL to have new number of bytes if need to
|      read in more from fastq file
|    o fqFILE to be at next point in file if more
|      file read into buffer
|    o outFILE to have contents of outBuffStr if
|      outBuffStr is full
|  - Returns:
|   o 0 if nothing went wrong
|   o 1 If the end of the file
|   o 4 If ran out of file
\--------------------------------------------------------*/
unsigned char
ulsamNoBuff_getSeq(
    signed char *buffStr,   /*buffer with input to scan*/
    unsigned long *posUL,   /*Position in buffer*/
    unsigned int lenBuffUI, /*Size of buffer*/
    unsigned long *bytesUL, /*Number chars in buffStr*/
    void *samFILE,           /*Fastq file with input*/
    void *outFILE           /*file to output reads to*/
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
