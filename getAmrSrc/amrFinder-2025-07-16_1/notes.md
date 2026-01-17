# conversion notes

These files were from the 2025-07-16 amrFinder (from
  NCBI) database. However, I was unable to convert the
  database.

I am trying to to convert NCBI's database to an AMR
  database. It looks like all the AMR mutations are in
  the `AMRProt-mutation.tsv` file. Also, there are DNA
  mutations in the `AMR_DNA-<species>.tsv` file.

However, I noticed the reference sequences from the
  `AMRProt-mutation.tsv` file were in the amino acid
  format. So, I gave up trying to convert this over since
  I need the reference sequence to be in nucleotides.

- Mutation types:
  - singel amino acid `gene_<ref><gene_pos><mutation>`
  - insertion `gene_<ref><gene_pos><ref><mutations...>`
  - deletion `gene_<ref><gene_pos>del`
  - multi deletion `gene_<ref_sequence><gene_pos>del`
  - stop codon `gene_<ref><gene_pos>stop`
  - frameshift `gene_<ref><gene_pos><possible_combinations><fs>`
    - has lowercase `fs` in the name

- for a mutation I need `gene_p.xxx<position>yyy`
- for a stop codon change I need `gene_p.xxx<position>*`
- for a frameshift I need `gene_p.xxx<position>fs`
- for a deletion I need `gene_p.xxx<position>del`
- for an insertion I need `gene_p.xxx<position>insXXX`
- for a LoF I need `gene_LoF` (not present)

This is the script to convert variant ids to the needed
  format.

```
awk '
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
   '
```
