# Use:

Clean up indels in homopolymer regions.

Logic: ONT sequecing often has indel errors in
  hompolymers. As a result, we often remove any small
  indels in hompolymers in the consensus during the manual
  curation step. Since these would be removed anyways, why
  not remove them before the consensus building and
  analysis step.

This is a bit rough, in that is works on deletions as
  events and not individual items. So, if you have a
  small 3 base pair deletion at the end of a homopolymer,
  it will remove the entire deletion. This includes the
  parts of the deletion not in the homopolymer. The reason
  for this is that I wanted to get this up quickly. Also,
  I figure this would be a rare event.

Here is an example

```
agaCCCCCatga
agaGCCC---ta

would go to

agaCCCCCatga
agaGCCCCatta
```

# License

Dual licensed under Unlicense or MIT. You pick the one
  that works best for you.

# Install:

## Unix:

```
cd ~/Downloads;
git clone https://github.com/jeremybuttler/bioTools;
cd bioTools/rmHomo;
make -f mkfile.unix;
sudo make -f mkfile.unix install;
```

## Windows:

Output sam files lines end with `\n`, not `\r`.

You need the C/C++ build tools from visual studio, or
  some other C compiler. I am assuming your are using
  visual studio build tools.

Download git hub repository to Downloads. Then extract
  (unzip) the repository. Open a developer terminal and
  move into
  bioTools `cd "%HOMEPATH%\Downloads\bioTools\rmHomo".
  You may have to modify the final part depending on
  the naming from github. Then do `name /F mkfile.win`.
  After this move `rmHomo.exe` to your install location.
  
# Use:

You can get the help message with `rmHomo -h`.

You can run
  with `rmHomo -ref reference.fasta -sam reads.sam -out output.sam`

Deletions that have been overwritten have the quality for
  the inserted reference base set to `!`, which is the
  symbol for a quality score of 0. However, if you want
  to mask replaced deletions, you can use `-mask N`.
