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
	void calculateFollowSet(unordered_set<string> parents, string name);
	void calculateFollowSet();
	void calculateFollowSetPart2(unordered_set<string> parents, string name, bool checkDependents);
	void addFollowToRuleMap();
	bool createLL1Table(string &error);

	RuleTable table;
};

#endif