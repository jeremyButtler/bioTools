# Use:

Bins Genbank accession numbers in a text or fasta file by
  assembly. This system relies on assembly assigning four
  or more letters to an assembly accession number. This
  does not separate protein from nucleotide.

Here are some example Genbank accession numbers. There is
  no easy rule of thumb, but from others.

- Proteins start with three letters or have a special
  refseq tag
- Assemblies start with four or more letters
- Genome accessions always start with `GCF_` or `GCA_`
- Nucleotide accession have one to two letters, though
  they can also start with a two letter refseq tag
  followed by an underscore (`NZ_`)

| type of accession            | accession example    |
|:-----------------------------|:---------------------|
| genome accession (refseq)    | GCF\_028878055.3     |
| genome accession (genbank)   | GCA\_028878055.3     |
| assembly accession           | WYAK01000010.1       |
| refseq assembly accession    | NZ_CASIGT010000001.1 |
| protein (no id on database)  | NP_001735.1          |
| regular nucleotide accession | MK526900.1           |
| regular protein accession    | QDX10313.1           |
| refseq nucleotide accession  | NZ_LN874954.1        |

# License:

CC0

# Install:

For unix do:

```
if [ ! -d "${HOME}/Downloads/bioTools" ];
then
   git clone \
      https://github.com/jeremyButtler/bioTools \
       "${HOME}/Downloads/bioTools";
   cd "${HOME}/Downloads/bioTools";
else
   cd "${HOME}/Downloads/bioTools";
   git pull;
fi;

make -f mkfile.unix;
```

# Using:

To get the help message do `gbAccBin -h`.

To process a text file with accession numbers you can
  do `gbAccBin -txt <file>.txt -prefix out`. In the text
  file you can use `@` to comment out lines. Blank lines
  are ignored. Also, if the accession number ends in a
  space, everything else after is ignored
  (ex: `MK526900 ignored`). The output is saved in
  `<prefix>-<accession>_<assembly_id>.txt` for assembly
  accession ids. For non-assembly accession numbers it is
  `<prefix>-<accession>.txt`.

To process a fasta or fasta.gz file do
  `gbAccBin -fa <file>.fa -prefix out`. The assembly
  accession output is saved in
  `<prefix>-<accession>_<assembly_id>.fa`. For
   non-assembly accession numbers it is
  `<prefix>-<accession>.fa`.
