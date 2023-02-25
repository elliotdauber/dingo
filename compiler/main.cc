#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <unistd.h>

#include "orchestrator.hh"

using namespace std;

int main(int argc, char* argv[])
{
    if (argc >= 2) {
        DSN::Orchestrator orchestrator;
        string filename = string(argv[1]);
        orchestrator.parse(filename);
        //TODO: return if parse failed
        orchestrator.lower();

        if (argc == 3) {
            ofstream outfile(argv[2]);
            orchestrator.print(outfile);
            // char* args[] = { "dot", "-Tpng", argv[2], NULL };
            // execvp("dot", args);
        } else {
            orchestrator.print(cout);
        }
    } else {
        cout << "usage: ./compiler <infile> <outfile?>" << endl;
    }
}
