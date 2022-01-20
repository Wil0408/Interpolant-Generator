CC = g++
LDFLAGS = -std=c++0x -O3 -lm

SOURCES = src/main.cpp src/resol_proof.cpp
OBJECTS = $(SOURCES:.c=.o)

EXECUTABLE = itp
INCLUDES = src/resol_proof.h 

all: $(SOURCES) bin/$(EXECUTABLE)

bin/$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

%.o: %.c ${INCLUDES}
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf *.o bin/$(EXECUTABLE)