#include <iostream>
#include "parser.h"

using namespace std;

void outputParseResults(stack<Token> tokenStack);

int main () {
	string fileNames[] = {"sampleProgram0.txt", "sampleProgram1.txt", "sampleProgram2.txt"};
	stack<Token> tokenStack;
	Parser parser;
	bool success;
	string error;

	for (int i = 0; i < 3; ++i) 
	{
		cout << "Attempting to parse file " << fileNames[i] << endl;
		success = parser.parse(fileNames[i], tokenStack, error);
		
		if (!success)
		{
			cout << "\n\nParsing failed: " << error << endl;
		} else
		{
			cout << "\n\nSuccessfully parsed file!\n";
			outputParseResults(tokenStack);	
		}
		cout << "\n\nPress enter to continue.\n";
		getchar();
	}

	success = parser.parseString("MAX 10", tokenStack, error);
	if (!success)
	{
		cout << "\n\nParsing failed: " << error << endl;
	} else
	{
		cout << "\n\nSuccessfully parsed file!\n";
		outputParseResults(tokenStack);	
	}
	cout << "\n\nPress enter to continue.\n";
	getchar();

	return 0;
}

void outputParseResults(stack<Token> tokenStack)
{
	cout << "Below are the tokens in reverse order:";
	while (!tokenStack.empty())
	{
		Token token = tokenStack.top();
		tokenStack.pop();
		cout << "\nToken value: " << setw(25) << left 
			<< token.token << setw(1) << "    Token Type: ";
			
		//I hate C++; it's so stupid that you have to do this to print enums
		switch (token.type)
		{
		case (TT_Identifier):
			cout << "Identifier";
			break;
		case (TT_Preprocessor):
			cout << "Preprocessor Directive";
			break;
		case (TT_StringLiteral):
			cout << "String Literal";
			break;
		case (TT_Integer):
			cout << "Integer";
			break;
		case (TT_Float):
			cout << "Float";
			break;
		case (TT_Operator):
			cout << "Operator";
			break;
		case (TT_Symbol):
			cout << "Symbol";
			break;
		case (TT_Comment):
			cout << "Comment";
			break;
		case (TT_Keyword):
			cout << token.token;
			break;
		}
	}
}