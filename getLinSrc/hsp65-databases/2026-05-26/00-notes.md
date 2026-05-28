# Goal:

- Versions:
  - minimap2 version 2.27-r1193
  - MAFFT v7.505 (2022/Apr/10)
  - bioTools version 2026-04-19
    - used filtsam


# get hsp65 reference list

Get hsp65 gene mutations for variaous mycobacteirum. I
  started by copying table one from Dai et al 2011. I then
  used sed and awk to format the table into something I
  can work with.

Dai J, Chen Y, Lauzardo M. Web-accessible database of
  hsp65 sequences from Mycobacterium reference strains.
  J Clin Microbiol. 2011 Jun;49(6):2296-303.
  doi: 10.1128/JCM.02602-10. Epub 2011 Mar 30.
  PMID: 21450960; PMCID: PMC3122750.

# 02 format the hsp65 table reference list

Here is what the command I did would have looked like in
  sed

```
sed '
       s/ /_/g;       # replace spaces with underscores
       s/\._/_/g;     # replace M._ with M_
       s/^\([^\t]*.\)[^\t]*./\1/g; # remove middle column
       s/_([^)]*.//g; # remove the citation [(number)]
       s/,/\t/g;      # replace commas with \t
       s/\t_/\t/g;    # remove leading underscores
       s/_\t/\t/g;    # remove trailing underscores
    ' "01-input/01-mycoSpecies-pmid2145096.txt" |
  awk '
        BEGIN{OFS="\t"; getline; lineStr = $0};
        { # MAIN
           if($1 !~ /M_/)
              lineStr = lineStr "\t" $0;
              # this species is broken into multiple lines
           else if(lineStr != "")
           { # Else If: on new line
              print lineStr;
              lineStr = $0;
           } # Else If: on new line
        }; # MAIN
        END{print lineStr;};
      ' \
  > "02-mycoSpecies-pmid2145096.tsv";
```
 
# 03 download the hsp65 gene list

Next I needed to download the reference genomes.

```
baseStr="https://eutils.ncbi.nlm.nih.gov/entrez/eutils";

while read -r lineStr;
do # Loop: download all refrences
   lineStr="$(printf "%s" "$lineStr" | tr '\t' ' ')";
   lineStr="$lineStr ";
   idStr="${lineStr%% *}";
   lineStr="${lineStr#* }"
   lineStr="${lineStr# }"

   urlStr="$baseStr/esearch.fcgi";
   urlStr="$urlStr?db=nuccore";
   urlStr="$urlStr&retmode=text";
   urlStr="$urlStr&retmax=10000";

   urlStr="$urlStr&term=${lineStr%% *}"
   lineStr="${lineStr#* }"
   lineStr="${lineStr# }"

   while [ "$lineStr" != "" ];
   do # Loop: add accession numbers
      urlStr="$urlStr,OR,${lineStr%% *}";
      lineStr="${lineStr#* }"
      lineStr="${lineStr# }"
   done # Loop: add accession numbers

   uidsStr="$(
      curl -g "$urlStr" |
         sed -n 's/<\/*Id>//gp;' |
         tr '\n' ',' |
         sed 's/[ \t]*//g; s/,$//;' \
   )"; # get uids for each reference

   urlStr="$baseStr/efetch.fcgi";
   urlStr="$urlStr?db=nuccore";
   urlStr="$urlStr&id=$uidsStr";
   urlStr="$urlStr&retmode=text";
   urlStr="$urlStr&rettype=fasta";
   urlStr="$urlStr&retmax=10000";
   
   curl -g "$urlStr" | sed "s/^>/>$idStr-/;" \
       >> 03-mycoSpecies-pmid2145096.fa;
   sleep 1;
done < "02-mycoSpecies-pmid2145096.tsv"
   # Loop: download all refrences
```

# 04 trim non-hsp65 bases

In some cases we downloaded full genomes, so we needed to
  remove the non-hsp65 bases.

```
minimap2 \
    -a \
    01-input/01-hsp65-NC000962.fa \
    03-mycoSpecies-pmid2145096.fa |
  filtsam -trim -out-fasta -sam - \
  > 04-mycoSpecies-pmid2145096-trim.fa;
```

# 05 alignment

I wanted to remove the genomes that have no snp
  differences from the dataset

```
minimap2 \
    -a \
    --eqx \
    --secondary-seq \
    04-mycoSpecies-pmid2145096-trim.fa \
    04-mycoSpecies-pmid2145096-trim.fa |
  filtsam -out-stats -sam - |
  awk '
        BEGIN{getline; print "0ref", "0query";};
        { # MAIN
           qryStr = $1;
           sub(/-.*/, "", qryStr);
           refStr = $2; sub(/-.*/, "", refStr);

           if(qryStr == refStr)
              next; # same species
           else if($12 > 0)
              next;
           else
              print refStr, qryStr;
        }; # MAIN
      ' |
  sort -V |
  uniq \
  > 05-nonDiff.tsv;
```

| species       | other hits                 |
|:--------------|:---------------------------|
| avium         | other avium subspecies     |
| TB            | bovis                      |
| TB            | caprea                     |
| TB            | microti                    |
| fortuitum     | conceptionese              |
| fortuitum     | marinum                    |
| fortuitum     | peregrinum                 |
| fortuitum     | other fortuitum subspeices |

Table: species identical. I am not inncluding subspeices.

# 06 and 07 build database

I removed the entries that could not be distinguished. The
  file is `06-mycoSpecies-pmid2145096-trim-rmDup.fa`.

I then mapped the genomes to the NC000962 (H37Rv) genome.
  I set the line wrap to only have 10 bases per line to
  make copying easier for me.

```
minimap2 \
    -a \
    --eqx \
    "${HOME}/Documents/freezeTBFiles/NC000962.fa" \
    06-mycoSpecies-pmid2145096-trim-rmDup.fa |
  samToAln \
     -ref "${HOME}/Documents/freezeTBFiles/NC000962.fa" \
     -wrap 25 \
  > "07-mycoSpecies-pmid2145096-trim-rmDup-map.aln";
```

# 08 build mutation tables

To bulid my database I wanted to get all hsp65 base
  mutations

```
awk '
      BEGIN{OFS="\t"; getline;};
      { # MAIN
         seqStr = $0; # sequence takes up one line

         # get hsp65 gene
         seqStr = substr(seqStr, 528787, 376);
         lenSI = split(seqStr, hsp65Str, "");

         printf "tuber";
         for(siNt = 1; siNt <= lenSI; ++siNt)
            printf "\t%i_%s",
                   431 + siNt - 1,
                   hsp65Str[siNt];
         printf "\ttuberculosis\n";
      } # MAIN
    ' "${HOME}/Documents/freezeTBFiles/NC000962.fa" \
  > "08-mycoSpecies-pmid2145096-trim-rmDup-map-mut.tsv";
```

I then used awk to get all the mutations for all the
  genomes in the databasese. In this script I limited the
  range to hsp65 bases 431 (528787) to 806 (529162).

```
awk \
    -f 00-scripts/hsp65AlnToMut.awk \
    "07-mycoSpecies-pmid2145096-trim-rmDup-map.aln" \
  >> "08-mycoSpecies-pmid2145096-trim-rmDup-map-mut.tsv";
```

I also added in the unique entries from my database.

```
awk '
      BEGIN{
         OFS="\t";
         getline;
         getline;

         # get bases for databases TB entry
         for(siCol = 10; siCol <= NF; ++siCol)
            tbAryStr[siCol - 10] = $siCol;
      };
      { # MAIN
         printf "%s__", $1;
         for(siCol = 10; siCol <= NF; ++siCol)
         { # Loop: grab non-TB mutations
            if(tbAryStr[siCol - 10] != $siCol)
               printf "\t%s", $siCol;
         } # Loop: grab non-TB mutations
         printf "\t%s\n", $1;
      }; # MAIN
   ' "../original/hsp65-db-complex.tsv" \
  >> "08-mycoSpecies-pmid2145096-trim-rmDup-map-mut.tsv";
```

# 09 build database

I then needed to convert the unique mutations into
  database entries.

```
awk \
    -f "00-scripts/mutToDb.awk" \
    -v prefixStr="09-hsp65" \
    "08-mycoSpecies-pmid2145096-trim-rmDup-map-mut.tsv";
```

I also expanded the sorthanded names names in my original
  database. I added `_org` to the end of there lineage
  name.

I also numbered most of the duplicate speices id. This
  only gets the species that were right next to each
  other.

```
awk \
    -f 00-scripts/markSpeciesDups.awk \
    09-hsp65-complex.tsv \
  > tmp.tsv;

mv tmp.tsv 09-hsp65-complex.tsv;
```

# 10 filter convert the database

I then needed to remove the complex variants that were
  duplicates. I used awk to find the duplicate sequences
  and then removed them by hand.

pseudoshottsii was removed due to at least two entires
  being close to fortuitum.

```
awk \
    -f 00-scripts/findDbDupRows.awk \
    09-hsp65-complex.tsv \
  > 10-dups.tsv;
```

Using awk I found that there were 265 unique entries:

```
awk ' { # MAIN
         for(siCol = 10; siCol <= NF; ++siCol)
            printf "\t%s", $siCol; printf "\n";
      }; # MAIN
    ' "09-hsp65-complex.tsv" |
  sort -V |
  uniq -c |
  wc -l;
```

However, I can only easily identify 271 of them

```
sort -V 09-hsp65-complex.tsv |
  uniq \
  > "10-hsp65-complex.tsv";
wl -l "10-hsp65-complex.tsv";
```

Using awk I was able to track down the duplicate rows:

```
awk \
   -f 00-scripts/findDbDupRows.awk \
   "10-hsp65-complex.tsv";
```
