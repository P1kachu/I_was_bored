//Simple project generator
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
using namespace std;

void createFolder(char *name[]);
void createCPPFile(char *name[]);
void createHFile(char *name[]);
void createMakefile(char *name[]);
void createMainFile(char *name[]);

int main(int argc, char* argv[])
{
  createFolder(argv);
  createCPPFile(argv);
  createHFile(argv);
  createMakefile(argv);
  createMainFile(argv);
  
  return 42;
}

void createFolder(char *name[])
{
  stringstream projectName;
  projectName << "mkdir "<< name[1];
  cout << "> " <<  projectName.str().c_str()<< endl;
  system(projectName.str().c_str());
}

void createCPPFile(char *name[])
{
  ofstream cpp;
  stringstream projectName;
  projectName <<name[1] << "/" << name[1] << ".cpp";
  cpp.open(projectName.str().c_str());
  cout << "> " <<  projectName.str().c_str() << endl;
  cpp << "#include \"" << name[1] << ".h\"\n";
  cpp.close();
}

void createHFile(char *name[])
{
  ofstream h;
  stringstream projectName;
  projectName << name[1] << "/" << name[1] << ".h";
  h.open(projectName.str().c_str());
  cout << "> " <<  projectName.str().c_str() << endl;
  string upper = name[1];
  for(int i = 0; upper[i]; i++)
    upper[i] = toupper(upper[i]);
  
  h << "#include <stdio.h>" << endl;
  h << "#include <stdlib.h>" << endl;
  h << "#include <iostream>" << endl;
  h << "using std::cin;" << endl;
  h << "using std::cout;" << endl;
  h << "using std::endl;" << endl;
  h << "#ifndef __" << upper << "__" << endl;
  h << "#define __" << upper << "__" << endl << endl << endl;
  h << "#endif" << endl;
  h.close();
}

void createMakefile(char *name[])
{
  ofstream makefile;
  stringstream projectName;
  projectName << name[1] << "/" << "Makefile";
  makefile.open(projectName.str().c_str());
  cout << "> " <<  projectName.str().c_str() << endl;
  
  makefile << "## Simple Makefile\n\n";
  makefile << "CC=g++\n\n";
  makefile << "CPPFLAGS=-Wall -Wextra -Werror\n";
  makefile << "CFLAGS= \n";
  makefile << "LDFLAGS=\n";
  makefile << "NAME=" << name[1] << "\n";
  makefile << "LDLIBS= \n\n";
  makefile << "SRC= $(NAME).cpp main.cpp\n\n";
  makefile << "all: \n\t$(CC) $(CPPFLAGS) $(SRC) -o $(NAME)\n\n";
  makefile << "main: \n\n";
  makefile << "clean:\n";
  makefile << "\t" << "rm -f *~ *.o\n";
  makefile << "\t" << "rm -f main\n\n";
  makefile << "#END";
  makefile.close();
}


void createMainFile(char *name[])
{
  ofstream main;
  stringstream projectName;
  projectName <<name[1] << "/main.cpp";
  main.open(projectName.str().c_str());
  cout << "> " <<  projectName.str().c_str() << endl;
  main << "#include \"" << name[1] << ".h\"\n\n";
  main << "int main()\n{\n\n    return 42;\n}";
  main.close();
}
