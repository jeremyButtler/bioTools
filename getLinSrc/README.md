# Use:

The getLin program detects lineages in mapped reads or a
  mapped consensus.

The ouput system is a mess, but then it is hard to have
  an orginized output when trying to find tandum repeat
  regions that can have any number of repeats.

# License:

Unlicense as primary, but if for any reason the Unlicense
  is a problem treat it as MIT.

# Install:

## Unix:

```
cd ~/Downloads;

if [ ! -d bioTools ]; then
   git clone \
      https://github.com/jeremyButtler/bioTools \
      bioTools;
   cd bioTools;
else
   cd bioTools;
   git pull;
fi;

cd getLinSrc;
make -f mkfile.unix;
```

If you have sudo privileges You can install getLin
  globally with:

```
sudo make -f mkfile.unix install;
```

If you do not have sudo privileges you can install getLin
   locally with:

``
if [ ! -d "${HOME}/local/bin" ]; then
   mkdir -p "${HOME}/local/bin";
fi;

make -f mkfile.unix PREFIX="${HOME}/local/bin" install;
```

# Using:

To get the help message do `getLin -h`.

## The databases

To use getLin you will need at least a simple database
  (one mutation is one lineage). You can find instructions
  see the `lineageSheets.md` file for instructions on
  how to build this. Also, the `simple.tsv` file is an
  example of this database for Africian Swine Fever Virus.

If you want to combine multiple single variant lineages
  into a single lineage you will need to build a complex
  database (multiple mutations for one lineage). For
  building this in the `lineageSheets.md` file. Also, the
  `complex.tsv`a file is an example of a complex database
  for Africian Swine Fever Virus.

## Using with reads

You will also need to map your reads/genome to your
  reference genome.

```
minimap2 -a reference.fasta reads.fastq > read-map.sam;
```

Then you can use getLin.

```
getLin -simple simple-database.tsv -sam read-map.sam;
```

To include a complex database do:

```
getLin \
    -simple simple-database.tsv \
    -complex complex-database.tsv \
    -sam read-map.sam;
```

## Using with reads

For a genome you will want to change the print mode to
  genome mode with `-pmode-genome`.

This example shows the genome mode through pipes.

```
minimap2 -a reference.fasta genome.fasta |
  getLin \
      -simple simple-database.tsv \
      -complex complex-database.tsv \
      -sam - \
      -pmode-genome;
```

## test case

I included a test case using some Africain Swine Fever
  Virus references from Mazloum et al. 2023 in
  test.sam.zip. Everything is already mapped, so you
  only need to run getLin.

Mazloum A, van Schalkwyk A, Chernyshev R, Igolkin A,
   Heath L, Sprygin A. A Guide to Molecular
   Characterization of Genotype II African Swine Fever
   Virus: Essential and Alternative Genome Markers.
   Microorganisms. 2023 Mar 2;11(3):642.
   doi: 10.3390/microorganisms11030642. PMID: 36985215;
   PMCID: PMC10056344.

For

```
getLin \
    -simple simple.tsv \
    -complex complex.tsv \
    -sam test.sam;
```

You output should look like:

| id         | type    | lineages...   |             |            |             |            |   |
|:-----------|:--------|:--------------|:------------|:-----------|:------------|:-----------|:--|
| MW306192.1 | genome  | 505-9R-TRS:3  | B602L-SNP:4 | I73R-IGR:2 | K145R-SNP:1 | O174-TRS:1 | * |
| JX857524.1 | genome  | B602L-SNP:UKR | *           |            |             |            |   |
| MT647531.1 | genome  | B602L-SNP:2   | *           |            |             |            |   |
| MT647544.1 | genome  | B602L-SNP:3   | *           |            |             |            |   |
| ON098025.1 | genome  | B602L-SNP:5   | *           |            |             |            |   |
| OM986235.1 | genome  | B602L-SNP:UKR | *           |            |             |            |   |
| OM986209.1 | genome  | B602L-SNP:UKR | *           |            |             |            |   |
| MT951767.1 | genome  | I73R-IGR:4    | *           |            |             |            |   |
| MG939587.1 | genome  | 505-9R-TRS:3  | B602L-SNP:1 | I73R-IGR:1 | K145R-SNP:2 | O174-TRS:2 | * |
| MG939584.1 | genome  | 505-9R-TRS:3  | B602L-SNP:1 | I73R-IGR:1 | K145R-SNP:2 | O174-TRS:1 | * |
| OM799941.1 | genome  | 505-9R-TRS:3  | B602L-SNP:1 | I73R-IGR:1 | K145R:3     | O174-TRS:1 | * |

Table: showing what the output should look like.
