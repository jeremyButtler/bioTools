#!/usr/bin/sh

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
genGenoTypeStr="";
#seqByIdStr=""; add in for k2 taxa

# marks if library location was used (bioTools)
genLibBl=0;
genBioBl=0;
genAlnBl=0;
genClustBl=0;
genGenoTypeBl=0;

# unique libraryes for freezeTB
genFreezeTB="";
genFreezeTBBl="";

# for building compile calls
coreFlagsStr=""; # CFLAGS
ccStr="";     # CC
dashOStr="";  # -o
spaceStr="   ";  # spacing for objFiles entry
newLineStr="";# holds new line to add between commands
warnStr="-Wall -Wextra -Wpedantic"; # warnings

# for printing out make file
headStr="";     # first part of file
libPathStr="";  # library paths
linkStr="";     # link (all:) command
objFilesStr=""; # object files for copiling (objFiles)
mainCmdStr="";  # commat to build main .c file
cleanStr="";    # command to clean up object files
installStr="";  # install command

osStr="$1";       # OS/mkfile type
nameStr="$2";       # program name
mainFileStr="$3"; # main file input by user


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
   - libraries or .h files:
     o libraries to include
     o do not use .c extensions
     o use .h extension to specify .h file
     o the script knows the dependencys for each library,
       so you only need to put the top level dependency,
       however, it is ok if you wish to include all
       libraries
       - for example; samEntry would also print entries
         for strAry, ulCp, base10str, and numToStr
     o Options:
       - base10str (genLib)
       - charCp (genLib)
       - checkSum (genLib)
       - endin (genLib)
       - genMath (genLib)
       - inflate (genLib)
       - numToStr (genLib)
       - ptrAry (genLib)
       - shellSort (genLib)
       - strAry (genLib)
       - ulCp (genLib)

       - adjCoords (genBio)
       - ampDepth (genBio)
       - cigToEqx (genBio)
       - codoTble (genBio)
       - edDist (genBio)
       - geneCoord (genBio)
       - kmerCnt (genBio)
       - maskPrim (genBio)
       - rmHomo (genBio)
       - samEntry (genBio)
       - seqST (genBio)
       - tbCon (genBio)
       - trimSam (genBio)

       - alnSet (genAln)
       - dirMatrix (genAln)
       - indexToCoord (genAln)
       - kmerFind (genAln)
       - mapRead (genAln)
       - memwater (genAln)
       - needle (genAln)
       - samToAln (genAln)
       - water (genAln)

       - illNano (genClust)
       - clustST (genClust)
       - edClust (genClust)
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

if [ "$1" = "" ]; then
   printf "%s\n" "$helpStr";
   exit;
elif [ "$1" = "-h" ]; then
   printf "%s\n" "$helpStr";
   exit;
elif [ "$1" = "--h" ]; then
   printf "%s\n" "$helpStr";
   exit;
elif [ "$2" = "" ]; then
   printf "%s\n" "$helpStr";
   exit;
elif [ "$2" = "-h" ]; then
   printf "%s\n" "$helpStr";
   exit;
elif [ "$2" = "--h" ]; then
   printf "%s\n" "$helpStr";
   exit;
elif [ "$3" = "" ]; then
   printf "%s\n" "$helpStr";
   exit;
elif [ "$3" = "-h" ]; then
   printf "%s\n" "$helpStr";
   exit;
elif [ "$3" = "--h" ]; then
   printf "%s\n" "$helpStr";
   exit;
fi

# add in some warnings I have seen others use. One or two
#   are left out because they often send warnings about
#   things I do (such as pointer casting used in ulCp)
warnStr="$warnStr -Wundef";
warnStr="$warnStr -Waggregate-return -Wwrite-strings";
warnStr="$warnStr -Wstrict-prototypes";
warnStr="$warnStr -Wpointer-arith -Wshadow"
warnStr="$warnStr -Wunreachable-code -Winit-self"
   # -Wfloat-eqaul messes up numToStr. I am comparing
   #   floats to see if there is a decimal, so float == 0
   #   is safe
   # -Wcast-align warns about ulCp on Mac's clang. I know
   #   it works on Mac so removing

#*********************************************************
# Sec02 Sub01:
#   - debuging mkfiles
#*********************************************************

if [ "$osStr" = "bug" ];
then # If: debugging make file
   headStr="$headStr\nCC=cc";
   headStr="LD=cc";
   headStr="$headStr\ncoreCFLAGS= -O0 -std=c89 -g";
   headStr="$headStr -Werror $warnStr -c";
   headStr="$headStr\nCFLAGS=-DNONE";
   headStr="$headStr\nNAME=$nameStr";
   headStr="$headStr\nPREFIX=/usr/local/bin";
   headStr="$headStr\nO=o.bug";

   genLibStr="\$(genLib)/";
   genBioStr="\$(genBio)/";
   genAlnStr="\$(genAln)/";
   genClustStr="\$(genClust)/";
   genGenoTypeStr="\$(genGenoType)/";
   #seqByIdStr="\$(seqByIdSrc)/"; # adad in for k2Taxa

   coreFlagsStr="\$(coreCFLAGS)";
   cFlagsStr="\$(CFLAGS)";
   ccStr="\$(CC)";
   dashOStr="-o ";

   installStr="";

   cleanStr="clean:"
   cleanStr="$cleanStr\n\trm \$(objFiles)";

   linkStr="\$(NAME): $mainFileStr.\$O";
   linkStr="$linkStr\n\t\$(LD) ${dashOStr}\$(NAME)";
   linkStr="$linkStr \$(objFiles)";

   slashSC="/";
# If: debugging make file

#*********************************************************
# Sec02 Sub02:
#   - unix mkfiles
#*********************************************************

elif [ "$osStr" = "unix" ];
then # Else If: general unix make file
   headStr="$headStr\nCC=cc";
   headStr="LD=cc";
   headStr="$headStr\ncoreCFLAGS= -O3 -std=c89 $warnStr";
   headStr=" -c";
   headStr="$headStr\nCFLAGS=-DNONE";
   headStr="$headStr\nNAME=$nameStr";
   headStr="$headStr\nPREFIX=/usr/local/bin";
   headStr="$headStr\nO=o.unix";

   genLibStr="\$(genLib)/";
   genBioStr="\$(genBio)/";
   genAlnStr="\$(genAln)/";
   genClustStr="\$(genClust)/";
   genGenoTypeStr="\$(genGenoType)/";
   #seqByIdStr="\$(seqByIdSrc)/"; # add in for k2Taxa

   coreFlagsStr="\$(coreCFLAGS)";
   cFlagsStr="\$(CFLAGS)";
   ccStr="\$(CC)";
   dashOStr="-o ";

   installStr="install:"
   installStr="$installStr\n\tmv \$(NAME) \$(PREFIX)"
   installStr="$installStr\n\tchmod a+x \$(PREFIX)"
   installStr="$installStr\/\$(NAME)"

   cleanStr="clean:"
   cleanStr="$cleanStr\n\trm \$(objFiles)";

   linkStr="\$(NAME): $mainFileStr.\$O";
   linkStr="$linkStr\n\t\$(LD) ${dashOStr}\$(NAME)";
   linkStr="$linkStr \$(objFiles)";

   slashSC="/";
# Else If: general unix make file

#*********************************************************
# Sec02 Sub03:
#   - static (unix) mkfiles
#*********************************************************

elif [ "$osStr" = "static" ];
then # Else If: static unix make file
   headStr="$headStr\nCC=cc";
   headStr="LD=cc";
   headStr="$headStr\ncoreCFLAGS= -O3 -std=c89 -static";
   headStr="$headStr $warnStr -c";
   headStr="$headStr\nCFLAGS=-DNONE";
   headStr="$headStr\nNAME=$nameStr";
   headStr="$headStr\nPREFIX=/usr/local/bin";
   headStr="$headStr\nO=o.static";

   genLibStr="\$(genLib)/";
   genBioStr="\$(genBio)/";
   genAlnStr="\$(genAln)/";
   genClustStr="\$(genClust)/";
   genGenoTypeStr="\$(genGenoType)/";
   #seqByIdStr="\$(seqByIdSrc)/"; add in for k2 taxa

   coreFlagsStr="\$(coreCFLAGS)";
   cFlagsStr="\$(CFLAGS)";
   ccStr="\$(CC)";
   dashOStr="-o ";

   installStr="install:"
   installStr="$installStr\n\tmv \$(NAME) \$(PREFIX)"
   installStr="$installStr\n\tchmod a+x \$(PREFIX)/\$(NAME)"

   cleanStr="clean:"
   cleanStr="$cleanStr\n\trm \$(objFiles)";

   linkStr="\$(NAME): $mainFileStr.\$O";
   linkStr="$linkStr\n\t\$(LD) ${dashOStr}\$(NAME)";
   linkStr="$linkStr \$(objFiles)";

   slashSC="/";
# Else If: static unix make file

#*********************************************************
# Sec02 Sub04:
#   - windows mkfiles
#*********************************************************

elif [ "$osStr" = "win" ];
then # Else If: windows make file
   headStr="$headStr\nCC=cl.exe";
   headStr="$headStr\nLD=link.exe";
   headStr="$headStr\ncoreCFLAGS= /c /O2 /Ot /Za /Tc";
   headStr="$headStr\nCFLAGS=/DNONE";
   headStr="$headStr\nNAME=$nameStr.exe";
   headStr="$headStr\nPREFIX=\"%localAppData%\"";
   headStr="$headStr\nO=o.win";

   genLibStr="\$(genLib)\\\\";
   genBioStr="\$(genBio)\\\\";
   genAlnStr="\$(genAln)\\\\";
   genClustStr="\$(genClust)\\\\";
   genGenoTypeStr="\$(genGenoType)\\\\";
   #seqByIdStr="\$(seqByIdSrc)\\\\"; # add in for k2Taxa

   coreFlagsStr="\$(coreCFLAGS)";
   cFlagsStr="\$(CFLAGS)";
   ccStr="\$(CC)";
   dashOStr="/Fo:";

   installStr="install:"
   installStr="$installStr\n\tmove \$(NAME) \$(PREFIX)"

   cleanStr="clean:"
   cleanStr="$cleanStr\n\tdel \$(objFiles)";

   linkStr="\$(NAME): $mainFileStr.\$O";
   linkStr="$linkStr\n\t\$(LD) /out:\$(NAME)";
   linkStr="$linkStr \$(objFiles)";

   slashSC="\\";
# Else If: windows make file

#*********************************************************
# Sec02 Sub05:
#   - plan9 mkfiles
#*********************************************************

elif [ "$osStr" = "plan9" ];
then # Else If: plan9 make file
   headStr="</\$objtype/mkfile";
   headStr="$headStr\ncoreCFLAGS=-DPLAN9";
   headStr="$headStr\nCFLAGS=-DNONE";
   headStr="$headStr\nNAME=$nameStr";
   headStr="$headStr\nPREFIX=\$home/bin";

   genLibStr="\$genLib/";
   genBioStr="\$genBio/";
   genAlnStr="\$genAln/";
   genClustStr="\$genClust/";
   genGenoTypeStr="\$(genGenoType)/";
   #seqByIdStr="\$seqByIdSrc/"; # add in when in k2Taxa

   coreFlagsStr="\$coreCFLAGS";
   cFlagsStr="\$CFLAGS";
   ccStr="\$CC";
   dashOStr="-o ";

   installStr="install:"
   installStr="$installStr\n\tmv \$(NAME) \$(PREFIX)"
   installStr="$installStr\n\tchmod a+x \$(PREFIX)/\$(NAME)"

   cleanStr="clean:"
   cleanStr="$cleanStr\n\trm \$(objFiles)";

   linkStr="\$(NAME): $mainFileStr.\$O";
   linkStr="$linkStr\n\t\$LD ${dashOStr}\$NAME";
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
#     - genGenoType libraries
#   o sec03 sub07:
#     - misc code in own folders
#<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

#*********************************************************
# Sec03 Sub01:
#   - setup start of main file build command
#*********************************************************

mainCmdStr="$mainFileStr.\$O: \\
	$mainFileStr.c";

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
#     - endin
#   o sec03 sub02 cat08:
#     - checkSum
#   o sec03 sub02 cat09:
#     - strAry
#   o sec03 sub02 cat10:
#     - ptrAry
#   o sec03 sub02 cat11:
#     - fileFun
#   o sec03 sub02 cat12:
#     - inflate
#*********************************************************

#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# Sec03 Sub02 Cat01:
#   - numToStr
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++

numToStr="${genLibStr}numToStr.\$O: \\
	${genLibStr}numToStr.c \\
	${genLibStr}numToStr.h
		$ccStr ${dashOStr}${genLibStr}numToStr.\$O \\
			$cFlagsStr $coreFlagsStr \\
			${genLibStr}numToStr.c"

numToStrObj="${genLibStr}numToStr.\$O";
numToStrDep="";

#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# Sec03 Sub02 Cat02:
#   - base10str
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++

base10Str="${genLibStr}base10str.\$O: \\
	${genLibStr}base10str.c \\
	${genLibStr}base10str.h
		$ccStr ${dashOStr}${genLibStr}base10str.\$O \\
			$cFlagsStr $coreFlagsStr \\
			${genLibStr}base10str.c"

base10strObj="${genLibStr}base10str.\$O";
base10strDep="";

#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# Sec03 Sub02 Cat03:
#   - ulCp
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++

ulCpStr="${genLibStr}ulCp.\$O: \\
	${genLibStr}ulCp.c \\
	${genLibStr}ulCp.h
		$ccStr ${dashOStr}${genLibStr}ulCp.\$O \\
			$cFlagsStr $coreFlagsStr \\
			${genLibStr}ulCp.c"

ulCpObj="${genLibStr}ulCp.\$O";
ulCpDep="";

#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# Sec03 Sub02 Cat04:
#   - charCp
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++

charCpStr="${genLibStr}charCp.\$O: \\
	${genLibStr}charCp.c \\
	${genLibStr}charCp.h
		$ccStr ${dashOStr}${genLibStr}charCp.\$O \\
			$cFlagsStr $coreFlagsStr \\
			${genLibStr}charCp.c"

charCpObj="${genLibStr}charCp.\$O";
charCpDep="";

#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# Sec03 Sub02 Cat05:
#   - genMath
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++

genMathStr="${genLibStr}genMath.\$O: \\
	${genLibStr}genMath.c \\
	${genLibStr}genMath.h
		$ccStr ${dashOStr}${genLibStr}genMath.\$O \\
			$cFlagsStr $coreFlagsStr \\
			${genLibStr}genMath.c"

genMathObj="${genLibStr}genMath.\$O";
genMathDep="";

#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# Sec03 Sub02 Cat06:
#   - shellSort
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++

shellSortStr="${genLibStr}shellSort.\$O: \\
	${genLibStr}shellSort.c \\
	${genLibStr}shellSort.h
		$ccStr ${dashOStr}${genLibStr}shellSort.\$O \\
			$cFlagsStr $coreFlagsStr \\
			${genLibStr}shellSort.c"

shellSortObj="${genLibStr}shellSort.\$O";
shellSortDep="";

#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# Sec03 Sub02 Cat07:
#   - endin
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++

endinStr="${genLibStr}endin.\$O: \\
	${genLibStr}endin.c \\
	${genLibStr}endin.h
		$ccStr ${dashOStr}${genLibStr}endin.\$O \\
			$cFlagsStr $coreFlagsStr \\
			${genLibStr}endin.c"

endinObj="${genLibStr}endin.\$O";
endinDep="";

#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# Sec03 Sub02 Cat08:
#   - checkSum
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++

checkSumStr="${genLibStr}checkSum.\$O: \\
	${genLibStr}checkSum.c \\
	${genLibStr}checkSum.h \\
	${genLibStr}endin.\$O
	$ccStr ${dashOStr}${genLibStr}checkSum.\$O \\
		$cFlagsStr $coreFlagsStr \\
		${genLibStr}checkSum.c";


checkSumObj="${genLibStr}checkSum.\$O";
checkSumDep="endin $endinDep";

#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# Sec03 Sub02 Cat09:
#   - strAry
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++

strAryStr="${genLibStr}strAry.\$O: \\
	${genLibStr}strAry.c \\
	${genLibStr}strAry.h \\
	${genLibStr}ulCp.\$O
		$ccStr ${dashOStr}${genLibStr}strAry.\$O \\
			$cFlagsStr $coreFlagsStr \\
			${genLibStr}strAry.c";


strAryObj="${genLibStr}strAry.\$O";
strAryDep="ulCp $ulCpDep";

#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# Sec03 Sub02 Cat10:
#   - ptrAry
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++

ptrAryStr="${genLibStr}ptrAry.\$O: \\
	${genLibStr}ptrAry.c \\
	${genLibStr}ptrAry.h \\
	${genLibStr}ulCp.\$O
		$ccStr ${dashOStr}${genLibStr}ptrAry.\$O \\
			$cFlagsStr $coreFlagsStr \\
			${genLibStr}ptrAry.c";


ptrAryObj="${genLibStr}ptrAry.\$O";
ptrAryDep="ulCp $ulCpDep";

#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# Sec03 Sub02 Cat11:
#   - fileFun
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++

fileFunStr="${genLibStr}fileFun.\$O: \\
	${genLibStr}fileFun.c \\
	${genLibStr}fileFun.h \\
	${genLibStr}ulCp.\$O
		$ccStr ${dashOStr}${genLibStr}fileFun.\$O \\
			$cFlagsStr $coreFlagsStr \\
			${genLibStr}fileFun.c";


fileFunObj="${genLibStr}fileFun.\$O";
fileFunDep="ulCp $ulCpDep";

#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# Sec03 Sub02 Cat12:
#   - inflate
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++

inflateStr="${genLibStr}inflate.\$O: \\
	${genLibStr}inflate.c \\
	${genLibStr}inflate.h \\
	${genLibStr}checkSum.\$O \\
	${genLibStr}ulCp.\$O \\
	${genLibStr}genMath.\$O \\
	${genLibStr}endLine.h
		$ccStr ${dashOStr}${genLibStr}inflate.\$O \\
			$cFlagsStr $coreFlagsStr \\
			${genLibStr}inflate.c";


inflateObj="${genLibStr}inflate.\$O";
inflateDep="checkSum $checkSumDep genMath $genMathDep";
inflateDep="$inflateDep ulCp $ulCpDep";


#*********************************************************
# Sec03 Sub03:
#   - genBio libraries
#   o sec03 sub03 cat01:
#     - codonFun
#   o sec03 sub03 cat02:
#     - kmerFun
#   o sec03 sub03 cat03:
#     - seqST
#   o sec03 sub03 cat04:
#     - gzSeqST
#   o sec03 sub03 cat05:
#     - kmerCnt
#   o sec03 sub03 cat06:
#     - geneCoord
#   o sec03 sub03 cat07:
#     - samEntry
#   o sec03 sub03 cat08:
#     - samRef
#   o sec03 sub03 cat09:
#     - adjCoords
#   o sec03 sub03 cat10:
#     - cigToEqx
#   o sec03 sub03 cat11:
#     - maskPrim
#   o sec03 sub03 cat12:
#     - trimSam
#   o sec03 sub03 cat13:
#     - ampDepth
#   o sec03 sub03 cat14:
#     - tbCon
#   o sec03 sub03 cat15:
#     - edDist
#   o sec03 sub03 cat16:
#     - rmHomo
#*********************************************************

#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# Sec03 Sub03 Cat01:
#   - codonFun
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++

codonFunStr="${genBioStr}codonFun.\$O: \\
	${genBioStr}codonFun.c \\
	${genBioStr}codonFun.h \\
	${genBioStr}codonTbl.h \\
	${genBioStr}ntTo2Bit.h \\
	${genBioStr}revNtTo2Bit.h
		$ccStr ${dashOStr}${genBioStr}codonFun.\$O \\
			$cFlagsStr $coreFlagsStr \\
			${genBioStr}codonFun.c"

codonFunObj="${genBioStr}codonFun.\$O";
codonFunDep="";

#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# Sec03 Sub03 Cat02:
#   - kmerFun
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++

kmerFunStr="${genBioStr}kmerFun.\$O: \\
	${genBioStr}kmerFun.c \\
	${genBioStr}kmerFun.h \\
	${genBioStr}kmerBit.h
		$ccStr ${dashOStr}${genBioStr}kmerFun.\$O \\
			$cFlagsStr $coreFlagsStr \\
			${genBioStr}kmerFun.c"

kmerFunObj="${genBioStr}kmerFun.\$O";
kmerFunDep="";

#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# Sec03 Sub03 Cat03:
#   - seqST
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++

seqSTStr="${genBioStr}seqST.\$O: \\
	${genBioStr}seqST.c \\
	${genBioStr}seqST.h \\
	${genLibStr}fileFun.\$O
		$ccStr ${dashOStr}${genBioStr}seqST.\$O \\
			$cFlagsStr $coreFlagsStr \\
			${genBioStr}seqST.c";


seqSTObj="${genBioStr}seqST.\$O";
seqSTDep="fileFun $fileFunDep";

#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# Sec03 Sub03 Cat04:
#   - gzSeqST
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++

gzSeqSTStr="${genBioStr}gzSeqST.\$O: \\
	${genBioStr}gzSeqST.c \\
	${genBioStr}gzSeqST.h \\
	${genBioStr}seqST.\$O \\
 	${genLibStr}inflate.\$O
		$ccStr ${dashOStr}${genBioStr}gzSeqST.\$O \\
			$cFlagsStr $coreFlagsStr \\
			${genBioStr}gzSeqST.c";


gzSeqSTObj="${genBioStr}gzSeqST.\$O";
gzSeqSTDep="inflate $inflateDep seqST $seqSTDep";

#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# Sec03 Sub03 Cat05:
#   - kmerCnt
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++

kmerCntStr="${genBioStr}kmerCnt.\$O: \\
	${genBioStr}kmerCnt.c \\
	${genBioStr}kmerCnt.h \\
	${genBioStr}seqST.\$O \\
	${genBioStr}ntTo2Bit.h \\
	${genLibStr}genMath.h
		$ccStr ${dashOStr}${genBioStr}kmerCnt.\$O \\
			$cFlagsStr $coreFlagsStr \\
			${genBioStr}kmerCnt.c";


kmerCntObj="${genBioStr}kmerCnt.\$O";
kmerCntDep="seqST $seqSTDep";

#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# Sec03 Sub03 Cat06:
#   - geneCoord
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++

geneCoordStr="${genBioStr}geneCoord.\$O: \\
	${genBioStr}geneCoord.c \\
	${genBioStr}geneCoord.h \\
	${genLibStr}fileFun.\$O \\
	${genLibStr}base10str.\$O \\
	${genLibStr}charCp.\$O \\
	${genLibStr}genMath.h
	$ccStr ${dashOStr}${genBioStr}geneCoord.\$O \\
		$cFlagsStr $coreFlagsStr \\
		${genBioStr}geneCoord.c";

geneCoordObj="${genBioStr}geneCoord.\$O";
geneCoordDep="fileFun $fileFun base10str $base10strDep";
geneCoordDep="$geneCoordDep $ulCpDep charCp $charCpDep";

#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# Sec03 Sub03 Cat07:
#   - samEntry
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++

samEntryStr="${genBioStr}samEntry.\$O: \\
	${genBioStr}samEntry.c \
	${genBioStr}samEntry.h \\
	${genLibStr}fileFun.\$O \\
	${genLibStr}strAry.\$O \\
	${genLibStr}base10str.\$O \\
	${genLibStr}numToStr.\$O \\
	${genBioStr}ntTo5Bit.h \\
	${genLibStr}endLine.h
		$ccStr ${dashOStr}${genBioStr}samEntry.\$O \\
			$cFlagsStr $coreFlagsStr \\
			${genBioStr}samEntry.c";

samEntryObj="${genBioStr}samEntry.\$O";
samEntryDep="fileFun $fileFunDep strAry $strAryDep";
samEntryDep="$samEntryDep numToStr $numToStrDep";
samEntryDep="$samEntryDep base10str $base10strDep";

#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# Sec03 Sub03 Cat08:
#   - samRef
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++

samRefStr="${genBioStr}samRef.\$O: \\
	${genBioStr}samRef.c \
	${genBioStr}samRef.h \\
	${genBioStr}samEntry.\$O \\
	${genLibStr}strAry.\$O \\
	${genLibStr}base10str.\$O \\
	${genLibStr}endLine.h
		$ccStr ${dashOStr}${genBioStr}samRef.\$O \\
			$cFlagsStr $coreFlagsStr \\
			${genBioStr}samRef.c";

samRefObj="${genBioStr}samRef.\$O";
samRefDep="samEntry $samEntryDep strAry $strAryDep";
samRefDep="$samRefDep base10str $base10strDep";

#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# Sec03 Sub03 Cat09:
#   - adjCoords
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++

adjCoordsStr="${genBioStr}adjCoords.\$O: \\
	${genBioStr}adjCoords.c \\
	${genBioStr}adjCoords.h \\
	${genBioStr}geneCoord.\$O \\
	${genBioStr}samEntry.\$O
		$ccStr ${dashOStr}${genBioStr}adjCoords.\$O \\
			$cFlagsStr $coreFlagsStr \\
			${genBioStr}adjCoords.c";

adjCoordsObj="${genBioStr}adjCoords.\$O";
adjCoordsDep="geneCoord $geneCoordDep";
adjCoordsDep="$adjCoordsDep samEntry $samEntryDep";

#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# Sec03 Sub03 Cat10:
#   - cigToEqx
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++

cigToEqxStr="${genBioStr}cigToEqx.\$O: \\
	${genBioStr}cigToEqx.c \\
	${genBioStr}cigToEqx.h \\
	${genBioStr}samEntry.\$O
		$ccStr ${dashOStr}${genBioStr}cigToEqx.\$O \\
			$cFlagsStr $coreFlagsStr \\
			${genBioStr}cigToEqx.c";

cigToEqxObj="${genBioStr}cigToEqx.\$O";
cigToEqxDep="samEntry $samEntryDep";

#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# Sec03 Sub03 Cat11:
#   - maskPrim
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++

maskPrimStr="${genBioStr}maskPrim.\$O: \\
	${genBioStr}maskPrim.c \\
	${genBioStr}maskPrim.h \\
	${genBioStr}samEntry.\$O \\
	${genLibStr}shellSort.\$O
		$ccStr ${dashOStr}${genBioStr}maskPrim.\$O \\
			$cFlagsStr $coreFlagsStr \\
			${genBioStr}maskPrim.c";

maskPrimObj="${genBioStr}maskPrim.\$O";
maskPrimDep="samEntry $samEntryDep";
maskPrimDep="$maskPrimDep shellSort shellSortDep";

#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# Sec03 Sub03 Cat12:
#   - trimSam
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++

trimSamStr="${genBioStr}trimSam.\$O: \\
	${genBioStr}trimSam.c \\
	${genBioStr}trimSam.h \\
	${genBioStr}samEntry.\$O
		$ccStr ${dashOStr}${genBioStr}trimSam.\$O \\
			$cFlagsStr $coreFlagsStr \\
			${genBioStr}trimSam.c";

trimSamObj="${genBioStr}trimSam.\$O";
trimSamDep="samEntry samEntryDep";

#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# Sec03 Sub03 Cat13:
#   - ampDepth
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++

ampDepthStr="${genBioStr}ampDepth.\$O: \\
	${genBioStr}ampDepth.c \\
	${genBioStr}ampDepth.h \\
	${genBioStr}geneCoord.\$O \\
	${genBioStr}samEntry.\$O
		$ccStr ${dashOStr}${genBioStr}ampDepth.\$O \\
			$cFlagsStr $coreFlagsStr \\
			${genBioStr}ampDepth.c";

ampDepthObj="${genBioStr}ampDepth.\$O";
ampDepthDep="geneCoord $geneCoordDep";
ampDepthDep="$ampDepthDep samEntry $samEntryDep";

#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# Sec03 Sub03 Cat14:
#   - tbCon
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++

tbConStr="${genBioStr}tbCon.\$O: \\
	${genBioStr}tbCon.c \\
	${genBioStr}tbCon.h \\
	${genBioStr}samEntry.\$O \\
	${genBioStr}tbConDefs.h \\
	${genLibStr}genMath.h \\
	${genLibStr}endLine.h
		$ccStr ${dashOStr}${genBioStr}tbCon.\$O \\
			$cFlagsStr $coreFlagsStr \\
			${genBioStr}tbCon.c";

tbConObj="${genBioStr}tbCon.\$O";
tbConDep="samEntry $samEntryDep";

#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# Sec03 Sub03 Cat15:
#   - edDist
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++

edDistStr="${genBioStr}edDist.\$O: \\
	${genBioStr}edDist.c \\
	${genBioStr}edDist.h \\
	${genBioStr}samEntry.\$O \\
	${genBioStr}seqST.\$O \\
	${genLibStr}genMath.h \\
	${genLibStr}endLine.h
		$ccStr ${dashOStr}${genBioStr}edDist.\$O \\
		$cFlagsStr $coreFlagsStr \\
		${genBioStr}edDist.c";


edDistObj="${genBioStr}edDist.\$O";
edDistDep="samEntry $samEntryDep seqST $seqSTDep";

#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# Sec03 Sub03 Cat16:
#   - rmHomo
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++

rmHomoStr="${genBioStr}rmHomo.\$O: \\
	${genBioStr}rmHomo.c \\
	${genBioStr}rmHomo.h \\
	${genBioStr}samEntry.\$O \\
	${genBioStr}seqST.\$O
		$ccStr ${dashOStr}${genBioStr}rmHomo.\$O \\
			$cFlagsStr $coreFlagsStr \\
			${genBioStr}rmHomo.c";


rmHomoObj="${genBioStr}rmHomo.\$O";
rmHomoDep="samEntry $samEntryDep seqST $seqSTDep";

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
#   o sec03 sub04 cat09:
#     - mapRead
#*********************************************************

#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# Sec03 Sub04 Cat01:
#   - indexToCoord
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++

indexToCoordStr="${genAlnStr}indexToCoord.\$O: \\
	${genAlnStr}indexToCoord.c \\
	${genAlnStr}indexToCoord.h
		$ccStr ${dashOStr}${genAlnStr}indexToCoord.\$O \\
			$cFlagsStr $coreFlagsStr \\
			${genAlnStr}indexToCoord.c";


indexToCoordObj="${genAlnStr}indexToCoord.\$O";
indexToCoordDep="";

#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# Sec03 Sub04 Cat02:
#   - alnSet
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++

alnSetStr="${genAlnStr}alnSet.\$O: \\
	${genAlnStr}alnSet.c \\
	${genAlnStr}alnDefs.h \\
	${genLibStr}fileFun.\$O \\
	${genLibStr}ulCp.\$O \\
	${genLibStr}base10str.\$O \\
	${genAlnStr}alnDefs.h \\
	${genLibStr}endLine.h
		$ccStr ${dashOStr}${genAlnStr}alnSet.\$O \\
			$cFlagsStr $coreFlagsStr \\
			${genAlnStr}alnSet.c";


alnSetObj="${genAlnStr}alnSet.\$O";
alnSetDep="base10str $base10strDep fileFun $fileFunDep"

#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# Sec03 Sub04 Cat03:
#   - dirMatrix
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++

dirMatrixStr="${genAlnStr}dirMatrix.\$O: \\
	${genAlnStr}dirMatrix.c \\
	${genAlnStr}alnSet.\$O \\
	${genAlnStr}indexToCoord.\$O \\
	${genBioStr}samEntry.\$O \\
	${genBioStr}seqST.\$O \\
	${genLibStr}charCp.\$O
		$ccStr ${dashOStr}${genAlnStr}dirMatrix.\$O \\
			$cFlagsStr $coreFlagsStr \\
			${genAlnStr}dirMatrix.c";


dirMatrixObj="${genAlnStr}dirMatrix.\$O";
dirMatrixDep="alnSet $alnSetDep";
dirMatrixDep="$dirMatrixDep indexToCoord";
dirMatrixDep="$dirMatrixDep $indexToCoordDep";
dirMatrixDep="$dirMatrixDep samEntry $samEntryDep";
dirMatrixDep="$dirMatrixDep seqST $seqSTDep";
dirMatrixDep="$dirMatrixDep charCp $charCpDep";

#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# Sec03 Sub04 Cat04:
#   - water
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++

waterStr="${genAlnStr}water.\$O: ${genAlnStr}water.c \\
	${genAlnStr}dirMatrix.\$O \\
	${genLibStr}genMath.h
		$ccStr ${dashOStr}${genAlnStr}water.\$O \\
			$cFlagsStr $coreFlagsStr \\
			${genAlnStr}water.c";


waterObj="${genAlnStr}water.\$O";
waterDep="dirMatrix $dirMatrixDep";

#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# Sec03 Sub04 Cat05:
#   - needle
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++

needleStr="${genAlnStr}needle.\$O: \\
	${genAlnStr}needle.c \\
	${genAlnStr}dirMatrix.\$O \\
	${genLibStr}genMath.h
		$ccStr ${dashOStr}${genAlnStr}needle.\$O \\
			$cFlagsStr $coreFlagsStr \\
			${genAlnStr}needle.c";


needleObj="${genAlnStr}needle.\$O";
needleDep="dirMatrix $dirMatrixDep";

#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# Sec03 Sub04 Cat06:
#   - memwater
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++

memwaterStr="${genAlnStr}memwater.\$O: \\
	${genAlnStr}memwater.c \\
	${genAlnStr}alnSet.\$O \\
	${genAlnStr}indexToCoord.\$O \\
	${genBioStr}seqST.\$O \\
	${genLibStr}genMath.h
		$ccStr ${dashOStr}${genAlnStr}memwater.\$O \\
			$cFlagsStr $coreFlagsStr \\
			${genAlnStr}memwater.c";


memwaterObj="${genAlnStr}memwater.\$O";
memwaterDep="alnSet $alnSetDep indexToCoord";
memwaterDep="$memwaterDep $indexToCoordDep";
memwaterDep="$memwaterDep seqST $seqSTDep";

#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# Sec03 Sub04 Cat07:
#   - kmerFind
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++

kmerFindStr="${genAlnStr}kmerFind.\$O: \\
	${genAlnStr}kmerFind.c \\
	${genAlnStr}memwater.\$O \\
	${genLibStr}shellSort.\$O \\
	${genLibStr}genMath.h \\
	${genBioStr}kmerBit.h
		$ccStr ${dashOStr}${genAlnStr}kmerFind.\$O \\
			$cFlagsStr $coreFlagsStr \\
			${genAlnStr}kmerFind.c";


kmerFindObj="${genAlnStr}kmerFind.\$O";
kmerFindDep="memwater $memwaterDep";
kmerFindDep="$kmerFindDep shellSort $shellSortDep";

#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# Sec03 Sub04 Cat08:
#   - samToAln
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++

samToAlnStr="${genAlnStr}samToAln.\$O: \\
	${genAlnStr}samToAln.c \\
	${genAlnStr}samToAln.h \\
	${genBioStr}samEntry.\$O \\
	${genBioStr}seqST.\$O \\
	${genAlnStr}alnSet.\$O
		$ccStr ${dashOStr}${genAlnStr}samToAln.\$O \\
			$cFlagsStr $coreFlagsStr \\
			${genAlnStr}samToAln.c";


samToAlnObj="${genAlnStr}samToAln.\$O";
samToAlnDep="alnSet $alnSetDep samEntry $samEntryDep";
samToAlnDep="$samToAlnDep seqST $seqSTDep";

#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# Sec03 Sub04 Cat09:
#   - mapRead
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++

mapReadStr="${genAlnStr}mapRead.\$O: \\
	${genAlnStr}mapRead.c \\
	${genAlnStr}mapRead.h \\
	${genAlnStr}needle.\$O \\
	${genAlnStr}water.\$O \\
	${genAlnStr}dirMatrix.\$O \\
	${genBioStr}kmerFun.\$O \\
	${genLibStr}shellSort.\$O \\
	${genAlnStr}defsMapRead.h  \\
	${genLibStr}genMath.h
	 	$ccStr ${dashOStr}${genAlnStr}mapRead.\$O \\
			$cFlagsStr $coreFlagsStr \\
			${genAlnStr}mapRead.c";


mapReadObj="${genAlnStr}mapRead.\$O";
mapReadDep="needle $needleDep dirMatrix $dirMatrixDep";
mapReadDep="$mapReadDep water $waterDep";
mapReadDep="$mapReadDep dirMatrix $dirMatrixDep";
mapReadDep="$mapReadDep samEntry $samEntryDep";
mapReadDep="$mapReadDep seqST $seqSTDep";
mapReadDep="$mapReadDep kmerFun $kmerFunDep";
mapReadDep="$mapReadDep shellSort $shellSortDep";

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

illNanoStr="${genClustStr}illNano.\$O: \\
	${genClustStr}illNano.c \\
	${genClustStr}illNano.h \\
	${genBioStr}samEntry.\$O
		$ccStr ${dashOStr}${genClustStr}illNano.\$O \\
			$cFlagsStr $coreFlagsStr \\
			${genClustStr}illNano.c;
";


illNanoObj="${genClustStr}illNano.\$O";
illNanoDep="samEntry $samEntryDep";

#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# Sec03 Sub05 Cat02:
#   - clustST
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++

clustSTStr="${genClustStr}clustST.\$O: \\
	${genClustStr}clustST.c \\
	${genClustStr}clustST.h \\
	${genBioStr}samEntry.\$O \\
	${genBioStr}tbCon.\$O \\
	${genBioStr}edDist.\$O \\
	${genLibStr}genMath.h
		$ccStr ${dashOStr}${genClustStr}clustST.\$O \\
			$cFlagsStr $coreFlagsStr \\
			${genClustStr}clustST.c
";

clustSTObj="${genClustStr}clustST.\$O";
clustSTDep="samEntry $samEntryDep tbCon $tbConDep";
clustSTDep="$clustSTDep edDist $edDistDep";

#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# Sec03 Sub05 Cat03:
#   - edClust
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++

edClustStr="${genClustStr}edClust.\$O: \\
	${genClustStr}edClust.c \\
	${genClustStr}edClust.h \\
	${genClustStr}clustST.\$O
		$ccStr ${dashOStr}${genClustStr}edClust.\$O \\
			$cFlagsStr $coreFlagsStr \\
			${genClustStr}edClust.c
";

edClustObj="${genClustStr}edClust.\$O";
edClustDep="clustST $clustSTDep";

#*********************************************************
# Sec03 Sub06:
#   - genGenoType libraries
#   o sec03 sub06 cat01:
#     - cgMLST
#*********************************************************

#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# Sec03 Sub06 Cat01:
#   - cgMLST
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++

cgMLSTStr="${genGenoTypeStr}cgMLST.\$O: \\
	${genGenoTypeStr}cgMLST.c \\
	${genGenoTypeStr}cgMLST.h \\
	${genBioStr}edDist.\$O \\
	${genLibStr}ptrAry.\$O
		$ccStr ${dashOStr}${genGenoTypeStr}cgMLST.\$O \\
			$cFlagsStr $coreFlagsStr \\
			${genGenoTypeStr}cgMLST.c
";

cgMLSTObj="${genGenoTypeStr}cgMLST.\$O";
cgMLSTDep="edDist $edDistDep ptrAry $ptrAryDep";

#*********************************************************
# Sec03 Sub07:
#   - misc code in own folders
#   o sec03 sub07 cat01:
#     - k2TaxaId
#*********************************************************

#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# Sec03 Sub07 Cat01:
#   - k2TaxaId
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++

tmpStr="..${slashSC}k2TaxaIdSrc${slashSC}k2TaxaId";
k2TaxaIdStr="$tmpStr.\$O: \\
	$tmpStr.c $tmpStr.h \\
	${genLibStr}fileFun.\$O \\
	${genLibStr}ptrAry.\$O \\
	${genLibStr}shellSort.\$O \\
	${genLibStr}numToStr.\$O \\
	${genLibStr}base10str.\$O \\
	${genLibStr}genMath.\$O
		$ccStr $dashOStr$tmpStr.\$O \\
			$cFlagsStr $coreFlagsStr $tmpStr.c";

k2TaxaIdObj="$tmpStr.\$O";
k2TaxaIdDep="fileFun $fileFunDep";
k2TaxaIdDep="$k2TaxaIdDep ptrAry $ptrAryDep";
k2TaxaIdDep="$k2TaxaIdDep base10str $base10strDep";
k2TaxaIdDep="$k2TaxaIdDep numToStr $numToStrDep";
k2TaxaIdDep="$k2TaxaIdDep genMath $genMathDep";
k2TaxaIdDep="$k2TaxaIdDep shellSort $shellSortDep";

#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# Sec04:
#   - build make file command
#   o sec04 sub01:
#     o move past name and OS and get user arguments
#   o sec04 sub02:
#     - deal with .h files and spacing + start mkfile loop
#   o sec04 sub03:
#     - genLib libraries + start makefile loop
#   o sec04 sub04:
#     - genBio libraries
#   o sec04 sub05:
#     - genAln libraries
#   o sec04 sub07:
#     - genClust libraries
#   o sec04 sub07:
#     - non-general library files
#   o sec04 sub08:
#     - move to next library or dependency
#<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

#*********************************************************
# Sec04 Sub01:
#   o move past name and OS and get user arguments
#*********************************************************

objFilesStr="objFiles=";
mainCntSI=0;
libCntSI=0;

# move past header
shift; # move past os type
shift; # move past program name
shift; # move past main file

while [ $# -gt 0 ];
do   # Loop: get dependencies
   depStr="$depStr $1";
   mainCntSI="$((mainCntSI + 1))";
   shift;
done # Loop: get dependencies


depStr="${depStr#"${depStr%%[![:space:]]*}"}";
   # clear leading spaces
libStr="${depStr%% *}";
   # get first argument
depStr="${depStr#"${depStr%%[[:space:]]*}"}";
   # pop off first arugment
depStr="${depStr#"${depStr%%[![:space:]]*}"}";
   # clear leading spaces

#*********************************************************
# Sec04 Sub02:
#   - deal with .h files and spacing + start mkfile loop
#*********************************************************

firstBl="";

while [ "$libStr" != "" ];
do # Loop: get dependencies

   if [ "$firstBl" = "" ]; then
      newLineStr="";
      firstBl=1;
   else
      newLineStr="\n\n";
   fi 

   # this step is to save any .h files the user provided
   #   that way they are included in the command
   hFileBl="$(\
      printf "%s" "$libStr" | sed 's/.*\.h$/\.h/' \
   )";

   if [ "$hFileBl" = ".h" ];
   then # If: is a .h file
      mainCmdStr="$mainCmdStr \\
	$libStr";
      libCntSI="$((libCntSI + 1))";

      libStr="${depStr%% *}";
         # get first argument
      depStr="${depStr#"${depStr%%[[:space:]]*}"}";
         # pop off first arugment
      depStr="${depStr#"${depStr%%[![:space:]]*}"}";
         # clear leading spaces

      continue;
   fi

   #******************************************************
   # Sec04 Sub03:
   #   - genLib libraries
   #   o sec04 sub03 cat01:
   #     - numToStr
   #   o sec04 sub03 cat02:
   #     - base10str
   #   o sec04 sub03 cat03:
   #     - ulCp
   #   o sec04 sub03 cat04:
   #     - charCp
   #   o sec04 sub03 cat05:
   #     - genMath
   #   o sec04 sub03 cat06:
   #     - shellSort
   #   o sec04 sub03 cat07:
   #     - endin
   #   o sec04 sub03 cat08:
   #     - checkSum
   #   o sec04 sub03 cat09:
   #     - strAry
   #   o sec04 sub03 cat10:
   #     - ptrAry
   #   o sec04 sub03 cat11:
   #     - fileFun
   #   o sec04 sub03 cat12:
   #     - inflate
   #******************************************************

   #++++++++++++++++++++++++++++++++++++++++++++++++++++++
   # Sec04 Sub03 Cat01:
   #   - numToStr
   #++++++++++++++++++++++++++++++++++++++++++++++++++++++

   if [ "$libStr" = "numToStr" ]; then
   # If: numToStr library
      if [ "$numToStrBl" = "" ]; then
         cmdStr="$cmdStr$newLineStr$numToStr";
         objFilesStr="$objFilesStr \\\\\n$spaceStr";
         objFilesStr="${objFilesStr}$numToStrObj";

         if [ $libCntSI -lt $mainCntSI ]; then
            mainCmdStr="$mainCmdStr \\\n	$numToStrObj";
         fi

         numToStrBl=1;
         depStr="$depStr $numToStrDep";
      fi

      if [ "$genLibBl" -lt 1 ]; then
         genLibBl=1;
         libPathStr="$libPathStr\ngenLib=..${slashSC}genLib";
      fi
   # If: numToStr library

   #++++++++++++++++++++++++++++++++++++++++++++++++++++++
   # Sec04 Sub03 Cat02:
   #   - base10str
   #++++++++++++++++++++++++++++++++++++++++++++++++++++++

   elif [ "$libStr" = "base10str" ]; then
   # Else If: base10str library
      if [ "$base10strBl" = "" ]; then
         cmdStr="$cmdStr$newLineStr$base10Str";
         objFilesStr="$objFilesStr \\\\\n$spaceStr";
         objFilesStr="${objFilesStr}$base10strObj";

         if [ $libCntSI -lt $mainCntSI ]; then
            mainCmdStr="$mainCmdStr \\
	$base10strObj";
         fi

         base10strBl=1;

         depStr="$depStr $base10strDep";
      fi

      if [ "$genLibBl" -lt 1 ]; then
         genLibBl=1;
         libPathStr="$libPathStr\ngenLib=..${slashSC}genLib";
      fi
   # Else If: base10str library

   #++++++++++++++++++++++++++++++++++++++++++++++++++++++
   # Sec04 Sub03 Cat03:
   #   - ulCp
   #++++++++++++++++++++++++++++++++++++++++++++++++++++++

   elif [ "$libStr" = "ulCp" ]; then
   # Else If: ulCp library
      if [ "$ulCpBl" = "" ]; then
         cmdStr="$cmdStr$newLineStr$ulCpStr";
         objFilesStr="$objFilesStr \\\\\n$spaceStr";
         objFilesStr="${objFilesStr}$ulCpObj";

         if [ $libCntSI -lt $mainCntSI ]; then
            mainCmdStr="$mainCmdStr \\
	$ulCpObj";
         fi

         ulCpBl=1;
         depStr="$depStr $ulCpDep";
      fi

      if [ "$genLibBl" -lt 1 ]; then
         genLibBl=1;
         libPathStr="$libPathStr\ngenLib=..${slashSC}genLib";
      fi
   # Else If: ulCp library

   #++++++++++++++++++++++++++++++++++++++++++++++++++++++
   # Sec04 Sub03 Cat04:
   #   - charCp
   #++++++++++++++++++++++++++++++++++++++++++++++++++++++

   elif [ "$libStr" = "charCp" ]; then
   # Else If: charCp library
      if [ "$charCpBl" = "" ]; then
         cmdStr="$cmdStr$newLineStr$charCpStr";
         objFilesStr="$objFilesStr \\\\\n$spaceStr";
         objFilesStr="${objFilesStr}$charCpObj";

         if [ $libCntSI -lt $mainCntSI ]; then
            mainCmdStr="$mainCmdStr \\
	$charCpObj";
         fi

         charCpBl=1;
         depStr="$depStr $charCpDep";
      fi

      if [ "$genLibBl" -lt 1 ]; then
         genLibBl=1;
         libPathStr="$libPathStr\ngenLib=..${slashSC}genLib";
      fi
   # Else If: charCp library

   #++++++++++++++++++++++++++++++++++++++++++++++++++++++
   # Sec04 Sub03 Cat05:
   #   - genMath
   #++++++++++++++++++++++++++++++++++++++++++++++++++++++

   elif [ "$libStr" = "genMath" ]; then
   # Else If: genMath library
      if [ "$genMathBl" = "" ]; then
         cmdStr="$cmdStr$newLineStr$genMathStr";
         objFilesStr="$objFilesStr \\\\\n$spaceStr";
         objFilesStr="${objFilesStr}$genMathObj";

         if [ $libCntSI -lt $mainCntSI ]; then
            mainCmdStr="$mainCmdStr \\
	$genMathObj";
         fi

         genMathBl=1;
         depStr="$depStr $genMathDep";
      fi

      if [ "$genLibBl" -lt 1 ]; then
         genLibBl=1;
         libPathStr="$libPathStr\ngenLib=..${slashSC}genLib";
      fi
   # Else If: genMath library

   #++++++++++++++++++++++++++++++++++++++++++++++++++++++
   # Sec04 Sub03 Cat06:
   #   - shellSort
   #++++++++++++++++++++++++++++++++++++++++++++++++++++++

   elif [ "$libStr" = "shellSort" ]; then
   # Else If: shellSort library
      if [ "$shellSortBl" = "" ]; then
         cmdStr="$cmdStr$newLineStr$shellSortStr";
         objFilesStr="$objFilesStr \\\\\n$spaceStr";
         objFilesStr="${objFilesStr}$shellSortObj";

         if [ $libCntSI -lt $mainCntSI ]; then
            mainCmdStr="$mainCmdStr \\
	$shellSortObj";
         fi

         shellSortBl=1;
         depStr="$depStr $shellSortDep";
      fi

      if [ "$genLibBl" -lt 1 ]; then
         genLibBl=1;
         libPathStr="$libPathStr\ngenLib=..${slashSC}genLib";
      fi
   # Else If: shellSort library

   #++++++++++++++++++++++++++++++++++++++++++++++++++++++
   # Sec04 Sub03 Cat07:
   #   - endin
   #++++++++++++++++++++++++++++++++++++++++++++++++++++++

   elif [ "$libStr" = "endin" ]; then
   # Else If: endin library
      if [ "$endinBl" = "" ]; then
         cmdStr="$cmdStr$newLineStr$endinStr";
         objFilesStr="$objFilesStr \\\\\n$spaceStr";
         objFilesStr="${objFilesStr}${endinObj}";

         if [ $libCntSI -lt $mainCntSI ]; then
            mainCmdStr="$mainCmdStr \\
	$endinObj";
         fi

         endinBl=1;
         depStr="$depStr $endinDep";
      fi

      if [ "$genLibBl" -lt 1 ]; then
         genLibBl=1;
         libPathStr="$libPathStr\ngenLib=..${slashSC}genLib";
      fi
   # Else If: endin library

   #++++++++++++++++++++++++++++++++++++++++++++++++++++++
   # Sec04 Sub03 Cat08:
   #   - checkSum
   #++++++++++++++++++++++++++++++++++++++++++++++++++++++

   elif [ "$libStr" = "checkSum" ]; then
   # Else If: checkSum library
      if [ "$checkSumBl" = "" ]; then
         cmdStr="$cmdStr$newLineStr$checkSumStr";
         objFilesStr="$objFilesStr \\\\\n$spaceStr";
         objFilesStr="${objFilesStr}${checkSumObj}";

         if [ $libCntSI -lt $mainCntSI ]; then
            mainCmdStr="$mainCmdStr \\
	$checkSumObj";
         fi

         checkSumBl=1;
         depStr="$depStr $checkSumDep";
      fi

      if [ "$genLibBl" -lt 1 ]; then
         genLibBl=1;
         libPathStr="$libPathStr\ngenLib=..${slashSC}genLib";
      fi
   # Else If: checkSum library

   #++++++++++++++++++++++++++++++++++++++++++++++++++++++
   # Sec04 Sub03 Cat09:
   #   - strAry
   #++++++++++++++++++++++++++++++++++++++++++++++++++++++

   elif [ "$libStr" = "strAry" ]; then
   # Else If: strAry library
      if [ "$strAryBl" = "" ]; then
         cmdStr="$cmdStr$newLineStr$strAryStr";
         objFilesStr="$objFilesStr \\\\\n$spaceStr";
         objFilesStr="${objFilesStr}${strAryObj}";

         if [ $libCntSI -lt $mainCntSI ]; then
            mainCmdStr="$mainCmdStr \\
	$strAryObj";
         fi

         strAryBl=1;
         depStr="$depStr $strAryDep";
      fi

      if [ "$genLibBl" -lt 1 ]; then
         genLibBl=1;
         libPathStr="$libPathStr\ngenLib=..${slashSC}genLib";
      fi
   # Else If: strAry library

   #++++++++++++++++++++++++++++++++++++++++++++++++++++++
   # Sec04 Sub03 Cat10:
   #   - ptrAry
   #++++++++++++++++++++++++++++++++++++++++++++++++++++++

   elif [ "$libStr" = "ptrAry" ]; then
   # Else If: ptrAry library
      if [ "$ptrAryBl" = "" ]; then
         cmdStr="$cmdStr$newLineStr$ptrAryStr";
         objFilesStr="$objFilesStr \\\\\n$spaceStr";
         objFilesStr="${objFilesStr}${ptrAryObj}";

         if [ $libCntSI -lt $mainCntSI ]; then
            mainCmdStr="$mainCmdStr \\
	$ptrAryObj";
         fi

         ptrAryBl=1;
         depStr="$depStr $ptrAryDep";
      fi

      if [ "$genLibBl" -lt 1 ]; then
         genLibBl=1;
         libPathStr="$libPathStr\ngenLib=..${slashSC}genLib";
      fi
   # Else If: ptrAry library

   #++++++++++++++++++++++++++++++++++++++++++++++++++++++
   # Sec04 Sub03 Cat11:
   #   - fileFun
   #++++++++++++++++++++++++++++++++++++++++++++++++++++++

   elif [ "$libStr" = "fileFun" ]; then
   # Else If: fileFun library
      if [ "$fileFunBl" = "" ]; then
         cmdStr="$cmdStr$newLineStr$fileFunStr";
         objFilesStr="$objFilesStr \\\\\n$spaceStr";
         objFilesStr="${objFilesStr}${fileFunObj}";

         if [ $libCntSI -lt $mainCntSI ]; then
            mainCmdStr="$mainCmdStr \\
	$fileFunObj";
         fi

         fileFunBl=1;
         depStr="$depStr $fileFunDep";
      fi

      if [ "$genLibBl" -lt 1 ]; then
         genLibBl=1;
         libPathStr="$libPathStr\ngenLib=..${slashSC}genLib";
      fi
   # Else If: fileFun library

   #++++++++++++++++++++++++++++++++++++++++++++++++++++++
   # Sec04 Sub03 Cat12:
   #   - inflate
   #++++++++++++++++++++++++++++++++++++++++++++++++++++++

   elif [ "$libStr" = "inflate" ]; then
   # Else If: inflate library
      if [ "$inflateBl" = "" ]; then
         cmdStr="$cmdStr$newLineStr$inflateStr";
         objFilesStr="$objFilesStr \\\\\n$spaceStr";
         objFilesStr="${objFilesStr}${inflateObj}";

         if [ $libCntSI -lt $mainCntSI ]; then
            mainCmdStr="$mainCmdStr \\
	$inflateObj";
         fi

         inflateBl=1;
         depStr="$depStr $inflateDep";
      fi

      if [ "$genLibBl" -lt 1 ]; then
         genLibBl=1;
         libPathStr="$libPathStr\ngenLib=..${slashSC}genLib";
      fi
   # Else If: inflate library

   #******************************************************
   # Sec04 Sub04:
   #   - genBio libraries
   #   o sec04 sub04 cat01:
   #     - codonFun
   #   o sec04 sub04 cat02:
   #     - kmerFun
   #   o sec04 sub04 cat03:
   #     - seqST
   #   o sec04 sub04 cat04:
   #     - gzSeqST
   #   o sec04 sub04 cat05:
   #     - kmerCnt
   #   o sec04 sub04 cat06:
   #     - geneCoord
   #   o sec04 sub04 cat07:
   #     - samEntry
   #   o sec04 sub04 cat08:
   #     - samRef
   #   o sec04 sub04 cat09:
   #     - adjCoords
   #   o sec04 sub04 cat10:
   #     - cigToEqx
   #   o sec04 sub04 cat11:
   #     - maskPrim
   #   o sec04 sub04 cat12:
   #     - trimSam
	#   o sec04 sub04 cat13:
   #     - ampDepth
	#   o sec04 sub04 cat14:
   #     - tbCon
	#   o sec04 sub04 cat15:
   #     - edDist
	#   o sec04 sub04 cat16:
   #     - rmHomo
   #******************************************************

   #++++++++++++++++++++++++++++++++++++++++++++++++++++++
   # Sec04 Sub04 Cat01:
   #   - codonFun
   #++++++++++++++++++++++++++++++++++++++++++++++++++++++

   elif [ "$libStr" = "codonFun" ]; then
   # Else If: codonFun library
      if [ "$codonFunBl" = "" ]; then
         cmdStr="$cmdStr$newLineStr$codonFunStr";
         objFilesStr="$objFilesStr \\\\\n$spaceStr";
         objFilesStr="${objFilesStr}${codonFunObj}";

         if [ $libCntSI -lt $mainCntSI ]; then
            mainCmdStr="$mainCmdStr \\
	$codonFunObj";
         fi

         codonFunBl=1;
         depStr="$depStr $codonFunDep";
      fi

      if [ "$genBioBl" -lt 1 ]; then
         genBioBl=1;
         libPathStr="$libPathStr\ngenBio=..${slashSC}genBio";
      fi
   # Else If: codonFun library

   #++++++++++++++++++++++++++++++++++++++++++++++++++++++
   # Sec04 Sub04 Cat02:
   #   - kmerFun
   #++++++++++++++++++++++++++++++++++++++++++++++++++++++

   elif [ "$libStr" = "kmerFun" ]; then
   # Else If: kmerFun library
      if [ "$kmerFunBl" = "" ]; then
         cmdStr="$cmdStr$newLineStr$kmerFunStr";
         objFilesStr="$objFilesStr \\\\\n$spaceStr";
         objFilesStr="${objFilesStr}${kmerFunObj}";

         if [ $libCntSI -lt $mainCntSI ]; then
            mainCmdStr="$mainCmdStr \\
	$kmerFunObj";
         fi

         kmerFunBl=1;
         depStr="$depStr $kmerFunDep";
      fi

      if [ "$genBioBl" -lt 1 ]; then
         genBioBl=1;
         libPathStr="$libPathStr\ngenBio=..${slashSC}genBio";
      fi
   # Else If: kmerFun library

   #++++++++++++++++++++++++++++++++++++++++++++++++++++++
   # Sec04 Sub04 Cat03:
   #   - seqST
   #++++++++++++++++++++++++++++++++++++++++++++++++++++++

   elif [ "$libStr" = "seqST" ]; then
   # Else If: seqST library
      if [ "$seqSTBl" = "" ]; then
         cmdStr="$cmdStr$newLineStr$seqSTStr";
         objFilesStr="$objFilesStr \\\\\n$spaceStr";
         objFilesStr="${objFilesStr}${seqSTObj}";

         if [ $libCntSI -lt $mainCntSI ]; then
            mainCmdStr="$mainCmdStr \\
	$seqSTObj";
         fi

         seqSTBl=1;

         depStr="$depStr $seqSTDep";
      fi

      if [ "$genBioBl" -lt 1 ]; then
         genBioBl=1;
         libPathStr="$libPathStr\ngenBio=..${slashSC}genBio";
      fi
   # Else If: seqST library

   #++++++++++++++++++++++++++++++++++++++++++++++++++++++
   # Sec04 Sub04 Cat04:
   #   - seqST
   #++++++++++++++++++++++++++++++++++++++++++++++++++++++

   elif [ "$libStr" = "gzSeqST" ]; then
   # Else If: gzSeqST library
      if [ "$gzSeqSTBl" = "" ]; then
         cmdStr="$cmdStr$newLineStr$gzSeqSTStr";
         objFilesStr="$objFilesStr \\\\\n$spaceStr";
         objFilesStr="${objFilesStr}${gzSeqSTObj}";

         if [ $libCntSI -lt $mainCntSI ]; then
            mainCmdStr="$mainCmdStr \\
	$gzSeqSTObj";
         fi

         gzSeqSTBl=1;

         depStr="$depStr $gzSeqSTDep";
      fi

      if [ "$genBioBl" -lt 1 ]; then
         genBioBl=1;
         libPathStr="$libPathStr\ngenBio=..${slashSC}genBio";
      fi
   # Else If: gzSeqST library

   #++++++++++++++++++++++++++++++++++++++++++++++++++++++
   # Sec04 Sub04 Cat05:
   #   - kmerCnt
   #++++++++++++++++++++++++++++++++++++++++++++++++++++++

   elif [ "$libStr" = "kmerCnt" ]; then
   # Else If: kmerCnt library
      if [ "$kmerCntBl" = "" ]; then
         cmdStr="$cmdStr$newLineStr$kmerCntStr";
         objFilesStr="$objFilesStr \\\\\n$spaceStr";
         objFilesStr="${objFilesStr}${kmerCntObj}";

         if [ $libCntSI -lt $mainCntSI ]; then
            mainCmdStr="$mainCmdStr \\
	$kmerCntObj";
         fi

         kmerCntBl=1;
         depStr="$depStr $kmerCntDep";
      fi

      if [ "$genBioBl" -lt 1 ]; then
         genBioBl=1;
         libPathStr="$libPathStr\ngenBio=..${slashSC}genBio";
      fi
   # Else If: kmerCnt library

   #++++++++++++++++++++++++++++++++++++++++++++++++++++++
   # Sec04 Sub04 Cat06:
   #   - geneCoord
   #++++++++++++++++++++++++++++++++++++++++++++++++++++++

   elif [ "$libStr" = "geneCoord" ]; then
   # Else If: geneCoord library
      if [ "$geneCoordBl" = "" ]; then
         cmdStr="$cmdStr$newLineStr$geneCoordStr";
         objFilesStr="$objFilesStr \\\\\n$spaceStr";
         objFilesStr="${objFilesStr}${geneCoordObj}";

         geneCoordBl=1;
         depStr="$depStr $geneCoordDep";
      fi

      if [ "$genBioBl" -lt 1 ]; then
         genBioBl=1;
         libPathStr="$libPathStr\ngenBio=..${slashSC}genBio";
      fi
   # Else If: geneCoord library

   #++++++++++++++++++++++++++++++++++++++++++++++++++++++
   # Sec04 Sub04 Cat07:
   #   - samEntry
   #++++++++++++++++++++++++++++++++++++++++++++++++++++++

   elif [ "$libStr" = "samEntry" ]; then
   # Else If: samEntry library
      if [ "$samEntryBl" = "" ]; then
         cmdStr="$cmdStr$newLineStr$samEntryStr";
         objFilesStr="$objFilesStr \\\\\n$spaceStr";
         objFilesStr="${objFilesStr}${samEntryObj}";

         if [ $libCntSI -lt $mainCntSI ]; then
            mainCmdStr="$mainCmdStr \\
	$samEntryObj";
         fi

         samEntryBl=1;
         depStr="$depStr $samEntryDep";
      fi

      if [ "$genBioBl" -lt 1 ]; then
         genBioBl=1;
         libPathStr="$libPathStr\ngenBio=..${slashSC}genBio";
      fi
   # Else If: samEntry library

   #++++++++++++++++++++++++++++++++++++++++++++++++++++++
   # Sec04 Sub04 Cat08:
   #   - samRef
   #++++++++++++++++++++++++++++++++++++++++++++++++++++++

   elif [ "$libStr" = "samRef" ]; then
   # Else If: samRef library
      if [ "$samRefBl" = "" ]; then
         cmdStr="$cmdStr$newLineStr$samRefStr";
         objFilesStr="$objFilesStr \\\\\n$spaceStr";
         objFilesStr="${objFilesStr}${samRefObj}";

         if [ $libCntSI -lt $mainCntSI ]; then
            mainCmdStr="$mainCmdStr \\
	$samRefObj";
         fi

         samRefBl=1;
         depStr="$depStr $samRefDep";
      fi

      if [ "$genBioBl" -lt 1 ]; then
         genBioBl=1;
         libPathStr="$libPathStr\ngenBio=..${slashSC}genBio";
      fi
   # Else If: samRef library

   #++++++++++++++++++++++++++++++++++++++++++++++++++++++
   # Sec04 Sub04 Cat09:
   #   - adjCoords
   #++++++++++++++++++++++++++++++++++++++++++++++++++++++

   elif [ "$libStr" = "adjCoords" ]; then
   # Else If: adjCoords library
      if [ "$adjCoordsBl" = "" ]; then
         cmdStr="$cmdStr$newLineStr$adjCoordsStr";
         objFilesStr="$objFilesStr \\\\\n$spaceStr";
         objFilesStr="${objFilesStr}${adjCoordsObj}";

         if [ $libCntSI -lt $mainCntSI ]; then
            mainCmdStr="$mainCmdStr \\
	$adjCoordsObj";
         fi

         adjCoordsBl=1;
         depStr="$depStr $adjCoordsDep";
      fi

      if [ "$genBioBl" -lt 1 ]; then
         genBioBl=1;
         libPathStr="$libPathStr\ngenBio=..${slashSC}genBio";
      fi
   # Else If: adjCoords library

   #++++++++++++++++++++++++++++++++++++++++++++++++++++++
   # Sec04 Sub04 Cat10:
   #   - cigToEqx
   #++++++++++++++++++++++++++++++++++++++++++++++++++++++

   elif [ "$libStr" = "cigToEqx" ]; then
   # Else If: cigToEqx library
      if [ "$cigToEqxBl" = "" ]; then
         cmdStr="$cmdStr$newLineStr$cigToEqxStr";
         objFilesStr="$objFilesStr \\\\\n$spaceStr";
         objFilesStr="${objFilesStr}${cigToEqxObj}";

         if [ $libCntSI -lt $mainCntSI ]; then
            mainCmdStr="$mainCmdStr \\
	$cigToEqxObj";
         fi

         cigToEqxBl=1;

         depStr="$depStr $cigToEqxDep";
      fi

      if [ "$genBioBl" -lt 1 ]; then
         genBioBl=1;
         libPathStr="$libPathStr\ngenBio=..${slashSC}genBio";
      fi
   # Else If: cigToEqx library

   #++++++++++++++++++++++++++++++++++++++++++++++++++++++
   # Sec04 Sub04 Cat11:
   #   - maskPrim
   #++++++++++++++++++++++++++++++++++++++++++++++++++++++

   elif [ "$libStr" = "maskPrim" ]; then
   # Else If: maskPrim library
      if [ "$maskPrimBl" = "" ]; then
         cmdStr="$cmdStr$newLineStr$maskPrimStr";
         objFilesStr="$objFilesStr \\\\\n$spaceStr";
         objFilesStr="${objFilesStr}${maskPrimObj}";

         if [ $libCntSI -lt $mainCntSI ]; then
            mainCmdStr="$mainCmdStr \\
	$maskPrimObj";
         fi

         maskPrimBl=1;
         depStr="$depStr $maskPrimDep";
      fi

      if [ "$genBioBl" -lt 1 ]; then
         genBioBl=1;
         libPathStr="$libPathStr\ngenBio=..${slashSC}genBio";
      fi
   # Else If: maskPrim library

   #++++++++++++++++++++++++++++++++++++++++++++++++++++++
   # Sec04 Sub04 Cat12:
   #   - trimSam
   #++++++++++++++++++++++++++++++++++++++++++++++++++++++

   elif [ "$libStr" = "trimSam" ]; then
   # Else If: trimSam library
      if [ "$trimSamBl" = "" ]; then
         cmdStr="$cmdStr$newLineStr$trimSamStr";
         objFilesStr="$objFilesStr \\\\\n$spaceStr";
         objFilesStr="${objFilesStr}${trimSamObj}";

         if [ $libCntSI -lt $mainCntSI ]; then
            mainCmdStr="$mainCmdStr \\
	$trimSamObj";
         fi

         trimSamBl=1;

         depStr="$depStr $trimSamDep";
      fi

      if [ "$genBioBl" -lt 1 ]; then
         genBioBl=1;
         libPathStr="$libPathStr\ngenBio=..${slashSC}genBio";
      fi
   # Else If: trimSam library

   #++++++++++++++++++++++++++++++++++++++++++++++++++++++
   # Sec04 Sub04 Cat13:
   #   - ampDepth
   #++++++++++++++++++++++++++++++++++++++++++++++++++++++

   elif [ "$libStr" = "ampDepth" ]; then
   # If: ampDepth library
      if [ "$ampDepthBl" = "" ]; then
         cmdStr="$cmdStr$newLineStr$ampDepthStr";
         objFilesStr="$objFilesStr \\\\\n$spaceStr";
         objFilesStr="${objFilesStr}${ampDepthObj}";

         if [ $libCntSI -lt $mainCntSI ]; then
            mainCmdStr="$mainCmdStr \\
	$ampDepthObj";
         fi

         ampDepthBl=1;
         depStr="$depStr $ampDepthDep";
      fi

      if [ "$genBioBl" -lt 1 ]; then
         genBioBl=1;
         libPathStr="$libPathStr\ngenBio=..${slashSC}genBio";
      fi
   # If: ampDepth library

   #++++++++++++++++++++++++++++++++++++++++++++++++++++++
   # Sec04 Sub04 Cat14:
   #   - tbCon
   #++++++++++++++++++++++++++++++++++++++++++++++++++++++

   elif [ "$libStr" = "tbCon" ]; then
   # If: tbCon library
      if [ "$tbConBl" = "" ]; then
         cmdStr="$cmdStr$newLineStr$tbConStr";
         objFilesStr="$objFilesStr \\\\\n$spaceStr";
         objFilesStr="${objFilesStr}${tbConObj}";

         if [ $libCntSI -lt $mainCntSI ]; then
            mainCmdStr="$mainCmdStr \\
	$tbConObj";
         fi

         tbConBl=1;

         depStr="$depStr $tbConDep";
      fi

      if [ "$genBioBl" -lt 1 ]; then
         genBioBl=1;
         libPathStr="$libPathStr\ngenBio=..${slashSC}genBio";
      fi
   # If: tbCon library

   #++++++++++++++++++++++++++++++++++++++++++++++++++++++
   # Sec04 Sub04 Cat15:
   #   - edDist
   #++++++++++++++++++++++++++++++++++++++++++++++++++++++

   elif [ "$libStr" = "edDist" ]; then
   # If: edDist library
      if [ "$edDistBl" = "" ]; then
         cmdStr="$cmdStr$newLineStr$edDistStr";
         objFilesStr="$objFilesStr \\\\\n$spaceStr";
         objFilesStr="${objFilesStr}${edDistObj}";

         if [ $libCntSI -lt $mainCntSI ]; then
            mainCmdStr="$mainCmdStr \\
	$edDistObj";
         fi

         edDistBl=1;
         depStr="$depStr $edDistDep";
      fi

      if [ "$genBioBl" -lt 1 ]; then
         genBioBl=1;
         libPathStr="$libPathStr\ngenBio=..${slashSC}genBio";
      fi
   # If: edDist library

   #++++++++++++++++++++++++++++++++++++++++++++++++++++++
   # Sec04 Sub04 Cat16:
   #   - rmHomo
   #++++++++++++++++++++++++++++++++++++++++++++++++++++++

   elif [ "$libStr" = "rmHomo" ]; then
   # If: rmHomo library
      if [ "$rmHomoBl" = "" ]; then
         cmdStr="$cmdStr$newLineStr$rmHomoStr";
         objFilesStr="$objFilesStr \\\\\n$spaceStr";
         objFilesStr="${objFilesStr}${rmHomoObj}";

         if [ $libCntSI -lt $mainCntSI ]; then
            mainCmdStr="$mainCmdStr \\
	$rmHomoObj";
         fi

         rmHomoBl=1;
         depStr="$depStr $rmHomoDep";
      fi

      if [ "$genBioBl" -lt 1 ]; then
         genBioBl=1;
         libPathStr="$libPathStr\ngenBio=..${slashSC}genBio";
      fi
   # If: rmHomo library

   #******************************************************
   # Sec04 Sub05:
   #   - genAln libraries
   #   o sec04 sub05 cat01:
   #     - indexToCoord
   #   o sec04 sub05 cat02:
   #     - alnSet
   #   o sec04 sub05 cat03:
   #     - dirMatrix
   #   o sec04 sub05 cat04:
   #     - water
   #   o sec04 sub05 cat05:
   #     - needle
   #   o sec04 sub05 cat06:
   #     - memwater
   #   o sec04 sub05 cat07:
   #     - kmerFind
   #   o sec04 sub05 cat08:
   #     - samToAln
   #******************************************************

   #++++++++++++++++++++++++++++++++++++++++++++++++++++++
   # Sec04 Sub05 Cat01:
   #   - indexToCoord
   #++++++++++++++++++++++++++++++++++++++++++++++++++++++

   elif [ "$libStr" = "indexToCoord" ]; then
   # Else If: indexToCoord library
      if [ "$indexToCoordBl" = "" ]; then
        cmdStr="$cmdStr$newLineStr$indexToCoordStr";
        objFilesStr="$objFilesStr \\\\\n$spaceStr";
        objFilesStr="${objFilesStr}${indexToCoordObj}";

         if [ $libCntSI -lt $mainCntSI ]; then
           mainCmdStr="$mainCmdStr \\
	$indexToCoordObj";
        fi

        indexToCoordBl=1;
        depStr="$depStr $indexToCoordDep";
      fi

      if [ "$genAlnBl" -lt 1 ]; then
         genAlnBl=1;
         libPathStr="$libPathStr\ngenAln=..${slashSC}genAln";
      fi
   # Else If: indexToCoord library

   #++++++++++++++++++++++++++++++++++++++++++++++++++++++
   # Sec04 Sub05 Cat02:
   #   - alnSet
   #++++++++++++++++++++++++++++++++++++++++++++++++++++++

   elif [ "$libStr" = "alnSet" ]; then
   # Else If: alnSet library
      if [ "$alnSetBl" = "" ]; then
         cmdStr="$cmdStr$newLineStr$alnSetStr";
         objFilesStr="$objFilesStr \\\\\n$spaceStr";
         objFilesStr="${objFilesStr}${alnSetObj}";

         if [ $libCntSI -lt $mainCntSI ]; then
            mainCmdStr="$mainCmdStr \\
	$alnSetObj";
         fi

         alnSetBl=1;

         depStr="$depStr $alnSetDep";
      fi

      if [ "$genAlnBl" -lt 1 ]; then
         genAlnBl=1;
         libPathStr="$libPathStr\ngenAln=..${slashSC}genAln";
      fi
   # Else If: alnSet library

   #++++++++++++++++++++++++++++++++++++++++++++++++++++++
   # Sec04 Sub05 Cat03:
   #   - dirMatrix
   #++++++++++++++++++++++++++++++++++++++++++++++++++++++

   elif [ "$libStr" = "dirMatrix" ]; then
   # Else If: dirMatrix library
      if [ "$dirMatrixBl" = "" ]; then
         cmdStr="$cmdStr$newLineStr$dirMatrixStr";
         objFilesStr="$objFilesStr \\\\\n$spaceStr";
         objFilesStr="${objFilesStr}${dirMatrixObj}";

         if [ $libCntSI -lt $mainCntSI ]; then
            mainCmdStr="$mainCmdStr \\
	$dirMatrixObj";
         fi

         dirMatrixBl=1;
         depStr="$depStr $dirMatrixDep";
      fi

      if [ "$genAlnBl" -lt 1 ]; then
         genAlnBl=1;
         libPathStr="$libPathStr\ngenAln=..${slashSC}genAln";
      fi
   # Else If: dirMatrix library

   #++++++++++++++++++++++++++++++++++++++++++++++++++++++
   # Sec04 Sub05 Cat04:
   #   - water
   #++++++++++++++++++++++++++++++++++++++++++++++++++++++

   elif [ "$libStr" = "water" ]; then
   # Else If: water library
      if [ "$waterBl" = "" ]; then
         cmdStr="$cmdStr$newLineStr$waterStr";
         objFilesStr="$objFilesStr \\\\\n$spaceStr";
         objFilesStr="${objFilesStr}${waterObj}";

         if [ $libCntSI -lt $mainCntSI ]; then
            mainCmdStr="$mainCmdStr \\
	$waterObj";
         fi

         waterBl=1;

         depStr="$depStr $waterDep";
      fi

      if [ "$genAlnBl" -lt 1 ]; then
         genAlnBl=1;
         libPathStr="$libPathStr\ngenAln=..${slashSC}genAln";
      fi
   # Else If: water library

   #++++++++++++++++++++++++++++++++++++++++++++++++++++++
   # Sec04 Sub05 Cat05:
   #   - needle
   #++++++++++++++++++++++++++++++++++++++++++++++++++++++

   elif [ "$libStr" = "needle" ]; then
   # Else If: needle library
      if [ "$needleBl" = "" ]; then
         cmdStr="$cmdStr$newLineStr$needleStr";
         objFilesStr="$objFilesStr \\\\\n$spaceStr";
         objFilesStr="${objFilesStr}${needleObj}";

         if [ $libCntSI -lt $mainCntSI ]; then
            mainCmdStr="$mainCmdStr \\
	$needleObj";
         fi

         needleBl=1;
         depStr="$depStr $needleDep";
      fi

      if [ "$genAlnBl" -lt 1 ]; then
         genAlnBl=1;
         libPathStr="$libPathStr\ngenAln=..${slashSC}genAln";
      fi
   # Else If: needle library

   #++++++++++++++++++++++++++++++++++++++++++++++++++++++
   # Sec04 Sub05 Cat06:
   #   - memwater
   #++++++++++++++++++++++++++++++++++++++++++++++++++++++

   elif [ "$libStr" = "memwater" ]; then
   # Else If: memwater library
      if [ "$memwaterBl" = "" ]; then
         cmdStr="$cmdStr$newLineStr$memwaterStr";
         objFilesStr="$objFilesStr \\\\\n$spaceStr";
         objFilesStr="${objFilesStr}${memwaterObj}";

         if [ $libCntSI -lt $mainCntSI ]; then
            mainCmdStr="$mainCmdStr \\
	$memwaterObj";
         fi

         memwaterBl=1;
         depStr="$depStr $memwaterDep";
      fi

      if [ "$genAlnBl" -lt 1 ]; then
         genAlnBl=1;
         libPathStr="$libPathStr\ngenAln=..${slashSC}genAln";
      fi
   # Else If: memwater library

   #++++++++++++++++++++++++++++++++++++++++++++++++++++++
   # Sec04 Sub05 Cat07:
   #   - kmerFind
   #++++++++++++++++++++++++++++++++++++++++++++++++++++++

   elif [ "$libStr" = "kmerFind" ]; then
   # Else If: kmerFind library
      if [ "$kmerFindBl" = "" ]; then
         cmdStr="$cmdStr$newLineStr$kmerFindStr";
         objFilesStr="$objFilesStr \\\\\n$spaceStr";
         objFilesStr="${objFilesStr}${kmerFindObj}";

         if [ $libCntSI -lt $mainCntSI ]; then
            mainCmdStr="$mainCmdStr \\
	$kmerFindObj";
         fi

         kmerFindBl=1;

         depStr="$depStr $kmerFindDep";
      fi

      if [ "$genAlnBl" -lt 1 ]; then
         genAlnBl=1;
         libPathStr="$libPathStr\ngenAln=..${slashSC}genAln";
      fi
   # Else If: kmerFin library

   #++++++++++++++++++++++++++++++++++++++++++++++++++++++
   # Sec04 Sub05 Cat08:
   #   - samToAln
   #++++++++++++++++++++++++++++++++++++++++++++++++++++++

   elif [ "$libStr" = "samToAln" ]; then
   # Else If: samToAln library
      if [ "$samToAlnBl" = "" ]; then
         cmdStr="$cmdStr$newLineStr$samToAlnStr";
         objFilesStr="$objFilesStr \\\\\n$spaceStr";
         objFilesStr="${objFilesStr}${samToAlnObj}";

         if [ $libCntSI -lt $mainCntSI ]; then
            mainCmdStr="$mainCmdStr \\
	$samToAlnObj";
         fi

         samToAlnBl=1;
         depStr="$depStr $samToAlnDep";
      fi

      if [ "$genAlnBl" -lt 1 ]; then
         genAlnBl=1;
         libPathStr="$libPathStr\ngenAln=..${slashSC}genAln";
      fi
   # Else If: samToAln library

   #++++++++++++++++++++++++++++++++++++++++++++++++++++++
   # Sec04 Sub05 Cat09:
   #   - mapRead
   #++++++++++++++++++++++++++++++++++++++++++++++++++++++

   elif [ "$libStr" = "mapRead" ]; then
   # Else If: mapRead library
      if [ "$mapReadBl" = "" ]; then
         cmdStr="$cmdStr$newLineStr$mapReadStr";
         objFilesStr="$objFilesStr \\\\\n$spaceStr";
         objFilesStr="${objFilesStr}${mapReadObj}";

         if [ $libCntSI -lt $mainCntSI ]; then
            mainCmdStr="$mainCmdStr \\
	$mapReadObj";
         fi

         mapReadBl=1;

         depStr="$depStr $mapReadDep";
      fi

      if [ "$genClustBl" -lt 1 ]; then
         genClustBl=1;
         libPathStr="$libPathStr\ngenClust=..${slashSC}genClust";
      fi
   # Else If: mapRead library

   #******************************************************
   # Sec04 Sub06:
   #   - genAln libraries
   #   o sec04 sub05 cat01:
   #     - illNano
   #   o sec04 sub05 cat02:
   #     - clustST
   #   o sec04 sub05 cat03:
   #     - edClust
   #******************************************************

   #++++++++++++++++++++++++++++++++++++++++++++++++++++++
   # Sec04 Sub06 Cat01:
   #   - illNano
   #++++++++++++++++++++++++++++++++++++++++++++++++++++++

   elif [ "$libStr" = "illNano" ]; then
   # Else If: illNano library
      if [ "$illNanoBl" = "" ]; then
         cmdStr="$cmdStr$newLineStr$illNanoStr";
         objFilesStr="$objFilesStr \\\\\n$spaceStr";
         objFilesStr="${objFilesStr}${illNanoObj}";

         if [ $libCntSI -lt $mainCntSI ]; then
            mainCmdStr="$mainCmdStr \\
	$illNanoObj";
         fi

         illNanoBl=1;
         depStr="$depStr $illNanoDep";
      fi

      if [ "$genClustBl" -lt 1 ]; then
         genClustBl=1;
         libPathStr="$libPathStr\ngenClust=..${slashSC}genClust";
      fi
   # Else If: illNano library

   #++++++++++++++++++++++++++++++++++++++++++++++++++++++
   # Sec04 Sub06 Cat02:
   #   - clustST
   #++++++++++++++++++++++++++++++++++++++++++++++++++++++

   elif [ "$libStr" = "clustST" ]; then
   # Else If: clustST library
      if [ "$clustSTBl" = "" ]; then
         cmdStr="$cmdStr$newLineStr$clustSTStr";
         objFilesStr="$objFilesStr \\\\\n$spaceStr";
         objFilesStr="${objFilesStr}${clustSTObj}";

         if [ $libCntSI -lt $mainCntSI ]; then
            mainCmdStr="$mainCmdStr \\
	$clustSTObj";
         fi

         clustSTBl=1;
         depStr="$depStr $clustSTDep";
      fi

      if [ "$genClustBl" -lt 1 ]; then
         genClustBl=1;
         libPathStr="$libPathStr\ngenClust=..${slashSC}genClust";
      fi
   # Else If: clustST library

   #++++++++++++++++++++++++++++++++++++++++++++++++++++++
   # Sec04 Sub06 Cat03:
   #   - edClust
   #++++++++++++++++++++++++++++++++++++++++++++++++++++++

   elif [ "$libStr" = "edClust" ]; then
   # Else If: edClust library
      if [ "$edClustBl" = "" ]; then
         cmdStr="$cmdStr$newLineStr$edClustStr";
         objFilesStr="$objFilesStr \\\\\n$spaceStr";
         objFilesStr="${objFilesStr}${edClustObj}";

         if [ $libCntSI -lt $mainCntSI ]; then
            mainCmdStr="$mainCmdStr \\
	$edClustObj";
         fi

         edClustBl=1;
         depStr="$depStr $edClustDep";
      fi

      if [ "$genClustBl" -lt 1 ]; then
         genClustBl=1;
         libPathStr="$libPathStr\ngenClust=..${slashSC}genClust";
      fi
   # Else If: edClust library

   #******************************************************
   # Sec04 Sub07:
   #   - genGenoType library files
   #   o sec04 sub07 cat01:
   #     - cgMLST
   #******************************************************

   #++++++++++++++++++++++++++++++++++++++++++++++++++++++
   # Sec04 Sub07 Cat01:
   #   - cgMLST
   #++++++++++++++++++++++++++++++++++++++++++++++++++++++

   elif [ "$libStr" = "cgMLST" ]; then
   # Else If: cgMLST program
      if [ "$cgMLSTBl" = "" ]; then
         cmdStr="$cmdStr$newLineStr$cgMLSTStr";
         objFilesStr="$objFilesStr \\\\\n$spaceStr";
         objFilesStr="${objFilesStr}${cgMLSTObj}";

         if [ $libCntSI -lt $mainCntSI ]; then
            mainCmdStr="$mainCmdStr \\
	$cgMLSTObj";
         fi

         cgMLSTBl=1;
         depStr="$depStr $cgMLSTDep";

         if [ "$genGenoTypeBl" -lt 1 ]; then
            genGenoTypeBl=1;
            libPathStr="$libPathStr\ngenGenoType=..${slashSC}genGenoType";
         fi
      fi
   # Else If: cgMLST program

   #******************************************************
   # Sec04 Sub08:
   #   - non-general library files
   #   o sec04 sub08 cat01:
   #     - k2TaxaId
   #******************************************************

   #++++++++++++++++++++++++++++++++++++++++++++++++++++++
   # Sec04 Sub08 Cat01:
   #   - k2TaxaId
   #++++++++++++++++++++++++++++++++++++++++++++++++++++++

   elif [ "$libStr" = "k2TaxaId" ]; then
   # Else If: k2TaxaId program
      if [ "$k2TaxaIdBl" = "" ]; then
         cmdStr="${cmdStr}${newLineStr}${k2TaxaIdStr}";
         objFilesStr="$objFilesStr \\\\\n$spaceStr";
         objFilesStr="${objFilesStr}$k2TaxaIdObj";

         if [ $libCntSI -lt $mainCntSI ]; then
            mainCmdStr="$mainCmdStr \\
	$k2TaxaIdObj";
         fi

         k2TaxaIdBl=1;
         depStr="$depStr $k2TaxaIdDep";
      fi
   # Else If: k2TaxaId program


   fi # check librarys called

   #******************************************************
   # Sec04 Sub08:
   #   - move to next library or dependency
   #******************************************************

   libCntSI="$((libCntSI + 1))";
   libStr="${depStr%% *}";
      # get first argument
   depStr="${depStr#"${depStr%%[[:space:]]*}"}";
      # pop off first arugment
   depStr="${depStr#"${depStr%%[![:space:]]*}"}";
      # clear leading spaces

   if [ "$libStr" = "" ];
   then # If: no lib in path
      if [ "$depStr" = "" ];
      then # Check if have more dependencies
         break; # finished
      else
         libStr="${depStr%% *}"; # reget argument
      fi # Check if have more dependencies
       
   fi # If: no lib in path
done

#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# Sec05:
#   - print mkfile (makefile)
#<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

# need to add in main file to object files list
objFilesStr="$objFilesStr \\\\\n$spaceStr$mainFileStr.\$O";

# need to complete compiler part of main file command
mainCmdStr="$mainCmdStr
		$ccStr ${dashOStr}$mainFileStr.\$O \\
			$cFlagsStr $coreFlagsStr $mainFileStr.c
"

{
   printf "%b\n" "$headStr"
   printf "%b\n\n" "$libPathStr"
   printf "%b\n\n" "$objFilesStr"
   printf "%b\n\n" "$linkStr"
   printf "%b\n" "$mainCmdStr" # cmdStr has newlines
   printf "%b\n\n" "$cmdStr"
   printf "%b\n\n" "$installStr"
   printf "%b\n" "$cleanStr"
} # print out make file
