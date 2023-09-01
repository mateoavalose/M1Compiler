from M1files.lexer import Lexer
from M1files.parser import Parser
from M1files.ast import Program
from M1files.tokens import(
    Token,
    TokenType
)

EOF_TOKEN:Token=Token(TokenType.EOF, '')
def start_repl()->None:
    while(source:=input('>> '))!='close()':
        lexer:Lexer=Lexer(source)
        parser:Parser=Parser(lexer)
        program:Program=parser.parse_program()
        
        print(program)