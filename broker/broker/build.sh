#!/bin/bash

cmake -B build -DCMAKE_EXPORT_COMPILE_COMMANDS=YES

rm -rf ./compile_commands.json

ln -s ./build/compile_commands.json ./compile_commands.json

cd ./build
make
cd ..
