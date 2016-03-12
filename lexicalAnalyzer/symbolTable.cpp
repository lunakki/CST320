#include "symbolTable.h"

using namespace std;

//Add a symbol to the table
//Will not add the symbol if the name is already in the table
void SymbolTable::addSymbol(Symbol symbol){
	symbolTable.insert(make_pair(symbol.name, symbol));
}


//Get the symbol from the table that matches a name
//Will throw an exception if not found
Symbol SymbolTable::getSymbol(string name){
	return symbolTable.at(name);
}

//Deletes a symbol with a given name
//Returns true if successfull and false if not
bool SymbolTable::deleteSymbol(string name){
	return symbolTable.erase(name) == 1;
}

//Updates a symbol based on the name of the new symbol
//Returns false if item was not already in the table to begin with
bool SymbolTable::updateSymbol(Symbol newSymbol){
	try {
		symbolTable.at(newSymbol.name) = newSymbol;
		return true;
	} catch (exception e) {
		return false;
	}
}

//Converts the symbolTable to a string for easy printing
string SymbolTable::toString() {
	string output = "";

	if (symbolTable.empty())
		return "Empty";

	for (auto& x: symbolTable)
		output += x.second.toString();

	return output;
}

bool SymbolTable::containsSymbol(string name) {
	try {
		symbolTable.at(name);
		return true;
	} catch (exception e) {
		return false;
	}
}
