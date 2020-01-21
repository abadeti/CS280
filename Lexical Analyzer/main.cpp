#include <stdio.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <list>
#include "lex.h"


using namespace std;
using std::cout;
using std::endl;

int main(int argc,  char * argv[]) {
    int linenumber  =1;
    bool v=false;
    bool ids =false;
    bool string1 =false;
   
    istream *input = &cin;
    
    ifstream inFile;
   
    for(int i =1; i<argc; i++){
    
     string vari(argv[i]);
        
    if(vari == "-v"){ 
        v = !v;
      
    }
        else if(vari =="-ids"){
         ids = !ids;
        
        }
        
        else if(vari == "-consts"){
       string1 = !string1;
        
        }
        
       else if(vari[0] == '-'){
           cerr<< "UNRECOGNIZED FLAG " << vari <<endl;
        return -1;
       } 
        
        
       else if (i == argc-1){
           
           
        
        inFile.open(vari);
           
      if( inFile.is_open() == false ) {
        cerr << "CANNOT OPEN "<< vari << endl;
        return -1;
    } 
            input = &inFile; 
        }
        else{
         cout << "ONLY ONE FILE NAME ALLOWED" <<endl;
         return -1;
        }
    }
    Lex tok;
    int tokenCounter=0;
   
    list<string> idents;
    list<string> sconsts;
    list<int> sconstsi;
    
    while((tok = getNextToken(*input, linenumber)) != Token:: ERR && tok != Token::DONE){
        tokenCounter++;
        if(v){
           cout<<tok<<endl;
        }
        
        if(string1)
        {
            if(tok == STR)
            {
                sconsts.push_back(tok.GetLexeme());    
            }
            
            else if(tok == INT)
            {
                sconstsi.push_back(stoi(tok.GetLexeme()));    
            }
        }
        
        if(tok.GetToken() == ID){
            idents.push_back(tok.GetLexeme());
        }
    }    
    if(tok.GetToken()== Token:: ERR){
        cout<< "Error on line " << linenumber << " (" <<tok.GetLexeme()<< ")"<<endl; 
        return -1;
    }
       
     if(ids){
         if(idents.size()>0){
          idents.unique();
          idents.sort();
         
         cout<< "IDENTIFIERS: ";
          for (string n : idents) {
              
           cout << n;
              if(n.compare(idents.back())){
                  cout<<", ";
              }
           
    }
         cout<<endl;
         }
            
        }
        
        if(string1 && sconsts.size()>1){
          sconsts.unique();
          sconsts.sort();
         
         cout<< "STRINGS:" <<endl;
          for (string n : sconsts) {
              
           cout << n<< endl;
          }
        
        
            
    }
          else if(string1 && sconstsi.size()>1){
          sconsts.unique();
         sconstsi.sort();
              
         cout<< "INTEGERS:" <<endl;
          for (int n : sconstsi) {
              
           cout << n<< endl;
          }
    }
                                   
                                   
    
    
    
    cout<<"Lines: "<< linenumber-1<<endl;
    
    if(linenumber >1){
    cout<<"Tokens: " << tokenCounter <<endl;
    }
    
    return 0;
}
