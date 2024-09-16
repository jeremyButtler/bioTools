# Use:

Finds an edit distance that accounts for errors between
  a mapped read and its reference or two reads mapped to
  the same reference. This is set up for Nanopore (ONT)
  sequencing.

By accounting for errors, I mean that SNPs with low
  quality scores are discarded. Small indels are also
  discarded.

Insertions between a non-mapped reference and query are
  not aligned. Instead the size of the indel is compared
  and the difference is treated as the distance. This
  makes things simpiler and faster, but results in less
  accuracy.

For non-modifed edit distances to the mapped reference use
  the "NM:i:" entry in the sam file (you just need
  something like awk).

```
awk '{
   if($6 == "*") next;
   sub(/.*NM:i:/, "", $12);
   sub(/[ \t].*, "", $12);
   print $1, $12;
 }' reads.sam
```

# License

Primary is unlicense (public domain), but if publice
  domain does not work or is not desired it defaults to
  MIT (alternate).

# Install:

## General unix local install

Installs for you only, but do not need permissions.

First; make sure the local install enviroment is set up

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
fi # permantly add $HOME/local/bin to your path

export "PATH=$HOME/local/bin:$PATH"

# you can remove $HOME/local/bin from your path by
# deleting the "export PATH=/home/<user name>/local/bin:"
# from your .bashrc file
```

Then install edDist 

```
cd $HOME/Downloads
git clone https://github.com/jeremybuttler/bioTools
cd bioTools/edDistSrc
make -f mkfile.unix
make -f mkfile.unix install PREFIX="$HOME/local/bin"
```

## General unix global install

Installs for everyone, but permissions are needed.

```
if [[ ! -d "$HOME/Downloads" ]]; then
   mkdir "$HOME/Downloads";
fi

cd $HOME/Downloads
git clone https://github.com/jeremybuttler/bioTools
cd bioTools/edDistSrc
make -f mkfile.unix
sudo make -f mkfile.unix install
```

## Windows

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

At this point copy the `edDist.exe` to your desired
  install location. Sorry I have no advice on were to
  install.


# Run

You can get the help message for edDist with `edDist -h`.

You can compare a reference or read mapped to the same
  reference by using `-ref ref.sam`. Use `-cmp-read` to
  set it for read to read error rates (doubles error
  rate).

```
edDist -sam reads.sam > distances.tsv
edDist -sam reads.sam -ref ref.sam -cmp-read > distances.tsv
```
