CXX = clang++

LIBCLANG_DIR = /usr/local/Cellar/llvm/15.0.3/lib
CLANG_INCLUDE_DIR = /usr/local/Cellar/llvm/15.0.3/include

CXXFLAGS += -I$(CLANG_INCLUDE_DIR) -std=c++17 -I../compiler
LDFLAGS += -L$(LIBCLANG_DIR) -lclang -lclangSupport -lclangAST -lclangBasic -lclangFrontend -lclangLex -lclangParse -lclangSema -lclangEdit -lclangAnalysis -lclangASTMatchers -lclangDriver -lclangSerialization -lclangToolingCore -lclangRewrite -lclangRewriteFrontend -lclangStaticAnalyzerFrontend -lclangStaticAnalyzerCheckers -lclangStaticAnalyzerCore -lclangARCMigrate -lclangDynamicASTMatchers -lclangFormat -lclangTooling -lclangToolingInclusions -lLLVMCore -lLLVMSupport -lllvm -lZSTD -lz -lncurses


SOURCES = main.cc ../compiler/dir.cc
OBJECTS = $(SOURCES:.cc=.o)
EXECUTABLE = clangparser

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(EXECUTABLE) $(OBJECTS)