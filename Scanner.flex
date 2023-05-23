%{
	#define yywrap() 1
	extern int yylex();
	#include<iostream>
	#include "bison.tab.h"
	#include<stdio.h>
%}
%option noyywrap

keywords (char|const|double|default|else|float|int|str|if|long|case|void|for|until)
arithmetic_operators (\+|\/|\*|\-|\%|\^|\*=|\/=|\%=|\^=|\&|\&=|\|=|\|)
comparison_operators (\==|\!=|\>=|\<=|\>|\<)
boolean_operators (or|and)
increment_operators (\++|\+=)
decrement_operators (\-=|\--)
assignment_operator (\=)
digits (0|1|2|3|4|5|6|7|8|9)
alphabets [a-zA-Z]
alphanums [a-zA-Z0-9]
separators (\;|\(|\)|\[|\]|\,|\:)
pre-processor (use)
continue_break (continue|break)
input_output (read|readLine|write)
return_type (return)
%%
{return_type} {return RETURN_TYPE;}
(func) {return FUNCTION;}
{continue_break} {return CONTINUE_BREAK;}
{input_output} { return WRITE_READ_COMMAND;}
{pre-processor} { yylval.sval = "(pre-processor directive)";return DIRECTIVE;}
\"[^\"]+\"|\"\" { yylval.sval = "(S_literal)";return STRING;}
{keywords} {yylval.sval = yytext; return KEYWORD;}
{boolean_operators} { yylval.sval = yytext; return BOOLEAN_OP; }
({alphabets}|\_)({alphanums}|\_)* { yylval.sval = yytext; return IDENTIFIER; }
(\;|\(|\)|\[|\]|\,|\:) { yylval.sval = yytext; return SEPARATOR; }
{digits}+|{digits}+\.{digits}+|\.{digits}+ { yylval.sval = yytext; return CONSTANT; }
'\\(n|r|t|b|f|v|0|\'|\"|\\)'|\'[^\\]\' { yylval.sval = yytext; return CHARACTER; }
{assignment_operator} { yylval.sval = yytext; return ASSIGNMENT; }
(\+|\/|\*|\-|\%|\^|\&|\|) { yylval.sval = yytext; return OP; }
(\*=|\/=|\%=|\^=|\&=|\+=|\-=|\|=) { yylval.sval = yytext; return OP_2; }
(\++) { yylval.sval = yytext; return INCREMENT; }
(\--) { yylval.sval = yytext; return DECREMENT; }
{comparison_operators} { yylval.sval = yytext; return COMPARISON; }
(\{|\}) { return SEPARATOR_2; }
%%
