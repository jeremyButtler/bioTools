# Use:

Splits reads into separate sam files by the reference they
  mapped to. This can also process refseq ids to look
  for accession ids that belong to the same assembly.

# License

Dual licensed under the Unlicense and MIT license. The
  Unlicense is the authors preferred license. In cases
  were the Unlicense is not respected or not wanted,
  please use the MIT license.

# Install

## Unix:

```
if [ ! -d "${HOME}/Downloads" ]; then
   mkdir "${HOME}/Downloads";
fi;

git clone \
    https://github.com/jeremyButtler/bioTools \
    "${HOME}/Downloads/bioTools;
cd "${HOME}/Downloads/bioTools/binSamSrc";
make -f mkfile.unix;
sudo make -f mkfile.unix install;

# for local installs do
#  make -f mkfile.unix;
#  make -f mkfile.unix PREFIX="${HOME}/local/bin" install;
```

# Usage:

To get the help message for binSam do `binSam -h`.

To spilt reads by reference and merge refseq assemblies
  do `binSam -sam mapped-reads.sam -prefix out`.

To spilt reads by reference and not merge refseq
  assemblies
  do `binSam -strict -sam mapped-reads.sam -prefix out`.
