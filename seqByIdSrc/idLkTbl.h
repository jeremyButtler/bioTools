/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' idLkTbl SOF: Start Of File
'   - holds look up table to convert read ids to hex
'     values
'   o header:
'     - defined variables and guards
'   o tbl01: hexTblUC_idLkTbl
'     - converts read id character to hex
'   o license:
'     - licensing for this code (CC0)
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*-------------------------------------------------------\
| Header:
|   - defined variables and guards
\-------------------------------------------------------*/

#ifndef READ_ID_LOOK_UP_CONVERSION_TABLE_H
#define READ_ID_LOOK_UP_CONVERSION_TABLE_H

#define def_blankUL_idLkTbl ((unsigned long) -1)
#define def_invsChar_idLkTbl 64

#define def_bitsInUL_idLkTbl (sizeof(unsigned long) << 3)
#define def_elmSizeUL_idLkTbl 4

/*these variables deal with maximum id sizes or number of
`  characters per id
*/
#define def_numElmUL_idLkTbl (def_bitsInUL_idLkTbl / def_elmSizeUL_idLkTbl)

#define def_maxCharInId_idLkTbl ((unsigned char) -1)

#define def_maxLimbs_idLkTbl (def_maxCharInId_idLkTbl / def_numElmUL_idLkTbl)

/*-------------------------------------------------------\
| Tbl01: hexTblUC_idLkTbl
|   - converts read id character to hex
| Returns:
|   - 0 for zero or any non-numeric and non-alphabetic
|       character
|     o the logic is zero is often not at the start while
|       non-alphebetic characters are rare
|   - 1-14 for numbers and letters
|     o there is a lot of overlap, but it should be at
|       most three characters
|   - 15 for f (hex 15)
|     o the long this fills should be set to -1, so
|       unfilled values will also be 15
|   - def_invsChar_idLkTbl for an invisible character
|   - def_invalidChar_idLkTbl for a non-id character
\-------------------------------------------------------*/
static unsigned char hexTblUC_idLkTbl[] =
{
     def_invsChar_idLkTbl, /*000 null*/
     def_invsChar_idLkTbl, /*001 start of heading*/
     def_invsChar_idLkTbl, /*002 start of text*/
     def_invsChar_idLkTbl, /*003 end of text*/
     def_invsChar_idLkTbl, /*004 end of transmission*/
     def_invsChar_idLkTbl, /*005 enquiry*/
     def_invsChar_idLkTbl, /*006 acknowledge*/
     def_invsChar_idLkTbl, /*007 bell*/
     def_invsChar_idLkTbl, /*008 backspace*/
     def_invsChar_idLkTbl, /*009 '\t' tab (horizontal)*/
     def_invsChar_idLkTbl, /*010 '\n' new line*/
     def_invsChar_idLkTbl, /*011 tab vertical*/
     def_invsChar_idLkTbl, /*012 form feed*/
     def_invsChar_idLkTbl, /*013 '\r' carriage reuturn*/
     def_invsChar_idLkTbl, /*014 shift out*/
     def_invsChar_idLkTbl, /*015 shift in*/
     def_invsChar_idLkTbl, /*016 data link escape*/
     def_invsChar_idLkTbl, /*017 device controll one*/
     def_invsChar_idLkTbl, /*018 device controll two*/
     def_invsChar_idLkTbl, /*019 device controll three*/
     def_invsChar_idLkTbl, /*020 device controll four*/
     def_invsChar_idLkTbl, /*021 negative acknowledge*/
     def_invsChar_idLkTbl, /*022 synchronous idle*/
     def_invsChar_idLkTbl, /*023 end transmission block*/
     def_invsChar_idLkTbl, /*024 cancle*/
     def_invsChar_idLkTbl, /*025 end of medium*/
     def_invsChar_idLkTbl, /*026 substitue*/
     def_invsChar_idLkTbl, /*027 escape*/
     def_invsChar_idLkTbl, /*028 file separator*/
     def_invsChar_idLkTbl, /*029 group separator*/
     def_invsChar_idLkTbl, /*030 record separator*/
     def_invsChar_idLkTbl, /*031 unit separator*/
     def_invsChar_idLkTbl, /*032 ' ' space*/
      0,                   /*033 '!'*/
      0,                   /*034 '"'*/
      0,                   /*035 '#'*/
      0,                   /*036 '$'*/
      0,                   /*037 '%'*/
      0,                   /*038 '&'*/
      0,                   /*039 "'"*/
      0,                   /*040 '('*/
      0,                   /*041 ')'*/
      0,                   /*042 '*'*/
      0,                   /*043 '+'*/
      0,                   /*044 ','*/ /*deliminator?*/
      0,                   /*045 '-'*/
      0,                   /*046 '.'*/ /*deliminator?*/
      0,                   /*047 '/'*/

      0,                   /*048 '0'*/
      1,                   /*049 '1'*/
      2,                   /*050 '2'*/
      3,                   /*051 '3'*/
      4,                   /*052 '4'*/
      5,                   /*053 '5'*/
      6,                   /*054 '6'*/
      7,                   /*055 '7'*/
      8,                   /*056 '8'*/
      9,                   /*057 '9'*/
      0,                   /*058 ':'*/ /*Illumina delim*/

      0,                   /*059 ';'*/ /*deliminator?*/
      0,                   /*060 '<'*/
      0,                   /*061 '='*/
      0,                   /*062 '>'*/
      0,                   /*063 '?'*/
      0,                   /*064 '@'*/

     /*Hex*/
     10,                   /*065 'A'*/
     11,                   /*066 'B'*/
     12,                   /*067 'C'*/
     13,                   /*068 'D'*/
     14,                   /*069 'E'*/
     15,                   /*070 'F'*/
      1,                   /*071 'G'*/
      2,                   /*072 'H'*/
      3,                   /*073 'I'*/
      4,                   /*074 'J'*/
      5,                   /*075 'K'*/
      6,                   /*076 'L'*/
      7,                   /*077 'M'*/
      8,                   /*078 'N'*/
      9,                   /*079 'O'*/
     10,                   /*080 'P'*/
     11,                   /*081 'Q'*/
     12,                   /*082 'R'*/
     13,                   /*083 'S'*/
     14,                   /*084 'T'*/
      1,                   /*085 'U'*/
      2,                   /*086 'V'*/
      3,                   /*087 'W'*/
      4,                   /*088 'X'*/
      5,                   /*089 'Y'*/
      6,                   /*090 'Z'*/

     /*specialcharacters [ \ ] ^ _ ` (91 to 96)*/
      0,                   /*091 '['*/
      0,                   /*092 '\'*/
      0,                   /*093 ']'*/
      0,                   /*094 '^'*/
      0,                   /*095 '_'*/
      0,                   /*096 '`'*/

     /*Hex*/
     10,                   /*97 'a'*/
     11,                   /*98 'b'*/
     12,                   /*99 'c'*/
     13,                   /*100 'd'*/
     14,                   /*101 'e'*/
     15,                   /*102 'f'*/
      1,                   /*103 'g'*/
      2,                   /*104 'h'*/
      3,                   /*105 'i'*/
      4,                   /*106 'j'*/
      5,                   /*107 'k'*/
      6,                   /*108 'l'*/
      7,                   /*109 'm'*/
      8,                   /*110 'n'*/
      9,                   /*111 'o'*/
     10,                   /*112 'p'*/
     11,                   /*113 'q'*/
     12,                   /*114 'r'*/
     13,                   /*115 's'*/
     14,                   /*116 't'*/
      1,                   /*117 'u'*/
      2,                   /*118 'v'*/
      3,                   /*119 'w'*/
      4,                   /*120 'x'*/
      5,                   /*121 'y'*/
      6,                   /*122 'z'*/

      0,                   /*123 '{'*/
      0,                   /*124 '|'*/ /*deliminator?*/
      0,                   /*125 '}'*/
      0,                   /*126 '~'*/
      0,                   /*127 'Del'*/
}; /*hexTblUC_idLkTbl*/

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
