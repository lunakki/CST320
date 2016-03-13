#ifndef RULETABLE_H
#define RULETABLE_H

#include <string>
#include <unordered_map>
#include <list>

using namespace std;

//Enum for the type; safer than a string
enum Type {
	T_Int, T_Float, T_String
};

//Enum for the use; safer than a string
enum Use {
	U_Var, U_Const, U_Func
};

//Symbols to be held in the the symbol table
struct Token {
	//Constructors
	Token(string name, bool isTerminal, bool hasLambda, bool isStarting, list<string> rule) {
		this->name = name;
		this->isTerminal = isTerminal;
		this->hasLambda = hasLambda;
		this->isStarting = isStarting;
		addRule(rule);
	}
	Token(string name) {
		this->name = name;
		isTerminal = true;
		hasLambda = false;
		isStarting = false;
	}

	void addRule(list<string>rule) {
		rules.insert(rules.begin(), rule);
	}

	string toString() {
		return name;
	}

	string name;
	bool isTerminal;
	bool hasLambda;
	bool isStarting;
	list<list<string>> rules;
};

class RuleTable{
public:
	void addToken(Token token);
	void addToken(string token);
	Token getToken(string name);
	bool deleteToken(string name);
	bool addRule(string name, list<string> rule);
	bool removeRule(string name, list<string> rule);
	string toString();
	bool containsToken(string name);

private:
	unordered_map<std::string,Token> tokenTable;	//Map for holding tokens and their properties

};

#endif