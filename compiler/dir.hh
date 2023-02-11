#ifndef __DIR_HH__
#define __DIR_HH__

#include <set>
#include <string>
#include <vector>

using namespace std;

namespace DIR {

class Pattern {
    string name;
};

class PatternInstance {
public:
    PatternInstance();
    Pattern* pattern;
    vector<Module*> modules;
    int id;
};

class Module {
public:
    Module();
    string name;
    set<string> parents;
    set<string> patterns;
    set<string> dependencies;
    set<string> methods;
};

} //end namespace DIR

#endif