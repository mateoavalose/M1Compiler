#ifndef TOKEN_H
#define TOKEN_H

#include <string>

enum class TokenType {
    ASSING, // =
    COMMA, // ,
    DIF, // Different
    DIVISION, // /
    ELSE, // else
    EOF_TOKEN, // Blank
    EQ, // Equals
    FALSE, // false
    FUNCTION, // function
    IDENTIFIER, // Letter
    IF, // if
    GT, // Greater than
    GTE, // Greater than or equal
    ILLEGAL, // Not identified
    INTEGER, // Number
    LBRACE, // {
    LET, // variable
    LPAREN, // (
    LT, //Less than
    LTE, // Less than or equal
    MINUS, // -
    MULTIPLICATION, // *
    NEGATION, // !
    PLUS, // +
    RBRACE, // }
    RETURN, // return
    RPAREN, // )
    SEMICOLON, // ;
    TRUE // true
};

static const char *enumStr[] = {"ASSIGN", "COMMA", "DIF", "DIVISION", "ELSE", "EOF_TOKEN", "EQ", "FALSE", "FUNCTION", 
"IDENTIFIER", "IF", "GT", "GTE", "ILLEGAL", "INTEGER", "LBRACE", "LET", "LPAREN", "LT", "LTE", 
"MINUS", "MULTIPLICATION", "NEGATION", "PLUS", "RBRACE", "RETURN", "RPAREN", "SEMICOLON", "TRUE"};

class Token {
public:
    TokenType type;
    std::string literal;

    Token(TokenType type, const std::string &lit);

    std::string to_string() const;
};

TokenType lookup_token_type(const std::string &literal);

#endif // TOKEN_H