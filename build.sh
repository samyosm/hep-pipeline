#!/bin/bash

rm -rf ./build
cmake -B build -S . -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_FLAGS="-march=native"
cmake --build build --parallel


