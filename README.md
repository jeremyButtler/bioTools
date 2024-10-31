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

# List of tools

- re-inventing the wheel:
  - alnwater (alnSeq/fluDI):
    - waterman alignment with gap extension penalties
    - slower than striped watermans
    - watermans are a dime a dozen
  - filtsam (freezeTB):
    - filter sam files by flag, length, median/mean
      q-scores, and coordinates
    - also supports read soft mask removal
    - samtools view nock off, with an odd twist
  - memwater (alnSeq/freezeTB):
    - waterman alignment with gap extension penalties
    - only returns score and alignment coordinates, but
      also uses less memory
    - watermans are a dime a dozen
  - primFind (fluDI/freezeTB):
    - find primers in sequence(s)
    - emboss primer finder program nock off (forgot name)
  - seqById (fqGetIds/find-co--infections):
    - extract sequences by read id from sam files and
      fastq files
    - seqkit grep nock off, without regular expressions
      and no fasta file support
  - tbCon (freezeTB):
    - reference based majority consensus that saves the
      cosensus as a sam file (convert cigar to eqx cigar
      with cigToEqx)
    - nock of of Ivar and who knows what other programs
    - positions of low confidence are replaced with a 'N',
      (no other anoymous bases are used)
- niche or odd programs
  - adjCoords (freezeTB):
    - adjust mapping coordinates from genes to entire
      genome
    - you need to provide a tsv file with gene names and
      coordinates
  - ampDepth (freezeTB):
    - finds mean read depth of amplicons and the genes
      they cover
    - you need to provide a coordinate file for this
    - convert regular cigars to eqx cigars (only one
      reference is supported)
  - adjMap (fluDI/freezeTB):
    - adjust mapping coordinates to a new reference
      - reference and reads must be sam files
    - needs some debugging still, but close
  - cigToEqx (freezeTB):
    - convert regular cigars to eqx cigars (only one
      reference is supported [TODO: fix])
  - krakTaxaId:
    - split up kraken reads by taxa id
    - side project, more of idea phase right now, so
      not working
  - edClust (fluDI/freezeTB):
    - needs testing, but is running
    - try to cluster reads by edit distance
  - edDist (fluDI/freezeTB):
    - finds modified edit distances
      - modified means only large indels and snps with
        q-scores > 6
      - for non-modified use the "NM:i:" entry in the
        sam file
      - can do sam file to sam file or by ref
    - not best program, but part of a bigger plan
  - illNano (freezeTB/fluDI dugging edClust):
    - finds Illumina variants from tbCon tsv file and then
      finds nanopore reads mapping Illumina profile
  - maskPrim (freezeTB):
    - mask primer sites by coordinates (for sam file)
    - tsv file with coordinates is needed
  - samToAln:
    - convert sam file to human readable alignment
    - uses first reference from file [TODO: fix])
    - this is how to get a viewable alignment for alnwater
    - emboss like format, except the "|"'s are replaced
      with cigar symbols "SIDX=".
  - trimSam (find-co--infections):
    - trims soft masking off reads in sam file
    - filtsam can do this with `-trim` and has filtering
      options

# genLib and genAln

My general libraries.

- genLib is for general libraries that are not related to
  biology.
- genBio is for general biology tasks
- genAln is for alignment tasks, such as pairwise aligners
  or primer scanning (biology)
- genClust is for clustering our grouping tasks (biology)

# Updates:

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
2. finish debugging ajdMap (side project)
   - not quite working, some reads get a few extra cigar
     entries (think due to softmasked end)
3. add in hirschberg/myers miller (personal project)
   - need to modify hirsch from alnSeq for new style
     and OS portablility. So, conversion project, with a
     bith of thought.
4. add using_this_code guides (using in C code) for
   programs and libraries
   - less likely to happen, lot of work for shoddy guides
5. work on bugs in graphAmpDepth.r
6. give samBin some attention (never conveted)
7. add in multi-reference support for samToAln and
   cigToEqx
8. get krakTaxaId working (current side project)

# Thanks

- Eric and Tara Ness, Bryce Inman, and the rest of the
  Bort's lab for providing a place to develop code
  - Also, some of this code was developed for or to
    debug freezeTB or fluDI
- Devin Drown for his mentor ship. seqById and hopefully
  edClust are improved version programs written for
  find-co--infections
- My dad for being there and supportive and a person to
  bounce ids off or talk to
