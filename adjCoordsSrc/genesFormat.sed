#!/bin/env sed -f
# converts gene features file (fasta file with gene names)
# from NCBI to more clearer format for me. I can quickly
# extract the coordinate list for ampDepth using
# `sed '/>/{s/>//; p;}'` after running this sed script

s/>[^|]*./>/; s/^>\(.*\)_gene_\([0-9]*\)/>\2 \1/;
# flips gene number and reference and puts a space
# between
# from: >reference_gene_number .*
# to  : >number reference .*

s/^>\([^ ]*\) \([^ ]*\).*\[gene=\([A-Za-z0-9]*\)\]/>\3-\1 \2/;
# gets gene name from [gene=name] entry
# from: >number reference_gene_number .* [gene=name]
# to  : >number reference name .*

s/^\(>[^ ]*\) \([^ ]*\).*\[location=\([^ ]*\)\].*/\1 \2 \3/;
# gets loction information and removes other tags
# from: >number reference name .* [location=start..end] .*
# to  : >number reference name start..end
# from: >number reference name .* [location=complement(start..end)] .*
# to  : >number reference name complement(start..end)

s/complement(/R/;
# converts complement( to R (reverse complement)
# from: >number reference name complement(start end)
# to  : >number reference name Rstart end)
# leaving R next to start so can distigiush between
# foward and reverse alter

s/)//;
# removes ) at complement end
# from: >number reference name Rstart end)
# to  : >number reference name Rstart end

# next commands remove the <number and number> entries
s/R<\([0-9]\)/R\1/;
# from: >number reference name R<start end
# to  : >number reference name Rstart end

s/[ \t]<\([0-9]\)/ \1/;
# from: >number reference name <start end
# to  : >number reference name start end

s/\.\.<\([0-9]\)/..\1/;
# from: >number reference name start..<end
# to  : >number reference name start..end
# from: >number reference name Rstart..<end
# to  : >number reference name Rstart..end

s/R>\([0-9]\)/R\1/g;
# from: >number reference name Rstart >end
# to  : >number reference name Rstart end

s/[ \t]>\([0-9]\)/ \1/;
# from: >number reference name start >end
# to  : >number reference name Rstart end

s/\.\.>\([0-9]\)/..\1/;
# from: >number reference name start..>end
# to  : >number reference name start..end
# from: >number reference name Rstart..>end
# to  : >number reference name Rstart..end

s/[ \t]\([0-9]*\)\.\.\([0-9]*\)/ F \1 \2/;
# add in forward direction symbol
# from: >number reference name start..end
# to  : >number reference name F start end

s/R\([0-9]*\)\.\.\([0-9]*\)/ R \1 \2/;
# separate reverse directio symbol
# from: >number reference name Rstart..end
# to  : >number reference name R start end

s/[ \t][ \t]*/ /g;
# make sure double spaces converted to single spaces

# End result
# Forward: >number reference name F start end
# Reverse: >number reference name R start end
