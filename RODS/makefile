CXX = g++
Linker = g++
OutputFile = RODS.exe
DynamicLib = RODS.dll

SourceFiles = $(wildcard *.cpp)
Objects = $(subst .cpp,.o,$(SourceFiles))
SharedObjects = $(subst .cpp,.cpp.o,$(SourceFiles))

CXXFLAGS = -Wall -O2
INCLUDE = -I. -I../../OpenBLAS/include -I../../armadillo/include
LIB = -lopenblas

.PHONY: all dylib clean

all: $(OutputFile)

$(OutputFile): $(Objects)
	$(Linker) -o $(OutputFile) $(Objects) $(LIB)

%.o:%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@ $(INCLUDE)

dylib: $(DynamicLib)

$(DynamicLib): $(SharedObjects)
	$(Linker) -shared -o $(DynamicLib) $(SharedObjects) $(LIB)

%.cpp.o:%.cpp
	$(CXX) -shared -fPIC $(CXXFLAGS) -c $< -o $@ $(INCLUDE)

clean:
	rm -rvf *.exe *.dll *.o *.csv
