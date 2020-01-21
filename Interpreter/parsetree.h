/*
 * parsetree.h
 */

#ifndef PARSETREE_H_
#define PARSETREE_H_

#include <vector>
#include <map>
#include "valSKEL.h"
using std::vector;
using std::map;

extern map<string,Val> symbols;

// NodeType represents all possible types
enum NodeType { ERRTYPE, INTTYPE, STRTYPE };

// a "forward declaration" for a class to hold values
class Value;

class ParseTree {
	int			linenum;
	ParseTree	*left;
	ParseTree	*right;

public:
	ParseTree(int linenum, ParseTree *l = 0, ParseTree *r = 0)
		: linenum(linenum), left(l), right(r) {}

	virtual ~ParseTree() {
		delete left;
		delete right;
	}

	int GetLineNumber() const { return linenum; }
    ParseTree *GetLeft() const { return left; }
	ParseTree *GetRight() const { return right; }

	int Traverse(int input, int (ParseTree::*func)(void)) {
		if( left ) input += left->Traverse(input, func);
		if( right ) input += right->Traverse(input, func);
		return Visit(input, func);
	}

	virtual int Visit(int input, int (ParseTree::*func)(void)) {
		return input;
	}

	int MaxDepth() const {
        int lc = 0, rc = 0;
		if( left ) lc = left->MaxDepth();
		if( right ) rc += right->MaxDepth();
		if( lc > rc ) return lc + 1;
        return rc + 1;
	}

	virtual bool IsIdent() const { return false; }
	virtual bool IsVar() const { return false; }
	virtual string GetId() const { return ""; }
    virtual int IsBang() const { return 0; }
    virtual bool IsLet() const { return false; }
    virtual Val Eval(map<string,Val>& syms) const = 0;

	int BangCount() const {
		int cnt = 0;
		if( left ) cnt += left->BangCount();
		if( right ) cnt += right->BangCount();
		cnt += IsBang();
		return cnt;
	}

	int CheckLetBeforeUse(map<string,Val>& var) {
        int cnt = 0;
		if( left ) cnt += left->CheckLetBeforeUse(var);
		if( right ) cnt += right->CheckLetBeforeUse(var);
		if( IsLet() )
			var[ this->GetId() ] = true;
        if( IsIdent() && var.find(GetId()) == var.end() ) {
            cout << "UNDECLARED VARIABLE " << GetId() << endl;
            cnt++;
        }
        return cnt;

	}

	//virtual Value Eval();
};

class StmtList : public ParseTree {

public:
	StmtList(ParseTree *l, ParseTree *r) : ParseTree(0, l, r) {}
    
    Val Eval(map<string,Val>& symbols)  const override {
         
         //left child 
         
         GetLeft()->Eval(symbols);
         
        
         if(GetRight()){ 
             
           GetRight()->Eval(symbols);
             
         } 
       
 
         
         return Val(); 
    
     }

//	int Visit(int input, int (ParseTree::*func)(void)) {
//		for( auto s : statements ) {
//			input += s->Visit(input, func);
//		}
//		return input;
//	}
};

class Let : public ParseTree {
	string id;

public:
	Let(Lex& t, ParseTree *e) : ParseTree(t.GetLinenum(), e), id(t.GetLexeme()) {}
    Val Eval(map<string,Val>& symbols)  const override {
         if(IsLet())
         {
             if(symbols.count(GetId()) && !symbols.empty())
                {
                    Val right = GetLeft()->Eval(symbols);
                    string left = GetId();
                    symbols[left] = right;
                }
                else
                {
                    Val right = GetLeft()->Eval(symbols);
                    string left = GetId();
                    symbols.insert(std::pair<string,Val>(left,right));
                }
         }
                
         return Val(); 
    
     }
    
	string GetId() const { return id; }
	bool IsLet() const { return true; }
};

class Print : public ParseTree {
public:
	Print(int line, ParseTree *e) : ParseTree(line, e) {}
    Val Eval(map<string,Val>& symbols)  const override { 
         
         
        Val val = GetLeft()->Eval(symbols);
       
        
         
         
               cout <<val;
             
        
         
         return Val(val); 
     
     }
};

class Loop : public ParseTree {

public:
	Loop(int line, ParseTree *e, ParseTree *s) : ParseTree(line, e, s) {}
    
    Val Eval(map<string,Val>& symbols)  const override { 
         
      
        
         while(GetLeft()->Eval(symbols).ValInt() != 0){
              GetRight()->Eval(symbols);
             
         }
         
         
         return Val(); 
     
     }
};

class If : public ParseTree {

public:
	If(int line, ParseTree *e, ParseTree *s) : ParseTree(line, e, s) {}
    Val Eval(map<string,Val>& symbols)  const override { 
         
        Val val= GetLeft()->Eval(symbols);
        if(!(val.isInt())){
            cout << "RUNTIME ERROR at " << GetLineNumber() << ": Expression is not an integer" << endl; 
            exit(0);
         }
        
         else{
             if(val.ValInt() != 0){
              GetRight()->Eval(symbols);
        
          
         
         }
         }
         return Val();
     }
};


class PlusExpr : public ParseTree {
public:
	PlusExpr(int line, ParseTree *l, ParseTree *r) : ParseTree(line,l,r) {}
    Val Eval(map<string,Val>& symbols)  const override{
        auto L = GetLeft()->Eval(symbols);
        if( L.isErr() )
            cout << "RUNTIME ERROR";
        
        auto R = GetRight()->Eval(symbols);
        if( R.isErr() )
            cout << "RUNTIME ERROR";

        auto answer = L + R;
        if( answer.isErr() )
            cout << "RUNTIME ERROR";
        
        return answer;
    }
};

class MinusExpr : public ParseTree {
public:
	MinusExpr(int line, ParseTree *l, ParseTree *r) : ParseTree(line,l,r) {}
    Val Eval(map<string,Val>& symbols)  const override { 
      
        
        auto a1 = GetLeft()->Eval(symbols);
        auto a2 = GetRight()->Eval(symbols);
        return a1 - a2; // uses overloaded operator-

    }
};

class TimesExpr : public ParseTree {
public:
	TimesExpr(int line, ParseTree *l, ParseTree *r) : ParseTree(line,l,r) {}
    Val Eval(map<string,Val>& symbols)  const override { 
      
        
        auto a1 = GetLeft()->Eval(symbols);
        auto a2 = GetRight()->Eval(symbols);
        return a1 * a2; // uses overloaded operator-

     
    
    }
};

class DivideExpr : public ParseTree {
public:
	DivideExpr(int line, ParseTree *l, ParseTree *r) : ParseTree(line,l,r) {}
    Val Eval(map<string,Val>& symbols)  const override { 
        auto a1 = GetLeft()->Eval(symbols);
        auto a2 = GetRight()->Eval(symbols);
        int a = a2.ValInt();
        if(a == 0)
        {
            cout << "RUNTIME ERROR at " << GetLineNumber() << ": Divide by zero error" << endl; 
            exit(0);
        }
        else
        {
        return a1 / a2;
        }
    }
};

class BangExpr : public ParseTree {
public:
	BangExpr(int line, ParseTree *l) : ParseTree(line,l) {}
    virtual int IsBang() const { return 1; }
    Val Eval(map<string,Val>& symbols)  const override { 
             Val x = GetLeft()->Eval(symbols);
             Val res = !x;
             return Val(res);
    
    }
};

class IConst : public ParseTree {
	int val;

public:
	IConst(Lex& t) : ParseTree(t.GetLinenum()) {
		val = stoi(t.GetLexeme());
	}
    Val Eval(map<string,Val>& symbols)  const override { return Val(val); }
};

class SConst : public ParseTree {
	string val;

public:
	SConst(Lex& t) : ParseTree(t.GetLinenum()) {
		val = t.GetLexeme();
	}
    Val Eval(map<string,Val>& symbols)  const override {
       
       return Val(val);
   
   
   }
};

class Ident : public ParseTree {
	string id;

public:
	Ident(Lex& t) : ParseTree(t.GetLinenum()), id(t.GetLexeme()) {}

	Val Eval(map<string,Val>& symbols)  const override { 
        
        if(symbols.find(id)!=symbols.end()){
            return symbols[id];
        
        }
        
        return Val();    
    }    
    
	bool IsIdent() const { return true; }
	string GetId() const { return id; }
};

#endif /* PARSETREE_H_ */
