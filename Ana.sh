#!/bin/sh

tag="all"

mkdir -p ./root/Ana/${tag}
irun=07
#for irun in {02..04}; do
root -b -q './macros/17Bnn/AnaOutput_17Bnn.cc++('$irun',"'${tag}'")'
#done

wait

echo 'Simulation output analyzed in a range of 0 to '$irun' MeV'
