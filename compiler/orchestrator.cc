#include <cassert>
#include <cctype>
#include <fstream>
#include <filesystem>

#include "dir.hh"
#include "log.hh"
#include "orchestrator.hh"
#include "visitor.hh"

DSN::Orchestrator::~Orchestrator()
{
}

void DSN::Orchestrator::parse(string& filename)
{
    string full_filepath = "";
    if (std::__fs::filesystem::path(filename).is_relative()
        and dir_stack.size() > 0) {
        full_filepath = dir_stack.back() + "/";
    }
    full_filepath += filename;

    std::__fs::filesystem::path path = std::__fs::filesystem::canonical(full_filepath);
    std::__fs::filesystem::path directory = path.parent_path();
    std::__fs::filesystem::path file = path.filename();

    // cout << "Directory: " << directory << endl;
    // cout << "File: " << file << endl;

    dir_stack.push_back(directory.string());

    string absolute_filepath = directory.string() + "/" + file.string();

    if (files_parsed.count(absolute_filepath)) {
        return;
    }
    files_parsed.insert(absolute_filepath);

    cout << "Parsing " << absolute_filepath << endl;
    ifstream in_file(absolute_filepath);
    if (!in_file.good()) {
        cout << "Bad file in Orchestrator::parse" << endl;
        exit(EXIT_FAILURE);
    }

    Scanner* scanner = new Scanner(&in_file);
    Parser* parser = new Parser(*scanner, *this);
    if (parser->parse() != 0) {
        Logger(ERROR) << "Parse failed for " << absolute_filepath << ", exiting!\n";
        exit(EXIT_FAILURE);
    } else {
        cout << "Parsed " << absolute_filepath << " successfully\n";
    }

    dir_stack.pop_back();

    in_file.close();
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

ostream&
DSN::Orchestrator::print(ostream& stream)
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