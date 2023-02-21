#ifndef __ORCHESTRATOR_HH__
#define __ORCHESTRATOR_HH__

#include <cstddef>
#include <istream>
#include <string>

#include "ast.hh"
#include "dir.hh"
#include "parser.tab.hh"
#include "scanner.hh"

using namespace std;

namespace DSN {

class Orchestrator {
public:
    Orchestrator() = default;

    virtual ~Orchestrator();

    /** 
    * parse - parse from a file
    * @param filename - valid string with input file
    */
    void parse(const char* const filename);
    /** 
    * parse - parse from a c++ input stream
    * @param is - istream&, valid input stream
    */
    void parse(istream& iss);

    void lower();

    void add_module_decl(ModuleDecl* decl);
    void add_pattern_defn(PatternDefinition* dfn);
    void add_pattern_appl(PatternApplication* appl);

    ostream& print(ostream& stream);

    map<string, DIR::Module*> get_modules();

private:
    void parse_helper(istream& stream);
    Parser* parser = nullptr;
    Scanner* scanner = nullptr;

    Program* program = new Program();

    const string red = "\033[1;31m";
    const string blue = "\033[1;36m";
    const string norm = "\033[0m";

    map<string, DIR::ModuleComposite*> modules;
};

}

#endif
