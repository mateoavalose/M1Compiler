from typing import Optional

from M1files.ast import (
    Expression,
    Identifier,
    LesStatement,
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
    def parse_program(self)->Program:
        program:Program=Program(statements=[])
        return program