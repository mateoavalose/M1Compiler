from enum import (
    auto,
    Enum,
    unique
    )
from typing import (
    NamedTuple,
    Dict
)

@unique
class TokenType(Enum):
    ASSING = auto() # =
    COMMA = auto() # ,
    EOF = auto() #Blank
    FUNCTION = auto() #function
    IDENTIFIER = auto() #Letter
    IF = auto() #if
    ILLEGAL = auto() #Not identified
    INTEGER = auto() #Number
    LBRACE = auto() # {
    LET = auto() # variable
    LPAREN = auto() # (
    MINUS = auto() # -
    PLUS = auto() # +
    RBRACE = auto() # }
    RPAREN = auto() # )
    SEMICOLON = auto() # ;


class Token(NamedTuple):
    token_type:TokenType
    literal:str
    def __str__(self) -> str:
        return f'Type: {self.token_type}, Literal {self.literal}'

def lookup_token_type(literal:str)->TokenType:
    keywords:Dict[str, TokenType]={
        'function':TokenType.FUNCTION,
        'variable':TokenType.LET,
        'if':TokenType.IF
    }
    return keywords.get(literal, TokenType.IDENTIFIER)