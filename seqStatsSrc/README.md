# Use:

Prints the read length, mean q-score, and median q-score
  for all reads in a fastq file. Maybe more added latter
  if I decided I want more.

Note: median q-scores seem to be a bit high. Not sure why.

# Install

```
git clone https://github.com/jeremybuttler/bioTools ~/Downloads/bioTools;
cd ~/Downloads/bioTools/seqStatsSrc;
make -f mkfile.unix;
sudo make -f mkfile.unix install;
```

# Running

The only input is a fastq file. It can be gzipped
  compressed (fastq.gz or fq.gz) or a regular fastq file.

You can get the help message with `seqStats -h`.

```
seqStats reads.fastq > stats.tsv
```

or to filter reads

```
seqStats -filter reads.fastq > filtered-reads.fastq
```

Output:

| id      | length      | mean_q               | medain_q               |
|:--------|:------------|:---------------------|:-----------------------|
| read_id | read_legnth | mean_q_score_of_read | medain_q_score_of_read |

Table: showing format of output for seqStats
