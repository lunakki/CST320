#include "ruleTable.h"

using namespace std;

//Add a symbol to the table
//Will not add the symbol if the name is already in the table
void RuleTable::addToken(Token token){
	tokenTable.insert(make_pair(token.name, token));
}

void RuleTable::addToken(string name){
	tokenTable.insert(make_pair(name, Token(name)));
}


//Get the symbol from the table that matches a name
//Will throw an exception if not found
Token RuleTable::getToken(string name){
	return tokenTable.at(name);
}

//Deletes a symbol with a given name
//Returns true if successfull and false if not
bool RuleTable::deleteToken(string name){
	return tokenTable.erase(name) == 1;
}

//Adds a rule to an existing token
//Creates the token if it doesn't exist
//Sets it to a non terminal in either case
//If the rule is lambda, it sets the token's hasLambda property instead
void RuleTable::addRule(string name, list<string> rule){
	if (!containsToken(name))
	{
		addToken(name);
	}
	tokenTable.at(name).isTerminal = false;
	if (rule.front() == "lambda")
	{
		tokenTable.at(name).hasLambda = true;
	} else {
		tokenTable.at(name).addRule(rule);
	}
}

//Converts the tokenTable to a string for easy printing
string RuleTable::toString() {
	string output = "";

	if (tokenTable.empty())
		return "Empty";

	for (auto& x: tokenTable)
		output += x.second.toString();

	return output;
}

bool RuleTable::containsToken(string name) {
	try {
		tokenTable.at(name);
		return true;
	} catch (exception e) {
		return false;
	}
}
