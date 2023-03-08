#ifndef __ORCHESTRATOR_HH__
#define __ORCHESTRATOR_HH__

#include <cstddef>
#include <istream>
#include <string>
#include <vector>

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
    void parse(string& filename);

    void lower();

    void add_module_decl(ModuleDecl* decl);
    void add_pattern_defn(PatternDefinition* dfn);
    void add_pattern_appl(PatternApplication* appl);

    ostream& print(ostream& stream);

    map<string, DIR::Module*> get_modules();
private:
    Program* program = new Program();

    const string red = "\033[1;31m";
    const string blue = "\033[1;36m";
    const string norm = "\033[0m";

    map<string, DIR::ModuleComposite*> modules;

    vector<string> dir_stack = vector<string>();
    set<string> files_parsed = set<string>();
};

}

#endif
