#!/bin/bash
# run the executable

project_name="cell_cycle_sim"

if [ "$1" = "release" ]; then
    cd out/build/release && ./"${project_name}"
else
    cd out/build/debug && ./"${project_name}"
fi
