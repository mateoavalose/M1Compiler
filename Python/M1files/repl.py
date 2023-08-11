from M1files.lexer import Lexer
from M1files.tokens import(
    Token,
    TokenType
)

EOF_TOKEN:Token=Token(TokenType.EOF, '')
def star_repel()->None:
    while(source:=input('>> '))!='close()':
        lexer:Lexer=Lexer(source)

        while(token:=lexer.next_token())!=EOF_TOKEN:
            print(str(token))