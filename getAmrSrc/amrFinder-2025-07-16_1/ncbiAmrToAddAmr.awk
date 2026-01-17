BEGIN{OFS="\t"; getline;};
{ # MAIN
  #________________get_the_variant_id________________
  geneStr = $4;
  sub(/_.*/, "_p.", geneStr);

  refStr = $4;
  sub(/.*_/, "", refStr); # remove gene name
  sub(/[0-9][0-9]*/, "", refStr); # remove coordinate

  coordSI = $3;

  mutStr = $4;
  sub(/.*[0-9][0-9]*/, "", mutStr);

  
  if(mutStr ~ /STOP/)
     mutStr = "*";
  else if(mutStr ~ /fs/)
     mutStr = "fs";
  else if(mutStr ~ /del/)
     ; # deletions are in the correct format
  else if(length(refStr) < length(mutStr))
     mutStr = "ins" mutStr;

  #________________get_amrs__________________________
  if($7 ~ /\//)
  { # Else If: AMR is resistance to multiple drug
     drugLenSI = split($7, drugAryStr, "\/");
  } # Else If: AMR is resistance to multiple drug

  else
  { # Else: resistance to only one drug resistance
     drugLenSI = 1;
     drugAryStr[1] = $7;
  } # Else: resistance to only one drug resistance

  #________________print_the_variant_id______________
  printf " -var\t%s%s%s%s\t-ref\t%s",
         geneStr,
         refStr,
         coordSI,
         mutStr,
         $2;
  for(siDrug = 1; siDrug <= drugLenSI; ++siDrug)
     printf " -other %s", drugAryStr[siDrug];
  printf "\n";
}; # MAIN

#_____________________OLD_________________________________
#BEGIN{OFS="\t"; getline;};
#
#{ # MAIN
#  geneStr = $4;
#  sub(/_.*/, "_p.", geneStr);
#
#  refStr = $4;
#  sub(/.*_/, "", refStr); # remove gene name
#  sub(/[0-9][0-9]*.*/, "", refStr); # remove coordinate
#
#  coordSI = $3;
#
#  mutStr = $4;
#  sub(/.*[0-9][0-9]*/, "", mutStr);
#
#  
#  if(mutStr ~ /STOP/)
#     mutStr = "*";
#  else if(mutStr ~ /fs/)
#     mutStr = "fs";
#  else if(mutStr ~ /del/)
#     ; # deletions are in the correct format
#  else if(length(refStr) < length(mutStr))
#     mutStr = "ins" mutStr;
#
#  print "-var", geneStr refStr coordSI mutStr,
#        "-ref", $2;
#}; # MAIN
