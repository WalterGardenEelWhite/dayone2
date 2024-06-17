#!/bin/sh

tag="all"

mkdir -p ./root/Plot/${tag}
irun=07
#for irun in {05..06}
#do
root -b -q './macros/17Bnn/PlotNEBTree.cc++('$irun',"'${tag}'")' 
#done
####################################
