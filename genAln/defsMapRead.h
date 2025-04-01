/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' defsMapRead SOF: Start Of File
'   - default values for mapRead
'   o header:
'     - gaurds
'   o sec01:
'     - kmer scan variables
'   o sec02:
'     - chaining and scoring variables
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*-------------------------------------------------------\
| Header:
|   - gaurds
\-------------------------------------------------------*/

#ifndef DEFAULTS_MAP_READ_H
#define DEFAULTS_MAP_READ_H

/*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
^ Sec01:
^   - kmer scan variables
\<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

/*kmer scan defaults*/
#define def_minLen_defsMapRead 0
#define def_secLen_defsMapRead 100
#define def_thirdLen_defsMapRead 10000

#define def_minKmer_defsMapRead 5
#define def_secKmer_defsMapRead 10
#define def_thirdKmer_defsMapRead 15

#define def_minChain_defsMapRead 1
#define def_secChain_defsMapRead 1
#define def_thirdChain_defsMapRead 3


/* What these defaults mean:
` - my logic is keep is small for small references, then
`   expand it large for larger references
` - min: < 100
`   - 5mers; kmer space is 1024
`   - chains are one kmer long
` - two: 100 to 399:
`   - 10mers, which should reduce matches, but might still
`     hit regions with high matches
`   - chains are at least 1 kmer (10 bases) long
` - third: > 399
`   - 15mers
`   - chains are at least 1 kmer (15 bases) long
*/

/*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
^ Sec02:
^   - chaining and scoring variables
\<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

#define def_minScore_defsMapRead 0.4f
    /*min % score to keep alignment*/
#define def_minMatch_defsMapRead 0.0f
   /*at least 80% matches to keep read*/
#define def_minPercLen_defsMapRead 0.2f
    /*min % of query length mapped*/
#define def_chainMinLen_defsMapRead 0.20f
   /*min % query bases in merged chain*/
#define def_maxPercLen_defsMapRead 0.25f
    /*maximum gap length in percent query length to merge
    `  chains at
    */

#define def_alnEnd_defsMapRead 1 /*align softmasked ends*/
#define def_subAln_defsMapRead 1 /*keep best sub-aln*/
#define def_chainGap_defsMapRead -1
  /*gap score for chaining step*/
#define def_chainMatch_defsMapRead 5
  /*match score for chaining step*/

/*the Needleman/Waterman matrix's can be more complex,
`  however, in this case keep same as core.
`  Also, their scores are only used in gap filling,
`  not in the final score, see alnDefs for the defaults
*/

#endif
