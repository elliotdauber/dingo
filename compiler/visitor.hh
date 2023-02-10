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

class NodeGenVisitor : public Visitor {
public:
    NodeGenVisitor(ostream& stream);
    void visit_program(Program* program) override;

private:
    ostream& stream;
};

class EdgeGenVisitor : public Visitor {
public:
    EdgeGenVisitor(ostream& stream);
    void visit_program(Program* program) override;

private:
    ostream& stream;
};

class LoweringVisitor : public Visitor {
public:
    LoweringVisitor(ostream& stream);

    void visit_program(Program* program) override;
    void visit_module_decl(ModuleDecl* decl) override;
    void visit_module(Module* module) override;
    void visit_module_list(ModuleList* class_list) override;
    void visit_decorator(Decorator* decorator) override;
    void visit_decorator_list(DecoratorList* decorator_list) override;
    void visit_method_arg(MethodArg* arg) override;
    void visit_method_arg_list(MethodArgList* arg_list) override;
    void visit_module_method(ModuleMethod* method) override;
    void visit_module_method_list(ModuleMethodList* method_list) override;
    void visit_for_each(ForEach* for_each) override;

private:
    ostream& stream;
    vector<DIR::Module> modules;
};

#endif