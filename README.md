# Battle Tanks

This repo has been adapted from the CSC 340 course project at The South Dakota 
School of Mines and Technology for educational purposes. In this repo, there is
a project called "projectx". This is a platform that supports multiple tanks
battling each other on a field. Developers can write and program tanks to play
on this field. In addition, the Squad7++ folder contains a tank, called 
Edelweiss, written for the course described above. The purpose of this project 
is to implement this tank using a neural network for educational purposes.

Getting started

See "Development Setup" in the wiki for further information on how to setup the
development environment and what is all required for the platform to run.

To compile the Edelweiss tank, cd into Squad7++ and type 'make'. It will compile
a .so file named "Edelweiss.so" and put it into a folder named "tanks" found in
the same direcetory. 

To run the platform from the Squad7++ directory, simply run the script 
"start.sh". This will use the current config file found in the projectx 
directory. To change the configFile or the current map, you can look in the 
"configFiles" folder for one you want to use and simply copy that version over
the config file found in the projectx directory. For example from the Squad7++ 
base directory, you can type the following:
    $cp configFiles/<configFileName> ../projectx/config.txt
    
Alternatively you can manually change config.txt found in the projectx folder 
to what you want. For more information, see "Config file rules" for more 
information.

From within the resting folder, the script "runVisualTests.sh" will run a set of
visual tests. The script "newTest.sh" will generate and run the platform using 
5 randomly generated configuration files.

