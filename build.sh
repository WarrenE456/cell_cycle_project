#!/bin/bash
# run make file to create executable

if [ "$1" = "release" ]; then
   cd out/build/release ; make
else
   cd out/build/debug ; make
fi
