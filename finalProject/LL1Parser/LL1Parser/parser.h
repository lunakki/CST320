#ifndef PARSER_H
#define PARSER_H

#include <fstream>
#include "ruleTable.h"

class GrammarParser {
public:
	bool parse(string fileName, string &error, RuleTable &outTable);
private:
	bool parseFile(string fileName, string &error);
	void calculateLambdaRulesR(unordered_set<string> parents, string name, bool checkDependents);
	void calculateFirstSet(unordered_set<string> parents, string name, bool checkDependents);

	RuleTable table;
};

#endif