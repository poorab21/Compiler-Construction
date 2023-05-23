#include<iostream>
#include<fstream>
std::string keywords[19] = {"char","break","const","continue","double","default","else","float","int","str",
						"if","long","case","func","for","until","write","read","readLine"};

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
	else if(IsDigit(char_stream[forward])==false) return forward-1;
}

//Checks if Lexeme is a String Literal
int IsLiteral_S(std::string char_stream,int forward)
{
	if(char_stream[forward] != '"')
	{
		return IsLiteral_S(char_stream,forward+1);
	}
	else return forward+1;
}

//Checks if Lexeme is a Separator
bool IsSeparator(std::string char_stream,int begin)
{
	if(char_stream[begin]==';' || char_stream[begin]=='{' || char_stream[begin]=='}' || char_stream[begin]=='(' || char_stream[begin]==')' || char_stream[begin]=='[' || char_stream[begin]==']' || char_stream[begin]==',' || char_stream[begin]==':')
	{
		return true;
	}
	return false;
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

//Checks if Lexeme is an Identifier
int IsIdentifier(std::string char_stream,int forward)
{
	if(IsAlpha(char_stream[forward]) || IsDigit(char_stream[forward]) || char_stream[forward]=='_') 
	{
		return IsIdentifier(char_stream,forward+1);
	}
	else return forward-1;
}

//Checks if Lexeme is a Character Literal
int IsLiteral_C(std::string char_stream,int forward)
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
}

//Checks if given word matches a keyword 
bool Match_Keyword(std::string word)
{
	for(int i=0;i<19;i++)
	{
		if(word==keywords[i]) return true;
	}
	return false;
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
	if(Match_Keyword(word)) return forward;
	else return -1;
}

//Checks if Lexeme is an Operator
int IsOperator(std::string char_stream,int forward)
{	// and (&&) Operator
	if(char_stream[forward]=='a' && char_stream[forward+1]=='n' && char_stream[forward+2]=='d' && char_stream[forward+3]==' ')
	{
		return forward+4;
	}
	else if(char_stream[forward]=='o' && char_stream[forward+1]=='r' && char_stream[forward+2]==' ') // or (||) Operator
	{
		return forward+3;
	}
	else if((char_stream[forward]=='+' && char_stream[forward+1]=='+') || (char_stream[forward]=='+' && char_stream[forward+1]=='='))
	{// (+=) or (++) incremental operator
		return forward+2;
	}
	else if((char_stream[forward]=='-' && char_stream[forward+1]=='-') || (char_stream[forward]=='-' && char_stream[forward+1]=='='))
	{// (--) or (-=) decremental operator
		return forward+2;
	}
	else if(char_stream[forward]=='+' || char_stream[forward]=='-') // (+ = addition operator) or (- = subtraction operator) 
	{
		return forward+1;
	}
	else if(char_stream[forward]=='*' && char_stream[forward+1]=='=') // (*=) product increment
	{
		return forward+2;
	}
	else if(char_stream[forward]=='*') // (*) Multiplication Operator
	{
		return forward+1;
	}
	else if(char_stream[forward]=='/' && char_stream[forward+1]=='=') // (/=) 
	{
		return forward+2;
	}
	else if(char_stream[forward]=='/') // (/) Division
	{
		return forward+1;
	}
	else if(char_stream[forward]=='%' && char_stream[forward+1]=='=') // (%=) 
	{
		return forward+2;
	}
	else if(char_stream[forward]=='%') //(%) remainder operator
	{
		return forward+1;
	}
	else if(char_stream[forward]=='~') //(~) tilde operator
	{
		return forward+1;
	}
	else if(char_stream[forward]=='!' && char_stream[forward+1]=='=') // (!=)
	{
		return forward+2;
	}
	else if(char_stream[forward]=='!') // (! operator)
	{
		return forward+1;
	}
	else if(char_stream[forward]=='@')
	{
		return forward+1;
	}
	else if(char_stream[forward]=='^' && char_stream[forward+1]=='=') // (^= operator)
	{
		return forward+2;
	}
	else if(char_stream[forward]=='^') // (^ operator)
	{
		return forward+1;
	}
	else if(char_stream[forward]=='&' && char_stream[forward+1]=='=') // (&= operator)
	{
		return forward+2;
	}
	else if(char_stream[forward]=='&') // (& operator)
	{
		return forward+1;
	}
	else if(char_stream[forward]=='=' && char_stream[forward+1]=='=') // (== operator)
	{
		return forward+2;
	}
	else if(char_stream[forward]=='=') //(= operator)
	{
		return forward+1;
	}
	else if(char_stream[forward]=='?') // (question_mark operator)
	{
		return forward+1;
	}
	else if(char_stream[forward]=='<' && char_stream[forward+1]=='<' && char_stream[forward+2]=='=') // (<<= operator)
	{
		return forward+3;
	}
	else if(char_stream[forward]=='<' && char_stream[forward+1]=='<') // (<< operator)
	{
		return forward+2;
	}
	else if(char_stream[forward]=='<' && char_stream[forward+1]=='=') // (<= operator) less than or equal
	{
		return forward+2;
	}
	else if(char_stream[forward]=='<') // (< operator) less than 
	{
		return forward+1;
	}
	else if(char_stream[forward]=='>' && char_stream[forward+1]=='>' && char_stream[forward+2]=='=')
	{ // (>>=) operator
		return forward+3;
	}
	else if(char_stream[forward]=='>' && char_stream[forward+1]=='>') // (>> operator)
	{
		return forward+2;
	}
	else if(char_stream[forward]=='>' && char_stream[forward+1]=='=') // (>= operator) more than or equal
	{
		return forward+2;
	}
	else if(char_stream[forward]=='>') // (> operator) more than
	{
		return forward+1;
	}
	else if(char_stream[forward]=='|' && char_stream[forward+1]=='=') // (|=)
	{
		return forward+2;
	}
	return -1;
}
int main()
{
	std::ofstream fileholder("Tokens.txt");
	std::ifstream source_file("Compiler.pg");
	int begin = 0,forward = 0;
	std::string stream = "";
	while(getline(source_file,stream))
	{
		while(begin<stream.length())
		{
			if(stream[begin]>='0' && stream[begin]<='9' || stream[begin]=='.')
			{	
				forward = IsConstant(stream,forward) + 1;
				begin = IsConstant(stream,forward) + 1;
				fileholder<<"(constant)";
			}
			else if(IsOperator(stream,forward) != -1)
			{
				begin = IsOperator(stream,forward);
				forward = IsOperator(stream,forward);
				fileholder<<"(Operator)";
			}
			else if(stream[begin]=='"')
			{
				begin = IsLiteral_S(stream,forward+1);
				forward = IsLiteral_S(stream,forward+1);
				fileholder<<"(S_Literal)";
			}
			else if(IsSeparator(stream,begin)==true)
			{
				fileholder<<"(Separator)";
				begin++;
				forward++;
			}
			else if(IsAlpha(stream[begin]) || stream[begin]=='_')
			{
				int result = IsKeyWord(stream,forward);
				if(IsDirective(stream,forward) != -1)
				{
					begin = IsDirective(stream,forward);
					forward = IsDirective(stream,forward);
					fileholder<<"(Preprocessor_Directive)";
				}
				else if(result != -1)
				{
					begin = result;
					forward = result;
					fileholder<<"(Keyword)";
				}
				else
				{
					begin = IsIdentifier(stream,forward) + 1;
					forward = IsIdentifier(stream,forward) + 1;
					fileholder<<"(Identifier)";	
				}
			}
			else if(stream[begin]=='\'')
			{
				begin = IsLiteral_C(stream,forward+1);
				forward = IsLiteral_C(stream,forward+1);
				fileholder<<"(C_Literal)";
			}
			else
			{
				begin++;
				forward=begin;
			}
		}
		begin = 0;
		forward = 0;
		fileholder<<std::endl;
	}
	fileholder.close();
	source_file.close();
}
