#include "ruleTable.h"

using namespace std;

//Add a symbol to the table
//Will not add the symbol if the name is already in the table
void RuleTable::addToken(Token token){
	if (token.name == "lambda")
		return;

	tokenTable.insert(make_pair(token.name, token));
}

void RuleTable::addToken(string name){
	if (name == "lambda")
		return;

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
	//Don't add empty lambda as a token
	if (name == "lambda")
		return;

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

void RuleTable::addDependency(string name, string dependency){
	//Don't add empty lambda as a token
	//Also don't add a token as its own dependent
	if (name == "lambda" || dependency == "lambda" || name == dependency)
		return;

	if (!containsToken(name))
	{
		addToken(name);
	}
	tokenTable.at(name).addDependency(dependency);
}

void RuleTable::addPrecendency(string name, string precendency){
	//Don't add empty lambda as a token
	//Also don't add a token as its own precendent
	if (name == "lambda" || precendency == "lambda" || name == precendency)
		return;

	if (!containsToken(name))
	{
		addToken(name);
	}
	tokenTable.at(name).addPrecendency(precendency);
}

void RuleTable::addToFirst(string name, string token) {
	if (name == "lambda" || token == "lambda" || name == token)
		return;

	if (!containsToken(name))
	{
		addToken(name);
	}
	tokenTable.at(name).addFirstSet(token);
}

void RuleTable::updateToken(Token token)
{
	try {
		tokenTable.at(token.name) = token;
	} catch (exception e) {
		return;
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

void RuleTable::clear() {
	tokenTable.clear();
}

void RuleTable::setStartingToken(string name) {
	start = name;
}

string RuleTable::getStartingToken() {
	return start;
}
