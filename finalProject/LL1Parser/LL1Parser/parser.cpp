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
	calculateFirstSet(parents, table.getStartingToken(), true);
	calculateFollowSet(parents, table.getStartingToken());

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
	int firstSetSize = token.firstSet.size();

	//A terminal's "first set" is only itself
	//Also it has no dependencies to deal with
	if (token.isTerminal)
	{
		token.addFirstSet(name);
		table.updateToken(token);
		return;
	}

	//This makes sure all tokens get calculated at least once
	//And also checks the dependents before the prependents (since we need their info first)
	//Can be turned off if a token is being recalculated (dependency isn't always linear)
	if (checkDependents)
		for (auto& x : token.dependencies)
		{
			//Don't call this if it's in the parents to prevent endless loops
			if (parents.count(x) == 0)
				calculateFirstSet(parents, x, true);
		}

	//Go through all the rules and calculate the first set based on each
	for (auto& aRule : token.rules)	//Each rule
	{
		for (auto& aToken : aRule)	//Each token in the rule
		{
			for (auto& aFirst : table.getToken(aToken).firstSet) //Each item in the first set of the token
			{
				token.addFirstSet(aFirst);
			}
			//We only care about the tokens that could be first
			//Don't check the next token if this one can't be lambda
			if (!table.getToken(aToken).hasLambda)
			{
				break;
			}
		}
	}

	//If items were added to the first set, recheck items dependent on this that aren't
	//already waiting to be checked
	//This is because two tokens may be dependent on each other
	//and we don't know which will be figured out first
	if (firstSetSize != token.firstSet.size())
	{
		table.updateToken(token);
		for (auto& x : token.prependencies)
		{
			//Don't call this if it's in the parents to prevent endless loops
			if (parents.count(name) == 0)
				calculateFirstSet(parents, x, false);
		}
	}
}

void GrammarParser::calculateFollowSet(unordered_set<string> parents, string name)
{
	Token token = table.getToken(name);
	unordered_set<string> dependencies = token.dependencies;
	list<list<string>> rules = token.rules;
	int firstSetSize = token.firstSet.size();

	//We don't care about terminals
	if (token.isTerminal)
	{
		return;
	}

	parents.insert(name);
	//This makes sure all tokens get calculated at least once
	//Skips anything that has already been checked (in parents)
	for (auto& x : token.dependencies)
	{
		//Don't call this if it's in the parents to prevent endless loops
		if (parents.count(x) == 0)
			calculateFollowSet(parents, x);
	}

	for (auto& aRule : token.rules)	//Each rule
	{
		for (list<string>::iterator itRule = aRule.begin(); itRule != aRule.end(); ++itRule)	//Each token in the rule
		{
			//We don't care about terminals
			if (!table.getToken(*itRule).isTerminal) 
			{
				//Iterate through the rest of the rule and see what all follows this token
				list<string>::iterator itRule2 = itRule;
				Token currToken = table.getToken(*itRule);
				for (++itRule2; itRule2 != aRule.end(); ++itRule2)
				{
					
					//Add everything in the first set of the following token to this token
					for (auto& aFirst : table.getToken(*itRule2).firstSet) //Each item in the first set of the token
					{
						currToken.addFollowSet(aFirst);
					}
					
					//The next token can follow this one only if this is lambda
					if (!table.getToken(*itRule2).hasLambda)
					{
						break;
					}
				}
				//If we got to the end of the rule then this token shares the follow set of
				//the token that this rule belongs to
				//Note that token so we can figure it out later
				if (itRule2 == aRule.end())
				{
					currToken.addSharesFollowSet(name);
				}
				table.updateToken(currToken);
			}
		}
	}
}