# Makefile for Flappy_birdS, ensure that SFML is installed on your system

CXX = g++
CXXFLAGS = -Wall -I/usr/include/SFML -w
LDFLAGS = -lsfml-graphics -lsfml-window -lsfml-system

SRC_DIR = src

# Source and Object files
SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS = $(patsubst $(SRC_DIR)/%.cpp, %.o, $(SOURCES))

EXECUTABLE = flappy_birdS

all: $(EXECUTABLE)

# Link the executable
$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $@ $(LDFLAGS)
	rm -f $(OBJECTS)  # Remove object files after linking

# Compile source files into object files
%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f *.o $(EXECUTABLE)
