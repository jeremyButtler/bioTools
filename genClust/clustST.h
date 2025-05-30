/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' clustST SOF: Start Of File
'   - holds structures used in clustST and clustST
'     (for clustering)
'   o TOF: Table Of Functions
'     - tof00: header entry
'     - tof01: structures in this file
'     - tof02: set_clustST support fuctions
'     - tof03: index_clustST general functions
'     - tof04: con_clust general functions
'     - tof05: build/use index_clustST; consensus to
'     - tof05: setting up/using index_clustST struct
'     - tof06: consensus and cluster functions
'   - TOF00: header entry
'     o header:
'       - forward declarations, defined variables, guards
'   - TOF01: structures
'     o .h st01: set_clustST
'       - has settings for clustering
'     o .h st02: index_clustST
'       - holds scores, clusters, and index's for sam file
'       - ~ 29 bytes per line
'     o .h st03: con_clustST
'       - holds a single consensus/profile for a cluster
'     o .h st04: var_clustST
'       - holds depth of variants in reference (clustST)
'     o .h st05: hist_clustST
'       - holds profile history for reads (clustST)
'   - TOF02: set_clustST support fuctions
'     o fun01: blank_set_clustST
'       - sets start and end to 0
'     o fun02: init_set_clustST
'       - sets settings in set_clustST struct to defaults
'     o fun03: freeStack_set_clustST
'       - no internal variables to free (here for support)
'     o fun04: freeHeap_set_clustST
'       - frees a heap allocated set_clustST struct
'   - TOF03: index_clustST general functions
'     o fun05: blank_index_clustST
'       - blanks an index_clustST struct
'     o fun06: init_index_clustST
'       - initializes an index_clustST struct
'     o fun07: freeStack_index_clustST
'       - frees and initializes variables in index_clustST
'     o fun09: freeHeap_index_clustST
'       - frees index_clustST struct
'     o fun10: setup_index_clustST
'       - setsup memory for index_clustST struct
'     o fun11: realloc_index_clustST
'       - reallocates memory for an index_clustST struct
'   - TOF04: con_clust general functions
'     o fun12: blank_con_clustST
'       - blank an clustST con_clustST struct
'     o fun13: init_con_clustST
'       - initialzie an clustST con_clustST struct
'     o fun14: freeStack_con_clustST
'       - frees variables inside an con_clustST struct
'     o fun15: freeHeap_con_clustST
'       - frees a con_clustST struct
'     o fun16: freeHeapList_con_clustST
'       - frees list of heap allocated con_clustST structs
'     o fun17: mk_con_clustST
'       - makes a con_clustST struct on the heap
'     o fun18: swap_con_clustST
'       - swaps to con_clustST structs (except nextST ptr)
'   - TOF05: build/use index_clustST struct (consensus to)
'     o fun20: mk_index_clustST
'       - get read scores for a sam file
'     o fun21: getRead_clustST
'       - gets a read by index from a sam file
'     o fun22: getCon_clustST
'       - builds a consensus for a cluster
'   - TOF06: consensus and cluster functions
'     o fun22: getCon_clustST
'       - builds a consensus for a cluster
'     o fun23: cmpCons_clustST
'       - compares two consensus clusters to see if same
'     o fun24: plist_con_clustST
'      - print clusters consensuses in a con_clustST list
'     o fun25: getClust_clustST
'       - extracts reads for a signle cluster
'     o fun26: pbins_clustST
'       - extracts reads for each cluster to a sam file
'   o license:
'     - licensing for this code (public domain / mit)
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*-------------------------------------------------------\
| Header:
|   - forward declarations, defined variables and guards
\-------------------------------------------------------*/

#ifndef CLUSTER_STRUCTURES_H
#define CLUSTER_STRUCTURES_H

struct samEntry;
struct set_tbCon;
struct res_edDist;


/*error return types*/
#define def_memErr_clustST 1
#define def_fileErr_clustST 2
#define def_noCon_clustST 4


/*cluster values*/
#define def_discard_clustST -1
#define def_header_clustST -2


/*default settings*/
/*settings: unique to clustST*/
#define def_minVar_clustST 3 /*min number of variants*/
#define def_minProb_clustST 0.001f /*min probility*/

/*settings: unique to clustST*/
#define def_conRebuild_clustST 1    /*rebuild con once*/
#define def_depthProfile_clustST 0  /*do depth profiling*/

#define def_readErrRate_clustST 0.046f
#define def_conErrRate_clustST 0.023f
#define def_maxVarErrRatio_clustST 100

#define def_lenWeight_clustST 2.0f

/*settings: for initial read filtering*/
#define def_minLen_clustST 50
#define def_minMapq_clustST 15
#define def_minMedQ_clustST 7
#define def_minAvgQ_clustST 7

/*settings: variant filtering (or ovlerlap)*/
#define def_minDepth_clustST 20
#define def_indelLen_clustST 10
#define def_minSnpQ_clustST 7

#define def_minPercDepth_clustST 0.001f
#define def_percOverlap_clustST 0.75f

#define def_window_clustST 500
#define def_windowError_clustST 200

/*settings: consensus filtering*/
#define def_maxConSim_clustST 0.99f
#define def_maxNPerc_clustST 0.05f /*10% N limit*/

#define def_repInterval_clustST 0
   /*do not report reads left*/

/*-------------------------------------------------------\
| ST01: set_clustST
|   - has settings for clustering
\-------------------------------------------------------*/
typedef struct set_clustST
{
   signed long repIntervalSL; /*how often to log status*/

   /*unique to profClsut--------------------------------*/
   float minProbF;     /*min probability to keep profile*/
   unsigned int minVarUI; /*min number varaints*/

   /*unique to clustST----------------------------------*/
   signed char depthProfBl; /*1; do a depth profile*/
   float readErrRateF;     /*read map read error rate*/
   float conErrRateF;      /*con map read error rate*/

   float lenWeightF;
       /*how much length should influence the score*/

   unsigned int varToErrUI;
      /*maximum number (discard if higher) allowed for
      ` 100 * (number variants) / (overlap * errRateF)
      `   - variant to error ratio
      */

   unsigned char conRebuildUC;
      /*number of times to rebuild consensus*/


   /*in both clustST and clustST----------------------*/

   /*initial read filtering*/
   unsigned int minLenUI; /*min read length to keep read*/
   unsigned char minMapqUC;/*min mapq to keep read*/
   unsigned int minMedQUI; /*min median q to keep read*/
   unsigned int minAvgQUI; /*min mean q to keep read*/
   unsigned int minDepthUI;/*min cluster depth to keep*/

   float minPercDepthF;   /*min % read depth for cluster*/
   float percOverlapF;
       /*minimum percentage of coverage the reference
       `  must have on the query. So, longer reads then
       `  reference are discourged
       */

   /*variant filtering*/
   unsigned int indelLenUI;/*min indel length*/
   unsigned char minSnpQUC;/*min snp q-score*/
   unsigned int winSizeUI; /*window size for window scan*/
   unsigned int winErrUI;  /*error percent for window*/

   /*consensus filtering*/
   float maxNPercF;        /*max % N's in consensus*/
   float maxConSimF;
      /*maximum similarity between consensuses before
      `   merging clusters
      */

   /*variables for holding start/end of consensus*/
   unsigned int startUI; /*work on removing one day*/
   unsigned int endUI;   /*work on removing one day*/
   signed int clustSI;   /*cluster on*/
}set_clustST;

/*-------------------------------------------------------\
| ST02: index_clustST
|   - holds scores, clusters, and index's for sam file
|   - ~ 29 bytes per line
\-------------------------------------------------------*/
typedef struct index_clustST
{
   signed char *scoreArySC;  /*scores for each read*/
   unsigned int *startAryUI; /*read starting positions*/
   unsigned int *endAryUI;   /*read ending positions*/
   unsigned int *refAryUI;   /*read reference numbers*/
   signed int  *clustArySI;  /*cluster reads assigned to*/

   unsigned long *indexAryUL;/*index of reads in file*/
   unsigned int *lenLineAryUI;/*length of line*/

   signed char *refIdAryStr; /*reference ids in sam file*/
   unsigned int *refNumAryUI;/*# for reference id*/
   unsigned int numRefUI;    /*number references in file*/
   unsigned int lenRefUI;    /*max # of refs (length)*/

   signed long keptSL;       /*number of kept reads*/
   unsigned long lenSamUL;   /*number lines in sam file*/
   unsigned long lenUL;      /*length of arrays*/
}index_clustST;

/*-------------------------------------------------------\
| ST03: con_clustST
|   - holds a single consensus/profile for a cluster
\-------------------------------------------------------*/
typedef struct con_clustST
{
   struct samEntry *samSTPtr; /*consensus sequence*/
   signed int clustSI;        /*cluster number*/
   unsigned int refNumUI;     /*reference assigned*/

   unsigned long numReadsUL;  /*number reads in clust*/
   unsigned long maxReadsUL;
      /*number reads covering same coordinates as
      ` cluster (do not have to be part of cluster)
      */
   unsigned int startUI;      /*first ref base in clust*/
   unsigned int endUI;        /*last ref base in clust*/

   signed char *typeArySC;    /*variant type*/
   unsigned int *posAryUI;    /*position of variants*/
   unsigned int lenProfUI;    /*length of arrays*/

   unsigned int numVarUI;     /*# variants in profile*/

   struct con_clustST *nextST; /*next con in list*/
}con_clustST;

/*-------------------------------------------------------\
| ST04: var_clustST
|   - holds depth of variants in reference (clustST)
\-------------------------------------------------------*/
typedef struct var_clustST
{
   unsigned int *aAryUI;   /*number of a's per position*/
   unsigned int *cAryUI;   /*number of c's per position*/
   unsigned int *gAryUI;   /*number of g's per position*/
   unsigned int *tAryUI;   /*number of t's per position*/
   unsigned int *delAryUI; /*large dels per position*/
   unsigned int *insAryUI; /*large inss per position*/

   unsigned int lenUI;    /*size of arrays*/
   unsigned int lenRefUI; /*length of reference*/
   unsigned int refNumUI; /*ref number assigned*/
}var_clustST;

/*-------------------------------------------------------\
| ST05: hist_clustST
|   - holds profile history for reads (clustST)
\-------------------------------------------------------*/
typedef struct hist_clustST
{
   unsigned int startUI;    /*first base in reads*/
   unsigned int endUI;      /*last base in reads*/

   unsigned int numVarUI;   /*number variants in profile*/
   unsigned long depthUL;   /*depth in overlap*/
   unsigned int lenUI;
       /*length of arrays (total variants in profile)*/

   unsigned int *indexAryUI; /*index of variants*/
   unsigned char *typeAryUC; /*key for mutation type*/
   unsigned int *profAryUI;  /*index in profile*/ 
   unsigned int *histAryUI;  /*# index's per bust step*/
   unsigned long *depthAryUL;/*depth for history steps*/
}hist_clustST;

/*-------------------------------------------------------\
| Fun01: blank_set_clustST
|   - sets start and end to 0
| Input:
|   - clustSetSTPtr;
|     o pointer to set_clustST struct to blank
| Output:
|   - Modifies:
|     o startUI and endUI in clustSetSTPtr to be 0
\-------------------------------------------------------*/
void
blank_set_clustST(
   struct set_clustST *clustSetSTPtr
);

/*-------------------------------------------------------\
| Fun02: init_set_clustST
|   - sets settings in set_clustST struct to defaults
| Input:
|   - clustSetSTPtr;
|     o pointer to set_clustST struct to initialize
| Output:
|   - Modifies:
|     o all variables in clustSetSTPtr to be defaults
\-------------------------------------------------------*/
void
init_set_clustST(
   struct set_clustST *clustSetSTPtr
);

/*-------------------------------------------------------\
| Fun03: freeStack_set_clustST
|   - no internal variables to free (here for support)
| Input:
|   - clustSetSTPtr;
|     o pointer to set_clustST struct with variables to
|       free
| Output:
|   - Modifies:
|     o initializes all variables (nothing to free)
\-------------------------------------------------------*/
void
freeStack_set_clustST(
   struct set_clustST *clustSetSTPtr
);

/*-------------------------------------------------------\
| Fun04: freeHeap_set_clustST
|   - frees a heap allocated set_clustST struct
| Input:
|   - clustSetSTPtr;
|     o pointer to set_clustST struct to free
| Output:
|   - Frees:
|     o clustSetSTPtr (you must set to 0)
\-------------------------------------------------------*/
void
freeHeap_set_clustST(
   struct set_clustST *clustSetSTPtr
);

/*-------------------------------------------------------\
| Fun05: blank_index_clustST
|   - blanks an index_clustST struct
| Input:
|   - indexSTPtr:
|     o pointer to index_clustST struct to blank
| Output:
|   - Modifies:
|     o lenSamUL, keptUL, and numRefUI in struct be 0
\-------------------------------------------------------*/
void
blank_index_clustST(
   struct index_clustST *indexSTPtr
);

/*-------------------------------------------------------\
| Fun06: init_index_clustST
|   - initializes an index_clustST struct
| Input:
|   - indexSTPtr:
|     o pointer to index_clustST struct to initializes
| Output:
|   - Modifies:
|     o all arrays to be null and lengths to be 0
\-------------------------------------------------------*/
void
init_index_clustST(
   struct index_clustST *indexSTPtr
);

/*-------------------------------------------------------\
| Fun07: freeStack_index_clustST
|   - frees and initializes variables in index_clustST
| Input:
|   - indexSTPtr:
|     o pointer to index_clustST struct with variables to
|       free
| Output:
|   - Frees:
|     o all arrays, then initializes
\-------------------------------------------------------*/
void
freeStack_index_clustST(
   struct index_clustST *indexSTPtr
);

/*-------------------------------------------------------\
| Fun09: freeHeap_index_clustST
|   - frees index_clustST struct
| Input:
|   - indexSTPtr:
|     o pointer to index_clustST struct to free
| Output:
|   - Frees:
|     o indexSTPtr (you must set to 0)
\-------------------------------------------------------*/
void
freeHeap_index_clustST(
   struct index_clustST *indexSTPtr
);

/*-------------------------------------------------------\
| Fun10: setup_index_clustST
|   - setsup memory for index_clustST struct
| Input:
|   - indexSTPtr:
|     o pointer to index_clustST struct to free
|   - sizeUL:
|     o new size of arrays
| Output:
|   - Modifies:
|     o arrays in index_clustST to have memory allocated
|   - Returns:
|     o 0 for no errors
|     o def_memErr_clustST for memory errors
\-------------------------------------------------------*/
signed char
setup_index_clustST(
   struct index_clustST *indexSTPtr,
   unsigned long sizeUL
);

/*-------------------------------------------------------\
| Fun11: realloc_index_clustST
|   - reallocates memory for an index_clustST struct
| Input:
|   - indexSTPtr:
|     o pointer to index_clustST struct to assign more
|       memory to
|   - sizeUL:
|     o new size of arrays; use indexSTPtr->lenUL to not
|       resize arrays (except refAryStr)
|   - numRefUI:
|     o new number of references; use indexSTPtr->lenRefUI
|       to not resize reference array
| Output:
|   - Modifies:
|     o arrays in index_clustST to be size of sizeUL
|   - Returns:
|     o 0 for no errors
|     o def_memErr_clustST for memory errors
\-------------------------------------------------------*/
signed char
realloc_index_clustST(
   struct index_clustST *indexSTPtr,
   unsigned long sizeUL,
   unsigned int numRefUI
);

/*-------------------------------------------------------\
| Fun12: blank_con_clustST
|   - blank an clustST con_clustST struct
| Input:
|   - conSTPtr:
|     o pointer to con_clustST struct to blank
| Output:
|   - Modifies:
|     o conSTPtr to be blank 
\-------------------------------------------------------*/
void
blank_con_clustST(
   struct con_clustST *conSTPtr
);

/*-------------------------------------------------------\
| Fun13: init_con_clustST
|   - initialzie an clustST con_clustST struct
| Input:
|   - conSTPtr:
|     o pointer to con_clustST struct to initialize
| Output:
|   - Modifies:
|     o conSTPtr to intialize
\-------------------------------------------------------*/
void
init_con_clustST(
   struct con_clustST *conSTPtr
);

/*-------------------------------------------------------\
| Fun14: freeStack_con_clustST
|   - frees variables inside an con_clustST struct
| Input:
|   - conSTPtr:
|     o pointer to con_clustST struct to free varaibles
| Output:
|   - Frees:
|     o samSTPtr in conSTPtr (nextST not freeded)
|   - Modifies:
|     o blanks conSTPtr
\-------------------------------------------------------*/
void
freeStack_con_clustST(
   struct con_clustST *conSTPtr
);

/*-------------------------------------------------------\
| Fun15: freeHeap_con_clustST
|   - frees a con_clustST struct
| Input:
|   - conSTPtr:
|     o pointer to con_clustST struct to free
| Output:
|   - Frees:
|     o conSTPtr
|   - Returns:
|     o next con_clustST struct in list
\-------------------------------------------------------*/
con_clustST *
freeHeap_con_clustST(
   struct con_clustST *conSTPtr
);

/*-------------------------------------------------------\
| Fun16: freeHeapList_con_clustST
|   - frees a list of heap allocated con_clustST structs
| Input:
|   - conSTPtr:
|     o pointer to con_clustST struct list to free
| Output:
|   - Frees:
|     o conSTPtr and all other structs in the list
\-------------------------------------------------------*/
void
freeHeapList_con_clustST(
   struct con_clustST *conSTPtr
);

/*-------------------------------------------------------\
| Fun17: mk_con_clustST
|   - makes a con_clustST struct on the heap
| Input:
|   - samSTPtr:
|     o pointer to sam entry struct to store (0 for none)
|     o only a shallow copy is done (pointer saved only)
|   - clustSI:
|     o cluster number to assign
|   - numReadsUL:
|     o number of reads in cluster
| Output:
|   - Returns:
|     o pointer con_clustST struct with input
|     o 0 for memory error
\-------------------------------------------------------*/
con_clustST *
mk_con_clustST(
   struct samEntry *samSTPtr, /*consensus to add*/
   signed int clustSI,        /*cluster number*/
   unsigned long numReadsUL   /*number reads in cluster*/
);

/*-------------------------------------------------------\
| Fun18: swap_con_clustST
|   - swaps to con_clustST structs (except nextST ptr)
| Input:
|   - firstSTPtr:
|     o first struct to swap
|   - secSTPtr:
|     o second struct to swap
| Output:
|   - Modifies:
|     o firstSTPtr to have secSTPtr variables
|     o secSTPtr to have frstSTPtr variables
|   - Note: does not swap nextST pointer
\-------------------------------------------------------*/
void
swap_con_clustST(
   struct con_clustST *firstSTPtr,
   struct con_clustST *secSTPtr
);

/*-------------------------------------------------------\
| Fun20: mk_index_clustST
|   - get read scores for a sam file
| Input:
|   - sizeUL:
|     o size to make arrays at start
|     o any value > 0 will do since arrays are resized
|       when needed, however, for best memory usage, input
|       number of lines in sam file
|   - clustSetSTPtr:
|     o settings for filtering reads
|   - samSTPtr:
|     o pointer to sam struct to use in read hunt
|   - buffStrPtr:
|     o pointer to c-string to use reading sam file
|   - lenBuffULPtr:
|     o pointer to unsigned long with size of buffStrPtr
|   - samFILE:
|     o sam file to convert to stats
| Output:
|   - Modifies:
|     o samSTPtr to hold last entry
|     o buffStrPtr to be larger if needed
|     o lenBuffULPtr to be new buffStrPtr size (if resized)
|     o samFILE to point to start
|   - Returns:
|     o 0 for memory error
|     o pointer to index_clustST struct with scores and
|       index's (index_clustST->keptSL is 0 if no reads)
|       - in clustArySI:
|         i. open reads set to 0
|         ii. headers set to def_header_clustST
|         iii. discarded reads set to def_discard_clustST
\-------------------------------------------------------*/
struct index_clustST *
mk_index_clustST(
   unsigned long sizeUL,
   struct set_clustST *clustSetSTPtr,
   struct samEntry *samSTPtr,
   signed char **buffStrPtr,
   signed long *lenBuffSL,
   void *samFILE
);

/*-------------------------------------------------------\
| Fun21: getRead_clustST
|   - gets a read by index from a sam file
| Input:
|   - samSTPtr:
|     o pointer to samEntry struct to hold read
|   - lineUL:
|     o line number of read in sam file
|   - offsetUL:
|     o number of bytes from current position to read
|   - indexSTPtr:
|     o pointer to index_clustST struct with line lengths
|   - buffStr:
|     o c-string to read in line with
|     o should be length of target reads line + 2
|   - samFILE:
|     o FILE pointer to sam file to get read from
| Output:
|   - Modifies:
|     o samSTPtr to have new read
|     o buffStr to have reads line
|     o samFILE to be on read after current read
|   - Returns:
|     o 0 for no errors
|     o def_memErr_clustST for memory errors
|     o def_fileErr_clustST for file errors
\-------------------------------------------------------*/
signed char
getRead_clustST(
   struct samEntry *samSTPtr,       /*will have read*/
   unsigned long lineUL,            /*line of read*/
   unsigned long offsetUL,          /*offset to read*/
   struct index_clustST *indexSTPtr,/*index's of reads*/
   signed char *buffStr,            /*for getting line*/
   void *samFILE                    /*file with read*/
);

/*-------------------------------------------------------\
| Fun22: getCon_clustST
|   - builds a consensus for a cluster
| Input:
|   - indexSTPtr:
|     o pointer to index_clustST struct with cluster array
|       to use
|     o clusters assigned with fun03 (0 is no cluster)
|   - clustSI:
|     o cluster number to build consensus for
|   - conSetSTPtr:
|     o pointer to set_tbCon struct with setting for
|       consensus building
|   - clustSetSTPtr:
|     o has settings for filtering and stores the first
|       and last possible base of the consensesus
|   - samSTPtr:
|     o pointer to samEntry struct for reading sam file
|   - buffStr:
|     o c-string to use reading sam file
|   - samFILE:
|     o sam file with reads to add to consensus
|   - errSCPtr:
|     o pointer to signed char to hold error type
| Output:
|   - Modifies:
|     o samSTPtr to be last read in file
|     o samFILE to be at start
|     o buffStrPtr to have last sam file entry
|     o errSCPtr:
|       - 0 for no errors
|       - def_memErr_clustST for any other error
|       - def_noCon_clustST if consensus had to many N's
|         or could not be built
|   - Returns:
|     o 0 if could not build consenensus or had error
|     o samEntry strucure array with consensus fragments
\-------------------------------------------------------*/
struct samEntry *
getCon_clustST(
   struct index_clustST *indexSTPtr,
   signed int clustSI,
   struct set_clustST *clustSetSTPtr,
   struct set_tbCon *conSetSTPtr,
   struct samEntry *samSTPtr,
   signed char *buffStr,
   void *samFILE,
   signed char *errSCPtr
);

/*-------------------------------------------------------\
| Fun23: cmpCons_clustST
|   - compares two consensus clusters to see if same
| Input:
|   - firstConSTPtr:
|     o pointer to con_clustST struct with first consensus
|       to compare
|   - secConSTPtr:
|     o pointer to con_clustST struct with second
|       consensus to compare
|   - noBoundCheckBl:
|     o 1: skip bounds check (one read overlaps other)
|     o 0: do a bounds check
|   - indexSTPtr:
|     o pointer to index_clustST with clusters
|   - clustSetSTPtr:
|     o pointer to set_clustST struct with settings
|   - edDistResSTPtr:
|     o pointer to edDistResSTPtr struct to hold results
|       from edDist
| Output:
|   - Modifies:
|     o clustArySI in indexSTPtr to have clusters merged
|       if consensus are to similar (lowest cluster
|       assigned)
|     o merges second cluster into first cluster if
|       clusters were to similar
|       - you must free the second cluster (always has
|         discarded cluster)
|   - Returns:
|     o 0 if clusters are different
|     o 1 if merged clusters
\-------------------------------------------------------*/
signed char
cmpCons_clustST(
   struct con_clustST *firstConSTPtr,
   struct con_clustST *secConSTPtr,
   signed char noBoundCheckBl, /*1: no bounds check*/
   struct index_clustST *indexSTPtr,
   struct set_clustST *clustSetSTPtr,
   struct res_edDist *edDistResSTPtr
);

/*-------------------------------------------------------\
| Fun24: plist_con_clustST
|   - print the clusters consensuses in a con_clustST list
| Input:
|   - conSTPtr:
|     o pointer to con_clustST struct list with
|       consensuses to print
|   - headerStr:
|     o c-string with header to print (null = no header)
|   - pgHeadStr:
|     o c-string with program header to print (null = no
|       header)
|   - outFILE:
|     o file to print consensuses to
| Output:
|   - Modifies:
|     o buffStrPtr to be resized if needed
|     o lenBuffULPtr to have current buffer size
|   - Prints:
|     o headers and consensus to outFILE as a sam file
|   - Returns:
|     o 0 for no errors
|     o def_memErr_clustST for memeory errors
\-------------------------------------------------------*/
signed char
plist_con_clustST(
   struct con_clustST *conSTPtr, /*consensuses to print*/
   signed char *headerStr,       /*sam file header*/
   signed char *pgHeadStr,       /*program header*/
   void *outFILE                 /*file to print to*/
);

/*-------------------------------------------------------\
| Fun25: getClust_clustST
|   - extracts reads for a signle cluster
| Input:
|   - indexSTPtr:
|     o pointer to index_clustST struct with clusters
|   - clustSI:
|     o cluster number to extract reads for
|   - headStr:
|     o program header to print
|   - samSTPtr:
|     o pointer to samEntry struct for reading sam file
|   - buffStrPtr:
|     o pointer to c-string to use reading sam file
|   - lenBuffULPtr:
|     o pointer to unsigned long with size of buffStrPtr
|   - samFILE:
|     o sam file to extract reads from
|   - outFILE:
|     o file to save reads to
| Output:
|   - Modifies:
|     o samSTPtr to be last read in file
|     o samFILE to be at start
|     o buffStrPtr to have last sam file entry
|     o lenBuffULPtr to have buffStrPtr size (if resized)
|   - Returns:
|     o 0 for no errors
|     o def_memErr for memory errors
\-------------------------------------------------------*/
signed char
getClust_clustST(
   struct index_clustST *indexSTPtr,
   signed int clustSI,
   signed char *headStr,
   struct samEntry *samSTPtr,
   signed char **buffStrPtr,
   unsigned long *lenBuffULPtr,
   void *samFILE,
   void *outFILE
);

/*-------------------------------------------------------\
| Fun26: pbins_clustST
|   - extracts reads for each cluster to a sam file
| Input:
|   - prefixStr:
|     o c-string with prefix for output file name
|   - clustSI:
|     o last cluster to extract reads for, should be last
|       cluster in targArySI, but does not need to be
|   - indexSTPtr:
|     o pointer to index_clustST struct with clusters
|   - pgHeadStr:
|     o program header to print (null = no print)
|   - samSTPtr:
|     o pointer to samEntry struct for reading sam file
|   - buffStrPtr:
|     o pointer to c-string to use reading sam file
|   - lenBuffULPtr:
|     o pointer to unsigned long with size of buffStrPtr
|   - samFILE:
|     o sam file to extract reads from
| Output:
|   - Modifies:
|     o samSTPtr to be last read in file
|     o samFILE to be at start
|     o buffStrPtr to have last sam file entry
|     o lenBuffULPtr to have buffStrPtr size (if resized)
|   - Returns:
|     o 0 for no errors
|     o def_memErr_clustST for memory errors
|     o def_fileErr_clustST for file errors
\-------------------------------------------------------*/
signed char
pbins_clustST(
   signed char *prefixStr,
   signed int clustSI,
   struct index_clustST *indexSTPtr,
   signed char *pgHeadStr,
   struct samEntry *samSTPtr,
   signed char **buffStrPtr,
   unsigned long *lenBuffULPtr,
   void *samFILE
);

#endif

/*=======================================================\
: License:
: 
: This code is under the unlicense (public domain).
:   However, for cases were the public domain is not
:   suitable, such as countries that do not respect the
:   public domain or were working with the public domain
:   is inconvient / not possible, this code is under the
:   MIT license.
: 
: Public domain:
: 
: This is free and unencumbered software released into the
:   public domain.
: 
: Anyone is free to copy, modify, publish, use, compile,
:   sell, or distribute this software, either in source
:   code form or as a compiled binary, for any purpose,
:   commercial or non-commercial, and by any means.
: 
: In jurisdictions that recognize copyright laws, the
:   author or authors of this software dedicate any and
:   all copyright interest in the software to the public
:   domain. We make this dedication for the benefit of the
:   public at large and to the detriment of our heirs and
:   successors. We intend this dedication to be an overt
:   act of relinquishment in perpetuity of all present and
:   future rights to this software under copyright law.
: 
: THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF
:   ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT
:   LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
:   FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO
:   EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM,
:   DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
:   CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR
:   IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
:   DEALINGS IN THE SOFTWARE.
: 
: For more information, please refer to
:   <https://unlicense.org>
: 
: MIT License:
: 
: Copyright (c) 2024 jeremyButtler
: 
: Permission is hereby granted, free of charge, to any
:   person obtaining a copy of this software and
:   associated documentation files (the "Software"), to
:   deal in the Software without restriction, including
:   without limitation the rights to use, copy, modify,
:   merge, publish, distribute, sublicense, and/or sell
:   copies of the Software, and to permit persons to whom
:   the Software is furnished to do so, subject to the
:   following conditions:
: 
: The above copyright notice and this permission notice
:   shall be included in all copies or substantial
:   portions of the Software.
: 
: THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF
:   ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT
:   LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
:   FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO
:   EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
:   FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN
:   AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
:   FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE
:   USE OR OTHER DEALINGS IN THE SOFTWARE.
\=======================================================*/
