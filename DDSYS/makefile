CXX = g++
Linker = g++
IntermediateDirectory =.
ObjectSuffix =.o
OutputFile = DDSys.exe

Objects=$(IntermediateDirectory)/node$(ObjectSuffix) $(IntermediateDirectory)/material1D$(ObjectSuffix) $(IntermediateDirectory)/inerter$(ObjectSuffix) $(IntermediateDirectory)/section$(ObjectSuffix) $(IntermediateDirectory)/elementRecorder$(ObjectSuffix) $(IntermediateDirectory)/element1D$(ObjectSuffix) $(IntermediateDirectory)/element$(ObjectSuffix) $(IntermediateDirectory)/elastic1D$(ObjectSuffix) $(IntermediateDirectory)/spis2$(ObjectSuffix) $(IntermediateDirectory)/dof$(ObjectSuffix) \
	$(IntermediateDirectory)/dashpot$(ObjectSuffix) $(IntermediateDirectory)/recorder$(ObjectSuffix) $(IntermediateDirectory)/dashpotExp$(ObjectSuffix) $(IntermediateDirectory)/material$(ObjectSuffix) $(IntermediateDirectory)/DDSYS$(ObjectSuffix) $(IntermediateDirectory)/sectionElasticBar$(ObjectSuffix) $(IntermediateDirectory)/spring$(ObjectSuffix) $(IntermediateDirectory)/spring2D$(ObjectSuffix) $(IntermediateDirectory)/springBilinear$(ObjectSuffix) $(IntermediateDirectory)/timeseries$(ObjectSuffix) \
	$(IntermediateDirectory)/springBoucWen$(ObjectSuffix) $(IntermediateDirectory)/truss2D$(ObjectSuffix) $(IntermediateDirectory)/element2D$(ObjectSuffix) $(IntermediateDirectory)/dofRecorder$(ObjectSuffix) $(IntermediateDirectory)/dsystem$(ObjectSuffix) 


.PHONY: all clean

all: $(OutputFile)

$(OutputFile): $(Objects)
	$(Linker) -o $(OutputFile) $(Objects)  -L../../OpenBLAS/bin -lopenblas

%.o:%.cpp
	$(CXX) -fPIC -c $< -o $@ -I. -I../../OpenBLAS/include -I../../armadillo/include

clean:
	rm -rvf *.exe *.dll *.o *.csv