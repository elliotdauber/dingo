# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -std=c++17 -Wall -I../compiler/

# TinyXML-2 library flags
TINYXML2_FLAGS = -ltinyxml2

# Source files
SOURCES = main.cpp

# Object files
OBJECTS = $(SOURCES:.cpp=.o)

# Target executable
TARGET = main

# Build rule
$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(OBJECTS) $(TINYXML2_FLAGS) -o $(TARGET)

# Object files build rule
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean rule
.PHONY: clean
clean:
	-rm -f $(OBJECTS) $(TARGET)
