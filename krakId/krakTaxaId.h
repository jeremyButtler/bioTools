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
| ST01: taxaList_krakTaxaId
|   - has taxa names and ids to extract by
\-------------------------------------------------------*/
typedef struct taxaList_krakTaxaId
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
}taxaList_krakTaxaId;

/*-------------------------------------------------------\
| Fun01: blank_taxaList_krakTaxaId
|   - blanks a taxaList_krakTaxaId structure
| Input:
|   - taxaListSTPtr:
|     o pointer to taxaList_krakTaxaId struct to blank
| Output:
|   - Modifies:
|     o nothing; just here for future poofing
\-------------------------------------------------------*/
void
blank_taxaList_krakTaxaId(
   struct *taxaList_krakTaxaId taxaListSTPtr
);

/*-------------------------------------------------------\
| Fun02: init_taxaList_krakTaxaId
|   - initializes a taxaList_krakTaxaId structure
| Input:
|   - taxaListSTPtr:
|     o pointer to taxaList_krakTaxaId to initiazlize
| Output:
|   - Modifies:
|     o sets everything in taxaListSTPtr to 0
\-------------------------------------------------------*/
void
init_taxaList_krakTaxaId(
   struct *taxaList_krakTaxaId taxaListSTPtr
);

/*-------------------------------------------------------\
| Fun03: freeStack_taxaList_krakTaxaId
|   - frees variables in a taxaList_krakTaxaId structure
| Input:
|   - taxaListSTPtr:
|     o pointer to taxaList_krakTaxaId with vars to free
| Output:
|   - Frees:
|     o all variables in taxaListSTPtr (sets vars to 0)
\-------------------------------------------------------*/
void
freeStack_taxaList_krakTaxaId(
   struct *taxaList_krakTaxaId taxaListSTPtr
);

/*-------------------------------------------------------\
| Fun04: freeHeap_taxaList_krakTaxaId
|   - frees a taxaList_krakTaxaId structure
| Input:
|   - taxaListSTPtr:
|     o pointer to taxaList_krakTaxaId to free
| Output:
|   - Frees:
|     o taxaListSTPtr (you must set to null (0))
\-------------------------------------------------------*/
void
freeHeap_taxaList_krakTaxaId(
   struct *taxaList_krakTaxaId taxaListSTPtr
);

/*-------------------------------------------------------\
| Fun05: setup_taxaList_krakTaxaId
|   - allocates memory for a taxaList_krakTaxaId struc
| Input:
|   - taxaListSTPtr:
|     o pointer to taxaList_krakTaxaId to setup memory
|   - numElmUL:
|     o number of elements to allocate memory for
| Output:
|   - Modifies:
|     o all arrays in taxaListSTPtr to have memory
|   - Returns:
|     o 0 for no errors
|     o def_memErr_krakTaxaId for memory errors
\-------------------------------------------------------*/
signed char
setup_taxaList_krakTaxaId(
   struct *taxaList_krakTaxaId taxaListSTPtr,
   unsigned long numElmUL
);

/*-------------------------------------------------------\
| Fun06: realloc_taxaList_krakTaxaId
|   - reallocates memory for a taxaList_krakTaxaId struc
| Input:
|   - taxaListSTPtr:
|     o pointer to taxaList_krakTaxaId to resize
|   - numElmUL:
|     o number of elements to resize to
| Output:
|   - Modifies:
|     o all arrays in taxaListSTPtr to be realloced
|   - Returns:
|     o 0 for no errors
|     o def_memErr_krakTaxaId for memory errors
\-------------------------------------------------------*/
signed char
realloc_taxaList_krakTaxaId(
   struct *taxaList_krakTaxaId taxaListSTPtr,
   unsigned long numElmUL
);

/*-------------------------------------------------------\
| Fun07: sortCodes_taxaList_krakTaxaId
|   - sorts the sortCodeArySL array while keeping the
|     indexArySL in sync a taxaList_krakTaxaId
| Input:
|   - taxaListSTPtr:
|     o pointer to taxaList_krakTaxaId to sort
| Output:
|   - Modifies:
|     o sortCodeArySL to be sorted lowest to greatest
|     o indexArySL to be in sync with sortCodeArySL
|       - idea: index is index to unsorted arrays
\-------------------------------------------------------*/
void
sortCodes_taxaList_krakTaxaId(
   struct *taxaList_krakTaxaId taxaListSTPtr
);

/*-------------------------------------------------------\
| Fun08: findCode_taxaList_krakTaxaId
|  - returns index of taxa code, searches sorted list
|    and returns unsorted list index
| Input:
|   - codeSL:
|     o taxa code to search for
|   - taxaListSTPtr:
|     o pointer to taxaList_krakTaxaId to search for code
| Output:
|  - Returns:
|    o index of taxa code
|    o -1 if taxa code not in taxaListSTPtr
\-------------------------------------------------------*/
signed long
findCode_taxaList_krakTaxaId(
   signed long codeSL,
   struct *taxaList_krakTaxaId taxaListSTPtr
);
