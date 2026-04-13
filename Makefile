CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2 -fopenmp -Iui

SRC = $(wildcard *.cpp) $(wildcard ui/*.cpp)
OBJ = $(SRC:.cpp=.o)

main: $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) -o ray -lglfw -lGL

fast: $(wildcard *.cpp)
	$(CXX) $(CXXFLAGS) -DNO_UI $(wildcard *.cpp) -o ray_fast -lglfw -lGL

clean:
	rm -f *.o ui/*.o ray ray_fast
