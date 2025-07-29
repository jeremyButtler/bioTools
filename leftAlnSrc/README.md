# Use:

leftAln left aligns deletions in  homopolymers and
  insertions that are the same base as the homopolymers
  they are in.

Note: most read mappers, such as minimap2 will
  automatically do this.

Examples of what leftAln does:

```
aaaa    aaaa
==D= --> D===
aa-a    -aaa

aa-a    -aaa
==I= --> I===
aaaa    aaaa

aa-a    aa-a
==I= --> ==I= <--This is not a matching insertion, so it
aata    aata      is not left aligned.
```

leftAln assumes that you never had both a deletion and
  an insertion (of the same base) in the same homopolymer.
  This case would be an error on the aligners part, so
  this case should never happen.

Ex:

```
aaa-aa     a-aaaa
==DI== --> DI====
aa-aaa     -aaaaa

aa-aaa     -aaaaa
==ID== --> ID====
aaa-aa     a-aaaa
```

# Install:

## Unix:

### Unix Globally

```
if [ ! -d "/usr/local/bin" ];
then
   sudo mkdir -p "/usr/local/bin";
fi;

cd ~/Downloads;
git clone https://github.com/jeremyButtler/bioTools;
cd bioTools/leftAlnSrc;
make -f mkfile.unix;
sudo make -f mkfile.unix install;
```

### Unix Locally

```
if [ ! -d "~/usr/bin" ];
then
   mkdir -p "~/usr/bin";
fi;

cd ~/Downloads;
git clone https://github.com/jeremyButtler/bioTools;
cd bioTools/leftAlnSrc;
make -f mkfile.unix;
make PREFIX="${HOME}/usr/bin"-f mkfile.unix install;

pathBl="$(grep "export PATH=\"\${HOME}\/usr\/bin" ~/.bashrc)";

if [ "$pathBl" = "" ];
then
   printf "export PATH=\"\${HOME}/usr/bin:\$PATH\n" >> ~/.bashrc";
   export PATH="${HOME}/usr/bin:$PATH";
fi; # make sure ~/usr/bin is in your path
```

# Using leftAln:

Do `leftAln -h` for the help message (not many options).

Do `leftAln -ref /path/to/reference.fasta -sam /path/to/file.sam > aligned.sam`
  to left align reads in a sam file.
