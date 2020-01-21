#include "parse.h"
#include "lex.h"

namespace Parser {
bool pushed_back = false;
Lex	pushed_token;

static Lex GetNextToken(istream& in, int& line) {
	if( pushed_back ) {
		pushed_back = false;
		return pushed_token;
	}
	return getNextToken(in, line);
}

static void PushBackToken(Lex& t) {
	if( pushed_back ) {
		abort();
	}
	pushed_back = true;
	pushed_token = t;
}

}

static map<string, bool> id;
static int error_count = 0;

void ParseError(int line, string msg)
{
	++error_count;
	cout << line << ": " << msg << endl;
}

ParseTree *Prog(istream& in, int& line)
{
	ParseTree *sl = Slist(in, line);

	if( sl == 0 )
		ParseError(line, "No statements in program");

	if( error_count )
		return 0;
    
    if(Parser::GetNextToken(in, line) != DONE )
		ParseError(line, "Unrecognized statement");

	return sl;
}

ParseTree *Slist(istream& in, int& line) {
	ParseTree *s = Stmt(in, line);
	if( s == 0 )
		return 0;
    
     if ( (Parser::GetNextToken(in, line)) != SC)
    {
        ParseError(line, "Missing Semi Colon");
         return 0;
    }

	return new StmtList(s, Slist(in,line));
}

ParseTree *Stmt(istream& in, int& line) {
    ParseTree *state = 0;
    Lex t = getNextToken(in, line);
    
    switch (t.GetToken()){
        case (LET):
            state = LetStmt(in, line);
            break;
            
        case (IF):
            state = IfStmt(in, line);
            break;
            
        case (PRINT):
            state = PrintStmt(in, line);
            break;
            
        case (LOOP):
            state = LoopStmt(in, line);
            break;
            
        case (ERR):
            ParseError(line, "UNDECLARED VARIABLE");
            break;
            
        case (DONE):
            break;
            
        case (SC):
            state = Stmt(in,line);
            break;
            
        default:
            Parser::PushBackToken(t);
            break;
    }
    return state;
}

ParseTree *LetStmt(istream& in, int& line) {
    Lex le = Parser::GetNextToken(in, line);
    if (le != ID)
    {
        ParseError(line, "Missing ID after Let");
        return 0;
    }
    string expr = le.GetLexeme();
    id.insert({expr, true});
    ParseTree *exp = Expr(in, line);
	if( exp == 0 ) {
		ParseError(line, "Missing expression after ID");
		return 0;
	}
    if(exp->IsIdent() && id.count(exp->GetId()) == 0)
    {
        cout << "UNDECLARED VARIABLE " << exp->GetId() << endl;
    }
    return new Let(le, exp);
}

ParseTree *PrintStmt(istream& in, int& line) {
    ParseTree *exp = Expr(in, line);
	if( exp == 0 ) {
		ParseError(line, "Missing expression");
		return 0;
	}
    if(exp->IsIdent() && id.count(exp->GetId()) == 0)
    {
        cout << "UNDECLARED VARIABLE " << exp->GetId() << endl;
    }
    return new Print(line, exp);
}

ParseTree *LoopStmt(istream& in, int& line) {
    ParseTree *exp = Expr(in, line);
	if( exp == 0 ) {
		ParseError(line, "Missing expression");
		return 0;
	}
    if(exp->IsIdent() && id.count(exp->GetId()) == 0)
    {
        cout << "UNDECLARED VARIABLE " << exp->GetId() << endl;
    }
    Lex b = Parser::GetNextToken(in, line);
    if (b != BEGIN)
    {
        ParseError(line, "Missing expression");
        return 0;
    }
    ParseTree *stringList = Slist(in, line);
	if( stringList == 0 ) {
		ParseError(line, "Missing expression");
		return 0;
	}
    Lex end = Parser::GetNextToken(in, line);
    if (end != END)
    {
        ParseError(line, "Missing expression");
        return 0;
    }
    return new Loop(exp, b, stringList,end );
}

ParseTree *IfStmt(istream& in, int& line) {
    ParseTree *exp = Expr(in, line);
	if( exp == 0 ) {
		ParseError(line, "Missing ID");
		return 0;
	}
    if(exp->IsIdent() && id.count(exp->GetId()) == 0)
    {
        cout << "UNDECLARED VARIABLE " << exp->GetId() << endl;
    }
    Lex b = Parser::GetNextToken(in, line);
    if (b != BEGIN)
    {
        ParseError(line, "Missing BEGIN");
        return 0;
    }
    ParseTree *stringList = Slist(in, line);
	if( stringList == 0 ) {
		ParseError(line, "Missing BEGIN");
		return 0;
	}
    Lex end = Parser::GetNextToken(in, line);
    if (end != END)
    {
        ParseError(line, "Missing END");
        return 0;
    }
    return new If(exp,b,stringList,end);
}

ParseTree *Expr(istream& in, int& line) {
	ParseTree *t1 = Prod(in, line);
	if( t1 == 0 ) {
		return 0;
	}

	while ( true ) {
		Lex t = Parser::GetNextToken(in, line);

		if( t != PLUS && t != MINUS ) {
			Parser::PushBackToken(t);
			return t1;
		}

		ParseTree *t2 = Prod(in, line);
		if( t2 == 0 ) {
			ParseError(line, "Missing expression");
			return 0;
		}

		if( t == PLUS )
			t1 = new PlusExpr(t.GetLinenum(), t1, t2);
		else
			t1 = new MinusExpr(t.GetLinenum(), t1, t2);
	}
}

ParseTree *Prod(istream& in, int& line) {
    ParseTree *t1 = Rev(in, line);
	if( t1 == 0 ) {
		return 0;
	}

	while ( true ) {
		Lex t = Parser::GetNextToken(in, line);

		if( t != STAR && t != SLASH ) {
			Parser::PushBackToken(t);
			return t1;
		}

		ParseTree *t2 = Rev(in, line);
		if( t2 == 0 ) {
			ParseError(line, "Missing expression");
			return 0;
		}

		if( t == STAR )
			t1 = new TimesExpr(t.GetLinenum(), t1, t2);
		else
			t1 = new DivideExpr(t.GetLinenum(), t1, t2);
	}
}

ParseTree *Rev(istream& in, int& line) {
    Lex t = Parser::GetNextToken(in, line);
	if(t==BANG)
    {
        Lex t2 = Parser::GetNextToken(in, line);
        if(t2 == BANG)
        {
            ParseTree *r = Rev(in, line);
            if(r==0)
            {
                return 0;
            }
            else
            {
                return r;
            }
        }
        else
        {
            Parser::PushBackToken(t2);
            ParseTree *t1 = Primary(in, line);
            if(t1 == 0)
            {
                return 0;
            }
            else
            {
                return new BangExpr(line, t1);
            }
        }
    }
    else
    {
        Parser::PushBackToken(t);
        ParseTree *t1 = Primary(in, line);
            if(t1 == 0)
            {
                return 0;
            }
            else
            {
                return t1;
            }
    }
	
}

ParseTree *Primary(istream& in, int& line) {
	Lex t = Parser::GetNextToken(in, line);

	if( t == ID ) {
		return new Ident(t);
	}
	else if( t == INT ) {
		return new IConst(t);
	}
	else if( t == STR ) {
		return new SConst(t);
	}
	else if( t == LPAREN ) {
		ParseTree *ex = Expr(in, line);
		if( ex == 0 ) {
			ParseError(line, "Missing expression after (");
			return 0;
		}
		if( Parser::GetNextToken(in, line) == RPAREN )
			return ex;

		ParseError(line, "Missing ) after expression");
		return 0;
	}

	ParseError(line, "Primary expected");
	return 0;
}
