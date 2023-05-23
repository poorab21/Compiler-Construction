#include<iostream>
#include<fstream>
#include"Lexical_Analyser.cpp"

int A(std::string,int);
int VALUE(std::string,int);
int FUNC_4(std::string,int);
int Match(std::string instruction,int &lookahead,char x)
{
	if(instruction[lookahead] != x) return -1; 
	else return lookahead+1;
}
void Clear_Space(std::string instruction,int &lookahead)
{
	while(instruction[lookahead]==32) lookahead++;
}

int CONTINUE_BREAK(std::string instruction,int lookahead)
{
	lookahead = IsKeyWord(instruction,lookahead);
	lookahead = IsSeparator(instruction,lookahead);
	if(instruction[lookahead] != '$') return -1;
	else return lookahead;
}

int INCREMENT_DECREMENT(std::string instruction,int lookahead)
{
	lookahead = IsIdentifier(instruction,lookahead);
	if(instruction[lookahead]=='+')
	{
		lookahead = Match(instruction,lookahead,'+');
		lookahead = Match(instruction,lookahead,'+');
		lookahead = IsSeparator(instruction,lookahead);
	}
	else if(instruction[lookahead]=='-')
	{
		lookahead = Match(instruction,lookahead,'-');
		lookahead = Match(instruction,lookahead,'-');
		lookahead = IsSeparator(instruction,lookahead);
	}
	
	if(instruction[lookahead] != '$') return -1;
	else return lookahead;
}

int B(std::string instruction,int lookahead)
{
	if(A(instruction,lookahead) != -1) return A(instruction,lookahead);
	else return Match(instruction,lookahead,';');
}

int A(std::string instruction,int lookahead)
{
	if(Match(instruction,lookahead,'"') != -1) 
	{
		lookahead = STRING(instruction,lookahead+1);
		lookahead = IsSeparator(instruction,lookahead);
		lookahead = B(instruction,lookahead);
		return lookahead;
	}
	else if(IsIdentifier(instruction,lookahead) != -1)
	{
		lookahead = IsIdentifier(instruction,lookahead);
		lookahead = IsSeparator(instruction,lookahead);
		lookahead = B(instruction,lookahead);
		return lookahead;
	}
	else return -1;
}

int WRITE_READ(std::string instruction,int lookahead)
{
	lookahead = IsKeyWord(instruction,lookahead);
	lookahead = IsSeparator(instruction,lookahead);
	lookahead = A(instruction,lookahead);
	if(instruction[lookahead]=='$' && instruction.substr(0,4) != "case") return lookahead;
	else return -1; 
}

int CONDITIONS(std::string instruction,int lookahead)
{
	lookahead = IsIdentifier(instruction,lookahead);
	Clear_Space(instruction,lookahead);
	lookahead = Is_Comparison(instruction,lookahead);
	Clear_Space(instruction,lookahead);
	if(IsIdentifier(instruction,lookahead) != -1) 
	{
		lookahead = IsIdentifier(instruction,lookahead);
	}
	else if(IsConstant(instruction,lookahead) != lookahead) 
	{	
		lookahead = IsConstant(instruction,lookahead);
	}
	else return -1;
	
	return lookahead;
}

int CONDITION(std::string instruction,int lookahead)
{
	Clear_Space(instruction,lookahead);
	lookahead = CONDITIONS(instruction,lookahead);
	Clear_Space(instruction,lookahead); 
	if(Match(instruction,lookahead,')') != -1) return lookahead;
	else if(lookahead==-1) return -1;
	else
	{
		lookahead = Is_Boolean(instruction,lookahead);
		return CONDITION(instruction,lookahead);
	}
}

int ELSE_IF(std::string instruction,int lookahead)
{
	lookahead = IsKeyWord(instruction,lookahead);
	Clear_Space(instruction,lookahead);
	lookahead = IsKeyWord(instruction,lookahead);
	lookahead = IsSeparator(instruction,lookahead);
	lookahead = CONDITION(instruction,lookahead);
	lookahead = IsSeparator(instruction,lookahead);
	
	
	if(instruction[lookahead] != '$') return -1;
	else return lookahead;
}

int IF_UNTIL_OP(std::string instruction,int lookahead)
{
	lookahead = IsKeyWord(instruction,lookahead);
	lookahead = IsSeparator(instruction,lookahead);
	lookahead = CONDITION(instruction,lookahead);
	lookahead = IsSeparator(instruction,lookahead);
	
	
	if(instruction[lookahead] != '$') return -1;
	else return lookahead;
}

int CASE_OPTION(std::string instruction,int lookahead)
{
	lookahead = IsSeparator(instruction,lookahead);
	if(IsIdentifier(instruction,lookahead) != -1) lookahead = IsIdentifier(instruction,lookahead);
	else if(Match(instruction,lookahead,'"') != -1) lookahead = STRING(instruction,lookahead+1);
	else if(Match(instruction,lookahead,'\'') != -1) lookahead = CHAR(instruction,lookahead+1);
	else if(IsDigit(instruction[lookahead])==true || instruction[lookahead]=='.') lookahead = IsConstant(instruction,lookahead);
	lookahead = IsSeparator(instruction,lookahead);
	lookahead = IsSeparator(instruction,lookahead);
	
	if(instruction[lookahead]=='$') return lookahead;
	else return -1;
}

int VARIABLE_3(std::string instruction,int lookahead)
{
	if(Match(instruction,lookahead,'"') != -1)
	{
		lookahead = STRING(instruction,lookahead+1);
		lookahead = IsSeparator(instruction,lookahead);
	}
	else if(Match(instruction,lookahead,'\'') != -1)
	{
		lookahead = CHAR(instruction,lookahead+1);
		lookahead = IsSeparator(instruction,lookahead);
	}
	else if(IsIdentifier(instruction,lookahead) != -1)
	{
		lookahead = IsIdentifier(instruction,lookahead);
		lookahead = IsSeparator(instruction,lookahead);
	}
	else if(IsConstant(instruction,lookahead) != lookahead)
	{
		lookahead = IsConstant(instruction,lookahead);
		lookahead = IsSeparator(instruction,lookahead);
	}
	else return -1;
	
	return lookahead;
}

int VARIABLE_2(std::string instruction,int lookahead)
{
	Clear_Space(instruction,lookahead);
	if(Is_Assignment(instruction,lookahead) != -1)
	{
		lookahead = Is_Assignment(instruction,lookahead);
		Clear_Space(instruction,lookahead);
		lookahead = VARIABLE_3(instruction,lookahead);
		return lookahead;
	}
	else return IsSeparator(instruction,lookahead);
}

int VARIABLE(std::string instruction,int lookahead)
{
	lookahead = IsKeyWord(instruction,lookahead);
	lookahead = IsSeparator(instruction,lookahead);
	
	if(IsIdentifier(instruction,lookahead) != -1)
	{
		lookahead = IsIdentifier(instruction,lookahead);
		lookahead = VARIABLE_2(instruction,lookahead);
	}
	else if(lookahead != IsConstant(instruction,lookahead))
	{
		lookahead = IsConstant(instruction,lookahead);
		lookahead = IsSeparator(instruction,lookahead);
		lookahead = IsSeparator(instruction,lookahead);
		lookahead = IsIdentifier(instruction,lookahead);
		lookahead = IsSeparator(instruction,lookahead);
	}
	else return -1;
	if(instruction[lookahead]=='$' && instruction.substr(0,5) != "write" && instruction.substr(0,4) != "read" && instruction.substr(0,2) != "if" && instruction.substr(0,4) != "else" && instruction.substr(0,5) != "until") return lookahead;
	else return -1;
}

int VALUE_2(std::string instruction,int lookahead)
{
	Clear_Space(instruction,lookahead);
	if(Match(instruction,lookahead,'+') != -1)
	{
		lookahead = Match(instruction,lookahead,'+');
		lookahead = VALUE(instruction,lookahead);
	}
	else if(Match(instruction,lookahead,'-') != -1)
	{
		lookahead = Match(instruction,lookahead,'-');
		lookahead = VALUE(instruction,lookahead);
	}
	else if(Match(instruction,lookahead,'/') != -1)
	{
		lookahead = Match(instruction,lookahead,'/');
		lookahead = VALUE(instruction,lookahead);
	}
	else if(Match(instruction,lookahead,'*') != -1)
	{
		lookahead = Match(instruction,lookahead,'*');
		lookahead = VALUE(instruction,lookahead);
	}
	else if(Match(instruction,lookahead,'%') != -1)
	{
		lookahead = Match(instruction,lookahead,'%');
		lookahead = VALUE(instruction,lookahead);
	}
	else if(Match(instruction,lookahead,'^') != -1)
	{
		lookahead = Match(instruction,lookahead,'^');
		lookahead = VALUE(instruction,lookahead);
	}
	else if(Match(instruction,lookahead,'&') != -1)
	{
		lookahead = Match(instruction,lookahead,'&');
		lookahead = VALUE(instruction,lookahead);
	}
	else if(Match(instruction,lookahead,'|') != -1)
	{
		lookahead = Match(instruction,lookahead,'|');
		lookahead = VALUE(instruction,lookahead);
	}
	return lookahead;
}

int VALUE(std::string instruction,int lookahead)
{
	Clear_Space(instruction,lookahead);
	if(IsIdentifier(instruction,lookahead) != -1) 
	{
		lookahead = IsIdentifier(instruction,lookahead);
		lookahead = VALUE_2(instruction,lookahead);
	}
	else if(IsConstant(instruction,lookahead) != lookahead)
	{
		lookahead = IsConstant(instruction,lookahead);
		lookahead = VALUE_2(instruction,lookahead);
	}
	else if(Match(instruction,lookahead,';') != -1) return -1;
	return lookahead;
}

int VARIABLE_NEW_VALUE_2(std::string instruction,int lookahead)
{
	Clear_Space(instruction,lookahead);
	if(Match(instruction,lookahead,'"') != -1)
	{
		lookahead = STRING(instruction,lookahead+1);
		lookahead = IsSeparator(instruction,lookahead);
	}
	else if(Match(instruction,lookahead,'\'') != -1)
	{
		lookahead = CHAR(instruction,lookahead+1);
		lookahead = IsSeparator(instruction,lookahead);
	}
	else
	{
		lookahead = VALUE(instruction,lookahead);
		lookahead = IsSeparator(instruction,lookahead);
	}
	return lookahead;
}

int OP_TYPE_3(std::string instruction,int lookahead)
{
	if(IsIdentifier(instruction,lookahead) != -1)
	{
		lookahead = IsIdentifier(instruction,lookahead);
		lookahead = IsSeparator(instruction,lookahead);
	}
	else
	{
		lookahead = IsConstant(instruction,lookahead);
		lookahead = IsSeparator(instruction,lookahead);
	}
	
	if(instruction[lookahead]=='$') return lookahead;
	else return -1;
}

int OP_TYPE_2(std::string instruction,int lookahead)
{
	lookahead = IsIdentifier(instruction,lookahead);
	Clear_Space(instruction,lookahead);
	if(Match(instruction,lookahead,'+') != -1 || Match(instruction,lookahead,'-') != -1 || Match(instruction,lookahead,'/') != -1 || Match(instruction,lookahead,'*') != -1 || Match(instruction,lookahead,'%') != -1 || Match(instruction,lookahead,'^') != -1 || Match(instruction,lookahead,'&') != -1 || Match(instruction,lookahead,'|') != -1)
	{
		lookahead++;
		lookahead = Match(instruction,lookahead,'=');
	}
	
	Clear_Space(instruction,lookahead);
	lookahead = OP_TYPE_3(instruction,lookahead);
	
	return lookahead;
}

int VARIABLE_NEW_VALUE(std::string instruction,int lookahead)
{
	if(OP_TYPE_2(instruction,lookahead) != -1)
	{
		lookahead = OP_TYPE_2(instruction,lookahead);
	}
	else if(IsIdentifier(instruction,lookahead) != -1)
	{
		lookahead = IsIdentifier(instruction,lookahead);
		Clear_Space(instruction,lookahead);
		lookahead = Is_Assignment(instruction,lookahead);
		Clear_Space(instruction,lookahead);
		lookahead = VARIABLE_NEW_VALUE_2(instruction,lookahead);
	}
	
	if(instruction[lookahead] != '$') return -1;
	else return lookahead;
}

int RETURN(std::string instruction,int lookahead)
{
	lookahead = IsKeyWord(instruction,lookahead);
	lookahead = VALUE(instruction,lookahead);
	lookahead = IsSeparator(instruction,lookahead);
	
	if(instruction[lookahead] != '$') return -1;
	else return lookahead;
}

int LOOP_5(std::string instruction,int lookahead)
{
	if(IsIdentifier(instruction,lookahead) != -1) return IsIdentifier(instruction,lookahead);
	else return IsConstant(instruction,lookahead);
}

int LOOP_4(std::string instruction,int lookahead)
{
	if(instruction[lookahead]=='+' && instruction[lookahead+1]=='+') return lookahead+2;
	else if(instruction[lookahead]=='-' && instruction[lookahead+1]=='-') return lookahead+2;
	else if(instruction[lookahead]=='-' && instruction[lookahead+1]=='=' || instruction[lookahead]=='+' && instruction[lookahead+1]=='=')
	{
		lookahead+=2;
		lookahead = LOOP_5(instruction,lookahead);
	}
	return lookahead;
}

int LOOP_3(std::string instruction,int lookahead)
{
	Clear_Space(instruction,lookahead);
	lookahead = IsIdentifier(instruction,lookahead);
	lookahead = LOOP_4(instruction,lookahead);
	return lookahead;
}

int LOOP_2(std::string instruction,int lookahead)
{
	Clear_Space(instruction,lookahead);
	lookahead = IsIdentifier(instruction,lookahead);
	Clear_Space(instruction,lookahead);
	lookahead = Is_Comparison(instruction,lookahead);
	Clear_Space(instruction,lookahead);
	if(IsIdentifier(instruction,lookahead) != -1) lookahead = IsIdentifier(instruction,lookahead);
	else lookahead = IsConstant(instruction,lookahead);
	
	Clear_Space(instruction,lookahead);
	lookahead = IsSeparator(instruction,lookahead);
	lookahead = LOOP_3(instruction,lookahead);
	return lookahead;
}

int LOOP(std::string instruction,int lookahead)
{
	lookahead = IsIdentifier(instruction,lookahead);
	Clear_Space(instruction,lookahead);
	lookahead = Is_Assignment(instruction,lookahead);
	Clear_Space(instruction,lookahead);
	if(IsIdentifier(instruction,lookahead) != -1) lookahead = IsIdentifier(instruction,lookahead);
	else lookahead = IsConstant(instruction,lookahead);
	
	Clear_Space(instruction,lookahead);
	lookahead = IsSeparator(instruction,lookahead);
	lookahead = LOOP_2(instruction,lookahead);
	return lookahead;
}

int FOR(std::string instruction,int lookahead)
{
	lookahead = IsKeyWord(instruction,lookahead);
	lookahead = IsSeparator(instruction,lookahead);
	Clear_Space(instruction,lookahead);
	lookahead = LOOP(instruction,lookahead);
	Clear_Space(instruction,lookahead);
	lookahead = IsSeparator(instruction,lookahead);
	
	if(instruction[lookahead] != '$') return -1;
	else return lookahead;
}


int FUNC(std::string instruction,int lookahead)
{
	lookahead = IsKeyWord(instruction,lookahead);
	lookahead = IsSeparator(instruction,lookahead);
	lookahead = IsKeyWord(instruction,lookahead);
	lookahead = IsSeparator(instruction,lookahead);
	Clear_Space(instruction,lookahead);
	
	lookahead = IsIdentifier(instruction,lookahead);
	lookahead = IsSeparator(instruction,lookahead);
	lookahead = IsSeparator(instruction,lookahead);
	if(instruction[lookahead] != '$') return -1;
	else return lookahead;
}
int FUNC_CALL(std::string instruction,int lookahead)
{
	lookahead = IsIdentifier(instruction,lookahead);
	lookahead = IsSeparator(instruction,lookahead);
	lookahead = IsSeparator(instruction,lookahead);
	lookahead = IsSeparator(instruction,lookahead);
	 
	if(instruction[lookahead] != '$') return -1;
	else return lookahead;
}
void S(std::string instruction,int &lookahead)
{
	if(CONTINUE_BREAK(instruction,lookahead) != -1) lookahead = CONTINUE_BREAK(instruction,lookahead);
	else if(instruction[lookahead]=='{' || instruction[lookahead]=='}') lookahead++;
	else if(VARIABLE(instruction,lookahead) != -1) lookahead = VARIABLE(instruction,lookahead);
	else if(CASE_OPTION(instruction,lookahead) != -1) lookahead = CASE_OPTION(instruction,lookahead);
	else if(IsDirective(instruction,lookahead) != -1)
	{
		lookahead = IsDirective(instruction,lookahead);
		if(Match(instruction,lookahead,'"') != -1)
			lookahead = STRING(instruction,lookahead+1);
	}
	else if(INCREMENT_DECREMENT(instruction,lookahead) != -1) lookahead = INCREMENT_DECREMENT(instruction,lookahead);
	else if(WRITE_READ(instruction,lookahead) != -1) lookahead = WRITE_READ(instruction,lookahead);
	else if(VARIABLE_NEW_VALUE(instruction,lookahead) != -1) lookahead = VARIABLE_NEW_VALUE(instruction,lookahead);
	else if(RETURN(instruction,lookahead) != -1) lookahead = RETURN(instruction,lookahead);
	else if(FOR(instruction,lookahead) != -1) lookahead = FOR(instruction,lookahead);
	else if(FUNC(instruction,lookahead) != -1) lookahead = FUNC(instruction,lookahead);
	else if(IF_UNTIL_OP(instruction,lookahead) != -1) lookahead = IF_UNTIL_OP(instruction,lookahead);
	else if(ELSE_IF(instruction,lookahead) != -1) lookahead = ELSE_IF(instruction,lookahead);
	else if(FUNC_CALL(instruction,lookahead) != -1) lookahead = FUNC_CALL(instruction,lookahead);
}
int main()
{
	std::ifstream fileholder("Source_File(Lexical Error).ANP");
	std::string instruction = "";
	int lookahead = 0,line = 0;
	while(getline(fileholder,instruction))
	{
		instruction+="$";
		line++;
		S(instruction,lookahead);
		if(instruction[lookahead] != '$')
		{
			Lexical_Error(instruction,lookahead);
			std::cout<<"Syntax Error in Line "<<line<<std::endl;
			return 0;
		}
		instruction = "";
		lookahead = 0;
	}
	std::cout<<"Parsing Successful"<<std::endl;
	fileholder.close();
}
