#ifndef LEXER_H
#define LEXER_H

#include <string>
#include "Tokens.h"

class Lexer {
//private:
public:
    bool _is_letter(char character);
    bool _is_number(char character);
    std::string _read_number();
    std::string _read_identifier();
    void _read_character();
    char _peek_character();
    Token _make_two_character_token(TokenType token_type);
    void _skip_whitespace();

//public:
    Lexer(const std::string &source);
    Token next_token();
};

#endif // LEXER_H