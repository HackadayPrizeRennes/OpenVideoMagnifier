INC	 = `pkg-config --cflags opencv`
LIBS	 = `pkg-config --libs opencv`
CXX	 = g++
CXXFLAGS = -c -Wall -ggdb $(INC) -std=c++11
LDFLAGS  = $(LIBS) -ldl
SRC = $(wildcard *.cpp)
OBJ	 = $(patsubst %.cpp,%.o,$(SRC))
EXEC	 = ../bin/clusterize

all : $(EXEC)

../bin/%.o : %.cpp
	$(CXX) -c $(CXXFLAGS) $< -o $@ 2> errorlog

$(EXEC) : $(OBJ)
	$(CXX) $(LDFLAGS) -export-dynamic $^ -o $@

clean :
	-@rm -f *~ ../bin/*.o $(EXEC)
