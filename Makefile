CC=g++
CFLAGS=-c -Wall
LDFLAGS=
SOURCES=constvars.cpp main.cpp random.cpp input.cpp initializers.cpp energy.cpp mc.cpp radial_dist.cpp tests.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=SPCE

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@
	rm -rf *.o

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@
