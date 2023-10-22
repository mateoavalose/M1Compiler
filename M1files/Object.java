package M1files;

enum ObjectType {
    BOOLEAN,
    INTEGER,
    NULL,
    FLOAT,
    STRING
}

public abstract class Object {
    public abstract ObjectType type();
    public abstract String inspect();
}

class ObjInteger extends Object {
    private int value;
    public ObjInteger(int value){
        this.value = value;
    }
    @Override
    public ObjectType type(){
        return ObjectType.INTEGER;
    }
    @Override
    public String inspect(){
        return String.valueOf(value);
    }
    public int getValue() {
        return value;
    }
}

class ObjBoolean extends Object {
    private boolean value;
    public ObjBoolean(boolean value){
        this.value = value;
    }
    @Override
    public ObjectType type(){
        return ObjectType.BOOLEAN;
    }
    @Override
    public String inspect(){
        return String.valueOf(value);
    }
}

class Null extends Object {
    @Override
    public ObjectType type(){
        return ObjectType.NULL;
    }
    @Override
    public String inspect(){
        return "null";
    }
}

class Float extends Object {
    private float value;
    public Float(float value){
        this.value = value;
    }
    @Override
    public ObjectType type(){
        return ObjectType.FLOAT;
    }
    @Override
    public String inspect(){
        return String.valueOf(value);
    }
}

class StringObject extends Object {
    private String value;
    public StringObject(String value){
        this.value = value;
    }
    @Override
    public ObjectType type(){
        return ObjectType.STRING;
    }
    @Override
    public String inspect(){
        return value;
    }
}