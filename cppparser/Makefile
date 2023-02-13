CXX = clang++
CPPPARSER_LIB = ../third_party/CppParser/builds/libcppparser.a ../third_party/common/third_party/boost_tp/builds/libboost_filesystem.a
CXXFLAGS += -std=c++17 -I../third_party/CppParser/pub -I../third_party/common/third_party/boost_tp/boost
LDFLAGS += $(CPPPARSER_LIB) -fprofile-arcs -ftest-coverage

all: myapp

myapp: main.cpp
	$(CXX) $(CXXFLAGS) $< $(LDFLAGS) -o $@

clean:
	rm -f myapp *.gcda *.gcno