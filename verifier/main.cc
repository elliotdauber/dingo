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
        map<string, DIR::ModuleComposite*> target_modules = orchestrator.modules;

        map<string, DIR::Module*> tester_modules = parse(argv[2]);

    } else {
        cout << "usage: ./verifier <dsn_file> <cpp_file>" << endl;
    }
    return 0;
}