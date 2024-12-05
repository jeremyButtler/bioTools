#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# buildMkfile SOF:
#   - builds a mkfile for a specific OS
#   o sec01:
#     - variable declarations
#   o sec02:
#     - set up headers and variables for specified mkfile
#   o sec03:
#     - set up mkfile build commands
#   o sec04:
#     - build make file command
#   o sec05:
#     - print mkfile (makefile)
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# Sec01:
#   - variable declarations
#<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

slashSC=""; # OS file separator

# potential .h/.c file locations (libraries)
genLibStr="";
genBioStr="";
genAlnStr="";
genClustStr="";
seqByIdStr="";

# marks if library location was used
genLibBl=0;
genBioBl=0;
genAlnBl=0;
genClustBl=0;
seqByIdBl=0;

# for building compile calls
cflagsStr=""; # CFLAGS
ccStr="";     # CC
ldStr="";     # LD (linker)
dashOStr="";  # -o
dashCStr="";  # -c
spaceStr="   ";  # spacing for objFiles entry
newLineStr="";# holds new line to add between commands

# for printing out make file
headStr="";     # first part of file
libPathStr="";  # library paths
linkStr="";     # link (all:) command
objFilesStr=""; # object files for copiling (objFiles)
mainCmdStr="";  # commat to build main .c file
cmdsStr="";     # commands to build object files
cleanStr="";    # command to clean up object files
installStr="";  # install command

osStr="$1";       # OS/mkfile type
nameStr="$2";       # program name
mainFileStr="$3"; # main file input by user

depAry=(); # depends needed from libraries
lenDep=0;  # number of dependencies
depSI=-1;  # index on in dependency array

helpStr="$(basename "$0") OS program_name main_file libraries ...
   - OS: OS t build mkfile for
     o bug: debug
     o unix: general unix (Linux/BSD/MAC)
     o static: unix supporting static (Linux/BSD)
     o win: windows
     o plan9: plan9
   - program_name:
     o what to name the output program
   - main_file:
     o main file for program
     o do not use .c exentsion
   - libraries:
     o libraries to include
     o do not use extensions (.c/.h)
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
       - clustST (genClust/clustST.c)
       - edClust (genClust/edClust.c)
"
 

#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# Sec02:
#   - set up headers and variables for specified mkfile
#   o sec02 sub01:
#     - debuging mkfiles
#   o sec02 sub02:
#     - unix mkfiles
#   o sec02 sub03:
#     - static (unix) mkfiles
#   o sec02 sub04:
#     - windows mkfiles
#   o sec02 sub05:
#     - plan9 mkfiles
#<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

if [[ "$1" == "" || "$1" == "-h" || "$1" == "--h" ]]; then
   printf "%s\n" "$helpStr";
   exit;
fi

if [[ "$2" == "" || "$2" == "-h" || "$2" == "--h" ]]; then
   printf "%s\n" "$helpStr";
   exit;
fi

if [[ "$3" == "" || "$3" == "-h" || "$3" == "--h" ]]; then
   printf "%s\n" "$helpStr";
   exit;
fi

#*********************************************************
# Sec02 Sub01:
#   - debuging mkfiles
#*********************************************************

if [[ "$osStr" == "bug" ]];
then # If: debugging make file
   headStr="LD=cc";
   headStr="$headStr\nCC=cc";
   headStr="$headStr\nCFLAGS= -O0 -std=c89 -Wall -Wextra";
   headStr="$headStr -Werror -ggdb -g";
   headStr="$headStr\nNAME=$nameStr";
   headStr="$headStr\nPREFIX=/usr/local/bin";
   headStr="$headStr\nO=o.bug";

   genLibStr="\$(genLib)/";
   genBioStr="\$(genBio)/";
   genAlnStr="\$(genAln)/";
   genClustStr="\$(genClust)/";
   seqByIdStr="\$(seqByIdSrc)/";

   cflagsStr="\$(CFLAGS)";
   ccStr="\$(CC)";
   ldStr="\$(LD)";
   dashOStr="-o ";
   dashCStr="-c";

   installStr="";

   cleanStr="clean:"
   cleanStr="$cleanStr\n\trm \$(objFiles)";

   linkStr="all: $mainFileStr.\$O";
   linkStr="$linkStr\n\t\$(LD) "$dashOStr"\$(NAME) \$(CFLAGS)";
   linkStr="$linkStr \$(objFiles)";

   slashSC="/";
# If: debugging make file

#*********************************************************
# Sec02 Sub02:
#   - unix mkfiles
#*********************************************************

elif [[ "$osStr" == "unix" ]];
then # Else If: general unix make file
   headStr="LD=cc";
   headStr="$headStr\nCC=cc";
   headStr="$headStr\nCFLAGS= -O3 -std=c89 -Wall -Wextra";
   headStr="$headStr\nNAME=$nameStr";
   headStr="$headStr\nPREFIX=/usr/local/bin";
   headStr="$headStr\nO=o.unix";

   genLibStr="\$(genLib)/";
   genBioStr="\$(genBio)/";
   genAlnStr="\$(genAln)/";
   genClustStr="\$(genClust)/";
   seqByIdStr="\$(seqByIdSrc)/";

   cflagsStr="\$(CFLAGS)";
   ccStr="\$(CC)";
   ldStr="\$(LD)";
   dashOStr="-o ";
   dashCStr="-c";

   installStr="install:"
   installStr="$installStr\n\tcp \$(NAME) \$(PREFIX)"
   installStr="$installStr\n\tchmod a+x \$(PREFIX)/\$(NAME)"

   cleanStr="clean:"
   cleanStr="$cleanStr\n\trm \$(objFiles)";

   linkStr="all: $mainFileStr.\$O";
   linkStr="$linkStr\n\t\$(LD) "$dashOStr"\$(NAME) \$(CFLAGS)";
   linkStr="$linkStr \$(objFiles)";

   slashSC="/";
# Else If: general unix make file

#*********************************************************
# Sec02 Sub03:
#   - static (unix) mkfiles
#*********************************************************

elif [[ "$osStr" == "static" ]];
then # Else If: static unix make file
   headStr="LD=cc";
   headStr="$headStr\nCC=cc";
   headStr="$headStr\nCFLAGS= -O3 -std=c89 -static";
   headStr="$headStr -Wall -Wextra";
   headStr="$headStr\nNAME=$nameStr";
   headStr="$headStr\nPREFIX=/usr/local/bin";
   headStr="$headStr\nO=o.static";

   genLibStr="\$(genLib)/";
   genBioStr="\$(genBio)/";
   genAlnStr="\$(genAln)/";
   genClustStr="\$(genClust)/";
   seqByIdStr="\$(seqByIdSrc)/";

   cflagsStr="\$(CFLAGS)";
   ccStr="\$(CC)";
   ldStr="\$(LD)";
   dashOStr="-o ";
   dashCStr="-c";

   installStr="install:"
   installStr="$installStr\n\tcp \$(NAME) \$(PREFIX)"
   installStr="$installStr\n\tchmod a+x \$(PREFIX)/\$(NAME)"

   cleanStr="clean:"
   cleanStr="$cleanStr\n\trm \$(objFiles)";

   linkStr="all: $mainFileStr.\$O";
   linkStr="$linkStr\n\t\$(LD) "$dashOStr"\$(NAME) \$(CFLAGS)";
   linkStr="$linkStr \$(objFiles)";

   slashSC="/";
# Else If: static unix make file

#*********************************************************
# Sec02 Sub04:
#   - windows mkfiles
#*********************************************************

elif [[ "$osStr" == "win" ]];
then # Else If: windows make file
   headStr="LD=link";
   headStr="$headStr\nCC=cl";
   headStr="$headStr\nCFLAGS= /O2 /Ot /Za /Tc";
   headStr="$headStr\nNAME=$nameStr.exe";
   headStr="$headStr\nPREFIX=%%HOME%%\\\\appData";
   headStr="$headStr\nO=o.win";

   genLibStr="\$(genLib)\\\\";
   genBioStr="\$(genBio)\\\\";
   genAlnStr="\$(genAln)\\\\";
   genClustStr="\$(genClust)\\\\";
   seqByIdStr="\$(seqByIdSrc)\\\\";

   cflagsStr="\$(CFLAGS)";
   ccStr="\$(CC)";
   ldStr="\$(LD)";
   dashOStr="/Fo:";
   dashCStr="/c";

   installStr="install:"
   installStr="$installStr\n\tcopy \$(NAME) \$(PREFIX)"

   cleanStr="clean:"
   cleanStr="$cleanStr\n\tdel \$(objFiles)";

   linkStr="all: $mainFileStr.\$O";
   linkStr="$linkStr\n\t\$(LD) /out:\$(NAME)";
   linkStr="$linkStr \$(objFiles)";

   slashSC="\\";
# Else If: windows make file

#*********************************************************
# Sec02 Sub05:
#   - plan9 mkfiles
#*********************************************************

elif [[ "$osStr" == "plan9" ]];
then # Else If: plan9 make file
   headStr="</\$objtype/mkfile";
   headStr="$headStr\nCFLAGS=-DPLAN9";
   headStr="$headStr\nNAME=$nameStr";
   headStr="$headStr\nPREFIX=\$home/bin";

   genLibStr="\$genLib/";
   genBioStr="\$genBio/";
   genAlnStr="\$genAln/";
   genClustStr="\$genClust/";
   seqByIdStr="\$seqByIdSrc/";

   cflagsStr="\$CFLAGS";
   ccStr="\$CC";
   ldStr="\$LD";
   dashOStr="-o ";
   dashCStr="";

   installStr="install:"
   installStr="$installStr\n\tcp \$(NAME) \$(PREFIX)"
   installStr="$installStr\n\tchmod a+x \$(PREFIX)/\$(NAME)"

   cleanStr="clean:"
   cleanStr="$cleanStr\n\trm \$(objFiles)";

   linkStr="all: $mainFileStr.\$O";
   linkStr="$linkStr\n\t\$LD "$dashOStr"\$NAME \$CFLAGS";
   linkStr="$linkStr \$objFiles";

   slashSC="/";
# Else If: plan9 make file

else
   exit; # not make file
fi # check makefile type

#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# Sec03:
#   - set up mkfile build commands
#   o sec03 sub01:
#     - setup start of main file build command
#   o sec03 sub02:
#     - genLib libraries
#   o sec03 sub03:
#     - genBio libraries
#   o sec03 sub04:
#     - genAln libraries
#   o sec03 sub05:
#     - genClust libraries
#   o sec03 sub06:
#     - non-general libraries
#     - not general enough to be a general library
#<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

#*********************************************************
# Sec03 Sub01:
#   - setup start of main file build command
#*********************************************************

mainCmdStr="$mainFileStr.\$O: $mainFileStr.c"

#*********************************************************
# Sec03 Sub02:
#   - genLib libraries
#   o sec03 sub02 cat01:
#     - numToStr
#   o sec03 sub02 cat02:
#     - base10str
#   o sec03 sub02 cat03:
#     - ulCp
#   o sec03 sub02 cat04:
#     - charCp
#   o sec03 sub02 cat05:
#     - genMath
#   o sec03 sub02 cat06:
#     - shellSort
#   o sec03 sub02 cat07:
#     - rmBlocks
#   o sec03 sub02 cat08:
#     - strAry
#   o sec03 sub02 cat09:
#     - ptrAry
#*********************************************************

#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# Sec03 Sub02 Cat01:
#   - numToStr
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++

numToStr=""$genLibStr"numToStr.\$O: "$genLibStr"numToStr.c "$genLibStr"numToStr.h
	$ccStr "$dashOStr""$genLibStr"numToStr.\$O "$dashCStr" $cflagsStr "$genLibStr"numToStr.c"

numToStrObj=""$genLibStr"numToStr.\$O";
numToStrDep=("");
lenNumToStrDep=1;

#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# Sec03 Sub02 Cat02:
#   - base10str
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++

base10Str=""$genLibStr"base10str.\$O: "$genLibStr"base10str.c "$genLibStr"base10str.h
	$ccStr "$dashOStr""$genLibStr"base10str.\$O "$dashCStr" $cflagsStr "$genLibStr"base10str.c"

base10strObj=""$genLibStr"base10str.\$O";

base10strDep=("");
lenBase10strDep=1;

#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# Sec03 Sub02 Cat03:
#   - ulCp
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++

ulCpStr=""$genLibStr"ulCp.\$O: "$genLibStr"ulCp.c "$genLibStr"ulCp.h
	$ccStr "$dashOStr""$genLibStr"ulCp.\$O "$dashCStr" $cflagsStr "$genLibStr"ulCp.c"

ulCpObj=""$genLibStr"ulCp.\$O";
ulCpDep=("");
lenUlCpDep=1;

#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# Sec03 Sub02 Cat04:
#   - charCp
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++

charCpStr=""$genLibStr"charCp.\$O: "$genLibStr"charCp.c "$genLibStr"charCp.h
	$ccStr "$dashOStr""$genLibStr"charCp.\$O "$dashCStr" $cflagsStr "$genLibStr"charCp.c"

charCpObj=""$genLibStr"charCp.\$O";
charCpDep=("");
lenCharCpDep=1;

#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# Sec03 Sub02 Cat05:
#   - genMath
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++

genMathStr=""$genLibStr"genMath.\$O: "$genLibStr"genMath.c "$genLibStr"genMath.h
	$ccStr "$dashOStr""$genLibStr"genMath.\$O "$dashCStr" $cflagsStr "$genLibStr"genMath.c"

genMathObj=""$genLibStr"genMath.\$O";
genMathDep=("");
lenGenMathDep=1;

#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# Sec03 Sub02 Cat06:
#   - shellSort
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++

shellSortStr=""$genLibStr"shellSort.\$O: "$genLibStr"shellSort.c "$genLibStr"shellSort.h
	$ccStr "$dashOStr""$genLibStr"shellSort.\$O "$dashCStr" $cflagsStr "$genLibStr"shellSort.c"

shellSortObj=""$genLibStr"shellSort.\$O";
shellSortDep=("");
lenShellSortDep=1;

#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# Sec03 Sub02 Cat07:
#   - rmBlocks
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++

rmBlocksStr=""$genLibStr"rmBlocks.\$O: "$genLibStr"rmBlocks.c "$genLibStr"rmBlocks.h
	$ccStr "$dashOStr""$genLibStr"rmBlocks.\$O "$dashCStr" $cflagsStr "$genLibStr"rmBlocks.c"

rmBlocksObj=""$genLibStr"rmBlocks.\$O";
rmBlocksDep=("");
lenRmBlocksDep=1;

#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# Sec03 Sub02 Cat08:
#   - strAry
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++

strAryStr=""$genLibStr"strAry.\$O: "$genLibStr"strAry.c "$genLibStr"strAry.h "$genLibStr"ulCp.\$O
	$ccStr "$dashOStr""$genLibStr"strAry.\$O "$dashCStr" $cflagsStr "$genLibStr"strAry.c";


strAryObj=""$genLibStr"strAry.\$O";
strAryDep=("ulCp" ${ulCpDep[0]});
lenStrAryDep=$((1 + lenUlCpDep));

#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# Sec03 Sub02 Cat09:
#   - ptrAry
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++

ptrAryStr=""$genLibStr"ptrAry.\$O: "$genLibStr"ptrAry.c "$genLibStr"ptrAry.h "$genLibStr"ulCp.\$O
	$ccStr "$dashOStr""$genLibStr"ptrAry.\$O "$dashCStr" $cflagsStr "$genLibStr"ptrAry.c";


ptrAryObj=""$genLibStr"ptrAry.\$O";
ptrAryDep=("ulCp" ${ulCpDep[0]});
lenPtrAryDep=$((1 + lenUlCpDep));

#*********************************************************
# Sec03 Sub03:
#   - genBio libraries
#   o sec03 sub03 cat01:
#     - codonTbl
#   o sec03 sub03 cat02:
#     - seqST
#   o sec03 sub03 cat03:
#     - kmerCnt
#   o sec03 sub03 cat04:
#     - geneCoord
#   o sec03 sub03 cat05:
#     - samEntry
#   o sec03 sub03 cat06:
#     - adjCoords
#   o sec03 sub03 cat07:
#     - cigToEqx
#   o sec03 sub03 cat08:
#     - maskPrim
#   o sec03 sub03 cat09:
#     - trimSam
#   o sec03 sub03 cat10:
#     - ampDepth
#   o sec03 sub03 cat11:
#     - tbCon
#   o sec03 sub03 cat12:
#     - edDist
#*********************************************************

#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# Sec03 Sub03 Cat01:
#   - codonTbl
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++

codonTblStr=""$genBioStr"codonTbl.\$O: "$genBioStr"codonTbl.c "$genBioStr"codonTbl.h "$genBioStr"ntTo2Bit.h "$genBioStr"revNtTo2Bit.h "$genLibStr"dataTypeShortHand.h
	$ccStr "$dashOStr""$genBioStr"codonTbl.\$O "$dashCStr" $cflagsStr "$genBioStr"codonTbl.c"

codonTblObj=""$genBioStr"codonTbl.\$O";
codonTblDep=("");
lenCodonTblDep=1;

#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# Sec03 Sub03 Cat02:
#   - seqST
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++

seqSTStr=""$genBioStr"seqST.\$O: "$genBioStr"seqST.c "$genBioStr"seqST.h "$genLibStr"ulCp.\$O "$genLibStr"dataTypeShortHand.h
	$ccStr "$dashOStr""$genBioStr"seqST.\$O "$dashCStr" $cflagsStr "$genBioStr"seqST.c";


seqSTObj=""$genBioStr"seqST.\$O";
seqSTDep=("ulCp" ${ulCpDep[*]});
lenSeqSTDep=$((1 + lenUlCpDep));

#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# Sec03 Sub03 Cat03:
#   - kmerCnt
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++

kmerCntStr=""$genBioStr"kmerCnt.\$O: "$genBioStr"kmerCnt.c "$genBioStr"kmerCnt.h "$genBioStr"seqST.\$O "$genBioStr"ntTo2Bit.h "$genLibStr"genMath.h
	$ccStr "$dashOStr""$genBioStr"kmerCnt.\$O "$dashCStr" $cflagsStr "$genBioStr"kmerCnt.c";


kmerCntObj=""$genBioStr"kmerCnt.\$O";
kmerCntDep=("seqST" ${seqSTDep})
lenKmerCntDep=$((1 + lenSeqSTDep));

#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# Sec03 Sub03 Cat04:
#   - geneCoord
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++

geneCoordStr=""$genBioStr"geneCoord.\$O: "$genBioStr"geneCoord.c "$genBioStr"geneCoord.h "$genLibStr"base10str.\$O "$genLibStr"ulCp.\$O "$genLibStr"charCp.\$O "$genLibStr"genMath.h "$genLibStr"dataTypeShortHand.h
	$ccStr "$dashOStr""$genBioStr"geneCoord.\$O "$dashCStr" $cflagsStr "$genBioStr"geneCoord.c";

geneCoordObj=""$genBioStr"geneCoord.\$O";
geneCoordDep=("base10str" ${base10strDep[*]} "ulCp" ${ulCpDep[*]} "charCp" ${charCpDep[*]})
lenGeneCoordDep=$((1 + lenBase10strDep + lenUlCpDep + lenCharCpDep));

#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# Sec03 Sub03 Cat05:
#   - samEntry
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++

samEntryStr=""$genBioStr"samEntry.\$O: "$genBioStr"samEntry.c "$genBioStr"samEntry.h "$genLibStr"strAry.\$O "$genLibStr"base10str.\$O "$genLibStr"numToStr.\$O "$genBioStr"ntTo5Bit.h "$genLibStr"dataTypeShortHand.h
	$ccStr "$dashOStr""$genBioStr"samEntry.\$O "$dashCStr" $cflagsStr "$genBioStr"samEntry.c";

samEntryObj=""$genBioStr"samEntry.\$O";
samEntryDep=("strAry" ${strAryDep[*]} "numToStr" ${numToStrDep[*]} "base10str" ${base10strDep[*]})
lenSamEntryDep=$((1 + lenStrAryDep + lenNumToStrDep + lenBase10strDep));

#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# Sec03 Sub03 Cat06:
#   - adjCoords
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++

adjCoordsStr=""$genBioStr"adjCoords.\$O: "$genBioStr"adjCoords.c "$genBioStr"adjCoords.h "$genBioStr"geneCoord.\$O "$genBioStr"samEntry.\$O
	$ccStr "$dashOStr""$genBioStr"adjCoords.\$O "$dashCStr" $cflagsStr "$genBioStr"adjCoords.c";

adjCoordsObj=""$genBioStr"adjCoords.\$O";
adjCoordsDep=("geneCoord" ${geneCoordDep[*]} "samEntry" ${samEntryDep[*]});
lenAdjCoordsDep=$((1 + lenGeneCoordDep + lenSamEntryDep));

#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# Sec03 Sub03 Cat07:
#   - cigToEqx
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++

cigToEqxStr=""$genBioStr"cigToEqx.\$O: "$genBioStr"cigToEqx.c "$genBioStr"cigToEqx.h "$genBioStr"samEntry.\$O
	$ccStr "$dashOStr""$genBioStr"cigToEqx.\$O "$dashCStr" $cflagsStr "$genBioStr"cigToEqx.c";

cigToEqxObj=""$genBioStr"cigToEqx.\$O";
cigToEqxDep=("samEntry" ${samEntryDep[*]});
lenCigToEqxDep=$((1 + lenSamEntryDep));

#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# Sec03 Sub03 Cat08:
#   - maskPrim
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++

maskPrimStr=""$genBioStr"maskPrim.\$O: "$genBioStr"maskPrim.c "$genBioStr"maskPrim.h "$genBioStr"samEntry.\$O "$genLibStr"shellSort.\$O
	$ccStr "$dashOStr""$genBioStr"maskPrim.\$O "$dashCStr" $cflagsStr "$genBioStr"maskPrim.c";

maskPrimObj=""$genBioStr"maskPrim.\$O";
maskPrimDep=("samEntry" ${samEntryDep[*]} "shellSort" ${shellSortDep[*]});
lenMaskPrimDep=$((1 + lenSamEntryDep + lenShellSortDep));

#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# Sec03 Sub03 Cat09:
#   - trimSam
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++

trimSamStr=""$genBioStr"trimSam.\$O: "$genBioStr"trimSam.c "$genBioStr"trimSam.h "$genBioStr"samEntry.\$O
	$ccStr "$dashOStr""$genBioStr"trimSam.\$O "$dashCStr" $cflagsStr "$genBioStr"trimSam.c";

trimSamObj=""$genBioStr"trimSam.\$O";
trimSamDep=("samEntry" ${samEntryDep[*]});
lenTrimSamDep=$((1 + lenSamEntryDep));

#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# Sec03 Sub03 Cat10:
#   - ampDepth
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++

ampDepthStr=""$genBioStr"ampDepth.\$O: "$genBioStr"ampDepth.c "$genBioStr"ampDepth.h "$genBioStr"geneCoord.\$O "$genBioStr"samEntry.\$O
	$ccStr "$dashOStr""$genBioStr"ampDepth.\$O "$dashCStr" $cflagsStr "$genBioStr"ampDepth.c";

ampDepthObj=""$genBioStr"ampDepth.\$O";
ampDepthDep=("geneCoord" ${geneCoordDep[*]} "samEntry" ${samEntryDep[*]});
lenAmpDepthDep=$((1 + lenGeneCoordDep + lenSamEntryDep));

#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# Sec03 Sub03 Cat11:
#   - tbCon
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++

tbConStr=""$genBioStr"tbCon.\$O: "$genBioStr"tbCon.c "$genBioStr"tbCon.h "$genBioStr"samEntry.\$O "$genBioStr"tbConDefs.h "$genLibStr"genMath.h
	$ccStr "$dashOStr""$genBioStr"tbCon.\$O "$dashCStr" $cflagsStr "$genBioStr"tbCon.c";

tbConObj=""$genBioStr"tbCon.\$O";
tbConDep=("samEntry" ${samEntryDep[*]});
lenTbConDep=$((1 + lenSamEntryDep));

#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# Sec03 Sub03 Cat12:
#   - edDist
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++

edDistStr=""$genBioStr"edDist.\$O: "$genBioStr"edDist.c "$genBioStr"edDist.h "$genBioStr"samEntry.\$O "$genBioStr"seqST.\$O "$genLibStr"genMath.h
	$ccStr "$dashOStr""$genBioStr"edDist.\$O "$dashCStr" $cflagsStr "$genBioStr"edDist.c";


edDistObj=""$genBioStr"edDist.\$O";
edDistDep=("samEntry" ${samEntryDep[*]} "seqST" ${seqSTDep[*]});
lenEdDistDep=$((1 + lenSamEntryDep + lenSeqSTDep));

#*********************************************************
# Sec03 Sub04:
#   - genAln libraries
#   o sec03 sub04 cat01:
#     - indexToCoord
#   o sec03 sub04 cat02:
#     - alnSet
#   o sec03 sub04 cat03:
#     - dirMatrix
#   o sec03 sub04 cat04:
#     - water
#   o sec03 sub04 cat05:
#     - needle
#   o sec03 sub04 cat06:
#     - memwater
#   o sec03 sub04 cat07:
#     - kmerFind
#   o sec03 sub04 cat08:
#     - samToAln
#*********************************************************

#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# Sec03 Sub04 Cat01:
#   - indexToCoord
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++

indexToCoordStr=""$genAlnStr"indexToCoord.\$O: "$genAlnStr"indexToCoord.c
	$ccStr "$dashOStr""$genAlnStr"indexToCoord.\$O "$dashCStr" $cflagsStr "$genAlnStr"indexToCoord.c";


indexToCoordObj=""$genAlnStr"indexToCoord.\$O";
indexToCoordDep=("");
lenIndexToCoordDep=0;

#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# Sec03 Sub04 Cat02:
#   - alnSet
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++

alnSetStr=""$genAlnStr"alnSet.\$O: "$genAlnStr"alnSet.c "$genAlnStr"alnDefs.h "$genLibStr"base10str.\$O "$genLibStr"dataTypeShortHand.h
	$ccStr "$dashOStr""$genAlnStr"alnSet.\$O "$dashCStr" $cflagsStr "$genAlnStr"alnSet.c";


alnSetObj=""$genAlnStr"alnSet.\$O";
alnSetDep=("base10str" ${base10strDep[*]});
lenAlnSetDep=$((1 + lenBase10strDep));

#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# Sec03 Sub04 Cat03:
#   - dirMatrix
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++

dirMatrixStr=""$genAlnStr"dirMatrix.\$O: "$genAlnStr"dirMatrix.c "$genAlnStr"alnSet.\$O "$genAlnStr"indexToCoord.\$O "$genBioStr"samEntry.\$O "$genBioStr"seqST.\$O "$genLibStr"charCp.\$O
	$ccStr "$dashOStr""$genAlnStr"dirMatrix.\$O "$dashCStr" $cflagsStr "$genAlnStr"dirMatrix.c";


dirMatrixObj=""$genAlnStr"dirMatrix.\$O";
dirMatrixDep=("alnSet" ${alnSetDep[*]} "indexToCoord" ${indexToCoordDep[*]} "samEntry" ${samEntryDep[*]} "seqST" ${seqSTDep[*]} "charCp" ${charCpDep[*]});
lenDirMatrixDep=$((1 + lenAlnSetDep + lenIndexToCoordDep + lenSamEntryDep + lenSeqSTDep + lenCharCpDep));

#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# Sec03 Sub04 Cat04:
#   - water
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++

waterStr=""$genAlnStr"water.\$O: "$genAlnStr"water.c "$genAlnStr"dirMatrix.\$O "$genLibStr"genMath.h
	$ccStr "$dashOStr""$genAlnStr"water.\$O "$dashCStr" $cflagsStr "$genAlnStr"water.c";


waterObj=""$genAlnStr"water.\$O";
waterDep=("dirMatrix" ${dirMatrixDep[*]});
lenWaterDep=$((1 + lenDirMatrixDep));

#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# Sec03 Sub04 Cat05:
#   - needle
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++

needleStr=""$genAlnStr"needle.\$O: "$genAlnStr"needle.c "$genAlnStr"dirMatrix.\$O "$genLibStr"genMath.h
	$ccStr "$dashOStr""$genAlnStr"needle.\$O "$dashCStr" $cflagsStr "$genAlnStr"needle.c";


needleObj=""$genAlnStr"needle.\$O";
needleDep=("dirMatrix" ${dirMatrixDep[*]});
lenNeedleDep=$((1 + lenDirMatrixDep));

#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# Sec03 Sub04 Cat06:
#   - memwater
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++

memwaterStr=""$genAlnStr"memwater.\$O: "$genAlnStr"memwater.c "$genAlnStr"alnSet.\$O "$genAlnStr"indexToCoord.\$O "$genBioStr"seqST.\$O "$genLibStr"genMath.h
	$ccStr "$dashOStr""$genAlnStr"memwater.\$O "$dashCStr" $cflagsStr "$genAlnStr"memwater.c";


memwaterObj=""$genAlnStr"memwater.\$O";
memwaterDep=("alnSet" ${alnSetDep[*]} "indexToCoord" ${indexToCoordDep[*]} "seqST" ${seqSTDep[*]});
lenMemwaterDep=$((1 + lenAlnSetDep + lenIndexToCoordDep + lenSeqSTDep));

#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# Sec03 Sub04 Cat07:
#   - kmerFind
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++

kmerFindStr=""$genAlnStr"kmerFind.\$O: "$genAlnStr"kmerFind.c "$genAlnStr"memwater.\$O "$genBioStr"seqST.\$O  "$genLibStr"shellSort.\$O "$genLibStr"genMath.h
	$ccStr "$dashOStr""$genAlnStr"kmerFind.\$O "$dashCStr" $cflagsStr "$genAlnStr"kmerFind.c";


kmerFindObj=""$genAlnStr"kmerFind.\$O";
kmerFindDep=("memwater" ${memwaterDep[*]} "shellSort" ${shellSortDep[*]});
lenKmerFindDep=$((1 + lenMemwaterDep + lenShellShortDep));

#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# Sec03 Sub04 Cat08:
#   - samToAln
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++

samToAlnStr=""$genAlnStr"samToAln.\$O: "$genAlnStr"samToAln.c "$genAlnStr"samToAln.h "$genBioStr"samEntry.\$O "$genBioStr"seqST.\$O "$genAlnStr"alnSet.\$O
	$ccStr "$dashOStr""$genAlnStr"samToAln.\$O "$dashCStr" $cflagsStr "$genAlnStr"samToAln.c";


samToAlnObj=""$genAlnStr"samToAln.\$O";
samToAlnDep=("alnSet" ${alnSetDep[*]} "samEntry" ${samEntryDep[*]} "seqST" ${seqSTDep[*]});
lenSamToAlnDep=$((1 + lenAlnSetDep + lenSamEntryDep + lenSeqSTDep));

#*********************************************************
# Sec03 Sub05:
#   - genClust libraries
#   o sec03 sub04 cat01:
#     - illNano
#   o sec03 sub04 cat02:
#     - clustST
#   o sec03 sub04 cat03:
#     - edClust
#*********************************************************

#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# Sec03 Sub05 Cat01:
#   - illNano
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++

illNanoStr=""$genClustStr"illNano.\$O: "$genClustStr"illNano.c "$genClustStr"illNano.h "$genBioStr"samEntry.\$O
	$ccStr "$dashOStr""$genClustStr"illNano.\$O "$dashCStr" $cflagsStr "$genClustStr"illNano.c";


illNanoObj=""$genClustStr"illNano.\$O";
illNanoDep=("samEntry" ${samEntryDep[*]});
lenIllNanoDep=$((1 + lenSamEntryDep));

#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# Sec03 Sub05 Cat02:
#   - clustST
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++

clustSTStr=""$genClustStr"clustST.\$O: "$genClustStr"clustST.c "$genClustStr"clustST.h "$genBioStr"samEntry.\$O "$genBioStr"tbCon.\$O "$genBioStr"edDist.\$O "$genLibStr"genMath.h
	$ccStr "$dashOStr""$genClustStr"clustST.\$O "$dashCStr" $cflagsStr "$genClustStr"clustST.c";

clustSTObj=""$genClustStr"clustST.\$O";
clustSTDep=("samEntry" ${samEntryDep[*]} "tbCon" ${tbConDep[*]} "edDist" ${edDistDep[*]});
lenClustSTDep=$((1 + lenSamEntryDep + lenTbConDep + lenEdDistDep));

#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# Sec03 Sub05 Cat03:
#   - edClust
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++

edClustStr=""$genClustStr"edClust.\$O: "$genClustStr"edClust.c "$genClustStr"edClust.h "$genClustStr"clustST.\$O
	$ccStr "$dashOStr""$genClustStr"edClust.\$O "$dashCStr" $cflagsStr "$genClustStr"edClust.c";

edClustObj=""$genClustStr"edClust.\$O";
edClustDep=("clustST" ${clustSTDep[*]});
lenEdClustDep=$((1 + lenClustSTDep));

#*********************************************************
# Sec03 Sub06:
#   - non-general libraries
#   - not general enough to be a general library
#   o sec03 sub06 cat01:
#     - k2TaxaId
#*********************************************************

#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# Sec03 Sub06 Cat01:
#   - k2TaxaId
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++

tmpStr=".."$slashSC"k2TaxaIdSrc"$slashSC"k2TaxaId";
k2TaxaIdStr="$tmpStr.\$O: $tmpStr.c $tmpStr.h "$genLibStr"ptrAry.\$O "$genLibStr"shellSort.\$O "$genLibStr"numToStr.\$O "$genLibStr"base10str.\$O "$genLibStr"genMath.\$O
	$ccStr $dashOStr$tmpStr.\$O "$dashCStr" $cflagsStr $tmpStr.c";

k2TaxaIdObj="$tmpStr.\$O";
k2TaxaIdDep=("ptrAry" ${ptrAryDep[*]} "base10str" ${base10strDep[*]} "numToStr" ${numToStrDep[*]} "genMath" ${genMathDep[*]} "shellSort" ${shellSortDep[*]} );
lenK2TaxaIdDep=$((1 + lenPtrAryDep + lenBase10strDep + lenNumToStrDep + lenGenMathDep + lenShellShortDep));

#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# Sec04:
#   - build make file command
#   o sec04 sub01:
#     o move past name and OS + start loop
#   o sec04 sub02:
#     - genLib libraries
#   o sec04 sub03:
#     - genBio libraries
#   o sec04 sub04:
#     - genAln libraries
#   o sec04 sub05:
#     - genClust libraries
#   o sec04 sub06:
#     - non-general library files
#   o sec04 sub07:
#     - move to next library or dependency
#<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

#*********************************************************
# Sec04 Sub01:
#   o move past name and OS + start loop
#*********************************************************

objFilesStr="objFiles=";
depSI=0;
lenDep=0;

# move past header
shift; # program name
shift; # os type

libStr="$1";
genLibBl=0;

while [[ $# -gt 0 || "$depSI" -lt "$lenDep" ]]; do

   if [[ "$firstBl" == "" ]]; then
      newLineStr="";
      firstBl=1;
   else
      newLineStr="\n\n";
   fi 

   #******************************************************
   # Sec04 Sub02:
   #   - genLib libraries
   #   o sec04 sub02 cat01:
   #     - numToStr
   #   o sec04 sub02 cat02:
   #     - base10str
   #   o sec04 sub02 cat03:
   #     - ulCp
   #   o sec04 sub02 cat04:
   #     - charCp
   #   o sec04 sub02 cat05:
   #     - genMath
   #   o sec04 sub02 cat06:
   #     - shellSort
   #   o sec04 sub02 cat07:
   #     - rmBlocks
   #   o sec04 sub02 cat08:
   #     - strAry
   #   o sec04 sub02 cat09:
   #     - ptrAry
   #******************************************************

   #++++++++++++++++++++++++++++++++++++++++++++++++++++++
   # Sec04 Sub02 Cat01:
   #   - numToStr
   #++++++++++++++++++++++++++++++++++++++++++++++++++++++

   if [[ "$libStr" == "numToStr" ]]; then
   # If: numToStr library
      if [[ "$numToStrBl" == "" ]]; then
         cmdStr="$cmdStr$newLineStr$numToStr";
         objFilesStr="$objFilesStr \\\\\n$spaceStr";
         objFilesStr=""$objFilesStr""$numToStrObj"";

         if [[ "$mainLibBl" == "" ]]; then
            mainCmdStr="$mainCmdStr $numToStrObj";
         fi

         numToStrBl=1;

         depAry+=(${numToStrDep[*]});
         lenDep=$((lenDep + lenNumToStrDep));
      fi

      if [[ "$genLibBl" -lt 1 ]]; then
         genLibBl=1;
         libPathStr="$libPathStr\ngenLib=.."$slashSC"genLib";
      fi
   # If: numToStr library

   #++++++++++++++++++++++++++++++++++++++++++++++++++++++
   # Sec04 Sub02 Cat02:
   #   - base10str
   #++++++++++++++++++++++++++++++++++++++++++++++++++++++

   elif [[ "$libStr" == "base10str" ]]; then
   # Else If: base10str library
      if [[ "$base10strBl" == "" ]]; then
         cmdStr="$cmdStr$newLineStr$base10Str";
         objFilesStr="$objFilesStr \\\\\n$spaceStr";
         objFilesStr=""$objFilesStr""$base10strObj"";

         if [[ "$mainLibBl" == "" ]]; then
            mainCmdStr="$mainCmdStr $base10strObj";
         fi

         base10strBl=1;

         depAry+=(${base10strDep[*]});
         lenDep=$((lenDep + lenBase10strDep));
      fi

      if [[ "$genLibBl" -lt 1 ]]; then
         genLibBl=1;
         libPathStr="$libPathStr\ngenLib=.."$slashSC"genLib";
      fi
   # Else If: base10str library

   #++++++++++++++++++++++++++++++++++++++++++++++++++++++
   # Sec04 Sub02 Cat03:
   #   - ulCp
   #++++++++++++++++++++++++++++++++++++++++++++++++++++++

   elif [[ "$libStr" == "ulCp" ]]; then
   # Else If: ulCp library
      if [[ "$ulCpBl" == "" ]]; then
         cmdStr="$cmdStr$newLineStr$ulCpStr";
         objFilesStr="$objFilesStr \\\\\n$spaceStr";
         objFilesStr=""$objFilesStr""$ulCpObj"";

         if [[ "$mainLibBl" == "" ]]; then
            mainCmdStr="$mainCmdStr $ulCpObj";
         fi

         ulCpBl=1;

         depAry+=(${ulCpDep[*]});
         lenDep=$((lenDep + lenUlCpDep));
      fi

      if [[ "$genLibBl" -lt 1 ]]; then
         genLibBl=1;
         libPathStr="$libPathStr\ngenLib=.."$slashSC"genLib";
      fi
   # Else If: ulCp library

   #++++++++++++++++++++++++++++++++++++++++++++++++++++++
   # Sec04 Sub02 Cat04:
   #   - charCp
   #++++++++++++++++++++++++++++++++++++++++++++++++++++++

   elif [[ "$libStr" == "charCp" ]]; then
   # Else If: charCp library
      if [[ "$charCpBl" == "" ]]; then
         cmdStr="$cmdStr$newLineStr$charCpStr";
         objFilesStr="$objFilesStr \\\\\n$spaceStr";
         objFilesStr=""$objFilesStr""$charCpObj"";

         if [[ "$mainLibBl" == "" ]]; then
            mainCmdStr="$mainCmdStr $charCpObj";
         fi

         charCpBl=1;

         depAry+=(${charCpDep[*]});
         lenDep=$((lenDep + lenCharCpDep));
      fi

      if [[ "$genLibBl" -lt 1 ]]; then
         genLibBl=1;
         libPathStr="$libPathStr\ngenLib=.."$slashSC"genLib";
      fi
   # Else If: charCp library

   #++++++++++++++++++++++++++++++++++++++++++++++++++++++
   # Sec04 Sub02 Cat05:
   #   - genMath
   #++++++++++++++++++++++++++++++++++++++++++++++++++++++

   elif [[ "$libStr" == "genMath" ]]; then
   # Else If: genMath library
      if [[ "$genMathBl" == "" ]]; then
         cmdStr="$cmdStr$newLineStr$genMathStr";
         objFilesStr="$objFilesStr \\\\\n$spaceStr";
         objFilesStr=""$objFilesStr""$genMathObj"";

         if [[ "$mainLibBl" == "" ]]; then
            mainCmdStr="$mainCmdStr $genMathObj";
         fi

         genMathBl=1;

         depAry+=(${genMathDep[*]});
         lenDep=$((lenDep + lenGenMathDep));
      fi

      if [[ "$genLibBl" -lt 1 ]]; then
         genLibBl=1;
         libPathStr="$libPathStr\ngenLib=.."$slashSC"genLib";
      fi
   # Else If: genMath library

   #++++++++++++++++++++++++++++++++++++++++++++++++++++++
   # Sec04 Sub02 Cat06:
   #   - shellSort
   #++++++++++++++++++++++++++++++++++++++++++++++++++++++

   elif [[ "$libStr" == "shellSort" ]]; then
   # Else If: shellSort library
      if [[ "$shellSortBl" == "" ]]; then
         cmdStr="$cmdStr$newLineStr$shellSortStr";
         objFilesStr="$objFilesStr \\\\\n$spaceStr";
         objFilesStr=""$objFilesStr""$shellSortObj"";

         if [[ "$mainLibBl" == "" ]]; then
            mainCmdStr="$mainCmdStr $shellSortObj";
         fi

         shellSortBl=1;

         depAry+=(${shellSortDep[*]});
         lenDep=$((lenDep + lenShellSortDep));
      fi

      if [[ "$genLibBl" -lt 1 ]]; then
         genLibBl=1;
         libPathStr="$libPathStr\ngenLib=.."$slashSC"genLib";
      fi
   # Else If: shellSort library

   #++++++++++++++++++++++++++++++++++++++++++++++++++++++
   # Sec04 Sub02 Cat07:
   #   - rmBlocks
   #++++++++++++++++++++++++++++++++++++++++++++++++++++++

   elif [[ "$libStr" == "rmBlocks" ]]; then
   # Else If: rmBlocks library
      if [[ "$rmBlocksBl" == "" ]]; then
         cmdStr="$cmdStr$newLineStr$rmBlocksStr";
         objFilesStr="$objFilesStr \\\\\n$spaceStr";
         objFilesStr=""$objFilesStr""$rmBlocksObj"";

         if [[ "$mainLibBl" == "" ]]; then
            mainCmdStr="$mainCmdStr $rmBlocksObj";
         fi

         rmBlocksBl=1;

         depAry+=(${rmBlocksDep[*]});
         lenDep=$((lenDep + lenRmBlocksDep));
      fi

      if [[ "$genLibBl" -lt 1 ]]; then
         genLibBl=1;
         libPathStr="$libPathStr\ngenLib=.."$slashSC"genLib";
      fi
   # Else If: rmBlocks library

   #++++++++++++++++++++++++++++++++++++++++++++++++++++++
   # Sec04 Sub02 Cat08:
   #   - strAry
   #++++++++++++++++++++++++++++++++++++++++++++++++++++++

   elif [[ "$libStr" == "strAry" ]]; then
   # Else If: strAry library
      if [[ "$strAryBl" == "" ]]; then
         cmdStr="$cmdStr$newLineStr$strAryStr";
         objFilesStr="$objFilesStr \\\\\n$spaceStr";
         objFilesStr=""$objFilesStr""$strAryObj"";

         if [[ "$mainLibBl" == "" ]]; then
            mainCmdStr="$mainCmdStr $strAryObj";
         fi

         strAryBl=1;

         depAry+=(${strAryDep[*]});
         lenDep=$((lenDep + lenStrAryDep));
      fi

      if [[ "$genLibBl" -lt 1 ]]; then
         genLibBl=1;
         libPathStr="$libPathStr\ngenLib=.."$slashSC"genLib";
      fi
   # Else If: strAry library

   #++++++++++++++++++++++++++++++++++++++++++++++++++++++
   # Sec04 Sub02 Cat09:
   #   - ptrAry
   #++++++++++++++++++++++++++++++++++++++++++++++++++++++

   elif [[ "$libStr" == "ptrAry" ]]; then
   # Else If: ptrAry library
      if [[ "$ptrAryBl" == "" ]]; then
         cmdStr="$cmdStr$newLineStr$ptrAryStr";
         objFilesStr="$objFilesStr \\\\\n$spaceStr";
         objFilesStr=""$objFilesStr""$ptrAryObj"";

         if [[ "$mainLibBl" == "" ]]; then
            mainCmdStr="$mainCmdStr $ptrAryObj";
         fi

         ptrAryBl=1;

         depAry+=(${ptrAryDep[*]});
         lenDep=$((lenDep + lenPtrAryDep));
      fi

      if [[ "$genLibBl" -lt 1 ]]; then
         genLibBl=1;
         libPathStr="$libPathStr\ngenLib=.."$slashSC"genLib";
      fi
   # Else If: ptrAry library

   #******************************************************
   # Sec04 Sub03:
   #   - genBio libraries
   #   o sec04 sub03 cat01:
   #     - codonTbl
   #   o sec04 sub03 cat02:
   #     - seqST
   #   o sec04 sub03 cat03:
   #     - kmerCnt
   #   o sec04 sub03 cat04:
   #     - geneCoord
   #   o sec04 sub03 cat05:
   #     - samEntry
   #   o sec04 sub03 cat06:
   #     - adjCoords
   #   o sec04 sub03 cat07:
   #     - cigToEqx
   #   o sec04 sub03 cat08:
   #     - maskPrim
   #   o sec04 sub03 cat09:
   #     - trimSam
   #   o sec04 sub03 cat10:
   #     - ampDepth
   #   o sec04 sub03 cat11:
   #     - tbCon
   #   o sec04 sub03 cat12:
   #     - edDist
   #******************************************************

   #++++++++++++++++++++++++++++++++++++++++++++++++++++++
   # Sec04 Sub03 Cat01:
   #   - codonTbl
   #++++++++++++++++++++++++++++++++++++++++++++++++++++++

   elif [[ "$libStr" == "codonTbl" ]]; then
   # Else If: codonTbl library
      if [[ "$codonTblBl" == "" ]]; then
         cmdStr="$cmdStr$newLineStr$codonTblStr";
         objFilesStr="$objFilesStr \\\\\n$spaceStr";
         objFilesStr=""$objFilesStr""$codonTblObj"";

         if [[ "$mainLibBl" == "" ]]; then
            mainCmdStr="$mainCmdStr $codonTblObj";
         fi

         codonTblBl=1;

         depAry+=(${codonTblDep[*]});
         lenDep=$((lenDep + lenCodonTblDep));
      fi

      if [[ "$genBioBl" -lt 1 ]]; then
         genBioBl=1;
         libPathStr="$libPathStr\ngenBio=.."$slashSC"genBio";
      fi
   # Else If: codonTbl library

   #++++++++++++++++++++++++++++++++++++++++++++++++++++++
   # Sec04 Sub03 Cat02:
   #   - seqST
   #++++++++++++++++++++++++++++++++++++++++++++++++++++++

   elif [[ "$libStr" == "seqST" ]]; then
   # Else If: seqST library
      if [[ "$seqSTBl" == "" ]]; then
         cmdStr="$cmdStr$newLineStr$seqSTStr";
         objFilesStr="$objFilesStr \\\\\n$spaceStr";
         objFilesStr=""$objFilesStr""$seqSTObj"";

         if [[ "$mainLibBl" == "" ]]; then
            mainCmdStr="$mainCmdStr $seqSTObj";
         fi

         seqSTBl=1;

         depAry+=(${seqSTDep[*]});
         lenDep=$((lenDep + lenSeqSTDep));
      fi

      if [[ "$genBioBl" -lt 1 ]]; then
         genBioBl=1;
         libPathStr="$libPathStr\ngenBio=.."$slashSC"genBio";
      fi
   # Else If: seqST library

   #++++++++++++++++++++++++++++++++++++++++++++++++++++++
   # Sec04 Sub03 Cat03:
   #   - kmerCnt
   #++++++++++++++++++++++++++++++++++++++++++++++++++++++

   elif [[ "$libStr" == "kmerCnt" ]]; then
   # Else If: kmerCnt library
      if [[ "$kmerCntBl" == "" ]]; then
         cmdStr="$cmdStr$newLineStr$kmerCntStr";
         objFilesStr="$objFilesStr \\\\\n$spaceStr";
         objFilesStr=""$objFilesStr""$kmerCntObj"";

         if [[ "$mainLibBl" == "" ]]; then
            mainCmdStr="$mainCmdStr $kmerCntObj";
         fi

         kmerCntBl=1;

         depAry+=(${kmerCntDep[*]});
         lenDep=$((lenDep + lenKmerCntDep));
      fi

      if [[ "$genBioBl" -lt 1 ]]; then
         genBioBl=1;
         libPathStr="$libPathStr\ngenBio=.."$slashSC"genBio";
      fi
   # Else If: kmerCnt library

   #++++++++++++++++++++++++++++++++++++++++++++++++++++++
   # Sec04 Sub03 Cat04:
   #   - geneCoord
   #++++++++++++++++++++++++++++++++++++++++++++++++++++++

   elif [[ "$libStr" == "geneCoord" ]]; then
   # Else If: geneCoord library
      if [[ "$geneCoordBl" == "" ]]; then
         cmdStr="$cmdStr$newLineStr$geneCoordStr";
         objFilesStr="$objFilesStr \\\\\n$spaceStr";
         objFilesStr=""$objFilesStr""$geneCoordObj"";

         geneCoordBl=1;

         depAry+=(${geneCoordDep[*]});
         lenDep=$((lenDep + lenGeneCoordDep));
      fi

      if [[ "$genBioBl" -lt 1 ]]; then
         genBioBl=1;
         libPathStr="$libPathStr\ngenBio=.."$slashSC"genBio";
      fi
   # Else If: geneCoord library

   #++++++++++++++++++++++++++++++++++++++++++++++++++++++
   # Sec04 Sub03 Cat05:
   #   - samEntry
   #++++++++++++++++++++++++++++++++++++++++++++++++++++++

   elif [[ "$libStr" == "samEntry" ]]; then
   # Else If: samEntry library
      if [[ "$samEntryBl" == "" ]]; then
         cmdStr="$cmdStr$newLineStr$samEntryStr";
         objFilesStr="$objFilesStr \\\\\n$spaceStr";
         objFilesStr=""$objFilesStr""$samEntryObj"";

         if [[ "$mainLibBl" == "" ]]; then
            mainCmdStr="$mainCmdStr $samEntryObj";
         fi

         samEntryBl=1;

         depAry+=(${samEntryDep[*]});
         lenDep=$((lenDep + lenSamEntryDep));
      fi

      if [[ "$genBioBl" -lt 1 ]]; then
         genBioBl=1;
         libPathStr="$libPathStr\ngenBio=.."$slashSC"genBio";
      fi
   # Else If: samEntry library

   #++++++++++++++++++++++++++++++++++++++++++++++++++++++
   # Sec04 Sub03 Cat06:
   #   - adjCoords
   #++++++++++++++++++++++++++++++++++++++++++++++++++++++

   elif [[ "$libStr" == "adjCoords" ]]; then
   # Else If: adjCoords library
      if [[ "$adjCoordsBl" == "" ]]; then
         cmdStr="$cmdStr$newLineStr$adjCoordsStr";
         objFilesStr="$objFilesStr \\\\\n$spaceStr";
         objFilesStr=""$objFilesStr""$adjCoordsObj"";

         if [[ "$mainLibBl" == "" ]]; then
            mainCmdStr="$mainCmdStr $adjCoordsObj";
         fi

         adjCoordsBl=1;

         depAry+=(${adjCoordsDep[*]});
         lenDep=$((lenDep + lenAdjCoordsDep));
      fi

      if [[ "$genBioBl" -lt 1 ]]; then
         genBioBl=1;
         libPathStr="$libPathStr\ngenBio=.."$slashSC"genBio";
      fi
   # Else If: adjCoords library

   #++++++++++++++++++++++++++++++++++++++++++++++++++++++
   # Sec04 Sub03 Cat07:
   #   - cigToEqx
   #++++++++++++++++++++++++++++++++++++++++++++++++++++++

   elif [[ "$libStr" == "cigToEqx" ]]; then
   # Else If: cigToEqx library
      if [[ "$cigToEqxBl" == "" ]]; then
         cmdStr="$cmdStr$newLineStr$cigToEqxStr";
         objFilesStr="$objFilesStr \\\\\n$spaceStr";
         objFilesStr=""$objFilesStr""$cigToEqxObj"";

         if [[ "$mainLibBl" == "" ]]; then
            mainCmdStr="$mainCmdStr $cigToEqxObj";
         fi

         cigToEqxBl=1;

         depAry+=(${cigToEqxDep[*]});
         lenDep=$((lenDep + lenCigToEqxDep));
      fi

      if [[ "$genBioBl" -lt 1 ]]; then
         genBioBl=1;
         libPathStr="$libPathStr\ngenBio=.."$slashSC"genBio";
      fi
   # Else If: cigToEqx library

   #++++++++++++++++++++++++++++++++++++++++++++++++++++++
   # Sec04 Sub03 Cat08:
   #   - maskPrim
   #++++++++++++++++++++++++++++++++++++++++++++++++++++++

   elif [[ "$libStr" == "maskPrim" ]]; then
   # Else If: maskPrim library
      if [[ "$maskPrimBl" == "" ]]; then
         cmdStr="$cmdStr$newLineStr$maskPrimStr";
         objFilesStr="$objFilesStr \\\\\n$spaceStr";
         objFilesStr=""$objFilesStr""$maskPrimObj"";

         if [[ "$mainLibBl" == "" ]]; then
            mainCmdStr="$mainCmdStr $maskPrimObj";
         fi

         maskPrimBl=1;

         depAry+=(${maskPrimDep[*]});
         lenDep=$((lenDep + lenMaskPrimDep));
      fi

      if [[ "$genBioBl" -lt 1 ]]; then
         genBioBl=1;
         libPathStr="$libPathStr\ngenBio=.."$slashSC"genBio";
      fi
   # Else If: maskPrim library

   #++++++++++++++++++++++++++++++++++++++++++++++++++++++
   # Sec04 Sub03 Cat09:
   #   - trimSam
   #++++++++++++++++++++++++++++++++++++++++++++++++++++++

   elif [[ "$libStr" == "trimSam" ]]; then
   # Else If: trimSam library
      if [[ "$trimSamBl" == "" ]]; then
         cmdStr="$cmdStr$newLineStr$trimSamStr";
         objFilesStr="$objFilesStr \\\\\n$spaceStr";
         objFilesStr=""$objFilesStr""$trimSamObj"";

         if [[ "$mainLibBl" == "" ]]; then
            mainCmdStr="$mainCmdStr $trimSamObj";
         fi

         trimSamBl=1;

         depAry+=(${trimSamDep[*]});
         lenDep=$((lenDep + lenTrimSamDep));
      fi

      if [[ "$genBioBl" -lt 1 ]]; then
         genBioBl=1;
         libPathStr="$libPathStr\ngenBio=.."$slashSC"genBio";
      fi
   # Else If: trimSam library

   #++++++++++++++++++++++++++++++++++++++++++++++++++++++
   # Sec04 Sub03 Cat10:
   #   - ampDepth
   #++++++++++++++++++++++++++++++++++++++++++++++++++++++

   elif [[ "$libStr" == "ampDepth" ]]; then
   # If: ampDepth library
      if [[ "$ampDepthBl" == "" ]]; then
         cmdStr="$cmdStr$newLineStr$ampDepthStr";
         objFilesStr="$objFilesStr \\\\\n$spaceStr";
         objFilesStr=""$objFilesStr""$ampDepthObj"";

         if [[ "$mainLibBl" == "" ]]; then
            mainCmdStr="$mainCmdStr $ampDepthObj";
         fi

         ampDepthBl=1;

         depAry+=(${ampDepthDep[*]});
         lenDep=$((lenDep + lenAmpDepthDep));
      fi

      if [[ "$genBioBl" -lt 1 ]]; then
         genBioBl=1;
         libPathStr="$libPathStr\ngenBio=.."$slashSC"genBio";
      fi
   # If: ampDepth library

   #++++++++++++++++++++++++++++++++++++++++++++++++++++++
   # Sec04 Sub03 Cat11:
   #   - tbCon
   #++++++++++++++++++++++++++++++++++++++++++++++++++++++

   elif [[ "$libStr" == "tbCon" ]]; then
   # If: tbCon library
      if [[ "$tbConBl" == "" ]]; then
         cmdStr="$cmdStr$newLineStr$tbConStr";
         objFilesStr="$objFilesStr \\\\\n$spaceStr";
         objFilesStr=""$objFilesStr""$tbConObj"";

         if [[ "$mainLibBl" == "" ]]; then
            mainCmdStr="$mainCmdStr $tbConObj";
         fi

         tbConBl=1;

         depAry+=(${tbConDep[*]});
         lenDep=$((lenDep + lenTbConDep));
      fi

      if [[ "$genBioBl" -lt 1 ]]; then
         genBioBl=1;
         libPathStr="$libPathStr\ngenBio=.."$slashSC"genBio";
      fi
   # If: tbCon library

   #++++++++++++++++++++++++++++++++++++++++++++++++++++++
   # Sec04 Sub03 Cat12:
   #   - edDist
   #++++++++++++++++++++++++++++++++++++++++++++++++++++++

   elif [[ "$libStr" == "edDist" ]]; then
   # If: edDist library
      if [[ "$edDistBl" == "" ]]; then
         cmdStr="$cmdStr$newLineStr$edDistStr";
         objFilesStr="$objFilesStr \\\\\n$spaceStr";
         objFilesStr=""$objFilesStr""$edDistObj"";

         if [[ "$mainLibBl" == "" ]]; then
            mainCmdStr="$mainCmdStr $edDistObj";
         fi

         edDistBl=1;

         depAry+=(${edDistDep[*]});
         lenDep=$((lenDep + lenEdDistDep));
      fi

      if [[ "$genBioBl" -lt 1 ]]; then
         genBioBl=1;
         libPathStr="$libPathStr\ngenBio=.."$slashSC"genBio";
      fi
   # If: edDist library

   #******************************************************
   # Sec04 Sub04:
   #   - genAln libraries
   #   o sec04 sub04 cat01:
   #     - indexToCoord
   #   o sec04 sub04 cat02:
   #     - alnSet
   #   o sec04 sub04 cat03:
   #     - dirMatrix
   #   o sec04 sub04 cat04:
   #     - water
   #   o sec04 sub04 cat05:
   #     - needle
   #   o sec04 sub04 cat06:
   #     - memwater
   #   o sec04 sub04 cat07:
   #     - kmerFind
   #   o sec04 sub04 cat08:
   #     - samToAln
   #******************************************************

   #++++++++++++++++++++++++++++++++++++++++++++++++++++++
   # Sec04 Sub04 Cat01:
   #   - indexToCoord
   #++++++++++++++++++++++++++++++++++++++++++++++++++++++

   elif [[ "$libStr" == "indexToCoord" ]]; then
   # Else If: indexToCoord library
      if [[ "$indexToCoordBl" == "" ]]; then
        cmdStr="$cmdStr$newLineStr$indexToCoordStr";
        objFilesStr="$objFilesStr \\\\\n$spaceStr";
        objFilesStr=""$objFilesStr""$indexToCoordObj"";

        if [[ "$mainLibBl" == "" ]]; then
           mainCmdStr="$mainCmdStr $indexToCoordObj";
        fi

        indexToCoordBl=1;

        depAry+=(${indexToCoord[*]});
        lenDep=$((lenDep + lenIndexToCoordDep));
      fi

      if [[ "$genAlnBl" -lt 1 ]]; then
         genAlnBl=1;
         libPathStr="$libPathStr\ngenAln=.."$slashSC"genAln";
      fi
   # Else If: indexToCoord library

   #++++++++++++++++++++++++++++++++++++++++++++++++++++++
   # Sec04 Sub04 Cat02:
   #   - alnSet
   #++++++++++++++++++++++++++++++++++++++++++++++++++++++

   elif [[ "$libStr" == "alnSet" ]]; then
   # Else If: alnSet library
      if [[ "$alnSetBl" == "" ]]; then
         cmdStr="$cmdStr$newLineStr$alnSetStr";
         objFilesStr="$objFilesStr \\\\\n$spaceStr";
         objFilesStr=""$objFilesStr""$alnSetObj"";

         if [[ "$mainLibBl" == "" ]]; then
            mainCmdStr="$mainCmdStr $alnSetObj";
         fi

         alnSetBl=1;

         depAry+=(${alnSetDep[*]});
         lenDep=$((lenDep + lenAlnSetDep));
      fi

      if [[ "$genAlnBl" -lt 1 ]]; then
         genAlnBl=1;
         libPathStr="$libPathStr\ngenAln=.."$slashSC"genAln";
      fi
   # Else If: alnSet library

   #++++++++++++++++++++++++++++++++++++++++++++++++++++++
   # Sec04 Sub04 Cat03:
   #   - dirMatrix
   #++++++++++++++++++++++++++++++++++++++++++++++++++++++

   elif [[ "$libStr" == "dirMatrix" ]]; then
   # Else If: dirMatrix library
      if [[ "$dirMatrixBl" == "" ]]; then
         cmdStr="$cmdStr$newLineStr$dirMatrixStr";
         objFilesStr="$objFilesStr \\\\\n$spaceStr";
         objFilesStr=""$objFilesStr""$dirMatrixObj"";

         if [[ "$mainLibBl" == "" ]]; then
            mainCmdStr="$mainCmdStr $dirMatrixObj";
         fi

         dirMatrixBl=1;

         depAry+=(${dirMatrixDep[*]});
         lenDep=$((lenDep + lenDirMatrixDep));
      fi

      if [[ "$genAlnBl" -lt 1 ]]; then
         genAlnBl=1;
         libPathStr="$libPathStr\ngenAln=.."$slashSC"genAln";
      fi
   # Else If: dirMatrix library

   #++++++++++++++++++++++++++++++++++++++++++++++++++++++
   # Sec04 Sub04 Cat04:
   #   - water
   #++++++++++++++++++++++++++++++++++++++++++++++++++++++

   elif [[ "$libStr" == "water" ]]; then
   # Else If: water library
      if [[ "$waterBl" == "" ]]; then
         cmdStr="$cmdStr$newLineStr$waterStr";
         objFilesStr="$objFilesStr \\\\\n$spaceStr";
         objFilesStr=""$objFilesStr""$waterObj"";

         if [[ "$mainLibBl" == "" ]]; then
            mainCmdStr="$mainCmdStr $waterObj";
         fi

         waterBl=1;

         depAry+=(${waterDep[*]});
         lenDep=$((lenDep + lenWaterDep));
      fi

      if [[ "$genAlnBl" -lt 1 ]]; then
         genAlnBl=1;
         libPathStr="$libPathStr\ngenAln=.."$slashSC"genAln";
      fi
   # Else If: water library

   #++++++++++++++++++++++++++++++++++++++++++++++++++++++
   # Sec04 Sub04 Cat05:
   #   - needle
   #++++++++++++++++++++++++++++++++++++++++++++++++++++++

   elif [[ "$libStr" == "needle" ]]; then
   # Else If: needle library
      if [[ "$needleBl" == "" ]]; then
         cmdStr="$cmdStr$newLineStr$needleStr";
         objFilesStr="$objFilesStr \\\\\n$spaceStr";
         objFilesStr=""$objFilesStr""$needleObj"";

         if [[ "$mainLibBl" == "" ]]; then
            mainCmdStr="$mainCmdStr $needleObj";
         fi

         needleBl=1;

         depAry+=(${needleDep[*]});
         lenDep=$((lenDep + lenNeedleDep));
      fi

      if [[ "$genAlnBl" -lt 1 ]]; then
         genAlnBl=1;
         libPathStr="$libPathStr\ngenAln=.."$slashSC"genAln";
      fi
   # Else If: needle library

   #++++++++++++++++++++++++++++++++++++++++++++++++++++++
   # Sec04 Sub04 Cat06:
   #   - memwater
   #++++++++++++++++++++++++++++++++++++++++++++++++++++++

   elif [[ "$libStr" == "memwater" ]]; then
   # Else If: memwater library
      if [[ "$memwaterBl" == "" ]]; then
         cmdStr="$cmdStr$newLineStr$memwaterStr";
         objFilesStr="$objFilesStr \\\\\n$spaceStr";
         objFilesStr=""$objFilesStr""$memwaterObj"";

         if [[ "$mainLibBl" == "" ]]; then
            mainCmdStr="$mainCmdStr $memwaterObj";
         fi

         memwaterBl=1;

         depAry+=(${memwaterDep[*]});
         lenDep=$((lenDep + lenMemwaterDep));
      fi

      if [[ "$genAlnBl" -lt 1 ]]; then
         genAlnBl=1;
         libPathStr="$libPathStr\ngenAln=.."$slashSC"genAln";
      fi
   # Else If: memwater library

   #++++++++++++++++++++++++++++++++++++++++++++++++++++++
   # Sec04 Sub04 Cat07:
   #   - kmerFind
   #++++++++++++++++++++++++++++++++++++++++++++++++++++++

   elif [[ "$libStr" == "kmerFind" ]]; then
   # Else If: kmerFind library
      if [[ "$kmerFindBl" == "" ]]; then
         cmdStr="$cmdStr$newLineStr$kmerFindStr";
         objFilesStr="$objFilesStr \\\\\n$spaceStr";
         objFilesStr=""$objFilesStr""$kmerFindObj"";

         if [[ "$mainLibBl" == "" ]]; then
            mainCmdStr="$mainCmdStr $kmerFindObj";
         fi

         kmerFindBl=1;

         depAry+=(${kmerFindDep[*]});
         lenDep=$((lenDep + lenKmerFindDep));
      fi

      if [[ "$genAlnBl" -lt 1 ]]; then
         genAlnBl=1;
         libPathStr="$libPathStr\ngenAln=.."$slashSC"genAln";
      fi
   # Else If: kmerFin library

   #++++++++++++++++++++++++++++++++++++++++++++++++++++++
   # Sec04 Sub04 Cat08:
   #   - samToAln
   #++++++++++++++++++++++++++++++++++++++++++++++++++++++

   elif [[ "$libStr" == "samToAln" ]]; then
   # Else If: samToAln library
      if [[ "$samToAlnBl" == "" ]]; then
         cmdStr="$cmdStr$newLineStr$samToAlnStr";
         objFilesStr="$objFilesStr \\\\\n$spaceStr";
         objFilesStr=""$objFilesStr""$samToAlnObj"";

         if [[ "$mainLibBl" == "" ]]; then
            mainCmdStr="$mainCmdStr $samToAlnObj";
         fi

         samToAlnBl=1;

         depAry+=(${samToAlnDep[*]});
         lenDep=$((lenDep + lenSamToAlnDep));
      fi

      if [[ "$genAlnBl" -lt 1 ]]; then
         genAlnBl=1;
         libPathStr="$libPathStr\ngenAln=.."$slashSC"genAln";
      fi
   # Else If: samToAln library

   #******************************************************
   # Sec04 Sub05:
   #   - genClust
   #   o sec04 sub05 cat01:
   #     - illNano
   #   o sec04 sub05 cat02:
   #     - clustST
   #   o sec04 sub05 cat03:
   #     - edClust
   #******************************************************

   #++++++++++++++++++++++++++++++++++++++++++++++++++++++
   # Sec04 Sub05 Cat01:
   #   - illNano
   #++++++++++++++++++++++++++++++++++++++++++++++++++++++

   elif [[ "$libStr" == "illNano" ]]; then
   # Else If: illNano library
      if [[ "$illNanoBl" == "" ]]; then
         cmdStr="$cmdStr$newLineStr$illNanoStr";
         objFilesStr="$objFilesStr \\\\\n$spaceStr";
         objFilesStr=""$objFilesStr""$illNanoObj"";

         if [[ "$mainLibBl" == "" ]]; then
            mainCmdStr="$mainCmdStr $illNanoObj";
         fi

         illNanoBl=1;

         depAry+=(${illNanoDep[*]});
         lenDep=$((lenDep + lenIllNanoDep));
      fi

      if [[ "$genClustBl" -lt 1 ]]; then
         genClustBl=1;
         libPathStr="$libPathStr\ngenClust=.."$slashSC"genClust";
      fi
   # Else If: illNano library

   #++++++++++++++++++++++++++++++++++++++++++++++++++++++
   # Sec04 Sub05 Cat02:
   #   - clustST
   #++++++++++++++++++++++++++++++++++++++++++++++++++++++

   elif [[ "$libStr" == "clustST" ]]; then
   # Else If: clustST library
      if [[ "$clustSTBl" == "" ]]; then
         cmdStr="$cmdStr$newLineStr$clustSTStr";
         objFilesStr="$objFilesStr \\\\\n$spaceStr";
         objFilesStr=""$objFilesStr""$clustSTObj"";

         if [[ "$mainLibBl" == "" ]]; then
            mainCmdStr="$mainCmdStr $clustSTObj";
         fi

         clustSTBl=1;

         depAry+=(${clustSTDep[*]});
         lenDep=$((lenDep + lenClustSTDep));
      fi

      if [[ "$genClustBl" -lt 1 ]]; then
         genClustBl=1;
         libPathStr="$libPathStr\ngenClust=.."$slashSC"genClust";
      fi
   # Else If: clustST library

   #++++++++++++++++++++++++++++++++++++++++++++++++++++++
   # Sec04 Sub05 Cat03:
   #   - edClust
   #++++++++++++++++++++++++++++++++++++++++++++++++++++++

   elif [[ "$libStr" == "edClust" ]]; then
   # Else If: edClust library
      if [[ "$edClustBl" == "" ]]; then
         cmdStr="$cmdStr$newLineStr$edClustStr";
         objFilesStr="$objFilesStr \\\\\n$spaceStr";
         objFilesStr=""$objFilesStr""$edClustObj"";

         if [[ "$mainLibBl" == "" ]]; then
            mainCmdStr="$mainCmdStr $edClustObj";
         fi

         edClustBl=1;

         depAry+=(${edClustDep[*]});
         lenDep=$((lenDep + lenEdClustDep));
      fi

      if [[ "$genClustBl" -lt 1 ]]; then
         genClustBl=1;
         libPathStr="$libPathStr\ngenClust=.."$slashSC"genClust";
      fi
   # Else If: edClust library

   #******************************************************
   # Sec04 Sub06:
   #   - non-general library files
   #   o sec04 sub06 cat01:
   #     - k2TaxaId
   #******************************************************

   #++++++++++++++++++++++++++++++++++++++++++++++++++++++
   # Sec04 Sub06 Cat01:
   #   - k2TaxaId
   #++++++++++++++++++++++++++++++++++++++++++++++++++++++

   elif [[ "$libStr" == "k2TaxaId" ]]; then
   # Else If: k2TaxaId program
      if [[ "$k2TaxaIdBl" == "" ]]; then
         cmdStr="$cmdStr$newLineStr$k2TaxaIdStr";
         objFilesStr="$objFilesStr \\\\\n$spaceStr";
         objFilesStr=""$objFilesStr""$k2TaxaIdObj"";

         if [[ "$mainLibBl" == "" ]]; then
            mainCmdStr="$mainCmdStr $k2TaxaIdObj";
         fi

         k2TaxaIdBl=1;

         depAry+=(${k2TaxaIdDep[*]});
         lenDep=$((lenDep + lenK2TaxaIdDep));
      fi
   # Else If: k2TaxaId program


   fi # check librarys called

   #******************************************************
   # Sec04 Sub07:
   #   - move to next library or dependency
   #******************************************************

   if [[ "$depSI" -ge "$lenDep" ]]; then
   # If: no other dependencies
      shift;
      libStr="$1"
      mainLibBl="";
   # If: no other dependencies

   else
   # Else: on library dependency
      libStr="";
      libStr="${depAry[depSI]}";
      depSI="$((depSI + 1))";
      mainLibBl=1;

      # in some cases my count will be off
      while [[ "$libStr" == "" && "$depSI" -lt "$lenDep" ]]; do
      # Loop: move past emtpy libs
         libStr="${depAry[depSI]}";
         depSI="$((depSI + 1))";
      done # Loop: move past empty libs

      # make sure no blank endings
      if [[ "$depSI" -ge "$lenDep" ]]; then
      # If: no other dependencies
         shift;
         libStr="$1"
         mainLibBl="";
      fi # If: no other dependencies
   # Else: on library dependency

   fi # check if library has dependencies
done

#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# Sec05:
#   - print mkfile (makefile)
#<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

# need to add in main file to object files list
objFilesStr="$objFilesStr \\\\\n$spaceStr$mainFileStr.\$O";

# need to complete compiler part of main file command
mainCmdStr="$mainCmdStr
	$ccStr "$dashOStr"$mainFileStr.\$O "$dashCStr" $cflagsStr $mainFileStr.c"

{
   printf "%b\n" "$headStr"
   printf "%b\n\n" "$libPathStr"
   printf "%b\n\n" "$objFilesStr"
   printf "%b\n\n" "$linkStr"
   printf "%b" "$mainCmdStr" # cmdStr has newlines
   printf "%b\n\n" "$cmdStr"
   printf "%b\n\n" "$installStr"
   printf "%b\n" "$cleanStr"
} # print out make file
