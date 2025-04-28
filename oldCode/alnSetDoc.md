- scoreMatrixSS: is the scoring matrix (27x27)
  - each nucleotide is convertred to a index using
    `nucleotide & def_ntToCode_alnST`
- gapSS: is the gap opening (start indel) penalty 
  - use negatives for penalty, positive for no penalty
- extendSS: is the gap extension (continue indel) penalty 
  - use negatives for penalty, positive for no penalty
- delArySS: matrix to return extendSS if last movement
  was a deletion, else gapSS
  - the input is one of four directions from
    ../genAln/alnDefs.h
    - def_mvStop_alnDefs
    - def_mvDel_alnDefs
    - def_mvIns_alnDefs
    - def_mvSnp_alnDefs
- insArySS: matrix to return extendSS if last movement
  was an insertion, else gapSS
  - the input is one of four directions from
    ../genAln/alnDefs.h
    - def_mvStop_alnDefs
    - def_mvDel_alnDefs
    - def_mvIns_alnDefs
    - def_mvSnp_alnDefs

