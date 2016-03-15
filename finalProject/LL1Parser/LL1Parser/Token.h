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

	//Functions for adding to collectiongs
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

	void addRuleFirstSet(string startToken, list<string> rule) {
		//Add a new entry if it doesn't exist
		list<list<string>> ruleSet;
		if (ruleFirstSet.count(startToken) == 0)
		{			
			ruleSet.push_back(rule);
			ruleFirstSet.insert(make_pair(startToken, ruleSet));
		} else //Update an existing entry
		{
			ruleSet = ruleFirstSet.at(startToken);
			//Check if the rule is already in the set
			for (auto &eachRule : ruleSet)	//Each rule in the set of rules
			{
				if (rule == eachRule)
				{
					return;	//If the rule is already in the set, don't add it
				}
			}
			ruleFirstSet.at(startToken).push_back(rule);
		}
	}

	//For easy printing
	string toString() {
		//Terminals just print out the name
		if (isTerminal)
			return "Token: " + name + " (terminal)\n";

		//Non terminals print out all of their members
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

	//The name of the token (like S)
	string name;	
	//Whether or not the token is a terminal
	bool isTerminal;
	//Whether or not the token can be lambda (obviously never true for terminals)
	bool hasLambda;
	//Any rules this token can be replaced with
	//One rule is a list of strings to make it easy to iterate through each token
	//The collection of these lists is in another list
	list<list<string>> rules;
	//All tokens that this one is dependent on (they appear in its rules)
	//Example: S -> abC would have a, b, and C as dependents
	unordered_set<string> dependencies;
	//All tokens dependent on this one
	//In the above example, S is a precendent of a, b, and C
	unordered_set<string> precendencies;
	//The first set for this token
	//For terminals, it's itself
	unordered_set<string> firstSet;
	//The follow set for this token
	//Blank for terminals
	unordered_set<string> followSet;
	//Any tokens that this one's follow set depends on the follow set of
	//Example: S -> abC, C's follow set is dependent on the follow set of S
	//because it comes at the end of the rule
	unordered_set<string> dependFollowSet;
	//Same as above but reverse; S is a precendent of C.
	unordered_set<string> precendFollowSet;
	//This is essentially an LL(1) table for a particular token in map form
	unordered_map<string, list<list<string>>> ruleFirstSet;
};

#endif