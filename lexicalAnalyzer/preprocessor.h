#ifndef PREPROCESSOR_H
#define PREPROCESSOR_H

#include "utils.h"
#include "symbolTable.h"
#include <stack>

class Preprocessor {
public:
	bool preprocess(stack<Token> tokenStack, string error);
private:

};

#endif