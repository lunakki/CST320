#ifndef TOKEN_H
#define TOKEN_H

#include <string>
#include <list>
#include <unordered_set>
#include <unordered_map>

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

	void addPrecendency(string precendency) {
		precendencies.insert(precendency);
	}

	void addFirstSet(string token) {
		firstSet.insert(token);
	}

	void addFollowSet(string token) {
		followSet.insert(token);
	}

	void addSharesFollowSet(string token) {
		dependFollowSet.insert(token);
	}

	void addPrependFollowSet(string token) {
		precendFollowSet.insert(token);
	}

	void addRuleFollowSet(string startToken, list<string> rule) {
		//Add a new entry if it doesn't exist
		list<list<string>> ruleSet;
		if (ruleFollowSet.count(startToken) == 0)
		{			
			ruleSet.push_back(rule);
			ruleFollowSet.insert(make_pair(startToken, ruleSet));
		} else //Update an existing entry
		{
			ruleSet = ruleFollowSet.at(startToken);
			//Check if the rule is already in the set
			for (auto &eachRule : ruleSet)	//Each rule in the set of rules
			{
				if (rule == eachRule)
				{
					return;	//If the rule is already in the set, don't add it
				}
			}
			ruleFollowSet.at(startToken).push_back(rule);
		}
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

		output += "\tPrecendencies: ";
		for (auto& token: precendencies)
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

		output += "\tDependent follow sets: ";
		for (auto& token: dependFollowSet)
		{
			output += token + " ";
		}
		output += "\n";
		return output;

		output += "\tPrecendent follow sets: ";
		for (auto& token: precendFollowSet)
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
	unordered_set<string> precendencies;
	unordered_set<string> firstSet;
	unordered_set<string> followSet;
	unordered_set<string> dependFollowSet;
	unordered_set<string> precendFollowSet;
	unordered_map<string, list<list<string>>> ruleFollowSet;
};

#endif