#include <iostream>
#include <map>
#include <unordered_map>

#include "HeaderFiles/tokens.h"

// Token Class

Token::Token(TokenType type, const std::string &lit) : type(type), literal(lit) {}

std::string Token::to_string() const {
    return "Type: " + std::to_string(static_cast<int>(type)) + ", Literal: " + literal;
}

TokenType lookup_token_type(const std::string &literal) {
    std::unordered_map<std::string, TokenType> keywords = {
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