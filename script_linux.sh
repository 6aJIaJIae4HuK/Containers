#!/bin/sh

mkdir tmp
cd tmp
cmake ../
make
ctest -C Release -V -R test1
cd ../
rm -rf tmp

