#include "ast.hh"
#include "visitor.hh"
#include <iostream>
#include <istream>
#include <sstream>

Program::Program()
    : module_decls()
    , pattern_defns()
    , pattern_appls()
{
}
void Program::add_module_decl(ModuleDecl* decl)
{
    module_decls.push_back(decl);
}

void Program::add_pattern_defn(PatternDefinition* defn)
{
    pattern_defns.push_back(defn);
}

void Program::add_pattern_appl(PatternApplication* appl)
{
    pattern_appls.push_back(appl);
}
void Program::accept(Visitor* v) { v->visit_program(this); }

AbstractModuleList::AbstractModuleList(Type* type)
    : type(type)
{
}

Module::Module(string name)
    : AbstractModuleList(new Type("module", false))
    , name(name)
{
}
void Module::accept(Visitor* v) { v->visit_module(this); }

ModuleList::ModuleList()
    : AbstractModuleList(new Type("module", true))
    , items()
{
}
ModuleList::ModuleList(ModuleList* init_list, Module* new_item)
    : AbstractModuleList(new Type("module", true))
    , items(init_list->items)
{
    items.push_back(new_item);
}
ModuleList::ModuleList(Module* item)
    : AbstractModuleList(new Type("module", true))
    , items({ item })
{
}
void ModuleList::accept(Visitor* v)
{
    v->visit_module_list(this);
}

Decorator::Decorator(string decorator)
    : decorator(decorator)
{
}
void Decorator::accept(Visitor* v)
{
    v->visit_decorator(this);
}

DecoratorList::DecoratorList()
    : items()
{
}
DecoratorList::DecoratorList(DecoratorList* init_list, Decorator* new_item)
    : items(init_list->items)
{
    items.push_back(new_item);
}
DecoratorList::DecoratorList(Decorator* item)
    : items({ item })
{
}
void DecoratorList::accept(Visitor* v)
{
    v->visit_decorator_list(this);
}

MethodArg::MethodArg(string type, string qualifiers)
    : type(type)
    , qualifiers(qualifiers)
{
}

void MethodArg::accept(Visitor* v)
{
    v->visit_method_arg(this);
}

MethodArgList::MethodArgList()
    : items()
{
}
MethodArgList::MethodArgList(MethodArgList* init_list, MethodArg* new_item)
    : items(init_list->items)
{
    items.push_back(new_item);
}
MethodArgList::MethodArgList(MethodArg* item)
    : items({ item })
{
}
void MethodArgList::accept(Visitor* v)
{
    v->visit_method_arg_list(this);
}

ModuleMethod::ModuleMethod(DecoratorList* decorators, MethodArg* ret_type, string name, MethodArgList* args)
    : decorators(decorators)
    , ret_type(ret_type)
    , name(name)
    , args(args)
{
}

void ModuleMethod::accept(Visitor* v)
{
    v->visit_module_method(this);
}

ModuleMethodList::ModuleMethodList()
    : items()
{
}
ModuleMethodList::ModuleMethodList(ModuleMethodList* init_list, AbstractModuleMethod* new_item)
    : items(init_list->items)
{
    items.push_back(new_item);
}
ModuleMethodList::ModuleMethodList(AbstractModuleMethod* item)
    : items({ item })
{
}
void ModuleMethodList::accept(Visitor* v)
{
    v->visit_module_method_list(this);
}

ModuleDecl::ModuleDecl(Module* module, ModuleList* parent_modules, ModuleList* dependencies, ModuleMethodList* methods)
    : module(module)
    , parent_modules(parent_modules)
    , dependencies(dependencies)
    , methods(methods)
{
}
void ModuleDecl::accept(Visitor* v)
{
    v->visit_module_decl(this);
}

ForEach::ForEach(string individual_name, ModuleList* aggregate, ModuleMethodList* methods)
    : individual_name(individual_name)
    , aggregate(aggregate)
    , methods(methods)
{
}
void ForEach::accept(Visitor* v)
{
    v->visit_for_each(this);
}

Type::Type(string type, bool is_array)
    : type(type)
    , is_array(is_array)
{
}
void Type::accept(Visitor* v)
{
    v->visit_type(this);
}

Member::Member(string name, Type* type)
    : name(name)
    , type(type)
{
}
void Member::accept(Visitor* v)
{
    v->visit_member(this);
}

MemberList::MemberList()
    : items()
{
}
MemberList::MemberList(MemberList* init_list, Member* new_item)
    : items(init_list->items)
{
    items.push_back(new_item);
}
MemberList::MemberList(Member* item)
    : items({ item })
{
}
void MemberList::accept(Visitor* v)
{
    v->visit_member_list(this);
}

Spec::Spec(ModuleDecl* module_decl)
    : module_decl(module_decl)
{
}
void Spec::accept(Visitor* v)
{
    v->visit_spec(this);
}

SpecList::SpecList()
    : items()
{
}
SpecList::SpecList(SpecList* init_list, Spec* new_item)
    : items(init_list->items)
{
    items.push_back(new_item);
}
SpecList::SpecList(Spec* item)
    : items({ item })
{
}
void SpecList::accept(Visitor* v)
{
    v->visit_spec_list(this);
}

PatternDefinition::PatternDefinition(string name, MemberList* members, SpecList* specs)
    : name(name)
    , members(members)
    , specs(specs)
{
}

void PatternDefinition::accept(Visitor* v)
{
    v->visit_pattern_defn(this);
}

MemberAssignment::MemberAssignment(string member_name, AbstractModuleList* assignment)
    : member_name(member_name)
    , assignment(assignment)
{
}

void MemberAssignment::accept(Visitor* v)
{
    v->visit_member_assignment(this);
}

MemberAssignmentList::MemberAssignmentList()
    : items()
{
}
MemberAssignmentList::MemberAssignmentList(MemberAssignmentList* init_list, MemberAssignment* new_item)
    : items(init_list->items)
{
    items.push_back(new_item);
}
MemberAssignmentList::MemberAssignmentList(MemberAssignment* item)
    : items({ item })
{
}
void MemberAssignmentList::accept(Visitor* v)
{
    v->visit_member_assignment_list(this);
}

PatternApplication::PatternApplication(string name, MemberAssignmentList* member_assignments)
    : name(name)
    , member_assignments(member_assignments)
{
}

void PatternApplication::accept(Visitor* v)
{
    v->visit_pattern_appl(this);
}
