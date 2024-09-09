# Use:

Holds small tools I have made for biology programs. The
  tools are either niche tools, re-inventing the wheel, or
  just plain odd. They all should compile on Linux, BSDs,
  Mac, and Windows (without cwygin).

These tools are often built to help me with my projects or
  are smaller parts of my projects. I have included the
  project they were designed for / first appeared in.

# License

Primary is unlicense (public domain), but if publice
  domain does not work or is not desired it defaults to
  MIT (alternate).

# List of tools

- re-inventing the wheel:
  - seqById (fqGetIds):
    - extract sequences by read id from sam files and
      fastq files
  - filtsam (freezeTB):
    - filter sam files by flag, length, median/mean
      q-scores, and coordinates
    - also supports read soft mask removal
  - alnwater (alnSeq):
    - waterman alignment with gap extension penalties
    - slower than striped watermans
  - alnMemwater (alnSeq):
    - waterman alignment with gap extension penalties
    - only returns score and alignment coordinates, but
      also uses less memory
  - primFind (fluDI/freezeTB):
    - find primers in sequence(s)
- niche
  - adjCoords (freezeTB):
    - adjust mapping coordinates from genes to entire
      genome
    - you need to provide a tsv file with gene names and
      coordinates
  - ampDepth (freezeTB):
    - finds mean read depth of amplicons and the genes
      they cover
    - you need to provide a coordinate file for this
  - cigToEqx (freezeTB):
    - convert regular cigars to eqx cigars
  - trimSam (find-co--infections):
    - trims soft masking off reads in sam file
    - filtsam can do this with `-trim` and has filtering
      options
  - maskPrim (freezeTB):
    - mask primer sites by coordinates (for sam file)
    - tsv file with coordinates is needed
  - edDist (fluDI/freezeTB):
    - working on, finds modified edit distances
      - modified means only large indels and snps with
        q-scores > 6
      - can do sam file to sam file or by ref
  - adjMap (fluDI/freezeTB):
    - adjust mapping coordinates to a new reference
      - reference and reads must be sam files
    - needs some debugging still, but close
  - samToAln:
    - convert sam file to human readable alignment
    - this is how to get a viewable alignment for alnwater
    - emboss like format, except the "|"'s are replaced
      with cigar symbols "SIDX=".

# genLib and genAln

My general libraries.

genLib is for general libraries that do not have a
  specific task.

genAln is for alignment tasks.
