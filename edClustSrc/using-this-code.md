# Goal:

Not so great (terriable) guide on how to use edClust in
  your own program.

This is out of date

# Introduction:

edClust is a slow program designed to cluster reads by
  edit distance. Memory usage is minmal
  (~ 29 bytes * number reads) plus consensuses built.
  Also, file writes, but not reads are minimal (only
  consensuses, the log, and finshed clusters are printed).

# Structures:

When using edClust, there are six structures that you
  will use.

- set_edClust: settings for edClust plus some variables
  for keeping track of position
- set_tbCon: settings for consensus building (tbCon)
- samEntry: holds a single sam file line
- index_edClust: indexing and scoring of sam file and
  the clusters each read is assigned to
- con_edClust: holds consensus for each cluster

## set_edClust:

The set_edClust structure (st01 edClust.h) holds the
  settings for edClust and variables to keep track of the
  current cluster and the reference coordinates of the
  cluster.

Most the variables in set_edClust are settings, see the
  .h file for details. However, the startUI, endUI, and
  clustUI are present for clustering. The startUI and
  endUI variables keep track of the start and end of the
  reference for the current cluster being worked on. The
  clustSI variable tells which cluster is being worked on.

To initialize and set_edClust structure use
  init_set_edClust() (fun02 edClust.c/h). This will set
  all settings to default settings.

Variables inside set_edClust must be edit manually to
  change from defaults.

You can free an set_edClust structure with
  freeStack_edClust() (fun03 edClust.c/h) to free internal
  variables (none) or freeHeap_edClust()
  (fun04 edClust.c/h) to free a heap allocated set_edClust
  structure.

## set_tbCon:

The set_tbCon structure (st03 ../tbConSrc/tbCon.h) holds
  the settings for building a consensus. The print
  settings and mininum mapping quality are not used.

You can initialize the consensus settings to default using 
  init_tbCon() (fun10 ../tbConSrc/tbCon.c/h).

To modify a setting change the target variable in the
  set_tbCon structure.

You can free the internal variables (none) using
  freeStack_tbCon (fun11 tbCon.c/h).

There is no heap freeing function, so call
  freeStack_tbCon() and then free().

## samEntry:

The samEntry structure (st01 ../genLib/samEntry.c) holds
  the information from a single line from a sam file. You
  do not need to be to familiar with it for edClust.

- some key variables:
  - qryIdStr: holds read/query id/name
    - lenQryIdUC holds length
  - refIdStr: holds reference name
    - lenRefIdUC holds length
  - flagUS: flag in sam file
  - mapqUC: mapping quality
  - refStartUI: first reference base
  - refEndUI: last reference base
  - cigTypeStr: cigar symbols (I, X, =, M, D, S, H)
    - lenCigUI holds length
  - cigArySI: values for each cigar symbol (number)
    - lenCigUI holds length
  - seqStr: sequence for read
    - lenReadUI holds length
  - qStr: q-score entry for read
    - set to '*\0' if no q-score entry
  - extraStr: extra entries or were headers stored
    - lenExtraUI holds length

You will need to initialize it with init_samEntry()
  (fun02 samEntry.c/h). Then set up memory with
  setup_samEntry() (fun03 samEntry.c/h). The setup
  function will return a "def_memErr_samEntry" if there
  was a memory error.

You can read in sam files using get_samEntry()
  (fun12 ../genLib/samEntry.c/h; works with stdin) and you
  can print sam file entries using p_samEntry()
  (fun14 ../genLib/samEntry.c/h).

For get_samEntry you will need a samEntry structure to
  add the read to, a buffer to expand as needed, the
  current length of the buffer, and a FILE pointer to
  the sam file. Headers will be stored in the
  extraStr entry of the samEntry strucutre. You can
  check for headers
  with `if(samEntryPtr->extraStr[0] == '@')`.

For p_samEntry you will need a samEntry structure to
  print, a buffer to expand as needed, the
  current length of the buffer, a boolean (character),
  and a file to print to. The boolean (0 or anything else)
  controlls if a new line is printed at the end (0) or
  not (1). This is here so you can add extra entries to
  the end of the new sam file.

You can free the variables inside a samEntry structure
  with freeStack_samEntry()
  (fun04 ../genLib/samEntry.c/h).

You can free a heap allocated samEntry structure using
  freeHeap_samEntry() (fun05 ../genLib/samEntry.c/h).

## index_edClust:

The index_edClust structure (st03 edClust.h) stores the
  scores, the cluster assignments, the reference
  (as number), the reference starting coordinates, the
  reference ending coordinates, the position in the sam
  file, and the line length in the sam file for each read.

It also stores the total number of reads kept, the length
  of the sam file, and the length of the arrays.

- Variables:
  - scoreArySC: scores for each read
  - startAryUI: starting coordinates for each read
  - endAryUI: ending coordinates for each read
  - refAryUI: reference number for each read
  - clustArySI: cluster each read was assigned to
    - 0 means no cluster
    - def_discard_edClust means discarded
    - def_head_edClust means its a header line
  - indexAryUL: position of each read in file
    - for fseek
  - lenLineAryUI: line length each read takes in file
    - for fread
  - keptSL: number of kept reads
  - lenSamUL: number of lines in sam file
  - lenUL: size of arrays

The best way to make an index_edClust structure is to
  call getScore_edClust() (fun19 edClust.c/h). This will
  return a pointer to an index_edClust structure that is
  setup for your sam file. It uses realloc to assign
  memory on the flye, this allows you to get the scores
  and index's from stdin. It will return 0 for a memory
  error.

Input is the size to start the index_edClust structure
  at (0 is allowed), a set_edClust structure with read
  filtering settings, a pointer to a samEntry stucture,
  a buffer to read into, the length of the buffer, and
  a FILE pointer to the sam file to index/score.

I would recomened finding your file size with
  getNumLines_edClust (fun18 edClust.c/h) (non-stdin
  input). It is a bit rough, but will get you in the ball
  park.

You can free the index_edClust structure using
  freeHeap_index_edClust() (fun15 edClust.c/h).

## con_edClust:

The con_edClust structure (st02 edClust.c/h) holds the
  consensus built for a single cluster in edClust.

- Varaibles:
  - samSTPtr: is a samEntry structure with the consensus
  - clustSI: is this consensuses cluster
  - numReadsUL: number of reads assigned to cluster
  - startUI: is the first reference base in this cluster
  - endUI: is the last reference base in this cluster
  - nextST: is the next cluster

You can make a con_edClust structure using
  mk_con_edClust() (fun10 edClust.c/h). This takes in the
  samEntry structure to add, the cluster number, and the
  number fo reads assigned to the cluster. A 0 is returned
  for a memory error.

You can swap values between two con_edClust structures
  using swap_con_edClust() (fun11 edClust.c/h).

You can compare two clusters using cmpCons_edClust()
  (fun27 edClust.c/h). This returns 0 if clusters were
  considered different and 1 if clusters were the same. In
  cases were clusters are the values are merged (including
  the clusters assigned in index_edClust) into the frist
  cluster. The second cluster can then be freeded by you.

The input for cmpCons_edClust() is frist con_edClust
  structure to compare, the second con_edClust structure
  to compare, the index_edClust structure with the
  cluster assignments, and a set_edClust structure with
  comparision settings.

To free a con_edClust structure use freeHeap_conEdClust.

To free a list of con_edClust structures use
  freeHeapList_conEdClust.

# Functions:

The quickest way to cluster reads is to call
  cluster_edClust (fun28 edClust.c/h), which returns a
  con_edClust list with consensuses. It also modifies the
  input index_edClust pointer to point to a index_edClust
  structure with scores and clusters. See edClust.h for
  further documentation on input.

You can print out the consensuses for each cluster using
  plist_edCon_edClust() (fun29 edClust.c/h).

You can print out the reads for all clusters using
  pbins_edClust() (fun26 edClust.c/h).

You can print out the reads for a specific cluster using
  getReads_edClust() (fun25 edClust.c/h).

To manually build cluster_edClust you would need to frist
  build you index_edClust structure using getScore_edClust
  (fun19 edClust.c/h). The call getBestRead_edClust()
  (fun21 edClust.c/h) to find a read to cluster with. Then
  findNumMap_edClust() (fun23 edClust.c/h) to find the
  number of reads mapping to the best read. Finally, you
  can build the consensus using getCon_edClust()
  (fun24 edClust.c/h), which will return a samEntry
  structure.

You will then need to put the consensuses samEntry
  structure into a con_edClust with mk_con_edClust()
  (fun10 edClust.c/h). You can then compare the new
  consensus to the other cosensus with cmp_con_edClust()
  (fun27 edClust.c/h). If similar (returns 1) then free
  the newly made con_edClust structure, else add it to
  your list of con_edClust structures.
