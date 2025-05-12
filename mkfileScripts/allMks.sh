#!/usr/bin/sh

scriptDirStr="$(dirname "$0")";
cd "$scriptDirStr" || exit;
retDirStr="$(basename "$scriptDirStr" | sed 's/\.\///;')";

cd ../adjCoordsSrc || exit;
bash "../$retDirStr/allOSMkfiles.sh"  adjCoordsSrc mainAdjCoords adjCoords charCp ../bioTools.h ../genLib/endLine.h
cd "../$retDirStr" || exit;

cd ../alnNeedleSrc || exit;
bash "../$retDirStr/allOSMkfiles.sh" alnNeedle mainNeedle needle kmerCnt samEntry charCp base10str ../bioTools.h ../genLib/endLine.h ../genAln/alnDefs
cd "../$retDirStr" || exit;

cd ../alnWaterSrc || exit;
bash "../$retDirStr/allOSMkfiles.sh" alnWater mainWater water kmerCnt samEntry charCp base10str ../bioTools.h ../genLib/endLine.h ../genAln/alnDefs
cd "../$retDirStr" || exit;

cd ../ampDepthSrc || exit;
bash "../$retDirStr/allOSMkfiles.sh" ampDepth mainAmpDepth ampDepth trimSam charCp base10str ampDepthDefs.h ../bioTools.h ../genLib/endLine.h
cd "../$retDirStr" || exit;

cd ../cigToEqxSrc || exit;
bash "../$retDirStr/allOSMkfiles.sh" cigToEqx mainCigToEqx cigToEqx seqST charCp ../bioTools.h ../genLib/endLine.h
cd "../$retDirStr" || exit;

cd ../unGzSrc || exit;
bash "../$retDirStr/allOSMkfiles.sh" unGz unGz inflate ../bioTools.h
cd "../$retDirStr" || exit;

cd ../edClustSrc || exit;
bash "../$retDirStr/allOSMkfiles.sh" edClust mainEdClust edClust seqST charCp ../bioTools.h ../genLib/endLine.h ../genBio/tbConDefs.h
cd "../$retDirStr" || exit;

cd ../edDistSrc || exit;
bash "../$retDirStr/allOSMkfiles.sh" edDist mainEdDist edDist charCp ../bioTools.h ../genLib/endLine.h ../genLib/genMath.h
cd "../$retDirStr" || exit;

cd ../filtsamSrc || exit;
bash "../$retDirStr/allOSMkfiles.sh" filtsam mainFiltsam trimSam charCp ../bioTools.h ../genLib/endLine.h
cd "../$retDirStr" || exit;

cd ../illNanoSrc || exit;
bash "../$retDirStr/allOSMkfiles.sh" illNano mainIllNano illNano ../bioTools.h ../genLib/endLine.h
cd "../$retDirStr" || exit;

cd ../k2TaxaIdSrc || exit;
bash "../$retDirStr/allOSMkfiles.sh" k2TaxaId mainK2TaxaId k2TaxaId ../bioTools.h ../genLib/endLine.h
cd "../$retDirStr" || exit;

cd ../mapReadSrc || exit;
bash "../$retDirStr/allOSMkfiles.sh" mapRead mainMapRead mapRead gzSeqST ../bioTools.h
cd "../$retDirStr" || exit;

cd ../maskPrimSrc || exit;
bash "../$retDirStr/allOSMkfiles.sh" maskPrim mainMaskPrim maskPrim charCp ../bioTools.h ../genLib/endLine.h
cd "../$retDirStr" || exit;

cd ../memwaterSrc || exit;
bash "../$retDirStr/allOSMkfiles.sh" memwater mainMemwater memwater kmerCnt charCp ../bioTools.h ../genLib/endLine.h ../genAln/alnDefs.h
cd "../$retDirStr" || exit;

cd ../primFindSrc || exit;
bash "../$retDirStr/allOSMkfiles.sh" primFind mainPrimFind kmerFind kmerCnt charCp ../bioTools.h ../genLib/endLine.h ../genAln/alnDefs.h
cd "../$retDirStr" || exit;

cd ../revCmpSrc || exit;
bash "../$retDirStr/allOSMkfiles.sh" revCmp revCmp seqST ../bioTools.h ../genLib/endLine.h
cd "../$retDirStr" || exit;

cd ../rmHomoSrc || exit;
bash "../$retDirStr/allOSMkfiles.sh" rmHomo mainRmHomo rmHomo seqST ../bioTools.h ../genLib/endLine.h
cd "../$retDirStr" || exit;

cd ../samToAlnSrc || exit;
bash "../$retDirStr/allOSMkfiles.sh" samToAln mainSamToAln samToAln charCp ../bioTools.h ../genLib/endLine.h
cd "../$retDirStr" || exit;

cd ../tbConSrc || exit;
bash "../$retDirStr/allOSMkfiles.sh" tbCon mainTbCon samRef tbCon charCp ../bioTools.h ../genLib/endLine.h ../genLib/genMath.h ../genBio/tbConDefs.h
cd "../$retDirStr" || exit;

cd ../tranSeqSrc || exit;
bash "../$retDirStr/allOSMkfiles.sh" tranSeq tranSeq  seqST codonFun base10str ../bioTools.h ../genLib/endLine.h
cd "../$retDirStr" || exit;

cd ../trimSamSrc || exit;
bash "../$retDirStr/allOSMkfiles.sh" trimSam mainTrimSam  trimSam ../bioTools.h ../genLib/endLine.h
cd "../$retDirStr" || exit;
