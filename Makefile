# Makefile for Flappy_birdS, make sure that you have SFML installed on your system

CXX = g++
CXXFLAGS = -Wall -I/usr/include/SFML -w
LDFLAGS = -lsfml-graphics -lsfml-window -lsfml-system

SOURCES = $(wildcard *.cpp)
OBJECTS = $(SOURCES:.cpp=.o)
EXECUTABLE = flappy_birdS

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $@ $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(EXECUTABLE)
