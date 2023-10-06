package M1files;

public class Repl {
    Token EndOfFile_TOKEN = new Token(TokenType.EOF, "");

    public void start_repl(){
        String source;
        while(true){
            System.out.print(">> ");
            source = System.console().readLine();
            if(source.equals("exit()")) break;

            Lexer lexer = new Lexer(source);
            Token token = lexer.nextToken();
            while(token.getTokenType() != TokenType.EOF){
                System.out.println(token);
                token = lexer.nextToken();
            }
        }
    }
}
