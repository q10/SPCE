CC=g++
CFLAGS=-Wall -O3
CFLAGS2=-c
LDFLAGS=
SOURCES=constvars.cpp main.cpp random.cpp input.cpp output.cpp initializers.cpp energy.cpp rotation.cpp mc.cpp radial_dist.cpp sampling.cpp tests.cpp print.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=SPCE

all:
	$(CC) $(CFLAGS) *.cpp -o $(EXECUTABLE)
	rm -rf *.o *~

o2:
	$(CC) -Wall -O2 *.cpp -o $(EXECUTABLE)
	
o1:
	$(CC) -Wall -O1 *.cpp -o $(EXECUTABLE)

verbose: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@
	rm -rf *.o *~

.cpp.o:
	$(CC) $(CFLAGS2) $(CFLAGS) $< -o $@

clean:
	rm -rf *.o *~
	rm -f $(EXECUTABLE)
