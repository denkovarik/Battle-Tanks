#!/bin/bash
#This script wil move the .so file into the projectx directory,` and then will start the platform

for i in 1 2 3 4 5 6 7 8 9 
do
cp testConfigs/testConfig.txt ../../projectx/config.txt
cd ../../projectx
rm tanks/Edelweiss.so
cp ../squad-7/tanks/Edelweiss.so tanks/Edelweiss.so
./platform
cd ../squad-7/testing
done
