#ifndef __AST_HH__
#define __AST_HH__

#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

class Visitor;

class ProgramNode {
    virtual void accept(Visitor* v) = 0;
};

///////////////////////////////
//           Module          //
///////////////////////////////

class Type : public ProgramNode {
public:
    Type(string type, bool is_array);
    void accept(Visitor* v) override;
    string type;
    bool is_array;
    bool operator==(const Type& other) const
    {
        return type == other.type && is_array == other.is_array;
    }
    string str()
    {
        ostringstream oss;
        oss << type;
        if (is_array) {
            oss << "[]";
        }
        return oss.str();
    }
};

class Module;

class AbstractModuleList : ProgramNode {
public:
    AbstractModuleList(Type* type);
    virtual void accept(Visitor* v) = 0;
    virtual vector<Module*> get_modules() = 0;
    Type* type;
};

class Module : public AbstractModuleList {
public:
    Module(string name);
    void accept(Visitor* v) override;
    vector<Module*> get_modules() override { return { this }; }
    string name;
};

class ModuleList : public AbstractModuleList {
public:
    ModuleList();
    ModuleList(ModuleList* init_list, Module* new_items);
    ModuleList(Module* item);
    void accept(Visitor* v) override;
    vector<Module*> get_modules() override { return items; }
    vector<Module*> items;
};

class Decorator : public ProgramNode {
public:
    Decorator(string decorator);
    void accept(Visitor* v) override;
    string decorator;
};

class DecoratorList : public ProgramNode {
public:
    DecoratorList();
    DecoratorList(DecoratorList* init_list, Decorator* new_item);
    DecoratorList(Decorator* item);
    void accept(Visitor* v) override;
    vector<Decorator*> items;
};

class MethodArg : public ProgramNode {
public:
    MethodArg(string type, string qualifiers);
    void accept(Visitor* v) override;
    string type;
    string qualifiers;
};

class MethodArgList : public ProgramNode {
public:
    MethodArgList();
    MethodArgList(MethodArgList* init_list, MethodArg* new_item);
    MethodArgList(MethodArg* item);
    void accept(Visitor* v) override;
    vector<MethodArg*> items;
};

class ModuleMethod;

class AbstractModuleMethod : public ProgramNode {
public:
    virtual void accept(Visitor* v) = 0;
    virtual vector<ModuleMethod*> get_methods() = 0;
};

class ModuleMethod : public AbstractModuleMethod {
public:
    ModuleMethod(DecoratorList* decorators, string ret_type, string name, MethodArgList* args);
    void accept(Visitor* v) override;
    DecoratorList* decorators;
    string ret_type;
    string name;
    MethodArgList* args;

    vector<ModuleMethod*> get_methods() override
    {
        return { this };
    }
};

class ModuleMethodList : public ProgramNode {
public:
    ModuleMethodList();
    ModuleMethodList(ModuleMethodList* init_list, AbstractModuleMethod* new_items);
    ModuleMethodList(AbstractModuleMethod* item);
    void accept(Visitor* v) override;
    vector<AbstractModuleMethod*> items;
};

class ForEach : public AbstractModuleMethod {
public:
    ForEach(string individual_name, ModuleList* aggregate, ModuleMethodList* methods);
    void accept(Visitor* v) override;
    string individual_name;
    ModuleList* aggregate;
    ModuleMethodList* methods;

    vector<ModuleMethod*> get_methods() override
    {
        vector<ModuleMethod*> result {};
        for (size_t i = 0; i < methods->items.size(); i++) {
            vector<ModuleMethod*> method_list = methods->items[i]->get_methods();
            for (size_t j = 0; j < method_list.size(); j++) {
                ModuleMethod* method = method_list[j];
                result.push_back(method);
            }
        }
        return result;
    }
};

class ModuleDecl : public ProgramNode {
public:
    ModuleDecl(Module* module, ModuleList* parent_modules, ModuleList* dependencies, ModuleMethodList* methods);
    void accept(Visitor* v) override;
    Module* module;
    ModuleList* parent_modules;
    ModuleList* dependencies;
    ModuleMethodList* methods;
};

///////////////////////////////
//          Member           //
///////////////////////////////

class Member : public ProgramNode {
public:
    Member(string name, Type* type);
    void accept(Visitor* v) override;
    string name;
    Type* type;
};

class MemberList : public ProgramNode {
public:
    MemberList();
    MemberList(MemberList* init_list, Member* new_item);
    MemberList(Member* item);
    void accept(Visitor* v) override;
    Member* find(string name)
    {
        for (size_t i = 0; i < items.size(); i++) {
            if (items[i]->name == name) {
                return items[i];
            }
        }
        return nullptr;
    }
    vector<Member*> items;
};

///////////////////////////////
//           Spec            //
///////////////////////////////

class Spec : public ProgramNode {
public:
    Spec(ModuleDecl* module_decl);
    void accept(Visitor* v) override;
    ModuleDecl* module_decl;
};

class SpecList : public ProgramNode {
public:
    SpecList();
    SpecList(SpecList* init_list, Spec* new_item);
    SpecList(Spec* item);
    void accept(Visitor* v) override;
    vector<Spec*> items;
};

///////////////////////////////
//          Pattern          //
///////////////////////////////

class PatternDefinition : public ProgramNode {
public:
    PatternDefinition(string name, MemberList* members, SpecList* specs);
    void accept(Visitor* v) override;
    string name;
    MemberList* members;
    SpecList* specs;
};

class MemberAssignment : public ProgramNode {
public:
    MemberAssignment(string member_name, AbstractModuleList* assignment);
    void accept(Visitor* v) override;
    string member_name;
    AbstractModuleList* assignment;
};

class MemberAssignmentList : public ProgramNode {
public:
    MemberAssignmentList();
    MemberAssignmentList(MemberAssignmentList* init_list, MemberAssignment* new_item);
    MemberAssignmentList(MemberAssignment* item);
    void accept(Visitor* v) override;
    vector<MemberAssignment*> items;
};

class PatternApplication : public ProgramNode {
public:
    PatternApplication(string name, MemberAssignmentList* member_assignments);
    void accept(Visitor* v) override;
    string name;
    MemberAssignmentList* member_assignments;
};

class Program : public ProgramNode {
public:
    Program();
    void add_module_decl(ModuleDecl* decl);
    void add_pattern_defn(PatternDefinition* defn);
    void add_pattern_appl(PatternApplication* appl);
    void accept(Visitor* v) override;
    vector<ModuleDecl*> module_decls;
    vector<PatternDefinition*> pattern_defns;
    vector<PatternApplication*> pattern_appls;
};

#endif
