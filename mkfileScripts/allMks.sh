scriptDirStr="$(dirname "$0")";
cd "$scriptDirStr";
retDirStr="$(basename "$scriptDirStr" | sed 's/\.\///;')";

cd ../adjCoordsSrc;
bash "../$retDirStr/allOSMkfiles.sh"  adjCoordsSrc mainAdjCoords adjCoords charCp ../bioTools.h ../genLib/endLine.h
cd "../$retDirStr";

cd ../alnNeedleSrc;
bash "../$retDirStr/allOSMkfiles.sh" alnNeedle mainNeedle needle kmerCnt samEntry charCp base10str ../bioTools.h ../genLib/endLine.h ../genAln/alnDefs
cd "../$retDirStr";

cd ../alnWaterSrc;
bash "../$retDirStr/allOSMkfiles.sh" alnWater mainWater water kmerCnt samEntry charCp base10str ../bioTools.h ../genLib/endLine.h ../genAln/alnDefs
cd "../$retDirStr";

cd ../ampDepthSrc;
bash "../$retDirStr/allOSMkfiles.sh" ampDepth mainAmpDepth ampDepth trimSam charCp base10str ampDepthDefs.h ../bioTools.h ../genLib/endLine.h
cd "../$retDirStr";

cd ../cigToEqxSrc;
bash "../$retDirStr/allOSMkfiles.sh" cigToEqx mainCigToEqx cigToEqx seqST charCp ../bioTools.h ../genLib/endLine.h
cd "../$retDirStr";

cd ../edClustSrc;
bash "../$retDirStr/allOSMkfiles.sh" edClust mainEdClust edClust seqST charCp ../bioTools.h ../genLib/endLine.h ../genBio/tbConDefs.h
cd "../$retDirStr";

cd ../edDistSrc;
bash "../$retDirStr/allOSMkfiles.sh" edDist mainEdDist edDist charCp ../bioTools.h ../genLib/endLine.h ../genLib/genMath.h
cd "../$retDirStr";

cd ../filtsamSrc;
bash "../$retDirStr/allOSMkfiles.sh" filtsam mainFiltsam trimSam charCp ../bioTools.h ../genLib/endLine.h
cd "../$retDirStr";

cd ../illNanoSrc;
bash "../$retDirStr/allOSMkfiles.sh" illNano mainIllNano illNano ../bioTools.h ../genLib/endLine.h
cd "../$retDirStr";

cd ../k2TaxaIdSrc;
bash "../$retDirStr/allOSMkfiles.sh" k2TaxaId mainK2TaxaId k2TaxaId ../bioTools.h ../genLib/endLine.h
cd "../$retDirStr";

cd ../mapReadSrc;
bash "../$retDirStr/allOSMkfiles.sh" mapRead mainMapRead mapRead ptrAry ../bioTools.h ../genLib/endLine.h ../genAln/defsMapRead.h ../genAln/alnDefs.h
cd "../$retDirStr";

cd ../maskPrimSrc;
bash "../$retDirStr/allOSMkfiles.sh" maskPrim mainMaskPrim maskPrim charCp ../bioTools.h ../genLib/endLine.h
cd "../$retDirStr";

cd ../memwaterSrc;
bash "../$retDirStr/allOSMkfiles.sh" memwater mainMemwater memwater kmerCnt charCp ../bioTools.h ../genLib/endLine.h ../genAln/alnDefs.h
cd "../$retDirStr";

cd ../primFindSrc;
bash "../$retDirStr/allOSMkfiles.sh" primFind mainPrimFind kmerFind kmerCnt charCp ../bioTools.h ../genLib/endLine.h ../genAln/alnDefs.h
cd "../$retDirStr";

cd ../rmHomoSrc;
bash "../$retDirStr/allOSMkfiles.sh" rmHomo mainRmHomo rmHomo seqST ../bioTools.h ../genLib/endLine.h
cd "../$retDirStr";

cd ../samToAlnSrc;
bash "../$retDirStr/allOSMkfiles.sh" samToAln mainSamToAln samToAln charCp ../bioTools.h ../genLib/endLine.h
cd "../$retDirStr";

cd ../tbConSrc;
bash "../$retDirStr/allOSMkfiles.sh" tbCon mainTbCon tbCon charCp ../bioTools.h ../genLib/endLine.h ../genLib/genMath.h ../genBio/tbConDefs.h
cd "../$retDirStr";

cd ../tranSeqSrc;
bash "../$retDirStr/allOSMkfiles.sh" tranSeq tranSeq  seqST codonFun base10str ../bioTools.h ../genLib/endLine.h
cd "../$retDirStr";

cd ../trimSamSrc;
bash "../$retDirStr/allOSMkfiles.sh" trimSam mainTrimSam  trimSam ../bioTools.h ../genLib/endLine.h
cd "../$retDirStr";
