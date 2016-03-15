#include "parser.h"

//Public function to parse a grammar file
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
	calculateFollowSet();

	success = createLL1Table(error);
	if (!success)
		return success;

	outTable = table;
	return success;
}

//Reads the file and stores information about each token and rule
//At the end, the table will have every token that appears in a rule or has a rule,
//All of the rules listed for each token, whether or not it's a terminal (no rules found),
//Whether any of its rules are lambda,
//which tokens are dependent on it (meaning it appears in their rules),
//and which tokens it is dependent on (they appear in its rules)
//It returns an error if the file cannot be opened, the file is empty, or
//a line has a token name but no rule
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
			table.addPrecendency(word, tokenName);
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

//Recursively goes through the tokens to figure out if they can be evaluated to lambda or not
//If it's found that an item can be lambda, the appropriate property for the token is changed in the table
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
	else //We already knew it could be lambda, don't need to do anything else
	{
		return;
	}

	//If it changed from false to true, need to recheck anything dependent on it that isn't going
	//to be checked after this is run
	//This is because two tokens may be dependent on each other
	//and we don't know which will be figured out first
	if (token.hasLambda)
	{
		for (auto& x : token.precendencies)
		{
			//Don't call this if it's in the parents to prevent endless loops
			if (parents.count(name) == 0)
				calculateLambdaRulesR(parents, x, false);
		}
	}
}

//Recursively calculates the first set for each token
//This includes checking other rules as appropriate, and looking past tokens that can be lambda
//The first set is stored in the token table
//Note that lambda is not stored in the first set; there's a separate property to track that
//It also fills out a map of which rules for each token can start with what (to generate LL(1) table later)
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
				token.addRuleFirstSet(aFirst, aRule);
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
		for (auto& x : token.precendencies)
		{
			//Don't call this if it's in the parents to prevent endless loops
			if (parents.count(name) == 0)
				calculateFirstSet(parents, x, false);
		}
	}
}

//Recursively calculates the follow set for each token by looking through every rule
//It's recursive because there's no direct way to iterate over the table (it's a private member)
//It will record both terminals following a token and the first sets of non terminals following a token
//If a token is at the end of the rule, it makes a note of it for later use
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

void GrammarParser::calculateFollowSet()
{
	unordered_set<string> parents;
	//Calculate follow sets for cases that don't depend on other follow sets
	calculateFollowSet(parents, table.getStartingToken());

	//Add "end" token to table and to the follow set of the starting token
	//We'll need it there for the next step
	table.addToken("$");
	Token startToken = table.getToken(table.getStartingToken());
	startToken.addFollowSet("$");
	table.updateToken(startToken);

	//Calculate the follow set cases involved other follow sets
	calculateFollowSetPart2(parents, table.getStartingToken(), true);

	//Update the map for which rules go with which token pairs
	addFollowToRuleMap();
}

void GrammarParser::calculateFollowSetPart2(unordered_set<string> parents, string name, bool checkDependents)
{
	Token token = table.getToken(name);
	unordered_set<string> dependencies = token.dependencies;
	int followSetSize = token.followSet.size();
	parents.insert(name);

	//Don't need to check terminals
	if (token.isTerminal)
		return;

	//This makes sure all tokens get checked at least once
	//Can be turned off if a token is being rechecked
	if (checkDependents)
		for (auto& x : token.dependencies)
		{
			//Don't call this if it's in the parents to prevent endless loops
			if (parents.count(x) == 0)
				calculateFollowSetPart2(parents, x, true);
		}

	//Add all the follow sets from the tokens that this token appears at the end of a rule for
	for (auto& aFollowToken : token.dependFollowSet)
	{
		for (auto& aToken : table.getToken(aFollowToken).followSet)
		{
			token.addFollowSet(aToken);
		}
	}

	//If it changed the follow set, need to recheck anything dependent on it that isn't going
	//to be checked after this is run
	//This is because two tokens may be dependent on each other
	//and we don't know which will be figured out first
	if (followSetSize != token.followSet.size())
	{
		table.updateToken(token);
		for (auto& x : token.precendFollowSet)
		{
			//Don't call this if it's in the parents to prevent endless loops
			if (parents.count(name) == 0)
				calculateFollowSetPart2(parents, x, false);
		}
	}
}

//Now that we have the complete follow set, we need to add all the following tokens to the map
//that tracks which rules are associated with which token pairs
//in the case that the token could be lambda
void GrammarParser::addFollowToRuleMap()
{
	list<string> nonTerminals = table.getNonTerminals();
	list<string> lambdaRule;
	lambdaRule.push_back("lambda");
	for (string token : nonTerminals)
	{
		Token currToken = table.getToken(token);
		if (currToken.hasLambda)
		{
			for (string followToken : currToken.followSet)
				currToken.addRuleFirstSet(followToken, lambdaRule);
			table.updateToken(currToken);
		}
	}
}

bool GrammarParser::createLL1Table(string &error)
{
	list<string> terminals = table.getTerminals();
	list<string> nonTerminals = table.getNonTerminals();
	string html, fileName = "LL1.html";
	ofstream outFile;
	outFile.open(fileName);
	if (!outFile.is_open())
	{
		error = "Cannot open output file: " + fileName;
		return false;
	}

	//Header HTML
	html = "<!DOCTYPE html><html><head>\n<style> td { white-space: nowrap; } </style>";
	html += "\n<title>LL(1) Table</title></head><body><table border='1'>";
	
	//Row for terminal tokens
	html += "<tr>\n\t<td></td>";
	for (string token : terminals)
	{
		html += "\n\t<td>" + token + "</td>";
	}
	html += "</tr>\n";

	//Rows for non-terminals
	for (string name : nonTerminals)
	{
		Token token = table.getToken(name);
		//Put the name of the non terminal in the left column
		html += "\n<tr>\n\t<td>" + name + "</td>";
		for (string terminal : terminals)
		{
			//Create cell for every non terminal
			html += "\n\t<td>";
			//Make sure there are some rules for this token pair (if not the cell is just blank)
			if (token.ruleFirstSet.count(terminal) != 0)
			{
				bool firstRule = true;	//To insert breaks between rules
				for (list<string> rule : token.ruleFirstSet.at(terminal)) 
				{
					//Put a break before this rule if it's not the first
					if (!firstRule)
					{
						html += "<br>";
					}

					//Add every token in the rule to the line
					for (string aToken : rule)
					{
						html += aToken + " ";
					}
					//So we know to put a line break next loop
					firstRule = false;
				}
			}
			//Close the cell for the current token pair
			html += "</td>";
		}
		//Close the row for the nonTerminal row
		html += "</tr>";
	}

	//Close up all the tags and output to the file
	html += "</table></body></html>";
	outFile << html;
	outFile.close();
	return true;
}