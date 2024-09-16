/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' clustSam SOF: Start Of File
'   - clusters reads by similarity
'   o header:
'     - included libraries
'   o .h st01: profile_clustSam
'     - holds profile (used for clustering) for a
'       reference read
'   o fun01: blank_profile_samClust
'     - blanks an profile_samClust structure
'   o fun02: init_profile_samClust
'     - initializes a profile_samClust structure
'   o fun03: setupReadHitAry_profile_samClust
'     - allocates memory for the readHitArySI array in
'       a profile_samClust struct
'   o fun04: freeStack_profile_samClust
'     - frees all variables in a profile_samClust struct
'   o fun05: freeHeap_profile_samClust
'     - frees a profile_samClust struct on heap
'   o .c fun06: swap_profile_samClust
'     - swaps two index's in a the arrays in a
'       profile_clustSam struct
'   o fun07: sortDepth_profile_samClust
'     - sorts arrays in profile_samClust struct by depth
'   o fun08: sortPos_profile_samClust
'     - sorts arrays in profile_samClust struct by
'       position
'   o fun09: resort_profile_clustSam
'     - resorts profile_clustSam to original order
'   o .c fun10: allocRead_profile_clustSam
'     - allocates memory (makes sure have enough memory)
'       for a signle reads profile
'   o fun11: build_profile_clustSam
'     - builds a profile for a profile_clustSam struct
'   o fun12: addReadTo_profile_clustSam
'     - adds a read to a profile_clustSam struct
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*-------------------------------------------------------\
| Header:
|   - forward declartions, defined variables, and guards
\-------------------------------------------------------*/

typedef struct samEntry samEntry;
typedef struct seqST seqST;
typedef struct alnSet alnSet;

#define def_memErr_clustSam -1
#define def_fileErr_clustSam -2
#define def_noMap_clustSam 2

/*flags for skipping reads in profile*/
#define def_charNeg_clustSam ( (signed char) ( ((unsigned char) 1) << (((sizeof(signed char) << 3)) - 1) ) )
   /*negative flag for characters*/

#define def_discarded_clustSam (def_charNeg_clustSam | (def_charNeg_clustSam >> 1))
   /*read has been discarded from all profile regions*/

#define def_regionDiscard_clustSam (def_charNeg_clustSam | (def_charNeg_clustSam >> 2))
   /*already assigned to profile, but supports mutiple
   `  regions
   */

#define def_profileDiscard_clustSam (def_charNeg_clustSam | (def_charNeg_clustSam >> 2))
   /*does not match profile*/

#define def_blankProfile_clustSam 1
#define def_blankRegion_clustSam 2
#define def_blankDiscard_clustSam 4

#define def_blankNt_clustSam ((unsigned char) -1)
#define def_del_clustSam ((unsigned char) -2)

/*-------------------------------------------------------\
| ST01: profile_clustSam
|   - holds profile (used for clustering) for a
|     reference read
\-------------------------------------------------------*/
typedef struct profile_clustSam
{
   unsigned int *posAryUI; /*positon of profile base*/
   unsigned int *lenAryUI; /*length of event*/
   unsigned char *baseAryUC; /*bases for profile*/
   signed char *typeArySC; /*error type matching*/
   unsigned int *supAryUI; /*number reads supporting*/
   unsigned int *mapAryUI; /*number reads mapping to*/
   unsigned int *indexAryUI; /*array of original index's*/
      /*allows me to resort to original order*/

   unsigned int lenProfileUI; /*size of profile arrays*/

   unsigned int profStartUI;  /*start of profile read*/
   unsigned int profEndUI;    /*end of profile read*/

   signed int *readHitArySI; 
      /*
      `   read status (discarded/last matching profile)
      */

   unsigned long lenHitUL; /*length of readHitArySI*/

   /*history for profie depth and variants*/
   unsigned int *varAryUI; /*index of target variants*/
   unsigned int *histAryUI;/*when variant first appeared*/
   unsigned int *cntAryUI; /*number variants in profile*/
   unsigned int *depthAryUI;/*read depth*/

   unsigned int histIndexUI; /*position at in history*/

   schar refIdStr[128]; /*holds reference id*/
}clustSam;

/*-------------------------------------------------------\
| Fun01: blank_profile_samClust
|   - blanks a profile_samClust structure
| Input:
|   - profileSTPtr:
|     o pointer to a profile_samClust structure to blank
|   - blankRegFlagSC:
|     o blank discard status for readHitArySI (you can
|       combine flags with (flag1 | flag2 | ... | flagn)
|       - def_blankProfile_clustSam blanks all profile
|         discarded reads (no longer discarded)
|       - def_blankRegion_clustSam blanks all region
|         discarded reads (no longer discarded)
|       - def_blankDiscard_clustSam blanks all discarded
|         reads (no longer discarded)
| Output:
|   - Modifies:
|     o all variables in profileSTPtr to be 0 or default
\-------------------------------------------------------*/
void
blank_profile_samClust(
   struct profile_samClust *profileSTPtr,
   signed char blankRegFlagSC
);

/*-------------------------------------------------------\
| Fun02: init_profile_samClust
|   - initializes a profile_samClust structure
| Input:
|   - profileSTPtr:
|     o pointer to profile_samClust struct to initialize
| Output:
|   - Modifies:
|     o all arrays to be null (0) and array size to be 0
\-------------------------------------------------------*/
void
init_profile_samClust(
   struct profile_samClust *profileSTPtr
);

/*-------------------------------------------------------\
| Fun03: setupReadHitAry_profile_samClust
|   - allocates memory for the readHitArySI array in
|     a profile_samClust struct
| Input:
|   - profSTPtr:
|     o pointer to profile_samClust struct to setup
|       the readHitArySI array
|   - numReadsUL:
|     o number of reads in the sam file
| Output:
|   - Modifies:
|     o readHitArySI to have memory
|     o lenHitUL to be numReadsUL
|   - Returns:
|     o 0 for no errors
|     o def_memErr_clustSam for memory errors
\-------------------------------------------------------*/
signed char
setupReadHitAry_profile_samClust(
   stuct profile_samClust *profSTPtr,
   unsigned loong numReadsUL
);

/*-------------------------------------------------------\
| Fun04: freeStack_profile_samClust
|   - frees all variables in a profile_samClust struct
| Input:
|   - profileSTPtr:
|     o pointer to profile_samClust struct with internal
|       variables to free
| Output:
|   - Frees:
|     o all arrays and sets to 0
|   - Modifies:
|     o lenProfileUI to be 0
\-------------------------------------------------------*/
void
freeStack_profile_samClust(
   struct profile_samClust *profileSTPtr
);

/*-------------------------------------------------------\
| Fun05: freeHeap_profile_samClust
|   - frees a profile_samClust struct on heap
| Input:
|   - profileSTPtr:
|     o pointer to profile_samClust struct to free
| Output:
|   - Frees:
|     o profileSTPtr (you must set to null)
\-------------------------------------------------------*/
void
freeHeap_profile_samClust(
   struct profile_samClust *profileSTPtr
);

/*-------------------------------------------------------\
| Fun07: sortDepth_profile_samClust
|   - sorts arrays in profile_samClust struct by depth
| Input:
|   - profileSTPtr:
|     o pointer to profile_samClust struct to sort
| Output:
|   - Modifies:
|     o arrays in profileSTPtr to be sorted by depth
\-------------------------------------------------------*/
void
sortDepth_profile_samClust(
   struct profile_samClust *profileSTPtr
);

/*-------------------------------------------------------\
| Fun08: sortPos_profile_samClust
|   - sorts arrays in profile_samClust struct by position
| Input:
|   - profileSTPtr:
|     o pointer to profile_samClust struct to sort
| Output:
|   - Modifies:
|     o arrays in profileSTPtr to be sorted by position
\-------------------------------------------------------*/
void
sortPos_profile_samClust(
   struct profile_samClust *profileSTPtr
);

/*-------------------------------------------------------\
| Fun09: resort_profile_clustSam
|   - resorts profile_clustSam to original order; position
| Input:
|   o profSTPtr:
|     - pointer to a profiler_clustSam struct to resort
| Output:
|   - Modifies:
|     o arrays in profSTPtr to be resorted
\-------------------------------------------------------*/
void
resort_profile_clustSam(
   struct profile_clustSam *profSTPtr
);

/*-------------------------------------------------------\
| Fun11: build_profile_clustSam
|   - builds a profile for a profile_clustSam struct
| Input:
|   - profileSTPtr:
|     o pointer to profile_samClust struct to hold profile
|   - samSTPtr:
|     o pointer to samEntry struct with read to profile
|   - minQUC:
|     o minimum q-score to keep snp in profile
|   - minIndelLenUI:
|     o minimum indel length to keep indel in profile
|   - newRegBl:
|     o 1: mapping to new region (remove region discards)
|     o 0: keep region discard flags
| Output:
|   - Modifies:
|     o profileSTPtr to have varaint profile for samSTPtr
|   - Returns:
|     o 0 for no errors
|     o def_memErr_clustSam for memory errors
\-------------------------------------------------------*/
signed char
build_profile_clustSam(
   struct profile_samClust *profileSTPtr
   struct samEntry *samSTPtr,
   unsigned int minIndelLenUI,
   unsigned char minQUC,
   signed char newRegBl
);

/*-------------------------------------------------------\
| Fun12: addReadTo_profile_clustSam
|   - adds a read to a profile_clustSam struct
| Input:
|   - profSTPtr:
|     o pointer to profile_samClust struct to hold profile
|   - samSTPtr:
|     o pointer to samEntry struct with read to add
|   - readOnUL:
|     o read currently on in profile
|   - minIndelLenUI:
|     o minimum indel length to keep indel in profile
|   - minQUC:
|     o minimum q-score to keep snp in profile
|   - minPercCovF:
|     o minimum percent of read covering profile
|   - alnSetSTPtr:
|     o has alignment settings for insertion regions
| Output:
|   - Modifies:
|     o profSTPtr to have read depth and mapped reads
|       updated
|     o readHitArySI[readOnUL] to be set to
|       def_profileDiscard_clustSam if read failed checks
|   - Returns:
|     o 0 for no errors
|     o def_noMap_clustSam if read not in profile
|     o def_discarded_clustSam if read was marked as
|       discarded
|     o def_memErr_clustSam for memory errors
\-------------------------------------------------------*/
signed char
addReadTo_profile_clustSam(
   struct profile_samClust *profSTPtr
   struct samEntry *samSTPtr,
   unsigned long readOnUL,
   unsigned int minIndelLenUI,
   unsigned char minQUC,
   float minPercCovF,
   struct alnSet *alnSetSTPtr
);

/*-------------------------------------------------------\
| Fun13: bust_profile_samClust
|   - finds variants in profile to extract reads with
| Input:
|   - profSTPtr:
|     o pointer to profile_clustSam struct to bust
|       (get minimum profile for)
|   - minDepthUI:
|     o minimum depth before going bust
| Output:
|   - Modifies:
|     o profSTPtr:
|       o sorts profile arrays in profSTPtr by depth
|       o depthVarAryUI to have number variants in profile
|       o histIdnexUC to be on next history entry
\-------------------------------------------------------*/
void
bust_profile_samClust(
   struct profile_clustSam *profSTPtr,
   unsigned int minDepthUI
);
