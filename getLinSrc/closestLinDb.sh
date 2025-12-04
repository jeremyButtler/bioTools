#!/bin/sh

#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# closestLinDb.sh SOF: Start Of File
#   - build the databaes for getLin that uses a closest
#     lineage databas
#   o sec01:
#     - variables (user input)
#   o sec02:
#     - get and check user input
#   o sec03:
#     - convert the markdown file to a database
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# Sec01:
#   - variables (user input)
#<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

fileStr=""; # user needs to input this

groupStr="no-group"; # usefull metadate, ok if not present
geneStr="no-gene";   # usefull metadate, ok if not present
dirStr="Forward";    # always assume forward direction
offsetSI=0;          # no offset
outStr="out";
fudgeSI=10;          # 10 bases to fudge by

helpStr="$(basename "$0") -file file.md -gene gene_name
   - convert a markdown table into a colosest getLin
     database
Input:
   -md file.md
      o markdown table to convert to the simple and
        complex getLin database
        - Pipe table, the table starts after a line
          starting with '<!--START-->'
        - Pipe table ends at line starting with 'Table:'
        - Header: first two columns ignored, while the
          rest (columns three to end) have the species
          or lineages names
        - Rows after the header
          - first column is the position
          - second column is the nucleotide to match
          - third column and on
            - 'True' or 'T' if the species or lineage
              as this mutation
            - '.', 'F', or 'False' if species or lineage
              does not have the mutation
      o you could easily get away with a tsv file, so
        long as the '<!--START-->' and 'Table:' (end tag)
        tags are present
   -out $outStr:
      o prefix to save the getLin complex and simple
        closest database from
   -offset $offsetSI:
      o Used to convert gene coordinates to reference
        coordiants (offset + gene coordinate = coordinate)
   -fudge $fudgeSI:
      o number of mutations that can be off to consider
        a lineage or species as a potential hit
   -group $groupStr:
      o group name to assign to lineages
   -gene $geneStr:
      o gene to assign to lineages
   -forward [True]:
      o gene is in the forward direction
   -reverse [False]:
      o gene is in the reverse direction
Output:
   - prefix-simple.tsv file with the simple getLin
     database
   - prefix-complex.tsv file with the complex getLin
     database with every variant set to closest
";

#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# Sec02:
#   - get and check user input
#<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

while [ $# -gt 0 ];
do   # Loop: get user input
   if [ "$1" = "-md" ]; then
      shift;
      fileStr="$1";
   elif [ "$1" = "-out" ]; then
      shift;
      outStr="$1";
   elif [ "$1" = "-group" ]; then
      shift;
      groupStr="$1";
   elif [ "$1" = "-gene" ]; then
      shift;
      geneStr="$1";
   elif [ "$1" = "-fudge" ]; then
      shift;
      fudgeSI="$1";
   elif [ "$1" = "-forward" ]; then
      dirStr="Forward";
   elif [ "$1" = "-reverse" ]; then
      dirStr="Reverse";
   elif [ "$1" = "-offset" ]; then
      shift;
      offsetSI="$1";
   elif [ "$1" = "-h" ]; then
      printf "%s\n" "$helpStr";
      exit;
   elif [ "$1" = "--h" ]; then
      printf "%s\n" "$helpStr";
      exit;
   elif [ "$1" = "help" ]; then
      printf "%s\n" "$helpStr";
      exit;
   elif [ "$1" = "-help" ]; then
      printf "%s\n" "$helpStr";
      exit;
   elif [ "$1" = "--help" ]; then
      printf "%s\n" "$helpStr";
      exit;
   fi;

   shift;
done # Loop: get user input

if [ ! -f "$fileStr" ]; then
   if [ "$fileStr" = "" ]; then
      printf "no markdown file input for -md\n";
   else
      printf "could not open -md %s\n", fileStr;
   fi

   exit;
fi;

#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# Sec03:
#   - convert the markdown file to a database
#   o sec03 sub01:
#     - convert markdown documnet to the database for awk
#       to use
#   o sec03 sub02:
#     - get awk input, get species, & print header
#   o sec03 sub03:
#     - print simple database line and build
#       complex variant list for each species
#   o sec03 sub04:
#     - print complex database header and entries
#<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

#*********************************************************
# Sec03 Sub01:
#   - convert markdown documnet to the database for awk
#     to use
#*********************************************************

sed \
    -n \
    '  /Table/q;  # at end of table
       /|:--.*/d; # delete spacer column for table
       /^$/d;     # remove blank lines
       /<!--START-->/,/Table/{
          /<!--START-->/d;  # delete the start line
          s/|//g;           # remove pipes
          p;                # print the line
       }; # move from start to end
    ' \
    "$fileStr" |
  awk \
      -v groupStr="$groupStr" \
      -v geneStr="$geneStr" \
      -v dirStr="$dirStr" \
      -v offsetSI="$offsetSI" \
      -v fudgeSI="$fudgeSI" \
      -v outStr="$outStr" \
      '
         #************************************************
         # Sec03 Sub02:
         #   - get awk input, get species, & print header
         #************************************************

         BEGIN{
            OFS="\t";
            getline; # get the header line
            startSI = 3;
            adjustSI = startSI - 1;
            maxSpeciesSI = NF - startSI + 1;

            if(geneStr == "")
               geneStr = "gene";
            if(groupStr == "")
               groupStr = "out";
            if(dirStr == "")
               dirStr = "Forward";
            if(offsetSI == "")
               offsetSI = 0;
            if(outStr == "")
               outStr = "out";
            if(fudgeSI == "")
               fudgeSI = 0;

            simpleFileStr = outStr "-simple.tsv";
            complexFileStr = outStr "-complex.tsv";

            # get the species name
            for(siCol=startSI; siCol <= NF; ++siCol)
            { # Loop: get species names
               speciesAryStr[siCol - adjustSI] = $siCol;
               lenArySI[siCol - adjustSI] = 0;
            } # Loop: get species names

            # print the header
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
                  "max_trs_gap" > simpleFileStr;
         }; # BEGIN

         #************************************************
         # Sec03 Sub03:
         #   - print simple database line and build
         #     complex variant list for each species
         #************************************************

         { # MAIN
            posSI = $1;
            ntSC = $2;
            idStr = posSI "_" ntSC;
            posSI = posSI + offsetSI;

            print geneStr,    # gene name
                  groupStr,   # group name
                  idStr,      # id to assign the variant
                  posSI,      # starting position
                  posSI,      # ending position
                  "nt",       # nucleotide
                  "snp",      # always an snp
                  dirStr,     # direction
                  ntSC,       # nucleotide to find
                  idStr,      # lineage (not used)
                  "False",    # do not print the lineage
                  fudgeSI,    # no fudge (one snp)
                  0,          # not used for snp matches
                  0 >> simpleFileStr;
                  # not doing tandum repeats

            # add variant ids to each complex lineage
            for(siCol = startSI; siCol <= NF; ++siCol)
            { # Loop: find species supporting variants
               indexSI = siCol - adjustSI;

               if($siCol ~ /^[Tt]/)
               { # Loop: add variant to the species
                  ++lenArySI[indexSI];
                  varIdAryStr[indexSI][lenArySI[indexSI]] = idStr;
               }; # Loop: add variant to the species
            }; # Loop: find species supporting variants
         }; # MAIN

         #************************************************
         # Sec03 Sub04:
         #   - print complex database header and entries
         #************************************************

         END{
            print "id",
                  "group",
                  "gene",
                  "lineage",
                  "fudge",
                  "overwrites",
                  "print_lineage",
                  "type",
                  "variant_start" > complexFileStr;

            for(siCol = 1; siCol <= maxSpeciesSI; ++siCol)
            { # Loop: print complex variants
               printf "%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s",
                      speciesAryStr[siCol],
                      groupStr,
                      geneStr,
                      speciesAryStr[siCol],
                      0,      # let user adjust the fudge
                      "all",  # always overwrite
                      "True", # always print the lineage
                      "closest", # use closest species
                      "*" >> complexFileStr;
                        # start of variants

               for(siVar = 1;
                   siVar <= lenArySI[siCol];
                   ++siVar)
               {  # Loop: print variants for this lineage
                  printf "\t%s",
                         varIdAryStr[siCol][siVar] >> complexFileStr;
               }; # Loop: print variants for this lineage

               printf "\n" >> complexFileStr;
            }; # Loop: print complex variants
         }; # END
      ';
