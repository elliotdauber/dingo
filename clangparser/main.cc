#include "dir.hh"
#include "parse.hh"

int main(int argc, const char** argv)
{
    if (argc == 2) {
        map<string, DIR::Module*> modules = parse(argv[1]);
        DIR::Verifier v;
        string file_prefix = "clangparser_main";
        v.generate_graph_png(modules, file_prefix);
    } else {
        cerr << "Usage: ./cppparser <cpp_file.cpp>" << endl;
        return 1;
    }
    return 0;
}
