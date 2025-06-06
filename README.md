# Use:

Holds small tools I have made for biology programs. The
  tools are either niche tools, re-inventing the wheel, or
  just plain odd. They all should compile on Linux, BSDs,
  Mac, and Windows. I use them, but I would be suprised if
  other people used them.

These tools are often built to help me with my projects or
  are smaller parts of my projects. I have included the
  project they were designed for or first appeared in.

Still need to do OS tests on all tools. This is a spare
  time project, so does not get done.

# License

Primary is unlicense (public domain), but if publice
  domain does not work or is not desired it defaults to
  MIT (alternate).

# Installing

## Linux:

For X86 64 bit (amd/intel CPUs) Linux I have compiled
  static binaries with the musel libarary
  in `bin/x86_64_Linux`. Thanks to musl, these will be a
  bit smaller then what you can make.

Any binary ending in `AVX512`, `AVX2`, or `SSE` are
  SIMD versions of the program (currently only seqById).
  These will be faster then the regular version. However, 
  your CPU needs to support them. The fastest is `AVX512`,
  but that is the least likely to have any support. The
  next is `AVX2`, which your computer will likely support.
  If all else fails, then `SSE` will very likely work. If
  that fails, then the regular version should work or you
  are not using an `x86` 64 bit cpu.

## Unix (Mac/Linux/BSD):

You can build and install the entire repository using
  `mkfile.unix` or `mkfile.static` (do not use use static
  on Mac). You can also build and install individual
  programs. To get instructions for individual programs,
  read the README.md file in the programs direcotry.

```
if [ ! -d "/usr/local/bin" ];
then
   sudo mkdir -p "/usr/local/bin";
fi

cd ~/Downloads;
git clone https://github.com/jeremybuttler/bioTools;
cd bioTools;
make -f mkfile.unix;
sudo make -f mkfile.unix install;
```

For individual programs:

```
# only need to do this step once
if [ ! -d "/usr/local/bin" ];
then
   sudo mkdir -p "/usr/local/bin";
fi

cd ~/Downloads;
git clone https://github.com/jeremybuttler/bioTools;

# this step is done for each program
cd ~/Downloads/bioTools/<target-program>;
make -f mkfile.unix;
sudo make -f mkfile.unix install;
```

## Windows

Not tested yet, but here is the general idea. You will
  need to install the visual studio build tools. Then open
  a developer terminal.

```
cd "%HOMEPATH\Downloads\bioTools"
nmake /F mkfile.win
nmake PREFIX=bin\win /F mkfile.win
```

This should put all exe files in the `bin\win` directory.
  Move them to were they need to be.

These are command line programs, so you will need a
  terminal to run them.

# Scripts

The mkfileScripts directory has bash scripts to build
  makefiles for bioTools. If you plan on using my
  code, then you likely find buildMkfile.sh to be
  helpful in generating your make files.

- buildMkfile.sh is a bash script to build a make file
  for a specified OS type from the librarys
  - Input: OS program-name main-file librarys
  - Input: nothing or "-h" for help message
  - here is the example for unix tbCon
    - `bash buildMkfile.sh unix tbCon mainTbCon samEntry tbCon charCp`
  - Options:
    - bug: for debug (should not need)
    - unix: for general unix
    - static: for unix's that support static builds
    - win: windows (still need to test some programs)
    - plan9: for plan9 (have not tested all programs yet)
  - One of these days I will build a better system, this
    is my quick and dirty solution to OS portability.
- allOSMkfiles.sh is a bash scripts that calls
  buildMkfile.sh for all its OS's
  - Input: program-name main-file librarys
  - Input: nothing or "-h" for help message
  - Here is the example for the unix tbCon
  - Here is the example for tbCon
    - `bash allOSMkfiles.sh tbCon mainTbCon samEntry charCp tbCon charCp`
- allMks.sh is a bash script that makes all makes files
  for my working programs (except seqById)
  - call from top level (bash mkfileScripts/allMks.sh)
  - mostly for when I need to make a change and
    re-build all my make files at once

# testing:

The programs are built and debugged on Linux and then
  lightly tested on other OSs. So, not all features may
  tested on every OS. In this case it may use one or
  two light cases or just making sure it compiles. For
  windows, I currently only print `\n` (not `\r\n`) for
  end of lines.

# List of tools

- re-inventing the wheel:
  - alnNeedle (find-co--infections/alnSeq/fluDI):
    - needleman alignment with gap extension penalties
    - works on linux and plan9
  - alnWater (find-co--infecitons/alnSeq/fluDI):
    - waterman alignment with gap extension penalties
    - slower than striped watermans
    - Watermans are a dime a dozen
    - works on linux and plan9
  - filtsam (freezeTB):
    - filter sam files by flag, length, median/mean
      q-scores, and coordinates
      - for coordiantes use `-coords start,end`
    - also supports read soft mask removal
    - samtools view nock off, with an odd twist
    - works on linux and plan9
  - k2TaxaId (just wanted it):
    - split up kraken reads by taxa id using the kraken
      report
    - found out their is a similar tool to extract
      sequences instead of read ids (probably better) at
      [https://github.com/jenniferlu717/KrakenTools](
       https://github.com/jenniferlu717/KrakenTools
     )
    - works on linux complies on plan9
  - mapReads (freezeTB and maybe fluDI):
    - read mapper, slower than minimap2
    - main reason is I wanted a read mapper for freezeTB
      that I integrate and thus is always there
    - tested on Linux only
  - memwater (alnSeq/freezeTB):
    - waterman alignment with gap extension penalties
    - only returns score and alignment coordinates, but
      also uses less memory
    - Watermans are a dime a dozen
    - works on linux and plan9
  - primFind (fluDI/freezeTB):
    - find primers in sequence(s)
    - emboss primer finder program nock off (forgot name)
    - works on linux and plan9
  - revCmp (annoateASFV)
    - reverse complements sequences in fasta file
    - emboss nock off and who knows what else
  - seqById (fqGetIds/find-co--infections):
    - extract sequences by read id from fastq files and
      sam files
    - `seqkit grep` nock off, but without regular
       expression support and no fasta file support
    - works on linux and plan9
  - tbCon (freezeTB):
    - reference based majority consensus that saves the
      cosensus as a sam file (convert cigar to eqx cigar
      with cigToEqx)
    - nock of of Ivar and who knows what other programs
    - not best method for insertions (unsensitive)
    - positions of low confidence are replaced with a 'N',
      (no other anoymous bases are used)
    - works on linux and plan9
  - tranSeq (converted from asfv annotation)
    - translate fasta sequences to amino acids
    - emboss program knock of (think similar name) also,
      many of these exist already, 
  - unGz (freezeTB; for mapReads)
    - unzip .gz files (not .tar.gz), use gunzip -c instead
    - this is slower than gunzip, but is here to support
      mapRead
    - tested on linux only
- niche or odd programs
  - adjCoords (freezeTB):
    - adjust mapping coordinates from genes to entire
      genome
    - you need to provide a tsv file with gene names and
      coordinates
    - works on linux and plan9
  - ampDepth (freezeTB):
    - finds mean read depth of amplicons and the genes
      they cover
    - you need to provide a coordinate file for this
    - convert regular cigars to eqx cigars (only one
      reference is supported)
    - works on linux and plan9
  - cigToEqx (freezeTB):
    - convert regular cigars to eqx cigars
    - works on linux and plan9
  - edClust (fluDI/freezeTB):
    - try to cluster reads by edit distance
      - not very sensitive, but kinda works
    - works on linux and plan9
  - edDist (fluDI/freezeTB):
    - finds modified edit distances
      - modified means only large indels and snps with
        q-scores > 6
      - for non-modified use the "NM:i:" entry in the
        sam file
      - default is mapped reference comparision, but you
        can compare two sam entries (less accurate)
    - works on linux and plan9
  - illNano (freezeTB/fluDI; dugging edClust):
    - finds Illumina variants from tbCon tsv file and then
      finds nanopore reads mapping Illumina profile
    - works on linux and complies plan9 (not tested)
  - maskPrim (freezeTB):
    - mask primer sites by coordinates (for sam file)
    - tsv file with coordinates is needed
    - works on linux and plan9
  - rmHomo (freezeTB):
    - remove small indels in homopolymers (you pick sizes)
  - samToAln (freezeTB/fluDI; dugging aligners):
    - convert sam file to human readable alignment
    - this is how to get a viewable alignment for alnwater
    - emboss like format, except the "|"'s are replaced
      with cigar symbols "SIDX=" (more clearer)
    - works on linux and plan9
  - trimSam (find-co--infections):
    - trims soft masking off reads in sam file
    - filtsam includes trimSam with `-trim`
    - works on linux and plan9

# genLib and genAln

My general libraries.

- genLib is for general libraries that are not related to
  biology.
- genBio is for general biology tasks
- genAln is for alignment tasks, such as pairwise aligners
  or primer scanning (biology)
- genClust is for clustering our grouping tasks (biology)

# Updates:

- 2025-05-27
  - fixed bug in tranSeq were reverse reading frames
    (4 to 6) did not use the correct coordiantes when
    `-start` and `-end` were used
- 2025-05-26
  - fixed bug were rmHomo was not keeping refernce and
    query sequence in sync
  - fixed -coord-trim in filtssam not updating
    match, snp, del, ins, soft mask, and number aligned
    reference bases
- 2025-05-23
  - minor fixes to samEntry and tbCon .c files
  - findRefPos_samEntry (in genBio/samEntry.c) can now
    search backwards
- 2025-05-19:
  - fixed issue with filtsam `-coord-trim` setting
    reference position to the base ended on
- 2025-05-16:
  - fixed filtsams `-coord-trim` to trim by coordinates
    coorrectly
- 2025-05-13:
  - added new columns into illNanos `-out-uniq file.tsv` 
    output
- 2025-05-12:
  - separate samEntry into the core samEntry reading
    system (samEntry) and syste to read in references
    from sam file headers (samRef). This reduces compile
    size of each program by 4kb, saving 200 to 300kb in
    static binrary sizes.
- 2025-05-09:
  - fixed bug in fastq read part of primFind + push all
    changes since 2025-04-30
- 2025-05-08:
  - fixed alignment output errors in mapRead
  - added trim start and end options to filtsam
- 2025-05-06:
  - samEntry and seqST variables were changed
  - samEntry uses small, bufferd read in system now
  - filtSam can now filter by reference
  - filtSam now uses `-in-range` by default
  - add fileFun library to reduce potential OS line break
    issues
- 2025-04-30:
  - ../genLib/fileFun now has file reading functions
  - fixed error were last charcter in extra entry in
    sam files was removed
  - also added fileFun functions to multiple files
- 2025-04-29:
  - fixed segfault bug in kmerFind, might have sped it up
    to
  - added in fileFun.c file for getting number of lines
    in a file
- 2025-04-28:
  - added gz file support to mapRead and removed the `-fq`
    option
  - added `get_seqST` function to ../genBio/seqST.c for
    reading in fastx and fastx.gz files
  - removed global make files (not intended, but happened)
- 2025-04-24
  - changed ampDepth to print only hit genes in tables
    instead of first gene to last gene
  - added revCmp
  - added unGz
  - added in better line break support (maybe)
  - mkFileScripts are now sh complinent
- 2025-04-01
  - fixed error were samEntry would remove some bases from
    the end of a sequence when resizing the buffer during
    the read from file step
  - fixed break and complier error in memwater from
    2025-03-31 update
  - added in install all programs makefiles
- 2025-03-31
  - mapReads added
- 2025-03-24
  - added new lines support for each OS
    - windows is `\r\n`
    - Unix (Linux/Mac/BSD) is `\n`
- 2025-03-12
  - fixed pLen and tLen being mapq when printing sam
    files.
  - fixed rNext not being set to '*' in blank_samEntry
    (also added support for rNext = '\0' in p_samEntry)
  - fixed memory leak in tbCon
  - added rmHomo
  - added various small projects (cgMLST/mapRead/mkPng),
    none are in a working phase
- 2025-02-25
  - minor improvments to prim find to improve memory usage
    at higher kmer sizes
  - added tranSeq (translation program)
- 2024-01-07:
  - bug fixes (tbCon) and some general libraries
  - cigToEqx mulit-reference support
  - samToAln mulit-reference support
- 2024-12-12:
  - added awk gene coordianteds extraction script for
    exracting coordiantes from genbank genes fasta file
    - sed script had error
  - fixed bug in graphAmpDepth
  - fixed minor bugs in ulCp
- 2024-12-08:
  - fixed bug in samEntry that cause tbCon to crash
  - fixed divide by 0 error in tbCon (found thanks to
    edClust crashing in plan9 (unix ignored))
- 2024-12-05:
  - removed rmBlocks (added to different repository called
    build, will make public when farther along)
  - moved adjMap and samBin oldCode since they are
    incomplete and no longer being worked on any more
  - updated make files
  - converted my forward delcarations
    from (`typedef struct name name;`) to 'struct name;`
    - got warnings from Mac's clang that tye old form
      was not c89 complement. The new form is.
  - using code guide for genLib. Not great, but their.
  - added in special datatype for seqById and ulCp to
    allow 64 bit to be set on plan9 [need compiler flag]
    (plan9 likes to assign longs to 32 bit)
- 2024-11-19:
  - added rmBlocks (pretty much useless)
- 2024-11-13:
  - added k2TaxaId program
- 2024-10-31:
  - replaced program specific version numbers with a
    general "bioTools" version (less mantinace)
  - added in detection of carraige returns for samEntry
    and other programs (more windows freindly)
    - vector (scalar does) seqById does not recongize
      carriage returns, but this should not affect
      performace, so not worth changing.

# TODO:

1. make replacement for make (different repo/side project)
   - make is getting to clunky, need to take some time to
     make a OS portable replacement that is more geared to
     projects like bioTools (many programs referencing the
     same set of libraries)
2. add in hirschberg/myers miller (personal project)
   - need to modify hirsch from alnSeq for new style
     and OS portablility. So, conversion project, with a
     bith of thought.
3. add in my memWater matrix scan from alnSeq (personal
   project)
4. add using_this_code guides (using in C code) for
   programs and libraries
   - less likely to happen, lot of work for shoddy guides
5. add SIMD support to mapRead
   - likely will happen once get freezeTB to more polished
     state
6. ampDepth
   - allow ampDepth to print read depth for each position
   - set non-coordinate input to print depth for amplicons
     (no breaks in depths)
   

# Thanks

- Eric and Tara Ness, Bryce Inman, and the rest of the
  Bort's lab for providing a place to develop code
  - Also, some of this code was developed for or to
    debug freezeTB or fluDI
- Devin Drown for his mentor ship. seqById and hopefully
  edClust are improved version programs written for
  find-co--infections, which I worked on while under his
  mentorship.
- My dad for being there and supportive and a person to
  bounce ids off or talk to
