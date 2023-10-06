import M1files.Repl;

public class Main {
    public static void main(String[] args) {
        System.out.println("Welcome to the M1 Programming Language (Java). Type 'exit()' to exit.");
        Repl repl = new Repl();
        repl.start_repl();
    }
} 