#!/bin/sh

tag="all"

mkdir -p ./root/NEB/${tag}

irun=07

#for irun in {03..04}
#do

root -b -q './macros/17Bnn/AnaNEBTree.cc++('$irun',"'${tag}'")' 

#done
wait
####################################
