#!/bin/sh

#i=5
#Eh=$(printf "%02d" $i)
for irun in {01..10}
do

outfile="./g4mac/vis15Bnn${irun}.mac"

rm -rf ${outfile}
touch ${outfile}

inputroot="/action/gun/tree/InputFileName root/Gen/15BnnInp${irun}.root"
outputroot="/action/file/RunName Sim15Bnn${irun}"

echo "/control/execute geometry/geom_dayone_D.mac" >> ${outfile}
echo "/samurai/geometry/Update" >> ${outfile}
echo "/action/file/OverWrite y" >> ${outfile}
echo $outputroot >> ${outfile}
echo "/action/file/SaveDirectory root/Sim" >> ${outfile}

echo "/action/gun/Type Tree" >> ${outfile}
echo $inputroot >> ${outfile} 
#echo "/tracking/verbose 1" >> ${outfile}
echo "/action/gun/tree/TreeName tree" >> ${outfile}
echo "/action/data/NEBULA/StoreSteps false" >> ${outfile}
echo "/action/data/NEBULA/Resolution false" >> ${outfile}
echo "/action/gun/tree/beamOn 1000000" >> ${outfile}
simdayone $outfile

done
####################################
