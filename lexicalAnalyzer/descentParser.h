#ifndef DESCENT_PARSER_H
#define DESCENT_PARSER_H

#include <deque>
#include <string>
#include "utils.h"

class DescentParser {
public:
	bool parse(deque<Token> inTokens, string error);

private:
	deque<Token> tokens;
	deque<Token> tempTokens;
};
#endif