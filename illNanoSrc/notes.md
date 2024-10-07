Just fiddling around.

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
  > 1165-illNano_notran.tsv

cut \
    -f 4 \
    1165-illNano_notran.tsv |
  sort -V |
  uniq -c |
  sed 's/^[ \t]*//;' |
  sort -V \
  > 1165-illNano_notran-cont.txt

awk \
    'BEGIN{print "profile";};
     {if($2 !~ /X/ && $1 >= 20) print $2;}
    ' \
    < 1165-illNano_notran-cont.txt |
  grep \
      -f - \
      1165-illNano_notran.tsv | \
  grep -v 'X' \
  > 1165-illNano_notran-highCnt.tsv;

cut \
    -f 4 \
    1165-illNano_notran-highCnt.tsv |
  sort -V |
  uniq |
  grep -v "profile" |
  awk \
      '{
          printf "grep \\\n    \"%s\" \\\n    ", $1;
          printf "1165-illNano_notran-highCnt.tsv |\n  ";
          printf "seqById \\\n    -id - \\\n    ";
          printf "-sam ";
          printf "SRR13951165-map.sam\" \\\n";
          printf "    -out 1165-illNano-highCnt";
          printf "-%s.sam;\n\n", $1;
       }' \
  > extractScript.sh

rm extractScript.sh;

# some files have to few reads
rmReadsStr=( \
    $(wc -l 1165-illNano-highCnt-* |
      awk '{if($1 < 20) print $2;};') \
    );

for strRm in ${rmReadsStr[*]}; do
   rm "$strRm";
done

conReadsAryStr=($(ls 1165-illNano-highCnt-*))

for strCon in ${conReadsAryStr[*]}; do
   conStr="$(sed 's/\.sam/-tbCon&/;' <<< "$strCon")";
   tbCon \
      -sam "$strCon" \
      -min-depth 5 \
      -out "$conStr";

   filtsam \
      -out-fasta \
      -sam "$conStr" |
    sed \
      "s/>/>$conStr/;
       s/1165-illNano-highCnt-//;
       s/-tbCon.sam/-/;
      " \
    >> "1165-illNano-highCnt-allCons.fa";
done
```
