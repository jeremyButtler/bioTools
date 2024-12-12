BEGIN{OFS="\t"};

{ # MAIN
   if($0 !~ /^>/)
      next; # sequence entry

   sub(/.*\|/, "", $1);

   if($1 ~ /_gene_/)
   { # If: may have gene number
      geneStr = $1;
      sub(/.*_gene_/, "", geneStr);
      sub(/[A-Za-z_].*/, "", geneStr);
   } # If: may have gene number

   else if($1 ~ /_WP_/)
   { # Else If: have protein id
      geneStr = $1;
      sub(/.*_WP_/, "", geneStr);
      sub(/[A-Za-z_].*/, "", geneStr);
      sub(/\.[0-9].*/ "", geneStr);
   } # Else If: have protein id

   sub(/_cds_.*/, "", $1);
   sub(/\.[0-9].*/, "", $1);

   for(siCol = 1; siCol <=NF; ++siCol)
   { # Loop: find locus tags
      if($siCol ~ /\[gene=/)
      { # If: found gene entry
         sub(/\[gene=/, "", $siCol);
         sub(/\]/, "", $siCol);
         geneStr = $siCol;
      } # If: found gene entry

      if($siCol ~ /\[location=/)
      { # If: found coordinates
         sub(/\[location=/, "", $siCol);
         sub(/\]/, "", $siCol);

         # coordiantes are always in forward direction
         if($siCol ~ /complement/)
         { # If: reverse complement
            sub(/complement\(/, "", $siCol);
            dirBl = "R";
         } # If: reverse complement

         else
            dirBl = "F";

         sub(/\.\./, "\t", $siCol);

         if($siCol ~ /join/)
         { # If: mering cooriantes (circular issue)
            sub(/join./, "", $siCol);
            sub(/,.*/, "", $siCol);
         } # If: mering cooriantes (circular issue)

         gsub(/\)/, "", $siCol);
         gsub(/[<>]/, "", $siCol);
         posStr = $siCol;
      } # If: found coordinates
   } # Loop: find locus tags

   if(geneStr == "")
      geneStr = "NA";

   print geneStr, $1, dirBl, posStr;
} # MAIN
