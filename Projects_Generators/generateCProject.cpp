//Simple project generator
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
using namespace std;

void create_Folder(char *name[]);
void create_c_File(char *name[]);
void create_header_file(char *name[]);
void create_Makefile(char *name[]);
void create_main(char *name[]);

int main(int argc, char* argv[])
{
  if (argc < 2)
  {
    cout << "Usage: ./generateCProject name_of_project" << endl;
    return 1;
  }
  create_Folder(argv);
  create_c_File(argv);
  create_header_file(argv);
  create_Makefile(argv);
  create_main(argv);
  
  return 42;
}

void create_Folder(char *name[])
{
  stringstream projectName;
  projectName << "mkdir "<< name[1];
  cout << "> " <<  projectName.str().c_str()<< endl;
  system(projectName.str().c_str());
}

void create_c_File(char *name[])
{
  ofstream cpp;
  stringstream projectName;
  projectName <<name[1] << "/" << name[1] << ".c";
  cpp.open(projectName.str().c_str());
  cout << "> Creating " <<  projectName.str().c_str() << endl;
  cpp << "#include \"" << name[1] << ".h\"\n";
  cpp.close();
}

void create_header_file(char *name[])
{
  ofstream h;
  stringstream projectName;
  projectName << name[1] << "/" << name[1] << ".h";
  h.open(projectName.str().c_str());
  cout << "> Creating " <<  projectName.str().c_str() << endl;
  string upper = name[1];
  for(int i = 0; upper[i]; i++)
    upper[i] = toupper(upper[i]);
  
  h << "#include <stdio.h>" << endl;
  h << "#include <stdlib.h>" << endl;
  h << "#ifndef " << upper << "_H" << endl;
  h << "# define IGNORE_UNUSED(x) { x = x; }" << endl << endl;
  h << "# define " << upper << "_H" << endl << endl << endl;
  h << "#endif /* !" << upper << "_H */" << endl;
  h.close();
}

void create_Makefile(char *name[])
{
  ofstream makefile;
  stringstream projectName;
  projectName << name[1] << "/" << "Makefile";
  makefile.open(projectName.str().c_str());
  cout << "> Creating " <<  projectName.str().c_str() << endl;
  
  makefile << "## Simple Makefile\n\n";
  makefile << "CC=gcc\n\n";
  makefile << "CFLAGS= -Wall -Wextra -Werror -Wunused -std=c99 -pedantic"
    " -Wfloat-equal -Wundef -Wshadow -Wpointer-arith"
    " -Wbad-function-cast -Wcast-qual -Wcast-align"
    " -Waggregate-return -Wstrict-prototypes -Wmissing-prototypes"
    " -Wmissing-declarations -Wnested-externs"
    " -Wunreachable-code\n";

  makefile << "LDFLAGS=\n";
  makefile << "NAME=" << name[1] << "\n";
  makefile << "LDLIBS= \n\n";
  makefile << "SRC= $(NAME).c main.c\n\n";
  makefile << "all: \n\t$(CC) $(CFLAGS) $(SRC) -o $(NAME)\n\n";
  makefile << "main: \n\n";
  makefile << "clean:\n";
  makefile << "\t" << "rm -f *~ *.o\n";
  makefile << "\t" << "rm -f main\n\n";
  makefile << "#END";
  makefile.close();
}


void create_main(char *name[])
{
  ofstream main;
  stringstream projectName;
  projectName <<name[1] << "/main.c";
  main.open(projectName.str().c_str());
  cout << "> Creating " <<  projectName.str().c_str() << endl;
  main << "#include \"" << name[1] << ".h\"\n\n";
  main << "int main(int argc, char* argv[])\n{\n\n    IGNORE_UNUSED(argc);\n    IGNORE_UNUSED(argv);\n\n    return 0;\n}";
  main.close();
}
