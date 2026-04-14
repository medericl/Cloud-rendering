CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2 -fopenmp -Isrc/ui

SRC = main.cpp \
      $(wildcard src/vector/*.cpp) \
      $(wildcard src/image/*.cpp) \
      $(wildcard src/scene/*.cpp) \
      $(wildcard src/cloud/*.cpp) \
      $(wildcard src/ui/*.cpp)
OBJ = $(SRC:.cpp=.o)

main: $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) -o ray -lglfw -lGL

fast:
	$(CXX) $(CXXFLAGS) -DNO_UI main.cpp \
	  $(wildcard src/vector/*.cpp) \
	  $(wildcard src/image/*.cpp) \
	  $(wildcard src/scene/*.cpp) \
	  $(wildcard src/cloud/*.cpp) \
	  -o ray_fast -lglfw -lGL

clean:
	rm -f main.o src/vector/*.o src/image/*.o src/scene/*.o src/cloud/*.o src/ui/*.o ray ray_fast
