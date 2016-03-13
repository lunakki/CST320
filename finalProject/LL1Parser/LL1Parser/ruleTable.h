#ifndef RULETABLE_H
#define RULETABLE_H

#include "Token.h"
#include <unordered_map>

using namespace std;

class RuleTable{
public:
	void addToken(Token token);
	void addToken(string token);
	Token getToken(string name);
	bool deleteToken(string name);
	void addRule(string name, list<string> rule);
	void addDependency(string name, string dependency);
	bool removeRule(string name, list<string> rule);
	string toString();
	bool containsToken(string name);
	void clear();

private:
	unordered_map<std::string,Token> tokenTable;	//Map for holding tokens and their properties

};

#endif