1) find number of references
   - one reference; use it
   - > 1 reference, align read each reference separatly
2) check if reference or query is longer
   - if reference shorter;
     - reference becomes query
     - query becomes reference
   - else keep query as query
3) split reference into windows
   - each window is 2.5x the query length
     - idea is that with 2.5x, even in the worst case we
       have 1.5x sequence in a window
   - each window moves by one query length
4) count number shared kmers between query and reference
   in one window
   - both forward and reverse (finds direction)
5) if 70% of kmers are shared, merge kmers into chains
   - direction with best count used
   - each reference kmer is mapped to its matching query
     kmers
   - if mathced reference and query kmer are already
     part of a chain, move on (already solved)
   - matching kmers are exteneded until no matches, then
     kept if at least 20kmers long; otherwise discarded
6) merge chains
   a. count last reference chain chains length as number
      bases/score; assing longest chain to best score
   b. move back one reference kmer; stop if no more chains
   c. if no overlap with best scoring chain, add
      best score to current chains length, then
      repeat step b (set best score if new best)
   d. if overlap, scan scored chains for best
      score, then add length to score; repeat step b
7) reslove gaps in best chain
   - for query only gaps, count as deletions
   - for reference only gaps, count as insertions
   - for shared gap (ref/query) do needleman alignment
8) score alignment and find best subalignment
   - find score, if score is neagtive, record score
     (if best), restart score (next subalignment)
9) return best scoring subalignment (may be whole)

Using the maximize step in the chain merging is not an
  optimal solution, however, it does reduce the
  complexity.

~~~dita
+------------------------------------+<----------+
| compare reference and query length +<--------+ |
|                                    +<------+ | |
+--------+---------------------+-----+<----+ | | |
         |                     |           | | | |
 +-------+-------+  +----------+-------+   | | | |
 |  query longer |  | reference longer |   | | | |
 +-------+-------+  +---------+--------+   | | | |
         |                    |            | | | |
 +-------+-------+  +---------+--------+   | | | |
 | o scan query  |  | o scan reference |   | | | |
 | o match ref   |  | o match query    |   | | | |
 +-------+-------+  +---------+--------+   | | | |
         |                    |            | | | |
 +-------+--------------------+--------+   | | | |
 | find shared kmers in scan and match |   | | | |
 +--------+-------------------+--------+   | | | |
          |                   |            | | | |
 +--------+-------+  +--------+-------+    | | | |
 | Match;         |  | No match:      |    | | | |
 |  o merge kmers |  |   o next query +----+ | | |
 +--------+-------+  -----------------+      | | |
          |                                  | | |
 +--------+-----------------------+          | | |
 | o handel easy calls            |          | | |
 |   o single base gaps/snps      |          | | |
 |   o clear deletions/insertions |          | | |
 |     o no bases between kmers   |          | | |
 | o bridge kmers using gaps to   |          | | |
 |   fill in alignment points     |          | | |
 +---------------+----------------+          | | |
                 |                           | | |
 +---------------+---------------+           | | |
 | o count percent shared  kmers |           | | |
 |   kmers                       |           | | |
 +-----+------------------+------+           | | |
       |                  |                  | | |
 +-----+-----+    +-------+--------+         | | |
 | if enough |    | if not enough  +---------+ | |
 |           |    |   o next query |           | |
 +-----+-----+    +----------------+           | |
       |                                       | |
 +-----+---------------------+                 | |
 | finish aligning           |                 | |
 |   o needleman for short   |                 | |
 |   o hirschberge for large |                 | |
 +---------------------+-----+                 | |
       |               |                       | |
 +-----+------+  +-----+-----+                 | |
 | good score |  | low score +-----------------+ |
 +------+-----+  +-----------+                   |
        |                                        |
 +------+----------+                             |
 | print alignment +                             |
 +--------+--------+                             |
          |                                      |
 +--------+--------+                             |
 | get next query  +-----------------------------+
 +-----------------+
~~~

