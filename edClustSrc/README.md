# Use:

Clusters mapped reads by a modified edit distance. This
  is slow and often only clusters a fraction of reads.

Note: for insertions; edit distance only compares the
  insertion size (large insertion only). it does not
  re-align overlapping insertion regions.

Plenty of alternatives (ex. Hairsplitter).

# License

Primary is unlicense (public domain), but if public
  domain does not work or is not desired it defaults to
  MIT (alternate).

# Build:

Each make file (mkfile) is prefixed by the OS type
  (mkfile.OS) it builds.

- Make files
  - mkfile.unix: general unix
  - mkfile.static: static builds for Linux/BSD only
  - mkfile.win: for windows

# General unix

## Local install

The disadvantage of a local install is that it is only
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

Then install edClust

```
# install edClust locally

cd $HOME/Downloads
git clone https://github.com/jeremybuttler/bioTools
cd bioTools/edClust
make -f mkfile.unix
make -f mkfile.unix install PREFIX="$HOME/local/bin"
```

To enable status reporting during clustering
   replace `make -f mkfile.unix`
   with `make -f mkfile.unix EXTRA_FLAGS=-DP_STATUS`. This
   is not enabled by default becaus it is a bit buggy
   on non-unix (plan9, maybe windows) systems.

## global install

Everyone has access, but you need to install as root.

```
if [[ ! -d "$HOME/Downloads" ]]; then
   mkdir "$HOME/Downloads";
fi

# install edClust globally

cd $HOME/Downloads
git clone https://github.com/jeremybuttler/bioTools
cd bioTools/edClust
make -f mkfile.unix
sudo make -f mkfile.unix install
```

You will be prompted for your user password.

To enable status reporting during clustering
   replace `make -f mkfile.unix`
   with `make -f mkfile.unix EXTRA_FLAGS=-DP_STATUS`. This
   is not enabled by default becaus it is a bit buggy
   on non-unix (plan9, maybe windows) systems.

# Windows

Make sure the developer terminal is installed (install
  visual studio or at least the developer terminal part of
  it). Download the bioTools repository to your Downloads
  directory (folder). Open the developer console
  (start menu->visual studio->terminal). If you started
  visual studio then go to view->terminal.

```
cd %HOME%\Downloads\bioTools\edClust
nmake /F mkfile.win
```

At this point copy the `edClust.exe` to your desired
  install location. Sorry I have no advice on were to
  install.

# Using:

You can get a help message for edClust with `edClust -h`.
  It is long, but has every command.

To use edClust provide a sam file with `-sam file.sam` and 
  a prefix to call the output with `-prefix <name>`. The
  output will be prefix-log.txt with the log,
  prefix-con.sam with the consensuses (mapped to their
  references), and a sam file for each cluster
  (prefix-reference-cluster.sam).

For progress the number of reads remaing for clustering
  are printed to stderr. It will take a bit to cluster
  large files.

The memory usage in bytes is roughly
  29 * (number lines in sam file).

```
minimap2 -a reference.fasta reads.fastq > reads-map.sam
edClust -sam reads-map.sam -prefix reads-map-clust
```

# How works:

The first step in edClust is to find a rough count of the
  number of lines in the sam file (for memory allocation).
  After that reads are scored and indexed. In the indexing
  step the file position, length of line, reference the
  read mapped to (as number), the start of read on the
  reference, and end of read on reference are recorded.

The scoring formula is score = floor(floor(log10(aligned length)) * weight) + floor(median q-score).
   The default weight is 2.

In the clustering step the read with the highest score
  (best read) is chosen. The edit distance to other reads
  overlapping the "best read" is found. The edit
  distance is divided by the expected number of errors
  (error rate (4.6%) * overlap length) to get
  the percentage of variants to errors. Reads with
  percentages over 10% are not clustered.

For reads with having at least one window (at least 500
  bases), the maximum window error rate is used to scan
  for regions that have a high number of differences.
  Not very complex, just gets the number of varaints in
  each 500 base long chunk.

For clusters with to few reads, the best read is discarded
  and a new best read is found.

Clusters with enough reads are then built into a consensus
  with tbCon (low read depth positions masked instead of
  fragmented). The reads are then remapped to the
  consensuses (error rate changes to 2.3%) and the
  consensus is then rebuilt (default is one time).

Consensuses with to many masked bases are discarded. This
  is to remove consensuses with low read depths or
  disagreements at variable positions.

The consensus is then compared to past built consensuses.
  Highly similar (> 99%) consensuses are merged (including
  their reads) into a single cluster. The reads assigned
  to the cluster are removed from the pool of reads. If
  long enough, windows are also used to see if same.

A new best read is then found and the cycle repeats until
  the number of reads remaining is less then the min read
  depth.

After clustering, consensuses sharing 75% overlap
  and a similarity > 99% are merged. Window scanning is
  also done to separate out long reads with regions with
  large differnces.

After merging, the percentage of mapped reads filtering
  step is done. First, the number of reads mapping with
  at least a 75% overlap to each clusters positions is
  found. Then the percentage of depth for the cluster is
  found (number clustered reads) / (number reads).
  Clusters with low read depth percentages (under 0.1%)
  are removed.

As you can guess, this will take some time to run.

# Updates

- 2025-01-07:
  - added in clustering logging
  - fixed multi-reference crash issues
