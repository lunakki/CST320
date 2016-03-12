#include "preprocessor.h"
using namespace std;

bool Preprocessor::preprocess(stack<Token> &inTokenStack, string &error)
{
	while (!tokenStack.empty())
		tokenStack.pop();

	while (!inTokenStack.empty())
	{
		Token currToken = inTokenStack.top();
		inTokenStack.pop();
		if (currToken.type == TT_Preprocessor)
		{
			if (currToken.token.substr(0,7) == "#define ")
			{

			}
		} else if (currToken.type != TT_Comment)
		{
			tokenStack.push(currToken);
		}


	}


	error = "stub";
	return false;
}