CC=g++
CFLAGS=-Wall -O3
CFLAGS2=-c
LDFLAGS=
SOURCES=constvars.cpp main.cpp random.cpp input.cpp output.cpp initializers.cpp energy.cpp mc.cpp radial_dist.cpp sampling.cpp tests.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=SPCE

verbose: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@
	rm -rf *.o *~

.cpp.o:
	$(CC) $(CFLAGS2) $(CFLAGS) $< -o $@

all:
	$(CC) $(CFLAGS) *.cpp -o $(EXECUTABLE)
	rm -rf *.o *~