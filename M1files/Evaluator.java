package M1files;

import java.util.List;

//import java.util.List;
//import java.util.Optional;

public class Evaluator {
    public static final ObjBoolean TRUE = new ObjBoolean(true);
    public static final ObjBoolean FALSE = new ObjBoolean(false);
    public static final Null NULL = new Null();

    public static Object evaluateBangOperatorExpression(Object right) {
        if (right == TRUE) {
            return FALSE;
        } else if (right == FALSE) {
            return TRUE;
        } else if (right == NULL) {
            return TRUE;
        } else {
            return FALSE;
        }
    }

    public static ObjBoolean toBooleanObject(boolean value) {
        return value ? TRUE : FALSE;
    }

    public static Object evaluateMinusOperatorExpression(Object right) {
        if (!(right instanceof ObjInteger)) {
            return NULL;
        }
        ObjInteger integer = (ObjInteger) right;
        return new ObjInteger(-integer.getValue());
    }

    public static Object evaluatePrefixExpression(String operator, Object right) {
        if ("!".equals(operator)) {
            return evaluateBangOperatorExpression(right);
        } else if ("-".equals(operator)) {
            return evaluateMinusOperatorExpression(right);
        } else {
            return NULL;
        }
    }

    public static Object evaluateIntegerInfixExpression(String operator, Object left, Object right) {
        if (!(left instanceof ObjInteger) || !(right instanceof ObjInteger)) {
            return NULL;
        }
        ObjInteger leftInteger = (ObjInteger) left;
        ObjInteger rightInteger = (ObjInteger) right;

        int leftValue = leftInteger.getValue();
        int rightValue = rightInteger.getValue();

        switch (operator) {
            case "+":
                return new ObjInteger(leftValue + rightValue);
            case "-":
                return new ObjInteger(leftValue - rightValue);
            case "*":
                return new ObjInteger(leftValue * rightValue);
            case "/":
                if (rightValue == 0) {
                    return NULL;
                }
                return new ObjInteger(leftValue / rightValue);
            case "<":
                return toBooleanObject(leftValue < rightValue);
            case "<=":
                return toBooleanObject(leftValue <= rightValue);
            case ">":
                return toBooleanObject(leftValue > rightValue);
            case ">=":
                return toBooleanObject(leftValue >= rightValue);
            case "==":
                return toBooleanObject(leftValue == rightValue);
            case "!=":
                return toBooleanObject(leftValue != rightValue);
            default:
                return NULL;
        }
    }

    public static Object evaluateInfixExpression(String operator, Object left, Object right) {
        if (left.type() == ObjectType.INTEGER && right.type() == ObjectType.INTEGER) {
            return evaluateIntegerInfixExpression(operator, left, right);
        } else if (operator.equals("==")) {
            return toBooleanObject(left.equals(right));
        } else if (operator.equals("!=")) {
            return toBooleanObject(!left.equals(right));
        }
        return NULL;
    }

    public static Object evaluate(ASTNode node) {
        if (node instanceof Program) {   
            return evaluateStatements(((Program) node).getStatements());
        } else if (node instanceof ExpressionStatement) {
            Expression expression = ((ExpressionStatement) node).getExpression();
            return expression != null ? evaluate(expression) : null;
        } else if (node instanceof Integer) {
            Integer integerNode = (Integer) node;
            return new ObjInteger(integerNode.getValue());
        } else if (node instanceof Boolean) {
            return toBooleanObject(((Boolean) node).getValue());
        } else if (node instanceof Prefix) {
            Prefix prefixNode = (Prefix) node;
            Object right = evaluate(prefixNode.getRight());
            return right != null ? evaluatePrefixExpression(prefixNode.getOperator(), right) : null;
        } else if (node instanceof Infix) {
            Infix infixNode = (Infix) node;
            Object left = evaluate(infixNode.getLeft());
            Object right = evaluate(infixNode.getRight());
            return (left != null && right != null) ? evaluateInfixExpression(infixNode.getOperator(), left, right) : null;
        } else if (node instanceof Block) {
            return evaluateStatements(((Block) node).getStatements());
        } else if (node instanceof If) {
            return evaluateIfExpression((If) node);
        }
        return null;
    }

    public static Object evaluateStatements(List<Statement> statements) {
        Object result = null;
        for (Statement statement : statements) {
            result = evaluate(statement); 
        }
        return result;
    }

    public static boolean isTruthy(Object obj) {
        if (obj == NULL) {
            return false;
        } else if (obj == TRUE) {
            return true;
        } else if (obj == FALSE) {
            return false;
        }
        return true;
    }

    public static Object evaluateIfExpression(If ifExpression) {
        if (ifExpression.getCondition() != null) {
            Object condition = evaluate(ifExpression.getCondition());
            if (condition != null) {
                if (isTruthy(condition)) {
                    if (ifExpression.getConsequence() != null) {
                        return evaluate(ifExpression.getConsequence());
                    }
                } else if (ifExpression.getAlternative() != null) {
                    return evaluate(ifExpression.getAlternative());
                }
            }
        }
        return NULL;
    }
}