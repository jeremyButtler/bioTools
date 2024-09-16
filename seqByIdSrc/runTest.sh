#idsStr="$HOME/files/testing/seqByIdTests/19999.ids";
#fqStr="$HOME/files/testing/seqByIdTests/20kReads.fq";

#idsStr="$HOME/files/testing/seqByIdTests/allOnt.ids";
#fqStr="$HOME/files/testing/seqByIdTests/AllUP9.fastq";

#idsStr="$HOME/files/testing/seqByIdTests/allIll.ids";
idsStr="$HOME/files/testing/seqByIdTests/seqkit-benchmark-data/tmpEven20k.ids.filt";
fqStr="$HOME/files/testing/seqByIdTests/seqkit-benchmark-data/dataset_C.fq";

./seqById \
    -id "$idsStr" \
    -fq "$fqStr" \
  > del.fq;
awk \
    '{print $1; getline; getline; getline; getline; getline};' \
    < del.fq \
  > del.ids;

printf "extract: %s\n" "$(wc -l del.ids)";
printf "targets: %s\n" "$(wc -l "$idsStr")";

cat \
    del.ids \
    "$idsStr" |
  sort -V |
  uniq -c | \
  awk '{if($1 != 2) print $2;};' |
  grep \
    -f - \
    -n "$fqStr" |
  awk \
      '{
          split($1, idAry, ":");
          idAry[1] = ((idAry[1] -1) / 4) + (idAry[1] % 4);
          print idAry[2], idAry[1];
       };' \
  > del-missing.ids

printf "missing: %s\n" "$(wc -l del-missing.ids)";
