#!/bin/awk -f

# marks duplcate specis ids in a getlin complex database

BEGIN{
   OFS="\t";
   getline;
   print $0;
}; # BEGIN

{  # MAIN
   if($1 != idStr && $4 != lineageStr)
   { # If: new lineage
      if(idStr != "")
         print outStr;
      outStr = $0;
      idStr = $1;
      lineageStr = $4;
      cntSI = 0;
   } # If: new lineage

   else
   { # Else: duplicate species
      ++cntSI;
      outStr = outStr "\n" $1 "_" cntSI;
      outStr = outStr "\t" $2 "\t" $3;
      outStr = outStr "\t" $4 "_" cntSI;
      for(siCol = 5; siCol <= NF; ++siCol)
         outStr = outStr "\t" $siCol;
   } # Else: duplicate species
}; # MAIN
