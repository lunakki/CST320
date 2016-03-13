#ifndef TOKEN_H
#define TOKEN_H

#include <string>
#include <list>
#include <unordered_set>

using namespace std;

//Tokens to be held in the table
struct Token {
	//Constructors
	Token(string name, bool isTerminal, bool hasLambda, list<string> rule) {
		this->name = name;
		this->isTerminal = isTerminal;
		this->hasLambda = hasLambda;
		addRule(rule);
	}
	Token(string name) {
		this->name = name;
		isTerminal = true;
		hasLambda = false;
	}

	void addRule(list<string>rule) {
		rules.push_back(rule);
	}

	void addDependency(string dependency) {
		dependencies.insert(dependency);
	}

	void addPrependency(string prependency) {
		prependencies.insert(prependency);
	}

	void addFirstSet(string token) {
		firstSet.insert(token);
	}

	void addFollowSet(string token) {
		followSet.insert(token);
	}

	void addSharesFollowSet(string token) {
		sharesFollowSet.insert(token);
	}

	string toString() {
		if (isTerminal)
			return "Token: " + name + " (terminal)\n";

		string output = "Token: " + name + "\n\tIs terminal: no";
		
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

		output += "\tPrependencies: ";
		for (auto& token: prependencies)
		{
			output += token + " ";
		}
		output += "\n";

		output += "\tFirst set: ";
		for (auto& token: firstSet)
		{
			output += token + " ";
		}
		output += "\n";

		output += "\tFollow set: ";
		for (auto& token: followSet)
		{
			output += token + " ";
		}
		output += "\n";

		output += "\tShared follow set: ";
		for (auto& token: sharesFollowSet)
		{
			output += token + " ";
		}
		output += "\n";
		return output;
	}

	string name;
	bool isTerminal;
	bool hasLambda;
	list<list<string>> rules;
	unordered_set<string> dependencies;
	unordered_set<string> prependencies;
	unordered_set<string> firstSet;
	unordered_set<string> followSet;
	unordered_set<string> sharesFollowSet;
};

#endif