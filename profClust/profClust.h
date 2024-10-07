/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' profClust SOF: Start Of File
'   - clusters reads using variant profiles from reference
'   o header:
'     - included libraries
'   o fun27: addRead_var_profClust
'     - adds the variants in a read to var_profClust
'   o fun28: addReadRef_var_profClust
'     - adds the variants in a read to var_profClust
'       struct using the input reference
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*-------------------------------------------------------\
| Header:
|   - forward declartions, defined variables, and guards
\-------------------------------------------------------*/

#ifndef PROFILE_CLUSTER_H
#define PROFILE_CLUSTER_H

typedef struct samEntry samEntry;
typedef struct set_clustST set_clustST;     /*st01*/
typedef struct index_clustST index_clustST; /*st02*/
typedef struct con_clustST con_clustST;     /*st03*/
typedef struct var_clustST var_clustST;     /*st04*/
typedef struct hist_clustST hist_clustST;   /*st05*/

#define def_memErr_profClust 1
#define def_fileErr_profClust 2
#define def_noMap_profClust 4

/*-------------------------------------------------------\
| Fun27: addRead_var_profClust
|   - adds the variants in a read to var_profClust struct
| Input:
|   - samSTPtr:
|     o pointer to samEntry struct with read to add
|   - varSTPtr:
|     o pointer to var_profClust struct to add read
|       variants to
|   - setSTPtr:
|     o pointer to a set_profClust struct with settings
| Output:
|   - Mofifies:
|     o varSTPtr to have variants added to arrays
\-------------------------------------------------------*/
void
addRead_var_profClust(
   struct samEntry *samSTPtr,
   struct var_profClust *varSTPtr,
   struct set_profClust *setSTPtr
);

/*-------------------------------------------------------\
| Fun28: addReadRef_var_profClust
|   - adds the variants in a read to var_profClust struct
|     using input reference
| Input:
|   - qrySTPtr:
|     o pointer to samEntry struct with read to add
|   - refSTPtr:
|     o pointer to samEntry struct with reference
|   - varSTPtr:
|     o pointer to var_profClust struct to add read
|       variants to
|   - setSTPtr:
|     o pointer to a set_profClust struct with settings
| Output:
|   - Mofifies:
|     o varSTPtr to have variants added to arrays
\-------------------------------------------------------*/
void
addReadRef_var_profClust(
   struct samEntry *qrySTPtr,
   struct samEntry *refSTPtr,
   struct var_profClust *varSTPtr,
   struct set_profClust *setSTPtr
);
