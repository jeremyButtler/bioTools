# Use:

siBinSearch holds the binary search functions for signed
  ints.

Files: siBinSearch.c siBinSearch.h

- Functions:
  - `get_siBinSearch`: find a signed int in a sorted array
  - `getStart_siBinSearch`: find the first index of a
    value that matches the query in a sorted array

## get\_siBinSearch:

Finds a index of a number in a sorted signed int array.

- Input:
  1. number to find
  2. sorted signed int array to search
  3. length of the signed int array
- Output:
  - Returns:
    - index the number (input 1) is at
    - -1 if the number (input 1) could not be found

```
#ifdef PLAN9
   #include <u.h>
   #include <libc.h>
#else
   #include <stdlib.h>
#endif

#include <stdio.h>
#include "siBinSearch.h"

int
main(
   void
){
   signed int arraySI[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8,9};

   printf(
      "5 is at index %i\n",
      get_siBinSearch(5, arraySI, 10)
   );
} /*main*/
```

## getStart\_siBinSearch:

Finds the first index of the first time a number in a
  appears in a sorted signed int array.

- Input:
  1. number to find
  2. sorted signed int array to search
  3. length of the signed int array
- Output:
  - Returns:
    - index of the first match to the number (input 1)
    - -1 if the number (input 1) could not be found

```
#ifdef PLAN9
   #include <u.h>
   #include <libc.h>
#else
   #include <stdlib.h>
#endif

#include <stdio.h>
#include "siBinSearch.h"

int
main(
   void
){
   signed int arraySI[10] = {0, 1, 2, 3, 5, 5, 6, 7, 8,9};

   printf(
      "the first 5 is at index %i\n",
      getStart_siBinSearch(5, arraySI, 10)
   );
} /*main*/
```
