#include <cassert>
#include <cctype>
#include <fstream>

#include "dir.hh"
#include "orchestrator.hh"
#include "visitor.hh"

DSN::Orchestrator::~Orchestrator()
{
    delete (scanner);
    scanner = nullptr;
    delete (parser);
    parser = nullptr;
}

void DSN::Orchestrator::parse(const char* const filename)
{
    assert(filename != nullptr);
    std::ifstream in_file(filename);
    if (!in_file.good()) {
        exit(EXIT_FAILURE);
    }
    parse_helper(in_file);
}

void DSN::Orchestrator::parse(std::istream& stream)
{
    if (!stream.good() && stream.eof()) {
        return;
    }
    parse_helper(stream);
}

void DSN::Orchestrator::parse_helper(std::istream& stream)
{

    delete (scanner);
    try {
        scanner = new Scanner(&stream);
    } catch (std::bad_alloc& ba) {
        std::cerr << "Failed to allocate scanner: (" << ba.what() << "), exiting!!\n";
        exit(EXIT_FAILURE);
    }

    delete (parser);
    try {
        parser = new Parser((*scanner) /* scanner */,
            (*this) /* orchestrator */);
    } catch (std::bad_alloc& ba) {
        std::cerr << "Failed to allocate parser: (" << ba.what() << "), exiting!!\n";
        exit(EXIT_FAILURE);
    }
    const int accept(0);
    if (parser->parse() != accept) {
        std::cerr << "Parse failed!!\n";
    } else {
        cout << "parsed successfully!" << endl;
    }
    return;
}

void DSN::Orchestrator::lower()
{
    ModuleCreatorVisitor* creator = new ModuleCreatorVisitor();
    program->accept(creator);
    ModuleUpdaterVisitor* updater = new ModuleUpdaterVisitor(creator->modules);
    program->accept(updater);
    PatternApplierVisitor* applier = new PatternApplierVisitor(updater->modules);
    program->accept(applier);

    modules = applier->modules;
}

std::ostream&
DSN::Orchestrator::print(std::ostream& stream)
{
    stream << "digraph example {" << endl;
    stream << "rankdir=LR;" << endl;
    stream << "node [shape=square];" << endl;

    DIR::NodeGenVisitor* node_gen = new DIR::NodeGenVisitor(stream);

    for (auto it = modules.begin(); it != modules.end(); ++it) {
        it->second->accept(node_gen);
    }

    stream << endl;

    DIR::EdgeGenVisitor* edge_gen = new DIR::EdgeGenVisitor(stream);

    for (auto it = modules.begin(); it != modules.end(); ++it) {
        it->second->accept(edge_gen);
    }

    stream << "label=\"The System\"" << endl;
    stream << "style=filled" << endl;
    stream << "fillcolor=yellow" << endl;
    stream << "}" << endl;

    for (auto outer_it = modules.begin(); outer_it != modules.end(); ++outer_it) {
        string name = outer_it->first;
        DIR::ModuleComposite* modules = outer_it->second;
        for (DIR::Module*& module : modules->get_modules()) {
            cout << name;
            if (module->parents.size() > 0) {
                cout << " < ";
                int idx = 0;
                for (auto it = module->parents.begin(); it != module->parents.end(); ++it) {
                    cout << (*it)->name;
                    if (idx++ != module->parents.size() - 1)
                        cout << ", ";
                }
            }
            cout << " : [";

            int idx = 0;
            for (auto it = module->dependencies.begin(); it != module->dependencies.end(); ++it) {
                cout << (*it)->name;
                if (idx++ != module->dependencies.size() - 1)
                    cout << ", ";
            }
            cout << "] {" << endl;
            for (auto it = module->methods.begin(); it != module->methods.end(); ++it) {
                (*it)->print();
                cout << ";" << endl;
            }
            cout << "}" << endl;
            // if (module->patterns.size() > 0) {
            //     cout << "patterns: ";
            //     idx = 0;
            //     for (auto it = module->patterns.begin(); it != module->patterns.end(); ++it) {
            //         cout << *it;
            //         if (idx++ != module->patterns.size() - 1)
            //             cout << ", ";
            //     }
            // }
            cout << endl
                 << endl;
        }
    }

    return stream;
}

void DSN::Orchestrator::add_module_decl(ModuleDecl* decl)
{
    program->add_module_decl(decl);
}

void DSN::Orchestrator::add_pattern_defn(PatternDefinition* defn)
{
    program->add_pattern_defn(defn);
}
void DSN::Orchestrator::add_pattern_appl(PatternApplication* appl)
{
    program->add_pattern_appl(appl);
}