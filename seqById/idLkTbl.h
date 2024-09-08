/*########################################################
# Name: idLkTbl
#   - holds look up table to convert read ids to hex
#     values
########################################################*/

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' SOF: Start Of File
'   o header:
'     - defined variables and guards
'   o tbl01: hexTblUC_idLkTbl
'     - converts read id character to hex
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
