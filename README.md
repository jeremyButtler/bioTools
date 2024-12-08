# Use:

Holds small tools I have made for biology programs. The
  tools are either niche tools, re-inventing the wheel, or
  just plain odd. They all should compile on Linux, BSDs,
  Mac, and Windows (without cwygin). I use them, but I
  would be suprised if other people used them. Basically
  here incase someone else decides they need one.

These tools are often built to help me with my projects or
  are smaller parts of my projects. I have included the
  project they were designed for / first appeared in.

Still need to do OS tests on all tools. This is a spare
  time project, so does not get done.

# License

Primary is unlicense (public domain), but if publice
  domain does not work or is not desired it defaults to
  MIT (alternate).

# Scripts

The mkfileScripts directory has bash scripts to build
  makefiles for bioTools.

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
- allOSMkfiles.sh is a bash scripts that calls
  buildMkfile.sh for all its OS's
  - Input: program-name main-file librarys
  - Input: nothing or "-h" for help message
  - Here is the example for the unix tbCon
  - Here is the example for tbCon
    - `bash allOSMkfiles.sh tbCon mainTbCon samEntry charCp tbCon charCp`

# testing:

The programs are built and debugged on Linux and then
  lightly tested on other OSs. So, not all features may
  tested on every OS. In this case may invovle one or
  two light cases or just making sure it complies.

# List of tools

- re-inventing the wheel:
  - filtsam (freezeTB):
    - filter sam files by flag, length, median/mean
      q-scores, and coordinates
    - also supports read soft mask removal
    - samtools view nock off, with an odd twist
    - works on linux and plan9
  - alnNeelde (find-co--infections/alnSeq/fluDI):
    - needleman alignment with gap extension penalties
    - needlemans are a dime a dozen. This one rivals
      biopythons pairwise alignmer for memory and might
      even be faster (the alnSeq needleman python lib
      was).
    - works on linux and plan9
  - alnWater (find-co--infecitons/alnSeq/fluDI):
    - waterman alignment with gap extension penalties
    - slower than striped watermans
    - watermans are a dime a dozen
    - works on linux and plan9
  - memwater (alnSeq/freezeTB):
    - waterman alignment with gap extension penalties
    - only returns score and alignment coordinates, but
      also uses less memory
    - watermans are a dime a dozen
    - works on linux and plan9
  - primFind (fluDI/freezeTB):
    - find primers in sequence(s)
    - emboss primer finder program nock off (forgot name)
    - works on linux and plan9
  - seqById (fqGetIds/find-co--infections):
    - extract sequences by read id from sam files and
      fastq files
    - seqkit grep nock off, without regular expressions
      and no fasta file support
    - works on linux and plan9
  - tbCon (freezeTB):
    - reference based majority consensus that saves the
      cosensus as a sam file (convert cigar to eqx cigar
      with cigToEqx)
    - nock of of Ivar and who knows what other programs
    - positions of low confidence are replaced with a 'N',
      (no other anoymous bases are used)
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
    - convert regular cigars to eqx cigars (only one
      reference is supported [TODO: fix])
    - works on linux and plan9
  - edClust (fluDI/freezeTB):
    - needs testing, but is running
    - try to cluster reads by edit distance
    - works on linux and plan9
  - edDist (fluDI/freezeTB):
    - finds modified edit distances
      - modified means only large indels and snps with
        q-scores > 6
      - for non-modified use the "NM:i:" entry in the
        sam file
      - can do sam file to sam file or by ref
    - not best program, but part of a bigger plan
    - works on linux and plan9
  - illNano (freezeTB/fluDI; dugging edClust):
    - finds Illumina variants from tbCon tsv file and then
      finds nanopore reads mapping Illumina profile
    - works on linux and complies plan9 (not tested)
  - maskPrim (freezeTB):
    - mask primer sites by coordinates (for sam file)
    - tsv file with coordinates is needed
    - works on linux and plan9
  - samToAln (freezeTB/fluDI; dugging alingers):
    - convert sam file to human readable alignment
    - uses first reference from file [TODO: fix])
    - this is how to get a viewable alignment for alnwater
    - emboss like format, except the "|"'s are replaced
      with cigar symbols "SIDX=".
    - works on linux and plan9
  - trimSam (find-co--infections):
    - trims soft masking off reads in sam file
    - filtsam can do this with `-trim` and has filtering
      options
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
     projects like bioTools (many programs referencing
     same set of libraries)
2. add in hirschberg/myers miller (personal project)
   - need to modify hirsch from alnSeq for new style
     and OS portablility. So, conversion project, with a
     bith of thought.
3. add using_this_code guides (using in C code) for
   programs and libraries
   - less likely to happen, lot of work for shoddy guides
4. add in multi-reference support for samToAln and
   cigToEqx

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
