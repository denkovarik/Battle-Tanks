# Battle Tanks

## Description
Welcome to Battle-Tanks. This repo has been adapted from the CSC 340 
Indroduction to Software Engineering course 
project at The South Dakota School of Mines and Technology for educational 
purposes. In this repo, there is a project called "projectx". This is a 
platform that supports multiple tanks battling each other on a field. 
Developers can write and program tanks to play on this field. In addition, 
the Squad7++ directory contains a tank, called Edelweiss, written to run on 
the projectx platform for the course described above. 


## Getting Started
This guide will walk you through downloading and running the project. 
Note that this software was developed and tested in Ubuntu on the command 
line.

### Required Software
* g++
   * Linux C++ Compiler 
* freeglut3-dev
   * OpenGL - Development libraries
* SOIL
   * Image processing package
* make
   * Linux compiler script manager
* gdb
   * Linux debugger

### Optional Software for Windows Users
* Xming
   * X-server package for displaying platform on windows
   
### Clone the repository
  * Clone with SSH
    * git clone git@github.com:denkovarik/Battle-Tanks.git
  * Clone with HTTPS
    * git clone https://github.com/denkovarik/Battle-Tanks.git
    
### Build the Project
  * make
  
### Usage
  * This command will start a set of example runs for demonstration
    * ./start
    
  * Alternatively, you can start a single run by typing the following
    * ./projectx/platform
    
* You can modify the map the platform runs by modifying the projectx configuration file 'config.txt'. This file is located in the 'projectx' directory. 
* To have your tank run on the platform, it must be compiled as a .so file and located in the 'projectx/tanks' directory. In addition, you must add the name of your tank to config.txt for itto be loaded onto the platform. An example config file can be found in the projectx directory as a file named 'config.sample'.

