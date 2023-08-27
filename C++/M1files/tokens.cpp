#include <iostream>
#include <map>
#include <string>

#include "HeaderFiles/tokens.h"

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

struct Token {
    TokenType type;
    std::string literal;

    std::string to_string() const {
        return "Type: " + std::to_string(static_cast<int>(type)) + ", Literal: " + literal;
    }
};

TokenType lookup_token_type(const std::string& literal) {
    std::map<std::string, TokenType> keywords = {
        {"function", TokenType::FUNCTION},
        {"variable", TokenType::LET},
        {"if", TokenType::IF},
        {"else", TokenType::ELSE}
    };
    auto it = keywords.find(literal);
    if (it != keywords.end()) {
        return it->second;
    }
    return TokenType::IDENTIFIER;
}