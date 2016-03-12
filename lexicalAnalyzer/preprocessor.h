#ifndef PREPROCESSOR_H
#define PREPROCESSOR_H

#include "utils.h"
#include "symbolTable.h"
#include "parser.h"
#include <deque>

class Preprocessor {
public:
	bool preprocess(deque<Token> &inTokenStack, string &error);
private:
	
};

#endif