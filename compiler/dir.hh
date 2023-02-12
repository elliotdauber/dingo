#ifndef __DIR_HH__
#define __DIR_HH__

#include <iostream>
#include <set>
#include <string>
#include <vector>

using namespace std;

namespace DIR {

class DIRModule {
public:
    DIRModule();
    string name;
    set<string> parents;
    set<string> patterns;
    set<string> dependencies;
    set<string> methods;
};

class Pattern {
    string name;
};

class PatternInstance {
public:
    PatternInstance();
    Pattern* pattern;
    // vector<DModule*> modules;
    int id;
};

class Type {
public:
    Type(string type, string qualifiers)
        : type(type)
        , qualifiers(qualifiers)
    {
    }
    string type;
    string qualifiers;
};

class Method {
public:
    Method(string decorators, Type* ret_type, string name, vector<Type*> args)
        : decorators(decorators)
        , ret_type(ret_type)
        , name(name)
        , args(args)
    {
    }
    string decorators;
    Type* ret_type;
    string name;
    vector<Type*> args;

    void print()
    {
        cout << decorators;
        cout << " " << ret_type->type << " " << name << "(";
        for (size_t i = 0; i < args.size(); i++) {
            cout << args[i]->type;
            cout << args[i]->qualifiers;
            if (i != args.size() - 1)
                cout << ", ";
        }
        cout << ")";
    }
};

class Module;

class ModuleComposite {
public:
    enum Kind {
        List,
        Single
    };

    virtual vector<DIR::Module*> get_modules() = 0;
    virtual Kind kind() = 0;
};

class Module : public ModuleComposite {
public:
    Module()
        : parents()
        , dependencies()
        , patterns()
        , methods()
    {
    }
    string name;
    set<Module*> parents;
    set<Module*> dependencies;
    set<PatternInstance*> patterns;
    vector<Method*> methods;

    vector<DIR::Module*> get_modules() override { return { this }; }
    Kind kind() override { return List; }
};

class ModuleList : public ModuleComposite {
public:
    ModuleList() { }
    vector<DIR::Module*> get_modules() override
    {
        vector<Module*> result {};
        for (size_t i = 0; i < modules.size(); i++) {
            vector<DIR::Module*> sub_modules = modules[i]->get_modules();
            for (size_t j = 0; j < sub_modules.size(); j++) {
                result.push_back(sub_modules[j]);
            }
        }
        return result;
    }

    void add(DIR::ModuleComposite* to_add)
    {
        modules.push_back(to_add);
    }

    Kind kind() override { return Single; }

private:
    vector<ModuleComposite*> modules;
};

} //end namespace DIR

#endif