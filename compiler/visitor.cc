#include "visitor.hh"

#include <iostream>
#include <set>

//////////////////////////////////////////////////
//               PrintingVisitor                //
//////////////////////////////////////////////////

PrintingVisitor::PrintingVisitor(ostream& stream)
    : stream(stream)
{
}

void PrintingVisitor::visit_program(Program* program)
{
    for (size_t i = 0; i < program->module_decls.size(); i++) {
        program->module_decls[i]->accept(this);
    }
}
void PrintingVisitor::visit_module(Module* module)
{
    stream << "module: " << module->name;
}
void PrintingVisitor::visit_module_decl(ModuleDecl* decl)
{
    decl->module->accept(this);
    stream << "{" << endl;
    decl->dependencies->accept(this);
    stream << "}" << endl;
}
void PrintingVisitor::visit_module_list(ModuleList* modules)
{
    for (size_t i = 0; i < modules->items.size(); i++) {
        modules->items[i]->accept(this);
    }
}

//////////////////////////////////////////////////
//                NodeGenVisitor                //
//////////////////////////////////////////////////

NodeGenVisitor::NodeGenVisitor(ostream& stream)
    : stream(stream)
{
}

void NodeGenVisitor::visit_program(Program* program)
{
    stream << "digraph example {" << endl;
    stream << "rankdir=LR;" << endl;
    stream << "node [shape=circle];" << endl;
    for (size_t i = 0; i < program->module_decls.size(); i++) {
        string name = program->module_decls[i]->module->name;
        stream << name << " [label=\"" << name << "\"];" << endl;
    }
    // stream << "}" << endl;
}

//////////////////////////////////////////////////
//                EdgeGenVisitor                //
//////////////////////////////////////////////////

EdgeGenVisitor::EdgeGenVisitor(ostream& stream)
    : stream(stream)
{
}

void EdgeGenVisitor::visit_program(Program* program)
{
    stream << endl;
    for (size_t i = 0; i < program->module_decls.size(); i++) {
        string name = program->module_decls[i]->module->name;
        ModuleList* parents = program->module_decls[i]->parent_modules;
        for (size_t j = 0; j < parents->items.size(); j++) {
            stream << name << " -> " << parents->items[j]->name << "[arrowhead=odot];" << endl;
        }
        for (size_t j = 0; j < program->module_decls[i]->dependencies->items.size(); j++) {
            string dep_name = program->module_decls[i]->dependencies->items[j]->name;
            stream << name << " -> " << dep_name << "[arrowhead=normal];" << endl;
        }
    }
    stream << "label=\"The System\"" << endl;
    stream << "style=filled" << endl;
    stream << "fillcolor=yellow" << endl;
    stream << "}" << endl;
}

//////////////////////////////////////////////////
//                LoweringVisitor               //
//////////////////////////////////////////////////

LoweringVisitor::LoweringVisitor(ostream& stream)
    : stream(stream)
    , modules()
{
}

void update_module(DIR::Module* module, ModuleDecl* decl)
{
    ModuleList* parents = decl->parent_modules;
    for (size_t i = 0; i < parents->items.size(); i++) {
        module->parents.insert(parents->items[i]->name);
    }

    ModuleList* dependencies = decl->dependencies;
    for (size_t i = 0; i < dependencies->items.size(); i++) {
        module->dependencies.insert(dependencies->items[i]->name);
    }

    ModuleMethodList* methods = decl->methods;
    for (size_t i = 0; i < methods->items.size(); i++) {
        AbstractModuleMethod* abstract_method = methods->items[i];
        vector<ModuleMethod*> methods = abstract_method->get_methods();
        for (size_t j = 0; j < methods.size(); j++) {
            module->methods.insert(methods[j]->str());
        }
    }
}

void LoweringVisitor::visit_program(Program* program)
{
    map<string, DIR::Module*> module_map {};
    for (size_t i = 0; i < program->module_decls.size(); i++) {
        DIR::Module* module = new DIR::Module();
        ModuleDecl* decl = program->module_decls[i];
        string name = decl->module->name;
        if (module_map.count(name)) {
            cerr << "Duplicated module name: " << name << endl;
            exit(1);
        }
        module->name = name;
        module_map[name] = module;
        update_module(module, decl);
    }

    //TODO: check for circular inheritance hierarchies

    map<string, PatternDefinition*> patterns {};
    for (size_t i = 0; i < program->pattern_defns.size(); i++) {
        string name = program->pattern_defns[i]->name;
        if (patterns.count(name)) {
            cerr << "Duplicated pattern name: " << name << endl;
            exit(1);
        }
        patterns[name] = program->pattern_defns[i];
    }

    for (size_t i = 0; i < program->pattern_appls.size(); i++) {
        PatternApplication* pattern_appl = program->pattern_appls[i];
        string name = pattern_appl->name;
        if (!patterns.count(name)) {
            cerr << "Attempting to apply an undeclared pattern: " << name << endl;
            exit(1);
        }
        PatternDefinition* pattern_defn = patterns[name];

        size_t defn_num_members = pattern_defn->members->items.size();
        size_t appl_num_members = pattern_appl->member_assignments->items.size();
        if (appl_num_members != defn_num_members) {
            cerr << "Attemping to apply pattern, but got the incorrect number of member assignments: Pattern " << name << " contains " << defn_num_members << " members, but application tried to assign " << appl_num_members << " members. " << endl;
            exit(1);
        }

        map<string, AbstractModuleList*> member_bindings;
        for (size_t j = 0; j < appl_num_members; j++) {
            MemberAssignment* assignment = pattern_appl->member_assignments->items[j];
            string member_name = assignment->member_name;
            Member* defn_member = pattern_defn->members->find(member_name);

            if (defn_member == nullptr) {
                cerr << "Tried to assign to the member " << member_name << " in an application of the pattern " << name << ", but this member does not exist on the pattern." << endl;
                exit(1);
            }

            Type* defn_member_type = defn_member->type;
            Type* member_assignment_type = assignment->assignment->type;
            if (!(*defn_member_type == *member_assignment_type)) {
                cerr << "Type error: The type of field " << member_name << " in the application of the pattern " << name << " is wrong. The type should be " << defn_member_type->str() << ", but is " << member_assignment_type->str() << "." << endl;
                exit(1);
            }

            member_bindings[defn_member->name] = assignment->assignment;
        }

        SpecList* specs = pattern_defn->specs;

        if (specs->items.size() > member_bindings.size()) {
            cerr << "Too many specs defined for pattern " << name << ". Expected at most " << member_bindings.size() << ", but received " << specs->items.size() << "." << endl;
        }

        for (size_t j = 0; j < specs->items.size(); j++) {
            Spec* spec = specs->items[j];
            string spec_name = spec->module_decl->module->name;
            ModuleList* spec_dependencies = spec->module_decl->dependencies;
            if (!member_bindings.count(spec_name)) {
                cerr << "In pattern " << name << ", defined a spec for " << spec_name << ", but this is not a defined member." << endl;
                exit(1);
            }
            vector<Module*> module_list = member_bindings[spec_name]->get_modules();

            ostringstream pattern_instance_name;
            pattern_instance_name << name << i;

            for (size_t k = 0; k < module_list.size(); k++) {
                string module_name = module_list[k]->name;
                //TODO: is this guaranteed to exist?
                DIR::Module* cur_module = module_map[module_name];
                update_module(cur_module, spec->module_decl);
                cur_module->patterns.insert(pattern_instance_name.str());
            }
        }
    }
    for (auto outer_it = module_map.begin(); outer_it != module_map.end(); ++outer_it) {
        string name = outer_it->first;
        DIR::Module* module = outer_it->second;
        cout << name;
        if (module->parents.size() > 0) {
            cout << " < ";
            for (auto it = module->parents.begin(); it != module->parents.end(); ++it) {
                cout << *it;
                cout << ", ";
            }
        }
        cout << " : [";
        for (auto it = module->dependencies.begin(); it != module->dependencies.end(); ++it) {
            cout << *it << ", ";
        }
        cout << "] {" << endl;
        for (auto it = module->methods.begin(); it != module->methods.end(); ++it) {
            cout << "    " << *it << ";" << endl;
        }
        cout << "}" << endl;
        cout << "patterns: ";
        for (auto it = module->patterns.begin(); it != module->patterns.end(); ++it) {
            cout << *it << ", ";
        }
        cout << endl
             << endl;
    }
}
void LoweringVisitor::visit_module_decl(ModuleDecl* decl)
{
}
void LoweringVisitor::visit_module(Module* module)
{
}
void LoweringVisitor::visit_module_list(ModuleList* class_list)
{
}
void LoweringVisitor::visit_decorator(Decorator* decorator)
{
}
void LoweringVisitor::visit_decorator_list(DecoratorList* decorator_list)
{
}
void LoweringVisitor::visit_method_arg(MethodArg* arg)
{
}
void LoweringVisitor::visit_method_arg_list(MethodArgList* arg_list)
{
}
void LoweringVisitor::visit_module_method(ModuleMethod* method)
{
}
void LoweringVisitor::visit_module_method_list(ModuleMethodList* method_list)
{
}
void LoweringVisitor::visit_for_each(ForEach* for_each)
{
}