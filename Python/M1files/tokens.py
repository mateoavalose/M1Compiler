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
    DIF = auto() #Different
    DIVISION = auto() # /
    ELSE = auto() # else
    EOF = auto() #Blank
    EQ = auto() #Equals
    FALSE = auto() # false
    FUNCTION = auto() #function
    IDENTIFIER = auto() #Letter
    IF = auto() #if
    GT = auto() #Greater than
    GTE = auto() #Greater than or equal
    ILLEGAL = auto() #Not identified
    INTEGER = auto() #Number
    LBRACE = auto() # {
    LET = auto() # variable
    LPAREN = auto() # (
    LT = auto() #Less than
    LTE = auto() #Less than or equal
    MINUS = auto() # -
    MULTIPLICATION = auto() # *
    NEGATION = auto() #!
    PLUS = auto() # +
    RBRACE = auto() # }
    RETURN = auto() # return
    RPAREN = auto() # )
    SEMICOLON = auto() # ;
    TRUE = auto() # true


class Token(NamedTuple):
    token_type:TokenType
    literal:str
    def __str__(self) -> str:
        return f'Type: {self.token_type}, Literal {self.literal}'

def lookup_token_type(literal:str)->TokenType:
    keywords:Dict[str, TokenType]={
        'function':TokenType.FUNCTION,
        'variable':TokenType.LET,
        'if':TokenType.IF,
        'else':TokenType.ELSE,
        'return':TokenType.RETURN,
        'true':TokenType.TRUE,
        'false':TokenType.FALSE
    }
    return keywords.get(literal, TokenType.IDENTIFIER)