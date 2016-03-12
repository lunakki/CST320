#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include <stack>
#include <fstream>
#include <string>
#include <iomanip>
#include "utils.h"

class Parser{
public:
	bool parse(string fileName, stack<Token> &outTokenStack, string &error);
	bool parseString(string toParse, stack<Token> &outTokenStack, string &error);
	Parser(){};
private:
	bool parse();
	void resetVariables();
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

#endif