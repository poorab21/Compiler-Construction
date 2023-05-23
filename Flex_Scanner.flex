%{
	#define yywrap() 1
%}
keywords (char|break|const|continue|double|default|else|float|int|str|if|long|switch|func|for|until|write|read|readLine)
operators (\+|\/|\*|\-|\%|\~|\!|\@|\^|\&|\=|\?|\<|\>|or|and|\++|\--|\<<|\>>|\<=|\+=|\-=|\*=|\/=|\==|\|=|\%=|\&=|\^=|\&&|\!=|\<<=|\>>=)
digits (0|1|2|3|4|5|6|7|8|9)
alphabets [a-zA-Z]
alphanums [a-zA-Z0-9]
separators (\;|\{|\}|\(|\)|\[|\]|\,|\:)
pre-processor (use)
dot (\.)
%%
{pre-processor} printf("(pre-processor directive)",yytext);
{dot} printf("(dot)",yytext);
'\\(n|r|t|b|f|v|0|\'|\"|\\)'|'.' printf("(C_literal)",yytext);
{operators} printf("(Operator)",yytext);
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
