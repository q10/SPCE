CC=g++
CFLAGS=-c -Wall -O3
LDFLAGS=
SOURCES=constvars.cpp main.cpp random.cpp input.cpp output.cpp initializers.cpp energy.cpp mc.cpp radial_dist.cpp sampling.cpp tests.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=SPCE

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@
	rm -rf *.o *~

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

new:
	$(CC) $(CFLAGS) *.cpp -o $(EXECUTABLE)