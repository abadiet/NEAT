#!/bin/bash

cmake -B ./build/ -S ./
cd ./build/
make
cp ./libneat.a ../lib/libneat.a