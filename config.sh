#!/bin/bash
# runs cmake and puts the make file in out/build

if [ "$1" = "release" ]; then
   cmake -DCMAKE_BUILD_TYPE=Release -B out/build/release
else
    cmake -DCMAKE_BUILD_TYPE=Debug -B out/build/debug
fi
