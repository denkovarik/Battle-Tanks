#!/bin/bash
#This script wil move the .so file into the projectx directory,` and then will start the platform

for i in 1 2 3 4 5
do
g++ genTestConfig.cpp -o gen
./gen
rm gen
cp testConfigs/testConfig.txt ../../projectx/config.txt
cd ../../projectx
rm tanks/Edelweiss.so
cp ../squad7++/tanks/Edelweiss.so tanks/Edelweiss.so
./platform
cd ../squad7++/testing
done
