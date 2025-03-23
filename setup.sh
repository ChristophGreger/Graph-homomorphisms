#!/bin/sh

# Initialize and update all submodules
git submodule update --init --recursive

# Build nauty
cd external/nauty || { echo "external/nauty directory not found"; exit 1; }
./configure
make
cd ../..

# Build tamaki exact
cd external/tamaki || { echo "external/tamaki directory not found"; exit 1; }
make exact
cd ../..
