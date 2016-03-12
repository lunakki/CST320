#ifndef DESCENT_PARSER_H
#define DESCENT_PARSER_H

#include <deque>
#include <string>
#include "utils.h"

class DescentParser {
public:
	bool parse(deque<Token> inTokens, string error);

private:
	deque<Token> tokens;
	deque<Token> tempTokens;
	Token currentToken;
	
	void removeToken(int &count);
	void replaceTokens(int count);

	bool program(int &parentCount);
	bool funcProto(int &parentCount);
	bool type(int &parentCount);
	bool params(int &parentCount);
	bool multiParams(int &parentCount);
	bool mTypeParams(int &parentCount);
	bool function(int &parentCount);
	bool statList(int &parentCount);
	bool stat(int &parentCount);
	bool mIdStat(int &parentCount);
	bool expression(int &parentCount);
	bool mExpression(int &parentCount);
	bool mOptArgs(int &parentCount);
	bool exprRecurs(int &parentCount);
	bool rElse(int &parentCount);
	bool moreElse(int &parentCount);
	bool mIdProcCall(int &parentCount);
	bool args(int &parentCount);
	bool mExpArgs(int &parentCount);
	bool varDecl(int &parentCount);
	bool mTypeVarDecl(int &parentCount);
	bool optExpr(int &parentCount);
};
#endif