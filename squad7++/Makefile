CXX = g++
CXXFLAGS = -g -std=c++11 -fPIC
INCS = -Isrc/
LIBS = -ldl
#LIBS += -lCTF -Llibraries
SOFLAGS = -DDYNAMIC

SRC_PATH= src/
TANK_PATH= tanks/
LIB_PATH= libraries/

TANKS = Edelweiss/Edelweiss.so

TANKS_LINK = src/Actor.o #need to link in the base class for the .so to have everything.
TANKS_LINK += Edelweiss/canHitAnythingFunctions.cpp
TANKS_LINK += Edelweiss/situationalAwarnessFunctions.cpp
TANKS_LINK += Edelweiss/moveFunctions.cpp

%.so: %.cpp
	$(CXX) $(CXXFLAGS) -shared $< $(TANKS_LINK) -o $@ $(SOFLAGS) $(LIBS)

%.h.gch: %.h
	$(CXX) -x c++-header -c $< -o $@ $(INCS) $(LIBS)
	
tanks:	$(TANKS)
	mkdir -p $(TANK_PATH)
	mv $^ $(TANK_PATH)

test:
	g++ -std=c++11 Edelweiss/Edelweiss.cpp Edelweiss/Move.cpp Edelweiss/canHitAnythingFunctions.cpp Edelweiss/moveFunctions.cpp Edelweiss/situationalAwarnessFunctions.cpp Edelweiss/src/Actor.o testing/testCatch.o testing/allTests.cpp testing/EdelweissTester.cpp testing/moveTesting/moveTester.cpp testing/moveTesting/moveTests.cpp -o catch

cleanTanks:
	rm -rf $(TANK_PATH)
