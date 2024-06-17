#!/bin/sh

#for irun in {01..03}
#for irun in {04..05}
#for irun in {06..07}
#do

irun=07
tag="all"
#mkdir -p ./g4mac/${tag} #./root/Sim/${tag}
outfile="./g4mac/${tag}/vis17Bnn${irun}_${tag}.mac"

rm -rf ${outfile}
touch ${outfile}

inputroot="/action/gun/tree/InputFileName root/Gen/${tag}/17BnnInp${irun}_${tag}.root"
outputroot="/action/file/RunName Sim17Bnn${irun}_${tag}"

echo "/control/execute geometry/geom_dayone_D.mac" >> ${outfile}
echo "/samurai/geometry/Update" >> ${outfile}
echo "/action/file/OverWrite y" >> ${outfile}
echo $outputroot >> ${outfile}
echo "/action/file/SaveDirectory root/Sim/${tag}" >> ${outfile}

echo "/action/gun/Type Tree" >> ${outfile}
echo $inputroot >> ${outfile} 
#echo "/tracking/verbose 1" >> ${outfile}
echo "/action/gun/tree/TreeName tree" >> ${outfile}
echo "/action/data/NEBULA/StoreSteps false" >> ${outfile}
echo "/action/data/NEBULA/Resolution false" >> ${outfile}
echo "/action/gun/tree/beamOn 10000000" >> ${outfile}
simdayone $outfile

#done
####################################
