#include<iostream>
#include<fstream>
#include<stdlib.h>
int length = 0;
std::string keywords[21] = {"char","break","const","continue","double","default","else","float","int","str",
						"if","long","case","func","for","until","write","read","readLine","return","void"};

//Checks if Character is a digit
bool IsDigit(char c)
{
	if(c>='0' && c<='9') return true;
	else return false;
}

//Checks if Character is an Alphabet
bool IsAlpha(char c)
{
	if(c>='a' && c<='z' || c>='A' && c<='Z') return true;
	else return false;
}

//Checks if Lexeme is a Constant
int IsConstant(std::string char_stream,int forward)
{
	if(char_stream[forward]>='0' && char_stream[forward]<='9')
	{
		return IsConstant(char_stream,forward+1);
	}
	else if(char_stream[forward]=='.' && IsDigit(char_stream[forward-1]) && IsDigit(char_stream[forward+1]))
	{
		return IsConstant(char_stream,forward+1);
	}
	else if(char_stream[forward]=='.' && IsDigit(char_stream[forward+1]))
	{
		return IsConstant(char_stream,forward+1);
	}
	else if(IsDigit(char_stream[forward])==false) return forward;
}

//Checks if Lexeme is a String Literal
int STRING(std::string char_stream,int forward)
{
	if(char_stream[forward] != '"')
	{
		return STRING(char_stream,forward+1);
	}
	else return forward+1;
}

//Checks if Lexeme is a Separator
int IsSeparator(std::string char_stream,int begin)
{
	if(char_stream[begin]==';' || char_stream[begin]=='{' || char_stream[begin]=='}' || char_stream[begin]=='(' || char_stream[begin]==')' || char_stream[begin]=='[' || char_stream[begin]==']' || char_stream[begin]==',' || char_stream[begin]==':')
	{
		return begin+1;
	}
	return -1;
}

//Checks if Lexeme is a Preprocessor Directive (Importing File)
int IsDirective(std::string char_stream,int forward)
{
	if(char_stream[forward]=='u' && char_stream[forward+1]=='s' && char_stream[forward+2]=='e' && char_stream[forward+3]==' ')
	{
		return forward+4;
	}
	return -1;
}

//Checks if given word matches a keyword 
bool Match_Keyword(std::string word)
{
	for(int i=0;i<21;i++)
	{
		if(word==keywords[i]) return true;
	}
	return false;
}

//Checks if Lexeme is an Identifier
int IsIdentifier(std::string char_stream,int forward)
{
	std::string word = "";
	if(char_stream[forward]=='_' || IsAlpha(char_stream[forward]))
	{
		while(char_stream[forward]=='_' || IsAlpha(char_stream[forward]) || IsDigit(char_stream[forward]))
		{
			word+=char_stream[forward];
			forward++;
		}
		if(Match_Keyword(word)==false) return forward;
	}
	else return -1;
}

//Checks if Lexeme is a Character Literal
int CHAR(std::string char_stream,int forward)
{
	if(char_stream[forward]=='\\')
	{
		if(char_stream[forward+1]=='n' || char_stream[forward+1]=='r' || char_stream[forward+1]=='t' || char_stream[forward+1]=='b' || char_stream[forward+1]=='f' || char_stream[forward+1]=='v' || char_stream[forward+1]=='0' || char_stream[forward+1]=='\'' || char_stream[forward+1]=='\"' || char_stream[forward+1]=='\\')
		{
			if(char_stream[forward+2]=='\'')
			{
				return forward+3;
			}
		}
	}
	else if(char_stream[forward] != '\\' && char_stream[forward+1]=='\'')
	{
		return forward+2;
	}
	return -1;
}

//Checks if given Lexeme is a Keyword
int IsKeyWord(std::string char_stream,int forward)
{
	//Take out word and independently analyse it
	std::string word = "";
	while(IsAlpha(char_stream[forward]))
	{
		word+=char_stream[forward];
		forward++;
	}
	//Checks if extracted word is Keyword
	if(Match_Keyword(word) && char_stream[forward] != '_') 
	{
		length = word.length();
		return forward;
	}
	else return -1;
}

//Checks if Lexeme is Boolean Operator (and | or)
int Is_Boolean(std::string char_stream,int forward)
{
	if(char_stream[forward]=='a' && char_stream[forward+1]=='n' && char_stream[forward+2]=='d' && char_stream[forward+3]==' ')
	{
		return forward+4;
	}
	else if(char_stream[forward]=='o' && char_stream[forward+1]=='r' && char_stream[forward+2]==' ')
	{
		return forward+3;
	}
	return -1;
}

//Checks if Lexeme is an Assignment Operator (=)
int Is_Assignment(std::string char_stream,int forward)
{
	if(char_stream[forward]=='=' && char_stream[forward+1] != '=')
	{
		return forward+1;
	}	
	return -1;
}

//Checks if Lexeme is an Increment Operator (++ | +=)
int Is_Increment(std::string char_stream,int forward)
{
	if(char_stream[forward]=='+' && char_stream[forward+1]=='+')
	{
		return forward+2;
	}
	else if(char_stream[forward]=='+' && char_stream[forward+1]=='=')
	{
		return forward+2;
	}
	return -1;
}

// Checks if Lexeme is a Decrement Operator (-- | -=)
int Is_Decrement(std::string char_stream,int forward)
{
	if(char_stream[forward]=='-' && char_stream[forward+1]=='-')
	{
		return forward+2;
	}
	else if(char_stream[forward]=='-' && char_stream[forward+1]=='=')
	{
		return forward+2;
	}
	return -1;
}

//Checks if Lexeme is a Comparison Operator (== | != | >= | <= | > | < )
int Is_Comparison(std::string char_stream,int forward)
{
	if(char_stream[forward]=='=' && char_stream[forward+1]=='=')
	{
		return forward+2;
	}
	else if(char_stream[forward]=='!' && char_stream[forward+1]=='=')
	{
		return forward+2;
	}
	else if(char_stream[forward]=='>' && char_stream[forward+1]=='=')
	{
		return forward+2;
	}
	else if(char_stream[forward]=='<' && char_stream[forward+1]=='=')
	{
		return forward+2;
	}
	else if(char_stream[forward]=='>' && char_stream[forward+1] != '=')
	{
		return forward+1;
	}
	else if(char_stream[forward]=='<' && char_stream[forward+1] != '=')
	{
		return forward+1;
	}
	return -1;
}
//Checks if Lexeme is Arithmetic Operator
int Is_Arithmetic(std::string char_stream,int forward)
{
	if(char_stream[forward]=='+' && (char_stream[forward+1] != '=' && char_stream[forward+1] != '+'))
	{
		return forward+1;
	}
	else if(char_stream[forward]=='-' && char_stream[forward+1] != '-' && char_stream[forward+1] != '=')
	{
		return forward+1;
	}
	else if(char_stream[forward]=='*' && char_stream[forward+1]=='=')
	{
		return forward+2;
	}
	else if(char_stream[forward]=='*' && char_stream[forward+1] != '=')
	{
		return forward+1;
	}
	else if(char_stream[forward]=='%' && char_stream[forward+1]=='=')
	{
		return forward+2;
	}
	else if(char_stream[forward]=='%' && char_stream[forward+1] != '=')
	{
		return forward+1;
	}
	else if(char_stream[forward]=='^' && char_stream[forward+1]=='=')
	{
		return forward+2;
	}
	else if(char_stream[forward]=='^' && char_stream[forward+1] != '=')
	{
		return forward+1;
	}
	else if(char_stream[forward]=='/' && char_stream[forward+1]=='=')
	{
		return forward+2;
	}
	else if(char_stream[forward]=='/' && char_stream[forward+1] != '=')
	{
		return forward+1;
	}
	else if(char_stream[forward]=='&' && char_stream[forward+1]=='=')
	{
		return forward+2;
	}
	else if(char_stream[forward]=='&' && char_stream[forward+1] != '=')
	{
		return forward+1;
	}
	else if(char_stream[forward]=='|' && char_stream[forward+1]=='=')
	{
		return forward+2;
	}
	else if(char_stream[forward]=='|' && char_stream[forward+1] != '=')
	{
		return forward+1;
	}
	return -1;
}

void Lexical_Error(std::string stream,int begin)
{
	while(begin<stream.length())
	{
		if(stream[begin]=='\'') begin = CHAR(stream,begin+1);
		else if(IsDirective(stream,begin) != -1) begin = IsDirective(stream,begin);
		else if(IsKeyWord(stream,begin) != -1) begin = IsKeyWord(stream,begin);
		else if(Is_Boolean(stream,begin) != -1) begin = Is_Boolean(stream,begin);
		else if(IsIdentifier(stream,begin) != -1) begin = IsIdentifier(stream,begin);
		else if(IsSeparator(stream,begin) != -1) begin = IsSeparator(stream,begin);
		else if(Is_Arithmetic(stream,begin) != -1) begin = Is_Arithmetic(stream,begin);
		else if(Is_Assignment(stream,begin) != -1) begin = Is_Assignment(stream,begin);
		else if(Is_Comparison(stream,begin) != -1) begin = Is_Comparison(stream,begin);
		else if(Is_Increment(stream,begin) != -1) begin = Is_Increment(stream,begin);
		else if(Is_Decrement(stream,begin) != -1) begin = Is_Decrement(stream,begin);
		else if(stream[begin]=='.' || IsDigit(stream[begin])) begin = IsConstant(stream,begin);
		else if(stream[begin]=='"') begin = STRING(stream,begin+1);
		else if(stream[begin]==32) begin++;
		else if(stream[begin]=='$') return;
		else 
		{
			std::cout<<"Lexical Error"<<std::endl;
			exit(0);
		}
	}
}
