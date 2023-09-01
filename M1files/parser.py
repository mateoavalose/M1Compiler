from typing import (
    Optional,
    List,
    Dict,
    Callable
    )
from enum import IntEnum
from M1files.ast import (
    Block,
    Boolean,
    Call,
    Expression,
    ExpressionStatement,
    Function,
    Identifier,
    If,
    Infix,
    LetStatement,
    Prefix,
    Program,
    Statement,
    ReturnStatement
    )
from M1files.lexer import Lexer
from M1files.tokens import (
    Token,
    TokenType
)

PrefixParseFn=Callable[[],Optional[Expression]]
InfixParseFn=Callable[[Expression],Optional[Expression]]
PrefixParseFns:Dict[TokenType,PrefixParseFn]#={}
InfixParseFns:Dict[TokenType,InfixParseFn]#={}
class Precendece(IntEnum):
    LOWEST = 1
    EQUALS = 2
    LESSGREATER = 3
    SUM = 4
    PRODUCT = 5
    PREFIX = 6
    CALL = 7

PRECEDENTES:Dict[TokenType,Precendece]={
    TokenType.EQ:Precendece.EQUALS,
    TokenType.LT:Precendece.LESSGREATER,
    TokenType.GT:Precendece.LESSGREATER,
    TokenType.DIF:Precendece.EQUALS,
    TokenType.PLUS:Precendece.SUM,
    TokenType.MINUS:Precendece.SUM,
    TokenType.MULTIPLICATION:Precendece.PRODUCT,
    TokenType.DIVISION:Precendece.PRODUCT,
    TokenType.LBRACE:Precendece.CALL,

}

class Parser:
    def __init__(self, lexer:Lexer)->None:
        self._lexer = lexer
        self._current_token:Optional[Token] = None
        self._peek_token:Optional[Token] = None
        self._advance_tokens()
        self._advance_tokens()
    def parse_program(self)->Program:
        program:Program=Program(statements=[])
        assert self._current_token is not None
        while self._current_token.type != TokenType.EOF:
            statement = self._parse_statement()
            if statement is not None:
                program.statements.append(statement)
            self._advance_tokens()
        return program
    def _advance_tokens(self)->None:
        self._current_token=self._peek_token
        self._peek_token=self._lexer.next_token()
    def _expected_token(self, token_type:TokenType) -> bool:
        assert self._peek_token is not None
        if self._peek_token.type == token_type:
            self._advance_tokens()
            return True
        return False
    def _parse_let_statement(self)->Optional[LetStatement]:
        assert self._current_token is not None
        let_statement:LetStatement = LetStatement(token = self._current_token)
        if not self._expected_token(TokenType.IDENT):
            return None
        let_statement.name = Identifier(token = self._current_token, value = self._current_token.literal)
        if not self._expected_token(TokenType.ASSIGN):
            return None
        # TODO Falta evaluar la expresión
        while self._current_token.type != TokenType.SEMICOLON:
            self._advance_tokens() 
        
        return let_statement

    def _parse_statement(self)->Optional[Statement]:
        assert self._current_token is not None
        if self._current_token.type == TokenType.LET:
            return self._parse_let_statement()
        elif self._current_token.type == TokenType.RETURN:
            return self._parse_return_statement()
        else:
            return None
        
    def _parse_block(self)-> Block:
        assert self._current_token is not None
        block_statement:Block = Block(token = self._current_token, statements = [])
        self._advance_tokens()
        while not self._current_token.type == TokenType.RBRACE \
        and not self._current_token.type == TokenType.EOF:
            statement = self._parse_statement()
            if statement:
                block_statement.statements.append(statement)
            self._advance_tokens() 
        return block_statement
    def _parse_if(self) -> Optional[If]:
        assert self._current_token is not None
        if_expression:If = If(token = self._current_token)

        if not self._expected_token(TokenType.LPAREN):
            return None
        self._advance_tokens()

        if_expression.condition = self._parse_expression(Precendece.LOWEST)

        if not self._expected_token(TokenType.RPAREN):
            return None
        if not self._expected_token(TokenType.LBRACE):
            return None
        if_expression.consequence = self._parse_block()

        assert self._peek_token is not None
        if self._peek_token.type == TokenType.ELSE:
            self._advance_tokens()
            if not self._expected_token(TokenType.LBRACE):
                return None
            if_expression.alternative = self._parse_block()

        return if_expression
    
    def _parse_expression(self, precender:Precendece)->Optional[Expression]:
        assert self._current_token is not None
        try:
            prefix_parse_fn:self._prefix_parse_fns[self._current_token.type]
        except KeyError:
            message = f'No se encontró una función para parsear el token {self._current_token.type}'
            self._errors.append(message)
            return None