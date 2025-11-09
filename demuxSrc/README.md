# Use:

Find amplicons in sequences, such as reference genomes or
  reads.

The origin of the name came from my thought of using it
  as a barcode demuxer, but I found out that the first
  100 bases in a Minion sequenced read are to noisy.
  Adapters are always missed and barcodes need low score
  thresholds. Use `dorado demux`. The reason for the
  attempt was me demuxing with the wrong kit.

# License:

The primary is the Unlicense, but if that does not work
  treat this as under the MIT license.

# Install:

## Unix (Linux/Mac/BSD):

```
cd "${HOME}/Downloads;

if [ ! -d bioTools ]; then
   git clone https://github.com/jeremyButtler/bioTools bioTools;
fi;

cd bioTools/demuxSrc;
make -f mkfile.unix;
sudo make -f mkfile.unix install;
```

# Usage:

You can print the help message with `demux -h`.

You will need to provide a tsv file with `-prim-tsv`. The
  first row is the header and is ignored. The next rows
  should have the primers.

- TSV format:
  - Column 1: name of primers (no spaces or tabs)
  - Column 2: True, if both primers need to be present or
    False if only one primer needs to be present
    - for now demux ingores this column and assumes True
      if two primers are provided
  - Column 3: forward primer sequence
  - Column 4: reverse primer sequence

Here is an example of a tsv for two primers:

```
id	paired	for_seq	rev_seq
katG	T	ACCCACCCATTACAGAAACC	AGCAGGTTCACGAAGAAGTC
embA	T	CATCCTCACCGCCCTTA	CAACCTGTGGCTTCTTCTC
```

This is not valid for input, but it shows what the table
  would look like.

| id   | paired | for_seq              | rev_seq              |
|:-----|:------:|:---------------------|:---------------------|
| katG | T      | ACCCACCCATTACAGAAACC | AGCAGGTTCACGAAGAAGTC |
| embA | T      | CATCCTCACCGCCCTTA    | CAACCTGTGGCTTCTTCTC  |

Table: of what the input table would look like if it was
  a  markdown input. You can not use this table in the
  actual program.

Other then the table you will need to provide one or more
  fastx (fasta or fastq) files at the end. The file(s) can
  be gz compressed (fasta.gz or fastq.gz).

```
demux -prim-tsv primers.tsv reads.fasq > trimmed-reads.fq;
```

The default output is a log and the trimmed reads to
  stdout (terminal). You can rename the log with
  the `-prefix <your_name>` command.

```
demux -prim-tsv primers.tsv reads.fasq -prefix reads-log \
  > trimmed-reads.fq;
```
