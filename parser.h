#ifndef PARSER_H
#define PARSER_H

//Usar os Tokens por numeros
typedef enum {
    TOKEN_MAIN, TOKEN_INT, TOKEN_FLOAT, TOKEN_STRING, TOKEN_BOOL, TOKEN_VOID,
    TOKEN_IF, TOKEN_ELSE, TOKEN_FOR, TOKEN_PRINT, TOKEN_RETURN,
    TOKEN_FUNCTION, TOKEN_TRUE, TOKEN_FALSE,

    LBRACE,      // {
    RBRACE,      // }
    LPAREN,      // (
    RPAREN,      // )
    SEMI,        // ;
    COMMA,       // ,
    ASSIGN,      // =
    PLUS,        // +
    MINUS,       // -
    STAR,        // *
    SLASH,       // /

    EQ,          // ==
    NEQ,         // !=
    GT,          // >
    LT,          // <
    GE,          // >=
    LE,          // <=

    ID,
    NUMINT,
    NUMDEC,
    TEXTO,

    ENDFILE,      // Fim do arquivo (EOF)
    UNKNOWN       // Token desconhecido/erro
} TokenType;

extern int yylex();
extern char* yytext;
extern FILE* yyin;
extern int yylineno;

#endif 