#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <stack>

using namespace std;

const string KEYWORDS[] = {"for", "while", "void", "else", "int", "float", "string", "main"};

//Used to store information about the type of token
enum TokenType {
	TT_Empty, TT_Identifier, TT_Preprocessor, TT_StringLiteral, TT_Integer, TT_Float, TT_Operator, TT_Symbol, TT_Keyword, TT_Comment
};

//To track the current state of the token being worked with
enum TokenState {
	TS_NoToken, TS_Identifier, TS_Integer, TS_CompleteFloat, TS_CompleteString, TS_LessThan, TS_GreaterThan, TS_Equal,
	TS_CompleteOperator, TS_IncompleteFloat, TS_IncompleteString, TS_IncompleteAnd, TS_IncompleteOr, 
	TS_Directive, TS_Not, TS_Division, TS_Comment
};

//To store a token
struct Token {
	string token;
	TokenType type;
	Token(string token, TokenType type) {
		this->token = token;
		this->type = type;
	}
	Token(char token, TokenType type) {
		this->token = token;
		this->type = type;
	}
};

#endif