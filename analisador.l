%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIMBOLOS 100
#define MAX_ERROS 100 

// tabela de simbolo
typedef struct {
    char nome[100];
} Simbolo;


// tabela erros
typedef struct {
    char lexema[100];
    int linha;
} ErroLexico; 

Simbolo tabela_de_simbolos[MAX_SIMBOLOS];
int num_simbolos = 0;

ErroLexico erros_lexicos[MAX_ERROS];
int num_erros = 0;


// adiciona na tabela de simbolo
void adicionar_simbolo(const char *nome, int linha) {
    for (int i = 0; i < num_simbolos; i++) {
        if (strcmp(tabela_de_simbolos[i].nome, nome) == 0) {
            return;  
        }
    }
    
    strcpy(tabela_de_simbolos[num_simbolos].nome, nome);
    num_simbolos++;
}


// mostra a tabela de simbolo
void exibir_tabela_simbolos() {
    printf("\nTabela de Simbolos:\n");
    printf("-----------------------------------------\n");
    for (int i = 0; i < num_simbolos; i++) {
        printf("| %-3d | %-30s |\n", i + 1, tabela_de_simbolos[i].nome);
    }
    printf("-----------------------------------------\n");
}

// adiciona na tabela de erro
void registrar_erro_lexico(const char *lexema, int linha) {
   
    if (num_erros < MAX_ERROS) {
        strcpy(erros_lexicos[num_erros].lexema, lexema);
        erros_lexicos[num_erros].linha = linha;
        num_erros++;
    } else {
        fprintf(stderr, "Limite de erros lexicais atingido.\n");
    }
}

//mostra a tabela de erro
void exibir_erros_lexicos() {
    if (num_erros > 0) {
        printf("\nRelatorio de Erros:\n");
        printf("--------------------------------------------------\n");
        printf("| Lexema                   | Linha              |\n");
        printf("--------------------------------------------------\n");
        for (int i = 0; i < num_erros; i++) {
            printf("| %-24s | %-18d |\n", erros_lexicos[i].lexema, erros_lexicos[i].linha);
        }
        printf("--------------------------------------------------\n");
    } else {
        printf("\nNenhum erro lexico encontrado.\n");
    }
}

// gerencia o erro
void erro_lexico(const char *lexema, int linha) {

//    printf("Erro lexico encontrado: '%s' na linha %d\n", lexema, linha); // mostra o erro na lista de tokens

    registrar_erro_lexico(lexema, linha); // chama a função de registrar o erro
}

extern int yylineno;
%}

%option yylineno
%option noyywrap

D       [0-9]
L       [a-zA-Z]
NUMINT     {D}({D})*
NUMDEC {D}({D})*\.{D}({D})*
ID      {L}({L}|{D})*
TEXTO   \"([^\\\"]|\\.)*\"  
COMENT  \/\/.*

%%

{COMENT}               { /* ignora comentario */ }

"int"                  { printf("INT\n", yytext); }
"float"                { printf("FLOAT\n", yytext); }
"char"                 { printf("CHAR\n", yytext); }
"string"               { printf("STRING\n", yytext); }
"bool"                 { printf("BOOL\n", yytext); }

"if"                   { printf("IF\n", yytext); }
"else"                 { printf("ELSE\n", yytext); }
"for"                  { printf("FOR\n", yytext); }
"do"                   { printf("DO\n", yytext); }
"while"                { printf("WHILE\n", yytext); }

"class"                { printf("CLASS\n", yytext); }
"function"             { printf("FUNCTION\n", yytext); }
"break"                { printf("BREAK\n", yytext); }
"return"               { printf("RETURN\n", yytext); }
"void"                 { printf("VOID\n", yytext); }
"null"                 { printf("NULL\n", yytext); }

"true"                 { printf("TRUE\n", yytext); }
"false"                { printf("FALSE\n", yytext); }

{NUMINT}                  { printf("NUMINT, %s\n", yytext); }
{NUMDEC}              { printf("NUMDEC, %s\n", yytext); }
{ID}                   {
                            int id = -1;
                            for (int i = 0; i < num_simbolos; i++) {
                                if (strcmp(tabela_de_simbolos[i].nome, yytext) == 0) {
                                    id = i + 1;
                                    break;
                                }
                            }
                            if (id == -1) {
                                id = num_simbolos + 1;
                                strcpy(tabela_de_simbolos[num_simbolos].nome, yytext);
                                num_simbolos++;
                            }

                            printf("ID, %d\n", id);
                        }

{TEXTO}                { printf("TEXTO\n", yytext); }

"="                    { printf("%s\n", yytext); }
"+"                    { printf("%s\n", yytext); }
"-"                    { printf("%s\n", yytext); }
"*"                    { printf("%s\n", yytext); }
"/"                    { printf("%s\n", yytext); }
"%"                    { printf("%s\n", yytext); }

"=="                   { printf("%s\n", yytext); }
"!="                   { printf("%s\n", yytext); }
">"                    { printf("%s\n", yytext); }
"<"                    { printf("%s\n", yytext); }
">="                   { printf("%s\n", yytext); }
"<="                   { printf("%s\n", yytext); }

"||"                   { printf("%s\n", yytext); }
"&&"                   { printf("%s\n", yytext); }
"!"                    { printf("%s\n", yytext); }

","                    { printf("%s\n", yytext); }
"."                    { printf("%s\n", yytext); }
";"                    { printf("%s\n", yytext); }
":"                    { printf("%s\n", yytext); }
"\\"                   { printf("%s\n", yytext); }
"("                    { printf("%s\n", yytext); }
")"                    { printf("%s\n", yytext); }
"{"                    { printf("%s\n", yytext); }
"}"                    { printf("%s\n", yytext); }
"["                    { printf("%s\n", yytext); }
"]"                    { printf("%s\n", yytext); }
"?"                    { printf("%s\n", yytext); }
"#"                    { printf("%s\n", yytext); }


[ \t\n]+               ; /* ignora espacos em branco e quebras de linha */

.                     { erro_lexico(yytext, yylineno); } /* peaga o erro lexico com o lexema e linha */

%%

int main() {
    FILE *file = fopen("codigo_fonte.txt", "r");
    if (file == NULL) {
        fprintf(stderr, "Erro ao abrir o arquivo.\n");
        return 1;
    }


    printf("Lista de Tokens:\n");
    printf("--------------------------------------------------\n");
    yyin = file; 
    yylex();      
    fclose(file);

    exibir_tabela_simbolos();
    exibir_erros_lexicos();


    return 0;
}


// comandos para rodar
// win_flex analisador.l 
// gcc lex.yy.c -o analisador
// ./analisador
// 