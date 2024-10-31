/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' krakTaxaId SOF: Start Of File
'   - has functions and structs to get read ids by kraken
|     organisms
'   o fun09: getLevel_krakTaxaId
'     - get the level in tree of a kraken entry; col 4
'   o fun10: readReport_krakTaxaId
'     - gets list of organism codes
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#define def_maxLevels_krakTaxaId 64 /*deepest level*/
#define def_maxDepth_krakTaxaId 32 /*maximum tree depth*/

#define def_unclassifed_krakTaxaId 0
#define def_root_krakTaxaId 0         /*very broad level*/
#define def_domain_krakTaxaId 1 << 3  /*8*/
#define def_phylum_krakTaxaId 1 << 4  /*16*/

#define def_class_krakTaxaId 1 << 5   /*32*/
#define def_order_krakTaxaId 1 << 6   /*64*/
#define def_family_krakTaxaId 1 << 7  /*128*/
#define def_genus_krakTaxaId 1 << 8   /*256*/
#define def_species_krakTaxaId 1 << 9 /*512*/

/*-------------------------------------------------------\
| ST01: taxa_krakTaxaId
|   - has taxa names and ids to extract by
\-------------------------------------------------------*/
typedef struct taxa_krakTaxaId
{
   signed long *codeArySL;    /*taxa codes for oragnisms*/
   signed char *nameAryStr;/*string name with taxa names*/
   signed long *mergeArySL;    /*has index's of levels*/
   unsigned short *levelAryUS;/*position of taxa*/

   /*these are for quick look ups*/
   signed long *sortCodeArySL; /*sorted codeArySL*/
   signed long *indexArySL; /*index of codeArySL in sort*/

   unsigned long numTaxaUL;  /*number detected taxa*/
   unsigned long sizeTaxaUL; /*size of all arrays*/
}taxa_krakTaxaId;

/*-------------------------------------------------------\
| Fun01: blank_taxa_krakTaxaId
|   - blanks a taxa_krakTaxaId structure
| Input:
|   - taxaSTPtr:
|     o pointer to taxa_krakTaxaId struct to blank
| Output:
|   - Modifies:
|     o nothing; just here for future poofing
\-------------------------------------------------------*/
void
blank_taxa_krakTaxaId(
   struct *taxa_krakTaxaId taxaSTPtr
);

/*-------------------------------------------------------\
| Fun02: init_taxa_krakTaxaId
|   - initializes a taxa_krakTaxaId structure
| Input:
|   - taxaSTPtr:
|     o pointer to taxa_krakTaxaId to initiazlize
| Output:
|   - Modifies:
|     o sets everything in taxaSTPtr to 0
\-------------------------------------------------------*/
void
init_taxa_krakTaxaId(
   struct *taxa_krakTaxaId taxaSTPtr
);

/*-------------------------------------------------------\
| Fun03: freeStack_taxa_krakTaxaId
|   - frees variables in a taxa_krakTaxaId structure
| Input:
|   - taxaSTPtr:
|     o pointer to taxa_krakTaxaId with vars to free
| Output:
|   - Frees:
|     o all variables in taxaSTPtr (sets vars to 0)
\-------------------------------------------------------*/
void
freeStack_taxa_krakTaxaId(
   struct *taxa_krakTaxaId taxaSTPtr
);

/*-------------------------------------------------------\
| Fun04: freeHeap_taxa_krakTaxaId
|   - frees a taxa_krakTaxaId structure
| Input:
|   - taxaSTPtr:
|     o pointer to taxa_krakTaxaId to free
| Output:
|   - Frees:
|     o taxaSTPtr (you must set to null (0))
\-------------------------------------------------------*/
void
freeHeap_taxa_krakTaxaId(
   struct *taxa_krakTaxaId taxaSTPtr
);

/*-------------------------------------------------------\
| Fun05: setup_taxa_krakTaxaId
|   - allocates memory for a taxa_krakTaxaId struc
| Input:
|   - taxaSTPtr:
|     o pointer to taxa_krakTaxaId to setup memory
|   - numElmUL:
|     o number of elements to allocate memory for
| Output:
|   - Modifies:
|     o all arrays in taxaSTPtr to have memory
|   - Returns:
|     o 0 for no errors
|     o def_memErr_krakTaxaId for memory errors
\-------------------------------------------------------*/
signed char
setup_taxa_krakTaxaId(
   struct *taxa_krakTaxaId taxaSTPtr,
   unsigned long numElmUL
);

/*-------------------------------------------------------\
| Fun06: realloc_taxa_krakTaxaId
|   - reallocates memory for a taxa_krakTaxaId struc
| Input:
|   - taxaSTPtr:
|     o pointer to taxa_krakTaxaId to resize
|   - numElmUL:
|     o number of elements to resize to
| Output:
|   - Modifies:
|     o all arrays in taxaSTPtr to be realloced
|   - Returns:
|     o 0 for no errors
|     o def_memErr_krakTaxaId for memory errors
\-------------------------------------------------------*/
signed char
realloc_taxa_krakTaxaId(
   struct *taxa_krakTaxaId taxaSTPtr,
   unsigned long numElmUL
);

/*-------------------------------------------------------\
| Fun07: sortCodes_taxa_krakTaxaId
|   - sorts the sortCodeArySL array while keeping the
|     indexArySL in sync a taxa_krakTaxaId
| Input:
|   - taxaSTPtr:
|     o pointer to taxa_krakTaxaId to sort
| Output:
|   - Modifies:
|     o sortCodeArySL to be sorted lowest to greatest
|     o indexArySL to be in sync with sortCodeArySL
|       - idea: index is index to unsorted arrays
\-------------------------------------------------------*/
void
sortCodes_taxa_krakTaxaId(
   struct *taxa_krakTaxaId taxaSTPtr
);

/*-------------------------------------------------------\
| Fun08: findCode_taxa_krakTaxaId
|  - returns index of taxa code, searches sorted list
|    and returns unsorted list index
| Input:
|   - codeSL:
|     o taxa code to search for
|   - taxaSTPtr:
|     o pointer to taxa_krakTaxaId to search for code
| Output:
|  - Returns:
|    o index of taxa code
|    o -1 if taxa code not in taxaSTPtr
\-------------------------------------------------------*/
signed long
findCode_taxa_krakTaxaId(
   signed long codeSL,
   struct *taxa_krakTaxaId taxaSTPtr
);

/*-------------------------------------------------------\
| Fun09: getLevel_krakTaxaId
|   - get the level in heirchyy of a kraken entry (col 4)
| Input:
|   - taxaStr:
|     o string with taxa assignment to get number for
| Output:
|   - Returns:
|     - taxa numuber + sub-taxa level (ex: for D1; D + 1)
|       o unclassified def_unclassifed_krakTaxaId; no level
|       o root def_root_krakTaxaId + level
|       o domain def_domain_krakTaxaId + level
|       o phylum def_phylum_krakTaxaId + level
|       o class def_class_krakTaxaId + level
|       o order def_order_krakTaxaId + level
|       o family def_family_krakTaxaId + level
|       o genus def_genus_krakTaxaId + level
|       o species def_species_krakTaxaId + level
\-------------------------------------------------------*/
unsigned short
getLevel_krakTaxaId(
   signed char *taxaStr
);

/*-------------------------------------------------------\
| Fun10: readReport_krakTaxaId
|   - gets codes of organisims for read ids print otu
| Input:
|   - startLevUS:
|     o lowest level (nearest root) to start printing for
|     o use getLevel_krakTaxaId to get this
|   - endLevUS:
|     o highest level (nearest tip) to start printing for
|     o use getLevel_krakTaxaId to get this
|   - minDepthUL:
|     o min read depth to keep an id
|   - pStrictBl:
|     o 1: only keep ids mapping to its level
|     o 0: merge lower tree (root) levels with their
|          upper (tip) levels
|   - errSCPtr:
|     o pointer to signed char to have errors
|   - inFILE:
|     o FILE pointer to kraken2 tsv report with taxons
|       to extract
| Output:
|   - Modifies:
|     o errSCPtr:
|       - 0 for no errors
|       - def_memErr_krakId for memory errors
|       - def_fileErr_krakId for file errors
|       - def_noIds_krakId if no id's in file
|   - Returns:
|     o taxa_krakId with list of taxon ids and names
|       to extract
|       - negative numbers in codeArySL denote merged
|         tips (! pScrictBl) or for pScrictBl, taxa to
|         ignore
|     o 0 for no ids or error
\-------------------------------------------------------*/
struct taxa_krakTaxaId *
readReport_krakTaxaId(
   unsigned short startLevUS,   /*lowest level to print*/
   unsigned short endLevUS,     /*highest level to print*/
   unsigned long minDepthUL,    /*min depth to keep id*/
   signed char pStrictBl,       /*1: do not merge levels*/
   signed char *errSCPtr,
   FILE *inFILE                 /*kraken2 report*/
);
