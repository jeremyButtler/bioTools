# Use:

Extracts sequences using read ids from fastq and sam
  files. The read ids should be distinguished by numbers
  (0-9 and a-f).

# License

Primary is unlicense (public domain), but if publice
  domain does not work or is not desired it defaults to
  MIT (alternate).

# Updates:

- 2024-09-15: added in header entry for output sam files

# Build:

Each make file (mkfile) is prefixed by the OS type
  (mkfile.OS) it builds for and if it includes vector
  (SIMD) support (mkfile.OS.SIMD). The SIMD support could
  be better inlined, but it does work.

- Make files
  - mkfile.unix: general unix, no SIMD support
    - mkfile.unix neon: unix with NEON support (MAC/ARM)
    - mkfile.unix.sse: unix with SSE suport
    - mkfile.unix.avx2: unix with AVX2 support
    - mkfile.unix.avx512: unix with AVX512 support
  - mkfile.static: static builds for Linux/BSD, no SIMD
    - mkfile.unix neon: unix with NEON support (MAC/ARM)
    - mkfile.unix.sse: unix with SSE suport
    - mkfile.unix.avx2: unix with AVX2 support
    - mkfile.unix.avx512: unix with AVX512 support
  - mkfile.win: for windows, no SIMD support
    - you could get SIMD support, just have to add it in

# General unix

## Local install

The disavatange of a local install is that it is only
  available to you. However, it does not need any extra
  permissions.


First make sure your local enviroment is set up.

```
# make sure have local install and Downloads directory

if [[ ! -d "$HOME/Downloads" ]]; then
   mkdir "$HOME/Downloads";
fi

if [[ ! -d "$HOME/local/bin" ]]; then
   mkdir -p "$HOME/local/bin";
fi

# check if have path setup to local (add to path if not)

localBl="$(grep "$HOME/local/bin" <<< "$PATH")"

if [[ "$localBl" == "" ]]; then
   printf "export \"PATH=$HOME/local/bin:$PATH\"\n" >> $HOME/.bashrc;
fi

export "PATH=$HOME/local/bin:$PATH"

# you can remove $HOME/local/bin from your path by
# deleting the "export PATH=/home/<user name>/local/bin:"
# from your .bashrc file
```

Then install seqById

```
# install seqById locally

cd $HOME/Downloads
git clone https://github.com/jeremybuttler/bioTools
cd bioTools/seqByIdSrc
make -f mkfile.unix
make -f mkfile.unix install PREFIX="$HOME/local/bin"
```

For 32 bit systems
  do `make -f mkfile.unix CFLAGS=-O3 -DTHIRTY_TWO_BIT`
  instead of `make -f mkfile.unix`. Not tested, but it
  should work.

## global install

Everyone has access, but you need to install as root.

```
if [[ ! -d "$HOME/Downloads" ]]; then
   mkdir "$HOME/Downloads";
fi

# install seqById globally

cd $HOME/Downloads
git clone https://github.com/jeremybuttler/bioTools
cd bioTools/seqByIdSrc
make -f mkfile.unix
sudo make -f mkfile.unix install
```

You will be prompted for your user password.

For 32 bit systems
  do `make -f mkfile.unix CFLAGS=-O3 -DTHIRTY_TWO_BIT`
  instead of `make -f mkfile.unix`. Not tested, but it
  should work.

# Windows

Make sure the developer terminal is installed (install
  visual studio or at least the developer terminal part of
  it). Download the bioTools repository to your Downloads
  directory (folder). Open the developer console
  (start menu->visual studio->terminal). If you started
  visual studio then go to view->terminal.

```
cd %HOME%\Downloads\bioTools\seqByIdSrc
nmake /F mkfile.win
```

For 32 bit systems
  do `make -f mkfile.unix CFLAGS=/O2 /Ot /Za -DTHIRTY_TWO_BIT /Tc`
  instead of `make -f mkfile.unix`. Not tested, but it
  should work.

At this point copy the `seqById.exe` to your desired
  install location. Sorry I have no advice on were to
  install.

# Using

seqById prints a help message with `seqById -h`.

You can get ids from fastq files using `-fq file.fq` and
  from sam files using `-sam file.sam`.

To provide a list of read ids use `-id file.ids`. The only
  expectation is that read ids are the first item and end
  with whites space (space, tab, or newline). This means
  you can do `seqById -id file.sam -fq file.fastq`. It
  does not matter if read ids start with an `@` or an
  '>' (orginally planed fasta, but not implemented).
  
```
seqById -id file.ids -fq file.fastq -out extracted.fastq
seqById -id file.ids -sam file.sam -out extracted.sam
```

# How seqById works

1. ids are read in and converted to hex based numbers
   (idLkTbl.h has conversion table) and the id sum is
   found
   - converion ends at space, tab, or new line
   - all id values are summed up into a single 32 bit
     (16 bit for -DTHIRTY_TWO_BIT) number
   - there is overlap, so not best method for fasta files
   - 0-9 + a-f is 0 to 15
   - g-t is 1 to 14
   - u-z is 1 to 6
   - everything else (special characters) are 0
   - blanks/padding are -1
2. ids are sorted by hash value (kunths mulitlpicative
   hash), then by least to greatest
   - (id sum * golden number) >> (64 - log2(hash size))
     - (64 - log2(hash size)) goes to
       (32 - log2(hash size)) on 32 bit systems.
3. the number of reads per hash is recorded and the
   index's of the first read with the hash is stored in
   the hash table.
4. the fastq/sam file is read in and the read id is
   converted to hex and a sum
5. the hash for the read id is found, then used to find
   index of potential matching read ids
6. a binary search is used to search all read ids with the
   same hash for the target read id.
7. if a match is found, the entry is printed, else not
   - `-v` makes it so matches are not printed.

I have a few tricks for parsing the fastq file. First, for
  the default install I use unsigned longs to search for
  end of lines (ulCp.c/h). Basically sudo vectors. For
  SIMD I used vectors to find the ends of lines.

I also store the input in a large buffer (multiple reads
  in one buffer to reduce file IO costs. I do a similar
  thing with the output (large buffer that is printed
  out at once).
