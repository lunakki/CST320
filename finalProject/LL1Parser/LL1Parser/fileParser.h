#ifndef FILEPARSER_H
#define FILEPARSER_H

#include <fstream>
#include "ruleTable.h"

class FileParser {
public:
	bool parse(string fileName, string error, RuleTable &table);
private:
};

#endif