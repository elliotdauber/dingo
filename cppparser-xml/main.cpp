#include "dir.hh"
#include <iostream>
#include <map>
#include <string>
#include <tinyxml2.h>

using namespace std;

int main()
{
    tinyxml2::XMLDocument doc;
    if (doc.LoadFile("foo.xml") != tinyxml2::XML_SUCCESS) {
        cerr << "Failed to load XML file." << endl;
        return 1;
    }

    tinyxml2::XMLElement* root = doc.FirstChildElement("GCC_XML");
    if (!root) {
        cerr << "Failed to find root element 'GCC_XML'." << endl;
        return 1;
    }

    map<string, DIR::Module*> modules {};

    for (tinyxml2::XMLElement* element = root->FirstChildElement("Class");
         element != nullptr;
         element = element->NextSiblingElement("Class")) {
        const char* file = element->Attribute("file");
        if (file == nullptr || string(file) != "f0") {
            const char* name = element->Attribute("name");
            if (name != nullptr) {
                cout << name << endl;
            }
        }
    }

    return 0;
}