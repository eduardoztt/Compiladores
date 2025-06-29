#include <stdio.h>
#include <stdlib.h>
#include "parser.h" 

void S(); void A(); void B(); void X(); void X_prime(); void X_double_prime();
void C(); void C_prime(); void Y(); void O(); void O_prime(); void P(); void P_prime();
void Q(); void D(); void K(); void L(); void M(); void N(); void E(); void T();
void G(); void U();
void R(); void R_prime(); void J(); void I(); void Z(); void V(); void W();
void W_prime(); void W_double_prime();

TokenType token;

const char* token_to_str[] = {
    "main", "int", "float", "string", "bool", "void",
    "if", "else", "for", "print", "return",
    "function", "true", "false'",
    "'{'", "'}'", "'('", "')'", "';'", "','", "'='", "'+'", "'-'", "'*'", "'/'",
    "'=='", "'!='", "'>'", "'<'", "'>='", "'<='",
    "um identificador", "um número inteiro", "um número decimal", "um texto",
    "o fim do arquivo", "um token desconhecido"
};


void error(const char* expected) {
    fprintf(stderr, "Erro Sintatico na linha %d: Esperado %s mas encontrado '%s'\n", yylineno, expected, yytext);
    
    while (token != SEMI && token != RBRACE && token != ENDFILE && token != LBRACE) {
        token = yylex();
    }
}

void match(TokenType expected_token) {
    if (token == expected_token) {
        if (token != ENDFILE) {
             token = yylex();
        }
    } else {
        char msg[200];
        sprintf(msg, "%s", token_to_str[expected_token]);
        error(msg);
    }
}


void S() {
    if (token == TOKEN_MAIN) {
        match(TOKEN_MAIN);
        match(LBRACE); 
        A();
        match(RBRACE);
        match(SEMI);
        Z();
    } else {
        error("'main'");
    }
}

void A() {
    B();
    X();
    X_prime();
}

void B() {
    if (token == TOKEN_INT || token == TOKEN_FLOAT || token == TOKEN_STRING || token == TOKEN_BOOL) {
        match(token); 
        match(ID);
        match(SEMI);
        B();
    }
    // Produção & (vazio)
}

void X() {
    if (token == ID) {
        match(ID);
        if (token == ASSIGN) {
            C();
        } else if (token == LPAREN) {
            R_prime();
        } else {
            error("'=' para atribuicao ou '(' para chamada de funcao");
        }
        match(SEMI);
        X();
    } else if (token == TOKEN_IF) {
        D(); X();
    } else if (token == TOKEN_FOR) {
        E(); X();
    } else if (token == TOKEN_PRINT) {
        G(); X();
    }
}


void X_prime() {
    if (token == TOKEN_RETURN) {
        match(TOKEN_RETURN);
        X_double_prime();
        match(SEMI);
    }
}

void X_double_prime() {
    if (token == ID) {
        match(ID);
    }
}

void C() {
    match(ASSIGN);
    C_prime();
}


void C_prime() {
    if (token == ID || token == NUMINT || token == NUMDEC || token == LPAREN) { 
        O();
    } else if (token == TOKEN_TRUE || token == TOKEN_FALSE || token == TEXTO) {
        Y();
    } else {
        error("expressão, valor booleano ou texto");
    }
}

void Y() {
    if (token == ID || token == NUMDEC || token == NUMINT) {
        L();
    } else if (token == TOKEN_TRUE) {
        match(TOKEN_TRUE);
    } else if (token == TOKEN_FALSE) {
        match(TOKEN_FALSE);
    } else if (token == TEXTO) {
        match(TEXTO);
    } else {
        error("valor (true/false/texto) ou número/ID");
    }
}

void O() {
    P();
    O_prime();
}

void O_prime() {
    if (token == PLUS) {
        match(PLUS);
        P();
        O_prime();
    } else if (token == MINUS) {
        match(MINUS);
        P();
        O_prime();
    }
}

void P() {
    Q();
    P_prime();
}

void P_prime() {
    if (token == STAR) {
        match(STAR);
        Q();
        P_prime();
    } else if (token == SLASH) {
        match(SLASH);
        Q();
        P_prime();
    }
}

void Q() {
    if (token == LPAREN) { 
        match(LPAREN);
        O();
        match(RPAREN);
    } 
    else if (token == ID) {
        match(ID); 

        if (token == LPAREN) { 
            match(LPAREN);
            J();
            match(RPAREN);
        }
    } 
    else if (token == NUMINT || token == NUMDEC || token == TEXTO || token == TOKEN_TRUE || token == TOKEN_FALSE) {
        L();
    }
    else {
        error("um identificador, número, texto ou chamada de função no início de uma expressão");
    }
}


void D() {
    match(TOKEN_IF);
    match(LPAREN);
    K();
    match(RPAREN);
    match(LBRACE);
    X();
    match(RBRACE);
    N();
    match(SEMI);
}

void K() {
    L();
    M();
    L();
}

void L() {
    if (token == ID) match(ID);
    else if (token == NUMDEC) match(NUMDEC);
    else if (token == NUMINT) match(NUMINT);
    else error("ID, número inteiro ou decimal");
}

void M() {
    if (token >= EQ && token <= LE) {
        match(token);
    } else {
        error("operador de comparacao (==, !=, >, etc.)");
    }
}

void N() {
    if (token == TOKEN_ELSE) {
        match(TOKEN_ELSE);
        match(LBRACE);
        X();
        match(RBRACE);
    }
}

void E() {
    match(TOKEN_FOR);
    match(LPAREN);
    match(ID);
    match(ASSIGN);
    match(NUMINT);
    match(SEMI);
    K(); 
    match(SEMI);
    T();
    match(RPAREN);
    match(LBRACE);
    X();
    match(RBRACE);
    match(SEMI);
}

void T() {
    if (token == PLUS) {
        match(PLUS);
        match(NUMINT);
    } else if (token == MINUS) {
        match(MINUS);
        match(NUMINT);
    } else if (token == NUMINT) { 
        match(NUMINT);
    } else {
        error("'+', '-' ou um número para a expressão do for");
    }
}

void G() {
    match(TOKEN_PRINT);
    match(LPAREN);
    U();
    match(RPAREN);
    match(SEMI);
}

void U() {
    if (token == ID) {
        match(ID);
    } else if (token == TEXTO) {
        match(TEXTO);
    } else {
        error("ID ou TEXTO para print");
    }
}


void Z() {
    if (token == TOKEN_FUNCTION) {
        match(TOKEN_FUNCTION);
        V();
        match(ID);
        match(LPAREN);
        W();
        match(RPAREN);
        match(LBRACE);
        A();
        match(RBRACE);
        match(SEMI);
        Z();
    }
}


void V() {
    if(token == TOKEN_INT || token == TOKEN_FLOAT || token == TOKEN_STRING || token == TOKEN_VOID) {
        match(token);
    } else {
        error("tipo de retorno da funcao (int, float, string, void)");
    }
}

void R() {
    if (token == ID) {
        match(ID);
        R_prime();
    } else {
        error(token_to_str[ID]);
    }
}

void R_prime() {
    if (token == LPAREN) { 
        match(LPAREN);
        J();
        match(RPAREN);
    } else if (token == ASSIGN) { 
        match(ASSIGN);
        match(ID);
        match(LPAREN);
        J();
        match(RPAREN);
    } else {
        error("'(' ou '=' para chamada de funcao");
    }
}

void J() {
    if (token == ID) { 
        match(ID);
        I();
    }
}

void I() {
    if (token == COMMA) { 
        match(COMMA);
        match(ID);
        I(); 
    }
}

void W() {
    if (token == TOKEN_INT || token == TOKEN_FLOAT || token == TOKEN_STRING || token == TOKEN_BOOL) {
        match(token);
        match(ID);
        W_prime();
    }
}

void W_prime() {
    if (token == COMMA) {
        match(COMMA);
        W_double_prime();
    }
}

void W_double_prime() {
    if (token == TOKEN_INT || token == TOKEN_FLOAT || token == TOKEN_STRING || token == TOKEN_BOOL) {
        match(token);
        match(ID);
        W_prime();
    } else {
        error("um tipo de parametro (int, float, etc.)");
    }
}


int main(int argc, char **argv) {
    char* filename = "codigo_fonte.txt";

    if (argc > 1) {
        filename = argv[1];
    }

    yyin = fopen(filename, "r");
    if (!yyin) {
        fprintf(stderr, "Nao foi possível abrir o arquivo %s\n", filename);
        return 1;
    }

    printf("Analisando o arquivo: %s\n", filename);
    printf("----------------------------------------\n");

    token = yylex();

    S();

    if (token == ENDFILE) {
        printf("\n----------------------------------------\n");
        printf("Analise Sintatica concluida com sucesso!\n");
    } else {
        error("codigo extra apos o final");
        printf("\n----------------------------------------\n");
        printf("Analise Sintatica concluida com erros.\n");
    }

    fclose(yyin);
    return 0;
}