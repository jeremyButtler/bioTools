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

It is best to use the default report (`--report file.tsv`,
  however, you can use the report
  from `--report-minimizer-data` by using -minimize.  You
  will also need the output file `--output file.tsv` with
  the ids. Output will be saved to the name supplied 
  by `-prefix name`. Output files are
  named `prefix-taxaId-orginismName.ids`.

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

```
kraken2 \
   --db ~/files/k2-db/k2_20240904_pluspf_08Gb \
   --output ids.tsv \
   --report report.tsv \
   --report-minimizer-data \
   reads.fastq;

k2TaxaId \
    -report report.tsv \
    -minimize \
    -id ids.tsv \
    -preifix goodName;
```

Currently reads are appended to files, so old data is
  appended to instead of being overwriten. Need to change
  this at some point.

You can view the help message with `k2TaxaId -h`.

You can controll the range of taxa printed by using
  `-start` (nearest to root of tree) and `-end`
  (nearest to tip of tree).

When using `-no-merge-root` to avoid merging root ids
  into the tips, make sure you set `-end` to a less
  specific level, such as `-end S` instead of `-end S15`.

By default k2TaxaId merges reads into the taxa levels
  nearest the tip (`-end`). This may mean multiple read
  ids may be assigned to multiple files. You can disable
  this with `-no-merge-root`.

Also by default k2TaxaId merges reads after `-end` into
  the first printable root taxa. You can disable this
  with -no-merge-tip`.

# Updates:

- 2025-06-11:
  - fixed inifinite loop issue with `-no-merge-root` and
    setup so so that `-no-merge-root -no-merge-tip` will
    print any taxa at or after `-end <taxa level>`
- 2024-12-04:
  - fixed issues with missing ids from levels or ids
    assigned to wrong taxa
  - fixed issues with incorrect output and removed name
    limits (previosly was 64 characters, but some names
    are longer).
  - added low read depth or out of range tip merging
  - added ability to process kraken2 minimizer reports
    (krakenUnique) (`kraken2 --report-minimizer-data ...`)
