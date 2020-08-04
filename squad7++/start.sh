#!/bin/bash
#This script wil move the .so file into the projectx directory,` and then will start the platform

rm ../projectx/tanks/Edelweiss.so
cp tanks/Edelweiss.so ../projectx/tanks/Edelweiss.so

cd ../projectx

cp ../squad7++/configFiles/mazeWithTarget.txt config.txt
./platform

cp ../squad7++/configFiles/smallMapWithTargets.txt config.txt
./platform

cp ../squad7++/configFiles/smallMapMouse.txt config.txt
./platform

cp ../squad7++/configFiles/smallMapSentry.txt config.txt
./platform

cp ../squad7++/configFiles/smallMapCat.txt config.txt
./platform

cp ../squad7++/configFiles/smallMapTurret.txt config.txt
./platform

cp configFiles/config.txt ../projectx/config.txt
./platform
