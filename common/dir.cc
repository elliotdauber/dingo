#include "dir.hh"
#include "log.hh"
#include <fstream>
#include <sstream>

namespace DIR {

void Module::accept(Visitor* v)
{
    v->visit_module(this);
}

vector<Method*> Module::methods_with(char decorator)
{
    vector<Method*> result;
    for (Method* method : methods) {
        for (char dec : method->decorators) {
            if (dec == decorator) {
                result.push_back(method);
                break;
            }
        }
    }
    return result;
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
    if ((context.interface == "public" or context.interface == "all") and module->methods_with('+').size() > 0) {
        stream << "|";
        for (Method*& method : module->methods_with('+')) {
            method->print(stream);
            stream << "\\l";
        }
    }
    if ((context.interface == "private" or context.interface == "all") and module->methods_with('-').size() > 0) {
        stream << "|";
        for (Method*& method : module->methods_with('-')) {
            method->print(stream);
            stream << "\\l";
        }
    }
    if (context.patterns) {
        for (PatternInstance* pattern : module->patterns) {
            stream << "|";
            pattern->print(stream);
            stream << "\\l" << pattern->fields[module->name] << "\\l";
        }
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
            Logger(WARNING) << "Warning: Module " << module_name << " was defined in code, but not in Dingo.\n";
            continue;
        }
        Module* target_module = target[module_name];
        for (auto dep_it = tester_module->dependencies.begin(); dep_it != tester_module->dependencies.end(); ++dep_it) {
            string dependency_name = (*dep_it)->name;
            auto find_it = find_if(target_module->dependencies.begin(), target_module->dependencies.end(), [&dependency_name](const Module* p) {
                return p->name == dependency_name;
            });
            if (find_it == target_module->dependencies.end()) {
                Logger(ERROR) << "Error: In the code, the module " << module_name << " has a dependency on " << dependency_name << ", but this was not specified in the Dingofile.\n";
                return false;
            }
        }

        for (auto parent_it = tester_module->parents.begin(); parent_it != tester_module->parents.end(); ++parent_it) {
            string parent_name = (*parent_it)->name;
            auto find_it = find_if(target_module->parents.begin(), target_module->parents.end(), [&parent_name](const Module* p) {
                return p->name == parent_name;
            });
            if (find_it == target_module->parents.end()) {
                Logger(ERROR) << "Error: In the code, the module " << module_name << " has parent " << parent_name << ", but this was not specified in the Dingofile.\n";
                return false;
            }
        }

        //TODO: this disallows overloaded methods
        for (Method* tester_method : tester_module->methods) {
            string method_name = tester_method->name;
            auto find_it = find_if(target_module->methods.begin(), target_module->methods.end(), [&method_name](const Method* m) {
                return m->name == method_name;
            });
            if (find_it != target_module->methods.end()) {
                //found a method with the same name in the target
                if (!(*tester_method == **find_it)) {
                    Logger(ERROR) << "Error: In the code, the method " << method_name << " in module " << module_name << " has the incorrect signature. Check the Dingofile for the correct signature.\n";
                    exit(1);
                }
            } else {
                Logger(WARNING) << "Warning. In the Dingofile, the method " << method_name << " in module " << module_name << " is declared, but is not used in the code.\n";
            }
        }
    }

    for (auto module_it = target.begin(); module_it != target.end(); ++module_it) {
        string module_name = module_it->first;
        Module* target_module = module_it->second;
        if (!tester.count(module_name)) {
            Logger(WARNING) << "Warning: Module " << module_name << " was defined in the Dingofile, but not in code.\n";
            continue;
        }
        Module* tester_module = tester[module_name];
        for (auto dep_it = target_module->dependencies.begin(); dep_it != target_module->dependencies.end(); ++dep_it) {
            string dependency_name = (*dep_it)->name;
            auto find_it = find_if(tester_module->dependencies.begin(), tester_module->dependencies.end(), [&dependency_name](const Module* p) {
                return p->name == dependency_name;
            });
            if (find_it == tester_module->dependencies.end()) {
                Logger(WARNING) << "Warning: In the Dingofile, the module " << module_name << " has a dependency on " << dependency_name << ", but this is not used in the code.\n";
            }
        }

        for (auto parent_it = target_module->parents.begin(); parent_it != target_module->parents.end(); ++parent_it) {
            string parent_name = (*parent_it)->name;
            auto find_it = find_if(tester_module->parents.begin(), tester_module->parents.end(), [&parent_name](const Module* p) {
                return p->name == parent_name;
            });
            if (find_it == tester_module->parents.end()) {
                Logger(ERROR) << "Error: In the Dingofile, the module " << module_name << " has parent " << parent_name << ", but this is not true in the code\n";
                return false;
            }
        }
    }
    return true;
}

void Verifier::generate_graph_png(map<string, Module*> modules, GraphContext& context, string& output_file)
{
    string dotfile_name = "intermediate.dingo.dot";
    ofstream dotfile(dotfile_name);
    dotfile
        << "digraph example {" << endl;
    dotfile << "rankdir=LR;" << endl;
    dotfile << "node [shape=record];" << endl;

    DIR::NodeGenVisitor* node_gen = new DIR::NodeGenVisitor(dotfile, context);

    for (auto it = modules.begin(); it != modules.end(); ++it) {
        it->second->accept(node_gen);
    }

    dotfile << endl;

    DIR::EdgeGenVisitor* edge_gen = new DIR::EdgeGenVisitor(dotfile);

    for (auto it = modules.begin(); it != modules.end(); ++it) {
        it->second->accept(edge_gen);
    }

    dotfile << "label=\"" << context.label << "\"" << endl;
    dotfile << "style=filled" << endl;
    dotfile << "fillcolor=yellow" << endl;
    dotfile << "}" << endl;

    dotfile.close();

    ostringstream oss;
    oss << "dot -Tpng " << dotfile_name << " > " << output_file << ";"
        << " rm " << dotfile_name;
    FILE* fp = popen(oss.str().c_str(), "r");

    char buffer[256];
    string output = "";
    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        output += buffer;
    }

    cout << output << endl;

    pclose(fp);
}

} //end namespace DIR