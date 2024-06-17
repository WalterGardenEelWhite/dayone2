#!/bin/sh

for irun in {1..10}; do
    root -b -q './macros/GenInput_15Bnn.cc++('$irun')'
done

#for irun in {1..10}
#do
#
#root -b -q '('$irun')'
#
#done

####################################
