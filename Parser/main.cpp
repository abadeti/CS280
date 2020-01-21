#include <iostream>
#include <string>
#include <fstream>
#include "lex.h"
#include "parse.h"
#include "parsetree.h"

using namespace std;
int line = 0;
istream *input = &cin;
ifstream inFile;

#include <typeinfo>
#include <cxxabi.h>
string demangle(const char* name) {
    int status;
    char* demangledName = abi::__cxa_demangle(name, nullptr, nullptr, &status);
    string returnValue = string(demangledName);
    free(demangledName);
    return returnValue;
}

void debugPrint(ParseTree* tree, unsigned int level = 0) {
    const string className = demangle(typeid(*tree).name());
    ParseTree* currTree = tree->left;
    cout << string(level, '\t') << className << endl;
    if (currTree)
        debugPrint(currTree, level+1);
    currTree = tree->right;
    if (currTree)
        debugPrint(currTree, level + 1);
}

int main(int argc, const char * argv[]) {
    
    for(int i =1; i<argc; i++){
        
        string vari(argv[i]);
        
            if (i == argc-1){
            
            inFile.open(vari);
            
            if( inFile.is_open() == false ) {
                cerr << "COULD NOT OPEN "<< vari << endl;
                return -1;
            }
            input = &inFile;
        }
        
        else{
            
            cout << "TOO MANY FILENAMES" <<endl;
            return -1;
        }
        
        
        
    }
    
    ParseTree *tree = Prog(*input,line);
    if(tree ==0){
    //null 
    
    }
    
    if(tree !=0){
        //traverse tree 
        
        cout<<"BANG COUNT: " << tree->BangCount()<<endl;
        cout<<"MAX DEPTH: " << tree->maxdepth()<<endl;
        
       
    }
 
    //debugPrint(tree);
    return 0;
}
