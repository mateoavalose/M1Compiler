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
            Parser parser = new Parser(lexer);
            Program program = parser.parseProgram();
            Object evaluated = Evaluator.evaluate(program);
            if(evaluated != null) 
                System.out.println(evaluated.inspect());
        }
    }
}
