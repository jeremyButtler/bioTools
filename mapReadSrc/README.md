# Use:

Map reads to a reference sequence. This is slower than
  minimap2 and is less sensitive (will miss hits). The
  only reason this exists is for freezeTB to operate
  indepently.

The main issue in sensitivity, is that mapRead requires
  the sequence to have multiple 20+ base long match
  (a chain) regions. Also, the chains must cover at at
  least 20% of the query. This results in missing many
  alignments that minimap2 may detect. For example, in
  one sample we mapped to tuberculosis, we missed over
  4000 16sRNA sequences from non-tuberculosis bacteria. We
  also missed around 27 sequences that were from other 
  locations (less than 20% of bases in 20+ base pair match
  regions).

Also, in some cases mapRead may prefer indels, when
  minimap2 preferes a different alignment. For
  tuberculosis reads this seems to happen in the direct
  repeat region, were many 20mers are repeated. It comes
  from the chain merging step.

Finally, mapRead often prefers indels at the end of
  homopolymers (minimap2 pefers start of homopolymers).
  This is a minor note, but it can affect what the indel
  may do.

# License:

Dual licensed under Unlicense or MIT, you pick which one
  you prefer or can work with.

# Install:

## Unix (Linux/Mac/BSD):

Only tested on Linux. However, nothing in code to prevent
  portability to Mac.

```
cd ~/Downloads;
git clone https://github.com/jeremybuttler/bioTools;
cd bioTools/mapReadSrc;

if [ ! -d "/usr/local/bin" ];
then
   sudo mkdir -p "/usr/local/bin";
   # this is sometimes missing on MAC
fi

make -f mkfile.unix;
sudo make -f mkfile.unix install;
```

## Windows:

In theory this should work, but is a pain. You will need
  the microsoft C++ build essentials (included with
  visual studio). Then open a developer terminal.

Clone/Download the bioTools repository, unzip and save the
  unzipped files Downloads. Make sure it is named
  bioTools and that you do not have something like
  `bioTools\bioTools`. Then open a developer terminal.

```
cd "%HOMEPATH%\Downloads\bioTools\mapReadSrc"
nmake /F mkfile.win
```

install mapRead.exe were you want. Just remember you can
  only run mapRead from the terminal.

# Running:

**-fa-gz and -fq-gz are a future goal, so do not work**

To get the help message do `mapRead -h`.

You can run mapRead with a reference in a fasta file,
  however, it will take mapRead extra time to index the
  reference. If you plan to use the same reference again
  with the same kmer length/minimum chain size, then use
  `mapRead -index reference.fasta > reference.index`. You
  can then
  use `mapRead -ref-index reference.index -fq reads.fastq > mapped-reads.sam`
  to map reads.

## Filters:

Map reads has several filters you can use. The first are
  the kmer settings, which are set
  by `-kmer-val reference-length,kmer,chain-length`. These
  settings are included in the reference.index file, so if
  you can not chagne these if using `-ref-index`.

- reference-length: is the minimum reference size to apply
  this kmer setting to
  - 0 or your smallest value is any length
- kmer: is the size of one kmer (up to 15 bases)
  - avoid using short kmer lengths, otherwise you get to
    many hits
- chain-length: is the minium number of kmers needed to
  keep a chain (sequence of one or more matching kmers)
  - this allows you to get kmer lengths longer than 15
    bases

The other filter settings remove alignments or prevent
  further analysis at key steps.

- -min-perc-score: percentage of maximum aligned score (
  excludes masked bases) a alignment must have to keep
- -min-perc-match: minimum percent of sequence length must
  be matches to keep alignment
- -min-len-perc: minimum percent of sequence length the
  alignment must cover
  - best to keep low
- -min-chain-nt: minimum percent of sequence the chains
  must cover (applied after chain merging step)
  - only one aligment must meet this

## How works

1. convert referernce sequence to kmers
2. create sorted look up table for reference kmer
   locations
   - look_up_table[index] = reference position
3. convert query sequence to kmers
4. find all matching reference positions that share
   (kmer length + chain length - 1) bases
   - this is a chain
5. merge chains
   - chains can not be merged if distacne is > 25%
   - chains can not be merged if gap_length is >
     nucleotides (bases) in chain to merge
   - each chain is merged to get the maximum number of
     nucleotides (bases)
   - chains are merged from last query base to first base
   - Filter step: no chains reads removed
   - Filter step: afterwards, reads with low percentage
     of chain bases are removed
6. fill in gaps in chains using needle
   - unless only indel is present (no snps or matches)
   - unless only one snp
7. score chains and keep best sub-alignment
   - Remaining filter steps are appplied
8. repeat steps 6 to 7 until all possible chains have
   been checked
   - if chain is part of previous alignment it is skipped
   - best scoring alignment kept
9. repeat 4 to 8 for reverse complement read
10. mark lowest scoring alignment (reverse/foward) as
    secondary
11. print kept alignments
    secondary
