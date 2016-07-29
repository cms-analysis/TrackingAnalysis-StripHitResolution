#!/bin/bash

g++ -I $ROOTSYS/include RunTrackerAnalysis.C `root-config --glibs` `root-config --libs` `root-config --cflags` -o RunTrackerAnalysis
