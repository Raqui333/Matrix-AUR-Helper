CXX = g++
CXXFLAGS = -std=c++17 -lstdc++fs
SRC = $(wildcard src/*.cpp)
OBJ = $(SRC:.cpp=.o)
BIN = matrix

all: $(BIN)

matrix: $(OBJ)
	$(CXX) $^ -o $@ $(CXXFLAGS)

.PHONY: clean

clean:
	@rm src/*.o 2> /dev/null
	@rm matrix 2> /dev/null
