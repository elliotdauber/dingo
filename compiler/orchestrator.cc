#include <cassert>
#include <cctype>
#include <fstream>

#include "dir.hh"
#include "log.hh"
#include "orchestrator.hh"
#include "visitor.hh"

DSN::Orchestrator::~Orchestrator()
{
    delete (scanner);
    scanner = nullptr;
    delete (parser);
    parser = nullptr;
}

void DSN::Orchestrator::parse(string& filename)
{
    std::ifstream in_file(filename);
    if (!in_file.good()) {
        cout << "bad file in Orchestrator::parse" << endl;
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
        Logger(ERROR) << "Failed to allocate scanner: (" << ba.what() << "), exiting!!\n";
        exit(EXIT_FAILURE);
    }

    delete (parser);
    try {
        parser = new Parser((*scanner) /* scanner */,
            (*this) /* orchestrator */);
    } catch (std::bad_alloc& ba) {
        Logger(ERROR) << "Failed to allocate parser: (" << ba.what() << "), exiting!!\n";
        exit(EXIT_FAILURE);
    }
    const int accept(0);
    if (parser->parse() != accept) {
        Logger(ERROR) << "Parse failed, exiting!\n";
        exit(EXIT_FAILURE);
    } else {
        cout << "parsed Dingofile successfully\n";
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
    DIR::Verifier v;
    DIR::GraphContext context;
    context.label = "compiler main";
    context.view_mode = "all";
    string output_file = "compiler_main.png";
    v.generate_graph_png(get_modules(), context, output_file);

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

map<string, DIR::Module*> DSN::Orchestrator::get_modules()
{
    map<string, DIR::Module*> result {};
    for (auto it = modules.begin(); it != modules.end(); ++it) {
        string name = it->first;
        DIR::ModuleComposite* composite = it->second;
        if (composite->get_modules().size() == 1) {
            result[name] = composite->get_modules()[0];
        }
    }
    return result;
}