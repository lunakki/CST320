#include "parser.h"

//Clears out variables before running a new parse
void Parser::resetVariables() 
{
	currentToken = "";
	currentState = TS_NoToken;
	currentChar = ' ';
	while (!tokenStack.empty())
		tokenStack.pop_front();
}

bool Parser::parseString (string toParse, deque<Token> &outTokenStack, string &error) 
{
	bool continueParsing = true;
	int length = toParse.length();
	error = "";
	resetVariables();

	//Process tokens
	for (int i = 0; i < length && continueParsing; ++i)
	{
		currentChar = toParse[i];
		continueParsing = parse();
	}

	//If there's still a token being processed, finish it
	if (currentState != TS_NoToken) {
		currentChar = ' ';
		continueParsing = parse();
	}

	if (!continueParsing)
	{
		error = "\n\nERROR: Bad input. ";
		if (currentState == TS_NoToken)
		{
			error = "\nInvalid character: " + currentChar;
		}
		else
		{
			error = "Invalid token: " + currentToken + currentChar;
		}
		return false;
	}
	outTokenStack = tokenStack;
	return true;

	error = "Stub";
	return false;
}

bool Parser::parse (string fileName, deque<Token> &outTokenStack, string &error) {

	fstream file;
	bool continueParsing = true;
	error = "";
	resetVariables();

	file.open(fileName);
	if (!file.is_open())
	{
		error = "File failed to open for parsing.";
		return false;
	} 
	while(!file.eof() && continueParsing) 
	{
		file.get(currentChar);
		if (file.eof())
		{
			//White space will signify end of token but otherwise be ignored
			currentChar = ' ';
		}
		
		continueParsing = parse();
	}

	file.close();
	file.clear();
	//Check for errors
	if (!continueParsing)
	{
		error = "\n\nERROR: Bad input. ";
		if (currentState == TS_NoToken)
		{
			error = "\nInvalid character: " + currentChar;
		}
		else
		{
			error = "Invalid token: " + currentToken + currentChar;
		}
		return false;
	}
	outTokenStack = tokenStack;
	return true;
}

bool Parser::parse()
{
	bool continueParsing = false;

	switch(currentState)
	{
	case (TS_NoToken):
		continueParsing = AddCharNoToken();
		break;
	case (TS_Identifier):
		continueParsing = AddCharIdentifier();
		break;
	case (TS_Integer):
		continueParsing = AddCharInteger();
		break;
	case (TS_CompleteFloat):
		continueParsing = AddCharCompleteFloat();
		break;
	case (TS_CompleteString):
		continueParsing = AddCharCompleteString();
		break;
	case (TS_LessThan):
		continueParsing = AddCharLessThan();
		break;
	case (TS_GreaterThan):
		continueParsing = AddCharGreaterThan();
		break;
	case (TS_Equal):
		continueParsing = AddCharEqual();
		break;
	case (TS_CompleteOperator):
		continueParsing = AddCharCompleteOperator();
		break;
	case (TS_Directive):
		continueParsing = AddCharDirective();
		break;
	case (TS_IncompleteFloat):
		continueParsing = AddCharIncompleteFloat();
		break;
	case (TS_IncompleteString):
		continueParsing = AddCharIncompleteString();
		break;
	case (TS_IncompleteAnd):
		continueParsing = AddCharIncompleteAnd();
		break;
	case (TS_IncompleteOr):
		continueParsing = AddCharIncompleteOr();
		break;
	case (TS_Not):
		continueParsing = AddCharNot();
		break;
	case (TS_Division):
		continueParsing = AddCharDivision();
		break;
	case (TS_Comment):
		continueParsing = AddCharComment();
		break;
	default:
		break;
	}

	return continueParsing;
}

bool Parser::AddCharNoToken()
{
	switch(currentChar)
	{
	//Start of preprocessor directive
	case ('#'):
		currentState = TS_Directive;
		break;
	//Symbol; always one character so just throw onto the stack
	case ('('):
	case (')'):
	case (','):
	case ('{'):
	case ('}'):
	case (';'):
		tokenStack.push_back(Token(currentChar, TT_Symbol));
		break;
	//whitespace, just ignore
	//this is only here so it stays in a good state
	case (' '):
	case ('\t'):
	case ('\n'):
		break;
	//Operators that can't be added to
	case ('%'):
	case ('*'):
	case ('-'):
	case ('+'):
		currentState = TS_CompleteOperator;
		break;
	//Operators that could be interpreted another way with another character
	case ('<'):
		currentState = TS_LessThan;
		break;
	case ('>'):
		currentState = TS_GreaterThan;
		break;
	case ('='):
		currentState = TS_Equal;
		break;
	case ('!'):
		currentState = TS_Not;
		break;
	case ('&'):
		currentState = TS_IncompleteAnd;
		break;
	case ('|'):
		currentState = TS_IncompleteOr;
		break;
	case ('/'):
		currentState = TS_Division;
		break;
	//Start of a string
	case ('"'):
		currentState = TS_IncompleteString;
		break;
	//Start of a float (no digit in front)
	case ('.'):
		currentState = TS_IncompleteFloat;
		break;
	default:
		//Start of an integer (could turn into a float but deal with that later)
		if (currentChar >= '0' && currentChar <= '9')
		{
			currentState = TS_Integer;
		//Start of an identifier
		} else if (currentChar == '_' || isalpha(currentChar))
		{
			currentState = TS_Identifier;
		//Invalid data
		} else 
		{
			return false;
		}
	}
	//This isn't necessary for all states but it never hurts
	currentToken = currentChar;
	return true;
}

bool Parser::AddCharIdentifier()
{
	if (!CheckForTokenEnd(TT_Identifier))
	{
		//Add more onto the identifier
		if (currentChar == '_' || isalpha(currentChar))
		{
			currentToken += currentChar;
		//Invalid data
		}	else 	{
			return false;
		}
	}
	return true;
}

bool Parser::AddCharInteger()
{
	if (!CheckForTokenEnd(TT_Integer))
	{
		//Add more onto the integer
		if (currentChar >= '0' && currentChar <= '9')
		{
			currentToken += currentChar;
		//Turn it into a float
		}	else if (currentChar == '.')
		{
			currentToken += currentChar;
			currentState = TS_IncompleteFloat;
		//Invalid data
		}	else 	{
			return false;
		}
	}
	return true;
}

bool Parser::AddCharCompleteFloat()
{
	if (!CheckForTokenEnd(TT_Float))
	{
		//Add more onto the float
		if (currentChar >= '0' && currentChar <= '9')
		{
			currentToken += currentChar;
		//Invalid data
		}	else 	{
			return false;
		}
	}
	return true;
}

bool Parser::AddCharCompleteString()
{
	//Can't add anything to the end of a closed string
	return CheckForTokenEnd(TT_StringLiteral);
}

bool Parser::AddCharLessThan()
{
	if (!CheckForTokenEnd(TT_Operator))
	{
		//Turn it into an output operator
		if (currentChar == '<')
		{
			currentToken += currentChar;
			currentState = TS_CompleteOperator;
		//Invalid data
		}	else 	{
			return false;
		}
	}
	return true;
}

bool Parser::AddCharGreaterThan()
{
	if (!CheckForTokenEnd(TT_Operator))
	{
		//Turn it into an input operator
		if (currentChar == '>')
		{
			currentToken += currentChar;
			currentState = TS_CompleteOperator;
		//Invalid data
		}	else 	{
			return false;
		}
	}
	return true;
}

bool Parser::AddCharEqual()
{
	if (!CheckForTokenEnd(TT_Operator))
	{
		//Turn it into an equality operator
		if (currentChar == '=')
		{
			currentToken += currentChar;
			currentState = TS_CompleteOperator;
		//Invalid data
		}	else 	{
			return false;
		}
	}
	return true;
}

bool Parser::AddCharNot()
{
	if (!CheckForTokenEnd(TT_Operator))
	{
		//Turn it into an inequality operator
		if (currentChar == '=')
		{
			currentToken += currentChar;
			currentState = TS_CompleteOperator;
		//Invalid data
		}	else 	{
			return false;
		}
	}
	return true;
}

bool Parser::AddCharCompleteOperator()
{
	//Can't add anything to the end of a complete operator (by definition)
	return CheckForTokenEnd(TT_Operator);
}

bool Parser::AddCharDirective()
{
	//End of directive
	if (currentChar == '\n')
	{
		tokenStack.push_back(Token(currentToken, TT_Preprocessor));
		currentToken = "";
		currentState = TS_NoToken;
	//Keep adding (we don't care what it is; preprocessor will take care of that)
	} else {
		currentToken += currentChar;
	}
	return true;
}

bool Parser::AddCharIncompleteFloat()
{
	//Add more onto the float
	if (currentChar >= '0' && currentChar <= '9')
	{
		currentToken += currentChar;
		currentState = TS_CompleteFloat;
	//Invalid data
	}	else 	{
		return false;
	}

	return true;
}

bool Parser::AddCharIncompleteString()
{
	//End of string
	if (currentChar == '"')
	{
		currentState = TS_CompleteString;
	//Invalid data
	}	else if (currentChar == '\n' )
	{
		return false;
	}
	//Anything besides a new line gets added to the string
	currentToken += currentChar;
	return true;
}

bool Parser::AddCharIncompleteAnd()
{
	//Only way this can become good is if it becomes &&
	if (currentChar == '&')
	{
		currentState = TS_CompleteOperator;
	//Invalid data
	} else 
	{
		return false;
	}
	return true;
}

bool Parser::AddCharIncompleteOr()
{
	//Only way this can become good is if it becomes ||
	if (currentChar == '|')
	{
		currentState = TS_CompleteOperator;
	//Invalid data
	} else 
	{
		return false;
	}
	return true;
}

bool Parser::AddCharDivision()
{
	if (!CheckForTokenEnd(TT_Operator))
	{
		//Turn it into a comment
		if (currentChar == '/')
		{
			currentToken += currentChar;
			currentState = TS_Comment;
		//Invalid data
		}	else 	{
			return false;
		}
	}
	return true;
}

bool Parser::AddCharComment()
{
	//End of comment
	if (currentChar == '\n')
	{
		tokenStack.push_back(Token(currentToken, TT_Comment));
		currentToken = "";
		currentState = TS_NoToken;
	//Keep adding (we don't care what it is)
	} else {
		currentToken += currentChar;
	}
	return true;
}

//Deals with token separators (symbols and whitespace)
//Returns true if it handled the character and false otherwise
bool Parser::CheckForTokenEnd(TokenType type)
{
	switch(currentChar)
	{
	//End of token; add to stack and reset
	case (' '):
	case ('\t'):
	case ('\n'):
		if (IsKeyword(currentToken))
			tokenStack.push_back(Token(currentToken, TT_Keyword));
		else
			tokenStack.push_back(Token(currentToken, type));
		break;
	//End of token; add to stack and reset
	//Add symbol to stack too
	case ('('):
	case (')'):
	case (','):
	case ('{'):
	case ('}'):
	case (';'):
		if (IsKeyword(currentToken))
			tokenStack.push_back(Token(currentToken, TT_Keyword));
		else
			tokenStack.push_back(Token(currentToken, type));
		tokenStack.push_back(Token(currentChar, TT_Symbol));
		break;
	//Not a token separator
	default:
		return false;
	}
	currentToken = "";
	currentState = TS_NoToken;
	return true;
}

bool Parser::IsKeyword(string token){
	for each(string s in KEYWORDS) {
		if (s == token)
			return true;
	}
	return false;
}

