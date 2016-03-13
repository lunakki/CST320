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
	void updateToken(Token token);
	void addRule(string name, list<string> rule);
	void addDependency(string name, string dependency);
	void addPrependency(string name, string prependency);
	void addToFirst(string name, string token);
	bool removeRule(string name, list<string> rule);
	string toString();
	bool containsToken(string name);
	void clear();
	void setStartingToken(string name);
	string getStartingToken();

private:
	unordered_map<std::string,Token> tokenTable;	//Map for holding tokens and their properties
	string start;

};

#endif