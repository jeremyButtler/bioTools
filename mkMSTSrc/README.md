# Use:

Builds a minimum spanning tree from user input. This
  version uses prims algorithm and a quick heap (not
  quite). Only the lowest cost edge for each node is kept
  in the heap.

# License:

Dual licensed under the Unlicense or MIT. Pick the one
  that works for you, with your company, or your country.

# Install:

To install mkMST on a Unix (Linux/Mac/Bsd):

```
git clone \
    https://github.com/jeremybuttler/bioTools \
    "${HOME}/Downloads/bioTools;
cd "${HOME}/Downloads/bioTools/mkMSTSrc";
make -f mkfile.unix;
sudo make -f mkfile.unix install;
```

# How to use mkMST:

You can get the help message for mkMST with `mkMST -h`.

The needed input for mkMST is a feature file with the
  lineages to build the tree from.

The first row of the feature file is a header and is
  ignored. The second row and onwards has the lineage for
  each sequence.

The first column of the feature file is the sequence name.
  The second column and onward are the lineages for each
  sequence. Each column is treated as a match or mismatch.
  So, the maximum difference per column is one.

This example has first three spacers from the spoligotype.
  It also includes two randomly made lineages types. The
  `*` and all cases of `na` (`NA`, `Na`, and `nA`) are
  entries that are not known (can not be compared).

```
sequence  lineage_1 lineage_2   space_1 spacer_2 spacer_3
NC000962  3         gene_1a     1       0        0
my_sample 2         gene_1b     1       0        1
some_seq  *         na          1       0        1
```

The distances in the example above are. The some\_seq
  entry is closer to both NC000962 and my\_sample because
  lineage\_1 and lineage\_2 are unknown (`*` or `na`).

| reference  | NC000962 | my_sample | some_seq |
|:-----------|:---------|:----------|:---------|
| NC000962   | na       | 3         | 1        |
| my\_sample | 3        | na        | 0        |
| some\_seq  | 1        | 0         | na       |

Table: the distances for the previous examples. some\_seq
  has two lineages (lineage\_1 and lineage\_2) that have
  unknown entries, which makes it look closer to both
  NC000962 and my\_sample.
