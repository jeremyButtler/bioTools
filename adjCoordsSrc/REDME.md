# Use:

Adust gene mappings to reference mappings.

Logic is if I am only ampifying 20 genes, then why am I
  alinging those 20 genes to a 4mb genome with 4000 genes
  for each sequencing run. Instead, lets align the genes
  (or amplicons) to the 4mb reference once and then align
  each runs reads to the 20 genes (or amplicons). After
  that do a quick step to ajdust coordianates.

# License

Primary is unlicense (public domain), but if publice
  domain does not work or is not desired it defaults to
  MIT (alternate).

# Build:

Each make file (mkfile) is prefixed by the OS type
  (mkfile.OS) it builds.

- mkfile.unix: general unix
- mkfile.static: static builds for Linux/BSD
- mkfile.win: for windows

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

Then install adjCoords

```
# install adjCoords locally

cd $HOME/Downloads
git clone https://github.com/jeremybuttler/bioTools
cd bioTools/adjCoordsSrc
make -f mkfile.unix
make -f mkfile.unix install PREFIX="$HOME/local/bin"
```

## global install

Everyone has access, but you need to install as root.

```
if [[ ! -d "$HOME/Downloads" ]]; then
   mkdir "$HOME/Downloads";
fi

# install seqById globally

cd $HOME/Downloads
git clone https://github.com/jeremybuttler/bioTools
cd bioTools/adjCoords
make -f mkfile.unix
sudo make -f mkfile.unix install
```

You will be prompted for your user password.


# Using:

You can print the help message with `adjCoords -h`.

You will need to provide a gene table (same format as
  ampDepth) and a sam file to remap to.

The gene table names should be the same as the reference
  mapped to. It should look like

```
gene	reference	direction	start	end
danA	NC000962.3	F	1	1525
pknB	NC000962.3	R	15590	17460
```

Running

1. build gene coordinates table

```
minimap2 -a reference.fasta genes.fasta |
  awk '
     BEGIN{OFS=FS="\t"};

     {
        if($1 ~ /^@/)
           next;
        startSI = $4;
        endSI = startSI + length($10);

        if($2 != 0 && $2 % 16 == 0)
           dirSC = 'R';
        else
           dirSC = 'F';
        printf $1, $3, dirSC, startSI, endSI
     }'
  > genes-table.tsv;
```

2. map reads and adjust

```
minimap2 -a genees.fasta reads.fastq > reads.sam |
  adjCoords -coords gene-table.tsv -sam - -out reads-adj.sam
```
