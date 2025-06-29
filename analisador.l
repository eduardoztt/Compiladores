%{
#include "parser.h" // Inclui as definições de token
#include <string.h>


void yyerror(const char *s) {
    fprintf(stderr, "Erro Lexico na linha %d: %s (lexema: '%s')\n", yylineno, s, yytext);
}
%}

%option yylineno
%option noyywrap

D       [0-9]
L       [a-zA-Z]
NUMINT     {D}+
NUMDEC  {D}+\.{D}+
ID      {L}({L}|{D})*
TEXTO   \"([^\\\"]|\\.)*\"
COMENT  \/\/.*

%%

{COMENT}                      { /* ignora comentario */ }

"main"                        { return TOKEN_MAIN; }
"int"                         { return TOKEN_INT; }
"float"                       { return TOKEN_FLOAT; }
"char"                        | 
"string"                      { return TOKEN_STRING; }
"bool"                        { return TOKEN_BOOL; }
"void"                        { return TOKEN_VOID; }

"if"                          { return TOKEN_IF; }
"else"                        { return TOKEN_ELSE; }
"for"                         { return TOKEN_FOR; }
"print"                       { return TOKEN_PRINT; }
"return"                      { return TOKEN_RETURN; }
"function"                    { return TOKEN_FUNCTION; }

"true"                        { return TOKEN_TRUE; }
"false"                       { return TOKEN_FALSE; }

{NUMINT}                      { return NUMINT; }
{NUMDEC}                      { return NUMDEC; }
{ID}                          { return ID; }
{TEXTO}                       { return TEXTO; }

"{"                           { return LBRACE; }
"}"                           { return RBRACE; }
"("                           { return LPAREN; }
")"                           { return RPAREN; }
";"                           { return SEMI; }
","                           { return COMMA; }
"="                           { return ASSIGN; }
"+"                           { return PLUS; }
"-"                           { return MINUS; }
"*"                           { return STAR; }
"/"                           { return SLASH; }

"=="                          { return EQ; }
"!="                          { return NEQ; }
">"                           { return GT; }
"<"                           { return LT; }
">="                          { return GE; }
"<="                          { return LE; }

[ \t\n]+                      { /* ignora espacos */ }

.   {                          fprintf(stderr, "Lexema desconhecido '%s' na linha %d\n", yytext, yylineno); }

<<EOF>>                       { return ENDFILE; }

%%

// Para rodar o codigo
// win_flex analisador.l
// gcc parser.c lex.yy.c -o parser.exe
// \parser.exe