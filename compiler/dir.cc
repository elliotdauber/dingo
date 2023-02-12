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

} //end namespace DIR