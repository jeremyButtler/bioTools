#!/usr/bin/sh

scriptDirStr="$(dirname "$0")";
cd "$scriptDirStr" || exit;
retDirStr="$(basename "$scriptDirStr" | sed 's/\.\///;')";

cd ../adjCoordsSrc || exit;
sh "../$retDirStr/allOSMkfiles.sh"  adjCoordsSrc mainAdjCoords adjCoords charCp ../bioTools.h ../genLib/endLine.h
cd "../$retDirStr" || exit;

cd ../alnNeedleSrc || exit;
sh "../$retDirStr/allOSMkfiles.sh" alnNeedle mainNeedle needle kmerCnt samEntry charCp base10str ../bioTools.h ../genLib/endLine.h ../genAln/alnDefs
cd "../$retDirStr" || exit;

cd ../alnWaterSrc || exit;
sh "../$retDirStr/allOSMkfiles.sh" alnWater mainWater water kmerCnt samEntry charCp base10str ../bioTools.h ../genLib/endLine.h ../genAln/alnDefs
cd "../$retDirStr" || exit;

cd ../ampDepthSrc || exit;
sh "../$retDirStr/allOSMkfiles.sh" ampDepth mainAmpDepth ampDepth trimSam charCp base10str ampDepthDefs.h ../bioTools.h ../genLib/endLine.h
cd "../$retDirStr" || exit;

cd ../cigToEqxSrc || exit;
sh "../$retDirStr/allOSMkfiles.sh" cigToEqx mainCigToEqx cigToEqx seqST charCp ../bioTools.h ../genLib/endLine.h
cd "../$retDirStr" || exit;

cd ../diCoordsSrc || exit;
sh "../$retDirStr/allOSMkfiles.sh" diCoords mainDICoords diCoords samEntry ../bioTools.h ../genLib/endLine.h
cd "../$retDirStr" || exit;

cd ../unGzSrc || exit;
sh "../$retDirStr/allOSMkfiles.sh" unGz unGz inflate ../bioTools.h
cd "../$retDirStr" || exit;

cd ../edClustSrc || exit;
sh "../$retDirStr/allOSMkfiles.sh" edClust mainEdClust edClust seqST charCp ../bioTools.h ../genLib/endLine.h ../genBio/tbConDefs.h
cd "../$retDirStr" || exit;

cd ../edDistSrc || exit;
sh "../$retDirStr/allOSMkfiles.sh" edDist mainEdDist edDist charCp ../bioTools.h ../genLib/endLine.h ../genLib/genMath.h
cd "../$retDirStr" || exit;

cd ../filtsamSrc || exit;
sh "../$retDirStr/allOSMkfiles.sh" filtsam mainFiltsam trimSam charCp ../bioTools.h ../genLib/endLine.h
cd "../$retDirStr" || exit;

cd ../hirschbergSrc || exit;
sh ../$retDirStr/allOSMkfiles.sh hirschberg mainHirschberg hirschberg seqST kmerCnt charCp ../bioTools.h ../genLib/endLine.h > mkfile.bug
cd "../$retDirStr" || exit;

cd ../illNanoSrc || exit;
sh "../$retDirStr/allOSMkfiles.sh" illNano mainIllNano illNano ../bioTools.h ../genLib/endLine.h
cd "../$retDirStr" || exit;

cd ../k2TaxaIdSrc || exit;
sh "../$retDirStr/allOSMkfiles.sh" k2TaxaId mainK2TaxaId k2TaxaId ../bioTools.h ../genLib/endLine.h
cd "../$retDirStr" || exit;

cd ../leftAlnSrc || exit;
sh "../$retDirStr/allOSMkfiles.sh" leftAln leftAln rmHomo seqST ../bioTools.h ../genLib/endLine.h
cd "../$retDirStr" || exit;

cd ../mapReadSrc || exit;
sh "../$retDirStr/allOSMkfiles.sh" mapRead mainMapRead mapRead rmHomo gzSeqST ../bioTools.h ../genLib/endLine.h
cd "../$retDirStr" || exit;

cd ../maskPrimSrc || exit;
sh "../$retDirStr/allOSMkfiles.sh" maskPrim mainMaskPrim maskPrim charCp ../bioTools.h ../genLib/endLine.h
cd "../$retDirStr" || exit;

cd ../memwaterSrc || exit;
sh "../$retDirStr/allOSMkfiles.sh" memwater mainMemwater memwater kmerCnt charCp ../bioTools.h ../genLib/endLine.h ../genAln/alnDefs.h
cd "../$retDirStr" || exit;

cd ../memwaterScanSrc || exit;
sh "../$retDirStr/allOSMkfiles.sh" memwaterScan mainMemwaterScan memwaterScan base10str ulCp ../bioTools.h
cd "../$retDirStr" || exit;

cd ../primFindSrc || exit;
sh "../$retDirStr/allOSMkfiles.sh" primFind mainPrimFind kmerFind kmerCnt charCp ../bioTools.h ../genLib/endLine.h ../genAln/alnDefs.h
cd "../$retDirStr" || exit;

cd ../revCmpSrc || exit;
sh "../$retDirStr/allOSMkfiles.sh" revCmp revCmp seqST ../bioTools.h ../genLib/endLine.h
cd "../$retDirStr" || exit;

cd ../rmHomoSrc || exit;
sh "../$retDirStr/allOSMkfiles.sh" rmHomo mainRmHomo rmHomo seqST ../bioTools.h ../genLib/endLine.h
cd "../$retDirStr" || exit;

cd ../samToAlnSrc || exit;
sh "../$retDirStr/allOSMkfiles.sh" samToAln mainSamToAln samToAln charCp ../bioTools.h ../genLib/endLine.h
cd "../$retDirStr" || exit;

cd ../seqStatsSrc || exit;
sh "../$retDirStr/allOSMkfiles.sh" seqStats mainSeqStats seqStats gzSeqST base10str ../bioTools.h
cd "../$retDirStr" || exit;

cd ../tbConSrc || exit;
sh "../$retDirStr/allOSMkfiles.sh" tbCon mainTbCon samRef tbCon charCp ../bioTools.h ../genLib/endLine.h ../genLib/genMath.h ../genBio/tbConDefs.h
cd "../$retDirStr" || exit;

cd ../tranSeqSrc || exit;
sh "../$retDirStr/allOSMkfiles.sh" tranSeq tranSeq  seqST codonFun base10str ../bioTools.h ../genLib/endLine.h
cd "../$retDirStr" || exit;

cd ../trimSamSrc || exit;
sh "../$retDirStr/allOSMkfiles.sh" trimSam mainTrimSam  trimSam ../bioTools.h ../genLib/endLine.h
cd "../$retDirStr" || exit;
