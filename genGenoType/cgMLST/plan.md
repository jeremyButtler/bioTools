For cgMLST it is a comparision to find the best mapping
  allele for a loci. It then uses some fudge factor on
  the edit distace (SNPs + indels) to find if it is a new
  allele or matches the found allele.

+---------------------+ +----+ +----------------------+
| Input               | | or | | Input                |
|   o sam file with   | +----+ |   o sam file with    |
|     mapped reads    |        |     mapped consensus |
| Input               |        | Input                |
|   o loci mapped to  |        |   o loci mapped to   |
|     sam refernence  |        |     sam refernence   |
|     as sam file     |        |     as sam file      |
++-------+------------+        +-------------+--------+
 ^       |                                   |
 |       +----------------+------------------+
 |                        |
 |           +------------+------------+
 |           | find loci with mapping  +<-------------+
 |           | coordinates             |              |
 |           +------------+------------+              |
 |                        |                           |
 |          +-------------+-------------+             |
 |          | find edit distance to all |             |
 |          | alleles for loci          |             |
 |          |   o edDist                |             |
 |          +-------------+-------------+             |
 |                        |                           |
 |             +----------+----------+                |
 |             | find closest allele |                |
 |             +----------+----------+                |
 |                        |                           |
 |          +-------------+-------------+             |
 |          | check if distance between |             |
 |          | read and allele in fudge  |             |
 |          | range                     |             |
 |          +-------------+-------------+             |
 |                        |                           |
 |              ----------+---------+                 |
 |              |                   |                 |
 |    +---------+--------+ +------------------+       |
 |    | yes              | | no               |       |
 |    |   o assing read  | |   o assing read  |       |
 |    |     to allele    | |     as unique    |       |
 |    +---------+--------+ +---------+--------+       |
 |              |                    |                |
 |              +---------+----------+                |
 |                        |                           |
 |              +---------+---------------+           |
 |              |                         |           |
 | +------------+-------------+ +---------+---------+ |
 | | if no more loci          | | more loci in read | |
 +-+   o move to next read    | |   o move to next  +-+
   |     or consenus          | |     loci          |
   +--------------------------+ +-------------------+
