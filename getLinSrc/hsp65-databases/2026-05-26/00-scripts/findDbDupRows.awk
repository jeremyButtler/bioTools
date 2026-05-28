#!/bin/awk -f

# finds duplicates rows in the getlin complex database

{ # MAIN
   ++cntSI;
   idAryStr[cntSI] = $4;

   mutAryStr[cntSI] = $10;
   for(siCol = 11; siCol <= NF; ++siCol)
      mutAryStr[cntSI] = mutAryStr[cntSI] "\t" $siCol;
}; # MAIN

END{
   for(siSeq = 1; siSeq <= cntSI; ++siSeq)
   { # Loop: find duplicates
      for(siRow = siSeq + 1; siRow <= cntSI; ++siRow)
      { # Loop: compare two sequences
         if(mutAryStr[siRow] == mutAryStr[siSeq])
            print idAryStr[siSeq], idAryStr[siRow];
      }; # Loop: compare two sequences
   }; # Loop: find duplicates
}; # MAIN

