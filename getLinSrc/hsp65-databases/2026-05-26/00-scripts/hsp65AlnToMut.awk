BEGIN{OFS="\t";};

{ # MAIN
   if($2 == "qry:")
   { # If: on the query
      if(speciesStr != "")
      { # If: new sequence
         shortStr = substr(speciesStr, 1, 5);
         printf "%s\t%s\t%s\n",
                shortStr,
                snpsStr,
                speciesStr;
      } # If: new sequence

      speciesStr = $3;
      sub(/-.*/, "", speciesStr); # remove accession
      sub("M_", "", speciesStr);  # isolate species
      snpsStr = "";
   } # If: on the query

   else if($1 == "qry:")
      split($3, seqStr, "");
   else if($1 == "ref:")
      posSI = $2;
   else if($1 == "cig:")
   { # Else If: have the cigar entry
      lenSI = split($2, cigStr, "");
      siNt = 0;

      for(siCig = 1; siCig <= lenSI; ++siCig)
      { # Loop: track mutations
         if(cigStr[siCig] == "M")
            ++siNt;
         else if(cigStr[siCig] == "X")
         { # Else If: have a mismatch
            if(seqStr[siCig] != "N")
            { # If: not an anonymous base
               mutPosSI = posSI + siNt + 431 - 528787;
                  # 528787 is base 431 in hsp65

               if(mutPosSI >= 431 && mutPosSI <= 806)
               { # If: is my primer target bases
                  snpsStr = snpsStr "\t" mutPosSI "_";
                  snpsStr = snpsStr seqStr[siCig];
               } # If: is my primer target bases
            } # If: not an anonymous base

            ++siNt;
         } # Else If: have a mismatch
      } # Loop: track mutations
   } # Else If: have the cigar entry
} # MAIN
