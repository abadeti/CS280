#include "lex.h"
#include <map>
#include <sstream>

using namespace std;

string stringToUpper(string &s)
{
   for(int i = 0; i < s.length(); i++)
  {
    s[i] = toupper(s[i]);
  }
   return s;
}

 map <string, Token > TokenMap = {
        { "PRINT", PRINT},
        { "LET",LET},
        { "IF",IF},
        { "LOOP",LOOP},
        { "BEGIN",BEGIN},
        { "END", END},
        { "ID",ID},
        { "INT",INT},
        { "STR", STR},
        { "PLUS", PLUS},
        { "MINUS", MINUS},
        { "STAR", STAR},
        { "SLASH", SLASH},
        { "BANG", BANG},
        { "LPAREN", LPAREN},
        { "RPAREN", RPAREN},
        { "SC", SC},
        { "ERR", ERR},
        { "DONE", DONE}
    };

map < Token,string > TokenMap2 = {
        {  PRINT, "PRINT"},
        {  LET, "LET"},
        {  IF, "IF"},
        {  LOOP, "LOOP"},
        {  BEGIN, "BEGIN"},
        {  END, "END"},
        {  ID, "ID"},
        {  INT, "INT"},
        {  STR, "STR"},
        {  PLUS, "PLUS"},
        {  MINUS, "MINUS"},
        {  STAR, "STAR"},
        {  SLASH, "SLASH"},
        {  BANG, "BANG"},
        {  LPAREN, "LPAREN"},
        {  RPAREN, "RPAREN"},
        {  SC, "SC"},
        {  ERR, "ERR"},
        {  DONE, "DONE"}
    };
    
ostream& operator <<(ostream& out, const Lex& tok){

    Token tt = tok.GetToken();
      out<<TokenMap2[tt];
    if(tt== ID || tt==INT || tt==STR || tt== ERR){
      out<< "(" << tok.GetLexeme() <<")";
    }
    
    return out;
}
 
Lex getNextToken(istream& in, int& linenumber){    
    char ch;
    string lexeme;
    
    enum  LexState  {BEGIN, IDENT, INT, STR, ERR, DONE, COMMENT} state = BEGIN; 
    
    while(in.get(ch)){ 
        
     switch(state){
             
         case BEGIN:
             
             if(isalpha(ch)){
                 lexeme += ch;
                 state = IDENT;
             
             }
             
             if(isdigit(ch)){
                 lexeme += ch;
                 state = INT;
                 
             }
             
             if(ch == '"'){
                 state = STR;
              
             }
              
            if(ch == '+' ){ 
                lexeme += ch;
               return Lex(PLUS, lexeme, linenumber);
                
            }
             
            if(ch == '-'){
                lexeme += ch;
                return Lex(MINUS, lexeme, linenumber);
                }
             
            if(ch == '*'){
                lexeme += ch;
              return Lex(STAR, lexeme, linenumber);
                lexeme.clear();
            
            }
            
            if(ch == '/'){ 
                if (in.peek() == '/') {
                    state = COMMENT;
                    }
            
                else
                {
                    lexeme += ch;
                    return Lex(SLASH, lexeme, linenumber);
                    lexeme.clear();
                    
                 
             }
            }
             
            if(ch == '('){
                lexeme += ch;
                return Lex(LPAREN, lexeme, linenumber);
                lexeme.clear();
            
            }
             
            if(ch == ')'){
            lexeme += ch;
               return Lex(RPAREN, lexeme, linenumber);
                lexeme.clear();
            }
             
             if(ch == '.'){
                lexeme.clear();
               lexeme += ch;
               return Lex(Token::ERR, lexeme, linenumber);
                lexeme.clear();
            }
             
              if(ch == '_'){
            lexeme += ch;
               return Lex(Token::ERR, lexeme, linenumber);
                lexeme.clear();
            }
               
             if(ch == '$'){
            lexeme += ch;
               return Lex(Token::ERR, lexeme, linenumber);
                lexeme.clear();
            }
            if(ch == ';'){
                lexeme += ch;
                return Lex(SC, lexeme, linenumber);
            }
             
            if(ch == '!'){
                lexeme += ch;
                return Lex(BANG, lexeme, linenumber);
                lexeme.clear();
            }
             
            if(ch == '\n'){
                linenumber++;
                lexeme.clear();
                
            }
    
            break;
                   
         case IDENT:
             if(isalpha(ch) || isdigit(ch)){
                 lexeme +=ch;
                 if(!isalnum(in.peek())) {
                    
                 }
                 
             }
             else {     
                 
                 
                 in.putback(ch);
                
                string lex2 = lexeme;
                
                 if (TokenMap.count(stringToUpper(lex2)) > 0){   
                   
                     return Lex(TokenMap[lex2], lexeme, linenumber);
                     
                 }
                 
                 else{ 
                     
                 
              return Lex(Token::ID, lexeme, linenumber);                  
                 }
             }             
            break;
             
         case INT:
             if(isdigit(ch)){
                 lexeme += ch;
             }
             
             else if(isalpha(ch)){
                 lexeme +=ch;
               
                return Lex(Token::ERR, lexeme, linenumber);
                
             }
            
             else{ 
                in.putback(ch);
          return Lex(Token:: INT, lexeme, linenumber);

             }
            
            break;
             
         case STR:
             if(ch == '"'){
             
           return Lex(Token::STR, lexeme, linenumber);
        
             }
             else if(ch == '\\')
             {
                 if(isalpha(in.peek()) || isspace(in.peek()) || ispunct(in.peek())){
                     if(in.peek() == 'n'){
                         in.get(ch);
                         lexeme += '\n';
                     }
                     
                     else{
                         in.get(ch);
                         lexeme +=ch;
                     }
                 
                 }
                 else{
                 lexeme += "\\";
                 
                 }
                 
             
             }
                 /**
                 if(in.peek() == '\\')
                 {
                     lexeme+='\\';
                 }
                 if(in.peek() == 'n')
                 {
                     lexeme += '\n';
                 }
                 in.get();
                 **/
             
             else if(ch == '\n')
             {
                 lexeme +=ch;
                 lexeme = '"' + lexeme;
                 return Lex(Token::ERR, lexeme, linenumber);
             }
             else
             {

             lexeme += ch;
             }
             
            break;
         
         case COMMENT:
             {
                 if(ch == '\n'){
                     linenumber++;
                     state = BEGIN;
                 }
                 char get();
             }
             break;

         case DONE:
            break;
             
         case ERR:
             {
                 return Lex(Token::ERR, lexeme, linenumber);
             }
             break;
             
         }
    
    }
    
   
   if(in.eof()){
               return Lex(Token::DONE, lexeme, linenumber);
            }
    
return Lex();
    
}
