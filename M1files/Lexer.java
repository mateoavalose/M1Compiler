package M1files;

public class Lexer {
    private String source;
    private char character;
    private int position;
    private int readPosition;

    public Lexer(String source) {
        this.source = source;
        this.position = 0;
        this.readPosition = 0;
        readCharacter();
    }

    public Token nextToken(){
        skipWhitespace();
        Token token = new Token(TokenType.ILLEGAL, String.valueOf(character));
        if(character == '=') {
            if(peekCharacter() == '=') { // Assing or equals '=' or '=='
                token =  makeTwoCharacterToken(TokenType.EQ); // '=='
            } else {
                token = new Token(TokenType.ASSIGN, String.valueOf(character)); // '='
            }
        } else if(character == ',') { // Comma ,
            token = new Token(TokenType.COMMA, String.valueOf(character));
        } else if(character == '!') { // Different or equals '!='
            if(peekCharacter() == '=') { 
                token =  makeTwoCharacterToken(TokenType.DIF); // '!='
            } else {
                token =  new Token(TokenType.NEGATION, String.valueOf(character)); // '!'
            }
        } else if(character == '/') { // Division
            token = new Token(TokenType.DIVISION, String.valueOf(character));
        } else if(character == '\u0000') { // Blank
            token = new Token(TokenType.EOF, String.valueOf(character));
        } else if(Character.isLetter(character)) { // Letter
            String literal = readIdentifier();
            TokenType tokenType = Translator.lookupTokenType(literal);
            token = new Token(tokenType, literal);
        } else if(character == '>') { // Greater than 
            if(peekCharacter() == '=') {
                token = makeTwoCharacterToken(TokenType.GTE); // '>='
            } else {
                token = new Token(TokenType.GT, String.valueOf(character)); // '>'
            }
        } else if(Character.isDigit(character)){ // Number
            String literal = readNumber();
            //TokenType tokenType = Translator.lookupTokenType(literal);
            token = new Token(TokenType.INTEGER, literal);
        } else if(character == '{') { // Rbrace '{'
            token = new Token(TokenType.LBRACE, String.valueOf(character));
        } else if(character == '(') { // Lparen '('
            token = new Token(TokenType.LPAREN, String.valueOf(character));
        } else if(character == '<') { // Less than
            if(peekCharacter() == '=') {
                token = makeTwoCharacterToken(TokenType.LTE); // '<='
            } else {
                token = new Token(TokenType.LT, String.valueOf(character)); // '<'
            }
        } else if(character == '-') { // Minus '-'
            token = new Token(TokenType.MINUS, String.valueOf(character));
        } else if(character == '*') { // Multiplication '*'
            token = new Token(TokenType.MULTIPLICATION, String.valueOf(character));
        } else if(character == '+') { // Plus '+'
            token = new Token(TokenType.PLUS, String.valueOf(character));
        } else if(character == '}'){ // Rbrace '}'
            token = new Token(TokenType.RBRACE, String.valueOf(character));
        } else if(character == ')'){ // Rparen ')'
            token = new Token(TokenType.RPAREN, String.valueOf(character));
        } else if(character == ';') { // Semicolon ';'
            token = new Token(TokenType.SEMICOLON, String.valueOf(character));
        }
        readCharacter();
        return token;
    }

    private String readNumber(){
        int initialPosition = position;
        while(Character.isDigit(character)) {
            readCharacter();
        }
        return source.substring(initialPosition, position);
    }
    private String readIdentifier(){
        int initialPosition = position;
        while(position < source.length() && Character.isLetter(character)) {
            readCharacter();
        }
        return source.substring(initialPosition, position);
    }
    private void readCharacter(){
        if(readPosition >= source.length()) {
            character = '\u0000';
        } else {
            character = source.charAt(readPosition);
        }
        position = readPosition;
        readPosition++;
    }
    private char peekCharacter(){
        if(readPosition >= source.length()) {
            return '\u0000';
        } 
        return source.charAt(readPosition);
    }
    private Token makeTwoCharacterToken(TokenType tokenType){
        char prefix = character;
        readCharacter();
        char suffix = character;
        return new Token(tokenType, String.valueOf(prefix) + suffix);
    }
    private void skipWhitespace(){
        while(character == ' ' || character == '\t' || character == '\n' || character == '\r') {
            readCharacter();
        }
    }
}