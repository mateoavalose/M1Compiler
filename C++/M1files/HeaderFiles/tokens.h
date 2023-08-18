#ifndef TOKENS_H
#define TOKENS_H

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

class Token {
public:
    TokenType type;
    std::string literal;

    Token(TokenType type, const std::string &literal);
};

TokenType lookup_token_type(const std::string &literal);

#endif // TOKENS_H
