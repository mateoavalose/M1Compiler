#include <iostream>
#include <regex>
#include "HeaderFiles/tokens.h"
#include <string>
#include <cctype>

#include "HeaderFiles/lexer.h"

class Lexer
{
private:
    std::string _source;
    std::string _character;
    int _position;
    int _read_position;

public:
    Lexer(const std::string &source): _source(source), _position(0), _read_position(0)
    {
        _read_character();
    }

    Token next_token()
    {
        Token token(TokenType::ILLEGAL, _character);
        _skip_whitespace();
        if (std::regex_match(_character, std::regex("^=$"))) // Assing or equals =
        {
            if (_peek_character() == '=')
                Token token = _make_two_character_token(TokenType::EQ); // Equals ==
            else
                Token token(TokenType::ASSIGN, _character);
        }
        else if (std::regex_match(_character, std::regex("^!$"))) // Different than !
        {
            if (_peek_character() == '=')
                Token token = _make_two_character_token(TokenType::DIF);
            else
                Token token(TokenType::NEGATION, _character);
        }
        else if (std::regex_match(_character, std::regex("^\\+$"))) // Plus +
            Token token(TokenType::PLUS, _character);
        else if (std::regex_match(_character, std::regex("^$"))) // Blank
            Token token(TokenType::EOF_TOKEN, _character);
        else if(std::regex_match(_character, std::regex("^{$"))) // Lbrace {
            Token token(TokenType::LBRACE, _character);
        else if(std::regex_match(_character, std::regex("^}$"))) // Rbrace }
            Token token(TokenType::RBRACE, _character);
        else if(std::regex_match(_character, std::regex("^\\($"))) // Lparen (
            Token token(TokenType::LPAREN, _character);
        else if(std::regex_match(_character, std::regex("^\\)$"))) // Rparen )
            Token token(TokenType::RPAREN, _character);
        else if(std::regex_match(_character, std::regex("^-$"))) // Minus -
            Token token(TokenType::MINUS, _character);
        else if(std::regex_match(_character, std::regex("^,$"))) // Comma ,
            Token token(TokenType::COMMA, _character);
        else if(std::regex_match(_character, std::regex("^;$"))) // Semicolon ;
            Token token(TokenType::SEMICOLON, _character);
        else if(_is_letter(_character[0])) // Letter
        {
            std::string literal = _read_identifier();
            TokenType token_type = lookup_token_type(literal);
            return Token{token_type, literal};
        }
        else if(_is_number(_character[0])) // Number
        {
            std::string literal = _read_identifier();
            TokenType token_type = lookup_token_type(literal);
            return Token{token_type, literal};
        }
        else if(std::regex_match(_character, std::regex("^>$"))) // Greater than > or equals >=
        {
            if(_peek_character() == '=')
                Token token = _make_two_character_token(TokenType::GTE);
            else
                Token token(TokenType::GT, _character);
        } 
        else if(std::regex_match(_character, std::regex("^<$"))) // Less than < or equals <=
        {
            if(_peek_character() == '=')
                Token token = _make_two_character_token(TokenType::LTE);
            else
                Token token(TokenType::LT, _character);
        } 
        /*else // Not identified, ILLEGAL
        {
            std::cout << _character << std::endl;
            Token token(TokenType::ILLEGAL, _character);
        }*/
        _read_character();
        return token;
    }

    //-----------------------------------------------------------------------------------
    bool Lexer::_is_letter(char character)
    {
        return std::regex_match(std::string(1, character), std::regex("^[a-záéíóúA-ZÁÉÍÓÚñÑ_]$"));
    }

    bool Lexer::_is_number(char character)
    {
        return std::regex_match(std::string(1, character), std::regex("^\\d$"));
    }

    std::string Lexer::_read_number()
    {
        int initial_position = _position;
        while (_position < _source.length() && _is_number(_source[_position]))
        {
            _read_character();
        }
        return _source.substr(initial_position, _position - initial_position);
    }

    std::string Lexer::_read_identifier()
    {
        int initial_position = _position;
        while (_position < _source.length() && _is_letter(_source[_position]))
        {
            _read_character();
        }
        return _source.substr(initial_position, _position - initial_position);
    }

    void Lexer::_read_character()
    {
        if (_read_position >= _source.length())
        {
            _character = "";
        }
        else
        {
            _character = _source[_read_position];
        }
        _position = _read_position;
        _read_position += 1;
    }

    char Lexer::_peek_character()
    {
        if (_read_position >= _source.length())
        {
            return '\0'; // Null character to indicate end of string
        }
        return _source[_read_position];
    }

    Token Lexer::_make_two_character_token(TokenType token_type)
    {
        std::string prefix = std::string(1, _character[0]);
        _read_character();
        std::string suffix = std::string(1, _character[0]);
        return Token{token_type, prefix + suffix};
    }

    void Lexer::_skip_whitespace()
    {
        while (std::regex_match(_character, std::regex("^\\s$")))
        {
            _read_character();
        }
    }
};