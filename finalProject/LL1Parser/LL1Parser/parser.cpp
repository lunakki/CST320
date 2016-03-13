#include "parser.h"

bool GrammarParser::parse(string fileName, string &error, RuleTable &outTable)
{
	unordered_set<string> parents;
	bool success;
	table.clear();


	success = parseFile(fileName, error);
	if (!success)
		return success;
	calculateLambdaRulesR(parents, table.getStartingToken(), true);

	outTable = table;
	return success;
}

bool GrammarParser::parseFile(string fileName, string &error) {
	fstream file;
	string tokenName, word;
	list<string> rule;
	table.clear();
	file.open(fileName);

	if (!file.is_open())
	{
		error = "Unable to open file";
		return false;
	}

	file >> tokenName;
	if (!file.eof())
	{
		table.setStartingToken(tokenName);
	} else {
		error = "Empty file: " + fileName;
		return false;
	}
	while(!file.eof())
	{
		while (file.peek() != '\n' && file.peek() != EOF)
		{
			file >> word;
			table.addToken(word);
			table.addDependency(tokenName, word);
			table.addPrependency(word, tokenName);
			rule.push_back(word);
		}
		if (!rule.empty())
		{
			table.addRule(tokenName, rule);
		} else	{
			error = "Token listed without a rule: " + tokenName;
			return false;
		}
		rule.clear();
		file >> tokenName;
	}
	return true;
}

void GrammarParser::calculateLambdaRulesR(unordered_set<string> parents, string name, bool checkDependents) {
	Token token = table.getToken(name);
	unordered_set<string> dependencies = token.dependencies;
	list<list<string>> rules = token.rules;
	parents.insert(name);

	//Don't need to check terminals
	if (token.isTerminal)
		return;

	//Check if all the tokens this one is dependent can be lambda
	//This makes sure all tokens get checked at least once
	//Can be turned off if a token is being rechecked
	if (checkDependents)
		for (auto& x : token.dependencies)
		{
			//Don't call this if it's in the parents to prevent endless loops
			if (parents.count(x) == 0)
				calculateLambdaRulesR(parents, x, true);
		}

	//Go through all the rules and check if any can go to lambda
	//Stop as soon as we find one
	if (!token.hasLambda)
	{
		for (auto& aRule : token.rules)
		{
			bool isLambda = true;
			for (auto& aToken : aRule)
			{
				//Check if all of the tokens in this rule can go to lambda
				//If one can't, break out and note it
				if (!table.getToken(aToken).hasLambda)
				{
					isLambda = false;
					break;
				}
			}
			//If all went to lambda in a rule then this token can too
			if (isLambda)
			{
				token.hasLambda = true;
				table.updateToken(token);
				break;
			}
		}
	}
	else //It already could be lambda, don't need to do anything else
	{
		return;
	}

	//If it changed from false to true, need to recheck anything dependent on it that isn't going
	//to be checked after this is run
	//This is because two tokens may be dependent on each other
	//and we don't know which will be figured out first
	if (token.hasLambda)
	{
		for (auto& x : token.prependencies)
		{
			//Don't call this if it's in the parents to prevent endless loops
			if (parents.count(name) == 0)
				calculateLambdaRulesR(parents, x, false);
		}
	}
}

void GrammarParser::calculateFirstSet(unordered_set<string> parents, string name, bool checkDependents)
{
	Token token = table.getToken(name);
	unordered_set<string> dependencies = token.dependencies;
	list<list<string>> rules = token.rules;
	parents.insert(name);

	//Don't need to check terminals
	if (token.isTerminal)
		return;

	//Check if all the tokens this one is dependent can be lambda
	//This makes sure all tokens get checked at least once
	//Can be turned off if a token is being rechecked
	if (checkDependents)
		for (auto& x : token.dependencies)
		{
			//Don't call this if it's in the parents to prevent endless loops
			if (parents.count(x) == 0)
				calculateLambdaRulesR(parents, x, true);
		}

	//Go through all the rules and check if any can go to lambda
	//Stop as soon as we find one
	if (!token.hasLambda)
	{
		for (auto& aRule : token.rules)
		{
			bool isLambda = true;
			for (auto& aToken : aRule)
			{
				//Check if all of the tokens in this rule can go to lambda
				//If one can't, break out and note it
				if (!table.getToken(aToken).hasLambda)
				{
					isLambda = false;
					break;
				}
			}
			//If all went to lambda in a rule then this token can too
			if (isLambda)
			{
				token.hasLambda = true;
				table.updateToken(token);
				break;
			}
		}
	}
	else //It already could be lambda, don't need to do anything else
	{
		return;
	}

	//If it changed from false to true, need to recheck anything dependent on it that isn't going
	//to be checked after this is run
	//This is because two tokens may be dependent on each other
	//and we don't know which will be figured out first
	if (token.hasLambda)
	{
		for (auto& x : token.prependencies)
		{
			//Don't call this if it's in the parents to prevent endless loops
			if (parents.count(name) == 0)
				calculateLambdaRulesR(parents, x, false);
		}
	}
}