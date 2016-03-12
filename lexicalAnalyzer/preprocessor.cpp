#include "preprocessor.h"
using namespace std;

bool Preprocessor::preprocess(stack<Token> &inTokenStack, string &error)
{
	stack <Token> tempTokenStack, directiveTokenStack;
	string parseError;
	SymbolTable symbolTable;
	Parser parser;
	bool parseSuccess;

	//Process preprocessor directives and remove comments
	while (!inTokenStack.empty())
	{
		Token currToken = inTokenStack.top();
		inTokenStack.pop();
		if (currToken.type == TT_Preprocessor)
		{
			//Check what kind of directive it is
			if (currToken.token.substr(0,7) == "#define ")
			{
				parseSuccess = parser.parseString(currToken.token.substr(8, currToken.token.length()), directiveTokenStack, parseError);
				
				//Check that it was able to parse and has correct arguments
				if (!parseSuccess)
				{
					error = "Could not parse preprocessor directive: " + currToken.token;
					return false;
				}
				if (directiveTokenStack.size() < 1) {
					error = "No identifier for preprocessor directive: " + currToken.token;
					return false;
				}
				if (directiveTokenStack.size() > 2) {
					error = "Too many arguments for preprocessor directive (max is 2): " + currToken.token;
					return false;
				}
				
				//Get ID and value
				if (directiveTokenStack.size() == 1) {	
					if (directiveTokenStack.top().type != TT_Identifier) {
						error = "Invalid identifier for preprocessor directive: " + currToken.token;
						return false;
					}
				} else if (directiveTokenStack.size() == 2) {
					Token value = directiveTokenStack.top();
					Type valueType;
					directiveTokenStack.pop();
					Token id = directiveTokenStack.top();
					//Verify correct arguments are given
					if (id.type != TT_Identifier) {
						error = "Invalid identifier for preprocessor directive: " + currToken.token;
						return false;
					}
					if (value.type == TT_Integer)
					{
						valueType = T_Int;
					} else if (value.type == TT_Float) {
						valueType = T_Float;
					} else if (value.type == TT_StringLiteral) {
						valueType = T_String;
					} else {
						error = "Invalid value for proprocessor directive: " + currToken.token;
						return false;
					}
					//This will only add if it doesn't already exist
					//Since we're going in reverse order, it'll add the most recent declaration
					//All preprocessor directives are required to be before other statements in my language
					symbolTable.addSymbol(Symbol(id.token, valueType, U_Const, value.token));
				}
			//Could add other things like #include here in the future
			} else {
				error = "Unrecognized preprocessor directive: " + currToken.token;
				return false;
			}
		//If not a preprocessor directive or comment, just push it onto a new stack
		//This gives us a new stack without the comments or preprocessor directives
		} else if (currToken.type != TT_Comment)
		{
			tempTokenStack.push(currToken);
		}
	} //end while

	error = "stub";
	return false;
}