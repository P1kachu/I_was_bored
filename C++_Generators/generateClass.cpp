//generateClass
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
using namespace std;

void createCPP(string name);
void createH(string name);

int main(int argc, char *argv[])
{
    if(argc != 2)
    {
        cout << "Please enter the name of the class." << endl;
        return 42;
    }
    createCPP(argv[1]);
    createH(argv[1]);
    cout << "Done" << endl;
    return 0;
}

void createCPP(string name)
{

    ofstream cpp;
    stringstream ss;
    ss << name << ".cpp";
    cpp.open(ss.str().c_str());
    cpp << "#include \"" << name << ".h\"\n\n";
    cpp << name << "::" << name << "() //ctor\n{\n\n}\n\n";
    cpp << name << "::~" << name << "() //dtor\n{\n\n}";
    cpp.close();
    cout << ">" << name << ".cpp" << endl;
}


void createH(string name)
{

    ofstream h;
    stringstream ss;
    ss << name << ".h";
    h.open(ss.str().c_str());
    string upper = name;
    for(int i = 0; upper[i]; i++)
    upper[i] = toupper(upper[i]);

    h << "#include <iostream>\n";
    h << "#include <stdlib.h>\n";
    h << "#ifndef _" << upper << "_H_\n";
    h << "#define _" << upper << "_H_\n\n";
    h << "class " << name << endl;
    h << "{\n\tpublic:\n\t\t" << name << "();\n";
    h << "\t\tvirtual ~" << name << "();\n\n";
    h << "\tprotected:\n\tprivate:\n};\n\n";
    h << "#endif";
    h.close();
    cout << ">" << name << ".h" << endl;
}
