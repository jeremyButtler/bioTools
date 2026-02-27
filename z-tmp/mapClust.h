/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' mapClust SOF: Start Of File
'   - clustering reads by using a read mapper to find
'     similar reads (denovo)
'   o header:
'     - forward declarations, defined variables, and
'       guards for people who include .h files in .h files
'   o .h st01: set_mapClust:
'     - has the settings for the mapping clustering step
'     - also have default settings in this block
'   o .h st02: index_mapClust
'     - index's reads in the fastq file and assigns
'       clusters
'     - also have the defiend symbols for this struct in
'       this block
'   o st03: read_mapClust
'     - holds a single read
'   o st04: clust_mapClust
'     - holds a single cluster
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*-------------------------------------------------------\
| Header:
|  - forward declarations, defined variables, and guards
|    for people who include .h files in .h files
\-------------------------------------------------------*/

#ifndef DENOVO_CLUSTERING_BY_READ_MAPPING_H
#define DENOVO_CLUSTERING_BY_READ_MAPPING_H

struct seqST;
struct samEntry;

/*-------------------------------------------------------\
| ST01: set_mapClust
|   - settings for the mapping clustering step
\-------------------------------------------------------*/
typedef struct
set_mapClust{
   signed int startTrimSI;
      /*number bases to ignore at the start of each read*/
   signed int endTrimSI;
      /*number bases to ignore at the end of each read*/
}set_mapClust;

/*-------------------------------------------------------\
| ST02: index_mapClust
|   - has the index of every read in the fastq files
\-------------------------------------------------------*/
#define def_noClust_mapClust -1 /*no cluster assigned*/

typedef struct
index_mapClust{
   unsinged long *indexAryUL; /*index of every read*/
   signed int *clusterArySI;  /*cluster of each read*/
   signed int *scoreArySI;    /*score for each read*/
   signed long lenSL;         /*number of reads in file*/
   signed long sizeSL;        /*array sizes*/
}index_mapClust;

/*-------------------------------------------------------\
| ST03: read_mapClust
|   - holds a single read
\-------------------------------------------------------*/
typedef struct
read_mapClust{
   struct seqST *readST;  /*has the read sequence*/

   /*this array is sorted, but does not have every kmer
   `  in it (to save space). It is used for a quick
   `  comparison check
   */
   signed int *kmerArySI;    /*kmers in the cluster*/
   signed int *kmerCntArySI; /*number times kmer repeats*/
   signed int kmerLenSI;     /*number kmers*/
}read_mapClust;

/*-------------------------------------------------------\
| ST04: clust_mapClust
|   - holds a single cluster
\-------------------------------------------------------*/
typedef struct
clust_mapClust{
   struct samEntry *conST; /*has the consensus sequence*/
   signed int idSI;        /*id for this cluster*/
   signed int readNumSI;   /*number of reads in cluster*/

   /*this array is sorted, but does not have every kmer
   `  in it (to save space). It is used for a quick
   `  comparison check
   */
   signed int *kmerArySI;    /*kmers in the cluster*/
   signed int *kmerCntArySI; /*number times kmer repeats*/
   signed int kmerLenSI;     /*number kmers*/

   struct clust_mapClust *next;
}clust_mapClust;

#endif
