CXX = g++
Linker = g++
OutputFile = DDSys.exe

SourceFiles = $(wildcard *.cpp)
Objects = $(subst .cpp,.o,$(SourceFiles))

CXXFLAGS = -Wall -O2
INCLUDE = -I. -I../../OpenBLAS/include -I../../armadillo/include
LIB = -L../../OpenBLAS/bin -lopenblas

.PHONY: all clean

all: $(OutputFile)

$(OutputFile): $(Objects)
	$(Linker) -o $(OutputFile) $(Objects) $(LIB)

%.o:%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@ $(INCLUDE)

clean:
	rm -rvf *.exe *.dll *.o *.csv