CXX = clang++
CXXFLAGS=-std=c++14 -Wno-deprecated-register -I./common -g
COMMON_SRC = $(wildcard common/*.cc)
COMMON_OBJ = $(COMMON_SRC:.cc=.o)
COMPILER_SRC = $(wildcard compiler/*.cc)
COMPILER_OBJ = $(COMPILER_SRC:.cc=.o)
CLANGPARSER_SRC = $(wildcard clangparser/*.cc)
CLANGPARSER_OBJ = $(CLANGPARSER_SRC:.cc=.o)

BIN_DIR = bin

LDFLAGS = 

all: compiler cppparser

#compiler

CPPOBJ = compiler/main compiler/orchestrator compiler/ast compiler/visitor
SOBJ =  parser lexer

FILES = $(addsuffix .cc, $(CPPOBJ))

OBJS  = $(addsuffix .o, $(CPPOBJ))

CLEANLIST =  $(addsuffix .o, $(OBJ)) $(OBJS) \
				 compiler/parser.tab.cc compiler/parser.tab.hh \
				 compiler/location.hh compiler/position.hh \
			    compiler/stack.hh compiler/parser.output compiler/parser.o \
				 compiler/lexer.o compiler/lexer.yy.cc \

compiler: $(COMMON_OBJ) $(FILES)
	$(MAKE) $(SOBJ)
	$(MAKE) $(OBJS)
	$(CXX) $(CXXFLAGS) $(COMMON_OBJ) -o $(BIN_DIR)/compiler $(OBJS) compiler/parser.o compiler/lexer.o $(LIBS)


parser: compiler/parser.yy
	bison -d -v --file-prefix=compiler/parser compiler/parser.yy 
	$(CXX) $(CXXFLAGS) -c -o compiler/parser.o compiler/parser.tab.cc

lexer: compiler/lexer.l
	flex --outfile=compiler/lexer.yy.cc  $<
	$(CXX)  $(CXXFLAGS) -c compiler/lexer.yy.cc -o compiler/lexer.o



# clangparser

LIBCLANG_DIR = /usr/local/Cellar/llvm/15.0.3/lib
CLANG_INCLUDE_DIR = /usr/local/Cellar/llvm/15.0.3/include

CLANGCXXFLAGS = -I$(CLANG_INCLUDE_DIR) -std=c++17 -I../compiler
CLANGLDFLAGS = -L$(LIBCLANG_DIR) -lclang -lclangSupport -lclangAST -lclangBasic -lclangFrontend -lclangLex -lclangParse -lclangSema -lclangEdit -lclangAnalysis -lclangASTMatchers -lclangDriver -lclangSerialization -lclangToolingCore -lclangRewrite -lclangRewriteFrontend -lclangStaticAnalyzerFrontend -lclangStaticAnalyzerCheckers -lclangStaticAnalyzerCore -lclangARCMigrate -lclangDynamicASTMatchers -lclangFormat -lclangTooling -lclangToolingInclusions -lLLVMCore -lLLVMSupport -lllvm -lZSTD -lz -lncurses

cppparser: $(COMMON_OBJ) $(CLANGPARSER_OBJ)
	$(CXX) $(CXXFLAGS) $(CLANGCXXFLAGS) $(COMMON_OBJ) $(CLANGPARSER_OBJ) -o $(BIN_DIR)/cppparser $(LDFLAGS) $(CLANGLDFLAGS)

clangparser/%.o: clangparser/%.cc
	$(CXX) $(CXXFLAGS) $(CLANGCXXFLAGS) -c $< -o $@

clean:
	rm -f $(COMMON_OBJ) $(COMPILER_OBJ) $(CLANGPARSER_OBJ) $(CLEANLIST) bin/*