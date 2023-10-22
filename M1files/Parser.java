package M1files;

import java.util.List;
//import java.util.Optional;
import java.util.Map;
import java.util.ArrayList;
import java.util.Collections;
import java.util.EnumMap;

// Import classes from M1files package
/*import M1files.Token;
import M1files.TokenType;
import M1files.Lexer;
import M1files.Block;
import M1files.Boolean;
import M1files.Call;
import M1files.Expression;
import M1files.ExpressionStatement;
import M1files.Function;
import M1files.Identifier;
import M1files.If;
import M1files.Infix;
import M1files.Integer;
import M1files.LetStatement;
import M1files.Prefix;
import M1files.Program;
import M1files.ReturnStatement;
import M1files.Statement;*/

public class Parser {
    private Lexer lexer;
    private Token currentToken;
    private Token peekToken;
    private List<String> errors;
    
    enum Precedence {
        LOWEST,
        EQUALS, 
        LESSGREATER, 
        SUM, 
        PRODUCT, 
        PREFIX, 
        CALL
    }

    interface PrefixParseFn extends java.util.function.Function<Void, Expression> {}
    interface InfixParseFn extends java.util.function.Function<Expression, Expression> {}
    Map<TokenType, PrefixParseFn> prefixParseFns = new EnumMap<>(TokenType.class);
    Map<TokenType, InfixParseFn> infixParseFns = new EnumMap<>(TokenType.class);
    Map<TokenType, Precedence> precedences = new EnumMap<>(TokenType.class);
    
    public Parser(Lexer lexer){
        precedences.put(TokenType.EQ, Precedence.EQUALS);
        precedences.put(TokenType.DIF, Precedence.EQUALS);
        precedences.put(TokenType.LT, Precedence.LESSGREATER);
        precedences.put(TokenType.GT, Precedence.LESSGREATER);
        precedences.put(TokenType.PLUS, Precedence.SUM);
        precedences.put(TokenType.MINUS, Precedence.SUM);
        precedences.put(TokenType.DIVISION, Precedence.PRODUCT);
        precedences.put(TokenType.MULTIPLICATION, Precedence.PRODUCT);
        precedences.put(TokenType.LBRACE, Precedence.CALL);

        this.lexer = lexer;
        this.currentToken = null;
        this.peekToken = null;
        this.errors = new ArrayList<>();

        this.prefixParseFns = registerPrefixParseFns();
        this.infixParseFns = registerInfixParseFns();

        advanceTokens();
        advanceTokens();
    }
    
    public Program parseProgram() {
        Program program = new Program(new ArrayList<>());
        assert currentToken != null;
        while (currentToken.getTokenType() != TokenType.EOF) {
            Statement statement = parseStatement();
            if (statement != null)
                program.addStatement(statement);
            advanceTokens();
        }
        return program;
    }
    
    private void advanceTokens() {
        currentToken = peekToken;
        peekToken = lexer.nextToken();
    }

    private Precedence currentPrecedence() {
        assert currentToken != null;
        try {
            return precedences.get(currentToken.getTokenType());
        } catch (NullPointerException e) {
            return Precedence.LOWEST;
        }
    }
    
    private boolean expectedToken(TokenType tokenType) {
        assert peekToken != null;
        if (peekToken.getTokenType() == tokenType) {
            advanceTokens();
            return true;
        }
        expectedTokenError(tokenType);
        return false;
    }

    private void expectedTokenError(TokenType tokenType) {
        assert peekToken != null;
        String message = "A " + tokenType + " token was expected, but " + peekToken.getTokenType() + " was found";
        errors.add(message);
    }
    
    private Block parseBlock() {
        assert currentToken != null;
        Block blockStatement = new Block(currentToken, new ArrayList<>());
        advanceTokens();
        while (currentToken.getTokenType() != TokenType.RBRACE && currentToken.getTokenType() != TokenType.EOF) {
            Statement statement = parseStatement();
            if (statement != null) {
                blockStatement.addStatement(statement);
            }
            advanceTokens();
        }
        return blockStatement;
    }

    private Boolean parseBoolean() {
        assert currentToken != null;
        return new Boolean(currentToken, currentToken.getTokenType() == TokenType.TRUE);
    }
    
    private Call parseCall(Expression function) {
        assert currentToken != null;
        Call call = new Call(currentToken, function, new ArrayList<>());
        call.setArguments(parseCallArguments());
        return call;
    }

    private List<Expression> parseCallArguments() {
        List<Expression> arguments = new ArrayList<>();
        assert peekToken != null;
        if (peekToken.getTokenType() == TokenType.RPAREN) {
            advanceTokens();
            return arguments;
        }
        advanceTokens();
        Expression expression = parseExpression(Precedence.LOWEST);
        if (expression != null) {
            arguments.add(expression);
        }
        while (peekToken.getTokenType() == TokenType.COMMA) {
            advanceTokens();
            advanceTokens();
            expression = parseExpression(Precedence.LOWEST);
            if (expression != null) {
                arguments.add(expression);
            }
        }
        if (!expectedToken(TokenType.RPAREN)) {
            return null;
        }
        return arguments;
    }

    private Expression parseExpression(Precedence precedence) {
        assert currentToken != null;
        try {
            PrefixParseFn prefixParseFn = prefixParseFns.get(currentToken.getTokenType());
            if (prefixParseFn == null) {
                String message = "Function to parse was not found for token type: " + currentToken.getTokenType();
                errors.add(message);
                return null;
            }
            Expression leftExpression = prefixParseFn.apply(null);
            assert peekToken != null;
            while (!peekToken.getTokenType().equals(TokenType.SEMICOLON) && precedence.compareTo(peekPrecedence()) < 0) {
                try {
                    InfixParseFn infixParseFn = infixParseFns.get(peekToken.getTokenType());
                    advanceTokens();
                    assert leftExpression != null;
                    leftExpression = infixParseFn.apply(leftExpression);
                } catch (Exception e) {
                    return leftExpression;
                }
            }
            return leftExpression;
        } catch (Exception e) {
            return null;
        }
    }

    private ExpressionStatement parseExpressionStatement() {
        assert currentToken != null;
        ExpressionStatement expressionStatement = new ExpressionStatement(currentToken, null);
        expressionStatement.setExpression(parseExpression(Precedence.LOWEST));
        assert peekToken != null;
        if (peekToken.getTokenType() == TokenType.SEMICOLON) {
            advanceTokens();
        }
        return expressionStatement;
    }
    
    private Expression parseGroupedExpression() {
        advanceTokens();
        Expression expression = parseExpression(Precedence.LOWEST);
        if (!expectedToken(TokenType.RPAREN)) {
            return null;
        }
        return expression;
    }

    private Function parseFunction() {
        assert currentToken != null;
        Function function = new Function(currentToken, new ArrayList<>(), null);
        if (!expectedToken(TokenType.LPAREN)) {
            return null;
        }
        function.setParameters(parseFunctionParameters());
        if (!expectedToken(TokenType.LBRACE)) {
            return null;
        }
        function.setBody(parseBlock());
        return function;
    }

    private List<Identifier> parseFunctionParameters() {
        List<Identifier> params = new ArrayList<>();
        assert peekToken != null;
        if (peekToken.getTokenType() == TokenType.RPAREN) {
            advanceTokens();
            return params;
        }
        advanceTokens();
        assert currentToken != null;
        Identifier identifier = new Identifier(currentToken);
        params.add(identifier);
        while (peekToken.getTokenType() == TokenType.COMMA) {
            advanceTokens();
            advanceTokens();
            assert currentToken != null;
            identifier = new Identifier(currentToken);
            params.add(identifier);
        }
        if (!expectedToken(TokenType.RPAREN)) {
            return Collections.emptyList();
        }
        return params;
    }

    private Identifier parseIdentifier() {
        assert currentToken != null;
        return new Identifier(currentToken);
    }
    
    private If parseIf() {
        assert currentToken != null;
        If ifExpression = new If(currentToken, null, null, null);
        if (!expectedToken(TokenType.LPAREN)) {
            return null;
        }
        advanceTokens();
        ifExpression.setCondition(parseExpression(Precedence.LOWEST));
        if (!expectedToken(TokenType.RPAREN)) {
            return null;
        }
        if (!expectedToken(TokenType.LBRACE)) {
            return null;
        }
        ifExpression.setConsequence(parseBlock());
        assert peekToken != null;
        if (peekToken.getTokenType() == TokenType.ELSE) {
            advanceTokens();
            if (!expectedToken(TokenType.LBRACE)) {
                return null;
            }
            ifExpression.setAlternative(parseBlock());
        }
        return ifExpression;
    }

    private Infix parseInfixExpression(Expression left) {
        assert currentToken != null;
        Infix infix = new Infix(currentToken, left, currentToken.getLiteral(), null);
        Precedence precedence = currentPrecedence();
        advanceTokens();
        infix.setRight(parseExpression(precedence));
        return infix;
    }
    
    private Integer parseInteger() {
        assert currentToken != null;
        try {
            return new Integer(currentToken, java.lang.Integer.parseInt(currentToken.getLiteral()));
            //integer.setValue(Integer(currentToken, currentToken.getLiteral()));
        } catch (NumberFormatException e) {
            String message = "Could not parse " + currentToken.getLiteral() + " as integer";
            errors.add(message);
            return null;
        }
    }
    
    private LetStatement parseLetStatement() {
        assert currentToken != null;
        LetStatement letStatement = new LetStatement(currentToken, null, null);
        if (!expectedToken(TokenType.IDENTIFIER)) {
            return null;
        }
        letStatement.setName(parseIdentifier());
        if (!expectedToken(TokenType.ASSIGN)) {
            return null;
        }
        advanceTokens();
        letStatement.setValue(parseExpression(Precedence.LOWEST));
        assert peekToken != null;
        if (peekToken.getTokenType() == TokenType.SEMICOLON) {
            advanceTokens();
        }
        return letStatement;
    }
    
    private Prefix parsePrefixExpression() {
        assert currentToken != null;
        Prefix prefix = new Prefix(currentToken, currentToken.getLiteral(), null);
        advanceTokens();
        prefix.setRight(parseExpression(Precedence.PREFIX));
        return prefix;
    }

    private ReturnStatement parseReturnStatement() {
        assert currentToken != null;
        ReturnStatement returnStatement = new ReturnStatement(currentToken, null);
        advanceTokens();
        returnStatement.setReturnValue(parseExpression(Precedence.LOWEST));
        assert peekToken != null;
        if (peekToken.getTokenType() == TokenType.SEMICOLON) {
            advanceTokens();
        }
        return returnStatement;
    }
    
    private Statement parseStatement() {
        assert currentToken != null;
        if (currentToken.getTokenType() == TokenType.LET) {
            return parseLetStatement();
        } else if (currentToken.getTokenType() == TokenType.RETURN) {
            return parseReturnStatement();
        } else {
            return parseExpressionStatement();
        }
    }

    private Precedence peekPrecedence() {
        assert peekToken != null;
        try {
            return precedences.get(peekToken.getTokenType());
        } catch (NullPointerException | IllegalArgumentException e) {
            return Precedence.LOWEST;
        }
    }

    private Map<TokenType, InfixParseFn> registerInfixParseFns() {
        Map<TokenType, InfixParseFn> infixFns = new EnumMap<>(TokenType.class);
        infixFns.put(TokenType.PLUS, this::parseInfixExpression);
        infixFns.put(TokenType.MINUS, this::parseInfixExpression);
        infixFns.put(TokenType.DIVISION, this::parseInfixExpression);
        infixFns.put(TokenType.MULTIPLICATION, this::parseInfixExpression);
        infixFns.put(TokenType.EQ, this::parseInfixExpression);
        infixFns.put(TokenType.DIF, this::parseInfixExpression);
        infixFns.put(TokenType.LT, this::parseInfixExpression);
        infixFns.put(TokenType.GT, this::parseInfixExpression);
        infixFns.put(TokenType.LPAREN, this::parseCall);
        return infixFns;
    }
    
    private Map<TokenType, PrefixParseFn> registerPrefixParseFns() {
        Map<TokenType, PrefixParseFn> prefixFns = new EnumMap<>(TokenType.class);
        prefixFns.put(TokenType.FALSE, token -> parseBoolean());
        prefixFns.put(TokenType.FUNCTION, token -> parseFunction());
        prefixFns.put(TokenType.IDENTIFIER, token -> parseIdentifier());
        prefixFns.put(TokenType.IF, token -> parseIf());
        prefixFns.put(TokenType.INTEGER, token -> parseInteger());
        prefixFns.put(TokenType.LPAREN, token -> parseGroupedExpression());
        prefixFns.put(TokenType.MINUS, token -> parsePrefixExpression());
        prefixFns.put(TokenType.NEGATION, token -> parsePrefixExpression());
        prefixFns.put(TokenType.TRUE, token -> parseBoolean());
        return prefixFns;
        }
}