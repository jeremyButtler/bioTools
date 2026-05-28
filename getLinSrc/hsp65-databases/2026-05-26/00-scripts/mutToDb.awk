#!/bin/awk -f

#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# mutToDb.awk SOF:
#   - converts the hsp65 mutation table to a set of getLin
#     databases
#   o sec01:
#     - setup and read in TB bases
#   o sec02:
#     - read in the non-TB mutations
#   o sec03:
#     - print out the simple and complex databases
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

##########################################################
# Name: mutToDb.awk
#   - converts a hsp65 mutation list set to getLin
#     databases
# Input:
#   -v prefixStr="$<value>": prefix to name the output
#      files with
#   - mutations.tsv: file with mutations
#     o format: name\tmutation_1\tmutation_2\tmutation_3\t
#               ...\tname
# Output:
#   - saves complex database to prefixStr-complex.tsv
#   - saves simple database to prefixStr-simple.tsv
##########################################################

#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# Sec01:
#   - setup and read in TB bases
#   o sec01 sub01:
#     - set and initialize hit arrays
#   o sec01 sub02:
#     - read in TB sequence and set up its entry
#<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

#*********************************************************
# Sec01 Sub01:
#   - set and initialize hit arrays
#*********************************************************

BEGIN{
   OFS="\t";
   numSeqSI = 0;

   if(prefixStr == "")
      prefixStr = "out";
   simpleDbStr = prefixStr "-simple.tsv";
   complexDbStr = prefixStr "-complex.tsv";

   # blank the
   for(siNt = 1; siNt <= 376; ++siNt)
   { # Loop: set hit array to 0
      ntArySC[posSI, 1] = 0; # marks if have a mutation
      ntArySC[posSI, 2] = 0; # A count
      ntArySC[posSI, 3] = 0; # C count
      ntArySC[posSI, 4] = 0; # G count
      ntArySC[posSI, 5] = 0; # T count
   } # Loop: set hit array to 0

   #******************************************************
   # Sec01 Sub02:
   #   - read in TB sequence and set up its entry
   #******************************************************

   getline; # get TB hsp65 line

   ++numSeqSI;
   outAryStr[numSeqSI] = "TB" "\thsp65" "\thsp65\t" "TB";
   outAryStr[numSeqSI] = outAryStr[numSeqSI] "\t10";
   outAryStr[numSeqSI] = outAryStr[numSeqSI] "\tall";
   outAryStr[numSeqSI] = outAryStr[numSeqSI] "\tTrue";
   outAryStr[numSeqSI] = outAryStr[numSeqSI] "\tclosest";
   outAryStr[numSeqSI] = outAryStr[numSeqSI] "\t*";

   for(siMut = 2; siMut < NF; ++siMut)
   { # Loop: read in TB bases
      tbArySC[siMut - 1] = $siMut; # get TB hsp65 bases
      mutAryStr[numSeqSI, siNt] = 0;
   } # Loop: read in TB bases
}; # BEGIN

#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# Sec02:
#   - read in the non-TB mutations
#   o sec02 sub01:
#     - set up the sequences mutations list and the first
#       part of its complex database entry
#   o sec02 sub02:
#     - read in mutations and assign to sequence and also
#       keep track of total time each mutation happens
#<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

#*********************************************************
# Sec02 Sub01:
#   - set up the sequences mutations list and the first
#     part of its complex database entry
#*********************************************************

{ # MAIN
   if(NF < 3)
      next; # no mutations

   ++numSeqSI;
   outAryStr[numSeqSI] = $1 "\thsp65" "\thsp65\t" $NF;
   outAryStr[numSeqSI] = outAryStr[numSeqSI] "\t10";
   outAryStr[numSeqSI] = outAryStr[numSeqSI] "\tall";
   outAryStr[numSeqSI] = outAryStr[numSeqSI] "\tTrue";
   outAryStr[numSeqSI] = outAryStr[numSeqSI] "\tclosest";
   outAryStr[numSeqSI] = outAryStr[numSeqSI] "\t*";

   # blank the array keeping track of the sequences
   #   mutations
   for(siNt = 1; siNt <= 376; ++siNt)
      mutAryStr[numSeqSI, siNt] = 0;

   #******************************************************
   # Sec02 Sub02:
   #   - read in mutations and assign to sequence and also
   #     keep track of total time each mutation happens
   #******************************************************

   for(siMut = 2; siMut < NF; ++siMut)
   { # Loop: read in mutations
      posSI = $siMut;
      sub(/_./, "", posSI);
      posSI -= 430;
      mutAryStr[numSeqSI, posSI] = $siMut;

      baseSC = $siMut;
      sub(/[0-9]*_/, "", baseSC);

      if(baseSC == "A")
         baseSC = 2;
      else if(baseSC == "C")
         baseSC = 3;
      else if(baseSC == "G")
         baseSC = 4;
      else if(baseSC == "T")
         baseSC = 5;
      ++ntArySC[posSI, 1];
      ++ntArySC[posSI, baseSC];
   } # Loop: read in mutations
}; # MAIN

#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# Sec03:
#   - print out the simple and complex databases
#   o sec03 sub01:
#     - print the simple database header
#   o sec03 sub02:
#     - fill in complex database entries + start loop
#   o sec03 sub03:
#     - print the simple databse
#   o sec03 sub04:
#     - print the complex databse
#<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<


#*********************************************************
# Sec03 Sub01:
#   - print the simple database header
#*********************************************************

END{
   print "gene",
         "group",
         "id",
         "start",
         "end",
         "molecule",
         "type",
         "direction",
         "base",
         "lineage",
         "print_lineage",
         "fudge",
         "min_score",
         "max_trs_gap" > simpleDbStr;

   #******************************************************
   # Sec03 Sub02:
   #   - fill in complex database entries + start loop
   #******************************************************

   # print the simple and complex database
   for(siNt = 1; siNt <= 376; ++siNt)
   { # Loop: print out mutations
      if(ntArySC[siNt, 1] == 0)
         continue;

      for(siSeq = 1; siSeq <= numSeqSI; ++siSeq)
      { # Loop: fill in complex database entries
         if(mutAryStr[siSeq, siNt] == 0)
            outAryStr[siSeq] = outAryStr[siSeq] "\t" tbArySC[siNt];
         else
            outAryStr[siSeq] = outAryStr[siSeq] "\t" mutAryStr[siSeq, siNt];
      } # Loop: fill in complex database entries

      #***************************************************
      # Sec03 Sub03:
      #   - print the simple databse
      #***************************************************

      for(siBase = 1; siBase <= 5; ++siBase)
      { # Loop: print out the simple database
         if(siBase == 1)
         { # If: getting reference base
            baseSC = tbArySC[siNt];
            sub(/.*_/, "", baseSC);
         } # If: getting reference base

         if(siBase == 2)
            baseSC = "A";
         else if(siBase == 3)
            baseSC = "C";
         else if(siBase == 4)
            baseSC = "G";
         else if(siBase == 5)
            baseSC = "T";

         if(ntArySC[siNt, siBase] != 0)
         { # If: a species supports this hsp65 mutation
            print "hsp65",
                  "hsp65",
                  siNt + 430 "_" baseSC,
                  siNt + 528786,
                  siNt + 528786,
                  "nt",
                  "snp",
                  "Forward",
                  baseSC,
                  siNt + 430 "_" baseSC,
                  "False",
                  0,
                  0,
                  0 > simpleDbStr;
         } # If: a species supports this hsp65 mutation
      } # Loop: print out the simple database
   } # Loop: print out mutations

   #******************************************************
   # Sec03 Sub04:
   #   - print the complex databse
   #******************************************************

   print "id",
         "group",
         "gene",
         "lineage",
         "fudge",
         "overwrites",
         "print_lineage",
         "type",
         "variant_start" > complexDbStr;

   for(siSeq = 1; siSeq <= numSeqSI; ++siSeq)
      print outAryStr[siSeq] > complexDbStr;
}; # END
