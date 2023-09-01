#ifndef TOKEN_H
#define TOKEN_H

#include <string>

enum class TokenType {
    ASSIGN, //=
    COMMA, // ,
    DIF, // !=
    ELSE, // else
    EOF_TOKEN, // Blank (End of File)
    EQ, // Equals
    FUNCTION, // function
    IDENTIFIER, // Letter
    IF, // if
    GT, // > (Greater than)
    GTE, // >= (Greater than or equal)
    ILLEGAL, // Not identified
    INTEGER, //Number
    LBRACE, // {
    LET, // variable
    LPAREN, // (
    LT, // < (Less than)
    LTE, // <= (Less than or equal)
    MINUS, // - 
    NEGATION, // !
    PLUS, // +
    RBRACE, // }
    RPAREN, // )
    SEMICOLON // ;
};

static const char *enumStr[] = {"ASSIGN", "COMMA", "DIF", "ELSE", "EOF_TOKEN", "EQ", "FUNCTION", 
"IDENTIFIER", "IF", "GT", "GTE", "ILLEGAL", "INTEGER", "LBRACE", "LET", "LPAREN", "LT", "LTE", 
"MINUS", "NEGATION", "PLUS", "RBRACE", "RPAREN", "SEMICOLON"};

class Token {
public:
    TokenType type;
    std::string literal;

    Token(TokenType type, const std::string &lit);

    std::string to_string() const;
};

TokenType lookup_token_type(const std::string &literal);

#endif // TOKEN_H