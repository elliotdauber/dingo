#include "visitor.hh"

#include <iostream>
#include <regex>
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
//                LoweringVisitor               //
//////////////////////////////////////////////////

void ModuleCreatorVisitor::visit_program(Program* program)
{
    for (size_t i = 0; i < program->module_decls.size(); i++) {
        visit_module_decl(program->module_decls[i]);
    }
}

void ModuleCreatorVisitor::visit_module_decl(ModuleDecl* decl)
{
    DIR::Module* module = new DIR::Module();
    string name = decl->module->name;
    if (modules.count(name)) {
        cerr << "Duplicated module name: " << name << endl;
        exit(1);
    }
    module->name = name;
    modules[name] = module;
}

void ModuleUpdaterVisitor::visit_program(Program* program)
{
    for (size_t i = 0; i < program->module_decls.size(); i++) {
        program->module_decls[i]->accept(this);
    }
}

void ModuleUpdaterVisitor::visit_module_decl(ModuleDecl* decl)
{
    vector<DIR::Module*> module_list = modules[decl->module->name]->get_modules();
    for (DIR::Module*& module : module_list) {
        ModuleList* parents = decl->parent_modules;
        for (size_t i = 0; i < parents->items.size(); i++) {
            string parent_name = parents->items[i]->name;
            if (!modules.count(parent_name)) {
                cerr << "Parent " << parent_name << " of module " << module->name << " is not declared." << endl;
                exit(1);
            }
            vector<DIR::Module*> parent_modules = modules[parent_name]->get_modules();
            for (size_t j = 0; j < parent_modules.size(); j++) {
                module->parents.insert(parent_modules[j]);
            }
        }

        ModuleList* dependencies = decl->dependencies;
        for (size_t i = 0; i < dependencies->items.size(); i++) {
            string dep_name = dependencies->items[i]->name;
            if (!modules.count(dep_name)) {
                cerr << "Dependency " << dep_name << " of module " << module->name << " is not declared." << endl;
                exit(1);
            }
            vector<DIR::Module*> dependency_modules = modules[dep_name]->get_modules();
            for (size_t j = 0; j < dependency_modules.size(); j++) {
                module->dependencies.insert(dependency_modules[j]);
            }
        }

        ModuleMethodList* mod_methods = decl->methods;
        for (size_t i = 0; i < mod_methods->items.size(); i++) {
            AbstractModuleMethod* abstract_method = mod_methods->items[i];
            methods.clear();
            foreach_bindings.clear();
            abstract_method->accept(this);
            for (size_t i = 0; i < methods.size(); i++) {
                //TODO: check method types to make sure none have the same type and
                //name
                // methods[i]->print();
                // cout << endl;
                module->methods.push_back(methods[i]);
            }
        }
    }
}

void ModuleUpdaterVisitor::visit_module_method(ModuleMethod* method)
{
    string name = method->name;
    name = regex_replace(name, regex("[<>]"), "");

    // cout << name << endl;

    for (auto it = foreach_bindings.begin(); it != foreach_bindings.end(); ++it) {
        ostringstream oss;
        oss << "\\{" << it->first << "\\}";
        // cout << oss.str() << endl;
        regex r(oss.str());
        name = regex_replace(name, r, it->second);
    }

    //TODO: allow int, string, void, etc
    string ret_type = method->ret_type->type;
    if (foreach_bindings.count(ret_type)) {
        ret_type = foreach_bindings[ret_type];
    } else if (!modules.count(ret_type)) {
        cerr << "Return type " << ret_type << " in method " << name << " is unknown." << endl;
        exit(1);
    } else {
        DIR::ModuleComposite* composite = modules[ret_type];
        if (composite->kind() == DIR::ModuleComposite::Kind::List) {
            cerr << "Attempting to assign module[] type " << ret_type << " to a return val, which should be a module type." << endl;
            exit(1);
        } else {
            vector<DIR::Module*> m = composite->get_modules();
            if (m.size() == 0) {
                cerr << "module " << ret_type << " is empty. Something went very wrong." << endl;
                exit(1);
            } else {
                ret_type = m[0]->name;
            }
        }
    }

    vector<DIR::Type*> args {};
    for (size_t i = 0; i < method->args->items.size(); i++) {
        //TODO: allow int, string, void, etc
        string arg_type = method->args->items[i]->type;
        if (foreach_bindings.count(arg_type)) {
            arg_type = foreach_bindings[arg_type];
        } else if (!modules.count(arg_type)) {
            cerr << "Arg type " << arg_type << " in method " << name << " is unknown." << endl;
            exit(1);
        } else {
            DIR::ModuleComposite* composite = modules[arg_type];
            if (composite->kind() == DIR::ModuleComposite::Kind::List) {
                cerr << "Attempting to assign module[] type " << arg_type << " to an arg, which should be a module type." << endl;
                exit(1);
            } else {
                vector<DIR::Module*> m = composite->get_modules();
                if (m.size() == 0) {
                    cerr << "module " << arg_type << " is empty. Something went very wrong." << endl;
                    exit(1);
                } else {
                    arg_type = m[0]->name;
                }
            }
        }
        args.push_back(new DIR::Type(arg_type, method->args->items[i]->qualifiers));
    }

    ostringstream decorators;
    for (size_t i = 0; i < method->decorators->items.size(); i++) {
        decorators << method->decorators->items[i]->decorator;
    }

    methods.push_back(new DIR::Method(
        decorators.str(),
        new DIR::Type(ret_type, method->ret_type->qualifiers),
        name,
        args));
}

void ModuleUpdaterVisitor::visit_for_each(ForEach* foreach)
{
    //TODO: flatten list first to remove code duplication
    vector<Module*> foreach_modules = foreach->aggregate->get_modules();
    for (size_t i = 0; i < foreach_modules.size(); i++) {
        string foreach_module_name = foreach_modules[i]->name;
        if (modules.count(foreach_module_name)) {
            vector<DIR::Module*> module_list = modules[foreach_module_name]->get_modules();
            for (size_t j = 0; j < module_list.size(); j++) {
                string module_name = module_list[j]->name;

                foreach_bindings[foreach->individual_name] = module_name;

                vector<AbstractModuleMethod*> foreach_methods = foreach->methods->items;
                for (size_t k = 0; k < foreach_methods.size(); k++) {
                    foreach_methods[k]->accept(this);
                }

                foreach_bindings.erase(foreach->individual_name);
            }
        } else {
            foreach_bindings[foreach->individual_name] = foreach_modules[i]->name;

            vector<AbstractModuleMethod*> foreach_methods = foreach->methods->items;
            for (size_t j = 0; j < foreach_methods.size(); j++) {
                foreach_methods[j]->accept(this);
            }

            foreach_bindings.erase(foreach->individual_name);
        }
    }
}

void PatternApplierVisitor::visit_pattern_appl(PatternApplication* pattern_appl)
{
    string pattern_name = pattern_appl->pattern_name;
    if (!pattern_defns.count(pattern_name)) {
        cerr << "Attempting to apply an undeclared pattern: " << pattern_name << endl;
        exit(1);
    }
    DIR::Pattern* pat = new DIR::Pattern(pattern_name);
    DIR::PatternInstance* pat_instance = new DIR::PatternInstance(pat, pattern_appl->canonical_name);
    PatternDefinition* pattern_defn
        = pattern_defns[pattern_name];

    size_t defn_num_members = pattern_defn->members->items.size();
    size_t appl_num_members = pattern_appl->member_assignments->items.size();
    if (appl_num_members != defn_num_members) {
        cerr << "Attemping to apply pattern, but got the incorrect number of member assignments: Pattern " << pattern_name << " contains " << defn_num_members << " members, but application tried to assign " << appl_num_members << " members. " << endl;
        exit(1);
    }

    for (size_t j = 0; j < appl_num_members; j++) {
        MemberAssignment* assignment = pattern_appl->member_assignments->items[j];
        string member_name = assignment->member_name;
        Member* defn_member = pattern_defn->members->find(member_name);

        if (defn_member == nullptr) {
            cerr << "Tried to assign to the member " << member_name << " in an application of the pattern " << pattern_name << ", but this member does not exist on the pattern." << endl;
            exit(1);
        }

        Type* defn_member_type = defn_member->type;
        Type* member_assignment_type = assignment->assignment->type;
        if (!(*defn_member_type == *member_assignment_type)) {
            cerr << "Type error: The type of field " << member_name << " in the application of the pattern " << pattern_name << " is wrong. The type should be " << defn_member_type->str() << ", but is " << member_assignment_type->str() << "." << endl;
            exit(1);
        }

        vector<Module*> assignment_modules = assignment->assignment->get_modules();

        if (assignment->assignment->type->is_array) {
            DIR::ModuleList* module_list = new DIR::ModuleList();

            for (size_t k = 0; k < assignment_modules.size(); k++) {
                if (!modules.count(assignment_modules[k]->name)) {
                    cerr << "Tried to apply pattern with module " << assignment_modules[k]->name << " for pattern " << pattern_appl->pattern_name << ", field " << member_name << ", but this module does not exist." << endl;
                    exit(1);
                }
                DIR::ModuleComposite* composite = modules[assignment_modules[k]->name];
                module_list->add(composite);
            }

            //TODO: make sure we aren't overwriting something??
            modules[defn_member->name] = module_list;
        } else {
            //TODO: is this guaranteed to be safe?
            if (!modules.count(assignment_modules[0]->name)) {
                cerr << "Tried to apply pattern with module " << assignment_modules[0]->name << " for pattern " << pattern_appl->pattern_name << ", field " << member_name << ", but this module does not exist." << endl;
                exit(1);
            }
            modules[defn_member->name] = modules[assignment_modules[0]->name];
        }

        DIR::ModuleComposite* mod_comp = modules[defn_member->name];
        for (DIR::Module* module : mod_comp->get_modules()) {
            module->patterns.insert(pat_instance);
        }
    }

    SpecList* specs = pattern_defn->specs;

    if (specs->items.size() > appl_num_members) {
        cerr << "Too many specs defined for pattern " << pattern_name << ". Expected at most " << appl_num_members << ", but received " << specs->items.size() << "." << endl;
    }

    ModuleUpdaterVisitor* module_updater = new ModuleUpdaterVisitor(modules);

    for (size_t j = 0; j < specs->items.size(); j++) {
        Spec* spec = specs->items[j];
        string spec_name = spec->module_decl->module->name;

        //TODO: use modules here? or something more specific?
        if (!modules.count(spec_name)) {
            cerr << "In pattern " << pattern_name << ", defined a spec for " << spec_name << ", but this is not a defined member." << endl;
            exit(1);
        }
        spec->module_decl->accept(module_updater);
    }

    modules = module_updater->modules;

    //remove bindings
    for (size_t j = 0; j < appl_num_members; j++) {
        MemberAssignment* assignment = pattern_appl->member_assignments->items[j];
        string member_name = assignment->member_name;
        Member* defn_member = pattern_defn->members->find(member_name);
        modules.erase(defn_member->name);
    }
}

void PatternApplierVisitor::visit_program(Program* program)
{
    for (size_t i = 0; i < program->pattern_defns.size(); i++) {
        string name = program->pattern_defns[i]->name;
        if (pattern_defns.count(name)) {
            cerr << "Duplicated pattern name: " << name << endl;
            exit(1);
        }
        pattern_defns[name] = program->pattern_defns[i];
    }

    for (size_t i = 0; i < program->pattern_appls.size(); i++) {
        program->pattern_appls[i]->accept(this);
    }
}
