%{
	#define yywrap() 1
%}
keywords (char|break|const|continue|double|default|else|float|int|str|if|long|case|func|for|until|write|read|readLine)
arithmetic_operators (\+|\/|\*|\-|\%|\^|\*=|\/=|\%=|\^=|\&|\&=|\|=|\|)
comparison_operators (\==|\!=|\>=|\<=|\>|\<)
boolean_operators (or|and)
increment_operators (\++|\+=)
decrement_operators (\-=|\--)
assignment_operator (\=)
digits (0|1|2|3|4|5|6|7|8|9)
alphabets [a-zA-Z]
alphanums [a-zA-Z0-9]
separators (\;|\{|\}|\(|\)|\[|\]|\,|\:)
pre-processor (use)
%%
{pre-processor} printf("(pre-processor directive)",yytext);
'\\(n|r|t|b|f|v|0|\'|\"|\\)'|\'[^\\]\' printf("(C_literal)",yytext);
{arithmetic_operators} printf("(arithmetic_operator)",yytext);
{comparison_operators} printf("(comparison_operator)",yytext);
{boolean_operators} printf("(boolean_operator)",yytext);
{increment_operators} printf("(increment_operator)",yytext);
{decrement_operators} printf("(decrement_operator)",yytext);
{assignment_operator} printf("(assignment_operator)",yytext);
\"[^\"]+\"|\"\" printf("(S_literal)",yytext);
{digits}+|{digits}+\.{digits}+|\.{digits}+ printf("(constant)",yytext);
{keywords} printf("(keyword)",yytext);
({alphabets}|\_)({alphanums}|\_)* printf("(identifier)",yytext);
(\;|\{|\}|\(|\)|\[|\]|\,|\:) printf("(separator)",yytext);
%%
int main()
{
yyin = fopen("compiler.pg","r");
yyout = freopen("Tokens.txt","w",stdout);
yylex();
}
