get data and run illNano

```
prefetch SRR13951207;
fastq-dump SRR13951207;

prefetch SRR13951165;   # little under 500 mega bytes
fastq-dump SRR13951165;

minimap2 \
    -a \
    --eqx \
    -x sr \
    MW661279.fa \
    SRR13951207.fastq |
  filtsam \
    -F 4 \
    -F 256 \
    -F 2048
  > SRR13951207-map-filt.sam;
  # filtsam is from bioTools

minimap2 \
    -a \
    --eqx \
    -x sr \
    MW661279.fa \
    SRR13951165.fastq |
  filtsam \
    -F 4 \
    -F 256 \
    -F 2048
  > SRR13951165-map.sam;
  # filtsam is from bioTools

tbCon \
   -sam SRR13951207-map-filt.sam \
   -out-tsv 1165-ill-con.tsv;
   # tbCon is from bioTools

./illNano \
    -ill-tsv 1165-ill-con.tsv \
    -sam-ont SRR13951165-map.sam \
    -uniq-out 1165-map-tbCon-illNano_uniq.tsv \
  > 1165-map-tbCon-illNano.tsv

# -uniq-out gives me the unique depth profiles
```

get depths for each position

```
cut \
    -f 7 \
    1165-map-tbCon-illNano_uniq.tsv |
  sed 's/[0-9]*://g s/_/ /g;'
```

get row number of profiles with depth over 10

```
cut \
    -f 7 \
    1165-map-tbCon-illNano_uniq.tsv |
  sed 's/[0-9]*://g; s/_/\t/g' |
  awk '{
      for(uiCol = 1; uiCol <= NF; ++uiCol)
         if($uiCol > 9){print NR; next;};
    };'
```

get profiles with at least 10x read depth

```
cut \
    -f 7 \
    1165-map-tbCon-illNano_uniq.tsv |
  sed 's/[0-9]*://g; s/_/\t/g' |
  awk '{
      for(uiCol = 1; uiCol <= NF; ++uiCol)
         if($uiCol > 9){print NR "p;"; next;};
    };' |
  sed \
    -f - \
    -n \
    1165-map-tbCon-illNano_uniq.tsv \
  > 1165-map-tbCon-illNano_uniq-10x.tsv;
```
