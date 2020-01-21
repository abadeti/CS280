#include <iostream>
#include <vector>
#include<stdio.h> 
#ifndef VAL_H
#define VAL_H
#include "lex.h"
#include "parsetree.h"

#include <string>
using namespace std;

class Val {
    int        i;
    string    s;
    enum ValType { ISINT, ISSTR, ISERR } vt;
    
public:
    Val() : i(0), vt(ISERR) {}
    Val(int i) : i(i), vt(ISINT) {}
    Val(string s) : i(0), s(s), vt(ISSTR) {}
    Val(ValType vt, string errmsg) : i(0), s(errmsg), vt(ISERR) {}
    
    ValType getVt() const { return vt; }
    
    bool isErr() const { return vt == ISERR; }
    bool isInt() const { return vt == ISINT; }
    bool isStr() const { return vt == ISSTR; }
    
    int ValInt() const {
        if( isInt() ) return i;
        throw std::logic_error("This Val is not an Int");
    }
    string ValString() const {
        if( isStr() ) return s;
        throw std::logic_error("This Val is not a Str");
    }
    
    friend ostream& operator<<(ostream& out, const Val& v) {
        switch( v.vt ) {
            case ISERR:
                out << v.s;
                break;
            case ISINT:
                out << v.i;
                break;
            case ISSTR:
                out << v.s;
                break;
        }
        
        return out;
    }
    
    string GetErrMsg() const {
        if( isErr() ) return s;
        throw std::logic_error("This Val is not an Error");
    }
   
    Val operator+(const Val& op) const {
        if( isInt() && op.isInt() )
            return ValInt() + op.ValInt();
        if( isStr() && op.isStr() )
            return ValString() + op.ValString();
        throw std::logic_error("Type mismatch on operands of +");
    }
    
    Val operator-(const Val& op) const {
    if(isInt() && op.isInt()){
        int diff = ValInt() - op.ValInt();
          return Val(diff);
         }
        
        else{
         throw std::logic_error("Type mismatch on operands of -");
            
        }
    }
    
    Val operator*(const Val& op) const {
    
    if(isInt() && op.isInt()){
          int mul = ValInt() * op.ValInt();
          return mul;
        }
        
       
        else if(isStr() && op.isInt()){
            if(op.ValInt()>=0)
            {
             string mul;
           int sp = op.ValInt();
            while(sp >0){
                mul += ValString() ;
                
                sp--;
            }
            
            return mul;
        
        }
        else
        {
            throw std::logic_error("Cannot multiply string by negative int");
        }
        }
        
        else if(op.isStr() && isInt()){
             string mul;
           int sp = ValInt();
            while(sp >0){
                mul += op.ValString() ;
                
                sp--;
            }
            
            return mul;
        
        }
        
        else{
         throw std::logic_error("Type mismatch on operands of *");
            
        }
        
         return Val();
    
    }
    
    
    Val operator/(const Val& op) const {
        if(isInt() && op.isInt() && op.ValInt() != 0){
        int diff = ValInt() / op.ValInt();
          return Val(diff);
         }
        
        else{
         throw std::logic_error("Divide by zero error");
            
        }
    }
    
    
    Val operator!() const {
		string save = "";
        bool isneg = false;
        if(isInt())
        {
            int work = ValInt();
            if(work < 0)
            {
                work = work * -1;
                isneg = true;
            }
            save = to_string(work);
        }
        else
        {
            save = ValString();
        }
        
        string rv = "";
        for(int i = save.length()-1; i >= 0 ; i--)
        {
            rv += save[i];
        }
        
        if(isInt())
        {
            int works = stoi(rv);
            if(isneg)
                works = works * -1;
            return Val(works);
                           
        }
        else
        {
            return Val(rv);
        }
	}
};

#endif
