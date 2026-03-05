# Use:

Has a list of updates I have tracked for bioTools

- 2026-03-04
  - found a bug in k2TaxaId were the history array that
    printed out the read ids (function 12 in k2TaxaId.c)
    would go past 32 items when to many species were
    present. This would overwrite the id array. This has
    been fixed.
  - getLin.c had a max file name of 64 chars. In some
    cases the file path would be longer. It know has a
    size of 1024 bytes
  - using valgrind with inflate would sometimes have a
    "using unitialized values in checks". This was do to
    ulCp checking 8 bytes at a time and was just noise. I
    fixed inflate.c to initialize the window array to
    remove this noise.
- 2026-03-03
  - added in multi file support for
- 2026-02-27
  - fixed bug were k2TaxaIds would not merge child taxa
    (tips) into parents (roots) [ex: species into family].
  - changed k2TaxaId defaults to start at family and end
    at Genus (0). For kraken2 the species level seems to
    noisy.
  - added a bunch of code I am currently testing (genSort,
    genTree, mkMSTSrc)
- 2026-02-19:
  - fixed an error in k2TaxaId were it would not correct
    `/`'s to a safe character. The kranken2 references
    have all invalid file names characters replaced with
    `_`.  Valid characters are `0-9`, `A-Z`, `a-z`, `-`,
    and `.`
- 2026-02-17:
  - found error in geneCoords were the inner sorting was
    ending early. This was fixed and ampDepth should now
    work.
- 2026-01-29:
  - fixed getAmr inifinite loop for addAmr mutation
    types
- 2026-01-28:
  - fixed bug in amp depth were the gene coverage file
    mean read depths were off
- 2026-01-16:
  - added getAmr and addAmr (in getAmr)
- 2026-01-13:
  - changed the geneCoord structure to include the number
    of genes. This changes several functions.
- 2025-12-19:
  - fixed an error when reading a sam file (samEntry),
    were a read would be counted as an error if the last
    charater read in after the q-score entry was a tab or
    space. This was a rare error.
- 2025-12-04:
  - added closest lineage method for the getLin complex
    databases. This picks a complex lineage with the most
    matching variants from a group of complex lineages.
  - the getLin complex database format has chagned. The
    old format still works, however, the newer version
    will not work.
- 2025-11-28:
  - added the print all variants flag to getLin. This will
    print all variants marked as do not print. Overwriting
    will still happen for complex variants.
- 2025-11-25:
  - getLin, changed complex database required lineage
    symbol to `&`
  - getLin, add in complex database not this lineage
    symbol (`!`)
- 2025-11-24:
  - fixed minnor error casuing file error when reading
    large unmapped reads in sam files
- 2025-11-20:
  - added getLin for detecting lineages
- 2025-11-12:
  - added revSeqtoAA_codonFun to codon fun to allow
    conversions of reverse complement sequences to amino
    acid sequences
  - for demux primer trimming; fixed sequence start and
    end being printed out as index 0 (now is index 1)
  - added gene coordinate detection in demux
- 2025-11-10:
  - added in space support for sam file reading. It can
    now parse the sam file entries (1 (query id) to 12
    (extra entries)) if there are spaces
  - for the samEntry struct added an interal setup call
    for get_samEntry and lineTo_samEntry.
- 2025-11-09:
  - converted demux to an amplicon extraction program
- 2025-11-04:
  - fixed binSam chrash when no reference ids were not in
    header. binSam now reports that no references were
    present instead of crashing.
- 2025-11-03:
  - add binSam to bin reads by reference in a sam file
- 2025-10-15:
  - fixed minor bug in primFind output that did not print
    reference length for reverse paired primers
  - minor bug were ampDepth with `-p-gene-cover` would not
    print the last gene in the table
- 2025-10-09:
  - added in filter option to memwaterScan to remove
    nested alignments, also allowing for direction setting
    on query sequence
- 2025-10-08:
  - added the filter opton to seqStats to allow fastq file
    filtering
- 2025-10-06:
  - added the sequence length to the third column of
    primFind
- 2025-10-03:
  - added the `-p-gene-cover` to ampDepth to print out
    percent coverage by gene
- 2025-10-02:
  - added the memwaterScan to allow detection of multiple
    alignments.
    - currently a little to senesitive, needs a filter
      step
- 2025-09-23:
  - realized diCoords in fluDI was used beyond fluDI, so I
    moved diCoords from fluDI to bioTools. This invovled
    several minor tweaks.
- 2025-09-16:
  - fixed minor bug in samToAln were I removed a variable
    it need to compile
- 2025-08-05:
  - added multi reference support to rmHomo
- 2025-07-28:
  - added printing of read depths to ampDepth
    - no longer just summary
- 2025-07-21:
  - added leftAln for left alignment in homopolymers
  - mapRead now uses leftAln to do left indel alignment
    - left indel alignment is on by default
- 2025-07-04:
  - unGz; fixed XLEN (extra entry length) being wrong
    endin format (reason why unGz errored out on Illumina
    fastq.gz files)
   - unGz and mapRead now use binary ("rb" instead of "r")
     to read files. Needed on Windows
   - k2TaxaId bug were when -no-merge-root is used, but
     not -no-merge-tip in kraken2 reports causes one taxa
     to get multiple non-target read ids
     - this was due to the back track array being set to
       0 (first taxa) instead of -1
- 2025-06-30:
  - fixed negative tlen values be printed as very large
    numbers
- 2025-06-11:
  - k2TaxaId fixed inifinite loop issue
    with `-no-merge-root` and setup so so
    that `-no-merge-root -no-merge-tip` will print any
    taxa at or after `-end <taxa level>`
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

