#!/bin/sh

for irun in {1..10}
do

root -b -q './macros/AnaNEBTree.cc++('$irun')' 

done
wait
####################################
