package M1files;

import java.util.List;

public interface ASTNode {
    String tokenLiteral();
    String toString();
}

class Statement implements ASTNode {
    private Token token;
    public Statement(Token token){
        this.token = token;
    }
    @Override
    public String tokenLiteral(){
        return token.getLiteral();
    }
}

class Expression implements ASTNode {
    private Token token;
    public Expression(Token token){
        this.token = token;
    }
    @Override
    public String tokenLiteral(){
        return token.getLiteral();
    }
}

class Program implements ASTNode {
    private List<Statement> statements;
    public Program(List<Statement> statements){
        this.statements = statements;
    }
    @Override
    public String tokenLiteral(){
        if(!statements.isEmpty()){
            return statements.get(0).tokenLiteral();
        }
        return "";
    }
    @Override
    public String toString(){
        StringBuilder sb = new StringBuilder();
        for(Statement s : statements){
            sb.append(s.toString());
        }
        return sb.toString();
    }
    public void addStatement(Statement statement) {
        statements.add(statement);
    }
    public List<Statement> getStatements() {
        return statements;
    }
}

class Identifier extends Expression {
    private String value;
    public Identifier(Token token){
        super(token);
        this.value = token.getLiteral();
    }
    @Override
    public String toString(){
        return value;
    }
}

class LetStatement extends Statement {
    private Identifier name;
    private Expression value;
    public LetStatement(Token token, Identifier name, Expression value){
        super(token);
        this.name = name;
        this.value = value;
    }
    @Override
    public String toString(){
        String nameStr = (name != null)? name.toString() : "";
        String valueStr = (value != null)? value.toString() : "";
        return tokenLiteral() + " " + nameStr + " = " + valueStr + ";";
    }
    void setName(Identifier name) {
        this.name = name;
    }
    Identifier getName() {
        return name;
    }
    void setValue(Expression value) {
        this.value = value;
    }
    Expression getValue() {
        return value;
    }
}

class ReturnStatement extends Statement {
    private Expression returnValue;
    public ReturnStatement(Token token, Expression returnValue){
        super(token);
        this.returnValue = returnValue;
    }
    @Override
    public String toString(){
        String returnValueStr = (returnValue != null)? returnValue.toString() : "";
        return tokenLiteral() + " " + returnValueStr + ";";
    }
    void setReturnValue(Expression returnValue) {
        this.returnValue = returnValue;
    }
    Expression getReturnValue() {
        return returnValue;
    }
}

class Integer extends Expression {
    private int value;
    public Integer(Token token, int value){
        super(token);
        this.value = value;
    }
    @Override
    public String toString(){
        return String.valueOf(value);
    }
    void setValue(int value) {
        this.value = value;
    }
    int getValue() {
        return value;
    }
}

class Boolean extends Expression {
    private boolean value;
    public Boolean(Token token, boolean value){
        super(token);
        this.value = value;
    }
    @Override
    public String toString(){
        return String.valueOf(value);
    }
    void setValue(boolean value) {
        this.value = value;
    }
    boolean getValue() {
        return value;
    }
}

class Block extends Statement {
    private List<Statement> statements;
    public Block(Token token, List<Statement> statements){
        super(token);
        this.statements = statements;
    }
    @Override
    public String toString(){
        StringBuilder sb = new StringBuilder();
        for(Statement s : statements){
            sb.append(s.toString());
        }
        return sb.toString();
    }
    void addStatement(Statement statement) {
        statements.add(statement);
    }
    List<Statement> getStatements() {
        return statements;
    }
}

class Function extends Expression {
    private List<Identifier> parameters;
    private Block body;
    public Function(Token token, List<Identifier> parameters, Block body){
        super(token);
        this.parameters = parameters;
        this.body = body;
    }
    @Override
    public String toString(){
        String paramList = String.join(", ", parameters.stream().map(Identifier::toString).toArray(String[]::new));
        return tokenLiteral() + "(" + paramList + ") " + body.toString();
    }
    void setParameters(List<Identifier> parameters) {
        this.parameters = parameters;
    }
    List<Identifier> getParameters() {
        return parameters;
    }
    void setBody(Block body) {
        this.body = body;
    }
    Block getBody() {
        return body;
    }
}

class If extends Expression {
    private Expression condition;
    private Block consequence;
    private Block alternative;
    public If(Token token, Expression condition, Block consequence, Block alternative){
        super(token);
        this.condition = condition;
        this.consequence = consequence;
        this.alternative = alternative;
    }
    @Override
    public String toString(){
        String out = " if " + condition.toString() + " " + consequence.toString();
        if(alternative != null){
            out += " else " + alternative.toString();
        }
        return out;
    }
    void setCondition(Expression condition) {
        this.condition = condition;
    }
    Expression getCondition() {
        return condition;
    }
    void setConsequence(Block consequence) {
        this.consequence = consequence;
    }
    Block getConsequence() {
        return consequence;
    }
    void setAlternative(Block alternative) {
        this.alternative = alternative;
    }
    Block getAlternative() {
        return alternative;
    }
}

class Infix extends Expression {
    private Expression left;
    private String operator;
    private Expression right;
    public Infix(Token token, Expression left, String operator, Expression right){
        super(token);
        this.left = left;
        this.operator = operator;
        this.right = right;
    }
    @Override
    public String toString(){
        return "(" + left.toString() + " " + operator + " " + right.toString() + ")";
    }
    void setLeft(Expression left) {
        this.left = left;
    }
    Expression getLeft() {
        return left;
    }
    void setRight(Expression right) {
        this.right = right;
    }
    Expression getRight() {
        return right;
    }
    void setOperator(String operator){
        this.operator = operator;
    }
    String getOperator(){
        return operator;
    }
}

class Prefix extends Expression {
    private String operator;
    private Expression right;
    public Prefix(Token token, String operator, Expression right){
        super(token);
        this.operator = operator;
        this.right = right;
    }
    @Override
    public String toString(){
        return "(" + operator + " " + right.toString() + ")";
    }
    void setRight(Expression right) {
        this.right = right;
    }
    Expression getRight() {
        return right;
    }
    void setOperator(String operator){
        this.operator = operator;
    }
    String getOperator(){
        return operator;
    }
}

class Call extends Expression {
    private Expression function;
    private List<Expression> arguments;
    public Call(Token token, Expression function, List<Expression> arguments){
        super(token);
        this.function = function;
        this.arguments = arguments;
    }
    @Override
    public String toString(){
        assert arguments != null;
        String argList = String.join(", ", arguments.stream().map(Expression::toString).toArray(String[]::new));
        return function.toString() + "(" + argList + ")";
    }
    void setArguments(List<Expression> arguments) {
        this.arguments = arguments;
    }
    List<Expression> getArguments() {
        return arguments;
    }
}

class ExpressionStatement  extends Statement {
    private Expression expression;
    public ExpressionStatement(Token token, Expression expression){
        super(token);
        this.expression = expression;
    }
    @Override
    public String toString(){
        return expression.toString();
    }
    void setExpression(Expression expression) {
        this.expression = expression;
    }
    Expression getExpression() {
        return expression;
    }
}