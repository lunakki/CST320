#ifndef TOKEN_H
#define TOKEN_H

#include <string>
#include <list>
#include <unordered_set>

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

	void addDependency(string dependency) {
		dependencies.insert(dependency);
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
		output += "\tDependencies: ";
		for (auto& token: dependencies)
		{
			output += token + " ";
		}
		output += "\n";
		return output;
	}

	string name;
	bool isTerminal;
	bool hasLambda;
	bool isStarting;
	list<list<string>> rules;
	unordered_set<string> dependencies;
};

#endif