CXX = g++
CXXFLAGS = -std=c++17 -lstdc++fs
SRC = $(wildcard src/*.cpp)
OBJ = $(SRC:.cpp=.o)
BIN = matrix
PWD = $(shell pwd)

all: $(BIN)

matrix: $(OBJ)
	$(CXX) $^ -o $@ $(CXXFLAGS)

.PHONY: all matrix clean install

install:
	ln -sf $(PWD)/matrix /usr/local/bin
	mkdir -p /usr/share/zsh/site-functions
	ln -sf $(PWD)/completions/zsh/_matrix /usr/share/zsh/site-functions

clean:
	@rm src/*.o 2> /dev/null
	@rm matrix 2> /dev/null
