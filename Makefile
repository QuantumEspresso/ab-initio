CXX=g++
CXXFLAGS=-std=c++11 -Wall -I/home/norbert/lib/include/spglib/
CXXLIBS=-L/home/norbert/lib/lib/ -lsymspg -lm
X=main

OBJS=$(X).o
$(X): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(X) $(CXXLIBS) 

$(X).o: read_input.h var.h $(X).C
	$(CXX) $(CXXFLAGS) -c $(X).C

.PHONY: clean

clean:
	rm -f $(OBJS) $(X)
