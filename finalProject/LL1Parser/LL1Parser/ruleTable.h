#ifndef RULETABLE_H
#define RULETABLE_H

#include <string>
#include <unordered_map>
#include <list>

using namespace std;

//Tokens to be held in the table
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
		rules.push_back(rule);
	}

	string toString() {
		string output = "Token: " + name + "\n\tIs terminal: ";
		if (isTerminal)
			output += "yes";
		else
			output += "no";
		
		output += "\n\tHas lambda rule: ";
		if (hasLambda)
			output += "yes";
		else
			output += "no";
		output += "\n\tRules: \n";
		for (list<list<string>>::iterator itRules = rules.begin(); itRules != rules.end(); ++itRules)
		{
			output += "\t\t";
			for (list<string>::iterator itOneRule = itRules->begin(); itOneRule != itRules->end(); ++itOneRule)
			{
				output += *itOneRule + " ";
			}
			output += "\n";
		}
		return output;
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
	void addRule(string name, list<string> rule);
	bool removeRule(string name, list<string> rule);
	string toString();
	bool containsToken(string name);
	void clear();

private:
	unordered_map<std::string,Token> tokenTable;	//Map for holding tokens and their properties

};

#endif