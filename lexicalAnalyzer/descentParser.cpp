#include "descentParser.h"

//Attempts to parses a deque of tokens
//Returns true if sucessful; false otherwise
bool DescentParser::parse(deque<Token> inTokens, string error)
{
	int count = 0;
	bool success;
	tokens = inTokens;
	tokens.push_back(Token("end", TT_End));
	tempTokens.clear();
	success = program(count);
	//Only successful if rule succeeded and all tokens were used
	return (success && (tokens.front().type == TT_End));
}

//Removes one token from the deque being processed
//Adds it to a temporary deque in case replacement is necessary
//Requires a count variable to ensure the removal is being tracked
void DescentParser::removeToken(int &count)
{
	tempTokens.push_back(tokens.front());
	tokens.pop_front();
	++count;
}

//Replaces the given number of tokens from the temporary deque back into the deque being parsed
void DescentParser::replaceTokens(int count)
{
	for (int i = 0; i < count; ++i)
	{
		tokens.push_front(tokens.front());
		tempTokens.pop_front();
	}
}

//Rule functions
//See grammar.docx for grammar documentation
//They are named the same there as they are here and are listed in the same order
//parentCount is for tracking how many tokens each function removes to allow replacement
bool DescentParser::program(int &parentCount)
{
	//Counts are for tracking how many tokens were removed
	//Useless here since it's the top level but it's done the same for consistency
	//and in case this becomes part of another rule in the future
	int count = 0;
	//Check if first part of rule matches
	if (funcProto(count))
	{
		//It passed; check next part
		if (function(count))
		{
			//All parts passed
			//Pass count up to parent and return true
			parentCount += count;
			return true;
		}
	}
	//Rule didn't match
	//Put checked tokens back and return false
	replaceTokens(count);
	return false;
}

bool DescentParser::funcProto(int &parentCount)
{
	//Track how many tokens this function removes
	int count = 0;
	if (type(count))
	{
		//Matched the first rule
		//Check the next portion of the rule
		if (tokens.front().type == TT_Identifier)
		{
			//Remove the token since it matched a terminal
			//This increases the count for us
			removeToken(count);
			//Check the next part of the rule
			if (tokens.front().type == TT_Symbol && tokens.front().token == "(")
			{
				removeToken(count);
				if (params(count))
				{
					if (tokens.front().type == TT_Symbol && tokens.front().token == ")")
					{
						removeToken(count);
						if (tokens.front().type == TT_Symbol && tokens.front().token == ";")
						{
							removeToken(count);							
							if (funcProto(count))
								//Give parent count of removed tokens in case it has to backtrack
								parentCount += count;
								return true;
						}
					}
				}
			}
		}
	}
	//Only rule left is lambda
	//Replace tokens removed during check of earlier rule and return true
	replaceTokens(count);
	return true;
}

bool DescentParser::type(int &parentCount)
{
	//Track how many tokens this function removes
	int count = 0;
	TokenType type = tokens.front().type;
	switch (type)
	{
	case TT_Keyword:
		//Make sure it's the correct keyword "void"
		//If not then it doesn't match any rules
		if (tokens.front().token != "void")
			break;
	case TT_Integer:
	case TT_Float:
	case TT_StringLiteral:
		//If any of these are true then a rule is matched
		removeToken(count);		
		//Add count to parent's count in case it has to backtrack
		parentCount += count;
		return true;
	}

	//Doesn't match any rules
	replaceTokens(count);
	return false;
}

bool DescentParser::params(int &parentCount)
{
	int count = 0;
	if (type(count))
	{
		if (tokens.front().type == TT_Identifier)
		{
			removeToken(count);			
			if (mTypeParams(count))
			{
				parentCount += count;
				return true;
			}
		}
	}

	//lambda rule
	replaceTokens(count);
	return true;
}

bool DescentParser::multiParams(int &parentCount)
{
	int count = 0;
	if (type(count))
	{
		if (tokens.front().type == TT_Identifier)
		{
			removeToken(count);			
			if (mTypeParams(count))
			{
				parentCount += count;
				return true;
			}
		}
	}

	//No lambda rule
	replaceTokens(count);
	return false;
}

bool DescentParser::mTypeParams(int &parentCount)
{
	int count = 0;
	if (tokens.front().type == TT_Symbol && tokens.front().token == ",")
	{
		removeToken(count);		
		if (multiParams(count))
		{
			parentCount += count;
			return true;
		}
	}

	replaceTokens(count);
	return true;
}

bool DescentParser::function(int &parentCount)
{
	int count = 0;

	if (type(count))
	{
		if (tokens.front().type == TT_Identifier)
		{
			removeToken(count);			
			if (tokens.front().type == TT_Symbol && tokens.front().token == "(")
			{
				removeToken(count);				
				if (args(count))
				{
					if (tokens.front().type == TT_Symbol && tokens.front().token == ")")
					{
						removeToken(count);						
						if (tokens.front().type == TT_Symbol && tokens.front().token == "{")
						{
							removeToken(count);							
							if (statList(count))
							{
								if (tokens.front().type == TT_Keyword && tokens.front().token == "return")
								{
									removeToken(count);									
									if (optExpr(count))
									{
										if (tokens.front().type == TT_Symbol && tokens.front().token == "}")
										{
											removeToken(count);
											parentCount += count;
											return true;
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}

	replaceTokens(count);
	return false;
}

bool DescentParser::statList(int &parentCount)
{
	int count = 0;

	if (stat(count))
	{
		if (tokens.front().type == TT_Symbol && tokens.front().token == ";")
		{
			removeToken(count);
			if (statList(count))
			{
				parentCount += count;
				return true;
			}
		}
	}

	replaceTokens(count);
	return true;
}

bool DescentParser::stat(int &parentCount)
{
	int count = 0;
	if (tokens.front().type == TT_Keyword && tokens.front().token == "if")
	{
		removeToken(count);
		if (tokens.front().type == TT_Symbol && tokens.front().token == "(")
		{
			removeToken(count);
			if (expression(count))
			{
				if (tokens.front().type == TT_Symbol && tokens.front().token == ")")
				{
					removeToken(count);
					if (tokens.front().type == TT_Symbol && tokens.front().token == "{")
					{
						removeToken(count);
						if (statList(count))
						{
							if (tokens.front().type == TT_Symbol && tokens.front().token == "}")
							{
								removeToken(count);
								if (rElse(count))
								{
									parentCount += count;
									return true;
								}
							}
						}
					}
				}
			}
		}
	}

	//First rule failed; put back tokens and try the next
	replaceTokens(count);
	count = 0;
	if (tokens.front().type == TT_Keyword && tokens.front().token == "while")
	{
		removeToken(count);
		if (tokens.front().type == TT_Symbol && tokens.front().token == "(")
		{
			removeToken(count);
			if (expression(count))
			{
				if (tokens.front().type == TT_Symbol && tokens.front().token == ")")
				{
					removeToken(count);
					if (tokens.front().type == TT_Symbol && tokens.front().token == "{")
					{
						removeToken(count);
						if (statList(count))
						{
							if (tokens.front().type == TT_Symbol && tokens.front().token == "}")
							{
								removeToken(count);
								parentCount += count;
								return true;
							}
						}
					}
				}
			}
		}
	}

	//Second rule failed; try the next
	replaceTokens(count);
	count = 0;
	if (tokens.front().type == TT_Identifier)
	{
		removeToken(count);
		if (mIdStat(count))
		{
			parentCount += count;
			return true;
		}
	}

	//Third rule failed
	replaceTokens(count);
	count = 0;
	if (tokens.front().type == TT_Keyword && tokens.front().token == "cout")
	{
		removeToken(count);
		if (tokens.front().type == TT_Operator && tokens.front().token == "<<")
		{
			removeToken(count);
			if (expression(count))
			{
				parentCount += count;
				return true;
			}
		}
	}

	//Fourth rule failed
	replaceTokens(count);
	count = 0;
	if (tokens.front().type == TT_Keyword && tokens.front().token == "cin")
	{
		removeToken(count);
		if (tokens.front().type == TT_Operator && tokens.front().token == ">>")
		{
			removeToken(count);
			if (tokens.front().type == TT_Identifier)
			{
				removeToken(count);
				parentCount += count;
				return true;
			}
		}
	}

	//Fifth rule failed
	replaceTokens(count);
	count = 0;
	if (varDecl(count))
	{
		parentCount += count;
		return true;
	}

	//Sixth rule failed
	replaceTokens(count);
	count = 0;
	if (tokens.front().type == TT_Symbol && tokens.front().token == "(")
	{
		removeToken(count);
		if (expression(count))
		{
			if (tokens.front().type == TT_Symbol && tokens.front().token == ")")
			{
				removeToken(count);
				if (exprRecurs(count))
				{
					parentCount += count;
					return true;
				}
			}
		}
	}

	//Seventh rule failed
	replaceTokens(count);
	count = 0;
	//Going to combine the last three rules into one check
	if (tokens.front().type == TT_Integer || tokens.front().type == TT_Float || tokens.front().type == TT_StringLiteral)
	{
		removeToken(count);
		if (exprRecurs(count))
		{
			parentCount += count;
			return true;
		}
	}

	//No rules matched
	replaceTokens(count);
	return false;
}

bool DescentParser::mIdStat(int &parentCount)
{
	int count = 0;

	if(tokens.front().type == TT_Operator && tokens.front().token == "=")
	{
		removeToken(count);
		if (expression(count))
		{
			parentCount += count;
			return true;
		}
	}

	//Next rule
	replaceTokens(count);
	count = 0;

	if (tokens.front().type == TT_Operator && tokens.front().token != "=")
	{
		removeToken(count);
		if (expression(count))
		{
			if (exprRecurs(count))
			{
				parentCount += count;
				return true;
			}
		}
	}

	//Next rule
	replaceTokens(count);
	count = 0;

	if (mIdProcCall(count))
	{
		if (exprRecurs(count))
		{
			parentCount += count;
			return true;
		}
	}

	//lambda
	replaceTokens(count);
	return true;
}

bool DescentParser::expression(int &parentCount)
{
	int count = 0;
	if (tokens.front().type == TT_Identifier)
	{
		removeToken(count);
		if (mExpression(count))
		{
			parentCount += count;
			return true;
		}
	}

	//Next rule
	replaceTokens(count);
	count = 0;
	if (tokens.front().type == TT_Symbol && tokens.front().token == "(")
	{
		removeToken(count);
		if (expression(count))
		{
			if (tokens.front().type == TT_Symbol && tokens.front().token == ")")
			{
				removeToken(count);
				if (exprRecurs(count))
				{
					parentCount += count;
					return true;
				}
			}
		}
	}

	//Next rule; going to combine three into one check
	replaceTokens(count);
	count = 0;
	if (tokens.front().type == TT_Integer || tokens.front().type == TT_Float || tokens.front().type == TT_StringLiteral)
	{
		removeToken(count);
		if (exprRecurs(count))
		{
			parentCount += count;
			return true;
		}
	}

	//no match
	replaceTokens(count);
	return false;
}

bool DescentParser::mExpression(int &parentCount)
{
	int count = 0;
	if (tokens.front().type == TT_Operator && tokens.front().token != "=")
	{
		removeToken(count);
		if (expression(count))
		{
			if (exprRecurs(count))
			{
				parentCount += count;
				return true;
			}
		}
	}

	//next rule
	replaceTokens(count);
	count = 0;

	if (tokens.front().type == TT_Symbol && tokens.front().token == "(")
	{
		removeToken(count);
		if (mOptArgs(count))
		{
			parentCount += count;
			return true;
		}
	}

	//lambda
	replaceTokens(count);
	return true;
}

bool DescentParser::mOptArgs(int &parentCount)
{
	int count = 0;

	if (tokens.front().type == TT_Symbol && tokens.front().token == ")")
	{
		removeToken(count);
		if (exprRecurs(count))
		{
			parentCount += count;
			return true;
		}
	}

	//next rule
	replaceTokens(count);
	count = 0;
	if (args(count))
	{
		if (tokens.front().type == TT_Symbol && tokens.front().token == ")")
		{
			removeToken(count);
			if (exprRecurs(count))
			{
				parentCount += count;
				return true;
			}
		}
	}

	//no match
	replaceTokens(count);
	return false;
}

bool DescentParser::exprRecurs(int &parentCount)
{
	int count = 0;
	if (tokens.front().type == TT_Operator && tokens.front().token != "=")
	{
		removeToken(count);
		if (expression(count))
		{
			if (exprRecurs(count))
			{
				parentCount += count;
				return true;
			}
		}
	}


	//lambda
	replaceTokens(count);
	return true;
}

bool DescentParser::rElse(int &parentCount)
{
	int count = 0;
	if (tokens.front().type == TT_Keyword && tokens.front().token == "else")
	{
		removeToken(count);
		if (moreElse(count))
		{
			parentCount += count;
			return true;
		}
	}

	//lambda
	replaceTokens(count);
	return true;
}

bool DescentParser::moreElse(int &parentCount)
{
	int count = 0;
	if (tokens.front().type == TT_Keyword && tokens.front().token == "if")
	{
		removeToken(count);
		if (tokens.front().type == TT_Symbol && tokens.front().token == "(")
		{
			removeToken(count);
			if (expression(count))
			{
				if (tokens.front().type == TT_Symbol && tokens.front().token == ")")
				{
					removeToken(count);
					if (tokens.front().type == TT_Symbol && tokens.front().token == "{")
					{
						removeToken(count);
						if (statList(count))
						{
							if (tokens.front().type == TT_Symbol && tokens.front().token == "}")
							{
								removeToken(count);
								if (rElse(count))
								{
									parentCount += count;
									return true;
								}
							}
						}
					}
				}
			}
		}
	}

	//First rule failed; put back tokens and try the next
	replaceTokens(count);
	count = 0;

	if (tokens.front().type == TT_Symbol && tokens.front().token == "{")
	{
		removeToken(count);
		if (statList(count))
		{
			if (tokens.front().type == TT_Symbol && tokens.front().token == "}")
			{	
				removeToken(count);
				parentCount += count;
				return true;
			}	
		}
	}

	//No match
	replaceTokens(count);
	return false;
}

bool DescentParser::mIdProcCall(int &parentCount)
{
	int count = 0;

	if (tokens.front().type == TT_Symbol && tokens.front().token == "(")
	{
		removeToken(count);
		if (mOptArgs(count))
		{
			parentCount += count;
			return true;
		}
	}

	//no match
	replaceTokens(count);
	return false;
}

bool DescentParser::args(int &parentCount)
{
	int count = 0;
	if (expression(count))
	{
		if (mExpArgs(count))
		{
			parentCount += count;
			return true;
		}
	}

	//no match
	replaceTokens(count);
	return false;
}

bool DescentParser::mExpArgs(int &parentCount)
{
	int count = 0;
	if (tokens.front().type == TT_Symbol && tokens.front().token == ",") 
	{
		removeToken(count);
		if (args(count))
		{
			parentCount += count;
			return true;
		}
	}

	//lambda
	replaceTokens(count);
	return true;
}

bool DescentParser::varDecl(int &parentCount)
{
	int count = 0;

	if (type(count))
	{
		if (tokens.front().type == TT_Identifier)
		{
			removeToken(count);
			if (mTypeVarDecl(count))
			{
				parentCount += count;
				return true;
			}
		}
	}
	
	//no match
	replaceTokens(count);
	return false;
}

bool DescentParser::mTypeVarDecl(int &parentCount)
{
	int count = 0;
	if (tokens.front().type == TT_Operator && tokens.front().token == "=")
	{
		removeToken(count);
		if (expression(count))
		{
			parentCount += count;
			return true;
		}
	}

	//lambda
	replaceTokens(count);
	return true;
}

bool DescentParser::optExpr(int &parentCount)
{
	int count = 0;
	if (expression(count))
	{
		parentCount += count;
		return true;
	}

	//lambda
	replaceTokens(count);
	return true;
}
