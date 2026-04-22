# Simple Makefile for Minesweeper Game
# Compiles 5 source files without CMake

CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Iinclude
SOURCES = src/main.cpp src/cell.cpp src/board.cpp src/game.cpp src/display.cpp
OBJECTS = $(SOURCES:.cpp=.o)
EXECUTABLE = minesweeper

.PHONY: all clean

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(EXECUTABLE)

run: $(EXECUTABLE)
	./$(EXECUTABLE)
