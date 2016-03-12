#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H

#include <string>
#include <unordered_map>

using namespace std;

//Enum for the type; safer than a string
enum Type {
	T_Int, T_Float, T_String
};

//Enum for the use; safer than a string
enum Use {
	U_Var, U_Const, U_Func
};

//Symbols to be held in the the symbol table
struct Symbol {
	//Constructors
	Symbol(string name, Type type, Use use, string value) {
		this->name = name;
		this->type = type;
		this->use = use;
		this->value = value;
	}
	Symbol() {
		
	}
	//For easy printing
	string toString(){
		string typeStr, useStr;
		switch(use) {
		case(U_Var):
			useStr = "variable";
			break;
		case(U_Const):
			useStr = "constant";
			break;
		case(U_Func):
			useStr = "function";
			break;
		default:
			useStr = "unknown";
			break;
		}

		switch(type) {
		case(T_Int):
			typeStr = "int";
			break;
		case(T_Float):
			typeStr = "float";
			break;
		case(T_String):
			typeStr = "string";
			break;
		default:
			typeStr = "unknown";
			break;
		}

	return "name: " + name + "\ntype: " + typeStr + "\nUse: " + useStr + "\nvalue: " + value + "\n";
	}
	string name;
	Type type;
	Use use;
	string value;
};

class SymbolTable{
public:
	void addSymbol(Symbol symbol);
	Symbol getSymbol(string name);
	bool deleteSymbol(string name);
	bool updateSymbol(Symbol newSymbol);
	string toString();
	bool containsSymbol(string name);

private:
	unordered_map<std::string,Symbol> symbolTable;	//Map for holding symbols

};

#endif