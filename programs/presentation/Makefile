CXX = g++
CXXFLAGS = -std=c++11 -Wall
LDFLAGS =
SRC_DIR = src
INC_DIR = includes

SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS = $(SOURCES:$(SRC_DIR)/%.cpp=$(SRC_DIR)/%.o)
INCLUDES = $(wildcard $(INC_DIR)/*.hpp)

EXECUTABLE = presentation

all: $(EXECUTABLE)

$(EXECUTABLE): main.cpp $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -I $(INC_DIR) $^ -o $@

$(SRC_DIR)/%.o: $(SRC_DIR)/%.cpp $(INCLUDES)
	$(CXX) $(CXXFLAGS) -I$(INC_DIR) -c $< -o $@

clean:
	rm -f $(SRC_DIR)/*.o $(EXECUTABLE)