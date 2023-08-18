from unittest import TestCase
 
from M1files.ast import(
    Expression,
    Identifier,
    LesStatement,
    Program,
    Statement
)

from M1files.ast import Program
from M1files.lexer import Lexer
from M1files.parser import Parser
 
class ParserTest(TestCase):

    def test_parse_program(self) -> None:
        source: str = 'variable x =5;'
        lexer: Lexer = Lexer(source)
        parser: Parser = Parser(lexer)
        program: Program = parser.parse_program()
        self.assertIsNotNone(program)
        self.assertIsInstance(program, Program)

    def test_let_statements(self) -> None:
        source:str='''
            variable x = 5;
            variable y = 10;
            variable foo = 20;
        '''

        lexer:Lexer=Lexer(source)
        parser:Parser=Parser(lexer)
        program:Program=parser.parse_program()
        self.assertEqual(len(program.statements), 3)