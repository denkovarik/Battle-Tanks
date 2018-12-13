#!/bin/bash
#This script wil move the .so file into the projectx directory,` and then will start the platform

rm ../projectx/tanks/Edelweiss.so
cp tanks/Edelweiss.so ../projectx/tanks/Edelweiss.so

cd ../projectx

./platform
