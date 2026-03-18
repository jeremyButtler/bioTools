/*test if I can build a minimum spanning tree from a
`  file with lineages. The input file is named
`  `testFile.txt`. One possible answer is in
`  `testFileWorked.txt`
*/
#ifdef PLAN9
   #include <u.h>
   #include <libc.h>
#else
   #include <stdlib.h>
#endif

#include <stdio.h>
#include "../runPrim.h"

/*also need:
`   1. ../prim.h
`   2. ../../genSort/siBinSearch.h
`   3. ../../genLib/ulCp.h
`   4. ../../genLib/fileFun.h
`   5. ../../genLib/genMath.h
`   6. ../../genLib/endLine.h (.h only)
*/

int
main(
   void
){
   signed char errSC = 0;
   signed long errSL = 0;

   struct mst_runPrim *mstHeapST = 0;
   FILE *featureFILE = fopen("testFile.txt", "r");
   FILE *outFILE = fopen("delete.newick", "w");

   mstHeapST =
      indexFeatureFile_runPrim(featureFILE, &errSC);
   if(errSC)
      goto cleanUp_main;

   errSL = build_mst_runPrim(featureFILE, 0, mstHeapST);

   if(errSL)
      errSC = 1;
   else
   { /*Else: no error*/
      errSC = 0;
      mstToNewick_mst_runPrim(mstHeapST, outFILE);
   } /*Else: no error*/

   cleanUp_main:;
      fclose(outFILE);
      outFILE = 0;

      fclose(featureFILE);
      featureFILE = 0;

      freeHeap_mst_runPrim(mstHeapST);
      mstHeapST = 0;

      return errSC;
} /*main*/
