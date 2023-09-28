#include <iostream>
#include <string>
#include <regex>
#include <unordered_map>

#include <vector>
#include <memory>

#include <typeinfo>
#include <functional>

using namespace std;

//----------------------------------------------------------------------------------------------------------------------------------------
// Tokens

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

    Token(TokenType type, const std::string &lit) : type(type), literal(lit) {}

    std::string to_string() const {
        return "Type: " + std::to_string(static_cast<int>(type)) + ", Literal: " + literal;
    }
};

TokenType lookup_token_type(const std::string &literal) {
    std::unordered_map<std::string, TokenType> keywords = {
        {"function", TokenType::FUNCTION},
        {"variable", TokenType::LET},
        {"if", TokenType::IF},
        {"else", TokenType::ELSE},
        {"return", TokenType::RETURN},
        {"true", TokenType::TRUE},
        {"false", TokenType::FALSE}
    };
    auto it = keywords.find(literal);
    if (it != keywords.end()) {
        return it -> second;
    }
    return TokenType::IDENTIFIER;
}

//----------------------------------------------------------------------------------------------------------------------------------------
// Lexer

class Lexer
{
private:
    std::string _source;
    char _character;
    int _position;
    int _read_position;

    void _read_character() {
        if (_read_position >= _source.size()) {
            _character = '\0';
        } else {
            _character = _source[_read_position];
        }
        _position = _read_position;
        _read_position++;
    }

    char _peek_character() {
        if (_read_position >= _source.size()) {
            return '\0';
        }
        return _source[_read_position];
    }

    Token _make_two_character_token(TokenType token_type) {
        char prefix = _character;
        _read_character();
        char suffix = _character;
        return Token(token_type, std::string(1, prefix) + suffix);
    }

    void _skip_whitespace() {
        while (std::regex_match(std::string(1, _character), std::regex("^\\s$"))) {
            _read_character();
        }
    }

    bool _is_letter(char character) {
        return std::regex_match(std::string(1, character), std::regex("^[a-zA-ZáéíóúÁÉÍÓÚñÑ_]$"));
    }

    bool _is_number(char character) {
        return std::regex_match(std::string(1, character), std::regex("^\\d$"));
    }

    std::string _read_number() {
        int initial_position = _position;
        while (_is_number(_character)) {
            _read_character();
        }
        return _source.substr(initial_position, _position - initial_position);
    }

    std::string _read_identifier() {
        int initial_position = _position;
        while (_is_letter(_character)) {
            _read_character();
        }
        return _source.substr(initial_position, _position - initial_position);
    }

public:
    Lexer(const std::string &source) 
    : _source(source), _character('\0'), _position(0), _read_position(0) {
        _read_character();
    }

    Token next_token() {
        _skip_whitespace();
        Token token(TokenType::ILLEGAL, std::string(1, _character)); // Default to ILLEGAL token

        if (_character == '=') { // Assign = or Equals ==
            if (_peek_character() == '=') {
                token = _make_two_character_token(TokenType::EQ);
            } else {
                token = Token(TokenType::ASSING, std::string(1, _character));
            }
        } else if (_character == ',') { // Comma ,
            token = Token(TokenType::COMMA, std::string(1, _character));
        } else if (_character == '!') { // Different than != or negation !
            if (_peek_character() == '=') {
                token = _make_two_character_token(TokenType::DIF);
            } else {
                token = Token(TokenType::NEGATION, std::string(1, _character));
            }
        } else if (_character == '/') { // Division /
            token = Token(TokenType::DIVISION, std::string(1, _character));
        } else if(_character == '\0') { // Blank: End of File
            token = Token(TokenType::EOF_TOKEN, std::string(1, _character));
        } else if (_is_letter(_character)) { // Letter
            std::string literal = _read_identifier();
            TokenType token_type = lookup_token_type(literal);
            return Token(token_type, literal);
        } else if(_character == '>') { // Greater than > or equals >=
            if(_peek_character() == '=')
                token = _make_two_character_token(TokenType::GTE);
            else
                token = Token{TokenType::GT, std::string(1, _character)};
        } else if (_is_number(_character)) { // Number
            std::string literal = _read_number();
            TokenType token_type = lookup_token_type(literal);
            return Token(TokenType::INTEGER, literal);        
        } else if(_character == '{') { // Lbrace {
            token = Token(TokenType::LBRACE, std::string(1, _character));
        } else if(_character == '(') { // Lparen (
            token = Token(TokenType::LPAREN, std::string(1, _character));
        } else if(_character == '<') { // Less than < or equals <=
            if(_peek_character() == '=')
                token = _make_two_character_token(TokenType::LTE);
            else
                token = Token{TokenType::LT, std::string(1, _character)};
        } else if(_character == '-') { // Minus -
            token = Token(TokenType::MINUS, std::string(1, _character));
        } else if(_character == '*') { // Multiplication *
            token = Token(TokenType::MULTIPLICATION, std::string(1, _character));
        } else if (_character == '+') { // Plus +
            token = Token(TokenType::PLUS, std::string(1, _character));
        } else if(_character == '}') { // Rbrace }
            token = Token(TokenType::RBRACE, std::string(1, _character));
        } else if(_character == ')') { // Rparen )
            token = Token(TokenType::RPAREN, std::string(1, _character));
        } else if(_character == ';') { // Semicolon ;
            token = Token(TokenType::SEMICOLON, std::string(1, _character));
        }
        _read_character();
        return token;
    }
};

//----------------------------------------------------------------------------------------------------------------------------------------
// Abstract Syntax Tree (ast)

class ASTNode {
public:
    virtual std::string token_literal() = 0;
    virtual std::string to_string() = 0;
};

class Statement : public ASTNode {
public:
    Token token;

    Statement(Token token) : token(token) {}

    std::string token_literal() override {
        return token.literal;
    }
};

class Expression : public ASTNode {
public:
    Token token;

    Expression(Token token) : token(token) {}

    std::string token_literal() override {
        return token.literal;
    }
};

class Program : public ASTNode {
public:
    std::vector<std::shared_ptr<Statement>> statements;

    Program(std::vector<std::shared_ptr<Statement>> statements) : statements(statements) {}

    std::string token_literal() override {
        if (!statements.empty()) {
            return statements[0] -> token_literal();
        }
        return "";
    }

    std::string to_string() override {
        std::string result;
        for (const auto& statement : statements) {
            result += statement->to_string();
        }
        return result;
    }
};

class Identifier : public Expression {
public:
    std::string value;

    Identifier(Token token, std::string value) : Expression(token), value(value) {}

    std::string to_string() override {
        return value;
    }
};

class LetStatement : public Statement {
public:
    std::shared_ptr<Identifier> name;
    std::shared_ptr<Expression> value;

    LetStatement(Token token, std::shared_ptr<Identifier> name, std::shared_ptr<Expression> value)
        : Statement(token), name(name), value(value) {}

    std::string to_string() override {
        return token_literal() + " " + name->to_string() + " = " + value->to_string() + ";";
    }
};

class ReturnStatement : public Statement {
public:
    std::shared_ptr<Expression> return_value;

    ReturnStatement(Token token, std::shared_ptr<Expression> return_value)
        : Statement(token), return_value(return_value) {}

    std::string to_string() override {
        return token_literal() + " " + return_value->to_string() + ";";
    }
};

class astInteger : public Expression {
public:
    int value;

    astInteger(Token token, int value) : Expression(token), value(value) {}

    std::string to_string() override {
        return std::to_string(value);
    }
};

class astBoolean : public Expression {
public:
    bool value;

    astBoolean(Token token, bool value) : Expression(token), value(value) {}

    std::string to_string() override {
        return token_literal();
    }
};

class Block : public Statement {
public:
    std::vector<std::shared_ptr<Statement>> statements;

    Block(Token token, std::vector<std::shared_ptr<Statement>> statements) : Statement(token), statements(statements) {}

    std::string to_string() override {
        std::string result;
        for (const auto& statement : statements) {
            result += statement->to_string();
        }
        return result;
    }
};

class Function : public Expression {
public:
    std::vector<std::shared_ptr<Identifier>> parameters;
    std::shared_ptr<Block> body;

    Function(Token token, std::vector<std::shared_ptr<Identifier>> parameters, std::shared_ptr<Block> body)
        : Expression(token), parameters(parameters), body(body) {}

    std::string to_string() override {
        std::string param_list;
        for (size_t i = 0; i < parameters.size(); ++i) {
            param_list += parameters[i]->to_string();
            if (i < parameters.size() - 1) {
                param_list += ", ";
            }
        }
        return token_literal() + "(" + param_list + ") " + body->to_string();
    }
};

class If : public Expression {
public:
    std::shared_ptr<Expression> condition;
    std::shared_ptr<Block> consequence;
    std::shared_ptr<Block> alternative;

    If(Token token, std::shared_ptr<Expression> condition, std::shared_ptr<Block> consequence, std::shared_ptr<Block> alternative)
        : Expression(token), condition(condition), consequence(consequence), alternative(alternative) {}

    std::string to_string() override {
        std::string result = "if " + condition->to_string() + " " + consequence->to_string();
        if (alternative != nullptr) {
            result += " else " + alternative->to_string();
        }
        return result;
    }
};

class Infix : public Expression {
public:
    std::shared_ptr<Expression> left;
    std::string operator_;
    std::shared_ptr<Expression> right;

    Infix(Token token, std::shared_ptr<Expression> left, std::string operator_, std::shared_ptr<Expression> right)
        : Expression(token), left(left), operator_(operator_), right(right) {}

    std::string to_string() override {
        return "(" + left->to_string() + " " + operator_ + " " + right->to_string() + ")";
    }
};

class Prefix : public Expression {
public:
    std::string operator_;
    std::shared_ptr<Expression> right;

    Prefix(Token token, std::string operator_, std::shared_ptr<Expression> right)
        : Expression(token), operator_(operator_), right(right) {}

    std::string to_string() override {
        return "(" + operator_ + right->to_string() + ")";
    }
};

class Call : public Expression {
public:
    std::shared_ptr<Expression> function;
    std::vector<std::shared_ptr<Expression>> arguments;

    Call(Token token, std::shared_ptr<Expression> function, std::vector<std::shared_ptr<Expression>> arguments)
        : Expression(token), function(function), arguments(arguments) {}

    std::string to_string() override {
        std::string args;
        for (size_t i = 0; i < arguments.size(); ++i) {
            args += arguments[i]->to_string();
            if (i < arguments.size() - 1) {
                args += ", ";
            }
        }
        return function->to_string() + "(" + args + ")";
    }
};

class ExpressionStatement : public Statement {
public:
    std::shared_ptr<Expression> expression;

    ExpressionStatement(Token token, std::shared_ptr<Expression> expression)
        : Statement(token), expression(expression) {}

    std::string to_string() override {
        return expression -> to_string();
    }
};

//----------------------------------------------------------------------------------------------------------------------------------------
// Objects

enum class ObjectType {
    BOOLEAN,
    INTEGER,
    NULL_OBJ,
    FLOAT,
    STRING,
};

class Object {
public:
    virtual ObjectType type() const = 0;
    virtual std::string inspect() const = 0;
    virtual ~Object() {}
};

class objInteger : public Object {
public:
    int value;

    objInteger(int value) : value(value) {}

    ObjectType type() const override {
        return ObjectType::INTEGER;
    }

    std::string inspect() const override {
        return std::to_string(value);
    }
};

class objBoolean : public Object {
public:
    bool value;

    objBoolean(bool value) : value(value) {}

    ObjectType type() const override {
        return ObjectType::BOOLEAN;
    }

    std::string inspect() const override {
        return value ? "true" : "false";
    }
};

class Null : public Object {
public:
    ObjectType type() const override {
        return ObjectType::NULL_OBJ;
    }

    std::string inspect() const override {
        return "null";
    }
};

class Float : public Object {
public:
    float value;

    Float(float value) : value(value) {}

    ObjectType type() const override {
        return ObjectType::FLOAT;
    }

    std::string inspect() const override {
        return std::to_string(value);
    }
};

class String : public Object {
public:
    std::string value;

    String(const std::string& value) : value(value) {}

    ObjectType type() const override {
        return ObjectType::STRING;
    }

    std::string inspect() const override {
        return value;
    }
};

//----------------------------------------------------------------------------------------------------------------------------------------
// Parser

enum class Precendece {
    LOWEST = 1,
    EQUALS = 2,
    LESSGREATER = 3,
    SUM = 4,
    PRODUCT = 5,
    PREFIX = 6,
    CALL = 7
};

/*using PrefixParseFn = std::function<Expression*(Expression*)>;
using InfixParseFn = std::function<Expression*(Expression*)>;
std::unordered_map<TokenType, PrefixParseFn> PrefixParseFns;
std::unordered_map<TokenType, InfixParseFn> InfixParseFns;

using PrefixParseFn = std::function<std::shared_ptr<Expression>()>;
using InfixParseFn = std::function<std::shared_ptr<Expression>(std::shared_ptr<Expression>)>;
using PrefixParseFns = std::unordered_map<TokenType, PrefixParseFn>;
using InfixParseFns = std::unordered_map<TokenType, InfixParseFn>;*/




std::unordered_map<TokenType, Precendece> PRECEDENTES = {
    {TokenType::EQ, Precendece::EQUALS},
    {TokenType::DIF, Precendece::EQUALS},
    {TokenType::LT, Precendece::LESSGREATER},
    {TokenType::GT, Precendece::LESSGREATER},
    {TokenType::PLUS, Precendece::SUM},
    {TokenType::MINUS, Precendece::SUM},
    {TokenType::DIVISION, Precendece::PRODUCT},
    {TokenType::MULTIPLICATION, Precendece::PRODUCT},
    {TokenType::LBRACE, Precendece::CALL}
};

using PrefixParseFn = std::function<Expression*()>;
using InfixParseFn = std::function<Expression*(Expression*)>;
using PrefixParseFns = std::unordered_map<TokenType, PrefixParseFn>;
using InfixParseFns = std::unordered_map<TokenType, InfixParseFn>;

class Parser {
public:
    Lexer lexer;
    Token* current_token;
    Token* peek_token;
    std::vector<std::string> errors;

    PrefixParseFns _prefix_parse_fns;
    InfixParseFns _infix_parse_fns;

    Parser(Lexer lexer) : lexer(lexer) {
        current_token = nullptr;
        peek_token = nullptr;
        errors = {};

        /*PrefixParseFns prefix_parse_fns = _register_prefix_fns();
        InfixParseFns infix_parse_fns = _register_infix_fns();

        PrefixParseFns parse_prefix_fns = _register_prefix_fns();
        InfixParseFns parse_infix_fns = _register_infix_fns();*/

        _prefix_parse_fns = _register_prefix_fns();
        _infix_parse_fns = _register_infix_fns();

        advance_tokens();
        advance_tokens();
    }

    Program parse_program() {
        Program program({});
        while (current_token->type != TokenType::EOF_TOKEN) {
            auto statement = parse_statement();
            if (statement) {
                program.statements.push_back(std::make_shared<Statement>(*statement));
            }
            advance_tokens();
        }
        return program;
    }

private:
    void advance_tokens() {        
        current_token = peek_token;
        peek_token = new Token(lexer.next_token());
    }

    Precendece current_precedence() {
        if (!current_token) {
            return Precendece::LOWEST;
        }

        auto it = PRECEDENTES.find(current_token->type);
        if (it != PRECEDENTES.end()) {
            return it->second;
        }

        return Precendece::LOWEST;
    }

    bool expected_token(TokenType token_type) {
        if (peek_token && peek_token->type == token_type) {
            advance_tokens();
            return true;
        }
        expected_token_error(token_type);
        return false;
    }

    void expected_token_error(TokenType token_type) {
        if (peek_token) {
            errors.push_back(std::string("A ") + enumStr[static_cast<int>(token_type)] + " token was expected, but " + enumStr[static_cast<int>(peek_token->type)] + " was found");
        } else {
            errors.push_back(std::string("A ") + enumStr[static_cast<int>(token_type)] + " token was expected, but the end of input was found");
        }
    }

    Block parse_block() {
        Token token_from_pointer = *current_token;
        Block block_statement(token_from_pointer, {});
        advance_tokens();
        while (current_token->type != TokenType::RBRACE && current_token->type != TokenType::EOF_TOKEN) {
            auto statement = parse_statement();
            if (statement) {
                block_statement.statements.push_back(std::make_shared<Statement>(*statement));
            }
            advance_tokens();
        }
        return block_statement;
    }

    astBoolean parse_boolean() {
        bool isTrue = current_token->type == TokenType::TRUE;
        return astBoolean(*current_token, isTrue);
    }

    Call parse_call(Expression* function) {
        std::shared_ptr<Expression> function_ptr(function);
        std::vector<std::shared_ptr<Expression>> arguments = parse_call_arguments();
        Call call(*current_token, function_ptr, arguments);
        return call;
    }

    std::vector<std::shared_ptr<Expression>> parse_call_arguments() {
        std::vector<std::shared_ptr<Expression>> arguments;
        advance_tokens();
        if (peek_token && peek_token->type == TokenType::RPAREN) {
            advance_tokens();
            return arguments;
        }
        arguments.push_back(std::make_shared<Expression>(parse_expression(Precendece::LOWEST)));
        while (peek_token && peek_token->type == TokenType::COMMA) {
            advance_tokens();
            advance_tokens();
            arguments.push_back(std::make_shared<Expression>(parse_expression(Precendece::LOWEST)));
        }
        if (!expected_token(TokenType::RPAREN)) {
            return {};
        }
        return arguments;
    }

    std::shared_ptr<Expression> parse_expression(Precendece precender) {
        if (!current_token) {
            return nullptr;
        }
        auto prefix_parse_fn = _prefix_parse_fns[current_token->type];
        if(!prefix_parse_fn) {
            expected_token_error(current_token->type);
            return nullptr;
        }
        std::shared_ptr<Expression> left_expression(prefix_parse_fn());
        while (peek_token && peek_token->type != TokenType::SEMICOLON && precender < peek_precedence()) {
            auto infix_parse_fn = _infix_parse_fns[peek_token->type];
            advance_tokens();
            left_expression = std::shared_ptr<Expression>(infix_parse_fn(left_expression.get()));
        }
        return left_expression;
    }

    ExpressionStatement* parse_expression_statement() {
        auto expression_statement = new ExpressionStatement(*current_token, nullptr);
        expression_statement->expression = parse_expression(Precendece::LOWEST);
        if (peek_token && peek_token->type == TokenType::SEMICOLON) {
            advance_tokens();
        }
        return expression_statement;
    }

    std::shared_ptr<Expression> parse_grouped_expression() {
        advance_tokens();
        auto expression = parse_expression(Precendece::LOWEST);
        if (!expected_token(TokenType::RPAREN)) {
            return nullptr;
        }
        return expression;
    }

    Function* parse_function() {
        auto function = new Function(*current_token, {}, nullptr);
        if (!expected_token(TokenType::LPAREN)) {
            return nullptr;
        }
        function->parameters = parse_function_parameters();
        if (!expected_token(TokenType::LBRACE)) {
            return nullptr;
        }
        function->body = std::make_shared<Block>(parse_block());
        return function;
    }

    std::vector<std::shared_ptr<Identifier>> parse_function_parameters() {
        std::vector<std::shared_ptr<Identifier>> params;
        advance_tokens();
        if (peek_token && peek_token->type == TokenType::RPAREN) {
            advance_tokens();
            return params;
        }
        auto identifier = parse_identifier();
        params.push_back(std::make_shared<Identifier>(identifier));
        while (peek_token && peek_token->type == TokenType::COMMA) {
            advance_tokens();
            advance_tokens();
            identifier = parse_identifier();
            params.push_back(std::make_shared<Identifier>(identifier));
        }
        if (!expected_token(TokenType::RPAREN)) {
            return {};
        }
        return params;
    }

    Identifier* parse_identifier() {
        return new Identifier(*current_token, current_token->literal);
    }

    If* parse_if() {
        auto if_expression = new If(*current_token, nullptr, nullptr, nullptr);
        if (!expected_token(TokenType::LPAREN)) {
            return nullptr;
        }
        advance_tokens();
        if_expression->condition = parse_expression(Precendece::LOWEST);
        if (!expected_token(TokenType::RPAREN)) {
            return nullptr;
        }
        if (!expected_token(TokenType::LBRACE)) {
            return nullptr;
        }
        if_expression->consequence = std::make_shared<Block>(parse_block());
        if (peek_token && peek_token->type == TokenType::ELSE) {
            advance_tokens();
            if (!expected_token(TokenType::LBRACE)) {
                return nullptr;
            }
            if_expression->alternative = std::make_shared<Block>(parse_block());
        }
        return if_expression;
    }

    Infix* parse_infix_expression(Expression* left) {
        auto infix = new Infix(*current_token, std::make_shared<Expression>(*left), current_token->literal, nullptr);
        auto precedence = current_precedence();//nction for call to 'Infix::Infix(Token&, std::__cxx11::string&, Expression&)'
        advance_tokens();
        infix->right = parse_expression(precedence);
        return infix;
    }

    astInteger* parse_integer() {
        auto integer = new astInteger(*current_token, 0);
        try {
            integer->value = std::stoi(current_token->literal);
        } catch (const std::invalid_argument& e) {
            errors.push_back("Could not parse " + current_token->literal + " as an integer");
            return nullptr;
        }
        return integer;
    }
    
    LetStatement* parse_let_statement() {
        auto let_statement = new LetStatement(*current_token, nullptr, nullptr);
        if (!expected_token(TokenType::IDENTIFIER)) {
            return nullptr;
        }
        let_statement->name = std::make_shared<Identifier>(parse_identifier());
        if (!expected_token(TokenType::ASSING)) {
            return nullptr;
        }
        advance_tokens();
        let_statement->value = parse_expression(Precendece::LOWEST);
        if (peek_token && peek_token->type == TokenType::SEMICOLON) {
            advance_tokens();
        }
        return let_statement;
    }
    
    Prefix* parse_prefix_expression() {
        auto prefix = new Prefix(*current_token, current_token->literal, nullptr);
        advance_tokens();
        prefix->right = parse_expression(Precendece::PREFIX);
        return prefix;
    }
    
    ReturnStatement* parse_return_statement() {
        auto return_statement = new ReturnStatement(*current_token, nullptr);
        advance_tokens();
        return_statement->return_value = parse_expression(Precendece::LOWEST);
        if (peek_token && peek_token->type == TokenType::SEMICOLON) {
            advance_tokens();
        }
        return return_statement;
    }

    std::shared_ptr<Statement> parse_statement() {
        if (!current_token) {
            return nullptr;
        }
        if (current_token->type == TokenType::LET) {
            return std::make_shared<Statement>(parse_let_statement());
        } else if (current_token->type == TokenType::RETURN) {
            return std::make_shared<Statement>(parse_return_statement());
        } else {
            return std::make_shared<Statement>(parse_expression_statement());
        }
    }

    Precendece peek_precedence() {
        if (!peek_token) {
            return Precendece::LOWEST;
        }

        auto it = PRECEDENTES.find(peek_token->type);
        if (it != PRECEDENTES.end()) {
            return it->second;
        }

        return Precendece::LOWEST;
    }

    InfixParseFns _register_infix_fns() {
        InfixParseFns register_infix_fns;
        register_infix_fns[TokenType::PLUS] = [this](Expression* left) -> Expression* {
            return parse_infix_expression(left);
        };
        register_infix_fns[TokenType::MINUS] = [this](Expression* left) -> Expression* {
            return parse_infix_expression(left);
        };
        register_infix_fns[TokenType::DIVISION] = [this](Expression* left) -> Expression* {
            return parse_infix_expression(left);
        };
        register_infix_fns[TokenType::MULTIPLICATION] = [this](Expression* left) -> Expression* {
            return parse_infix_expression(left);
        };
        register_infix_fns[TokenType::EQ] = [this](Expression* left) -> Expression* {
            return parse_infix_expression(left);
        };
        register_infix_fns[TokenType::DIF] = [this](Expression* left) -> Expression* {
            return parse_infix_expression(left);
        };
        register_infix_fns[TokenType::LT] = [this](Expression* left) -> Expression* {
            return parse_infix_expression(left);
        };
        register_infix_fns[TokenType::GT] = [this](Expression* left) -> Expression* {
            return parse_infix_expression(left);
        };
        register_infix_fns[TokenType::LPAREN] = [this](Expression* left) -> Expression* {
            Call callExpression(left->token, nullptr, {});
            Expression* expressionPtr = &callExpression;
            return expressionPtr;
        };
        return register_infix_fns;
    }

    PrefixParseFns _register_prefix_fns() {
        PrefixParseFns register_prefix_fns;
        register_prefix_fns[TokenType::FALSE] = [this]() -> Expression* {
            astBoolean boolean = parse_boolean();
            return new astBoolean(boolean.token, boolean.value);
        };
        register_prefix_fns[TokenType::FUNCTION] = [this]() -> Expression* {
            return parse_function();
        };
        register_prefix_fns[TokenType::IDENTIFIER] = [this]() -> Expression* {
            Identifier* identifier_ptr = parse_identifier();
            return std::static_pointer_cast<Expression>(std::make_shared<Identifier>(*identifier_ptr)).get();
        };
        register_prefix_fns[TokenType::IF] = std::function<Expression*()>([this]() -> Expression* {
            If* ifExpression = parse_if();
            return static_cast<Expression*>(std::make_shared<If>(ifExpression->token, ifExpression->condition, ifExpression->consequence, ifExpression->alternative).get());
        });
        register_prefix_fns[TokenType::INTEGER] = [this]() -> Expression* {
            astInteger* integer = parse_integer();
            return new astInteger(integer->token, integer->value);
        };
        register_prefix_fns[TokenType::LPAREN] = [this]() -> Expression* {
            std::shared_ptr<Expression> expression = parse_grouped_expression();
            return expression.get();
        };
        register_prefix_fns[TokenType::MINUS] = [this]() -> Expression* {
            Prefix* prefix = parse_prefix_expression();
            return prefix;
        };
        register_prefix_fns[TokenType::NEGATION] = [this]() -> Expression* {
            Prefix* prefix = parse_prefix_expression();
            return prefix;
        };
        register_prefix_fns[TokenType::TRUE] = [this]() -> Expression* {
            astBoolean boolean = parse_boolean();
            return new astBoolean(boolean.token, boolean.value);
        };
        return register_prefix_fns;
    }
};

//----------------------------------------------------------------------------------------------------------------------------------------
// Evaluator

Object* TRUE = new objBoolean(true);
Object* FALSE = new objBoolean(false);
Object* NULL_OBJ = new Null();

Object* _evaluate_bang_operator_expression(Object* right) {
    if (right == TRUE) {
        return FALSE;
    } else if (right == FALSE) {
        return TRUE;
    } else if (right == NULL_OBJ) {
        return TRUE;
    } else {
        return FALSE;
    }
}

objBoolean _to_boolean_object(bool value) {
    return value ? TRUE : FALSE;
}

Object* _evaluate_minus_operator_expression(Object* right) {
    if (right->type() != ObjectType::INTEGER) {
        return NULL_OBJ;
    }
    objInteger* intRight = dynamic_cast<objInteger*>(right);
    return new objInteger(-intRight->value);
}

Object* _evaluate_prefix_expression(const std::string& oper, Object* right) {
    if (oper == "!") {
        return _evaluate_bang_operator_expression(right);
    } else if (oper == "-") {
        return _evaluate_minus_operator_expression(right);
    } else {
        return NULL_OBJ;
    }
}

Object* _evaluate_integer_infix_expression(const std::string& oper, Object* left, Object* right) {
    int leftValue = dynamic_cast<objInteger*>(left)->value;
    int rightValue = dynamic_cast<objInteger*>(right)->value;

    if (oper == "+") {
        return new objInteger(leftValue + rightValue);
    } else if (oper == "-") {
        return new objInteger(leftValue - rightValue);
    } else if (oper == "*") {
        return new objInteger(leftValue * rightValue);
    } else if (oper == "/") {
        return new objInteger(leftValue / rightValue);
    } else if (oper == "<") {
        return new objBoolean(_to_boolean_object((leftValue < rightValue)));
    } else if (oper == "<=") {
        return new objBoolean(_to_boolean_object((leftValue <= rightValue)));
    } else if (oper == ">") {
        return new objBoolean(_to_boolean_object((leftValue > rightValue)));
    } else if (oper == ">=") {
        return new objBoolean(_to_boolean_object((leftValue >= rightValue)));
    } else if (oper == "==") {
        return new objBoolean(_to_boolean_object((leftValue == rightValue)));
    } else if (oper == "!=") {
        return new objBoolean(_to_boolean_object((leftValue != rightValue)));
    }
    return NULL_OBJ;
}

Object* _evaluate_infix_expression(const std::string& oper, Object* left, Object* right) {
    if (left->type() == ObjectType::INTEGER && right->type() == ObjectType::INTEGER) {
        return _evaluate_integer_infix_expression(oper, left, right);
    } else if (oper == "==") {
        return new objBoolean(_to_boolean_object(left == right));
    } else if (oper == "!=") {
        return new objBoolean(_to_boolean_object(left != right));
    }
    return NULL_OBJ;
}

bool _is_truthy(Object* obj) {
    if (obj == NULL_OBJ) {
        return false;
    } else if (obj == TRUE) {
        return true;
    } else if (obj == FALSE) {
        return false;
    }
    return true;
}

//----------------------------

Object* _evaluate_statements(const std::vector<std::shared_ptr<Statement>>& statements) {
    Object* result = NULL_OBJ;
    for (const auto& statement : statements) {
        result = evaluate(statement.get());
    }
    return result;
}
Object* _evaluate_if_expression(std::shared_ptr<If> ifExpression) {
    Object* conditionResult = evaluate(ifExpression->condition);
    if (_is_truthy(conditionResult)) {
        return evaluate(ifExpression->consequence);
    } else if (ifExpression->alternative != nullptr) {
        return evaluate(ifExpression->alternative);
    }
    return NULL_OBJ;
}

Object* evaluate(ASTNode* node) {
    if (dynamic_cast<Program*>(node)) {
        Program* program = static_cast<Program*>(node);
        return _evaluate_statements(program->statements);
    } else if (dynamic_cast<ExpressionStatement*>(node)) {
        ExpressionStatement* expressionStatement = static_cast<ExpressionStatement*>(node);
        Expression* expression = expressionStatement->expression.get();
        if (expression != nullptr) {
            return evaluate(expression);
        }
    } else if (dynamic_cast<astInteger*>(node)) {
        return new objInteger(static_cast<astInteger*>(node)->value);
    } else if (dynamic_cast<astBoolean*>(node)) {
        return new objBoolean(_to_boolean_object(static_cast<astBoolean*>(node)->value));
    } else if (dynamic_cast<Prefix*>(node)) {
        Prefix* prefixNode = static_cast<Prefix*>(node);
        Expression* right = prefixNode->right.get();
        if (right != nullptr) {
            Object* result = _evaluate_prefix_expression(prefixNode->operator_, evaluate(right));
            return result;
        }
    } else if (dynamic_cast<Infix*>(node)) {
        Infix* infixNode = static_cast<Infix*>(node);
        Expression* left = infixNode->left.get();
        Expression* right = infixNode->right.get();
        if (left != nullptr && right != nullptr) {
            Object* result = _evaluate_infix_expression(infixNode->operator_, evaluate(left), evaluate(right));
            return result;
        }
    } else if (dynamic_cast<Block*>(node)) {
        Block* blockNode = static_cast<Block*>(node);
        return _evaluate_statements(blockNode->statements);
    } else if (dynamic_cast<If*>(node)) {
        If* ifNode = static_cast<If*>(node);
        return _evaluate_if_expression(std::make_shared<If>(*ifNode));
    }
    return NULL_OBJ;
}
Object* evaluate(std::shared_ptr<ASTNode> node) {
    return evaluate(node.get()); // Call the original evaluate function with the raw pointer
}

//----------------------------------------------------------------------------------------------------------------------------------------
// Repl

Token EndOfFile_TOKEN{TokenType::EOF_TOKEN, ""};
void start_repl() {
    while (true) {
        std::string source;
        std::cout << ">> ";
        std::getline(std::cin, source);
        if (source == "close()") {
            break;
        }

        Lexer lexer(source);
        Parser parser(lexer);
        Program program = parser.parse_program();
        Object* evaluated = evaluate(program);
        if (evaluated != nullptr) {
            std::cout << evaluated->inspect() << std::endl;
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------
// Main

int main() {
    std::cout << "Bienvenido al Lenguaje de Programacion M1 (C++)" << std::endl;
    start_repl();
    return 0;
};