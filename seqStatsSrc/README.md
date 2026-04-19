# Use:

Filter reads by quality score and length. You can also
  print the read length, mean q-score, and median q-score
  for all reads in a fastq file.

ONT and Illumina use different methods to find the mean
  quality scores. These methods can change the value of
  the quality score.

Illumina takes the mean of the quality scores for all
  bases. This is the way most tools find the quality
  score.

ONT converts each bases quality score to the error rate
  and then finds the mean. They then convert the mean back
  to a quality score. By doing this you get a lower
  quality score. `(0.1 + 0.01) / 2 = 0.055` (Q 12.6) is
  different then `(10 + 20) / 2 = 15` (Q 15).

As a rule the mean q-scores will be lower for ONT
  q-scores. I am not sure why the ONT median q-score is
  so high. Though I do know the variability in it comes
  from when I need to do a mean of two values.

# Install

```
if [ ! -d "${HOME}/Downloads/bioTools" ];
then
   git clone \
       https://github.com/jeremybuttler/bioTools \
       "${HOME}/Downloads/bioTools";
   cd "${HOME}/Downloads/bioTools/seqStatsSrc";
else
   cd "${HOME}/Downloads/bioTools/seqStatsSrc";
   git pull;
fi;

make -f mkfile.unix;
sudo make -f mkfile.unix install;
```

# Running

The only input is a fastq file. It can be gzipped
  compressed (fastq.gz or fq.gz) or a regular fastq file.

You can get the help message with `seqStats -h`.

## Filtering reads

```
seqStats -filter -no-stats -ont-mean-q 15 reads.fastq > filtered-reads.fastq
```

## Geting stats

```
seqStats reads.fastq > stats.tsv
```

Output:

| id      | length      | mean_q               | medain_q               |
|:--------|:------------|:---------------------|:-----------------------|
| read_id | read_legnth | mean_q_score_of_read | medain_q_score_of_read |

Table: showing format of output for seqStats
