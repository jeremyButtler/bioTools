# Goal:

Describe how to make or edit a lineage database for
  stopASFV.

The lineage database has two tsv files, the lineage
  variants file and the complex lineages file. The getLin
  program only needs the lineage variants file to work.
  The lineage variants file has the variants seen in each
  lineage. It can also assign lineages to variants
  consisting of one variant.

The complex lineages file allows getLin to assign lineages
  that require multiple variants. It is not needed, but
  is used to extend the results.

# general formatting rules

Both lineage databases are tsv, so spaces and tabs should
  never be used (except to separate columns). Also, you
  should never use a `:` in group ids. The `:` is used
  to separate the group id from the variant id.

# lineage variants file

The first line of lineage variants file is the header and
  is for humans, and so, is always ignored. Each line
  after has a single variant.

- one variant:
  1. Is the gene or best gene the variant is on (used when
     printing variant type)
  2. Is the group the variant is assigned to
     - all variants in a group are compared
  3. Has the id of the variant
     - this is used in the complex lineage file
  4. Has the first base of the variant in the reference
  5. Has the last base of the variant in the reference
  6. Tells if using nucleotides or amino acids
     - nt for nucleotides
     - aa for nucleotides
  7. Has the variant type
     - `snp` for a snp (can be more then one base)
     - `del` for a deletion
     - `ins` for an insertion (or use trs)
     - `len` for a length lineage type
       - means lineage is found by the length between the
         start base and end base
       - for this case replace the sequence with a number
     - `trs` for a tandum repeated sequence; or any repeat
       - this will count the number of repeats and assign
         the lineage based on the number of repeats
     - `trsFast` same as `trs`, but is speed up by using
       5mers to find likely matches and then a Smith
       Waterman alignment to confirm
       - requries at least 12 bases, at least 15 to 20
         reccommended
       - limited to nucleotide sequences (no amino acid)
     - `count` for counting then number of times a pattern
       is repeated
       - the entry will look like a `trs` lineage entry
     - `countFast` same as `count`, but is speed up by
       using 5mers to find likely matches and then a Smith
       Waterman alignment to confirm
       - requries at least 12 bases, at least 15 to 20
         reccommended
       - limited to nucleotide sequences (no amino acid)
  8. Is the direction (foward or reverse) of the pattern
     (9)
     - F or + is for forward
     - R or - is for reverse
  9. Is the pattern to use for the lineage
     - This pattern should always be in the reference
       sequences direction
     - `0` if this lineage has no pattern
       - Currently del lineages only
       - In reality getLin ignores this entry for deletion
         so you are safe putting other values here
     - A number (length lineages only) with the expected
       length of the lineage
     - `=` if this lineage is the default lineage to
       assign (not used with trs lineages)
       - default means, assign if no other lineage within
         the group supports the lineage, but the default
         length was covered
       - this program will set the start and end of this
         lineage to cover all lineages in the entire group
       - you can not use `|` to have multiple lineages
         as the defualt (it is a special assignment)
     - One or more nucleotides (A, T, G, C) to compare
       - This includes the sequence for a trs
       - For SNP lineages, the nucleotides must always be
         in the order on the reference sequence (do not
         reverse complement)
     - One or more amino acids to compare
       - This includes the sequence for a trs
     - A pipe separated list when multiple combininations
       are possible
       - Ex: list of three triplet repeats `ATG|AAA|CGG`
       - In trs sequence, this can be used to say any of
         the input sequences count as one length
  10. Lineage to assign if match found (or in case of
      reference, no other lineage in group matches)
      - Lineage name (output is `gene-<lineage_name>`)
      - For `trs` and `count` lineages, the number of
        copies lineage one has
        - TRS lineages are auto found based on this number
  11. Print this lineage if detected. An example were you
      might not print the lineage is when a lineage
      requires multiple variants to call (this would be
      a complex lineage).
      - T, True, or + print lineage
      - F, False, or - to not print lineage
  12. Amount to fudge in difference for deletion and TRS
      lineages
      - Use NA or 0 for no fudge
      - For insertions this is the score (matchs * 5) of
        the alignment
      - For alignments I use an EDNA matrix:
        - Match is 5
        - SNP is -4
        - Gap opening is -10
        - Gap extension is -1
  13. Minimum score for TRS (tandum repeat sequences),
      count lineages, or insertion alignments
      - Use NA or 0 for none
  14. Maximum gaps allowed between tandum repeats, before
      starting new TRS count
      - Use NA or 0 for none
  15. Not read, but I use for any notes I want to add

## Example of SNP lineages.

The B602L-1 lineage is a match to the reference genome (no
  other B602L lineage). The second is an alternative B602L
  lineage. The third lineage does not formally assign a
  lineage, but is detected. This one is used later in the
  complex lineages.

```
gene   group      id        start   end     molecule type  trsBlocks pattern        lineage plineage fudge min_score trs_gap  notes
B602L  B602L-SNP B602L-1    102545  102943  nt       SNP   NA        =              1       True     NA    NA        NA       default_lineage
B602L  B602L-SNP B602L-5    102708  102708  nt       SNP   NA        A              5       True     NA    NA        NA       NA
K145R  K145R-SNP K145R-3    66010   66010   nt       SNP   NA        T              NA      False    NA    NA        NA       not_printed
```

## Example of a deletion lineage.

In this case the coordinates are used to detect the
  lineage size. The fudge allows for an earlier and later
  coordinate start/end.

```
gene   group      id        start   end     molecule type  trsBlocks pattern        lineage plineage fudge min_score trs_gap
B602L  B602L-SNP  B602L-2   102730  102765  nt       DEL   NA        NA             2       True     10    NA        NA
```

## Example of a tandum repeat and count lineage.

The lineage id is the number of repeats need to get
  lineage 1 (1 for the first, two for the second). The
  start and end coordinates are were to search for the
  tandum repeats. I also included a count lineage at the
  bottom.

```
gene   group      id        start   end     molecule type    trsBlocks pattern        lineage plineage fudge min_score trs_gap note
O174   O174-TRS   O174-TRS  129175  129804  nt       trsFast No        TCACTACAATTGT  1       True     6     37        2       allows_two_snps_and_2_bases_between_trs_alignments
I73R   I73R-IGR   IGR       173273  173607  nt       TRS     No        GAATATATAG     2       True     4     40        0       allows_one_gap_or_two_snps
I73R   I73R-Count Count     173273  173607  nt       COUNT   No        GAATATATAG     2       True     0     40        0       allows_one_gap_or_two_snps
```

## Example TRS with multiple patterns

The sequence is built up from three blocks (patterns). The
  start and end coordinates are were to look for these
  blocks. Length is used to find the final lineage.

```
gene   group      id        start   end     molecule type  trsBlocks pattern        lineage plineage fudge min_score trs_gap
B602L  B602L-CVR  B602L-CVR 102545  102943  aa       TRS   Yes       CADT|NVDT|CASM 1       True      1     15        0
```

# Complex lineage file

The complex lineage file allows the merging off multiple
  variants to form a separate lineage.

- Columns
  1. ID to assign to the lineage
  2. Goup id the lineage belongs to
     - This can be a group from the single variants file
       or a new group
  3. Gene lineage is from
     - Use NA if not relevant (not used)
     - If present, is used in the final lineage output
  4. Lineage name to use
  5. The amount of lineages I can fudge by
     - how many lineages (complex or simple) can be missed
       and still call or at least consider the lineage
     - Can be 0 or `NA`
  6. Tells if overwrites the single variant lineage
     - Yes/True, means the single variant (simple) lineages
       used to get this lineage are overwritten (ignored)
     - Default: means the default lineage (from simple
       lineage sheet) is overwritten if this complex
       lineage is found
     - All: means both the default lineage and the simple
       lineages used to get this complex lineage are
       overwritten
     - No, means all single variant lineages are printed
  7. Marks if printing this lineage or not
     - This is useful if you are using the complex lineage
       as a part of other complex lineages, but do not
       want it printed.
     - True: for print
     - False: do not print
  8. How a lineage is found
     - `match` means the lineage is found by any match
       - In this mode multiple lineages can exists so
         long as they have enough detected lineages
     - `closest` means the closet lineage in the lineage
        group is choosen for the sequence
       - This still requires the lineage still be within
         the `fudge` range to even be considered
     - `na` use the default mode (`match`)
     - If this column is not present, then `match` is
       assumed (the not present if for backwards
       compatability)
  9. Columns till a `*` column are ingored
     - this allows future expension of the system and
       backwards compatbility
     - for notes, you should use a `#` symbol to tell
       getLin that everything till the first `*` is a
       note or should not be read
  10. Marks the start of the variant column, this is always
      a `*`
  11. And onwards are the variants to assign
     - Non-TRS lineages it is `<variant_name_(id)>`
     - TRS lineages it is `<variant_name_(id)>:lineage`
       - were lineage is based on the repeat length
     - Count lineages it is `<variant_name_(id)>:lineage`
       - were lineage is based on the number of duplicates
     - A `&` at the start means this variant is required
       - ex `&<variant_name_(id)>`
     - A `!` at the start means this variant can not be
       present
       - ex `!<variant_name_(id)>`
     - `NA` for no lineage
       - here to allow you to fill blank cells with
         something
  12. Using an `#` symbol means everything else should
      be ingored
      - Use this to add notes at the end

The variant list is always at the end after the '*'
  column. This allows a flexibal system were more entries
  can added between the lineages (column 6) and the start
  of the variants column (always a single `*`). By
  default, the first version of getLin ingores everything
  after column 6 until the variants start column (`*`).
 

The bottom lineage is a TRS example which is a combined
  lineage, but the user expects to see both the final and
  separate lineages. The top two lineages are lineages
  that are on the same gene and have multiple SNPs. There
  is no way to report these separately.

```
id        group     gene   type	fudge lingeage overwrites print_lineages	variants_start var1       var2
B602L-UKR B602L-SNP B602L  match	0     UKR      Yes        True	*              B602L-UKR1 B602L-UKR2
K145R     group     K145R  match	0     3        Yes        True	*              K145R-2    K145R-3
TRS_combo group     NA     match	0     mixed    No         True	*              IGR:2      O174-TRS:3
```

An exmaple adding in a notes column for human use.

```
id        group     gene   fudge lingeage overwrites print_lineages type		notes         variants_start var1       var2	notes
B602L-UKR B602L-SNP B602L  0     UKR      Yes        True	match	  #gene_lineage  *              B602L-UKR1 B602L-UKR2	# or notes can go here
K145R     group     K145R  0     3        Yes        True	match	  #gene_lineage  *              !K145R-2    K145R-3 	# or notes can go here
TRS_combo group     NA     0     mixed    No         True	match	  #final_lineage *              IGR:2      O174-TRS:3	# or notes can go here
```
