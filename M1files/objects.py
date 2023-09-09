from abc import(
    ABC,
    abstractmethod
)
from enum import(
    auto,
    Enum
)

class ObjectType(Enum):
    BOOLEAN = auto()
    INTEGER = auto()
    NULL = auto()
    FLOAT = auto()
    STRING = auto()
    
class Object(ABC):
    @abstractmethod
    def type(self) -> ObjectType:
        pass
    
    @abstractmethod
    def inspect(self) -> str:
        pass 

class Integer(Object):
    def __init__(self, value:int) -> None:
        self.value:int = value
    
    def type(self) -> ObjectType:
        return ObjectType.INTEGER

    def inspect(self) -> str:
        return str(self.value)
    
class Boolean(Object):
    def __init__(self, value:bool) -> None:
        self.value:bool = value
    
    def type(self) -> ObjectType:
        return ObjectType.BOOLEAN

    def inspect(self) -> str:
        return str(self.value)
    
class Null(Object):
    def type(self) -> ObjectType:
        return ObjectType.NULL

    def inspect(self) -> str:
        return "null"
    
class Float(Object):
    def __init__(self, value:float) -> None:
        self.value:float = value
    
    def type(self) -> ObjectType:
        return ObjectType.FLOAT

    def inspect(self) -> str:
        return str(self.value)

class String(Object):
    def __init__(self, value:str) -> None:
        self.value:str = value
    
    def type(self) -> ObjectType:
        return ObjectType.STRING

    def inspect(self) -> str:
        return str(self.value)