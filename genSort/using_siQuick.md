# Use:

siQuick holds the quick (sort, select, and heap) programs
  for signed ints.

Files: siQuick.c and siQuick.h

# siQuick:

- Functions:
  - `select_siQuick`: quick select (not tested)
  - `selectSync_siQuick`: quick select while keeping
    another signed int array in sync (not tested)
  - `sort_siQuick`: quick sort (not tested)
  - no quick heap yet

## select\_siQuick

The `select_siQuick` function does a quick select and
  partially sorts your array (a side effect of quick
  select).

- Input:
  1. signed int array to sort
  2. index of value to return back
     - set to last item (input 4) if is greater then
       start (input 3) - end (input 4)
  3. first index (index 0) in array to select from 
  4. last index (index 0) in array to select from 
- Output:
  - returns the value at the input index (input 2)
- Warning: this algorithm will partially sort your data
- O: O(n) (from online)

Untested example:

```
#ifdef PLAN9
   #include <libc.h>
#else
   #include <stdlib.h>
#endif

#include <stdio.h>
#include "../genSortAndHeap/siQuick.h"

int
main(
){
   #define def_size_main
   signed int siAry[def_size_main];
   signed int siVal = 0;

   /*_________fill_array_with_random_values_____________*/
   for(siVal = 0; siVal < 1000; ++siVal)
      siAry[siVal] = rand();

   /*___________find_the_value_at_the_middle____________*/
   siVal =                   /*gets midpionts value*/
      select_siQuick(
         siAry,              /*array to sort*/
         def_size_main / 2,  /*find the midpoint*/
         0,                  /*start at index one*/
         def_size_main - 1   /*end at last index*/
      );

   printf("value at %i is %i\n", def_size_main/2, siVal);
} /*main*/
```

## sort_siQuick

Sorts a signed int array using quick sort.

- Input:
  1. signed int array to sort
  2. first index (index 0) to sort
  3. last index (index 0) to sort

Untested example:

```
#ifdef PLAN9
   #include <libc.h>
#else
   #include <stdlib.h>
#endif

#include <stdio.h>
#include "../genSortAndHeap/siQuick.h"

int
main(
){
   #define def_size_main
   signed int siAry[def_size_main];
   signed int siVal = 0;

   /*_________fill_array_with_random_values_____________*/
   for(siVal = 0; siVal < 1000; ++siVal)
      siAry[siVal] = rand();

   /*___________sort_the_array__________________________*/
   sort_siQuick(siAry, 0, def_size_main - 1);
} /*main*/
```
