CXX = g++
CXXFLAGS = -std=c++11
OBJ = lab2_2.o
EXE = lab2_2

CROSS_COMPILE = aarch64-linux-gnu-
#CXX = $(CROSS_COMPILE)g++
LD = $(CROSS_COMPILE)ld

all:$(EXE)
$(EXE): $(OBJ)
	$(CXX) -o $(EXE) $(OBJ)

%.o:%.cpp
	$(CXX) -o $@ -c $<

	
clean:
	rm -f $(EXE)
	rm -f $(OBJ)

