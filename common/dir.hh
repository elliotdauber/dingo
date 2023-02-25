#ifndef __DIR_HH__
#define __DIR_HH__

#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>

using namespace std;

namespace DIR {

class Visitor;

class Pattern {
public:
    Pattern(string name)
        : name(name)
    {
    }
    string name;
};

class PatternInstance {
public:
    PatternInstance();
    PatternInstance(Pattern* pattern, int id)
        : pattern(pattern)
        , id(id)
    {
    }
    Pattern* pattern;
    // vector<DModule*> modules;
    int id;

    void print(ostream& stream)
    {
        stream << pattern->name << id;
    }
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

    bool operator==(const Type& other) const
    {
        return type == other.type and qualifiers == other.qualifiers;
    }
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

    set<char> create_decorator_set() const
    {
        set<char> dec_set;
        for (size_t i = 0; i < decorators.length(); i++) {
            dec_set.insert(decorators[i]);
        }
        return dec_set;
    }

    bool operator==(const Method& other) const
    {
        set<char> my_decorator_set = create_decorator_set();
        set<char> other_decorator_set = other.create_decorator_set();

        bool args_equal = true;
        if (args.size() != other.args.size()) {
            args_equal = false;
        } else {
            for (size_t i = 0; i < args.size(); i++) {
                if (!(*args[i] == *other.args[i])) {
                    args_equal = false;
                    break;
                }
            }
        }

        return my_decorator_set == other_decorator_set
            and *ret_type == *other.ret_type
            and name == other.name
            and args_equal;
    }

    string escape_chars(string input)
    {
        string output;
        for (char c : input) {
            if (c == '<') {
                output += "\\<";
            } else if (c == '>') {
                output += "\\>";
            } else {
                output += c;
            }
        }
        return output;
    }

    void print(ostream& stream)
    {
        stream << decorators;
        stream << " " << ret_type->type << " " << escape_chars(name) << "(";
        for (size_t i = 0; i < args.size(); i++) {
            stream << args[i]->type;
            stream << args[i]->qualifiers;
            if (i != args.size() - 1)
                stream << ", ";
        }
        stream << ")";
    }

    void print()
    {
        print(cout);
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
    virtual void accept(Visitor* v) = 0;
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
    Module(string name)
        : Module()
    {
        this->name = name;
    }
    string name;
    set<Module*> parents;
    set<Module*> dependencies;
    set<PatternInstance*> patterns;
    vector<Method*> methods;

    vector<Method*> methods_with(char decorator);

    vector<DIR::Module*> get_modules() override { return { this }; }
    Kind kind() override { return Single; }

    void accept(Visitor* v) override;
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

    Kind kind() override { return List; }

    void accept(Visitor* v) override;

private:
    vector<ModuleComposite*> modules;
};

class Visitor {
public:
    virtual void visit_module_list(ModuleList* module_list) { }
    virtual void visit_module(Module* module) { }
};

class GraphContext {
public:
    string label;
    string view_mode;
};

class NodeGenVisitor : public Visitor {
public:
    NodeGenVisitor(ostream& stream, GraphContext& context)
        : stream(stream)
        , context(context)
    {
    }
    void visit_module_list(ModuleList* module_list) override;
    void visit_module(Module* module) override;

    GraphContext context;

private:
    ostream& stream;
};

class EdgeGenVisitor : public Visitor {
public:
    EdgeGenVisitor(ostream& stream)
        : stream(stream)
    {
    }
    void visit_module_list(ModuleList* module_list) override;
    void visit_module(Module* module) override;

private:
    ostream& stream;
};

class Verifier {
public:
    //returns true if the tester is a valid implementation of target
    bool do_modules_conform(map<string, Module*> target, map<string, Module*> tester);

    void generate_graph_png(map<string, Module*> modules, GraphContext& context, string& output_file);
};

} //end namespace DIR

#endif