package M1files;

import java.util.HashMap;
import java.util.Map;
 
enum TokenType {
    ASSIGN, // '='
    COMMA, // ','
    DIF, // '!='
    DIVISION, // '/'
    ELSE, // 'else'
    EOF, // Blank
    EQ, // '=='
    FALSE, // 'false'
    FUNCTION, // 'function'
    IDENTIFIER, // Letter
    IF, // 'if'
    GT, // '>'
    GTE, // '>='
    ILLEGAL, // Not identified
    INTEGER, // Number
    LBRACE, // '{'
    LET, // 'variable'
    LPAREN, // '('
    LT, // '<'
    LTE, // '<='
    MINUS, // '-'
    MULTIPLICATION, // '*'
    NEGATION, // '!'
    PLUS, // '+'
    RBRACE, // '}'
    RETURN, // 'return'
    RPAREN, // ')'
    SEMICOLON, // ';'
    TRUE // 'true'
}
 
public class Token {
    private TokenType tokenType;
    private String literal;
    
    public Token(TokenType tokenType, String literal) {
        this.tokenType = tokenType;
        this.literal = literal;
    }
    
    public TokenType getTokenType() {
        return tokenType;
    }
    
    public String getLiteral() {
        return literal;
    }
    
    @Override
    public String toString() {
        return "Type: " + tokenType + ", Literal: " + literal;
    }
}

class Translator {
    public static TokenType lookupTokenType(String literal) {
        Map<String, TokenType> keywords = new HashMap<>();
        keywords.put("function", TokenType.FUNCTION);
        keywords.put("variable", TokenType.LET);
        keywords.put("if", TokenType.IF);
        keywords.put("else", TokenType.ELSE);
        keywords.put("return", TokenType.RETURN);
        keywords.put("true", TokenType.TRUE);
        keywords.put("false", TokenType.FALSE);
        return keywords.getOrDefault(literal, TokenType.IDENTIFIER);
    }
}