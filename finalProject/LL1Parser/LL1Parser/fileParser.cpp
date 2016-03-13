#include "fileParser.h"

bool FileParser::parse(string fileName, string error, RuleTable &outTable)
{
	fstream file;
	string tokenName, word;
	list<string> rule;
	RuleTable table;
	file.open(fileName);

	if (!file.is_open())
	{
		error = "Unable to open file";
		return false;
	}

	file >> tokenName;
	if (!file.eof())
	{
		while (file.peek() != '\n' && file.peek() != EOF)
		{
			file >> word;
			table.addToken(word);
			rule.push_back(word);
		}
		if (!rule.empty())
		{
			table.addToken(Token(tokenName, false, false, true, rule));
		} else	{
			error = "Token listed without a rule: " + tokenName;
			return false;
		}
	} else {
		error = "Empty file: " + fileName;
		return false;
	}
	while(file.peek() != EOF)
	{
		rule.clear();
		file >> tokenName;
		while (file.peek() != '\n' && file.peek() != EOF)
		{
			file >> word;
			table.addToken(word);
			rule.push_back(word);
		}
		if (!rule.empty())
		{
			table.addRule(tokenName, rule);
		} else	{
			error = "Token listed without a rule: " + tokenName;
			return false;
		}
	}

	outTable = table;
	return true;
}