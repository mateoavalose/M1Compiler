package M1files;

public class Lexer {
    private String source;
    private String character;
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
        Token token = new Token(TokenType.ILLEGAL, character);
        if(character.equals("=")) {
            if(peekCharacter().equals("=")) { // Assing or equals '=' or '=='
                token =  makeTwoCharacterToken(TokenType.EQ); // '=='
            } else {
                token = new Token(TokenType.ASSIGN, character); // '='
            }
        } else if(character.equals(",")) { // Comma ,
            token = new Token(TokenType.COMMA, character);
        } else if(character.equals("!")) { // Different or equals '!='
            if(peekCharacter().equals("=")) { 
                token =  makeTwoCharacterToken(TokenType.DIF); // '!='
            } else {
                token =  new Token(TokenType.NEGATION, character); // '!'
            }
        } else if(character.equals("/")) { // Division
            token = new Token(TokenType.DIVISION, character);
        } else if(character.equals("")) { // Blank
            token = new Token(TokenType.EOF, character);
        } else if(Character.isLetter(character.charAt(0))) { // Letter
            token = new Token(Translator.lookupTokenType(readIdentifier()), readIdentifier());
        } else if(character.equals(">")) { // Greater than 
            if(peekCharacter().equals("=")) {
                token = makeTwoCharacterToken(TokenType.GTE); // '>='
            } else {
                token = new Token(TokenType.GT, character); // '>'
            }
        } else if(Character.isDigit(character.charAt(0))){ // Number
            token = new Token(Translator.lookupTokenType(readNumber()), readNumber());
        } else if(character.equals("{")) { // Rbrace '{'
            token = new Token(TokenType.LBRACE, character);
        } else if(character.equals("(")) { // Lparen '('
            token = new Token(TokenType.LPAREN, character);
        } else if(character.equals("<")) { // Less than
            if(peekCharacter().equals("=")) {
                token = makeTwoCharacterToken(TokenType.LTE); // '<='
            } else {
                token = new Token(TokenType.LT, character); // '<'
            }
        } else if(character.equals("-")) { // Minus '-'
            token = new Token(TokenType.MINUS, character);
        } else if(character.equals("*")) { // Multiplication '*'
            token = new Token(TokenType.MULTIPLICATION, character);
        } else if(character.equals("+")) { // Plus '+'
            token = new Token(TokenType.PLUS, character);
        } else if(character.equals("}")){ // Rbrace '}'
            token = new Token(TokenType.RBRACE, character);
        } else if(character.equals(")")){ // Rparen ')'
            token = new Token(TokenType.RPAREN, character);
        } else if(character.equals(";")) { // Semicolon ';'
            token = new Token(TokenType.SEMICOLON, character);
        }
        readCharacter();
        return token;
    }

    private String readNumber(){
        int initialPosition = position;
        while(/*position < source.length() &&*/ Character.isDigit(character.charAt(position))) {
            readCharacter();
        }
        return source.substring(initialPosition, position);
    }
    private String readIdentifier(){
        int startPosition = position;
        while(position < source.length() && Character.isLetter(character.charAt(position))) {
            readCharacter();
        }
        return source.substring(startPosition, position);
    }
    private void readCharacter(){
        /*if(readPosition >= source.length()) {
            character = "";
        } else {
            character = source.substring(readPosition, readPosition + 1);
        }*/
        if (readPosition < source.length()) {
            character = source.substring(readPosition, readPosition + 1);
            position = readPosition;
            readPosition++;
        } else {
            character = "";
        }
    }
    private String peekCharacter(){
        if(readPosition >= source.length()) {
            return "";
        } 
        return source.substring(readPosition, readPosition + 1);
    }
    private Token makeTwoCharacterToken(TokenType tokenType){
        String prefix = character;
        readCharacter();
        String suffix = character;
        return new Token(tokenType, prefix + suffix);
    }
    private void skipWhitespace(){
        while(character.equals(" ") || character.equals("\t") || character.equals("\n") || character.equals("\r")) {
            readCharacter();
        }
    }
}