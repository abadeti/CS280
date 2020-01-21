/*
 * main.cpp
 */

#include "lex.h"
#include "parse.h"
#include "valSKEL.h"
#include <fstream>
using std::cin;
using std::cout;
using std::endl;
using std::ifstream;

int
main(int argc, char *argv[])
{
    map<string,Val> symbols;

	ifstream file;
	istream *in;
	int linenum = 0;

	if( argc == 1 ) {
		in = &cin;
	}

	else if( argc == 2 ) {
		file.open(argv[1]);
		if( file.is_open() == false ) {
			cout << "COULD NOT OPEN " << argv[1] << endl;
			return 1;
		}
		in = &file;
	}

	else {
		cout << "TOO MANY FILENAMES" << endl;
		return 1;
	}

	ParseTree *tree = Prog(*in, linenum);
    int undeclared = tree->CheckLetBeforeUse(symbols);
    if(undeclared == 1)
    {
        tree->CheckLetBeforeUse(symbols);
    }
    else
    {

    if(tree ==0){
    //null 
    
    }
    
    if(tree !=0){
        
        
        
        
        try {
           tree->Eval(symbols);
            }
        catch( std::logic_error& e) {
            linenum--;
            cout << "RUNTIME ERROR at " << linenum << ": " << e.what() << endl;
        
       }
	return 0;
}
    }
}
