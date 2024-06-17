#!/bin/sh

for irun in {1..10}

do
root -b -q './macros/AnaOutput_15Bnn.cc++('$irun')'
done

####################################
