CC=g++
CFLAGS=-c -Wall
LDFLAGS=
SOURCES=main.cpp random.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=SPCE

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
        $(CC) $(LDFLAGS) $(OBJECTS) -o $@
        rm -rf *.o

.cpp.o:
        $(CC) $(CFLAGS) $< -o $@
