# Use:

getAmr is designed to detect AMR mutations in reads using
  a tsv catalog. This program is the AMR detection program
  in freezeTB, but set up to handle multiple references.

addAmr can be used to build a database for getAmr using
  variant ids.

# License:

Dual licensed under public domain or MIT. Pick the one
  that works best for you.

# Install

To build you will use `make -f` with the make
  file targeting your OS. The OS the make file was built
  for is the end of the name.

- Make file names:
  - mkfile.unix: any unix (Mac/Linux/BSD) OS
  - mkfile.static: static build (Linux/BSD)
  - mkfile.win: windows (does not install)
    - use `nmake /F` instead of `make -f`

## Unix

```
make -f mkfile.unix
make -f addAmr_mkfile.unix;
sudo make -f mkfile.unix install;
sudo make -f addAmr_mkfile.unix install;
```

## Windows

```
nmake /F mkfile.win;
nmake /F addAmr_mkfile.win;
```

Then put the getAmr binary in a good location.

# Testing:

getAmr has been tested with a single reference database,
  but has not been tested with a multi-reference database.
  It has been tested with database built from the test.txt
  file by addAmr, which uses a reference column. It also
  has been tested with freezeTB databases, which has no
  reference column.

addAmr has be tested to build  a database with the
  test.txt file. The database has been saved as
  test-db.tsv.

# Run:

To run getAmr you will need to have an database of AMRs
  in the getAmr format. An example database can be found
  in freezeTB (the 2023 WHO TB AMR catalog). However, if
  you wish to make your catalog you will want to use
  **TODO: add this program in**

The input reads or consensuses need to be in a sam file
  format. So, you will need to use read mapper, such as
  minimap2.

The help message can be printed with `getAmr -h`.

## Running reads

For raw reads my filtering settings are quite low and are
  set up to catch noise. My logic is that the user is
  more likely to use read checking as part of an pipeline
  to detect mixed infections If you plan to not check
  consensuses, then I would recommend setting
  the `-min-amr-map-perc` (percentage of reads covering
  the AMR variant that supported the AMR variant) to at
  least 0.05 (5%).

```
minimap2 -a ../freezeTBFiles/NC000962.fa reads.fastq > tmp.sam;
getAmr -amr-tbl ../freezeTBFiles/who-2023.tsv -sam tmp.sam;
```

or

```
minimap2 -a ../freezeTBFiles/NC000962.fa reads.fastq |
  tbAmr -amr-tbl ../freezeTBFiles/who-2023.tsv -sam -;
```

## Running consensuses

Make sure you use the -sam-con entry for consensus.
  Otherwise tbAmr will apply read depth filters.

```
minimap2 -a ../freezeTBFiles/NC000962.fa consensus.fasta > tmp.sam;
tbAmr -amr-tbl ../freezeTBFiles/who-2023.tsv -sam-con tmp.sam;
```

or

```
minimap2 -a ../freezeTBFiles/NC000962.fa reads.fastq |
  tbAmr -amr-tbl ../freezeTBFiles/who-2023.tsv -sam-con -;
```

# Some extra commands:

## Printing read ids for AMR sequences

You can print out the read ids and the variants they
  mapped to using `-id-file file.tsv`. Each line will have
  an read id and one variant that it mapped to. Multiple
  lines are used for read ids that mapped to multple
  variants. You can use fqGetIds or seqkit extract the
  fastq entries using the read ids.

## Frameshift checking (not recommended)

You can apply frameshift checking using `-frameshift`.
  This checks for early stop codons, missing stop codons
  (if have gene end), and missing start codons (if have
  start of gene). It also checks if the reading
  frame offset caused by indels (deletions - insertions)
  is not a multiple of three (frame shift) (crude, but
  works and is quick). The indel count is applied on the
  gene and the AMR region (part of gene causing an AMR).

One consenquence of the indel counting is that a single
  positive indel can result in an gene knock out. So,
  reads, which have a lot of false positive indes, are
  going to make a lot of false positives.

# using addAmr

## intro and usage

You should be able to use addAmr to create a database for
  getAmr. This should work and the output looks good.
  However, I have not run a database thourgh getAmr yet.

The required input is a reference sequence, the
  coordinates for each gene (see ampDepth or `addAmr -h`),
  and a variants file with variants to convert.

You can get the help message with `addAmr -h`.

For addAmr, you can run:

```
addAmr \
    -ref reference.fasta \
    -coords coordinates.tsv \
    -var variants.tsv \
  > database.tsv`;
```

## variants file

You can create a database using addAmr using the variant
  ids from a file. The variant file has three required
  entries, the variant id, the reference id (`-ref <id>`),
  and any drug resistance. You can also add a comment with
  `-note <comment>`.

Here is the generalized example of the variant file.

```
variant_id	-ref	reference_id	-other	drug_name	-note	comment
```

You can see test.txt for some examples.

```
gyrB_p.Ser447Phe	-ref	NC_000962.3	-lfx	-note	levovloxcin_shorthand_flag
fgd1_LoF	-ref	NC_000962.3	-dlm	-note	delamanid_shorthand_flag
fgd1_p.Asn112fs	-ref	NC_000962.3	-other	delamanid	-note	and_drug_(this_is_delamanid)_flag
```

Each variant ID should start out with the gene name,
  followed by an underscore (`<gene_>`). The next part
  tells if the gene is coding (`<gene>_c.`), nucleotide
  (`<gene>_n.`), protein (`<gene>_p.`), gene loss of
  function (`<gene>_LoF`), or the entire gene was deleted
  deletion (`<gene>_deletion`). The last part tells the
  mutation type.

- General variant id examples:
  - Gene Loss of Function: `fgd1_LoF`
  - Gene deletion: `katG_deletion`


For nucleotide mutations, use position, reference, the
  symbol, and mutation. For a SNP it would look like
  `rrl_n.2270G>C`. For an insertion it would look like
  `eis_c.-8delc`. For an insertion you need to included
  the two bases the insertion, `ins` and then the base.
  For example, `rrl_n.2269_2270insT`.

- Nucleotide variant id examples:
  - SNP: `rrl_n.2270G>C`
  - insertion: `rrl_n.2269_2270insT`
  - multi-base insertion: `rrl_n.2269_2270insTAGG`
  - deletion `eis_c.-8delc`

For amino acid mutations, use the reference sequence,
  position, then mutation. The mutation can be `dup` for
  duplicates, `del` for deletion, `ins<AA>` for and
  insertion, `fs` for a frameshift, and an amino acid for
  an SNP mutation `<AA>`.

You can also use `?` for any amino acid and `*` for a
  stop mutation. You can use `ext*?` to get loss of
  stop codon. All start codons are treated as `Met`.

- Amino acid variant id examples:
  - SNP: `gyrB_p.Ser447Phe`
  - SNP: to stop: `Rv0678_p.Gln22*`
  - SNP: loss of stop: `pncA_p.Ter187Argext*?`
  - SNP: replace start with anything `Rv0678_p.Met1?`
  - deletion: `rpoB_p.Asn437_Asn438del`
  - insertion: `rpoB_p.Ser431_Gln432insArg`
  - frame shift: `Rv0678_p.Asp8fs`
  - duplication: `rpoB_p.Phe433dup`
  - larger duplication: `rpoB_p.Phe433_440dup`
    - note tested
