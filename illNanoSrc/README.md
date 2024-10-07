# Use:

Scans a sam file of mapped nanopore reads for variants
  found in Illumina sequencing.

ONT reads are error prone, so expect poor results and to
  have to do some filtering/figuring out how to group
  profiles. To sum it up, this is not a very good program.

# License:

Dual licensed under Unlicense (public domain) and MIT. By
  default this is public domain, however, if public domain
  does not work, or is not desired, then this defaults to
  MIT.

# Install:

## Unix (Mac, linux, ect ...)

```
git clone https://github.com/jeremybuttler/bioTools
cd bioTools/illNanoSrc
make -f mkfile.unix
sudo make -f mkfile.unix install

cd ../tbConSrc
make -f mkfile.unix
sudo make -f mkfile.unix install
```

## windows

Ye, windows is a pain (not tested, but should work).

Download the bioTools repository (save to Downloads)
Open a developer terminal
  (visual studio->view->developer terminal).

```
cd $$HOME$$\Downloads\bioTools\illNanoSrc
nmake /F mkfile.win

cd ..\tbConSrc
nmake /F mkfile.win
```

Copy `illNano.exe` (in bioTools/illNanoSrc) and
  `tbCon.exe` (int bioTools/tbConSrc) file to were you
  want. You will need to call everything by command line.
  Maybe put the .ext files in appData?

# Use:

You can get the help message with `illNano -h`.

You can get the help message for tbCon with `tbCon -h`.

You will need to get the tsv output from tbCon or use a
  similar formatted tsv (not a vcf). 

```
# build tsv with variants
minimap2 -a reference.fasta illumina-reads.fastq > illumina-reads.sam
tbCon -sam illumina-reads.sam -out-tsv ill-vars.tsv -out ill-con.sam

minimap2 -a reference.fasta ONT-reads.fastq > ONT-reads.sam
illNano -ill-tsv ill-vars.tsv -sam-ont ONT-reads.sam > ONT-vars.tsv
```

Then you need to figure out a way to filter variants.

- The output is a tsv with four columns (currently)
  1. ONT read id
  2. Mapped referenced (this does not do reference checks)
  3. Number of variant positions covered
  4. Variant profile (number_symbol; ex: 203A)

Here is a table of symbols in the variant profile
  (column 4):

| symbol |       meaning                  |
|:------:|:-------------------------------|
| number |  reference position of variant |
|   X    |  no Illumina variant           |
|   A    |  Illumina/ONT are A            |
|   C    |  Illumina/ONT are C            |
|   G    |  Illumina/ONT are G            |
|   T    |  Illumina/ONT are T            |
|   D    |  Illumina/ONT are deletion     |

Table: table of symbols for the variant profile line

Insertions are ignored.

See notes.md for some experimenting I did with filtering.
  I got a decent number of reads (probably 60000 of
  200000 with no X's). It is not a great script, but might
  give you a few ideas. I was working on SRA accession
  SRR13951165. The Illumina data set is SRA accession
  SRR13951207. It is an Noro virus data set for 2019 base
  called with the fast model on guppy 3.3, so a very noisy
  dataset.
