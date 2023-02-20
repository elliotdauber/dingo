#include "parse.hh"

int main(int argc, const char** argv)
{
    if (argc == 1) {
        parse(argv[1]);
    } else {
        cerr << "Usage: ./cppparser <cpp_file.cpp>" << endl;
        return 1;
    }
    return 0;
}
