# Define compiler
CXX = g++

# Program executable
EXE = bin/main.out

# Source dir
DIR = src

# Compiler standard flags.
CXX_STDFLAGS = -std=c++20

all:
	$(CXX) $(CXX_STDFLAGS) $(DIR)/*.cpp -o $(EXE)

run:
	./$(EXE)