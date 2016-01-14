#!/bin/sh
#export LD_PRELOAD=${LD_PRELOAD}:./Output/release/libDebugMalloc.so
export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:./Output/debug
./Output/debug/Testcase
