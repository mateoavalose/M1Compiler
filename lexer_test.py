from unittest import TestCase
from typing import List
from M1files.tokens import(
    Token,
    TokenType
)
from M1files.lexer import(
    Lexer
)

class TestTokens(TestCase):
    def test_ilegal(self) -> None:
        source:str='¿@'
        lexer:Lexer=Lexer(source)
        tokens:List[Token]=[]
        for i in range(len(source)):
            tokens.append(lexer.next_token())
        expected_token:List[Token]=[
            Token(TokenType.ILLEGAL, '!'),
            Token(TokenType.ILLEGAL, '¿'),
            Token(TokenType.ILLEGAL, '@'),
        ]
        self.assertEqual(expected_token, tokens)

    def test_signs(self) -> None:
        source:str='+-=,;'
        lexer:Lexer=Lexer(source)
        tokens:List[Token]=[]
        for i in range(len(source)):
            tokens.append(lexer.next_token())
        expected_token:List[Token]=[
            Token(TokenType.PLUS, '+'),
            Token(TokenType.MINUS, '-'),
            Token(TokenType.ASSING, '='),
            Token(TokenType.COMMA, ','),
            Token(TokenType.SEMICOLON, ';'),
        ]
        self.assertEqual(expected_token, tokens) 