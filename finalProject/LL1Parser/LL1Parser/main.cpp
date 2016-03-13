#include "parser.h"
#include <iostream>

using namespace std;

int main() {
	GrammarParser parser;
	RuleTable table;
	string error;
	bool success;
	success = parser.parse("grammar.txt", error, table);

	if (success)
	{
		cout << "Grammar file parsed. Tokens: \n" << table.toString();
	}
	else
	{
		cout << "Error parsing grammar file: " << error;
	}

	cout << "\n\nPress enter to continue.\n";
	getchar();
}