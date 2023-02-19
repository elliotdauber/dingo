#include "dir.hh"

namespace DIR {

void Module::accept(Visitor* v)
{
    v->visit_module(this);
}

void ModuleList::accept(Visitor* v)
{
    v->visit_module_list(this);
}

void NodeGenVisitor::visit_module_list(ModuleList* module_list)
{
    for (Module*& module : module_list->get_modules()) {
        module->accept(this);
    }
}

void NodeGenVisitor::visit_module(Module* module)
{
    stream << module->name << " [label=\"" << module->name;
    for (Method*& method : module->methods) {
        stream << "\n";
        method->print(stream);
    }
    stream << "\"];" << endl;
}

void EdgeGenVisitor::visit_module_list(ModuleList* module_list)
{
    for (Module*& module : module_list->get_modules()) {
        module->accept(this);
    }
}

void EdgeGenVisitor::visit_module(Module* module)
{
    string name = module->name;
    set<Module*> parents = module->parents;
    set<Module*> deps = module->dependencies;
    for (auto it = parents.begin(); it != parents.end(); ++it) {
        stream << name << " -> " << (*it)->name << "[arrowhead=odot];" << endl;
    }
    for (auto it = deps.begin(); it != deps.end(); ++it) {
        stream << name << " -> " << (*it)->name << "[arrowhead=normal];" << endl;
    }
}

bool Verifier::do_modules_conform(map<string, Module*> target, map<string, Module*> tester)
{
    for (auto module_it = tester.begin(); module_it != tester.end(); ++module_it) {
        string module_name = module_it->first;
        Module* tester_module = module_it->second;
        if (!target.count(module_name)) {
            cout << "Warning: Module " << module_name << " was defined in target code, but not in Dingo." << endl;
            continue;
        }
        Module* target_module = target[module_name];
        for (auto dep_it = tester_module->dependencies.begin(); dep_it != tester_module->dependencies.end(); ++dep_it) {
            string dependency_name = (*dep_it)->name;
            auto find_it = find_if(target_module->dependencies.begin(), target_module->dependencies.end(), [&dependency_name](const Module* p) {
                return p->name == dependency_name;
            });
            if (find_it == target_module->dependencies.end()) {
                cerr << "Error: In the target code, the module " << module_name << " has a dependency on " << dependency_name << ", but this was not specified in the Dingofile." << endl;
                return false;
            }
        }

        // for (auto it = tester_module->parents.begin(); it != tester_module->parents.end(); ++it) {
        //     string parent_name = (*it)->name;
        //     if (!target_module->parents.count(parent_name)) {
        //         cerr << "Error: In the target code, the module " << module_name << " has parent " << parent_name << ", but this was not specified in the Dingofile." << endl;
        //         exit(1);
        //     }
        // }
    }
    return true;
}

} //end namespace DIR