#include "dir.hh"
#include "orchestrator.hh"
#include "parse.hh"
#include <iostream>

int main(int argc, char* argv[])
{
    if (argc == 3) {
        //compile the .dsn file
        DSN::Orchestrator orchestrator;
        orchestrator.parse(argv[1]);
        orchestrator.lower();
        map<string, DIR::Module*> target_modules = orchestrator.get_modules();

        set<string> modules_to_process;
        for (auto it = target_modules.begin(); it != target_modules.end(); ++it) {
            modules_to_process.insert(it->first);
        }

        map<string, DIR::Module*> tester_modules = parse(argv[2], modules_to_process);

        DIR::Verifier v;
        if (v.do_modules_conform(target_modules, tester_modules)) {
            cout << "Success: everything looks good!" << endl;
        } else {
            cout << "Failure: check the errors" << endl;
        }

        string dingofile_prefix = "verifier_dingofile";
        string codefile_prefix = "verifier_codefile";
        v.generate_graph_png(target_modules, dingofile_prefix);
        v.generate_graph_png(tester_modules, codefile_prefix);

    } else {
        cout << "usage: ./verifier <dsn_file> <cpp_file>" << endl;
    }
    return 0;
}