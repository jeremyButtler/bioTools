scriptDirStr="$(printf "%s" "$(cd "$(dirname "$0")" && pwd)")"

helpStr="$(basename "$0") program_name main_file library_one library_two library_three ... library_n
   - builds make files for unix, windows, and plan9
   o program_name: name to call compiled program
   o main_file: main file of program (without .c extension)
   o library_number: library program uses (do not use
     .c/.h extensions)
     o the script knows the dependencys for each library,
       so you only need to put the top level dependency,
       however, it is ok if you wish to include all
       libraries
       - for example; samEntry would also print entries
         for strAry, ulCp, base10str, and numToStr
     o Options:
       - base10str (genLib/base10str)
       - charCp (genLib/charCp.c)
       - cigToEqx (genBio/cigToEqx.c)
       - codonTbl (genBio/codonTbl.c)
       - genMath (genLib/genMath.c)
       - numToStr (genLib/numToStr.c)
       - shellSort (genLib/shellSort.c)
       - strAry (genLib/strAry.c)
       - ulCp (genLib/ulCp.c)

       - adjCoords (genBio/adjCoords.c)
       - edDist (genBio/edDist.c)
       - geneCoord (genBio/geneCoord.c)
       - kmerCnt (genBio/kmerCnt.c)
       - maskPrim (genBio/maskPrim.c)
       - samEntry (genBio/samEntry.c)
       - seqST (genBio/seqST.c)
       - tbCon (genBio/tbCon.c)
       - trimSam (genBio/trimSam.c)

       - alnSet (genAln/alnSet.c)
       - dirMatrix (genAln/dirMatrix.c)
       - indexToCoord (genAln/indexToCoord.c)
       - kmerFind (genAln/kmerFind.c)
       - memwater (genAln/memwater.c)
       - needle (genAln/needle.c)
       - samToAln (genAln/samToAln.c)
       - water (genAln/water.c)

       - illNano (genClust/illNano.c)
"

if [[ $1 == "" || $1 == "-h" || $1 == "--h" ]]; then
   printf "%s\n" "$helpStr";
   exit
fi
# $* is all input

bash "$scriptDirStr/buildMkfile.sh" bug $* > mkfile.bug
bash "$scriptDirStr/buildMkfile.sh" unix $* > mkfile.unix
bash "$scriptDirStr/buildMkfile.sh" static $* > mkfile.static
bash "$scriptDirStr/buildMkfile.sh" win $* > mkfile.win
bash "$scriptDirStr/buildMkfile.sh" plan9 $* > mkfile.plan9
