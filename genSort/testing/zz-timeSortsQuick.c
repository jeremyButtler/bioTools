/*compare how the stack and non-stack quick sorts run
`   I suspect the times will be similar or worse for the
`   the stack
`As I suspected the recursive method is a bit faster
*/

#ifdef PLAN9
   #include <u.h>
   #include <libc.h>
#else
   #include <stdlib.h>
#endif

#include <stdio.h>
#include <time.h>

#include "../siQuick.h"

int
main(
   void
){
   #define def_size 100000
   signed int siStackAry[def_size];
   signed int siRecAry[def_size];

   signed int siPos = 0;
   clock_t start;
   clock_t end;

   srand(time(NULL));
   for(siPos = 0; siPos < def_size; ++siPos)
   { /*Loop: create the random array*/
      siStackAry[siPos] = rand() % def_size;
      siRecAry[siPos] = siStackAry[siPos];
   } /*Loop: create the random array*/

   start = clock();
   sortWithStack_siQuick(siStackAry, def_size);
   end = clock();

   printf(
      "Stack: %f\t%i\n",
      (double) (end - start) / CLOCKS_PER_SEC,
      siStackAry[0]
   );

   start = clock();
   sort_siQuick(siRecAry, def_size);
   end = clock();

   printf(
      "Rec__: %f\t%i\n",
      (double) (end - start) / CLOCKS_PER_SEC,
      siRecAry[0]
   );

   return 0;
} /*main*/
