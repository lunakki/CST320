#ifndef PREPROCESSOR_H
#define PREPROCESSOR_H

#include "utils.h"
#include "symbolTable.h"
#include "parser.h"
#include <stack>

class Preprocessor {
public:
	bool preprocess(queue<Token> &inTokenStack, string &error);
private:
	
};

#endif