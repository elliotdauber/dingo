#ifndef __VISITOR_HH__
#define __VISITOR_HH__

#include "ast.hh"
#include "dir.hh"

class Visitor {
public:
    virtual ~Visitor() = default;
    virtual void visit_program(Program* program) { }
    virtual void visit_module_decl(ModuleDecl* decl) { }
    virtual void visit_module(Module* module) { }
    virtual void visit_module_list(ModuleList* class_list) { }
    virtual void visit_decorator(Decorator* decorator) { }
    virtual void visit_decorator_list(DecoratorList* decorator_list) { }
    virtual void visit_method_arg(MethodArg* arg) { }
    virtual void visit_method_arg_list(MethodArgList* arg_list) { }
    virtual void visit_module_method(ModuleMethod* method) { }
    virtual void visit_module_method_list(ModuleMethodList* method_list) { }
    virtual void visit_for_each(ForEach* for_each) { }
    virtual void visit_type(Type* type) { }
    virtual void visit_member(Member* member) { }
    virtual void visit_member_list(MemberList* member_list) { }
    virtual void visit_spec(Spec* spec) { }
    virtual void visit_spec_list(SpecList* spec_list) { }
    virtual void visit_pattern_defn(PatternDefinition* defn) { }
    virtual void visit_pattern_appl(PatternApplication* appl) { }
    virtual void visit_member_assignment(MemberAssignment* assignment) { }
    virtual void visit_member_assignment_list(MemberAssignmentList* assignment_list) { }
};

class PrintingVisitor : public Visitor {
public:
    PrintingVisitor(ostream& stream);
    void visit_program(Program* program) override;
    void visit_module_decl(ModuleDecl* decl) override;
    void visit_module(Module* module) override;
    void visit_module_list(ModuleList* class_list) override;

private:
    ostream& stream;
};

class ModuleCreatorVisitor : public Visitor {
public:
    ModuleCreatorVisitor()
        : modules()
    {
    }

    void visit_program(Program* program) override;
    void visit_module_decl(ModuleDecl* decl) override;

    map<string, DIR::ModuleComposite*> modules;
};

class ModuleUpdaterVisitor : public Visitor {
public:
    ModuleUpdaterVisitor(map<string, DIR::ModuleComposite*> modules)
        : modules(modules)
        , methods()
        , foreach_bindings()
    {
    }

    void visit_program(Program* program) override;
    void visit_module_decl(ModuleDecl* decl) override;
    void visit_module_method(ModuleMethod* method) override;
    void visit_for_each(ForEach* foreach) override;

    map<string, DIR::ModuleComposite*> modules;
    vector<DIR::Method*> methods;
    map<string, string> foreach_bindings;
};

class PatternApplierVisitor : public Visitor {
public:
    PatternApplierVisitor(map<string, DIR::ModuleComposite*> modules)
        : modules(modules)
        , pattern_defns()
    {
    }

    void visit_program(Program* program) override;
    void visit_pattern_appl(PatternApplication* appl) override;

    map<string, DIR::ModuleComposite*> modules;
    map<string, PatternDefinition*> pattern_defns;
};

#endif