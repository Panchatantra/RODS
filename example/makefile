CXX = g++
Linker = g++

OutputFile = rods_example

SourceFiles = main.cpp

CXXFLAGS = -Wall -O2
INCLUDE = -I../RODS
LIB = -lrods

.PHONY: all

all: $(OutputFile)

$(OutputFile): $(SourceFiles)
	$(CXX) $(SourceFiles) -o $(OutputFile) $(LIB) $(INCLUDE) $(CXXFLAGS)