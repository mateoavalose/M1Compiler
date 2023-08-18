from typing import Optional

from M1files.ast import (
    Expression,
    Identifier,
    LetStatement,
    Program,
    Statement
)
from M1files.lexer import Lexer
from M1files.tokens import (
    Token,
    TokenType
)

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
        #Falta evaluar la expresión
        while self._current_token.type != TokenType.SEMICOLON:
            self._advance_tokens() 
        
        return let_statement

    def _parse_statement(self)->Optional[Statement]:
        assert self._current_token is not None
        if self._current_token.type == TokenType.LET:
            return self._parse_let_statement()
        else:
            return None