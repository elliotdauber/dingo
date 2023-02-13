#include <iostream>

#include "cppparser.h"

#include <boost/filesystem.hpp>

using namespace std;
namespace fs = boost::filesystem;

int main()
{
    cout << "ayo" << endl;
    CppParser parser;
    const auto ast = parser.parseFile("main.cpp");

    const auto& members = ast->members();
    CppIncludeEPtr hashInclude = members[0];
    cout << hashInclude->name_ << endl;
    return 0;
}