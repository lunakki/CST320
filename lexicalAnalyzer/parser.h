#include <iostream>
#include <stack>
#include <fstream>
#include <string>
#include <iomanip>

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

class Parser{
public:
	bool parse(string fileName, stack<Token> &outTokenStack, string &error);
	Parser();
private:
	bool AddCharNoToken();
	bool AddCharIdentifier();
	bool AddCharInteger();
	bool AddCharCompleteFloat();
	bool AddCharCompleteString();
	bool AddCharLessThan();
	bool AddCharGreaterThan();
	bool AddCharEqual();
	bool AddCharCompleteOperator();
	bool AddCharDirective();
	bool AddCharIncompleteFloat();
	bool AddCharIncompleteString();
	bool AddCharIncompleteAnd();
	bool AddCharIncompleteOr();
	bool AddCharNot();
	bool AddCharDivision();
	bool AddCharComment();
	bool CheckForTokenEnd(TokenType type);
	bool IsKeyword(string token);

	char currentChar;
	TokenState currentState;
	stack<Token> tokenStack;
	string currentToken;
};