# Use:

Extract read ids from kraken output using assigned taxa
  from report.

Not perfect, but it does work.

# License:

Dual licensed under the Unlicense and MIT. The default
  license is the Unlicense, with defaulting to MIT if
  the Unlicsense is not usable. However, you are also free
  to pick license that works best for you.

# Install:

## Unix:

```
if [[ ! -d "/usr/local/bin" ]]; then
   sudo mkdir -p "/usr/local/bin";
fi # deals with issue on Mac (Mac still has in path)

cd ~/Downloads
git clone https://github.com/jeremyButtler/bioTools
cd bioTools/k2TaxaIdSrc
make -f mkfile.unix
```

## Windows (not tested yet):

Download bioTools from github and unzip. Then open a
  developer console

```
cd $HOME$/path/to/bioTools/k2TaxaIdSrc;
nmake /F mkfile.win
```

copy k2TaxaId.exe to your install location

# Run:

You will need the default report (`--report file.tsv`,
  **DO NOT** use `--report-minimizer-data`) from kraken2.
  You will also need the output file `--output file.tsv`
  with the ids.

Example workflow:

```
kraken2 \
   --db ~/files/k2-db/k2_20240904_pluspf_08Gb \
   --output ids.tsv \
   --report report.tsv \
   reads.fastq;

k2TaxaId \
    -report report.tsv \
    -id ids.tsv \
    -preifix goodName;
```

You can view the help message with `k2TaxaId -h`.

You can controll the range of taxa printed by using
  `-start` (nearest to root of tree) and `-end`
  (nearest to tip of tree).

By default k2TaxaId merges reads into the taxa levels
  nearest the tip (`-end`). This may mean multiple read
  ids may be assigned to multiple files. You can disable
  this with `-no-merge-root`.

Also by default k2TaxaId merges reads after `-end` into
  the first printable root taxa. You can disable this
  with -no-merge-tip`.

At some point I should set this up so that low read depth
  tips are merged into roots, but I have not set that up
  yet. Probably never will.

# Updates:
