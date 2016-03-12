#include "preprocessor.h"
using namespace std;

bool Preprocessor::preprocess(deque<Token> &inTokenStack, string &error)
{
	deque <Token> tempTokenStack, directiveTokenStack;
	string parseError;
	SymbolTable symbolTable;
	Parser parser;
	bool parseSuccess;

	//Process preprocessor directives and remove comments
	while (!inTokenStack.empty())
	{
		Token id, value, currToken;
		currToken = inTokenStack.front();
		inTokenStack.pop_front();
		if (currToken.type == TT_Preprocessor)
		{
			//Check what kind of directive it is
			if (currToken.token.substr(0,8) == "#define ")
			{
				parseSuccess = parser.parseString(currToken.token.substr(8, currToken.token.length() - 8), directiveTokenStack, parseError);
				
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
				
				//Get identifier from directive
				id = directiveTokenStack.front();
				directiveTokenStack.pop_front();
				if (id.type != TT_Identifier) {
						error = "Invalid identifier for preprocessor directive: " + currToken.token;
						return false;
				}
				//Get value from directive, if present
				if (!directiveTokenStack.empty())
				{
					value = directiveTokenStack.front();
					directiveTokenStack.pop_front();
					Type valueType;

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
					//Add or update symbol in table as appropriate
					if (symbolTable.containsSymbol(id.token))
					{
						symbolTable.updateSymbol(Symbol(id.token, valueType, U_Const, value.token));
					} else {
						symbolTable.addSymbol(Symbol(id.token, valueType, U_Const, value.token));
					}
				}
			//Could add other things like #include here but mine just does #define
			} else {
				error = "Unrecognized preprocessor directive: " + currToken.token;
				return false;
			}
		//Check if we need to swap out an identifier with a value
		} else if (currToken.type == TT_Identifier)
		{
			if (symbolTable.containsSymbol(currToken.token))
			{
				Symbol symbol = symbolTable.getSymbol(currToken.token);
				TokenType tokenType;
				//Convert symbol type to token type
				switch (symbol.type) {
				case T_Int:
					tokenType = TT_Integer;
					break;
				case T_Float:
					tokenType = TT_Float;
					break;
				case T_String:
					tokenType = TT_StringLiteral;
					break;
				default: //Shouldn't happen
					error = "Unknown token type in symbol table for symbol: " + currToken.token;
					return false;
				}

				//Push replacement token on in place of original identifier
				tempTokenStack.push_back(Token(symbol.value, tokenType));
			} else { //Put the identifier back on the deque; no match found
				tempTokenStack.push_back(currToken);
			}
		//If not a preprocessor directive, comment, or identifier in the symbol table, just push it onto the new deque
		} else if (currToken.type != TT_Comment)
		{
			tempTokenStack.push_back(currToken);
		}
	} //end while

	//Resulting tempTokenStack has preprocessor directives processed and removed,
	//any defined identifiers replaced, and all comments removed
	inTokenStack = tempTokenStack;
	return true;
}