#include <iostream>
#include <string>
#include <regex>

#include "HeaderFiles/tokens.h"
#include "HeaderFiles/lexer.h"

Lexer::Lexer(std::string &source) {
    _source = source;
    _character = '\0';
    _position = 0;
    _read_position = 0;
}

void Lexer::_read_character() {
    if (_read_position >= _source.size()) {
        _character = '\0';
    } else {
        _character = _source[_read_position];
    }
    _position = _read_position;
    _read_position++;
}

char Lexer::_peek_character() {
    if (_read_position >= _source.size()) {
        return '\0';
    }
    return _source[_read_position];
}

Token Lexer::_make_two_character_token(TokenType token_type) {
    char prefix = _character;
    _read_character();
    char suffix = _character;
    return Token(token_type, std::string(1, prefix) + suffix);
}

void Lexer::_skip_whitespace() {
    while (std::regex_match(std::string(1, _character), std::regex("^\\s$"))) {
        _read_character();
    }
}

bool Lexer::_is_letter(char character) {
    return std::regex_match(std::string(1, character), std::regex("^[a-zA-ZáéíóúÁÉÍÓÚñÑ_]$"));
}

bool Lexer::_is_number(char character) {
    return std::regex_match(std::string(1, character), std::regex("^\\d$"));
}

std::string Lexer::_read_number() {
    int initial_position = _position;
    while (_is_number(_character)) {
        _read_character();
    }
    return _source.substr(initial_position, _position - initial_position);
}

std::string Lexer::_read_identifier() {
    int initial_position = _position;
    while (_is_letter(_character)) {
        _read_character();
    }
    return _source.substr(initial_position, _position - initial_position);
}

Token Lexer::next_token() {
    _skip_whitespace();
    Token token(TokenType::ILLEGAL, std::string(1, _character)); // Default to ILLEGAL token

    if (_character == '=') { // Assign = or Equals ==
        if (_peek_character() == '=') {
            token = _make_two_character_token(TokenType::EQ);
        } else {
            token = Token(TokenType::ASSING, std::string(1, _character));
        }
    } else if (_character == '!') { // Negation ! or Dif !=
        if (_peek_character() == '=') {
            token = _make_two_character_token(TokenType::DIF);
        } else {
            token = Token(TokenType::NEGATION, std::string(1, _character));
        }
    } else if (_character == '+') { // Plus +
        token = Token(TokenType::PLUS, std::string(1, _character));
    } else if(_character == '{') { // Lbrace {
        token = Token(TokenType::LBRACE, std::string(1, _character));
    } else if(_character == '}') { // Rbrace }
        token = Token(TokenType::RBRACE, std::string(1, _character));
    } else if(_character == '(') { // Lparen (
        token = Token(TokenType::LPAREN, std::string(1, _character));
    } else if(_character == ')') { // Rparen )
        token = Token(TokenType::RPAREN, std::string(1, _character));
    } else if(_character == '-') { // Minus -
        token = Token(TokenType::MINUS, std::string(1, _character));
    } else if(_character == ',') { // Comma ,
        token = Token(TokenType::COMMA, std::string(1, _character));
    } else if(_character == ';') { // Semicolon ;
        token = Token(TokenType::SEMICOLON, std::string(1, _character));
    } else if (_is_letter(_character)) { // Letter
        std::string literal = _read_identifier();
        TokenType token_type = lookup_token_type(literal);
        return Token(token_type, literal);
    } else if (_is_number(_character)) { // Number
        std::string literal = _read_number();
        TokenType token_type = lookup_token_type(literal);
        return Token(TokenType::INTEGER, literal);
    } else if(_character == '>') { // Greater than > or equals >=
        if(_peek_character() == '=')
            token = _make_two_character_token(TokenType::GTE);
        else
            token = Token{TokenType::GT, std::string(1, _character)};
    } else if(_character == '<') // Less than < or equals <=
    {
        if(_peek_character() == '=')
            token = _make_two_character_token(TokenType::LTE);
        else
            token = Token{TokenType::LT, std::string(1, _character)};
    } else if(_character == '\0') { // End of file
        token = Token(TokenType::EOF_TOKEN, std::string(1, _character));
    }
    _read_character();
    return token;
}