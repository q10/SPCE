CC=g++
CFLAGS=-Wall -O3
CFLAGS2=-c
LDFLAGS=
SOURCES=constvars.cpp main.cpp random.cpp input.cpp output.cpp initializers.cpp energy.cpp mc.cpp radial_dist.cpp sampling.cpp tests.cpp print.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=SPCE

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@
	rm -rf *.o *~

.cpp.o:
	$(CC) $(CFLAGS2) $(CFLAGS) $< -o $@

safe:
	$(CC) $(CFLAGS) *.cpp -o $(EXECUTABLE)
	rm -rf *.o *~