#!/bin/bash

# You can just as well change to clang++ if you'd like
cmake -B build -DCMAKE_EXPORT_COMPILE_COMMANDS=YES -DCMAKE_CXX_COMPILER=g++

rm -rf ./compile_commands.json

ln -s ./build/compile_commands.json .

cd ./build
make
cd ..
