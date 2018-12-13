#!/bin/bash
#This script wil move the .so file into the projectx directory,` and then will start the platform


cd ../../projectx
cp ../squad7++/tanks/Edelweiss.so tanks/Edelweiss.so
cp ../squad7++/configFiles/ramBug.txt config.txt
./platform

cp ../squad7++/configFiles/configLoss7.txt config.txt
./platform

cp ../squad7++/configFiles/configLoss6.txt config.txt
./platform

cp ../squad7++/configFiles/configLoss5.txt config.txt
./platform

cp ../squad7++/configFiles/configLoss4.txt config.txt
./platform

cp ../squad7++/configFiles/configLoss3.txt config.txt
./platform

cp ../squad7++/configFiles/configLoss2.txt config.txt
./platform

cp ../squad7++/configFiles/configLoss1.txt config.txt
./platform

cp ../squad7++/configFiles/configLoss0.txt config.txt
./platform

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
