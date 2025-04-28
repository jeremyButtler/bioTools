# Use: 

The logic here was that remapping would take less time
  then mapping. However, I found some cases were indel
  positioning (del in one, snp in other) would through
  off the result. In one case I had twice as many SNPs
  in the redasjusted alignment compared to just mapping
  with minimap2. I had no way to resolve or detect this
  issue so, this has been abandonded.
