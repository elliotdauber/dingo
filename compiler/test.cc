#include "dir.hh"

void test_verifier()
{
    DIR::Verifier v;

    map<string, DIR::Module*> target;
    map<string, DIR::Module*> tester;

    //TODO!

    bool conforming = v.do_modules_conform(target, tester);
}

int main()
{
    return 0;
}