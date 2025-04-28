# Use:

Detects primers in sequence. It is slow, but does ok for
  short primers that programs like minimap2 tend to
  struggle with.

# License:

Dual licensed, you pick: Unlicense or MIT license.

# Install Unix

```
git clone https://github.com/bioTools
cd bioTools/primFindSrc

if [ ! -d "/usr/local/bin" ];
then
   sudo mkdir "/usr/local/bin"
fi

make -f mkfile.unix
sudo make -f mkfile.unix install
```

# Install windows

This is a command line tool, so there is no GUI interace.

1. Install visual stuido build tools
2. Open a developer terminal (under visual studio)
3. Download bioTools repistory and unzip

```
cd %HOMEPATH%\Downloads\bioTools\primFindSrc
nmake /F mkfile.win
```

Copy the primFind.exe file to your desired location.

# Run

To get the help message do `primFind -h`.

Primers can be in two formats, as a fasta file `-prim-fa`
  or a tsv file `-prim-tsv`. The tsv file allows you to
  specify if primers are paired (both forward and reverse
  is needed to call).

For the tsv file the first column is the primer name (id).
  The second column is if it is paired (True) or not
  (False). The third column is the forward sequence (use
  "NA" for no forward primer). The fourth (last) column is
  the reverse sequence (use "NA" for no reverse primer).

```
name	paired	for_seq	rev_seq
primer1	True	atcgagg	gctggatg
primer2	False	gctagaa	cctatgca
primer3	False	NA	ttacgtaa
primer3	False	catgcaa	NA
```

Run primFind to scan a fasta file with primers in a tsv
  file `primFind -prim-tsv primers.tsv -fa ref.fasta`

Run primFind to scan a fastq file using primers in a fasta
  file `primFind -prim-fa primers.fasta -fa reads.fastq`

The ouput is a tsv file with the coordinates of the found
  primers and scores.
